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
    
def constructHistogram(data,fileName):
    import matplotlib.pyplot as plt
    plt.clf()
    plt.hist(data)
    plt.xlabel('Context Cooperativity',fontsize=18)
    plt.savefig('{0}coop.png'.format(fileName))


def cooperativityAnalysis(fileName):
    species,rules,par = extractRulesfromBNGL(fileName)    
    transformationCenter,transformationContext, \
    transformationProduct,atomicArray,actionNames= extractCenterContext(rules)
    
    modifiedContextDistribution = [Counter() for _ in transformationCenter]
    
    for idx,contextList in enumerate(transformationContext):
        for context in contextList[0]:
            for centerList in transformationCenter:
                for centerListInstance in centerList:
                    if context in centerListInstance:
                        modifiedContextDistribution[idx].update([context])
    mentions = [sum([x[y] for y in x]) for x in modifiedContextDistribution]
    constructHistogram([len(x) for x in modifiedContextDistribution],fileName)
    constructHistogram(mentions,fileName+'edges')
    
    


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
    for center in redundantDict:
        flag = False
        if len(redundantDict[center])>1:
            flag = True
        for rateKey in redundantDict[center]:
            if len([x for x in center if x not in ['Add','Delete']]) != 0:
                if (len(redundantDict[center][rateKey])>1) and not flag:
                    #print '++++',center,rateKey,redundantDict[center][rateKey]
                    redundantProcesses += len(redundantDict[center][rateKey])
                elif flag:
                    #print '----',center,rateKey,redundantDict[center][rateKey]
                    groupedProcesses += len(redundantDict[center][rateKey])
            totalProcesses += len(redundantDict[center][rateKey])
    print groupedProcesses*1.0/totalProcesses,redundantProcesses*1.0/totalProcesses
    return groupedProcesses*1.0/totalProcesses,redundantProcesses*1.0/totalProcesses

def extractRulesfromBNGL(fileName):
    console.bngl2xml('complex/{0}.bngl'.format(fileName),timeout=10)
    species,rules,par= readBNGXML.parseXML('{0}.xml'.format(fileName))
    return species,rules,par
    

import numpy as np
import scipy.interpolate as interpolate

def inverse_transform_sampling(data, n_bins=40, n_samples=1000):
    hist, bin_edges = np.histogram(data, bins=n_bins, density=True)
    cum_values = np.zeros(bin_edges.shape)
    cum_values[1:] = np.cumsum(hist*np.diff(bin_edges))
    inv_cdf = interpolate.interp1d(cum_values, bin_edges)
    r = np.random.rand(n_samples)
    return inv_cdf(r)

import matplotlib.pyplot as plt
import cPickle as pickle

def plotresults(results,yvals,fileName):
    plt.clf()
    plt.plot( results, yvals )
    plt.axis([0, 1, 0, 1])
    plt.xlabel('Percentage of grouped processes', fontsize=18)
    plt.savefig('{0}.png'.format(fileName))

    
if __name__ == "__main__":
    #createBipartiteGraph('complex/output5.bngl')
    #species,rules = extractRulesfromBNGL('output19')
    processList = []
    with open('sortedD.dump','rb') as f:
        atomizationStats = pickle.load(f)
    atomizationDict = {}
    atomizationList = []
    groupedDict = []
    for element in atomizationStats:
        try:
            atomizationDict[element['index']] = element['atomization']
        except:
            continue
    for element in range(1,491):
        
        try:
            if element in atomizationDict and atomizationDict[element] > 0:
                print element,
                processList.append(cooperativityAnalysis2('output{0}'.format(element)))
                atomizationList.append(atomizationDict[element])
                tmp = [element]
                tmp.extend(processList[-1])
                groupedDict.append(tmp)
        except (IOError,IndexError):
            print
            continue

    
    groupedProcess,redundantProcess = zip(*processList)
    #plt.clf()
    #plt.scatter(groupedProcess,atomizationList)
    #plt.show()
    print groupedDict
    '''
    groupedProcess=np.sort( groupedProcess )
    redundantProcess=np.sort( redundantProcess )
    yvals=np.arange(len(groupedProcess))/float(len(groupedProcess))
    plotresults(groupedProcess,yvals,'grouped>0')
    plotresults(redundantProcess,yvals,'redundant>0')
    '''
    #print processList
    #cooperativityAnalysis2('egfr_net')    
    #createContactMap(  'output/output5.bngl')