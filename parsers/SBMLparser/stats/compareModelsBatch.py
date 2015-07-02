import os
import fnmatch
import argparse
import concurrent.futures
from readBNGXML import parseXML
from copy import deepcopy
import itertools
from collections import defaultdict
import cPickle as pickle
import sys
sys.path.insert(0, '.')
sys.path.insert(0, os.path.join('.', 'SBMLparser'))
from rulifier import compareModels,componentGroups
import functools
import marshal
import progressbar
import multiprocessing as mp
import collapsedContactMap

import networkx as nx

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
def getContext(fileName, auxDirectory=None):
    if auxDirectory:
        auxFile = fileName.split('/')[-1]
        with open(os.path.join(auxDirectory, '{0}.dump'.format(auxFile))) as f:
            context = pickle.load(f)
            stateDict = pickle.load(f)
    else:
        context, stateDict = componentGroups.getContextRequirements(fileName)
    return [context, stateDict]

def default_to_regular(d): 
    if isinstance(d, compareModels.prettyDict):
        d = {k: default_to_regular(v) for k, v in d.iteritems()}
    return d


def getContextComparison(combination1, combination2, context1,context2):
    return compareModels.compareModelsContext(combination1, combination2,'dummy', context1, context2)

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
    parser.add_argument('-a', '--aux', type=str)
    return parser




def main():
    parser = defineConsole()
    namespace = parser.parse_args()
    inputDirectory = namespace.input
    bngxml = getFiles(inputDirectory, 'xml')
    combinationArray = list(itertools.combinations(bngxml, 2))
    modelPairs = {}
    futures = []    
    progress = progressbar.ProgressBar()
    workers = 3
    #with concurrent.futures.ProcessPoolExecutor(max_workers=workers) as executor:
    print combinationArray
    for combinationIdx in progress(range(0, len(combinationArray))):
        combination = combinationArray[combinationIdx]
        context1 = getContext(combination[0], namespace.aux)
        context2 = getContext(combination[1], namespace.aux)

        if len(context1[0]) == 0 or len(context2[0]) == 0:
            continue
        #futures.append(executor.submit(getContextComparison, combination[0],combination[1],context1,context2))

        score = getContextComparison(combination[0], combination[1], context1, context2)
        print score[0:-1]
        if len(score[-1]) > 0:
            modelPairs[score[0:2]] = score[-1]


        '''
        for future in concurrent.futures.as_completed(futures, timeout=3600):
            score = future.result()
            if len(score[-1]) > 0 and score[0:2] not in modelPairs:
                print score[0:2]
                modelPairs[score[0:2]] = score[-1]
        '''
    with open('comparisonMatrix.dump', 'wb') as f:
        pickle.dump(modelPairs.__repr__(), f)


@memoize
def getGraph(fileName):
    molecules, rules, _ = parseXML(fileName)
    extendedInformation = getContext(fileName)
    return collapsedContactMap.createCollapsedContact(rules, molecules, [1], "", extendedInformation[0],
                                                    True, True, False)


def filterGraph(superGraph,graph,prefixString,filterStrings,bgColor):
    nodeId = []
    for nodeName in graph.node:
        if any([nodeName.startswith(x+"_") or nodeName == x for x in filterStrings.keys()]):
            nodeCopy = deepcopy(graph.node[nodeName])
            if nodeName in filterStrings.keys():
                nodeCopy['graphics']['fill'] = bgColor
                nodeCopy['LabelGraphics']['anchor'] = "t"
            superGraph.add_node('{0}_{1}'.format(prefixString,nodeName),nodeCopy)
            nodeId.append(nodeName)

    for edge in graph.edges_iter(nodeId,data=True):
        edgeCopy = deepcopy(edge[2])
        
        if edge[0].split('_')[0] in filterStrings and (set([x.split('_')[1].lower() for x in edge[0:2]]) in [set(x[0]) for x in filterStrings[edge[0].split('_')[0]]]):
            edgeCopy['graphics']['width'] = 3
        superGraph.add_edge('{0}_{1}'.format(prefixString,edge[0]),'{0}_{1}'.format(prefixString,edge[1]),edgeCopy)

def analyzePairs():
    with open('comparisonMatrix.dump', 'r') as f:
        comparisonPairs = pickle.load(f)

    comparisonPairs = eval(comparisonPairs)
    for pair in comparisonPairs:
            print pair[0]

            graph, _ = getGraph(pair[0])
            print pair[1]
            graph2, _ = getGraph(pair[1])
            jointGraph = nx.DiGraph()
            print comparisonPairs[pair].keys()
            filterGraph(jointGraph, graph, pair[0].split('/')[-1].split('.')[0], comparisonPairs[pair],"#FFCCFF")
            filterGraph(jointGraph, graph2, pair[1].split('/')[-1].split('.')[0], comparisonPairs[pair],"#CCFFFF")
            fileName = 'differences/{0}_{1}.gml'.format(pair[0].split('.')[0].split('/')[-1],pair[1].split('.')[0].split('/')[-1])
            nx.write_gml(jointGraph, fileName)
            print '---'



if __name__ == "__main__":
    main()
    analyzePairs()
    