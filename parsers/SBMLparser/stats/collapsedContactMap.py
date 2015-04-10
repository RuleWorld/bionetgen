from readBNGXML import parseXML
import networkx as nx
import argparse

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



def extractSingleTransformation(rule):
    '''
    goes through a rule and extracts its reactioncenter,context and product
    atomic patterns per transformation action
    '''
    atomicArray = {}    
    transformationCenter = []
    transformationContext = []
    productElements = []
    actionName = []
    index=0
    label = []
    act = rule.actions
    react = rule.reactants
    mapp = rule.mapping
    product = rule.products
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


def createCollapsedContact(rules,species,transformations,fileName):
    '''
    creates a collapsed bipartite graph given a list of rules
    collapsed bipartite means that molecules are represented as structureless nodes and 

    Args:
        rules: The list of rules to process
        transformations: which transformations do we want to process (their index)
        fileName: output filename
        reactioncenter,context,products: booleans indicating whether we desire include the reactioncenter,context
    or product in our ouput file
    '''


    graph = nx.DiGraph()
    speciesNodes =[]
    processNodes = []
    for speciesUnit in species:
        graph.add_node(speciesUnit.name,graphics={'type':'roundrectangle','fill':'#FFCC00'})
        speciesNodes.append(speciesUnit.name)
    mainidx = 0

    for rule,_,_,_ in rules:
        mainidx += 1
        nonatomicset = False
        activeReactants = [y.name for x in rule.reactants for y in x.molecules]
        activeProducts = [y.name for x in rule.products for y in x.molecules] 

        atomicArray, transformationCenter, transformationContext, productElements,actionName,label =  extractSingleTransformation(rule)

        for idx in xrange(len(rule.actions)):
            if rule.actions[idx].action in ['Add','Delete']:
                nonatomicset = True
            elif rule.actions[idx].action in ['AddBond']:

                bondpartners = [x.split('(')[0] for x in transformationCenter[idx]]
                if len(bondpartners) == 2:
                    graph.add_edge(bondpartners[0],bondpartners[1],graphics={'fill':"#000000"})
                else:
                    graph.add_edge(bondpartners[0],bondpartners[0],graphics={'fill':"#000000"})
                for x in bondpartners:
                    if x in activeReactants:
                        activeReactants.remove(x)
                    if x in activeProducts:
                        activeProducts.remove(x)
            elif rule.actions[idx].action in ['StateChange']:
                molecule = [x.split('(')[0] for x in transformationCenter[idx]]
                state = [x.split('(')[1].split('~')[0] for x in transformationCenter[idx]]
                graph.add_node(molecule[0] +'_'+state[0],graphics={'type': "circle",'fill':"#CCFFCC"},LabelGraphics={'text':state[0]})
                processNodes.append(state[0])
                graph.add_edge(molecule[0]+'_'+state[0],molecule[0],graphics={'fill':"#000000"})
                if molecule[0] in activeReactants:
                    activeReactants.remove(molecule[0])
                if molecule[0] in activeProducts:
                    activeProducts.remove(molecule[0])
        if nonatomicset:
            graph.add_node(mainidx,graphics={'type': "hexagon" })
            processNodes.append(mainidx)
            for element in activeReactants:
                if element not in graph.node:
                    graph.add_node(element,graphics={'type':'roundrectangle','fill':'#FFCC00'})
                graph.add_edge(element,mainidx,graphics={'targetArrow': "standard"})
            for element in activeProducts:
                graph.add_edge(mainidx,element)
    print fileName
    nx.write_gml(graph,fileName)
    #graph.write('%s.dot' % fileName)
    #graph = pgv.AGraph('%s.dot' % fileName)
    #graph.layout(prog='fdp')
    #graph.draw('%s.png' % fileName)
    #subprocess.call(['dot', '-Tsvg', '{0}.dot'.format(fileName),'-o{0}.svg'.format(fileName)])


def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i','--input',type=str,help='settings file',required=True)
    parser.add_argument('-o','--output',type=str,help='output directory')
    return parser    


def main(fileName,outputfilename):
    molecules,rules,_ = parseXML(fileName)
    #print '---',rules
    #createBiPartite(rules,None,'simple', 
    #                       reactionCenter=True, context=True, products=True)
    #              
    createCollapsedContact(rules,molecules,[1],outputfilename)         

if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    inputFile = namespace.input
    if namespace.output != None:
        outputFile = namespace.output
    else:
        outputFile = inputFile + '.gml'
    main(inputFile,outputFile)
    #addAnnotations('fceri_ji')