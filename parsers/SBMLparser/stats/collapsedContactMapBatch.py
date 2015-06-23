import sys
import os
sys.path.insert(0, '.')
sys.path.insert(0, os.path.join('.','SBMLparser'))
import argparse
import progressbar
import fnmatch
from rulifier import componentGroups
import json
import cPickle as pickle
from collections import defaultdict
def getFiles(directory,extension):
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
            matches.append([filepath,os.path.getsize(os.path.join(root, filename))])

    #sort by size
    #matches.sort(key=lambda filename: filename[1], reverse=False)
    
    matches = [x[0] for x in matches]

    return matches


def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i','--input',type=str,help='input directory',required=True)
    parser.add_argument('-o','--output',type=str,help='output directory',required=True)
    return parser    


def default_to_regular(d):
    if isinstance(d, defaultdict):
        d = {k: default_to_regular(v) for k, v in d.iteritems()}
    return d

def createContextInformation(inputDirectory,outputDirectory):
    bngxml = getFiles(inputDirectory,'xml')
    progress = progressbar.ProgressBar()
    for idx in progress(range(len(bngxml))):
        fileName = bngxml[idx]
        #molecules,rules,_ = parseXML(fileName)
        extendedInformation, backup = componentGroups.getContextRequirements(fileName,False)   
        with open(os.path.join(outputDirectory, '{0}.dump'.format(fileName.split('/')[-1])),'w') as f:
            pickle.dump(default_to_regular(extendedInformation), f)
            pickle.dump(default_to_regular(backup), f)
        #collapsedContactMap.createCollapsedContact(rules,molecules,[1],fileName + '.gml',extendedInformation    )         



if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    inputDirectory = namespace.input

    createContextInformation(inputDirectory,namespace.output)

