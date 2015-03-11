import os.path
import collections
import cPickle as pickle
from copy import copy

import sys
sys.path.insert(0, '../utils/')
import readBNGXML
import progressbar
import numpy as np
from sklearn.cluster import AffinityPropagation
from sklearn.cluster import spectral_clustering
import difflib
import fnmatch
import matplotlib.pyplot as plt
import pandas
import seaborn as sns
def extractMoleculeTypesFromFile(fileName):
    species, rules, par = readBNGXML.parseXML(fileName)
    return species

def getValidFiles(directory, extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append(os.path.join(root, filename))
    return matches

def extractRulesFromFile(fileName):
    species, rules, par = readBNGXML.parseXML(fileName)
    return rules

def matchAnnotationsToSpecies(moleculeArray, annotationDictionary):
    '''
    Given a list of molecule types and a set of annotations, this method
    will match and create a dictionary containing  the annotations for
    the intersection
    '''
    parsedDictionary = {}
    finalDictionary = {}
    parsedDictionary = copy(annotationDictionary)
    for annotationName in annotationDictionary:
        #speciesName = '_'.join(annotationName.split('_')[0:-1])
        #speciesId = annotationName.split('_')[-1]
        parsedDictionary[speciesName] = annotationDictionary[annotationName]
        parsedDictionary[speciesId] = annotationDictionary[annotationName]

    # print parsedDictionary
    for molecule in moleculeArray:
        if molecule.name in annotationDictionary:
            finalDictionary[molecule.name] = parsedDictionary[molecule.name]

    return parsedDictionary


def createNeighborhoodDictionary(moleculeArray):
    neighborhoodDictionary = collections.defaultdict(list)
    moleculeNames = {x.name.lower(): x.name for x in moleculeArray}

    for molecule in moleculeArray:
        for component in molecule.components:
            if component.name in moleculeNames:
                neighborhoodDictionary[molecule.name].append(
                    moleculeNames[component.name])
    return neighborhoodDictionary


def resolveAnnotation(annotations):
    with open('parsedAnnotations.dump', 'rb') as f:
        parsedAnnotations = pickle.load(f)

    newAnnotations = {}
    for annotationKey in annotations:
        for annotation in annotations[annotationKey]:
            if annotation in parsedAnnotations and \
                    parsedAnnotations[annotation] != []:
                newAnnotations[annotationKey] = parsedAnnotations[annotation]
    return newAnnotations


def extractMoleculeTypes(folderName,annotationsFolder,includeAnnotations=True):
    '''
    return a list of molecule types structures
    from a group of BNG-XML files in a folder <folderName>
    '''
    moleculeTypesArray = []
    progress = progressbar.ProgressBar()
    #with open('annotations.dump', 'rb') as f:
    #    annotationsArray = pickle.load(f)

    #with open(os.path.join(annotationsFolder,'annotationDictionary.dump'),'rb') as f:
    #    annotationsArray = pickle.load(f)

    bngxmlFiles = getValidFiles(folderName,'xml')
    for element in progress(range(0, len(bngxmlFiles))):
        fileName = bngxmlFiles[element]
        
        try:
        
            moleculeTypes = extractMoleculeTypesFromFile(fileName)
            observablesLen = readBNGXML.getNumObservablesXML(fileName)
            if includeAnnotations:
                #annotations = matchAnnotationsToSpecies(moleculeTypes,
                #                                        annotationsArray[fileName])
                
                try:
                    annotations = annotationsArray['XMLExamples/non_curated/'+fileName[:-4].split('/')[1]]
                    resolvedAnnotations = resolveAnnotation(annotations)
                except KeyError:
                    print(fileName)
                    continue
                    

            else:
                annotations = []
                resolvedAnnotations = []
            neighborhoodDictionary = createNeighborhoodDictionary(
                moleculeTypes)
            moleculeTypesArray.append(
                [moleculeTypes, annotations, resolvedAnnotations,
                    neighborhoodDictionary,fileName,observablesLen])
        except IOError:
            continue
    return moleculeTypesArray

from pandas import Series

def extractProcessDisstribution(folderName):
    '''
    return a list of molecule types structures
    from a group of BNG-XML files in a folder <folderName>
    '''
    processArray = []
    progress = progressbar.ProgressBar()
    #with open('annotations.dump', 'rb') as f:
    #    annotationsArray = pickle.load(f)

    #with open(os.path.join(annotationsFolder,'annotationDictionary.dump'),'rb') as f:
    #    annotationsArray = pickle.load(f)

    bngxmlFiles = getValidFiles(folderName[0],'xml')
    modelHistogram = collections.defaultdict(list)
    actionDict = collections.defaultdict(list)
    for element in progress(range(0, len(bngxmlFiles))):
        actionHistogram = {'DeleteBond': 0, 'Add': 0, 'StateChange': 0, 
        'AddBond': 0, 'Delete': 0,'ChangeCompartment':0}
        fileName = bngxmlFiles[element]
        try:
            rules = extractRulesFromFile(fileName)
            for rule in rules:
                for action in rule[0].actions:
                    actionHistogram[action.action] += 1
            del actionHistogram['ChangeCompartment']
            totalProcesses = sum([actionHistogram[x] for x in actionHistogram])
            if totalProcesses == 0:
                continue
            actionHistogram = {x:actionHistogram[x]*1.0/totalProcesses for x in actionHistogram}
            for x in actionHistogram:
                actionDict['process'].append(x)
                actionDict['percentage'].append(actionHistogram[x])
                actionDict['database'].append(folderName[1])

            #actionHistogram.sort(key=lambda x:x[0])
            for element in actionHistogram:
                modelHistogram[element].append(actionHistogram[element])
            
        except IOError:
            continue


    
    for x in modelHistogram:
        modelHistogram[x] = [np.mean(modelHistogram[x]),np.std(modelHistogram[x])]

    return modelHistogram,actionDict
    '''
    s = Series([x for x in actionHistogram.elements()])
    vc = s.value_counts()
    ax = vc.plot(kind='bar',fontsize=18,rot=0)
    fig = ax.get_figure()
    fig.set_size_inches(18.5,12.0)
    fig.savefig('{0}process.png'.format(folderName))
    print '{0}process.png'.format(folderName)
    #print actionHistogram
    '''

def compareStrings(name1, name2,minThreshold=0.8):
    if len(name1) > 3 and len(name2) > 3:
        similarity = difflib.SequenceMatcher(a=name1,b=name2).quick_ratio()
        #if similarity < minThreshold:
        #    similarity = -1
        affinity =  1000 * similarity
        return affinity

    return -1e3


def compareAnnotations(annotation1,annotation2):
    affinity = 0
    for ann1 in annotation1:
        for ann2 in annotation2:
            affinity += compareStrings(ann1,ann2)
    return affinity


def distanceFunction(moleculeA, moleculeB):
    moleculeAStructure = moleculeA[0]
    moleculeBStructure = moleculeB[0]
    moleculeAAnn = moleculeA[1]
    moleculeBAnn = moleculeB[1]
    moleculeANeigh = moleculeA[3]
    moleculeBNeigh = moleculeB[3]


    affinity = compareStrings(moleculeAStructure, moleculeBStructure)
    
    affinity += 0.05 * compareAnnotations(moleculeAAnn,moleculeBAnn)
    for neighborA in moleculeANeigh:
        for neighborB in moleculeBNeigh:
            affinity += 0.1 * compareStrings(neighborA,neighborB,0.7)

    return affinity


def createSimilarityMatrix(modelMoleculeTypeList):
    similarityMatrix = np.zeros(
        (len(modelMoleculeTypeList), len(modelMoleculeTypeList)))

    progress = progressbar.ProgressBar()
    for index1 in progress(range(0, len(modelMoleculeTypeList))):
        similarityMatrix[index1][index1] = 1000
        for index2 in range(index1+1, len(modelMoleculeTypeList)):
            similarityMatrix[index1][index2] = distanceFunction(modelMoleculeTypeList[index1],
                                                                modelMoleculeTypeList[index2])
            similarityMatrix[index2][index1] = similarityMatrix[index1][index2]


    return similarityMatrix


def moleculeTypeNamesConsolidation(moleculetypestotalarray):
    modelMoleculeTypeList = []
    progress = progressbar.ProgressBar()

    for index in progress(range(0, len(moleculetypestotalarray))):
        model = moleculetypestotalarray[index]
        moleculeTypes = model[0]
        annotations = model[1]
        resolvedAnnotations = model[2]
        neighborhood = model[3]
        modelNumber = model[4]
        observablesLen = model[5]
        for molecule in moleculeTypes:
            if molecule.name in resolvedAnnotations:
                rann = resolvedAnnotations[molecule.name]
            else:
                rann = []
            if molecule.name in annotations:
                ann = annotations[molecule.name]
            else:
                ann = []
            modelMoleculeTypeList.append([molecule.name,
                                          rann, modelNumber,
                                          neighborhood[molecule.name],ann,observablesLen])
    return modelMoleculeTypeList


def rawAnnotationCoverage(directory):
    
    with open(os.path.join(directory,'annotationDictionary.dump'),'rb') as f:
        annotationInformation = pickle.load(f)
    print(sum([1 for x in annotationInformation.keys() for y in annotationInformation[x] if annotationInformation[x][y] != []]),sum([1
         for x in annotationInformation.keys() for y in annotationInformation[x]]))
    annotationCounter = collections.Counter()
    modelsWithoutAnnotation = collections.Counter()
    for model in annotationInformation:
        for species in annotationInformation[model]:
            if annotationInformation[model][species] == []:
                modelsWithoutAnnotation.update([model.split('/')[2]])
            for annotation in annotationInformation[model][species]:
                try:
                    annotationCounter.update([annotation.split('/')[3]])
                except:
                    continue
    print(annotationCounter)
    #print('no annotation',modelsWithoutAnnotation)


def preliminaryAnalysis(directory='new_non_curated',directory2='XMLExamples/non_curated'):
        
    
    print('building data structures...')
    moleculeTypesArray = extractMoleculeTypes(directory,directory2,False)
    with open(os.path.join(directory,'moleculeTypeDataSet.dump'), 'wb') as f:
        pickle.dump(moleculeTypesArray, f)
    
    with open(os.path.join(directory,'moleculeTypeDataSet.dump'), 'rb') as f:
        moleculeTypesArray = pickle.load(f)

    #clusterMoleculeTypes(moleculeTypesArray)
    modelMoleculeTypeList = moleculeTypeNamesConsolidation(moleculeTypesArray)
    '''
    print('building similarity matrix...')
    similarityMatrix = createSimilarityMatrix(modelMoleculeTypeList)
    with open(os.path.join(directory,'modelMoleculeTypeSimiliarityMatrix'),'wb') as f:
        pickle.dump(similarityMatrix,f)
    
    with open(os.path.join(directory,'modelMoleculeTypeSimiliarityMatrix'),'rb') as f:
        similarityMatrix = pickle.load(f)
    
    print('running clustering algorithm...')
    af = AffinityPropagation(affinity="precomputed",preference=500).fit(similarityMatrix)
    #af = spectral_clustering(affinity="precomputed").fit(similarityMatrix)


    with open(os.path.join(directory,'clusteringResult.dump'),'wb') as f:
        pickle.dump(af,f)
    

    with open(os.path.join(directory,'clusteringResult.dump'),'rb') as f:
        af = pickle.load(f)
    cluster_centers_indices = af.cluster_centers_indices_

    '''    
    print('final results:')
    #labels = af.labels_

    #n_clusters_ = len(cluster_centers_indices)

    #print('Estimated number of clusters: %d' % n_clusters_)
    print('Original number of molecule types: {0}'.format(len(modelMoleculeTypeList)))
    print('Original number of species: {0}'.format(sum([x[5] for x in moleculeTypesArray])))
    print('Number of molecule types with annotations: {0}'.format(len([x for x in modelMoleculeTypeList if x[4] != []])))
    print('number of models analyzed: {0}'.format(len(moleculeTypesArray)))
    moleculeDictionary = collections.defaultdict(list)
    #for element,label in zip(modelMoleculeTypeList,labels):
    #    moleculeDictionary[label].append(element)
    
    #print('molecule types without annotations: {0}'.format(([x for x in modelMoleculeTypeList if x[4] == []])))
    with open(os.path.join(directory,'finalDictionary'),'wb') as f:
        pickle.dump(moleculeDictionary,f)


def componentAnalysis(directory,atomizeThreshold=0):
    componentCount = []
    bindingCount = []
    stateCount = []
    with open(os.path.join(directory,'moleculeTypeDataSet.dump'), 'rb') as f:
        moleculeTypesArray = pickle.load(f)
    for model in moleculeTypesArray:
        modelComponentCount = [len(x.components) for x in model[0]]

        bindingComponentCount = [len([y for y in x.components if len(y.states) == 0]) 
                                    for x in model[0]]

        modificationComponentCount = [sum([max(1,len(y.states)) for y in x.components]) 
                                    for x in model[0]]

        bindingCount.append(bindingComponentCount)
        stateCount.append(modificationComponentCount)
        componentCount.append(modelComponentCount)
    #print [(np.mean(x),np.std(x)) for x in componentCount]
    interestingCount = [index for index,x in enumerate(componentCount) if np.mean(x) >= atomizeThreshold]
    componentCount = np.array(componentCount)
    bindingCount = np.array(bindingCount)
    stateCount = np.array(stateCount)

    componentCount = componentCount[interestingCount]
    bindingCount = bindingCount[interestingCount]
    stateCount = stateCount[interestingCount]

    totalCount = np.array([y for x in componentCount for y in x])
    bindingTotalCount = np.array([y for x in bindingCount for y in x])
    stateTotalCount = np.array([y for x in stateCount for y in x])
    '''
    print 'component summary', np.mean(totalCount),np.std(totalCount),len(totalCount)
    print 'binding summary',np.mean(bindingTotalCount),np.std(bindingTotalCount)
    print 'modification summary',np.mean(stateTotalCount),np.std(stateTotalCount)
    plt.clf()
    plt.hist(totalCount,bins=[0,1,2,3,4,5,6,7])
    plt.xlabel('Number of components',fontsize=20)
    plt.ylabel('Number of molecules',fontsize=20)
    plt.savefig('componentsvsmolecules.png')
    '''
    #plt.clf()
    zeroComponents  = [collections.Counter(x)[0] for x in componentCount]
    
    #print sum([x for x in zeroComponents if x > 7]),len([x for x in zeroComponents if x > 7])
    return totalCount,bindingTotalCount,stateTotalCount
    #plt.show()

def getXMLFailures(directory):
    bng = getValidFiles(directory,'bngl')
    bngxml =  getValidFiles(directory,'xml')

    bngxml = ['.'.join(x.split('.')[:-1]) + '.bngl' for x in bngxml]
    
    failures = [x for x in bng if x not in bngxml]
    print len(failures)
    with open(os.path.join(directory,'failure.dump'),'wb') as f:
        pickle.dump(failures,f)

def componentDensityPlot():
    directory = [('bnglTest','BNG control set'),('complex2','BioModels curated'),
    ('new_non_curated','BioModels non curated')]
    colors= sns.color_palette("Set1", 3)
    f, (ax1, ax2,ax3) = plt.subplots(3, 1, sharex=True, figsize=(8, 6))
    for color,direct in zip(colors,directory):
        totalCount, bindingCount, modifyCount = componentAnalysis(direct[0],0.1)
        sns.kdeplot(totalCount, shade=True,color=color,label=direct[1],ax=ax1,clip=(0, 8),bw=0.5)
        sns.kdeplot(bindingCount, shade=True,color=color,ax=ax2,clip=(0, 8),bw=0.5)
        sns.kdeplot(modifyCount, shade=True,color=color,ax=ax3,clip=(0, 8),bw=0.5)
    plt.xlabel('Number of components')
    ax1.set_title('Components per molecule')
    ax2.set_title('Binding components per molecule')
    ax3.set_title('Modification components per molecule')
    plt.savefig('componentDensity.png')

def processHistogram():
    directory = [('bnglTest','BNG control set'),('complex2','BioModels curated'),('new_non_curated','BioModels non curated')]
    processDistro  = []
    
    pandasDistro = collections.defaultdict(list)

    processOrder = ['Add','Delete','AddBond','DeleteBond','StateChange']

    '''
    for direct in directory:
        process,pandasc = extractProcessDisstribution(direct)
        processDistro.append(process)
        for element in pandasc:
            pandasDistro[element].extend(pandasc[element])
    
    pandasDistro = pandas.DataFrame(data=pandasDistro)
    print pandasDistro
    with open('processDistro.dump','wb') as f:
        pickle.dump(processDistro,f)
        pickle.dump(pandasDistro,f)
    '''

    with open('processDistro.dump','rb') as f:
        processDistro = pickle.load(f)
        pandasDistro = pickle.load(f)

    
    sns.factorplot("process", "percentage", "database", pandasDistro,kind='bar',x_order=processOrder)
    plt.savefig('processBarChar2.png')
    print processDistro[0]

    f, axs = plt.subplots(3, 2, sharex=True, figsize=(8, 8))

    colors= sns.color_palette("Set1", 3)
    for color,direct in zip(colors,directory):
        for index,process in enumerate(processOrder):
            actions = np.array(pandasDistro[pandasDistro.process==process][pandasDistro.database == direct[1]]['percentage'].values)
            sns.kdeplot(actions, shade=True,color=color,label=direct[1],ax=axs[index/2][index%2],clip=(0,1))
            ax=axs[index/2][index%2].set_title(process)
            axs[index/2][index%2].set_xlim([0,1])
    plt.savefig('processDensity.png')

    g = sns.FacetGrid(pandasDistro, row="process", col="database", hue="database",
        margin_titles=True,row_order=processOrder,xlim=(0,1))
    g.map(sns.kdeplot, "percentage",shade=True,clip=(0,1))
    plt.savefig('processDensityGrid.png')
    #plt.show()
    #
def rankMoleculeTypes(directory):
    with open(os.path.join(directory,'moleculeTypeDataSet.dump'), 'rb') as f:
        moleculeTypesArray = pickle.load(f)
    progress = progressbar.ProgressBar()
    moleculeTypeTuples = []
    for element in progress(moleculeTypesArray):
        for molecule in element[0]:
            moleculeTypeTuples.append([molecule.name,len(molecule.components),element[4]])
    moleculeTypesDatabase = pandas.DataFrame(data=moleculeTypeTuples,columns=['molecule','size','files'])    
    print moleculeTypesDatabase.sort('size',ascending=False).head(30)

if __name__ == "__main__":
    #preliminaryAnalysis(directory='new_non_curated',directory2='biomodels/non_metabolic')
    rankMoleculeTypes('new_non_curated')
    #processHistogram()

    #componentDensityPlot()    

    '''
    colors = ['r','Y','b']
    #print processDistro
    fig, ax = plt.subplots()
    #print np.arange(len(processDistro[0].keys()))
    index = 0

    rects = []
    processList = ['StateChange','AddBond','DeleteBond','Add','Delete']
    for x,y in enumerate(colors):
        rects.append(ax.bar(np.arange(len(processDistro[x].keys()))+0.25*x, [processDistro[x][z][0] for z in processList], 0.25, color=y, yerr=[processDistro[x][z][1] for z in processList]))
        
    

    ax.set_ylabel('Percentage')
    ax.set_title('Process percentage by database')
    ax.set_xticks(np.arange(len(processDistro[x].keys()))+0.35)
    ax.set_xticklabels( [x for x in processList] )

    ax.legend( (rects[0][0], rects[1][0],rects[2][0]), ('BNG control group', 'BioModels curated','BioModels non curated') )
    plt.savefig('processBarChar.png')
    '''
    #plt.show()
    #getXMLFailures(directory[0])
    #

    #main('bnglTest','')
    # rawAnnotationCoverage('XMLExamples/non_curated')
