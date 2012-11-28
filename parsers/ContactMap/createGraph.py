# -*- coding: utf-8 -*-
"""
Created on Wed Nov 21 16:56:19 2012

@author: proto
"""


from readBNGXML import parseXML
import re
from lxml import etree

import pygraphviz as pgv

def graphVizGraph(molecules):
    graph = pgv.AGraph()
    for idx,molecule in enumerate(molecules):
        print molecule.graphVizGraph(graph,"m%i" % idx)
    return graph
    
def createNodes(molecules):
    graph = etree.Element("graph")
    graphDictionary = {}
    for idx,molecule in enumerate(molecules):
        mol = etree.Element("node")
        mol.set("id","m%s"%idx)
        molGraph,molDictionary = molecule.createGraph(idx)
        graphDictionary.update(molDictionary)
        graphDictionary[mol.get("id")] = molecule.name
        mol.append(molGraph)
        graph.append(mol)
    print graphDictionary
    return graph,graphDictionary
    
def updateEdges(graph,graphDictionary,rules):
    for react,product,act,mapp,nameDict in rules:
        print '--'
        for element in act:
            if element.action in ['AddBond']:
                #delete the component tag
                mol1 = re.sub('_C[^_]*$', '', element.site1)
                mol2 = re.sub('_C[^_]*$', '', element.site2)
                getActionNodes(mol1,mol2,)

def createGraph(array,dictionary,nameDictionary,subarray,cindex):
    for element in array:
        if tuple([x.name for x in element.molecules]) in dictionary:
            sbr1 = dictionary[tuple([x.name for x in element.molecules])]
        else:
            sbr1 = subarray.subgraph(name = 'cluster%i_%s'% (len(subarray.nodes()),cindex))
            dictionary[tuple([x.name for x in element.molecules])] = sbr1

        nameDictionary.update(element.graphVizGraph(sbr1,sbr1.get_name()))

def biPartiteEdges(graph,dictionaries,reactants,products,actions,mapps,nameDict,idx):
    for action in actions:
        #if action.action == 'AddBond':
            graph.add_edge(dictionaries[0][action.site1],"R%i" %idx,directed=True)
            graph.add_edge(dictionaries[0][action.site2],"R%i" %idx,directed=True)
            for mapp in mapps:
                if action.site1 in mapp:
                    compSite1 = [x for x in mapp if x!= action.site1][0]
                    graph.add_edge("R%i"%idx,dictionaries[1][compSite1],directed=True)
                if action.site2 in mapp:
                    compSite2 = [x for x in mapp if x!= action.site2][0]
                    graph.add_edge("R%i"%idx,dictionaries[1][compSite2],directed=True)
    
def createBiPartite(rules):
    graph = pgv.AGraph(directed=True)
    gReactants = graph.subgraph(name='clusterReactants')
    gRules = graph.subgraph(name='clusterRules')
    gProducts = graph.subgraph(name='clusterProducts')
    
    reactantDictionary = {}
    reactantNameDictionary = {}
    productDictionary = {}
    productNameDictionary = {}
    idx = 1
    for react,product,_,_,_ in rules:
        createGraph(react,reactantDictionary,reactantNameDictionary,gReactants,'r%i' %idx)
        createGraph(product,productDictionary,productNameDictionary,gProducts,'p%i' % idx)
        gRules.add_node("R%i" % idx)
        idx += 1
    idx = 1
    for react,product,act,mapp,nameDict in rules:
        biPartiteEdges(graph,[reactantNameDictionary,productNameDictionary],react,product,act,mapp,nameDict,idx)
        idx+=1
        
    graph.write('simple.dot')
    graph.layout(prog='fdp')
    graph.draw('simple.png')

def main(fileName):
    mol,rules = parseXML(fileName)
    #graph,graphDictionary = createNodes(mol)
    createBiPartite(rules)
    #graph = graphVizGraph(mol)
    #graph.write("/home/proto/simple.dot")
    #updateEdges(graph,graphDictionary,rules)
    #nx.draw(graph)
    #plt.show()
    
    #https://groups.google.com/forum/?fromgroups=#!topic/networkx-discuss/Aq8900R8v2E    
    
    

if __name__ == "__main__":
    main("BM48.xml")
