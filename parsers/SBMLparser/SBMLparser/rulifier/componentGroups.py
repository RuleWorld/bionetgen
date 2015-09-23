from utils import readBNGXML
import argparse
from collections import defaultdict
from cStringIO import StringIO
from utils import extractAtomic
from copy import deepcopy
import networkx as nx

def molecule2stateTuples(molecule):
    """
    Receives a molecule structure, returns a tuple detailing the state of the contained states
    """
    tupleList = []
    for component in molecule.components:
        componentDefinition = [component.name]
        componentDefinition.append(1 if len(component.bonds) > 0 else 0)
        componentDefinition.append('' if len(component.states) == 0 else component.activeState)
        tupleList.append(tuple(componentDefinition))
    return tupleList


def extractCenterContext(rules, excludeReverse=False):
    transformationCenter = []
    transformationContext = []
    transformationProduct = []
    atomicArray = []
    actionNames = []
    label = []
    for idx, rule in enumerate(rules):
        if excludeReverse and rule[0].label.endswith('_reverse_'):
            continue
        label.append(rule[0].label)
        tatomicArray, ttransformationCenter, ttransformationContext, \
            tproductElements, tactionNames, tlabelArray = extractAtomic.extractTransformations(
                [rule], True)
        #if label[-1] == 'v9':
        #    print str(rule[0])
        #    print tproductElements
        #    print ttransformationContext

        transformationCenter.append(ttransformationCenter)
        transformationContext.append(ttransformationContext)

        actionNames.append(tactionNames)
        atomicArray.append(tatomicArray)
        transformationProduct.append(tproductElements)
    return label,transformationCenter, transformationContext, \
        transformationProduct, atomicArray, actionNames


def askQuestions(inputfile, molecule, center, context=None):
    _, rules, _ = readBNGXML.parseXML(inputfile)

    transformationCenter = []
    transformationContext = []
    transformationProduct = []
    atomicArray = []
    actionNames = []

    ruleArray = []
    contextArray = []
    for idx, rule in enumerate(rules):
        tatomicArray, ttransformationCenter, ttransformationContext, \
            tproductElements, tactionNames, tlabelArray = extractAtomic.extractTransformations(
                [rule], True)

        if any([molecule in y and center in y for x in ttransformationCenter for y in x]):
            if context:
                ruleArray.append(str(rule[0]).split(':')[0])
                contextArray.append([y for x in ttransformationContext for y in x if context in y and molecule in y])
            else:
                print rule
    return ruleArray,contextArray


def getChemicalStates(rules):
    """
    goes through a list of rules and creates a dictionary (molecules) of possible chemical states of its components
    """
    chemicalStates = defaultdict(list)
    for rule in rules:
        for chemicalSet in [rule[0].reactants, rule[0].products]:
            for chemical in chemicalSet:
                for molecule in chemical.molecules:
                    stateTuple = molecule2stateTuples(molecule)
                    #if ('grb2',1,'') in stateTuple and molecule.name == 'EGFR' and ('shc',1,'') in stateTuple:
                    #    print '---',str(rule[0])
                    chemicalStates[molecule.name].append(stateTuple)

    return chemicalStates


def patternsFromString(chemical):
    patterndict = defaultdict(list)
    chemicalString = str(chemical)
    molecules = chemicalString.split('.')
    for molecule in molecules:
        partitions = molecule.split('(')
        if '!' in partitions[1]:
            componentName = partitions[1].split('!')[0]
            patterndict[partitions[0]].append((componentName, 1, ''))
        elif '~' in partitions[1]:
            componentName = partitions[1].split('~')[0]
            stateName = partitions[1].split('~')[1][:-1]
            patterndict[partitions[0]].append((componentName, 0, stateName))
        else:
            componentName = partitions[1][:-1]
            patterndict[partitions[0]].append((componentName, 0, ''))
    return patterndict


def getRestrictedChemicalStates(labelArray, products, contexts):
    """
    goes through a list of reaction center and their associated context and creates a dictionary of molecules
    of possible chemical states
    """

    #sortedChemicalStates = defaultdict(lambda: defaultdict(lambda: defaultdict(set)))
    sortedChemicalStates = defaultdict(lambda: defaultdict(lambda: defaultdict(lambda: defaultdict(list))))
    counter = 1
    for label, product, context in zip(labelArray,products, contexts):
        for indvproduct, indvcontext in zip(product, context):
            pDict = defaultdict(list)
            cDict = defaultdict(list)
            for p in indvproduct:
                result = patternsFromString(p)
                for molecule in result:
                    for pattern in result[molecule]:
                        pDict[molecule].append(pattern)
            for c in indvcontext:
                result = patternsFromString(c)
                for molecule in result:
                    for pattern in result[molecule]:
                        cDict[molecule].append(pattern)
            for molecule in pDict:
                for componentState in pDict[molecule]:

                    # FIXME: This is to account for dimers where or places where there is more than one components with the same name. Truly this should be enother kind of classification
                    for componentState2 in [x for x in cDict[molecule] if x[0] != componentState[0]]:
                        sortedChemicalStates[molecule][componentState][componentState2[0]][componentState2[1:]].append(label)
                        #sortedChemicalStates[molecule][componentState][componentState2[0]].add(componentState2[1:])
    return sortedChemicalStates


def sortChemicalStates(chemicalStates):
    """
    sorts a dictionary of chemicalStates into a dictionary of molecules of chemicalStates
    of their associated-same-molecule-components (and their states)
    """
    sortedChemicalStates = defaultdict(lambda: defaultdict(lambda: defaultdict(set)))
    for molecule in chemicalStates:
        for stateInstance in chemicalStates[molecule]:
            for componentState in stateInstance:
                for componentState2 in stateInstance:
                    if componentState == componentState2:
                        continue
                    sortedChemicalStates[molecule][componentState][componentState2[0]].add(componentState2[1:])
    return sortedChemicalStates


def isActive(state):
    if state[0] == 1 or state[1] not in ['', '0']:
        return True
    return False


def componentStateSize(molecules, moleculeName, componentName):
    for molecule in molecules:
        if molecule.name == moleculeName:
            for component in molecule.components:
                if component.name == componentName:
                    return max(2, len(component.states))


def analyzeDependencies(componentStateCollection, state, moleculeName, molecules, dependencies):
    for componentName in componentStateCollection:
        stateSize = componentStateSize(molecules, moleculeName, componentName)

        if stateSize == len(componentStateCollection[componentName]):
            dependencies[moleculeName]['independent'].add((state, componentName))
            #print moleculeName,state,componentName,componentStateCollection[componentName]
        elif len(componentStateCollection[componentName]) == 1:
            activeState = list(componentStateCollection[componentName])[0]
            if isActive((state[1], state[2])) and isActive(activeState):
                dependencies[moleculeName]['requirement'].add(((componentName, activeState[0], activeState[1]), state))
            elif isActive((state[1], state[2])) and not isActive(activeState):
                dependencies[moleculeName]['nullrequirement'].add((((componentName, activeState[0], activeState[1])), state))
            #elif (not isActive((state[1], state[2]))) and isActive(activeState):
            #    dependencies[moleculeName]['nullrequirement'].add(((state, (componentName, activeState[0], activeState[1]))))


def detectDependencies(stateDictionary, molecules):
    dependencies = defaultdict(lambda: defaultdict(set))
    preprocessing = defaultdict(lambda :defaultdict(dict))
    #preprocess for dimer information
    for moleculeName in stateDictionary:
        parsedMoleculeName = moleculeName.split('%')[0]
        #parsedMoleculeName = moleculeName
        for state in stateDictionary[moleculeName]:
            preprocessing[parsedMoleculeName][state].update(stateDictionary[moleculeName][state])
    for moleculeName in preprocessing:
        for state in preprocessing[moleculeName]:
            analyzeDependencies(preprocessing[moleculeName][state], state, moleculeName, molecules, dependencies)
    return dependencies

from collections import Counter


def reverseState(moleculeName, state, molecules):
    for molecule in molecules:
        if molecule.name == moleculeName:
            for component in molecule.components:
                if component.name == state[0]:
                    if len(component.states) <= 1:
                        if isActive(state[1:]):
                            return (state[0], 0, '')
                        else:
                            return (state[0], 1, '')
                    else:
                        if isActive(state[1:]):
                            return (state[0], 0, '0')
                        else:
                            return (state[0], 0, [x for x in component.states if x != '0'][0])


def getMutualExclusions(stateDictionary, molecules):
    for molecule in stateDictionary:
        stateDictionary[molecule]['exclusion'] = [x for x in stateDictionary[molecule]['nullrequirement'] if
                                                  (reverseState(molecule, x[1], molecules), reverseState(molecule, x[0], molecules))
                                                  in stateDictionary[molecule]['nullrequirement']]
        stateDictionary[molecule]['nullrequirement'] = [x for x in stateDictionary[molecule]['nullrequirement'] if x not in stateDictionary[molecule]['exclusion']]


def getMotifRelationships(stateDictionary, molecules):
    motifDictionary = defaultdict(lambda: defaultdict(list))
    for molecule in stateDictionary:
        motifDictionary[molecule]['exclusion'] = set([tuple(sorted([x[0][0], x[1][0]])) for x in stateDictionary[molecule]['nullrequirement'] if
                                                  (reverseState(molecule, x[1], molecules), reverseState(molecule, x[0], molecules))
                                                  in stateDictionary[molecule]['nullrequirement']])
        motifDictionary[molecule]['ordering'] = [[x[0][0], x[1][0]] for x in stateDictionary[molecule]['requirement'] if
                                                    (reverseState(molecule,x[1],molecules), x[0])
                                                    in stateDictionary[molecule]['nullrequirement']]
        motifDictionary[molecule]['fullIndependence'] = set([tuple(sorted([x[0][0],x[1]])) for x in stateDictionary[molecule]['independent'] if isActive(x[0][1:]) and
                                                         any(x[1] == y[0][0] and x[0][0] == y[1] and isActive(y[0][1:]) for y in stateDictionary[molecule]['independent'])])

        motifDictionary[molecule]['partialIndependence+'] = set([tuple([x[0][0], x[1]]) for x in stateDictionary[molecule]['independent'] if isActive(x[0][1:]) and
                                                         any(x[1] in y[1] and x[0][0] in y[0] for y in stateDictionary[molecule]['requirement'])])

        motifDictionary[molecule]['partialIndependence-'] = set([tuple([x[0][0], x[1]]) for x in stateDictionary[molecule]['independent'] if isActive(x[0][1:]) and
                                                         any(x[1] in y[1] and x[0][0] in y[0] for y in stateDictionary[molecule]['nullrequirement'])])

    
    return motifDictionary
        #stateDictionary[molecule]['nullrequirement'] = [x for x in stateDictionary[molecule]['nullrequirement'] if x not in stateDictionary[molecule]['exclusion']]


def removeIndirectDependencies(dependencies, stateSpace):
    """
    goes through the list of dependencies and search for those states that requires more than 1 condition.
    If true, it will check if any of the set of prerequirements is a sufficient conditions for any of the other prerequirements
    to be valid. If true it will remove those redundant prerequirements them
    """
    indirect = defaultdict(set)
    for molecule in dependencies:
        for requirement in set([x[1] for x in dependencies[molecule]['requirement']]):
            prerequirements = [x[0] for x in dependencies[molecule]['requirement'] if requirement == x[1]]
            # if a particular state has more than one requirement

            if len(prerequirements) > 1:
                candidateMolecules = [x for x in stateSpace.keys() if molecule in x]
                relevantStateSpace = defaultdict(lambda: defaultdict(set))
                for candidate in candidateMolecules:
                    for x in prerequirements:
                        relevantStateSpace[x].update(stateSpace[candidate][x])  #= {x: stateSpace[molecule][x] for x in prerequirements}
                #print relevantStateSpace
                for rsp in relevantStateSpace:

                    for partner in relevantStateSpace[rsp]:
                        partnerDefinition = list(relevantStateSpace[rsp][partner])
                        # is any single prerequirement a sufficient condition for any other other ones?
                        if len(partnerDefinition) == 1 and (partner, partnerDefinition[0][0], partnerDefinition[0][1]) in prerequirements:
                            # if so mark for deletion
                            indirect[molecule].add(((partner, partnerDefinition[0][0], partnerDefinition[0][1]), requirement))
    for molecule in indirect:
        for indirectDependency in indirect[molecule]:
            dependencies[molecule]['requirement'].remove(indirectDependency)


def formatComponent(state, tense):
    if state[0] == 1:
        if tense == 'past':
            compStr = 'be bound'
        else:
            compStr = 'bind'
    else:
        compStr = 'be in state {0}'.format(state[1])
    return compStr


def printDependencyLog(dependencies):
    log = StringIO()
    for molecule in dependencies:
        for requirementType in dependencies[molecule]:
            #if requirementType in ['independent']:
            #    continue
            for baseMolecule in dependencies[molecule][requirementType]:
                if requirementType == 'requirement':
                    log.write('Molecule {0} needs component {1} to {2} for component {3} to {4}\n'.format(molecule, baseMolecule[0][0],
                              formatComponent(baseMolecule[0][1:], 'past'), baseMolecule[1][0],
                              formatComponent(baseMolecule[1][1:], 'present')))
                elif requirementType == 'exclusion':
                    log.write('In molecule {0} component {1} is mutually exclusive of component {2}\n'.format(molecule, baseMolecule[0][0],
                                                                                                              baseMolecule[1][0]))
                if requirementType == 'nullrequirement':
                    pass
                #if requirementType == 'independent':
                #    log.write('The setting of {0} to {1} in molecule {3} is independent from {2}\n'.format(baseMolecule[0][0],baseMolecule[0][1],baseMolecule[1],molecule))
    return log.getvalue()


def removeCounter(requirementDependencies):
    finalDependencies = defaultdict(lambda: defaultdict(set))
    for requirement in requirementDependencies:
        req2 = requirement.split('%')[0]
        for dependencies in requirementDependencies[requirement]:
            finalDependencies[req2][dependencies] = finalDependencies[req2][dependencies].union(requirementDependencies[requirement][dependencies])
    return finalDependencies

def getExclusionClusters(requirementDependencies):
    graphExclusionCliques = {x:[] for x in requirementDependencies.keys()}
    for molecule in requirementDependencies.keys():
        g = nx.Graph()
        gmod = nx.Graph()
        for exclusionRelationship in requirementDependencies[molecule]['exclusion']:
            if not exclusionRelationship[0].lower().endswith('mod') and not exclusionRelationship[1].lower().endswith('mod'):
                g.add_edge(exclusionRelationship[0], exclusionRelationship[1])
            elif exclusionRelationship[0].lower().endswith('mod') and exclusionRelationship[1].lower().endswith('mod'):
                gmod.add_edge(exclusionRelationship[0], exclusionRelationship[1])
        for graph in [g,gmod]:
            if(graph.nodes()):
                while len(graph.nodes()) > 1:
                    cliques = sorted(list(nx.find_cliques(graph)), key=len, reverse=True)
                    if len(cliques[0]) > 1:
                        graphExclusionCliques[molecule].append(cliques[0])
                    else:
                        break
                    #graphExclusionCliques[molecule] = cliques
                    for node in cliques[0]:
                        graph.remove_node(node)
    return graphExclusionCliques    
    #for exclusionRequirement in requirementDependencies:


def getContextRequirements(inputfile, collapse=True, motifFlag=False):
    """
    Receives a BNG-XML file and returns the contextual dependencies implied by this file
    """
    molecules, rules, _ = readBNGXML.parseXML(inputfile)
    label, center, context, product, atomicArray, actions = extractCenterContext(rules,excludeReverse=True)
    reactionCenterStateDictionary = getRestrictedChemicalStates(label, product, context)
    #print reactionCenterStateDictionary['Ras%0'][('sos',1,'')]['Ras_GDPmod']
    #print '--'
    #print reactionCenterStateDictionary['Ras%0'][('Ras_GDPmod',0,'Ras_GDP')]['sos']
    backupstatedictionary = deepcopy(reactionCenterStateDictionary)
    #print reactionCenterStateDictionary['EGFR%1'][('_Pmod',0,'_P')]
    #print reactionCenterStateDictionary['EGFR%0'][('_Pmod',0,'_P')]
    #return
    #chemicalStates = getChemicalStates(rules)
    #totalStateDictionary = sortChemicalStates(chemicalStates)
    requirementDependencies = detectDependencies(reactionCenterStateDictionary, molecules)

    #print '000'
    #print [x for x in requirementDependencies['EGFR']['nullrequirement'] if 'egf' in x[0][0] or 'egf' == x[1][0]]
    if collapse:
        removeIndirectDependencies(requirementDependencies, backupstatedictionary)

    if motifFlag:
        requirementDependencies = getMotifRelationships(requirementDependencies, molecules)
        exclusionCliques = getExclusionClusters(requirementDependencies)
    else:
        getMutualExclusions(requirementDependencies, molecules)
        exclusionCliques = {}
    
    #requirementDependencies = removeCounter(requirementDependencies)\
    #raise Exception
    
    return requirementDependencies, backupstatedictionary, exclusionCliques



def reverseContextDict(dependencies):
    """
    returns a molecule/component pair/relationship hierarchy
    @param a molecule/relationship/component pair hierarchy
    """
    reverseDependencies = defaultdict(lambda: defaultdict(str))

    for molecule in dependencies:
        for dependencyType in dependencies[molecule]:
            if dependencyType == 'independent':
                for relationship in dependencies[molecule][dependencyType]:
                    if relationship[0][1] == 1 or relationship[0][2] not in ['0', 0, '']:
                        reverseDependencies[molecule][(relationship[0][0], relationship[1])] = 'independent'
            else:
                for relationship in dependencies[molecule][dependencyType]:

                    if dependencyType == 'exclusion':
                        reverseDependencies[molecule][(relationship[1][0], relationship[0][0])] = 'nullrequirement'
                        reverseDependencies[molecule][(relationship[0][0], relationship[1][0])] = 'nullrequirement'
                    else:
                        reverseDependencies[molecule][(relationship[1][0], relationship[0][0])] = dependencyType
    return reverseDependencies

def defineConsole():
    """
    defines the program console line commands
    """
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i', '--input', type=str, help='settings file', required=True)
    return parser

if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    inputFile = namespace.input
    #print askQuestions(inputFile, 'EGFR', 'shc','grb2')
    dependencies, backup, _ = getContextRequirements(inputFile, collapse=True, motifFlag=True)
    
    #print dependencies
    #print(dict(dependencies['EGFR']))
    #print backup
    #print printDependencyLog(dependencies)
    