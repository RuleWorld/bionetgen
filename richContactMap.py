# -*- coding: utf-8 -*-
"""
Created on Sun Oct 13 20:10:48 2013

@author: proto
"""

import sys
import contextAnalyzer
import subprocess
from collections import Counter
sys.path.insert(0, '../utils/')
import consoleCommands as console
import readBNGXML
import extractAtomic
import pygraphviz as pgv


sys.path.insert(0,'../ContactMap')
import createGraph
from copy import copy


def createRuleBiPartite(rule,transformationCenter,transformationContext,
                        transformationProduct,actionNames,atomicArray,fileName,redundantPattern):
    graph = pgv.AGraph(directed=True, strict=True,ranksep='2.5',compound='true',
                            rankdir='LR')

    gReactants = graph.subgraph(name='clusterReactants', label='Chemicals',rankdir='TB')
    gRules = graph.subgraph(name='clusterRules', label='Transformations',rankdir='TB',ranksetp='2.5 ')
    gProducts = graph.subgraph(name='clusterProducts', label='Products',rankdir='TB')

    gReactants.node_attr.update(shape='square')
    gProducts.node_attr.update(shape='square')
    
    edgeIdx = 0
    reactantDictionary = {}
    productDictionary = {}
    edgeIdx = 1
    reactantbuiltlist = {}
    productbuildlist ={}

    transformationRange = range(0,len(transformationCenter))
    skipArray = []

    for element in transformationRange:
        options = [{'style':'filled','color':'pink'} for x in range(0,len(transformationCenter[element-1]))]
        if len(options) == 0:
            skipArray.append(element)
            continue
        edgeArray = createGraph.createSubGraph(transformationCenter[element-1],
                   gReactants,reactantDictionary,reactantbuiltlist,atomicArray,
                   gReactants,edgeIdx,'clusterr',options)
        for edge in edgeArray:
            graph.add_edge(edge+'_dummy',actionNames[element-1],
                           key = 'key%i' % edgeIdx, ltail=edge)
            edgeIdx+=1

    for idx,element in enumerate(transformationRange):
        if element in skipArray:
            continue
        tmp = copy(transformationContext[element-1])
        #if(len(redundantPattern) > 0):
        tmp.subtract(redundantPattern)
        
        negativeCounts = Counter(el for el in tmp.elements() if tmp[el] <= 0)
        importantContext = Counter(el for el in tmp.elements() if tmp[el] > 0)
        redundantContext = Counter(el for el in redundantPattern.elements() if el not in negativeCounts and  el in transformationContext[element-1])
        
        options = [{'style':'filled','color':'green'} for x in range(0,len(transformationContext[element-1]))]
        options2 = [{'style':'filled','color':'cyan'} for x in range(0,len(transformationContext[element-1]))]

        edgeArray = createGraph.createSubGraph(importantContext,
                   gReactants,reactantDictionary,reactantbuiltlist,atomicArray,
                   gReactants,edgeIdx,'clusterr',options)
        edgeArray2 = createGraph.createSubGraph(redundantContext,
                   gReactants,reactantDictionary,reactantbuiltlist,atomicArray,
                   gReactants,edgeIdx,'clusterr',options2)
                   
        edgeArray.extend(edgeArray2)
        for edge in edgeArray:
            graph.add_edge(edge+'_dummy',actionNames[element-1],
                           key = 'key%i' % edgeIdx, ltail=edge,color='red')
            edgeIdx+=1
    for element in transformationRange:
        if element in skipArray:
            continue
        options = [{'style':'filled','color':'pink'} for x in range(0,len(transformationProduct[element-1]))]
        edgeArray = createGraph.createSubGraph(transformationProduct[element-1],
                   gProducts,productDictionary,productbuildlist,atomicArray,
                   gProducts,edgeIdx,'clusterp',options)
        for edge in edgeArray:
            graph.add_edge(actionNames[element-1],edge+'_dummy',
                           key = 'key%i' % edgeIdx, lhead=edge)
            edgeIdx+=1


    graph.write('%s.dot' % fileName)
    subprocess.call(['dot', '-Tsvg', '{0}.dot'.format(fileName),'-o{0}.svg'.format(fileName)])
    subprocess.call(['dot', '-Tpng', '{0}.dot'.format(fileName),'-o{0}.png'.format(fileName)])


def reverseLookup(redundantDict):
    '''
    grab a dictioanry of reaction centers and associate them by reaction instead
    '''
    reverseDictionary = {}
    for center in redundantDict:
        for action in redundantDict[center]:
            for reaction in redundantDict[center][action]:
                if reaction not in reverseDictionary:
                    reverseDictionary[reaction] =  []
                reverseDictionary[reaction].append([center,action])
    return reverseDictionary

def dictionaryToCounter(redundantPattern):
    tmp = Counter()
    for element in redundantPattern:
        for key in element:
            tmp.update(element[key])
    return tmp

def extractCenterContext(rules):
    transformationCenter = []
    transformationContext = []
    transformationProduct = []
    atomicArray = []
    actionNames = []
    for idx,rule in enumerate(rules):
        tatomicArray, ttransformationCenter, ttransformationContext, \
                tproductElements,tactionNames,tlabelArray = extractAtomic.extractTransformations([rule])

        transformationCenter.append(ttransformationCenter)
        transformationContext.append(ttransformationContext)
        actionNames.append(tactionNames)
        atomicArray.append(tatomicArray)
        transformationProduct.append(tproductElements)
    return transformationCenter,transformationContext, \
    transformationProduct,atomicArray,actionNames
    
def createBipartiteGraph(fileName):

    rules = extractRulesfromBNGL(fileName)    
    
    transformationCenter,transformationContext, \
    transformationProduct,atomicArray,actionNames= extractCenterContext(rules)

    
    

    redundantDict,patternDictList= contextAnalyzer.extractRedundantContext(rules,transformationCenter,transformationContext)
    reverseRedundantDict = reverseLookup(redundantDict)
    
    idx  = 0
    for rule,center,context,product,action,atom in zip(rules,transformationCenter,transformationContext,transformationProduct,actionNames,atomicArray):
        redundantPattern = []
        if idx in reverseRedundantDict:
            for key in reverseRedundantDict[idx]:
                redundantPattern.append(patternDictList[key[0]][key[1]])
        redundantPatternCounter =  dictionaryToCounter(redundantPattern)
        createRuleBiPartite(rule,center,context,product,action,atom,'img/bipartite{0}'.format(idx),redundantPatternCounter)
        idx+=1
            

def matchName(rmolecule,rcomponent,componentList):
        
    for component in componentList:
        if rmolecule in component and rcomponent in component:
            return True
    return False
    #for idx,rule in enumerate(rules):
def createMap(rules,center,redundantContext,fileName):
    graph = pgv.AGraph(directed=True, strict=True,ranksep='2.5',compound='true',
                       rankdir='LR')
    subgraphs = {}
    center = [[x.site1,x.site2] for x in rules[0].actions]
    center = set([y for x in center for y in x if y != None])
    newCenter = []
    for element in rules[0].mapping:
        if element[0] in center:
            newCenter.append(element[1])
    if len(newCenter) == 0:
        newCenter = center
    redundantKeys = list(redundantContext.elements())    
    for product in rules[0].products:
        for molecule in product.molecules:
            if molecule not in subgraphs:
                newSubgraph = graph.subgraph(name='cluster_{0}'.format(molecule.idx), 
                                             label='{0}'.format(molecule.name),
                                    rankdir='TB',shape='square',style='rounded',
                                    pendwidth=2)
                subgraphs[molecule.idx] = newSubgraph
            else:
                newSubgraph = subgraphs[molecule.idx]
            for component in molecule.components:
                if component.idx in newCenter:
                    
                    color = 'red'
                elif matchName(molecule.name,component.name,redundantKeys):
                    color = 'green'
                else:
                    color = 'blue'
                newSubgraph.add_node(component.idx,label=component.name,color=color,penwidth=2,shape='circle',fontcolor=color)
        for bond in product.bonds:
            graph.add_edge(bond[0],bond[1],dir='none',len=0.1,weight=100,penwidth=2)

    graph.write('%s.dot' % fileName)
    subprocess.call(['dot', '-Tsvg', '{0}.dot'.format(fileName),'-o{0}.svg'.format(fileName)])


def createContactMap(fileName):
    rules = extractRulesfromBNGL(fileName)    
    transformationCenter = []
    transformationContext = []
    transformationProduct = []
    actionNames = []
    atomicArray = []

    for idx,rule in enumerate(rules):
        tatomicArray, ttransformationCenter, ttransformationContext, \
                tproductElements,tactionNames,tlabelArray = extractAtomic.extractTransformations([rule])

        transformationCenter.append(ttransformationCenter)
        transformationContext.append(ttransformationContext)
        actionNames.append(tactionNames)
        atomicArray.append(tatomicArray)
        transformationProduct.append(tproductElements)

    #tatomicArray, ttransformationCenter, ttransformationContext, \
    #            tproductElements,tactionNames,tlabelArray = extractAtomic.extractTransformations(rules)
    redundantDict,patternDictList= contextAnalyzer.extractRedundantContext(rules,transformationCenter,transformationContext)
    reverseRedundantDict = reverseLookup(redundantDict)

    for idx in range(0,len(rules)):
        redundantPattern = []
        if idx in reverseRedundantDict:
            for key in reverseRedundantDict[idx]:
                redundantPattern.append(patternDictList[key[0]][key[1]])
        redundantPatternCounter =  dictionaryToCounter(redundantPattern)

        createMap(rules[idx],transformationProduct[idx],redundantPatternCounter,'conImg/contatMap{0}'.format(idx))
    
def constructHistogram(data,fileName,legend,weights,normed=True):
    import matplotlib.pyplot as plt
    plt.clf()
    plt.hist(data,weights=weights,normed=normed)
    plt.xlabel(legend,fontsize=18)
    plt.savefig('{0}.png'.format(fileName))



def analyzeSpace(centerString,molecules):
    #spaceCoveredHelper()
    totalSpace = 1
    excludeMolecules = []
    operators = ['Add','Delete','AddBond','DeleteBond','StateChange','ChangeCompartment']
    try:            
        if 'AddBond' in centerString or 'StateChange' in centerString:
            for centerCandidate in [x for x in centerString if x not in operators]:
                if len(centerCandidate) >= 2:
                    moleculeName = [x.split('(')[0] for x in centerCandidate]
                    moleculeInstance = [x for x in molecules if x.name in moleculeName]
                    if len(moleculeInstance) == 1: #symmetry
                        moleculeInstance.append(moleculeInstance[0])
                    totalSpace *= spaceCoveredHelper(moleculeInstance[0],''
                                        ,[moleculeInstance[1]],[],molecules)
                    totalSpace *= spaceCoveredHelper(moleculeInstance[1],'',
                                         [moleculeInstance[0]],[],molecules)
    
                if len(centerCandidate) == 1:
                    moleculeName = [x.split('(')[0] for x in centerCandidate]
                    moleculeInstance = [x for x in molecules if x.name in moleculeName]
                    totalSpace *= spaceCoveredHelper(moleculeInstance[0],'',
                                                     [],[],molecules)/2
                                                     
        if 'DeleteBond' in centerString:
            for moleculeString in [x for x in centerString if x not in operators]:
                if len(moleculeString) == 1 and '.' in moleculeString[0]:
                    moleculeName = [x.split('(')[0] for x in moleculeString[0].split('.')]
                    moleculeInstance = [x for x in molecules if x.name in moleculeName]
                    if len(moleculeInstance) == 1: #symmetry
                        moleculeInstance.append(moleculeInstance[0])
                    totalSpace *= spaceCoveredHelper(moleculeInstance[0],''
                                        ,[moleculeName[1]],[],molecules)
                    totalSpace *= spaceCoveredHelper(moleculeInstance[1],'',
                                         [moleculeName[0]],[],molecules)
    except CycleError:
        return -1
    return totalSpace
    

def cooperativityAnalysis2(fileName):
    species,rules,par = extractRulesfromBNGL(fileName)   
    transformationCenter,transformationContext, \
    transformationProduct,atomicArray,actionNames= extractCenterContext(rules)
    #group by reaction center    
    redundantDict,patternDictList= contextAnalyzer.extractRedundantContext(rules,transformationCenter,transformationContext)
    totalProcesses  = 0
    groupedProcesses = 0
    redundantProcesses=0
    reverseRedundantDict = reverseLookup(redundantDict)
    totalSpaceCovered = []
    for center in redundantDict:
        reactionInCenter = len([y for x in redundantDict[center] 
        for y in redundantDict[center][x]])
        if reactionInCenter > 1 and len([x for x in center if x not in ['Add','Delete']]) > 1:
            totalSpace =  analyzeSpace(center,species)
            totalSpaceCovered.append((totalSpace,reactionInCenter))
        flag = False
        if len(redundantDict[center])>1:
            flag = True
        for rateKey in redundantDict[center]:
            if len([x for x in center if x not in ['Add','Delete']]) > 1:
                if (len(redundantDict[center][rateKey])>1) and not flag:
                    #print '++++',center,rateKey,redundantDict[center][rateKey]
                    redundantProcesses += len(redundantDict[center][rateKey])
                elif flag:
                    #print '----',center,rateKey,redundantDict[center][rateKey]
                    groupedProcesses += len(redundantDict[center][rateKey])
            totalProcesses += len(redundantDict[center][rateKey])
    #print groupedProcesses*1.0/totalProcesses,redundantProcesses*1.0/totalProcesses
    
    return groupedProcesses*1.0/totalProcesses,redundantProcesses*1.0/totalProcesses,totalSpaceCovered

def extractRulesfromBNGL(fileName):
    #console.bngl2xml('complex/{0}.bngl'.format(fileName),timeout=10)
    species,rules,par= readBNGXML.parseXML('{0}.xml'.format(fileName))
    return species,rules,par
    

import numpy as np


import matplotlib.pyplot as plt
import cPickle as pickle

def plotresults(results,yvals,fileName,legend=''):
    plt.clf()
    plt.plot( results, yvals )
    plt.axis([0, 1, 0, 1])
    plt.xlabel(legend, fontsize=18)
    plt.savefig('{0}.png'.format(fileName))


class CycleError(Exception):
     def __init__(self, moleculeName,visitedMolecules):
         self.moleculeName = moleculeName
         self.visitedMolecules = visitedMolecules
     def __str__(self):
         return repr(self.moleculeName) + ' '  + repr(self.visitedMolecules)
def spaceCovered(molecules,observablesLen):
    
    print [str(x) for x in molecules]
    speciesCount = 0
    excludedMolecules = []
    
    for individualMolecule in molecules:
        print '++++'
        visitedMolecules = []
        tmp = spaceCoveredHelper(individualMolecule,'',excludedMolecules,visitedMolecules,molecules)
        print '--',individualMolecule.name,visitedMolecules,tmp,excludedMolecules
        excludedMolecules.append(individualMolecule.name)
        
        speciesCount += tmp
    return min(observablesLen*1.0/speciesCount,1.0)



def spaceCoveredHelper(moleculeInstance,comeFromMolecule,excludedMolecules,visitedMolecules,molecules):
    multiplier = 1
    for component in moleculeInstance.components:
        if len(component.states) > 0:
            multiplier *= len(component.states)
        else:
            for molecule in molecules:
                if molecule.name.lower() == component.name:
                    if molecule.name not in excludedMolecules:
                        if molecule.name not in visitedMolecules or \
            len([x for x in moleculeInstance.components if x.name.lower() == component.name]) > len([x for x in visitedMolecules if x == moleculeInstance.name]):
                            tmp = copy(visitedMolecules)
                            tmp.append(moleculeInstance.name)
                            multiplier *= 2 * spaceCoveredHelper(molecule,
                    moleculeInstance.name,excludedMolecules,tmp,molecules)
                        elif molecule.name != comeFromMolecule \
                        or len([x for x in visitedMolecules if x == moleculeInstance.name]) > 3:
                            raise CycleError(molecule.name,visitedMolecules)
                    else:
                        multiplier += 0
    return multiplier
    
def spaceCoveredCDF():
    spaceCoveredArray = []
    atomizationDict = {}
    with open('sortedD.dump','rb') as f:
        atomizationStats = pickle.load(f)
    for element in atomizationStats:
        try:
            atomizationDict[element['index']] = element['atomization']
        except:
            continue
    
    for element in range(1,491):
        try:
            if element in atomizationDict and atomizationDict[element] > 0:
                console.bngl2xml('complex/output{0}.bngl'.format(element),timeout=10)
                species,_,_= readBNGXML.parseXML('output{0}.xml'.format(element))
                observablesLen = readBNGXML.getNumObservablesXML('output{0}.xml'.format(element))
                try:
                    spaceCoveredArray.append([atomizationDict[element],spaceCovered(species,observablesLen)])
                    print element,spaceCoveredArray[-1]
                except CycleError:
                    spaceCoveredArray.append([atomizationDict[element],0])
                    print element,-1
                
        except (IOError,IndexError):
            print
            continue
        
    with open('spaceCovered.dump','wb') as f:
        pickle.dump(spaceCoveredArray,f)
    atomization,space = zip(*spaceCoveredArray)
    heatmap, xedges, yedges = np.histogram2d(space, atomization, bins=8)
    extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
    plt.clf()
    plt.imshow(heatmap, extent=extent)
    plt.xlabel('space')
    plt.ylabel('atomization')
    plt.show()  
    
    plt.savefig('spaceCovered.png')
    #spaceCoveredArray=np.sort( spaceCoveredArray )
    #yvals=np.arange(len(spaceCoveredArray))/float(len(spaceCoveredArray))
    #plotresults(spaceCoveredArray,yvals,'spaceCovered')


def reactionBasedAtomization(reactions):
    atomizedProcesses = 0    
    trueProcessesCounter = 0
    for reaction in reactions:
        if '0' in  [str(x) for x in reaction[0].reactants] or '0' in \
        [str(x) for x in reaction[0].products]:
            continue
        trueProcessesCounter +=1
        for action in reaction[0].actions:
            if action.action in ['AddBond','DeleteBond','StateChange','ChangeCompartment']:
                atomizedProcesses += 1
                break
    if trueProcessesCounter ==0:
        return 0,0
    return atomizedProcesses,trueProcessesCounter

from collections import defaultdict
generalSignature = defaultdict(list)
def stoichiometryAnalysis(reactions):
    ato = []
    nonato = []
    for reaction in reactions:
        if '0' in  [str(x) for x in reaction[0].reactants] or '0' in \
        [str(x) for x in reaction[0].products]:
            continue
        signature = ((len(reaction[0].reactants),len(reaction[0].products)))
        if len([x for x in reaction[0].actions if x.action in ['AddBond','DeleteBond','StateChange']]) > 0:
            ato.append(signature)
        else:
            generalSignature[signature].append(reaction)
            nonato.append(signature)
    return ato,nonato
from collections import defaultdict
def reactionBasedAtomizationDistro():
    '''
    calculates a rection atomization based metric:
    ration of atomized reactions (non syndeg) in a model
    '''
    ratomizationList = []
    ratomizationDict = defaultdict(dict)
    ratomizationListm10 = []
    ratomizationListl10 = []
    largeUseless = []
    syndelArray = []
    syndel =0
    totalRatomizedProcesses = 0
    totalReactions = 0
    totalProcesses = 0
    validFiles= 0
    atomizedDistro = []
    nonAtomizedDistro= []
    interesting = []
    for element in range(1,549):
        try:
            #console.bngl2xml('complex/output{0}.bngl'.format(element),timeout=10)
            _,rules,_= readBNGXML.parseXML('complex/output{0}.xml'.format(element))
            atomizedProcesses,weight = reactionBasedAtomization(rules)
            ato,nonato = stoichiometryAnalysis(rules)
            atomizedDistro.extend(ato)
            nonAtomizedDistro.extend(nonato)
            #if (2,1) in nonato:
            #    interesting.append(element)
            score = atomizedProcesses*1.0/weight if weight != 0 else 0
            totalRatomizedProcesses += atomizedProcesses
            totalReactions += len(rules)
            totalProcesses += weight
            ratomizationDict[element]['score'] = score
            ratomizationDict[element]['weight'] = weight
            ratomizationDict[element]['length'] = len(rules)
            syndelArray.append((len(rules)-weight)*1.0/len(rules))
            if score == -1:
                syndel += 1
                print element,'syndel'
                #ratomizationList.append([0,0,len(rules)])
                continue
            ratomizationList.append([score,weight,len(rules)])
            if len(rules) > 10:
                if weight*1.0/len(rules) >=0.1 and score < 0.1:
                    largeUseless.append(element)
                ratomizationListm10.append([score,weight,len(rules)])
            else:
                ratomizationListl10.append([score,weight,len(rules)])
            print element,ratomizationList[-1]
            validFiles += 1
        except (IndexError,ZeroDivisionError):
            syndel += 1
            print
            continue
        except IOError:
            print 
            continue
    with open('ratomization.dump','wb') as f:
        pickle.dump(ratomizationDict,f)
        
    print '-----'
    print 'atomized',Counter(atomizedDistro)
    print 'nonatomized',Counter(nonAtomizedDistro)
    print 'models with 2->1 non atomized reactions',interesting
    with open('nonatomizedreactions.dump','wb') as f:
        pickle.dump(generalSignature,f)
    ratomization,weights,length = zip(*ratomizationList)
    
    ratomizationm10,weightsm10,lengthm10 = zip(*ratomizationListm10)
    ratomizationl10,weightsl10,lengthl10 = zip(*ratomizationListl10)
    
    constructHistogram(syndelArray,'syndelHist','Fraction of synthesis and degradation reactions',np.ones(len(syndelArray)),normed=False)
    constructHistogram(ratomization,'ratomizationHist','Reaction atomization level ({0} models)'.format(len(ratomization)),np.ones(len(ratomization)),normed=False)
    constructHistogram(ratomizationm10,'ratomizationwHist_m10','Reaction atomization level ({0} models)'.format(len(ratomizationm10)),lengthm10,normed=True)
    weights = np.array(weights)
    length = np.array(length)
    tmp = weights*1.0/length
    
    
    ratomizationWP1 = [x for x,y in zip(ratomization,tmp) if y < 0.1]
    ratomizationWP10 = [x for x,y,z in zip(ratomization,tmp,length) if y >= 0.1 and z<10]
    ratomizationWP11 = [x for x,y,z in zip(ratomization,tmp,length) if y >= 0.1 and z>=10]
    
    constructHistogram(ratomizationWP1,'ratomizationWP1','Reaction atomization level',np.ones(len(ratomizationWP1)),normed=False)
    constructHistogram(ratomizationWP10,'ratomizationWP10','Reaction atomization level',np.ones(len(ratomizationWP10)),normed=False)
    constructHistogram(ratomizationWP11,'ratomizationWP11','Reaction atomization level',np.ones(len(ratomizationWP11)),normed=False)
    
    print 'process={0}, rprocess={1}, reactions = {2},syndel={3},valid={4}'.format(totalProcesses,
totalRatomizedProcesses,totalReactions,syndel,validFiles)

       
    tmp2 = zip(ratomization,tmp)
    tmp2.sort(key=lambda x:x[0])
    
    #heatmap showing histogram of atomization vs non syn-deg    
    heatmap, xedges, yedges = np.histogram2d(tmp,ratomization,bins=6)
    heatmap = np.log2(heatmap)
    
    extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
    
    
    plt.clf()
    plt.imshow(heatmap, extent=extent,aspect='auto',origin='lower',interpolation='nearest')
    plt.xlabel('Atomization level')
    plt.ylabel('Percentage of non syn-del reactions')
    cb = plt.colorbar()
    cb.set_label('log2(Number of models)')
    plt.show()  
    plt.savefig('atomizationHeatMap.png')
    
    #heatmap showing average atomization of %syn-def vs model size
    ratomizationHeatmapCounter = defaultdict(lambda : defaultdict(list))
    _,bin_edges = np.histogram(length,4)
    digitizedLength = np.digitize(length,bin_edges)
    _,bin_edges2 = np.histogram(tmp,4)
    digitizedSynDeg = np.digitize(tmp,bin_edges2)
    for rat,wei,leng in zip(ratomization,digitizedLength,digitizedSynDeg):
        ratomizationHeatmapCounter[wei][leng].append(rat)
    ratomizationHeatmap = np.zeros([5,5])
    for xelement in ratomizationHeatmapCounter:
        for yelement in ratomizationHeatmapCounter[xelement]:
            
            ratomizationHeatmap[xelement-1][yelement-1] = np.mean(ratomizationHeatmapCounter[xelement][yelement])
            ratomizationHeatmapCounter[xelement][yelement] = np.mean(ratomizationHeatmapCounter[xelement][yelement])


    heatmap, xedges, yedges = np.histogram2d(length,tmp,bins=5)
    heatmap = np.log2(heatmap)
    
    extent = [yedges[0], yedges[-1], xedges[0], xedges[-1]]

    plt.clf()
    ax = plt.gca()
    ax.plot(length,1-tmp,'o',alpha=0.5)    
    plt.xlabel('Number of reactions',fontsize=24)
    plt.ylabel('Fraction of syndel reactions',fontsize=24)
    ax.grid(True)
    ax.set_xscale('log')
    #ax.set_autoscale_on(False)
    plt.ylim([-0.1,1.1])
    #ax.set_yscale('log')
    plt.savefig('reactionsvsnsyndel.png')
    
    plt.clf()
    plt.scatter(length, 1-tmp, s=500, 
                c=[max(x,0) for x in ratomization])
    plt.xlabel('Number of reactions',fontsize=24)
    plt.ylabel('Fracion of syndel reactions',fontsize=24)

    ax = plt.gca()
    #ax.set_xscale('log')
    ax.grid(True)
    plt.xlim(xmin=0,xmax=100)
    plt.gray()
    cb = plt.colorbar()
    cb.set_label('Atomization level')

    plt.savefig('reactionsvssyndelwlinear.png')
    

    
    plt.clf()
    plt.imshow(ratomizationHeatmap, 
               extent=extent
               ,aspect='auto',origin='lower',interpolation='nearest')
    plt.xlabel('Fraction of non syn-del reactionsModel size')
    plt.ylabel('Model Size (reactions)')
    cb = plt.colorbar()
    cb.set_label('Atomization level')
    plt.show()  
    plt.savefig('atomizationHeatMap2.png')

        

    
    
    plt.clf()
    plt.imshow(heatmap, extent=extent,aspect='auto',origin='lower',interpolation='nearest')
    plt.ylabel('Model size (reactions)')
    plt.xlabel('Percentage of non syn-del reactions')
    cb = plt.colorbar()
    cb.set_label('log2(Number of models)')
    plt.show()  
    plt.savefig('atomizationHeatMap3.png')


    ratomization=np.sort( ratomization )
    ratomizationm10=np.sort( ratomizationm10 )
    ratomizationl10=np.sort(ratomizationl10)

    yvals=np.arange(len(ratomization))/float(len(ratomization))
    plotresults(ratomization,yvals,'ratomizationw','Reaction atomization level ({0} models)'.format(len(ratomizationList)))
    yvals=np.arange(len(ratomizationm10))   /float(len(ratomizationm10))
    plotresults(ratomizationm10,yvals,'ratomization_m10w','Reaction atomization level >10 reactions({0} models)'.format(len(ratomizationListm10)))
    yvals=np.arange(len(ratomizationl10))/float(len(ratomizationl10))
    plotresults(ratomizationl10,yvals,'ratomization_l10w','Reaction atomization level <=10 reactions({0} models)'.format(len(ratomizationListl10)))
    print 'syndel',syndel
    print '>10 with 0',largeUseless
    

def createGroupingCDF():
    '''
    analyzes a group ofatomized files with species atomization > 0
    and groups them according to whether they have groups of reaction with 
    same reaction center different context/reaction rates
    '''
    processList = []
    with open('sortedD.dump','rb') as f:
        atomizationStats = pickle.load(f)
    atomizationDict = {}
    atomizationList = []
    groupedDict = []
    spaceCoveredDict = {}
    for element in atomizationStats:
        try:
            atomizationDict[element['index']] = element['atomization']
        except:
            continue
    
    for element in range(1,491):
        try:
            if element in atomizationDict and atomizationDict[element] > 0:

                analysis = cooperativityAnalysis2('output{0}'.format(element))
                #space analysis                
                spaceCoveredDict[element] = analysis[-1]
                print element,analysis[-1]
                #grouping analysis
                processList.append(analysis[:-1])
                atomizationList.append(atomizationDict[element])
                tmp = [element]
                tmp.extend(processList[-1])
                groupedDict.append(tmp)
        except (IOError,IndexError):
            print
            continue

    with open('spaceCovered.dump','wb') as f:
        pickle.dump(spaceCoveredDict,f)
    with open('groupedProcessList.dump','wb') as f:
        pickle.dump(groupedDict,f)
    groupedProcess,redundantProcess = zip(*processList)
    #plt.clf()
    #plt.scatter(groupedProcess,atomizationList)
    #plt.show()
    #print groupedDict
    groupedProcess=np.sort( groupedProcess )
    redundantProcess=np.sort( redundantProcess )
    yvals=np.arange(len(groupedProcess))/float(len(groupedProcess))
    plotresults(groupedProcess,yvals,'grouped>0')
    plotresults(redundantProcess,yvals,'redundant>0')

import bioservices

def analyzeGroupingCDF():
    bio = bioservices.BioModels()

    with open('groupedProcessList.dump','rb') as f:
        groupedDict = pickle.load(f)
    elementList,groupedProcessList,redundantProcessList = zip(*groupedDict)
    hist,bin_edges = np.histogram(redundantProcessList,bins=5)
    binIndexArray = np.digitize(redundantProcessList,bin_edges)
    modelAnnotationBin = defaultdict(list)
    
    for element,binIndex in zip(elementList,binIndexArray):
        try:
            info = bio.getSimpleModelsByIds('BIOMD%010i' % element)
            parsedInfo = bioservices.Service('name').easyXML(info)
            modelName = stats.removeTags(str(parsedInfo['modelname'][0]))
            modelName = modelName.split(' - ')
            if len(modelName) > 1:
                modelName = modelName[1]
            else:
                modelName = modelName[0].split('_')
                if len(modelName) > 1:
                    modelName = ' '.join(modelName[1:])
                else:
                    modelName = modelName[0]
            print element,binIndex
            modelAnnotationBin[binIndex].append((element,modelName))
        except:
            print parsedInfo
            break
    print modelAnnotationBin
    print hist,bin_edges


def createSpaceDistribution():
    '''
    analyzes groups of files that have grouped reaction and creates a 
    distribution of how much space thse context-sensitive reactions cover
    '''
    with open('spaceCovered.dump','rb') as f:
        space = pickle.load(f)
    infiniteCycles = 0
    infiniteCyclesFiles = set([])
    spaceCovered = []
    groupSize = []
    modelsWithGroups = []
    for element in space:
        for entry in space[element]:
            if entry[0] == -1:
                infiniteCycles += 1
                infiniteCyclesFiles.add(element)
                continue
            if entry[1]> entry[0]:
                continue
            
            spaceCovered.append(entry[1]*1.0/entry[0])
            modelsWithGroups.append((element,spaceCovered[-1]))
            groupSize.append(entry[0])
    constructHistogram(spaceCovered,'spaceCovered','Percentage of space covered by\
reaction groups',np.ones(len(spaceCovered)),False)

    #uncomment if we wish to obtain a list of which model belongs 
    #to which group
    #analyzeSpaceDistribution(modelsWithGroups)

import bioservices
import stats

def analyzeSpaceDistribution(analyzedModels):
    bio = bioservices.BioModels()
    number,spaceCovered = zip(*analyzedModels)
    
    hist,bin_edges = np.histogram(spaceCovered,bins=5)
    binIndexArray = np.digitize(spaceCovered,bin_edges)
    modelAnnotationBin = defaultdict(list)
    
    for element,binIndex in zip(number,binIndexArray):
        try:
            info = bio.getSimpleModelsByIds('BIOMD%010i' % element)
            parsedInfo = bioservices.Service('name').easyXML(info)
            modelName = stats.removeTags(str(parsedInfo['modelname'][0]))
            modelName = modelName.split(' - ')
            if len(modelName) > 1:
                modelName = modelName[1]
            else:
                modelName = modelName[0].split('_')
                if len(modelName) > 1:
                    modelName = ' '.join(modelName[1:])
                else:
                    modelName = modelName[0]
            print element,binIndex
            modelAnnotationBin[binIndex].append(modelName)
        except:
            print parsedInfo
            break
    print modelAnnotationBin

import pprint
def nonAtomizedSpeciesAnalysis():
    with open('nonatomizedreactions.dump','rb') as f:
        nonAtomizedList= pickle.load(f)
    f = open('nonatomizedresults.txt','wt')
    for signature in nonAtomizedList:
        count = Counter()
        reactionList = defaultdict(list)
        f.write('--------\n')
        f.write(str(signature) + '\n')
        for reaction in nonAtomizedList[signature]:
            count[tuple(sorted([x.action for x in reaction[0].actions]))] += 1
            reactionList[tuple(sorted([x.action for x in reaction[0].actions]))].append(str(reaction[0]))
            if signature in [(2,1),(1,2),(1,1)]:
                f.write(str(count) + '\n')
                f.write('+++\n')
                pprint.pprint(dict(reactionList),stream=f)
    f.close()
if __name__ == "__main__":
    #spaceCoveredCDF()
    #reactionBasedAtomizationDistro()
    nonAtomizedSpeciesAnalysis()
    #createGroupingCDF()
    #analyzeGroupingCDF()
    
    #createSpaceDistribution()
    #createBipartiteGraph('complex/output5.bngl')
    #species,rules = extractRulesfromBNGL('output19')

    #print processList
    #cooperativityAnalysis2('egfr_net')    
    #createContactMap(  'output/output5.bngl')