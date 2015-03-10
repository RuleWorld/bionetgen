import concurrent.futures

import fnmatch
import os
from subprocess import call        
import multiprocessing as mp
import progressbar
import sys

sys.path.insert(0, '../utils/')
import consoleCommands as console

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
        result = call(['python','sbmlTranslator.py','-i',
        #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
        fileName,
        '-o',os.path.join(outputdirectory, str(fileName.split('/')[-1])) + '.bngl',
        '-c','config/reactionDefinitions.json',
        '-n','config/namingConventions.json',
        '-a'],stdout=f)
    return result


def generateBNGXML(bnglFiles,format='BNGXML'):
    
    print 'converting {0} bnglfiles'.format(len(bnglFiles))
    progress = progressbar.ProgressBar()

    for i in progress(range(len(bnglFiles))):
        xmlName = '.'.join(bnglFiles[i].split('.')[:-1]) + '.xml'
        

        #if os.path.exists(xmlName):
        #    continue
        if format == 'BNGXML':
            console.bngl2xml(bnglFiles[i], timeout=120)
        elif format == 'SBML':
            console.bngl2sbml(bnglFiles[i],timeout=120)
        else:
            raise Exception

    print 'moving xml files'
    files = glob.iglob(os.path.join('.', "*.xml"))
    for xmlfile in files:
        if os.path.isfile(xmlfile):
            shutil.move(xmlfile, directory)


import pickle

def translate():
    directory = 'XMLExamples/non_curated'
    outputdirectory = 'new_non_curated'
    restart = True
    filenameset = getFiles(directory,'xml')

    if not restart:
        existingset = getFiles(outputdirectory,'bngl')
        existingset = ['.'.join(x.split('.')[:-1]).split('/')[-1] for x in existingset]
        filenameset = [x for x in filenameset if x.split('/')[-1] not in existingset]

    with open('failure2.dump','rb') as f:
        filenameset2 = pickle.load(f)
    filenameset = [x for x in filenameset if x in filenameset2]
    print(len(filenameset))
    
    futures = []
    workers = mp.cpu_count()*2
    progress = progressbar.ProgressBar(maxval= len(filenameset)).start()
    i = 0
    
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

    
if __name__ == "__main__":
    #translate()    
    with open('new_non_curated/failure.dump','rb') as f:
        s = pickle.load(f)
    generateBNGXML(s)