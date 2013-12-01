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

def simpleGraph(graph,species,observableList,prefix=''):
    '''
    build a simple contact map. species, components and addbond
    information is included
    '''
    edgeList = []
    nodeDict = {}    
    for singleSpecies in species:
        graph.add_node('{1}_{0}'.format(singleSpecies.name,prefix),weight=10,color='pink')
        nodeDict[singleSpecies.idx] = '{1}_{0}'.format(singleSpecies.name,prefix)
        for component in singleSpecies.components:
            graph.add_node('{2}_{0}({1})'.format(singleSpecies.name,component.name,prefix),weight=1,color='teal')
            edgeList.append(['{1}_{0}'.format(singleSpecies.name,prefix),'{2}_{0}({1})'.format(singleSpecies.name,component.name,prefix),10])
    for edge in observableList:
        if len(edge) == 2:
            edgeList.append(['{0}_{1}'.format(prefix,edge[0]),'{0}_{1}'.format(prefix,edge[1]),2])
    graph.add_weighted_edges_from(edgeList)
    return nodeDict

def main():
    with open('linkArray.dump', 'rb') as f:
        linkArray = pickle.load(f)
    with open('xmlAnnotationsExtended.dump','rb') as f:
        annotations = pickle.load(f)
    for cidx,cluster in enumerate(linkArray):
        if len(cluster) == 1:
            continue
        annotationsDict = {}
        #number = 19
        graph = nx.Graph()
        nodeDict = {}
        speciesEquivalence = {}
        for x in cluster:
            number = x-1
            print number
            try:
                annotationsDict[number] = annotations[number]
                with open('complex/output{0}.bngl.dict'.format(number),'rb') as f:
                    speciesEquivalence[number] = pickle.load(f)
                console.bngl2xml('complex/output{0}.bngl'.format(number))
                species,rules,parameterDict,observableList= readBNGXML.parseXML('output{0}.xml'.format(number))
                nodeDict[number] = simpleGraph(graph,species,observableList,number)
                
            except:
                continue
            
            
        annotationsIncidence = {}
    
        #get the model intersection points
        for element in annotationsDict:
            for species in annotationsDict[element]:
                key = annotationsDict[element][species][1]
                if key not in annotationsIncidence:
                    annotationsIncidence[key] = set([])
                annotationsIncidence[key].add(element)
       
       
        
        for element in annotationsIncidence:
            if len(annotationsIncidence[element]) > 1:
                tmpEdgeList = []
                for idx in annotationsIncidence[element]:
                    for species in [x for x in annotationsDict[idx] if annotationsDict[idx][x][1] == element]:
                        try:
                             if isinstance(speciesEquivalence[idx][species],str):
                                 tmpEdgeList.append('{0}_{1}'.format(idx,speciesEquivalence[idx][species][0:-2]))
                             else:
                                 tmpEdgeList.append('{0}_{1}'.format(idx,speciesEquivalence[idx][species].molecules[0].name))
                        except:
                            continue
                pairs = [(x,y,1) for x in tmpEdgeList for y in tmpEdgeList if x!= y] 
                graph.add_weighted_edges_from(pairs)
                        
        nx.write_gml(graph,'cgraph_{0}.gml'.format(cidx))
                    
    #print nodeDict
def main2():
    number = 19
    species,rules,parameterDict,observableList= readBNGXML.parseXML('output{0}.xml'.format(number))
    graph = nx.Graph()

    simpleGraph(graph,species,observableList,number)
    nx.write_gml(graph,'graph_{0}.gml'.format(number))


if __name__ == "__main__":
    main()

