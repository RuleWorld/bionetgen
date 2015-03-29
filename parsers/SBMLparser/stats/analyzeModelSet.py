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

def callSBML(filename):
    pass


def getFiles(directory,extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append([os.path.join(root, filename),os.path.getsize(os.path.join(root, filename))])

    #sort by size
    matches.sort(key=lambda filename: filename[1], reverse=False)
    
    matches = [x[0] for x in matches]

    return matches

def callSBMLTranslator(fileName,outputdirectory):
    with open(os.devnull,"w") as f:
        result = call(['python','SBMLparser/sbmlTranslator.py','-i',
        #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
        fileName,
        '-o',os.path.join(outputdirectory, str(fileName.split('/')[-1])) + '.bngl',
        '-c','config/reactionDefinitions.json',
        '-n','config/namingConventions.json',
        '-a'],stdout=f)
    return result

def convertXML(bnglfile,output):
    with open(os.devnull,"w") as f:
        retval = os.getcwd()
        os.chdir(output)
        result = call([bngExecutable,'--xml',bnglfile],stdout=f)
        os.chdir(retval)

def generateBNGXML(bnglFiles,output,format='BNGXML'):
    
    print 'converting {0} bnglfiles'.format(len(bnglFiles))
    futures = []
    workers = mp.cpu_count()-1
    progress = progressbar.ProgressBar(maxval= len(bnglFiles)).start()
    i = 0
    print 'running in {0} cores'.format(workers)
    with concurrent.futures.ProcessPoolExecutor(max_workers=workers) as executor:

        for bngl in progress(range(len(bnglFiles))):
            convertXML(bnglFiles[bngl])
            futures.append(executor.submit(convertXML, bnglFiles[bngl],output))
        for future in concurrent.futures.as_completed(futures,timeout=3600):
            i+=1
            progress.update(bngl)
    progress.finish()

    '''
    print 'moving xml files'
    files = glob.iglob(os.path.join('.', "*.xml"))
    for xmlfile in files:
        if os.path.isfile(xmlfile):
            shutil.move(xmlfile, directory)
    '''



def translate(filenameset,outputdirectory):
    restart = True

    if not restart:
        existingset = getFiles(outputdirectory,'bngl')
        existingset = ['.'.join(x.split('.')[:-1]).split('/')[-1] for x in existingset]
        filenameset = [x for x in filenameset if x.split('/')[-1] not in existingset]

    #with open('failure2.dump','rb') as f:
    #    filenameset2 = pickle.load(f)
    #filenameset = [x for x in filenameset if x in filenameset2]
    print(len(filenameset))
    
    futures = []
    workers = mp.cpu_count()-1
    progress = progressbar.ProgressBar(maxval= len(filenameset)).start()
    i = 0
    print 'running in {0} cores'.format(workers)
    with concurrent.futures.ProcessPoolExecutor(max_workers=workers) as executor:
        for filename in filenameset:
          futures.append(executor.submit(callSBMLTranslator, filename,outputdirectory))
        for future in concurrent.futures.as_completed(futures,timeout=3000):
            i+=1
            progress.update(i)
    progress.finish()
    
    '''
    for element in filenameset:
        sys.stderr.write(element + '\n')
        callSBMLTranslator(element,outputdirectory)
    '''


def loadFilesFromYAML(yamlFile):
    with open(yamlFile,'r') as f:
        yamlsettings = yaml.load(f)

    print yamlsettings
    return yamlsettings['inputfiles']


def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-s','--settings',type=str,help='settings file')
    parser.add_argument('-o','--output',type=str,help='output directory')

    return parser    


if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    if namespace.settings != None:
        filenameset = loadFilesFromYAML(namespace.settings)
        outputdirectory = namespace.output
    else:
        filenameset = getFiles('curated','bngl')
        #filenameset = getFiles('complex2','bngl')
        outputdirectory = 'curated'

    #translate(filenameset,outputdirectory)    
    #with open('new_non_curated/failure.dump','rb') as f:
    #    s = pickle.load(f)
    #filenameset = getFiles('complex2','bngl')
    generateBNGXML(filenameset,output= outputdirectory)
