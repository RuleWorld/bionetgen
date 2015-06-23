import os
import fnmatch
import argparse

import itertools
from collections import defaultdict
import cPickle as pickle
import sys
sys.path.insert(0, '.')
sys.path.insert(0, os.path.join('.', 'SBMLparser'))
from rulifier import compareModels
import functools
import marshal
import progressbar


def memoize(obj):
    cache = obj.cache = {}
    @functools.wraps(obj)
    def memoizer(*args, **kwargs):
        key = marshal.dumps([args, kwargs])
        if key not in cache:
            cache[key] = obj(*args, **kwargs)
        return cache[key]
    return memoizer

@memoize
def getContext(fileName, auxDirectory):
    auxFile = fileName.split('/')[-1]
    with open(os.path.join(auxDirectory, '{0}.dump'.format(auxFile))) as f:
        context = pickle.load(f)
        stateDict = pickle.load(f)

    return [context, stateDict]

def getFiles(directory, extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'

    Keyword arguments:
    directory -- The directory we will recurseviley get files from
    extension -- A file extension filter
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            filepath = os.path.abspath(os.path.join(root, filename))
            matches.append([filepath, os.path.getsize(os.path.join(root, filename))])

    #sort by size
    #matches.sort(key=lambda filename: filename[1], reverse=False)

    matches = [x[0] for x in matches]

    return matches

def defineConsole():
    parser = argparse.ArgumentParser(description='Model comparison batch process')
    parser.add_argument('-i', '--input', type=str, help='input directory', required=True)
    parser.add_argument('-a', '--aux', type=str,required=True)
    return parser




def main():
    parser = defineConsole()
    namespace = parser.parse_args()
    inputDirectory = namespace.input
    bngxml = getFiles(inputDirectory, 'xml')
    combinationArray = list(itertools.combinations(bngxml, 2))
    modelPairs = []
    progress = progressbar.ProgressBar()

    for combinationIdx in progress(range(0, len(combinationArray))):
        combination = combinationArray[combinationIdx]
        context1 = getContext(combination[0], namespace.aux)
        context2 = getContext(combination[1], namespace.aux)
        if len(context1[0]) == 0 or len(context2[0]) == 0:
            continue
        score = compareModels.compareModelsContext(combination[0], combination[1], 'comparisonInfo', context1, context2)
        if len(score) > 0:
            print combination
            modelPairs.append([combination, len(score), len(context1), len(context2)])
        with open('comparisonMatrix.dump', 'wb') as f:
            pickle.dump(modelPairs, f)


if __name__ == "__main__":
    main()