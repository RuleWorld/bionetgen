# -*- coding: utf-8 -*-
"""
Created on Wed Nov 21 16:56:19 2012

@author: proto
"""


from readBNGXML import parseXML
import re
from lxml import etree
import structures as st
from copy import deepcopy
import pygraphviz as pgv
from random import randint



def extractMolecules(site1,site2,chemicalArray):
  
    atomicPatterns = {}
    reactionCenter = set()
    context = set()
    
    for reactant in chemicalArray:
        ta,tr,tc = reactant.extractAtomicPatterns(site1,site2)
        #for element in ta:
        #    atomicPatterns.add(element)
        atomicPatterns.update(ta)
        for element in tr:
            reactionCenter.add(element)
        for element in tc:
            context.add(element)
        
    
    return atomicPatterns,reactionCenter,context

 
def getMapping(mapp,site):
    for mapping in mapp:
        if site in mapping:
            return [x for x in mapping if x != site][0]                


def solveWildcards(atomicArray):
    '''
    When you have a wildcard '+' as a bond configuration, this method allows you
    to go through the list of atomic elements and find which patterns the '+'
    can potentially resolve to
    '''
    standinArray = {}
    for wildcard in [x for x in atomicArray if '+' in x]:
        for atomic in [x for x in atomicArray if '+' not in x and len(atomicArray[x].molecules) > 1]:
            if atomicArray[wildcard].molecules[0].name in [x.name for x in atomicArray[atomic].molecules]:
                if wildcard not in standinArray:
                    standinArray[wildcard] = []
                standinArray[wildcard].append(atomicArray[atomic])
    atomicArray.update(standinArray)
            
            

def extractTransformations(rules):
    atomicArray = {}    
    transformationCenter = []
    transformationContext = []
    productElements = []
    for react,product,act,mapp,nameDict in rules:
        for action in act:
            atomic,reactionCenter,context = extractMolecules(action.site1,action.site2,react)
            transformationCenter.append(reactionCenter)
            transformationContext.append(context)
            atomicArray.update(atomic)    
            productSites = [getMapping(mapp,action.site1),getMapping(mapp,action.site2)]
            atomic,rc,_ = extractMolecules(productSites[0],productSites[1],product)
            productElements.append(rc)
            atomicArray.update(atomic)
    solveWildcards(atomicArray)                
    return atomicArray,transformationCenter,transformationContext,productElements
            

def createNode(atomicArray,chemical,chemicalDictionary,subgraph,nameHeader):
    clusterName = ''
    if '+' not in str(chemical) and len(atomicArray[chemical].molecules) == 1:
        clusterName = '%s_%s' % (nameHeader,atomicArray[chemical].molecules[0].name)
        if  clusterName not in chemicalDictionary:
            chemicalDictionary[chemical] = subgraph.subgraph(name = clusterName)
        sbr1 = chemicalDictionary[chemical]
        chemicalDictionary.update(atomicArray[chemical].graphVizGraph(sbr1,sbr1.get_name()))
    else:
        if '+' not in str(chemical):
            clusterName = nameHeader
            chemicalDictionary.update(atomicArray[chemical].graphVizGraph(subgraph,nameHeader))
    if clusterName != '':
        clusterName = '%s_%s' % (clusterName,str(atomicArray[chemical]))
    return clusterName
    
def createBiPartite(rules,transformations,fileName):
    atomicArray,transformationCenter,transformationContext,productElements = extractTransformations(rules)
    graph = pgv.AGraph(directed=True)
    gReactants = graph.subgraph(name='clusterReactants')
    gRules = graph.subgraph(name='clusterRules')
    gProducts = graph.subgraph(name='clusterProducts')
    
    reactantDictionary = {}
    productDictionary = {}

    for idx,element in enumerate(transformationCenter):
        if idx+1 in transformations:
            gRules.add_node("T%i" % (idx + 1))
            for reactant in element:
                clusterName = createNode(atomicArray,reactant,reactantDictionary,gReactants,'clusterr')
                graph.add_edge(clusterName,"T%i"%(idx+1),directed=True)
            
    for idx,element in enumerate(transformationContext):
        if idx+1 in transformations:
            for reactant in element:
                clusterName = createNode(atomicArray,reactant,reactantDictionary,gReactants,'clusterr')
                if clusterName != '':
                    graph.add_edge(clusterName,"T%i"%(idx+1),directed=True,color='red')
                else:
                    for element in atomicArray[reactant]:
                        clusterName = 'clusterr_%s'%str(element)
                        graph.add_edge(clusterName,"T%i"%(idx+1),directed=True,color='red')
    for idx,element in enumerate(productElements):
        if idx+1 in transformations:
            for product in element:
                clusterName =createNode(atomicArray,product,productDictionary,gProducts,'clusterp')
                graph.add_edge("T%i"%(idx+1),clusterName,directed=True)
            
    graph.write('simple.dot')
    graph.layout(prog='fdp')
    graph.draw('%s.png' % fileName)

def main(fileName):
    mol,rules = parseXML(fileName)
    #graph,graphDictionary = createNodes(mol)
    #for element in range(6,7):
    #    createBiPartite(rules,[element],'simple%i' % element)
    createBiPartite(rules,range(1,100),'simple')
    #graph = graphVizGraph(mol)
    #graph.write("/home/proto/simple.dot")
    #updateEdges(graph,graphDictionary,rules)
    #nx.draw(graph)
    #plt.show()
    
    #https://groups.google.com/forum/?fromgroups=#!topic/networkx-discuss/Aq8900R8v2E    
    
    

if __name__ == "__main__":
    main("fceri.xml")
