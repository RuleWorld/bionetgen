# -*- coding: utf-8 -*-
"""
Created on Tue Apr  2 21:06:43 2013

@author: proto
"""

# -*- coding: utf-8 -*-
"""
Created on Tue Dec  6 17:42:31 2011

@author: proto
"""
from pyparsing import Word, Suppress, Optional, alphanums, Group, ZeroOrMore
from copy import deepcopy
import analyzeSBML
import analyzeRDF
import utils.structures as st
from utils.util import logMess
import biogrid
from copy import deepcopy
import marshal
import functools

def memoize(obj):
    cache = obj.cache = {}

    @functools.wraps(obj)
    def memoizer(*args, **kwargs):
        #key = str(args) + str(kwargs)
        key = marshal.dumps([args,kwargs])
        if key not in cache:
            cache[key] = obj(*args, **kwargs)
        return cache[key]
    return memoizer


@memoize
def parseReactions(reaction):
    name = Word(alphanums + '_-') + ':'

    species = (Word(alphanums + "_:#-")
    + Suppress('()')) + ZeroOrMore(Suppress('+') + Word(alphanums + "_:#-")
    + Suppress("()"))

    rate = Word(alphanums + "()")
    
    grammar = Suppress(Optional(name)) + ((Group(species) | '0') + Suppress(Optional("<") + "->") +
              (Group(species) | '0') + Suppress(rate)) \
              ^ (species + Suppress(Optional("<") + "->") + Suppress(rate))
    result = grammar.parseString(reaction).asList()
    if len(result) < 2:
        result = [result, []]
    if '<->' in reaction and len(result[0]) == 1 and len(result[1]) == 2:
        result2 = [result[1], result[0]]
        result = result2
    return result




def resolveDependencyGraph(dependencyGraph, reactant, withModifications=False):
    topCandidate =  resolveDependencyGraphHelper(dependencyGraph, reactant, [], withModifications)
    return topCandidate        

class CycleError(Exception):
    """Exception raised for errors in the input.

    Attributes:
        expr -- input expression in which the error occurred
        msg  -- explanation of the error
    """

    def __init__(self, memory):
        self.memory = memory

def resolveDependencyGraphHelper(dependencyGraph, reactant, memory,
                                 withModifications=False):
    result = []
    #if type(reactant) == tuple:
    #    return []
    if reactant not in dependencyGraph or dependencyGraph[reactant] == [] or \
        dependencyGraph[reactant] == [[reactant]]:
        if not withModifications:
            result.append([reactant])
    else:
        for option in dependencyGraph[reactant]:
            tmp = []

            for element in option:
                if element in memory and not withModifications:
                    result.append([element])
                    continue
                elif element in memory:
                    logMess('ERROR:Atomization','dependency cycle detected on {0}'.format(element))
                    raise CycleError(memory)
                baseElement = resolveDependencyGraphHelper(dependencyGraph,element, 
                                                           memory + [element], withModifications)
                if baseElement is not None:
                    tmp.extend(baseElement)
            #if not withModifications:
            result.extend(tmp)
            if len(option) == 1 and withModifications and option[0] != reactant:
                result.append((option[0], reactant))
    return result


def addToDependencyGraph(dependencyGraph, label, value):
    if label not in dependencyGraph:
        dependencyGraph[label] = []
    if value not in dependencyGraph[label] and value != []:
        dependencyGraph[label].append(value)


def bindingReactionsAnalysis(dependencyGraph, reaction, classification):
    '''
    adds addBond based reactions based dependencies to the dependency graph
    '''
    totalElements = [item for sublist in reaction for item in sublist]
    for element in totalElements:
        addToDependencyGraph(dependencyGraph, element, [])
        if classification == 'Binding':
            if len(reaction[1]) == 1 and element not in reaction[0]:
                addToDependencyGraph(dependencyGraph, element, reaction[0])
            elif len(reaction[0]) == 1 and element not in reaction[1]:
                addToDependencyGraph(dependencyGraph, element, reaction[1])

def weightDependencyGraph(dependencyGraph):
    def measureGraph(element,path):
        counter = 1
        for x in path:
            if type(x) == list or type(x) == tuple:
                counter += measureGraph(element,x)
            elif x != '0' and x!= element:
                counter += 1
        return counter

    weights = []
    for element in dependencyGraph:
        path = resolveDependencyGraph(dependencyGraph, element)
        try:
            path2 = resolveDependencyGraph(dependencyGraph, element,True)
        except CycleError:
            path2 = []
        weight = measureGraph(element,path) + len(path2)
        weights.append([element, weight])

    weights = sorted(weights, key=lambda rule: (rule[1],len(rule[0])))
    return weights



def lexicallyIdentifyModificationCandidate(reactant,tmpCandidate,equivalenceTranslator):
    '''
    '''
    pass
    
def consolidateDependencyGraph(dependencyGraph, equivalenceTranslator,
                               equivalenceDictionary,sbmlAnalyzer):
    equivalenceTranslator = {}
    def selectBestCandidate(reactant, candidates, dependencyGraph,sbmlAnalyzer,
                            equivalenceTranslator=equivalenceTranslator,equivalenceDictionary=equivalenceDictionary):
        tmpCandidates = []
        modifiedElements = []
        unevenElements = []
        for individualAnswer in candidates:
            tmpAnswer = []
            flag = True
            if len(individualAnswer) == 1 and individualAnswer[0] == reactant:
                continue
            for chemical in individualAnswer:
                #we cannot handle tuple naming conventions for now
                if type(chemical) == tuple:
                    flag = False
                    continue
                
                rootChemical = resolveDependencyGraph(dependencyGraph, chemical)
                mod = resolveDependencyGraph(dependencyGraph, chemical, True)
                if mod != []:
                    modifiedElements.extend(mod)
                for element in rootChemical:
                    if len(element) == 1 and type(element[0]) == tuple:
                        continue
                    if element == chemical:
                        tmpAnswer.append(chemical)
                    elif type(element) == tuple:
                        tmpAnswer.append(element)
                    else:
                        tmpAnswer.append(element[0])
            if flag:
                tmpAnswer = sorted(tmpAnswer)
                tmpCandidates.append(tmpAnswer)
        #we cannot handle tuple naming conventions for now
        if len(tmpCandidates) == 0:
            logMess('CRITICAL:Atomization','I dont know how to process these candidates and I have no way to make an educated guess. Politely refusing to translate {0}={1}.'.format(reactant,candidates))
            return None, None
        originalTmpCandidates = deepcopy(tmpCandidates)
        #if we have more than one modified element for a single reactant
        #we can try to  choose the one that is most similar to the original
        #reactant
        #FIXME:Fails if there is a double modification
        newModifiedElements = {}
        for element in modifiedElements:
            if element[0] not in newModifiedElements or element[1] == reactant:
                newModifiedElements[element[0]] = element[1]
        #check if all candidates are the same
        #print '...',tmpCandidates[0]
        if tmpCandidates[1:] == tmpCandidates[:-1] or len(tmpCandidates) == 1:
            flag = True
            while flag:
                flag = False
                for idx, chemical in enumerate(tmpCandidates[0]):
                    if chemical in newModifiedElements:
                        tmpCandidates[0][idx] = newModifiedElements[chemical]
                        flag = True
                        break
            #tmpCandidates[0] = candidates[0]

        else:
            
        #temporal solution for defaulting to the first alternative
            #print '---','error',reactant,newModifiedElements,tmpCandidates
            totalElements = [y for x in tmpCandidates for y in x]
            elementDict = {}
            for word in totalElements:
                if word not in elementDict:
                    elementDict[word] = 0
                elementDict[word] += 1
            newTmpCandidates = [[]]
            for element in elementDict:
                if elementDict[element] % len(tmpCandidates) == 0:
                    newTmpCandidates[0].append(element)
                #elif elementDict[element] % len(tmpCandidates) != 0 and re.search('(_|^){0}(_|$)'.format(element),reactant):
                #    newTmpCandidates[0].append(element)
                #    unevenElements.append([element])
                else:
                    logMess('WARNING:Atomization','Are these actually the same? {0}={1}.'.format(reactant,candidates))
                    unevenElements.append(element)
            flag = True
            #this should be done on newtmpCandidates instead of tmpcandidates
            while flag:
                flag = False
                for idx, chemical in enumerate(tmpCandidates[0]):
                    if chemical in newModifiedElements and newModifiedElements[chemical] in reactant:
                        tmpCandidates[0][idx] = newModifiedElements[chemical]
                        flag = True
                        break
                        
        #if all the candidates are about modification changes to a complex
        #then try to do it through lexical analysis
        if all([len(candidate)==1 for candidate in candidates]) and \
        candidates[0][0] != reactant and len(tmpCandidates[0]) > 1:
            if reactant != None:
                pass
            #analyze based on standard modifications

            lexCandidate,translationKeys,tmpequivalenceTranslator = sbmlAnalyzer.analyzeSpeciesModification(candidates[0][0],reactant,originalTmpCandidates[0])
            #FIXME: this is iffy. is it always an append modification? could be prepend

            if lexCandidate !=None:
                lexCandidate = tmpCandidates[0][originalTmpCandidates[0].index(lexCandidate)]
                lexCandidateModification = lexCandidate + translationKeys[0]
                for element in tmpequivalenceTranslator:
                    if element not in equivalenceTranslator:
                        equivalenceTranslator[element] = []
                equivalenceTranslator[element].append((lexCandidate,lexCandidateModification))

                dependencyGraph[lexCandidateModification] = [[lexCandidate]]
                while lexCandidate in tmpCandidates[0]:
                    tmpCandidates[0].remove(lexCandidate)            
                    tmpCandidates[0].append(lexCandidateModification)
                return [tmpCandidates[0]],unevenElements
        
            
            else:
                fuzzyCandidateMatch = None
                '''
                if nothing else works and we know the result is a bimolecular 
                complex and we know which are the basic reactants then try to
                do fuzzy string matching between the two.
                TODO: extend this to more than 2 molecule complexes. 
                '''
                if len(tmpCandidates[0]) == 2:
                    tmpmolecules = []
                    tmpmolecules.extend(originalTmpCandidates[0])
                    tmpmolecules.extend(tmpCandidates[0])
                    if reactant == 'Mdm2_P_Ub3':
                        pass
                    #FIXME: Fuzzy artificial reaction is using old methods. Try to fix this
                    #or maybe not, no one was using it and when it was used it was wrong
                    #fuzzyCandidateMatch = sbmlAnalyzer.fuzzyArtificialReaction(originalTmpCandidates[0],[reactant],tmpmolecules)
                    fuzzyCandidateMatch = None
                if fuzzyCandidateMatch !=None:
                    logMess('INFO:Atomization','Used fuzzy string matching from {0} to {1}'.format(reactant,fuzzyCandidateMatch))
                    return [fuzzyCandidateMatch],unevenElements
                else:
                    #last ditch attempt using straighforward lexical analysis
                    tmpDependency,tmpEquivalence = sbmlAnalyzer.findClosestModification([reactant],dependencyGraph.keys())
                    if reactant in tmpDependency and tmpDependency[reactant] in tmpCandidates[0]:
                        for element in tmpDependency:
                            if element not in dependencyGraph:
                                dependencyGraph[element] = tmpDependency[element]
                        for element in tmpEquivalence:
                            if element not in equivalenceDictionary:
                                equivalenceDictionary[element] = []
                            for equivalence in tmpEquivalence[element]:
                                if equivalence[0] not in equivalenceDictionary[element]:
                                    equivalenceDictionary[element].append(equivalence[0])
                        if len(tmpDependency.keys()) > 0:
                            return tmpDependency[reactant],unevenElements
                    #the ive no idea whats going on branch
                    modificationCandidates = {}            
                    if modificationCandidates == {}:
                        logMess('CRITICAL:Atomization','I dont know how this is modified and I have no way to make an educated guess. Politely refusing to translate {0}={1}.'.format(reactant,candidates))
                        return None,None
                    for idx, molecule in enumerate(tmpCandidates[0]):
                        if molecule in modificationCandidates:
                            tmpCandidates[0][idx] = modificationCandidates[molecule]
                    return [tmpCandidates[0]], unevenElements
        elif len(tmpCandidates) > 1:
            pass
            
        return [tmpCandidates[0]], unevenElements

    prunnedDependencyGraph = deepcopy(dependencyGraph)
    weights = weightDependencyGraph(prunnedDependencyGraph)
    unevenElementDict = {}
    for element in weights:
        candidates = [x for x in prunnedDependencyGraph[element[0]]]
        #print '-',candidates
        if len(candidates) == 1 and type(candidates[0][0]) == tuple:
            prunnedDependencyGraph[element[0]] = []
        if len(candidates) >= 1:
            try:
                candidates, uneven = selectBestCandidate(element[0], candidates, prunnedDependencyGraph,sbmlAnalyzer)
            except CycleError:
                
                candidates = None
                uneven = []
            if uneven != []:
                unevenElementDict[element[0]] = (uneven)
        if candidates is None:
            prunnedDependencyGraph[element[0]] = []
        else:
            prunnedDependencyGraph[element[0]] = candidates
    weights = weightDependencyGraph(prunnedDependencyGraph)
    return prunnedDependencyGraph, weights, unevenElementDict,equivalenceTranslator


def identifyReaction(equivalenceDictionary, originalElement, modifiedElement):
    for classification in equivalenceDictionary:
        if set([originalElement, modifiedElement]) in [set(x) for x in equivalenceDictionary[classification]]:
            return classification
    return None


def createEmptySpecies(name):
    species = st.Species()
    molecule = st.Molecule(name)
    species.addMolecule(molecule)
    return species


def addStateToComponent(species, moleculeName, componentName, state):
    for molecule in species.molecules:
        if moleculeName == molecule.name:
            for component in molecule.components:
                if componentName == component.name:
                    tmp = component.activeState
                    if state not in component.states:
                        component.addState(state)
                    elif state in component.states:
                        component.setActiveState(state)
                    return tmp


def addComponentToMolecule(species, moleculeName, componentName):
    for molecule in species.molecules:
        if moleculeName == molecule.name:
            if componentName not in [x.name for x in molecule.components]:
                component = st.Component(componentName)
                molecule.addComponent(component)
                return True
    return False


def addBondToComponent(species, moleculeName, componentName, bond, priority=1):
    order = 1
    for molecule in species.molecules:
        if moleculeName == molecule.name:
            if priority == order or len([x for x in species.molecules if x.name == moleculeName]) == order:
                for component in molecule.components:
                    if componentName == component.name:
                        # if we are adding a second bond to the same component
                        #it actually means that we have two components with the
                        #same name
                        if len(component.bonds) == 0:
                            component.addBond(bond)
                            return
                        else:
                            newComponent = st.Component(componentName)
                            newComponent.addBond(bond)
                            molecule.addComponent(newComponent)
                            return
            else:
                order += 1


def getComplexationComponents2(species,bioGridFlag):
    '''
    method used during the atomization process. It determines how molecules
    in a species bind together
    '''
    def getBiggestMolecule(array):
        sortedMolecule = sorted(array, key=lambda rule: (len(rule.components),len(str(rule)),str(rule)))
        #sortedMolecule = sorted(sortedMolecule, key=lambda rule: len(rule.components))
        
        return sortedMolecule[-1]
    def getNamedMolecule(array,name):
        
        for molecule in array:
            if molecule.name == name:
                return molecule
    speciesDict = {}
    #this array will contain all molecules that bind together
    pairedMolecules = []
    for x in sorted(species.molecules,key=lambda x:len(x.components),reverse=True):
        for y in x.components:
            if y.name not in speciesDict:
                speciesDict[y.name] = []
            speciesDict[y.name].append(x)
    #this array wil contain all molecules that dont bind to anything
    orphanedMolecules = [x for x in species.molecules]
    #determine how molecules bind together
    redundantBonds = []
    for x in sorted(species.molecules,key=lambda x:len(x.components),reverse=True):
        for component in [y for y in x.components if y.name.lower()
                          in speciesDict.keys()]:
            if x.name.lower() in speciesDict:
                if(x in speciesDict[component.name.lower()]) and component.name in [y.name.lower() for y
                    in speciesDict[x.name.lower()]]:
                    for mol in speciesDict[x.name.lower()]:
                        if mol.name.lower() == component.name and x != mol and x in \
                        speciesDict[component.name]:
                            speciesDict[x.name.lower()].remove(mol)
                            speciesDict[component.name].remove(x)
                            if x not in orphanedMolecules and mol not in orphanedMolecules:
                                #FIXME: is it necessary to remove double bonds in complexes?
                                redundantBonds.append([x,mol])
                                #continue
                            pairedMolecules.append([x, mol])

                            if x in orphanedMolecules:
                                orphanedMolecules.remove(x)
                            if mol in orphanedMolecules:
                                orphanedMolecules.remove(mol)
    #if len(redundantBonds) > 0:
    #    print [[x[0].name,x[1].name] for x in redundantBonds],str(species)                    
    totalComplex = [set(x) for x in pairedMolecules]
    isContinuousFlag = True
    #iterate over orphaned and find unidirectional interactions
    #e.g. if a molecule has a previous known interaction with the
    #same kind of molecule, even if it has no available components
    #e.g. k-mers`
 
    for element in speciesDict:
        for individualMolecule in speciesDict[element]:
            if individualMolecule in orphanedMolecules:
                candidatePartner = [x for x in species.molecules if x.name.lower() == element and x != individualMolecule]
                if len(candidatePartner) == 1:
                    pairedMolecules.append([candidatePartner[0],individualMolecule])
                    orphanedMolecules.remove(individualMolecule)
    #determine which pairs form a continuous chain

    while isContinuousFlag:
        isContinuousFlag = False
        for idx in range(0, len(totalComplex) - 1):
            for idx2 in range(idx + 1, len(totalComplex)):
                if len([x for x in totalComplex[idx] if x in totalComplex[idx2]]) > 0:
                    totalComplex[idx] = totalComplex[idx].union(totalComplex[idx2])
                    totalComplex.pop(idx2)
                    isContinuousFlag = True
                    break
            if isContinuousFlag:
                break
    #now we process those molecules where we need to create a new component
    for element in orphanedMolecules:

        for mol1 in species.molecules:
            #when adding orphaned molecules make sure it's not already in
            #the list
            if mol1 == element and mol1 not in set().union(*totalComplex):
                totalComplex.append(set([mol1]))
    #now we process for those molecules we are not sure how do they bind
    while len(totalComplex) > 1:
        
        if len(totalComplex[0]) ==1 and len(totalComplex[1]) == 1:
            mol1 = list(totalComplex[0])[0]
            mol2 = list(totalComplex[1])[0]
        else:
            names1 =  [str(x.name) for x in totalComplex[0]]
            names2 =  [str(x.name) for x in totalComplex[1]]
            
            dbPair = set([])
            if bioGridFlag:
                bioGridDict = biogrid.loadBioGridDict()
            else:
                bioGridDict = {}
            comb = set([])
            equivalence = {}
            comb = [(x,y) for x in names1 for y in names2]
            dbPair = set([])
            for element in comb:
                if element[0].upper() in bioGridDict and element[1] in bioGridDict[element[0].upper()] or \
                element[1].upper() in bioGridDict and element[0] in bioGridDict[element[1].upper()]:
                    logMess('INFO:Atomization','Biogrid info: {0}:{1}'.format(element[0],element[1]))
                    dbPair.add((element[0],element[1]))
            dbPair = list(dbPair)
            if dbPair != []:
                logMess('WARNING:Atomization',"More than one interaction was found in {0}".format(dbPair))
                mol1 = getNamedMolecule(totalComplex[0],dbPair[0][0])
                mol2 = getNamedMolecule(totalComplex[1],dbPair[0][1])
            else:
                logMess('WARNING:Atomization',"We don't know how {0} and {1} bind together and there's \
no relevant BioGrid information. Defaulting to largest molecule".format(
                [x.name for x in totalComplex[0]],[x.name for x in totalComplex[1]]))
                mol1 = getBiggestMolecule(totalComplex[0])
                mol2 = getBiggestMolecule(totalComplex[1])
        pairedMolecules.append([mol1, mol2])
        totalComplex[0] = totalComplex[0].union(totalComplex[1])
        totalComplex.pop(1)
    #totalComplex.extend(orphanedMolecules)
    return pairedMolecules


def getTrueTag(dependencyGraph, molecule):
    '''
    given any modified or basic element it returns its basic
    name
    '''
    if dependencyGraph[molecule] == []:
        return molecule
    elif dependencyGraph[molecule][0][0] == molecule:
        return molecule
    else:
        return getTrueTag(dependencyGraph, dependencyGraph[molecule][0][0])


def createCatalysisRBM(dependencyGraph,element,translator,reactionProperties,
                       equivalenceDictionary,sbmlAnalyzer,database):
    '''
    if it's a catalysis reaction create a new component/state
    '''

    if dependencyGraph[element[0]][0][0] == element[0]:
        if element[0] not in translator:
            translator[element[0]] = createEmptySpecies(element[0])
    else:                      
        componentStateArray = []
        tmp = element[0]
        existingComponents = []
        memory = []
        forceActivationSwitch = False
        while dependencyGraph[tmp] != []:
            #what kind of catalysis are we dealing with
            #classification = identifyReaction(
            #                                  equivalenceDictionary, 
            #                                  dependencyGraph[tmp][0][0],tmp)
            classification = sbmlAnalyzer.findMatchingModification(tmp,dependencyGraph[tmp][0][0])
            if not classification:
                classification = identifyReaction(
                                                  equivalenceDictionary, 
                                                  dependencyGraph[tmp][0][0],tmp)
            if not classification:
                classification = sbmlAnalyzer.findMatchingModification(element[0],dependencyGraph[tmp][0][0])
            if not classification:
                classification = identifyReaction(
                                                  equivalenceDictionary, 
                                                  dependencyGraph[tmp][0][0],element[0])

            #if we know what classification it is then add the corresponding components and states
            if classification is not None:
			    #and  reactionProperties[classification][0] not in existingComponents:
                componentStateArray.append(reactionProperties[classification])
                #classificationArray.append([classification,
                #                            tmp,dependencyGraph[tmp]
                #                            [0][0]])
                existingComponents.append(reactionProperties[
                classification][0])
            #if we don't know we can create a force 1:1 modification
            elif database.forceModificationFlag and classification is None and not forceActivationSwitch:
                forceActivationSwitch = True
                baseName = getTrueTag(dependencyGraph, 
                                        dependencyGraph[element[0]][0][0])
                
                species = createEmptySpecies(baseName)
                componentStateArray.append(['genericMod',tmp])
                logMess('ATOMIZATION:WARNING','adding forced transformation: {0}:{1}:{2}'.format(baseName,dependencyGraph[element[0]],element[0]))
                #return
            #bail out if we couldn't figure out what modification it is
            elif classification is None:    
                logMess('ATOMIZATION:CRITICAL','unregistered modification: {0}:{1}'.format(element[0],dependencyGraph[element[0]]))
            memory.append(tmp)
            tmp = dependencyGraph[tmp][0][0]
            if tmp in memory:
                raise CycleError
        baseName = getTrueTag(dependencyGraph, 
                                dependencyGraph[element[0]][0][0])

        species = createEmptySpecies(baseName)
        #use the already existing structure if its in the
        #translator,otherwise empty
        if baseName in translator:
             species = translator[baseName]
        #modifiedSpecies = deepcopy(translator[dependencyGraph[element[0]][0][0]])

        #modified species needs to start from the base speceis sine componentStateArray should contain the full set of modifications
        #check that this works correctly for double modifications
        modifiedSpecies = deepcopy(translator[baseName])
        for componentState in componentStateArray:                   

            #if classification[0] != None:
                addComponentToMolecule(species, baseName, componentState[0])
                addComponentToMolecule(modifiedSpecies,baseName,
                    componentState[0])
                tmp = addStateToComponent(species,baseName, 
                                    componentState[0], componentState[1])
                if tmp == componentState[1]:
                    addStateToComponent(species,baseName,componentState[0],
                            componentState[1]+componentState[1])
                if addStateToComponent(modifiedSpecies, baseName, 
                        componentState[0], componentState[1]) == componentState[1]:
                    addStateToComponent(modifiedSpecies,baseName,componentState[0],
                            componentState[1]+componentState[1])
                addStateToComponent(species, baseName, componentState[0], '0')
        #update the base species
        if len(componentStateArray) > 0:
            translator[baseName] = deepcopy(species)
            translator[element[0]] = modifiedSpecies
    
def createBindingRBM(element,translator,dependencyGraph,bioGridFlag):
    species = st.Species()
    #go over the sct and reuse existing stuff
    for molecule in dependencyGraph[element[0]][0]:
        if molecule in translator:
            tmpSpecies = translator[molecule]
            if molecule != getTrueTag(dependencyGraph, molecule):
                original = translator[getTrueTag(dependencyGraph, molecule)]
                updateSpecies(tmpSpecies, original.molecules[0])
            species.addMolecule(deepcopy(tmpSpecies.molecules[0]))
        else:
            mol = st.Molecule(molecule)
            dependencyGraph[molecule] = deepcopy(mol)
            species.addMolecule(mol)
    #how do things bind together?
    moleculePairsList = getComplexationComponents2(species,bioGridFlag)
        #TODO: update basic molecules with new components
        #translator[molecule[0].name].molecules[0].components.append(deepcopy(newComponent1))
        #translator[molecule[1].name].molecules[0].components.append(deepcopy(newComponent2))
    for idx, molecule in enumerate(moleculePairsList):
        flag = False
        #add bonds where binding components already exist
        for component in molecule[0].components:
            if component.name == molecule[1].name.lower() and \
            len(component.bonds) == 0:
                component.bonds.append(idx)
                flag = True
                break
        if not flag:
            #create components if they dont exist already.
            #Add a bond afterwards
            newComponent1 = st.Component(molecule[1].name.lower())

            molecule[0].components.append(newComponent1)
            
            if newComponent1.name not in [x.name for x in translator[molecule[0].name].molecules[0]. \
            components]:
                translator[molecule[0].name].molecules[0]. \
                components.append(deepcopy(newComponent1))

            molecule[0].components[-1].bonds.append(idx)
        flag = False
        #same thing for the other member of the bond
        for component in molecule[1].components:
            if component.name == molecule[0].name.lower() and len(component.bonds) == 0:
                component.bonds.append(idx)
                flag = True
                break
        if not flag:
            newComponent2 = st.Component(molecule[0].name.lower())
            molecule[1].components.append(newComponent2)
            if molecule[0].name != molecule[1].name:
                if newComponent2.name not in [x.name for x in translator[molecule[1].name].molecules[0]. \
                components]:
                    translator[molecule[1].name].molecules[0].components.append(
                    deepcopy(newComponent2))
            molecule[1].components[-1].bonds.append(idx)

    #update the translator
    translator[element[0]] = species

    
def atomize(dependencyGraph, weights, translator, reactionProperties,
            equivalenceDictionary,bioGridFlag,sbmlAnalyzer,database):
    '''
    The atomizer's main methods. Receives a dependency graph
    '''
    for idx,element in enumerate(weights):
        #0 molecule
        if element[0] == '0':
            continue
        #undivisible molecules
        if dependencyGraph[element[0]] == []:
            if element[0] not in translator:
                translator[element[0]] = createEmptySpecies(element[0])
        else:
            if len(dependencyGraph[element[0]][0]) == 1:
                #catalysis
                createCatalysisRBM(dependencyGraph,element,translator,reactionProperties,
                                       equivalenceDictionary,sbmlAnalyzer,database)
            else:
                createBindingRBM(element,translator,dependencyGraph,bioGridFlag)

def updateSpecies(species, referenceMolecule):
    flag = False
    for moleculeStructure in species.molecules:
        if moleculeStructure.name == referenceMolecule.name:
            for component in referenceMolecule.components:
                count = [x.name for x in referenceMolecule.components].count(component.name)
                count -= [x.name for x in moleculeStructure.components].count(component.name)
                newComponent = st.Component(component.name)
                if len(component.states) > 0:
                    newComponent.addState('0')
                if count > 0:
                    for _ in range(0, count):
                        moleculeStructure.addComponent(deepcopy(newComponent))
                elif count < 0:
                    for _ in range(0, -count):
                        referenceMolecule.addComponent(deepcopy(newComponent))
                        flag = True
                elif count == 0:
                    localComponents = [x for x in moleculeStructure.components if x.name ==component.name]
                    referenceComponents = [x for x in referenceMolecule.components if x.name == component.name]
                    if [x.states for x in localComponents]!=[x.states for x in referenceComponents]:
                        for lc in localComponents:
                            for rc in referenceComponents:
                                for ls in lc.states:
                                    if ls not in rc.states:
                                        rc.addState(ls,update=False)
                                for rs in rc.states:
                                    if rs not in lc.states:
                                        lc.addState(rs,update=False)
                                        
            for component in moleculeStructure.components:
                count = [x.name for x in referenceMolecule.components].count(component.name)
                count -= [x.name for x in moleculeStructure.components].count(component.name)
                newComponent = st.Component(component.name)
                if len(component.states) > 0:
                    newComponent.addState(component.states[0])
                    newComponent.addState('0')
                if count > 0:
                    for idx in range(0, count):
                        moleculeStructure.addComponent(deepcopy(newComponent))
                elif count < 0:
                    for idx in range(0, -count):
                        referenceMolecule.addComponent(deepcopy(newComponent))
                        flag = True

    return flag


def propagateChanges(translator, dependencyGraph):
    flag = True
    while flag:
        flag = False
        for dependency in dependencyGraph:
            if dependencyGraph[dependency] == []:
                continue
            for molecule in dependencyGraph[dependency][0]:
                try:
                    if updateSpecies(translator[dependency],
                                     translator[getTrueTag(dependencyGraph,
                                                          molecule)].molecules[0]):
                        flag = True
                except:
                    logMess('CRITICAL:Program',
                    'Species is not being properly propagated')
                    flag = False

#TODO:bm19:Rafi_Rasi_GTP
import cProfile, pstats, StringIO
import cPickle as pickle

def createSpeciesCompositionGraph(parser, database, configurationFile,namingConventions,
                       speciesEquivalences=None,bioGridFlag=False):
    _, rules, _ = parser.getReactions(atomize=True)
    molecules, _, _,_ = parser.getSpecies()
    database.sbmlAnalyzer = \
    analyzeSBML.SBMLAnalyzer(parser,configurationFile, namingConventions,speciesEquivalences,conservationOfMass=True)
    #classify reactions
    database.classifications, equivalenceTranslator, database.eequivalenceTranslator,\
    indirectEquivalenceTranslator, \
    adhocLabelDictionary,lexicalDependencyGraph=  database.sbmlAnalyzer.classifyReactions(rules, molecules)
    referenceVariables = [database.classifications,equivalenceTranslator,
                          database.eequivalenceTranslator,indirectEquivalenceTranslator,adhocLabelDictionary]
    comparisonVariables = [deepcopy(x) for x in referenceVariables]
    #####input processing
    #states,components,other user options
    #with open('temp1.dict','w') as f:
    #    pickle.dump(referenceVariables,f)
    #with open('temp2.dict','w') as f:
    #    pickle.dump(comparisonVariables,f)
    database.reactionProperties = database.sbmlAnalyzer.getReactionProperties()
    #user defined and lexical analysis naming conventions are stored here
    database.reactionProperties.update(adhocLabelDictionary)
    
    database.translator, database.labelDictionary, \
    database.lexicalLabelDictionary = database.sbmlAnalyzer.getUserDefinedComplexes()
    
    database.dependencyGraph = {}
    #analyzeSBML.analyzeNamingConventions(molecules)
    #rdfAnnotations = analyzeRDF.getAnnotations(parser,'uniprot')
    siteAnnotations = analyzeRDF.getAnnotations(parser,'interpro')
    ####dependency graph
    #binding reactions
    for reaction, classification in zip(rules, database.classifications):
        bindingReactionsAnalysis(database.dependencyGraph,
                        list(parseReactions(reaction)),classification)


    for element in lexicalDependencyGraph:
        database.dependencyGraph[element] = lexicalDependencyGraph[element]
        #Check if I'm using a molecule that hasn't been used yet
        for dependencyCandidate in database.dependencyGraph[element]:
            for molecule in [x for x in dependencyCandidate if x not in database.dependencyGraph]:
                database.dependencyGraph[molecule] = []

    #catalysis reactions
    for key in database.eequivalenceTranslator:
        for namingEquivalence in database.eequivalenceTranslator[key]:
            baseElement = min(namingEquivalence, key=len)
            modElement = max(namingEquivalence, key=len)
            if key != 'Binding':
                if baseElement not in database.dependencyGraph or database.dependencyGraph[baseElement] == []:
                    if modElement not in database.dependencyGraph or database.dependencyGraph[modElement] == []:
                        database.dependencyGraph[baseElement] = []
                    #do we have a meaningful reverse dependence?
                    #elif all([baseElement not in x for x in database.dependencyGraph[modElement]]):
                    #    addToDependencyGraph(database.dependencyGraph,baseElement,[modElement])
                    #    continue
                addToDependencyGraph(database.dependencyGraph, modElement,
                                     [baseElement])
    for element in database.labelDictionary:
        if database.labelDictionary[element] == 0:
            continue
        elif len(database.labelDictionary[element][0]) == 0 or element == \
        database.labelDictionary[element][0][0]:
            addToDependencyGraph(database.dependencyGraph, element, [])
        else:
            database.dependencyGraph[element] = [list(
            database.labelDictionary[element][0])]
                                     
    #non lexical-analysis catalysis reactions
    if database.forceModificationFlag:
        for reaction, classification in zip(rules, database.classifications):
            preaction = list(parseReactions(reaction))
            if len(preaction[0]) == 1 and len(preaction[1]) == 1:
                if (preaction[0][0] in [0,'0']) or (preaction[1][0] in [0,'0']):
                    continue
                if preaction[1][0] in preaction[0][0]:
                    base = preaction[1][0]
                    mod = preaction[0][0]
                else:
                    mod = preaction[1][0]
                    base = preaction[0][0]
                if database.dependencyGraph[mod] == []:
                    if base in database.labelDictionary and \
                        database.labelDictionary[base]== 0:
                        continue
                    if mod in database.labelDictionary and \
                        database.labelDictionary[mod] == 0:
                        continue
                    if [mod] in database.dependencyGraph[base]:
                        continue
                    database.dependencyGraph[mod]  = [[base]]
    


    '''
    #complex catalysis reactions
    for key in indirectEquivalenceTranslator:
        #first remove these entries from the dependencyGraph since 
        #they are not true bindingReactions
        for namingEquivalence in indirectEquivalenceTranslator[key]:
            removedElement = ''
            tmp3 = deepcopy(namingEquivalence[1])
            if tmp3 in database.dependencyGraph[namingEquivalence[0][0]]:
                removedElement = namingEquivalence[0][0]
            elif tmp3 in database.dependencyGraph[namingEquivalence[0][1]]:
                removedElement = namingEquivalence[0][1]

            else:
                tmp3.reverse()
                if tmp3 in database.dependencyGraph[namingEquivalence[0][0]]:
                    removedElement = namingEquivalence[0][0]

                elif tmp3 in database.dependencyGraph[namingEquivalence[0][1]]:
                    removedElement = namingEquivalence[0][1]
            
            
            #then add the new, true dependencies
            #if its not supposed to be a basic element
            tmp = [x for x in namingEquivalence[1] if x not in namingEquivalence[2]]
            tmp.extend([x for x in namingEquivalence[2] if x not in namingEquivalence[1]])
            tmp2 = deepcopy(tmp)
            tmp2.reverse()
            
            
            ##TODO: map back for the elements in namingEquivalence[2]
            if tmp not in database.dependencyGraph[namingEquivalence[3][0]] \
                and tmp2 not in database.dependencyGraph[namingEquivalence[3][0]]:
                if sorted(tmp) == sorted(tmp3):
                    continue
                if all(x in database.dependencyGraph for x in tmp):
                    if removedElement in database.dependencyGraph:
                        database.dependencyGraph[removedElement].remove(tmp3)
                    logMess('INFO:Atomization','Removing {0}={1} and adding {2}={3} instead\
 from the dependency list since we determined it is not a true binding reaction based on lexical analysis'\
                    .format(removedElement,tmp3,namingEquivalence[3][0],tmp))
                    database.dependencyGraph[namingEquivalence[3][0]] = [tmp]
                else:
                    logMess('WARNING:Atomization','We determined that {0}={1} based on lexical analysis instead of \
{2}={3} (stoichiometry) but one of the constituent components in {1} is not a molecule so no action was taken'.format(namingEquivalence[3][0],
tmp,removedElement,tmp3))
    #user defined stuff
'''
    


    #stuff obtained from string similarity analysis
    for element in database.lexicalLabelDictionary:
        #similarity analysis has less priority than anything we discovered
        #before
        if element in database.dependencyGraph and \
        len(database.dependencyGraph[element]) > 0:
            continue

        if len(database.lexicalLabelDictionary[element][0]) == 0 or element == \
        database.lexicalLabelDictionary[element][0][0]:
            addToDependencyGraph(database.dependencyGraph, element, [])
        else:
            logMess('INFO:Atomization','added induced speciesStructure {0}={1}'\
            .format(element,database.lexicalLabelDictionary[element][0]))
            database.dependencyGraph[element] = [list(
            database.lexicalLabelDictionary[element][0])]
    
    #pure lexical analysis
    
    
    orphanedSpecies = [x for x in database.dependencyGraph if database.dependencyGraph[x] == []]
    strippedMolecules = [x.strip('()') for x in molecules]
    tmpDependency,database.tmpEquivalence = database.sbmlAnalyzer.findClosestModification(orphanedSpecies,strippedMolecules)          
    for species in tmpDependency:
        if tmpDependency[species] == []:
            addToDependencyGraph(database.dependencyGraph,species,[])
        for instance in tmpDependency[species]:
            addToDependencyGraph(database.dependencyGraph,species,instance)
    #####sct
    #FIXME: wtf was unevenelementdict supposed to be for
    
    prunnedDependencyGraph, database.weights, unevenElementDict,database.artificialEquivalenceTranslator = \
    consolidateDependencyGraph(database.dependencyGraph, equivalenceTranslator,database.eequivalenceTranslator,database.sbmlAnalyzer)
    return prunnedDependencyGraph,database
    
    
def sanityCheck(translator):
    '''
    checks for critical atomization errors like isomorphism
    '''
    stringrep = {x:str(translator[x]) for x in translator}
    repeats = set()
    for key in range(0,len(translator.keys())-1):
        for key2 in range(key+1,len(translator.keys())):
            if stringrep[translator.keys()[key]] == stringrep[translator.keys()[key2]]:
                repeats.add((translator.keys()[key],translator.keys()[key2]))
    for repeat in repeats:
        logMess('CRITICAL:Atomization','Elements {0} and {1} produce\
            the same translation. Emptying {1}.'.format(repeat[0],repeat[1]))
        if max(repeat) in translator:
            translator.pop(max(repeat))

def transformMolecules(parser, database, configurationFile,namingConventions,
                       speciesEquivalences=None,bioGridFlag=False):
    """
    main method. Receives a parser configuration, a configurationFile and a
    list of user defined species equivalences and returns a dictionary
    containing an atomized version of the model
    Keywords:
        ---parser: data structure containing the reactions and species we will use
        ---database:data structure containing the result of the outgoing translation
        ---configurationFile
        ---speciesEquivalences:predefined species
    """
    '''
    pr = cProfile.Profile()
    pr.enable()
    '''
    prunnedDependencyGraph,database = createSpeciesCompositionGraph(parser, database, configurationFile,namingConventions,
                       speciesEquivalences=speciesEquivalences,bioGridFlag=bioGridFlag)    

    
    for element in database.artificialEquivalenceTranslator:
        if element not in database.eequivalenceTranslator:
            database.eequivalenceTranslator[element] = []
        database.eequivalenceTranslator[element].extend(database.artificialEquivalenceTranslator[element])

    #special handling for double modifications like double phosporylation
    #FIXME: this needs to be done in a cleaner way(e.g. getting them 
    # from a file instead of being hardcoded)
    doubleModifications = {"Double-Phosporylation":"Phosporylation"}

    
    for element in doubleModifications:
        if doubleModifications[element] not in database.eequivalenceTranslator:
            continue
        if element not in database.eequivalenceTranslator:
            database.eequivalenceTranslator[element] = []
        
        baseElements = [x[0] for x in database.eequivalenceTranslator[doubleModifications[element]]]
        modifiedElements = [x[1] for x in database.eequivalenceTranslator[doubleModifications[element]]]
        
        #deleteEquivalences = [baseElements.index(x) for x in baseElements if x in modifiedElements]
        
        deleteEquivalences = [(x,modifiedElements[baseElements.index(x)]) for x in baseElements if x in modifiedElements]
        
        for eq in deleteEquivalences:
            if eq not in database.eequivalenceTranslator[element]:
                database.eequivalenceTranslator[element].append(eq)
                
        for eq in deleteEquivalences:
            
            if eq in database.eequivalenceTranslator[doubleModifications[element]]:
                database.eequivalenceTranslator[doubleModifications[element]].remove(eq)
    for modification in database.tmpEquivalence:
        for candidates in database.tmpEquivalence[modification]:
            for instance in candidates:
                addToDependencyGraph(database.eequivalenceTranslator,modification,instance)
                
    database.weights = sorted(database.weights, key=lambda rule: (rule[1],len(rule[0])))
    atomize(prunnedDependencyGraph, database.weights, database.translator, database.reactionProperties, 
                                                                database.eequivalenceTranslator,
                                                                bioGridFlag,database.sbmlAnalyzer,database)
    
    onlySynDec =  len([x for x in database.classifications if x not in ['Generation','Decay']]) == 0
    propagateChanges(database.translator, prunnedDependencyGraph)
    
    
    sanityCheck(database.translator)
    '''
    pr.disable()
    s = StringIO.StringIO()
    sortby = 'cumulative'
    ps = pstats.Stats(pr, stream=s).sort_stats(sortby)
    ps.print_stats(10)
    print s.getvalue()
    '''
    return database.translator,onlySynDec