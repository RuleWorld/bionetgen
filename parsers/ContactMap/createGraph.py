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
'''   
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
'''



def createGraph(chemicals,dictionary,clusterDictionary,subarray,cindex,nameDict):
    processedElements = []
    leftoverElements = []
    listOfBonds = {}
    for element in chemicals:
        if element.hasWildCardBonds():
            leftoverElements.append(element)
            continue
        if tuple([x.name for x in element.molecules]) in dictionary:
            sbr1 = dictionary[tuple([x.name for x in element.molecules])]
        else:
            sbr1 = subarray.subgraph(name = 'cluster%i_%s'% (len(subarray.nodes()),cindex))
            dictionary[tuple([x.name for x in element.molecules])] = sbr1
        clusterDictionary.update(element.graphVizGraph(sbr1,sbr1.get_name()))
        processedElements.append(element)
        listOfBonds.update(element.listOfBonds(nameDict))
    return processedElements,leftoverElements,listOfBonds

       
    
def findIsomorphisms(element,knowledgeDatabase):
    counter = 0
    isomorphs = []
    newSpecies = None
    for molecule in element.molecules:
        for component in molecule.components:
            if '+' in component.bonds:
                relationDatabase = knowledgeDatabase[molecule.name][component.name]
                counter += 1
                for  relation in relationDatabase:
                    if newSpecies == None:
                        newSpecies = deepcopy(element)
                    #TODO: i may need to generate new ids for this stuff
                    newMolecule = newSpecies.getMoleculeById(molecule.idx)
                    newComponent = newMolecule.getComponentById(component.idx)
                    #newMolecule.idx = '%s_m%i' % (newMolecule.idx,randint(0,10000))
                    #newComponent.idx = '%s_c%i' % (newMolecule.idx,randint(0,10000))
                    newSpecies.identifier = randint(0,10000)
                    newComponent.bonds.remove('+')
                    newBondNumber = max(newSpecies.getBondNumbers())+1
                    newComponent.addBond(newBondNumber)
                    newMoleculePartner = st.Molecule(relation[0],molecule.idx + '_%i' % randint(0,100000))
                    newComponentPartner = st.Component(relation[1],component.idx + '_%i' % randint(0,100000))
                    newComponentPartner.addBond(newBondNumber)
                    newMoleculePartner.addComponent(newComponentPartner)
                    newSpecies.addMolecule(newMoleculePartner)
                    #newSpecies.randomizeIds()
                    isomorphs.append(newSpecies)
                    newSpecies.bonds.append((newComponentPartner.idx,newComponent.idx))
                #for element in componentList
    return isomorphs

def createGraphWithKnowledge(chemicals,dictionary,clusterDictionary,subarray,cindex,knowledgeDatabase,leftOvers):
    processedElements = []
    leftoverElements = []
    if leftOvers == []:
        return leftoverElements
    print '\\\\',leftOvers
    
    for element in leftOvers:
        if tuple([x.name for x in element.molecules]) in dictionary:
            sbr1 = dictionary[tuple([x.name for x in element.molecules])]
        else:
            sbr1 = subarray.subgraph(name = clusterName)
            dictionary[tuple([x.name for x in element.molecules])] = sbr1
        isomorphArray = findIsomorphisms(element,knowledgeDatabase)
        for isomorph in isomorphArray:
            clusterDictionary.update(isomorph.graphVizGraph(sbr1,'%s_%i' %(sbr1.get_name(),isomorph.identifier)))
            #listOfBonds.update(element.listOfBonds(nameDict))
        
    
    '''
    for element in chemicals:
        if element.hasWildCardBonds():
            leftoverElements.append(element)
        if tuple([x.name for x in element.molecules]) in dictionary:
            sbr1 = dictionary[tuple([x.name for x in element.molecules])]
        else:
            sbr1 = subarray.subgraph(name = 'cluster%i_%s'% (len(subarray.nodes()),cindex))
            dictionary[tuple([x.name for x in element.molecules])] = sbr1
        
        nameDictionary.update(element.graphVizGraph(sbr1,sbr1.get_name()))
        processedElements.append(element)
    
    return processedElements,leftoverElements
    '''

def reactionCenter(graph,dictionaries,reactants,products,action,mapps,nameDict,idx):
    #add the reactant edges
    graph.add_edge(dictionaries[0][action.site1],"R%i" %idx,directed=True)
    graph.add_edge(dictionaries[0][action.site2],"R%i" %idx,directed=True)
    #add the product edges
    for mapp in mapps:
        if action.site1 in mapp:
            compSite1 = [x for x in mapp if x!= action.site1][0]
            graph.add_edge("R%i"%idx,dictionaries[1][compSite1],directed=True)
        if action.site2 in mapp:
            compSite2 = [x for x in mapp if x!= action.site2][0]
            graph.add_edge("R%i"%idx,dictionaries[1][compSite2],directed=True)

def context(graph,dictionaries,reactants,products,action,mapps,nameDict,idx):
    context = []
    for reactant in reactants:
        context.extend(reactant.notContainsComponentIdx([action.site1,action.site2]))
    for component in context:
        graph.add_edge(dictionaries[0][component.idx],"R%i" %idx,directed=True,color="red")

    print [str(x) for x in context]
    
def biPartiteEdges(graph,dictionaries,reactants,products,actions,mapps,nameDict,idx):
    
    
    for action in actions:
        #reaction center edges
        reactionCenter(graph,dictionaries,reactants,products,action,mapps,nameDict,idx)
        context(graph,dictionaries,reactants,products,action,mapps,nameDict,idx)
                
                    
    
    
def createBiPartite(rules):
    graph = pgv.AGraph(directed=True)
    gReactants = graph.subgraph(name='clusterReactants')
    gRules = graph.subgraph(name='clusterRules')
    gProducts = graph.subgraph(name='clusterProducts')
    
    reactantDictionary = {}
    nameDictionary = {}
    productDictionary = {}
    productNameDictionary = {}
    processed = []
    leftover = {}
    listofbonds = {}
    idx = 1
    for react,product,_,_,nameDict in rules:
        print nameDict
        tp,tl,LoB = createGraph(react,reactantDictionary,nameDictionary,gReactants,'r%i' %idx,nameDict)
        processed.extend(tp)
        leftover['r%i' %idx] = tl
        listofbonds.update(LoB)
        tp,tl,LoB = createGraph(product,productDictionary,nameDictionary,gProducts,'p%i' % idx,nameDict)
        listofbonds.update(LoB)
        processed.extend(tp)
        leftover['p%i' %idx] = tl
        gRules.add_node("R%i" % idx)
        idx += 1
    print '======',listofbonds
    while len(leftover) > 0:
        idx = 1
        for react,product,_,_,_ in rules:
            createGraphWithKnowledge(react,reactantDictionary,nameDictionary,gReactants,'r%i' % idx,listofbonds,leftover['r%i' % idx])
            createGraphWithKnowledge(product,productDictionary,nameDictionary,gProducts,'p%i' % idx,listofbonds,leftover['p%i' % idx])
            idx += 1  
        leftover = []
    idx = 1
    for react,product,act,mapp,nameDict in rules:
        biPartiteEdges(graph,[nameDictionary,nameDictionary],react,product,act,mapp,nameDict,idx)
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
