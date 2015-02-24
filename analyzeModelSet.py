import concurrent.futures

import fnmatch
import os
from subprocess import call        


def callSBML(filename):
    pass


def getFiles(directory,extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append(os.path.join(root, filename))
    return matches

def callSBMLTranslator(fileName):
    with open(os.devnull,"w") as f:
        result = call(['python','sbmlTranslator.py','-i',
        #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
        fileName,
        '-o','new_non_curated/' + str(fileName.split('/')[-1]) + '.bngl',
        '-c','config/reactionDefinitions.json',
        '-n','config/namingConventions.json',
        '-a'],stdout=f)
    return result


if __name__ == "__main__":
    directory = 'XMLExamples/non_curated'
    filenameset = getFiles(directory,'xml')
    futures = []
    with concurrent.futures.ProcessPoolExecutor(max_workers=64) as executor:
        for filename in filenameset:
          futures.append(executor.submit(callSBMLTranslator, filename))
        for future in concurrent.futures.as_completed(futures,timeout=3000):
            pass
