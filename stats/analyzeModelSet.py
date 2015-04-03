import concurrent.futures

import fnmatch
import os
from subprocess import call        
import multiprocessing as mp
import progressbar
import sys
import glob
import shutil
import yaml
from os.path import expanduser,join

sys.path.insert(0, '.')
sys.path.insert(0, os.path.join('.','SBMLparser'))
import argparse
home = expanduser("~")
bngExecutable = join(home,'workspace','bionetgen','bng2','BNG2.pl')
visualizeExecutable = join(home,'workspace','bionetgen','bng2','Perl2','Visualization','visualize.pl')




def getFiles(directory,extension):
    """
    Gets a list of <*.extension> files. include subdirectories and return the absolute 
    path. also sorts by size.
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append([os.path.join(root, filename),os.path.getsize(os.path.join(root, filename))])

    #sort by size
    matches.sort(key=lambda filename: filename[1], reverse=False)
    
    matches = [x[0] for x in matches]

    return matches

def callSBMLTranslator(fileName,outputdirectory,options=[]):
    """
    Translate an  SBML file into BNGL

    ----
    Keyword arguments:
    fileName -- The SBML xml file to be translated
    outputdirectory -- The directory where the resulting bngl will be placed
    """
    with open(os.devnull,"w") as f:
        result = call(['python','SBMLparser/sbmlTranslator.py','-i',
        #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
        fileName,
        '-o',os.path.join(outputdirectory, str(fileName.split('/')[-1])) + '.bngl',
        '-c','config/reactionDefinitions.json',
        '-n','config/namingConventions.json'
        ],stdout=f)
    return result

def convertXML(bnglfile,output,options=[]):
    """
    Convert a BNGL file into bng-xml

    ----
    Keyword arguments:
    bnglfile -- The BNGL  file to be translated
    outputdirectory -- The directory where the resulting bngl will be placed

    """
    with open(os.devnull,"w") as f:
        retval = os.getcwd()
        os.chdir(output)
        result = call([bngExecutable,'--xml',bnglfile],stdout=f)
        os.chdir(retval)

def generateGraph(bnglfile,outputdirectory,options):
    """
    Obtain a graph representation of a given BNGL file
    
    ----
    Keyword arguments:
    bnglfile -- The BNGL  file to be translated
    outputdirectory -- The directory where the resulting bngl will be placed

    """

    command = [visualizeExecutable,'--bngl',bnglfile]
    command.extend(options)
    with open(os.devnull,"w") as f:
        retval = os.getcwd()
        os.chdir(outputdirectory)
        result = call(command,stdout=f)
        os.chdir(retval)

    graphname = '.'.join(bnglfile.split('.')[:-1]) + '_regulatory.gml'
    graphname = graphname.split('/')[-1]
    return graphname


def generateBNGXML(bnglFiles,output,format='BNGXML'):
    """

    """
    print 'converting {0} bnglfiles'.format(len(bnglFiles))
    parallelHandling(bnglFiles,convertXML,output)


def parallelHandling(files,function,outputDir,options = []):
    futures = []
    workers = mp.cpu_count()-1
    progress = progressbar.ProgressBar(maxval= len(files)).start()
    i = 0
    print 'running in {0} cores'.format(workers)
    with concurrent.futures.ProcessPoolExecutor(max_workers=workers) as executor:
        for fileidx in progress(range(len(files))):
            futures.append(executor.submit(function, files[fileidx],outputDir,options))
        for future in concurrent.futures.as_completed(futures,timeout=3600):
            i+=1
            progress.update(fileidx)
    progress.finish()


def generateGraphs(bnglFiles,output,options=[]):
    """
    Calls visualize.pl to generate GML graphs from a BNG representation
    of a model
    """
    print 'converting {0} bnglfiles to GML'.format(len(bnglFiles))
    parallelHandling(bnglFiles,generateGraph,output,options)


def translate(filenameset,outputdirectory):
    print 'translating {0} bnglfiles to BNGL'.format(len(filenameset))
    parallelHandling(filenameset,callSBMLTranslator,outputdirectory)


def loadFilesFromYAML(yamlFile):
    with open(yamlFile,'r') as f:
        yamlsettings = yaml.load(f)

    print yamlsettings
    return yamlsettings


def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-s','--settings',type=str,help='settings file')
    parser.add_argument('-o','--output',type=str,help='output directory')
    parser.add_argument('-t','--type',type=str,help='atomize or convert to bng-xml')
    return parser    


if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    if namespace.settings != None:
        settings = loadFilesFromYAML(namespace.settings)
        filenameset = settings['inputfiles']
        outputdirectory = namespace.output
        ttype = namespace.type
        if 'options' in settings:
            options = settings['options']
        else:
            options = []
    else:
        filenameset = getFiles('curated','bngl')
        outputdirectory = 'curated'
        ttype = 'graph'
        options = []

    if ttype == 'atomize':
        translate(filenameset,outputdirectory)    
    elif ttype == 'bngxml':
        generateBNGXML(filenameset,output= outputdirectory)
    elif ttype == 'graph':
        generateGraphs(filenameset,output=outputdirectory,options = options)
    else:
        raise Exception('Invalid output type')
    #with open('new_non_curated/failure.dump','rb') as f:
    #    s = pickle.load(f)
    #filenameset = getFiles('complex2','bngl')
    #generateBNGXML(filenameset,output= outputdirectory)
