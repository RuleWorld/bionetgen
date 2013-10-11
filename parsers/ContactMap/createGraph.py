# -*- coding: utf-8 -*-
"""
Created on Wed Nov 21 16:56:19 2012

@author: proto
"""


from readBNGXML import parseXML
import pygraphviz as pgv
import subprocess
import pexpect

def extractMolecules(action,site1,site2,chemicalArray):
    '''
    this method goes through the chemicals in a given array 'chemicalArray'
    and extracts its atomic patterns into two arrays:
        those elements that are contained in [site1,site2] will be put in the
        reactionCenter set. The rest will    be placed in the context set.
        The entire system will be put into the atomicPatterns dictionary
    Keyword arguments:
    site1,site2 -- where the action takes place
    chemicalArray -- the list of species we will be extracting atomic patters 
    from
    '''
    atomicPatterns = {}
    reactionCenter = set()
    context = set()
    
    for reactant in chemicalArray:
        ta,tr,tc = reactant.extractAtomicPatterns(action,site1,site2)
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
            
            

            

def createNode(atomicArray, chemical, chemicalDictionary, subgraph, nameHeader, builtList):
    '''
    creates a child node for 'chemical' on the 'subgraph' graph
    Args:
        
    '''
    clusterName = ''
    
    if not str(chemical) in builtList:
        #if its not a complex
        if '+' not in str(chemical) and len(atomicArray[chemical].molecules) == 1:
            clusterName = '%s_%s' % (nameHeader,atomicArray[chemical].molecules[0].name)
            #direction of the bonds within a subgraph
            if 'Chemicals' in str(subgraph):
                layout = 'RL'
            else:
                layout = 'LR'
            chemicalDictionary.update(atomicArray[chemical].graphVizGraph(subgraph,clusterName,layout))
        else:
            #if it's a complex
            if '+' not in str(chemical):
                if 'Chemicals' in str(subgraph):
                    layout = 'RL'
                else:
                    layout = 'LR'

                clusterName = nameHeader
                chemicalDictionary.update(atomicArray[chemical].graphVizGraph(subgraph,nameHeader,layout))
        
        if clusterName != '':
            clusterName = 'cluster%s_%s_m0_%s' % (clusterName, 
                                str(atomicArray[chemical]), 
                                atomicArray[chemical].molecules[0].idx)
        builtList[str(chemical)] = clusterName
        return clusterName
    else:
        return builtList[str(chemical)]

def extractTransformations(rules):
    '''
    goes through the list of rules and extracts its reactioncenter,context and product
    atomic patterns per transformation action
    '''
    atomicArray = {}    
    transformationCenter = []
    transformationContext = []
    productElements = []
    actionName = []
    index=0
    label = []
    for react,product,act,mapp,_ in rules:
        index += 1
        for action in act:
            atomic,reactionCenter,context = extractMolecules(action.action,action.site1, 
                                                             action.site2, react)
            transformationCenter.append(reactionCenter)
            transformationContext.append(context)
            atomicArray.update(atomic)    
            productSites = [getMapping(mapp,action.site1), 
                            getMapping(mapp, action.site2)]
            atomic,rc,_ = extractMolecules(action.action,productSites[0], productSites[1],
                                           product)
            productElements.append(rc)
            atomicArray.update(atomic)
            actionName.append('%i-%s' % (index,action.action))
            r= '+'.join([str(x) for x in react])
            p = '+'.join([str(x) for x in product])
            label.append('->'.join([r,p,'%i-%s' % (index,action.action)]))

    solveWildcards(atomicArray)
                   
    return atomicArray, transformationCenter, transformationContext, productElements,actionName,label


def createSubGraph(element,
                   gSubgraph,chemicalDictionary,buildList,atomicArray,
                   graph,edgeIdx,tag):
    edgeArray = []
    for reactant in element:
        clusterName = createNode(atomicArray, reactant, 
                                 chemicalDictionary, gSubgraph, 
                                 tag,buildList)
        #theres a single context molecule
        if clusterName != '':
            edgeArray.append(clusterName)
        #theres multiple context nodes for a single transformation
        else:
            for atom in atomicArray[reactant]:
                clusterName = createNode(atomicArray, str(atom), 
                                chemicalDictionary, gSubgraph, 
                                tag,buildList)
                edgeArray.append(clusterName)
    return edgeArray           

def createBiPartite(rules, transformations, fileName, reactionCenter=True, 
                    context=True, products=True):
    '''
    creates a bipartite graph given a list of rules
    Args:
        rules: The list of rules to process
        transformations: which transformations do we want to process (their index)
        fileName: output filename
        reactioncenter,context,products: booleans indicating whether we desire include the reactioncenter,context
    or product in our ouput file
    '''
    
    #extract reactioncenter, context information
    atomicArray, transformationCenter, transformationContext, \
            productElements,actionNames,labelArray = extractTransformations(rules)
    #create the graph structure and the three main subgraphs
    
    if transformations != None and len(transformations) == 1:
        graph = pgv.AGraph(directed=True,strict=False,
                           label=labelArray[transformations[0]-1],
ranksep='1.5',rankdir='LR',compound='true') 
    else:
        graph = pgv.AGraph(directed=True, strict=False,ranksep='2.5',compound='true',
                            rankdir='LR')
    

    gReactants = graph.subgraph(name='clusterReactants', label='Chemicals',rankdir='TB')
    gRules = graph.subgraph(name='clusterRules', label='Transformations',rankdir='TB',ranksetp='2.5 ')
    gProducts = graph.subgraph(name='clusterProducts', label='Products',rankdir='TB')
    
    gReactants.node_attr.update(shape='square')
    gProducts.node_attr.update(shape='square')

    reactantDictionary = {}
    productDictionary = {}
    edgeIdx = 1
    reactantbuiltlist = {}
    productbuildlist ={}
    for idx,name in enumerate(actionNames):
        if transformations == None or idx+1 in transformations:
            gRules.add_node(name)
            gRules.add_node('{0}_dummy'.format(name),style='invis')
    #create reactionCenter nodes and edges
    if reactionCenter:
        transformationRange = range(0,len(transformationCenter)) if \
        transformations == None else transformations
        for element in transformationRange:
            edgeArray = createSubGraph(transformationCenter[element-1],
                       gReactants,reactantDictionary,reactantbuiltlist,atomicArray,
                       graph,edgeIdx,'clusterr')
            for edge in edgeArray:
                graph.add_edge(edge+'_dummy',actionNames[element-1],
                               key = 'key%i' % edgeIdx, ltail=edge)
                edgeIdx+=1

        
    #create context nodes
    if context:
        transformationRange = range(0,len(transformationContext)) if \
        transformations == None else transformations
        for idx,element in enumerate(transformationRange):
            edgeArray = createSubGraph(transformationContext[element-1],
                       gReactants,reactantDictionary,reactantbuiltlist,atomicArray,
                       graph,edgeIdx,'clusterr')
            for edge in edgeArray:
                graph.add_edge(edge+'_dummy',actionNames[element-1],
                               key = 'key%i' % edgeIdx, ltail=edge,color='red')
                edgeIdx+=1
    #create product nodes
    if products:
        
        transformationRange = range(0,len(productElements)) if \
        transformations == None else transformations
        for idx,element in enumerate(transformationRange):
            edgeArray = createSubGraph(productElements[element-1],
                       gProducts,productDictionary,productbuildlist,atomicArray,
                       graph,edgeIdx,'clusterp')
            for edge in edgeArray:
                graph.add_edge(actionNames[element-1],edge+'_dummy',
                               key = 'key%i' % edgeIdx, lhead=edge)
                edgeIdx+=1
        
        
    
    #output      
    print '%s.dot' % fileName
    graph.write('%s.dot' % fileName)
    #graph = pgv.AGraph('%s.dot' % fileName)
    #graph.layout(prog='fdp')
    #graph.draw('%s.png' % fileName)
    subprocess.call(['dot', '-Tsvg', '{0}.dot'.format(fileName),'-o{0}.svg'.format(fileName)])
    

def addAnnotations(fileName):
    bngl2xml('{0}.bngl'.format(fileName))
    _,rules = parseXML('{0}.xml'.format(fileName))
    totalAtomic = {}
    totalCenter = []
    totalContext = []
    totalProduct = []
    for rule in rules:
        atomicArray, transformationCenter, transformationContext, \
            productElements,actionNames,labelArray = extractTransformations([rule])
        totalAtomic.update(atomicArray)
        totalCenter.append(transformationCenter)
        totalContext.append(transformationContext)
        totalProduct.append(productElements)
        

def createXML(self):
    pass

def processBNGL(bngl,center,context,product):
    #xml = createXML(bngl)
    xml = bngl
    _,rules = parseXML(xml)
    
    createBiPartite(rules,None,bngl, 
                           reactionCenter=center, context=context, products=product)
                           
def bngl2xml(bnglFile):

    bngconsole = pexpect.spawn('bngdev --console')
    bngconsole.expect('BNG>')
    bngconsole.sendline('load {0}'.format(bnglFile))
    bngconsole.expect('BNG>')
    bngconsole.sendline('action writeXML()')
    bngconsole.expect('BNG>')
    bngconsole.close() 

    
def main(fileName):
    _,rules = parseXML(fileName)
    #print '---',rules
    #createBiPartite(rules,None,'simple', 
    #                       reactionCenter=True, context=True, products=True)
    
    for element in [2]:
        print element
        createBiPartite(rules, [element], 'simple%i' % element, 
                    reactionCenter=True, context=True, products=True)

        try:
            createBiPartite(rules, [element], 'simple%i' % element, 
                            reactionCenter=True, context=True, products=True)
            
        except:
            print 'xxx'
            continue
    
if __name__ == "__main__":
    #main("output9.xml")
    #bngl2xml('fceri_ji.bngl')
    main("fceri_ji.xml")
    #addAnnotations('fceri_ji')