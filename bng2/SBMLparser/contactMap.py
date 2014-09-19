# -*- coding: utf-8 -*-
"""
Created on Thu Nov 28 17:17:12 2013

@author: proto
"""
import sys
sys.path.insert(0, '../utils/')
import consoleCommands as console
import readBNGXML
import networkx as nx
import cPickle as pickle
from copy import copy
from collections import Counter

from os import listdir
from os.path import isfile, join

def simpleGraph(graph,species,observableList,prefix='',superNode={}):
    '''
    build a simple contact map. species, components and addbond
    information is included
    '''
    edgeList = []
    nodeDict = {}    
    for singleSpecies in species:
        if '{1}_{0}'.format(singleSpecies.name,prefix) in superNode:
            name = superNode['{1}_{0}'.format(singleSpecies.name,prefix)]
            graph.add_node(name,size=superNode[name],color='orange')
        else:
            name = '{1}_{0}'.format(singleSpecies.name,prefix)
            graph.add_node(name,size=1,color='pink')
            
        nodeDict[singleSpecies.idx] = name
        for component in singleSpecies.components:
            graph.add_node('{0}({1})'.format(name,component.name),size=0.5,color='teal')
            edgeList.append(['{0}'.format(name),'{0}({1})'.format(name,component.name),10])
    for edge in observableList:
        if len(edge) == 2:
            name1 = '{0}_{1}'.format(prefix,edge[0].split('(')[0])
            name2 = '{0}_{1}'.format(prefix,edge[1].split('(')[0])
            if name1 in superNode:
                name1 = superNode[name1]
            if name2 in superNode:
                name2 = superNode[name2]
            name1 = '{0}({1}'.format(name1,edge[0].split('(')[1])
            name2 = '{0}({1}'.format(name2,edge[1].split('(')[1])

            edgeList.append([name1,name2,2])
    graph.add_weighted_edges_from(edgeList)
    return nodeDict

def main():
    with open('linkArray.dump', 'rb') as f:
        linkArray = pickle.load(f)
    with open('xmlAnnotationsExtended.dump','rb') as f:
        annotations = pickle.load(f)
        
    speciesEquivalence = {}
    onlyDicts = [ x for x in listdir('./complex')]
    onlyDicts = [x for x in onlyDicts if '.bngl.dict' in x]

    for x in onlyDicts:
        with open('complex/{0}'.format(x),'rb') as f:
            speciesEquivalence[int(x.split('.')[0][6:])] = pickle.load(f)
        
    
    for cidx,cluster in enumerate(linkArray):
        #FIXME:only do the first cluster
        cidx = 0
        cluster = linkArray[0]
        if len(cluster) == 1:
            continue
        annotationsDict = {idx:x for idx,x in enumerate(annotations)}
        annotationsIncidence = {}

        #get the model intersection points

        for element in annotationsDict:
            for species in annotationsDict[element]:
                key = annotationsDict[element][species][1]
                if key not in annotationsIncidence:
                    annotationsIncidence[key] = set([])
                annotationsIncidence[key].add(element)

        #number = 19
        graph = nx.Graph()
        nodeDict = {}
        superNodeDict = {}
    
        for element in annotationsIncidence:
            if len(annotationsIncidence[element]) > 1:
                tmpEdgeList = []
                names= []
                for idx in annotationsIncidence[element]:
                    for species in [x for x in annotationsDict[idx] if annotationsDict[idx][x][1] == element]:
                         try:
                             print speciesEquivalence[idx]
                             if isinstance(speciesEquivalence[idx][species],str):
                                 tmpEdgeList.append('{0}_{1}'.format(idx,speciesEquivalence[idx][species][0:-2]))
                                 names.append(speciesEquivalence[idx][species][0:-2])
                             else:
                                 tmpEdgeList.append('{0}_{1}'.format(idx,speciesEquivalence[idx][species].molecules[0].name))
                                 names.append(speciesEquivalence[idx][species].molecules[0].name)
                         except:
                            
                            print '----','error',idx,species
                            pass
                names = [x.lower() for x in names]
                name =  Counter(names).most_common(5)
                name = [x for x in name if len(x[0])> 1]
                if len(name) == 0:
                    name = Counter(names).most_common(1)
                for edge in tmpEdgeList:
                    superNodeDict[edge] = name[0][0]
                if len(name) > 0:
                    superNodeDict[name[0][0]] = len(tmpEdgeList)
                #pairs = [(x,y,1) for x in tmpEdgeList for y in tmpEdgeList if x!= y] 
                #graph.add_weighted_edges_from(pairs)
        print superNodeDict
        for x in cluster:
            number = x-1
            try:
                console.bngl2xml('complex/output{0}.bngl'.format(number))
                species,rules,parameterDict,observableList= readBNGXML.parseXML('output{0}.xml'.format(number))
                
            except:
                continue
            nodeDict[number] = simpleGraph(graph,species,observableList,number,superNodeDict)
            
            
    
       
       
        
                        
        nx.write_gml(graph,'cgraph_{0}.gml'.format(cidx))
        break
                    
    #print nodeDict
def main2():
    number = 48
    species,rules,parameterDict,observableList= readBNGXML.parseXML('output{0}.xml'.format(number))
    graph = nx.Graph()

    simpleGraph(graph,species,observableList,number)
    nx.write_gml(graph,'graph_{0}.gml'.format(number))


if __name__ == "__main__":
    main()

