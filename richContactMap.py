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
        
        options = [{'style':'filled','color':'teal'} for x in range(0,len(transformationContext[element-1]))]
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
    
def createBipartiteGraph(fileName):
    rules = extractRulesfromBNGL(fileName)    
    transformationCenter = []
    transformationContext = []
    transformationProduct = []
    actionNames = []
    atomicArray = []
    

    #extract the context of such reactions
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
    
    idx  = 0
    for rule,center,context,product,action,atom in zip(rules,transformationCenter,transformationContext,transformationProduct,actionNames,atomicArray):
        redundantPattern = []
        if idx in reverseRedundantDict:
            keyList = reverseRedundantDict[idx]
            for key in keyList:
                redundantPattern.append(patternDictList[key[0]][key[1]])
        redundantPatternCounter =  dictionaryToCounter(redundantPattern)
        createRuleBiPartite(rule,center,context,product,action,atom,'img/bipartite{0}'.format(idx),redundantPatternCounter)
        idx+=1
            


    #for idx,rule in enumerate(rules):
        

def extractRulesfromBNGL(fileName):
    console.bngl2xml(fileName)
    species,rules,par= readBNGXML.parseXML('output19.xml')
    return rules
    
    
if __name__ == "__main__":
    createBipartiteGraph('output/output19.bngl')