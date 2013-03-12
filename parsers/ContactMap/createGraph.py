# -*- coding: utf-8 -*-
"""
Created on Wed Nov 21 16:56:19 2012

@author: proto
"""


from readBNGXML import parseXML
import pygraphviz as pgv



def extractMolecules(site1,site2,chemicalArray):
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
            atomic,reactionCenter,context = extractMolecules(action.site1, 
                                                             action.site2, react)
            transformationCenter.append(reactionCenter)
            transformationContext.append(context)
            atomicArray.update(atomic)    
            productSites = [getMapping(mapp,action.site1), 
                            getMapping(mapp, action.site2)]
            atomic,rc,_ = extractMolecules(productSites[0], productSites[1],
                                           product)
            productElements.append(rc)
            atomicArray.update(atomic)
            actionName.append('%i-%s' % (index,action.action))
            r= '+'.join([str(x) for x in react])
            p = '+'.join([str(x) for x in product])
            label.append('->'.join([r,p,'%i-%s' % (index,action.action)]))

    solveWildcards(atomicArray)
                   
    return atomicArray, transformationCenter, transformationContext, productElements,actionName,label
            

def createNode(atomicArray, chemical, chemicalDictionary, subgraph, nameHeader, builtList):
    '''
    creates a child node for 'chemical' on the 'subgraph' graph 
    '''
    clusterName = ''
    #if its not a complex
    if not str(chemical) in builtList:
        if '+' not in str(chemical) and len(atomicArray[chemical].molecules) == 1:
            clusterName = '%s_%s' % (nameHeader,atomicArray[chemical].molecules[0].name)
            chemicalDictionary.update(atomicArray[chemical].graphVizGraph(subgraph,clusterName))
        else:
            #if it's a complex
            if '+' not in str(chemical):
                clusterName = nameHeader
                chemicalDictionary.update(atomicArray[chemical].graphVizGraph(subgraph,nameHeader))
        
        if clusterName != '':
            clusterName = 'cluster%s_%s_m0_%s' % (clusterName, 
                                str(atomicArray[chemical]), 
                                atomicArray[chemical].molecules[0].idx)
        builtList[str(chemical)] = clusterName
        
        return clusterName
    else:
        return builtList[str(chemical)]

    
def createBiPartite(rules, transformations, fileName, reactionCenter=True, 
                    context=True, products=True):
    
    #extract reactioncenter, context information
    atomicArray, transformationCenter, transformationContext, productElements,actionNames,labelArray = \
                    extractTransformations(rules)
    #create the graph structure and the three main subgraphs
    
    if len(transformations) == 1:
        graph = pgv.AGraph(directed=True,strict=False,label=labelArray[transformations[0]-1]) 
    else:
        graph = pgv.AGraph(directed=True, strict=False)
    gReactants = graph.subgraph(name='clusterReactants', label='Chemicals')
    gRules = graph.subgraph(name='clusterRules', label='Transformations')
    gProducts = graph.subgraph(name='clusterProducts', label='Products')
    
    reactantDictionary = {}
    productDictionary = {}
    edgeIdx = 1
    reactantbuiltlist = {}
    productbuildlist ={}
    for idx,name in enumerate(actionNames):
        if idx+1 in transformations:
            gRules.add_node(name)
    #create reactionCenter nodes and edges
    if reactionCenter:
        for idx,element in enumerate(transformationCenter):
            if idx+1 in transformations:
                for reactant in element:
                    clusterName = createNode(atomicArray, reactant, 
                                             reactantDictionary, gReactants, 
                                             'clusterr',reactantbuiltlist)
                    graph.add_edge(clusterName, actionNames[idx], 
                                   key = 'key%i' % edgeIdx, dir='forward')
                    edgeIdx+=1
    #create context nodes
    if context:
        for idx,element in enumerate(transformationContext):
            if idx+1 in transformations:
                for reactant in element:
                    clusterName = createNode(atomicArray, reactant, 
                                             reactantDictionary, gReactants, 
                                             'clusterr',reactantbuiltlist)
                    if clusterName != '':
                        graph.add_edge(clusterName,actionNames[idx], 
                                       key = 'key%i' % edgeIdx, 
                                       dir='forward',color='red')
                        edgeIdx+=1  
                    else:
                        for atom in atomicArray[reactant]:
                            clusterName = createNode(atomicArray, str(atom), 
                                            reactantDictionary, gReactants, 
                                            'clusterr',reactantbuiltlist)
                            graph.add_edge(clusterName, actionNames[idx], 
                                           key = 'key%i' % edgeIdx, dir='forward', 
                                           color='red')
                            edgeIdx+=1
    #create product nodes
    if products:
        for idx,element in enumerate(productElements):
            if idx+1 in transformations:
                for product in element:
                    clusterName =createNode(atomicArray, product, 
                                            productDictionary, gProducts, 
                                            'clusterp', productbuildlist)
                    graph.add_edge(actionNames[idx], clusterName, 
                                   key = 'key%i' % edgeIdx, dir='foward')
                    edgeIdx+=1
    
    #output      
    print '%s.dot' % fileName
    graph.write('%s.dot' % fileName)
    graph = pgv.AGraph('%s.dot' % fileName)
    graph.layout(prog='fdp')
    graph.draw('%s.png' % fileName)

def main(fileName):
    _,rules = parseXML(fileName)
    #createBiPartite(rules,[x for x in range(1,25)],'simple', 
    #                       reactionCenter=True, context=True, products=True)
    
    for element in range(1,20):
        print element
        try:
            createBiPartite(rules, [element], 'simple%i' % element, 
                            reactionCenter=True, context=True, products=True)
            
        except:
            print 'xxx'
            continue
    
if __name__ == "__main__":
    main("output9.xml")
