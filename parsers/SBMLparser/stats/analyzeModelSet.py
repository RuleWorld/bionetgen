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
import readBNGXML
import pandas

sys.path.insert(0, '.')
sys.path.insert(0, os.path.join('.','SBMLparser'))
import argparse
home = expanduser("~")
sbmlTranslator = join(home, 'workspace', 'bionetgen', 'parsers', 'SBMLparser', 'SBMLparser', 'sbmlTranslator.py')
bngExecutable = join(home,'workspace','bionetgen','bng2','BNG2.pl')
visualizeExecutable = join(home,'workspace','bionetgen','bng2','Perl2','Visualization','visualize.pl')
graphAnalysis = join(home,'workspace','bionetgen','parsers','SBMLparser','stats','graphAnalysis.py')
collapsedContact = join(home,'workspace','bionetgen','parsers','SBMLparser','stats','collapsedContactMap.py')
compareModels = join(home, 'workspace', 'bionetgen', 'parsers', 'SBMLparser', 'SBMLparser', 'rulifier', 'compareModels.py')    
sbmlparserhome = join(home, 'workspace', 'bionetgen', 'parsers', 'SBMLparser', 'SBMLparser')

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
        result = call([sbmlTranslator,'-api',
        #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
        fileName,
        '-o', os.path.join(outputdirectory, str(fileName.split('/')[-1])) + '.bngl',
        '-c', 'config/reactionDefinitions.json',
        '-n', 'config/namingConventions.json',
        '-b', '/net/antonin/home/mscbio/jjtapia/workspace/bionetgen/bng2/BNG2.pl'
        ],stdout=f)
    return result

def convertXML(bnglfile, output, options=[]):
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

    command = [visualizeExecutable,'--bngl',bnglfile,'--groups','--collapse']
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


def dummy(result,output):
    pass

def parallelHandling(files,function,outputDir,options = [],postExecutionFunction=dummy):
    futures = []
    workers = mp.cpu_count()-1
    progress = progressbar.ProgressBar(maxval= len(files)).start()
    i = 0
    print 'running in {0} cores'.format(workers)
    with concurrent.futures.ProcessPoolExecutor(max_workers=workers) as executor:
        for fileidx in progress(range(len(files))):
            futures.append(executor.submit(function, files[fileidx],outputDir,options))
        for future in concurrent.futures.as_completed(futures,timeout=3600):
            postExecutionFunction(future.result(),outputDir)
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


def reactionBasedAtomizationFile(xmlFile,outputDataFrame,options):
    """
    get atomization metrics from a bng-xml file
    """
    ratomizationDict = {}

    try:
            # console.bngl2xml('complex/output{0}.bngl'.format(element),timeout=10)
        molecules, reactions, _ = readBNGXML.parseXML(xmlFile)

        atomizedProcesses = 0
        trueProcessesCounter = 0
        for reaction in reactions:
            if '0' in  [str(x) for x in reaction[0].reactants] or '0' in \
                    [str(x) for x in reaction[0].products]:
                continue
            trueProcessesCounter += 1
            # if len([x for x in action.action if x in ['Add','Delete']]) == 0:
            #    atomizedProcesses +=1

            for action in reaction[0].actions:
                if action.action in ['AddBond', 'DeleteBond', 'StateChange', 'ChangeCompartment']:
                    atomizedProcesses += 1
                    break


        #ato, nonato = stoichiometryAnalysis(rules)
        #atomizedDistro.extend(ato)
        #nonAtomizedDistro.extend(nonato)
        score = atomizedProcesses * 1.0 / trueProcessesCounter if trueProcessesCounter != 0 else 0

        #outputDataFrame = outputDataFrame.set_value(xmlFile,'score',score)
        #outputDataFrame = outputDataFrame.set_value(xmlFile,'lenght',len(rules))
        return xmlFile,score,len(reactions),len(molecules)*1.0/readBNGXML.getNumObservablesXML(xmlFile)
        #ratomizationDict['score'] = score
        #ratomizationDict['weight'] = weight
        #ratomizationDict['length'] = len(rules)
    except IOError:
        print 'io',xmlFile


def createCollapsedContact(xmlfile,outputdirectory,options=[]):
    """
    create a collapsed contact map for a given file
    """
    with open(os.devnull,"w") as f:
        result = call(['python',collapsedContact,'-i',
        #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
        xmlfile,
        '-o',os.path.join(outputdirectory, str(xmlfile.split('/')[-1])) + '._collapsed.gml',
        ],stdout=f)
    return result

def compareModelsContext(xmlfilepair,outputdirectory,options=[]):
    """
    compares a pair of models and returns their differences (if any) and the significance of those differences
    """
    with open(os.devnull, "w") as f:
        result = call(['python', '-m', sbmlparserhome, collapsedContact, '-f1', xmlfilepair[0], '-f2', xmlfilepair[1],
        #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
                       '-o', os.path.join(outputdirectory, '{0}-{1}.yaml'.format(namespace.file1, namespace.file2)),
                       ], stdout=f)
    return result        



def saveToDataframe(result,dataframe):
    """
    Store xml-analysis results in dataframe
    """
    filename = result[0].split('/')[-1]
    filename = '.'.join(filename.split('.')[:-1]) + '_regulatory.gml'
    dataframe = dataframe.set_value(filename,'atomization',result[1])
    dataframe = dataframe.set_value(filename,'weight',result[2])
    dataframe = dataframe.set_value(filename,'compression',result[3])

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
        filenameset = getFiles('curated/atomized','xml')
        outputdirectory = 'curated'
        ttype = 'atomizationScore'
        options = []

    if ttype == 'atomize':
        translate(filenameset,outputdirectory)    
    elif ttype == 'bngxml':
        generateBNGXML(filenameset,output= outputdirectory)
    elif ttype == 'graph':
        generateGraphs(filenameset,output=outputdirectory,options = options)
    elif ttype == 'entropy':
        call(['python',graphAnalysis,'-s',namespace.settings,'-o',outputdirectory])
    elif ttype == 'atomizationScore':
        print 'calculating score for {0} bng-xml files'.format(len(filenameset))
        atomizationScore = pandas.DataFrame()
        parallelHandling(filenameset,reactionBasedAtomizationFile,atomizationScore,postExecutionFunction=saveToDataframe)
        atomizationScore.to_hdf('atomizationResults.h5','atomization')
    elif ttype =='collapsedContact':
        parallelHandling(filenameset,createCollapsedContact,outputdirectory)
    elif type=='contextComparison':
        parallelHandling(filenameset, compareModelsContext, outputDirectory)
    else:
        raise Exception('Invalid output type')
    #with open('new_non_curated/failure.dump','rb') as f:
    #    s = pickle.load(f)
    #filenameset = getFiles('complex2','bngl')
    #generateBNGXML(filenameset,output= outputdirectory)
