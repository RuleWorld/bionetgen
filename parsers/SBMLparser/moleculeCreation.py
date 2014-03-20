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
import structures as st
from util import logMess
import biogrid
def parseReactions(reaction):
    species = (Word(alphanums + "_:#-")
    + Suppress('()')) + ZeroOrMore(Suppress('+') + Word(alphanums + "_:#-")
    + Suppress("()"))

    rate = Word(alphanums + "()")
    grammar = ((Group(species) | '0') + Suppress(Optional("<") + "->") +
              (Group(species) | '0') + Suppress(rate)) \
              ^ (species + Suppress(Optional("<") + "->") + Suppress(rate))
    result = grammar.parseString(reaction).asList()
    if len(result) < 2:
        result = [result, []]
    if '<->' in reaction and len(result[0]) == 1 and len(result[1]) == 2:
        result2 = [result[1], result[0]]
        result = result2
    return result


class Memoize:
    def __init__(self, function):
        self.function = function
        self.memo = {}

    def __call__(self, *args):
        if not args in self.memo:
            self.memo[args] = self.function(*args)
        return self.memo[args]


def resolveDependencyGraph(dependencyGraph, reactant, withModifications=False):
    return resolveDependencyGraphHelper(dependencyGraph, reactant, [], withModifications)


def resolveDependencyGraphHelper(dependencyGraph, reactant, memory,
                                 withModifications=False):
    result = []
    #if type(reactant) == tuple:
    #    return []
    if reactant not in dependencyGraph or dependencyGraph[reactant] == []:
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
                    logMess('ERROR','dependency cycle detected on {0}'.format(element))
                    print 'Detected cycle', element
                    return []
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


def dependencyGraph(dependencyGraph, reaction, classification):
    '''
    adds addBond based reactions based dependencies to the dependency graph
    '''
    totalElements = [item for sublist in reaction for item in sublist]
    for element in totalElements:
        addToDependencyGraph(dependencyGraph, element, [])
        if classification == 'Binding':
            if len(reaction[0]) == 2 and element not in reaction[0]:
                addToDependencyGraph(dependencyGraph, element, reaction[0])
            elif len(reaction[1]) == 2 and element not in reaction[1]:
                addToDependencyGraph(dependencyGraph, element, reaction[1])


def weightDependencyGraph(dependencyGraph):
    def measureGraph(path):
        counter = 1
        for x in path:
            if type(x) == list or type(x) == tuple:
                counter += measureGraph(x)
            elif x != '0':
                counter += 1
        return counter

    weights = []
    for element in dependencyGraph:

        path = resolveDependencyGraph(dependencyGraph, element)
        weight = measureGraph(path)
        weights.append([element, weight])

    weights = sorted(weights, key=lambda rule: rule[1])
    return weights



def lexicallyIdentifyModificationCandidate(reactant,tmpCandidate,equivalenceTranslator):
    '''
        
    '''
    
def consolidateDependencyGraph(dependencyGraph, equivalenceTranslator):
    def selectBestCandidate(reactant, candidates, dependencyGraph):
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
            return None, None

        #if we have more than one modified element for a single reactant
        #we can try to choose the one that is most similar to the original
        #reactant
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
            #print newTmpCandidates,unevenElements
        #print ';;;',tmpCandidates[0]
        if len(candidates) == 1 and len(candidates[0]) == 1 and \
        candidates[0][0] != reactant and len(tmpCandidates[0]) > 1:
            candidates = []
            modificationCandidates = {x[0]: x[1] for x in equivalenceTranslator
            if x[0] in tmpCandidates[0] and type(x[1]) is not tuple}
            if modificationCandidates == {}:
                logMess('WARNING','I dont know how this is modified and I have no way to make an educated guess. Politely refusing to translate {0}.'.format(reactant))
                tmpCandidates[0] = [reactant]
            for idx, molecule in enumerate(tmpCandidates[0]):
                if molecule in modificationCandidates:
                    tmpCandidates[0][idx] = modificationCandidates[molecule]
            return [tmpCandidates[0]], unevenElements

        return [tmpCandidates[0]], unevenElements

    prunnedDependencyGraph = deepcopy(dependencyGraph)
    weights = weightDependencyGraph(prunnedDependencyGraph)
    unevenElementDict = {}
    for element in weights:
        #print element
        candidates = [x for x in prunnedDependencyGraph[element[0]]]
        #print '-',candidates
        if len(candidates) == 1 and type(candidates[0][0]) == tuple:
            prunnedDependencyGraph[element[0]] = []
        if len(candidates) >= 1:
            candidates, uneven = selectBestCandidate(element[0], candidates, prunnedDependencyGraph)
            if uneven != []:
                unevenElementDict[element[0]] = (uneven)
        if candidates is None:
            prunnedDependencyGraph[element[0]] = []
        else:
            prunnedDependencyGraph[element[0]] = candidates
    weights = weightDependencyGraph(prunnedDependencyGraph)
    return prunnedDependencyGraph, weights, unevenElementDict


def identifyReaction(equivalenceDictionary, originalElement, modifiedElement):
    for classification in equivalenceDictionary:
        if (originalElement, modifiedElement) in equivalenceDictionary[classification]:
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
                if componentName == component.name and state not in component.states:
                    component.addState(state)
                elif state in component.states:
                    component.setActiveState(state)


def addComponentToMolecule(species, moleculeName, componentName):
    for molecule in species.molecules:
        if moleculeName == molecule.name:
            if componentName not in [x.name for x in molecule.components]:
                component = st.Component(componentName)
                molecule.addComponent(component)


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
        sortedMolecule = sorted(array, key=lambda rule: len(str(rule)))
        sortedMolecule = sorted(sortedMolecule, key=lambda rule: len(rule.components))
        #print 'sortedMolecule',[str(x) for x in sortedMolecule]
        return sortedMolecule[-1]
    def getNamedMolecule(array,name):
        
        for molecule in array:
            if molecule.name == name:
                return molecule
        
    speciesDict = {}
    #this array will contain all molecules that bind together
    pairedMolecules = []
    for x in species.molecules:
        for y in x.components:
            if y.name not in speciesDict:
                speciesDict[y.name] = []
            speciesDict[y.name].append(x)
    #this array wil contain all molecules that dont bind to anything
    orphanedMolecules = [x for x in species.molecules]
    #determine how molecules bind together
    for x in species.molecules:
        for component in [y for y in x.components if y.name.lower()
                          in speciesDict.keys()]:
            if x.name.lower() in speciesDict:
                if(x in speciesDict[component.name.lower()]) and component.name in [y.name.lower() for y
                    in speciesDict[x.name.lower()]]:
                    for mol in speciesDict[x.name.lower()]:
                        if mol.name.lower() == component.name and x != mol and x in \
                        speciesDict[component.name]:
                            pairedMolecules.append([x, mol])
                            speciesDict[x.name.lower()].remove(mol)
                            speciesDict[component.name].remove(x)
                            if x in orphanedMolecules:
                                orphanedMolecules.remove(x)
                            if mol in orphanedMolecules:
                                orphanedMolecules.remove(mol)
    totalComplex = [set(x) for x in pairedMolecules]
    isContinuousFlag = True
    #iterate over orphaned and find unidirectional interactions
    #e.g. if a molecule has a previous known interaction with the
    #same kind of molecule, even if it has no available components
    #e.g. k-mers`
    for element in speciesDict:
        for individualMolecule in speciesDict[element]:
            if individualMolecule in orphanedMolecules:
                candidatePartner = [x for x in species.molecules if x.name.lower() == element]
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
                    dbPair.add((element[0],element[1]))
            dbPair = list(dbPair)
            if dbPair != []:
                logMess('WARNING',"More than one interaction was find in {0}".format(dbPair))
                mol1 = getNamedMolecule(totalComplex[0],dbPair[0][0])
                mol2 = getNamedMolecule(totalComplex[1],dbPair[0][1])
            else:
                logMess('WARNING',"We don't know how {0} and {1} bind together and there's \
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


def atomize(dependencyGraph, weights, translator, reactionProperties,
            equivalenceDictionary,bioGridFlag):
    '''
    The atomizer's main methods. Receives a dependency graph
    '''
    for element in weights:
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
                if dependencyGraph[element[0]][0][0] == element[0]:
                    if element[0] not in translator:
                        translator[element[0]] = createEmptySpecies(element[0])
                else:                      
                    classificationArray = []
                    tmp = element[0]
                    existingComponents = []
                    while dependencyGraph[tmp] != []:
                        #what kind of catalysis are we dealing with
                        classification = identifyReaction(
                                                          equivalenceDictionary, 
                                                          dependencyGraph[tmp][0][0],tmp)
                        if classification is not None and \
                        reactionProperties[classification][0] not in existingComponents:
                            classificationArray.append([classification,
                                                        tmp,dependencyGraph[tmp]
                                                        [0][0]])
                            existingComponents.append(reactionProperties[
                            classification][0])
                        tmp = dependencyGraph[tmp][0][0]
                    species = createEmptySpecies(getTrueTag(
                    dependencyGraph, dependencyGraph[element[0]][0][0]))
                    #use the already existing structure if its in the
                    #translator,otherwise empty
                    if (getTrueTag(dependencyGraph, dependencyGraph[element[0]][0][0])) in translator:
                         species = translator[(getTrueTag(dependencyGraph, 
                                                          dependencyGraph[element[0]][0][0]))]
                    modifiedSpecies = deepcopy(species) 
                    for classification in classificationArray:                   
                        if classification[0] != None:
                            addComponentToMolecule(species, (
                            getTrueTag(dependencyGraph, 
                                       dependencyGraph[element[0]][0][0])),
reactionProperties[classification[0]][0])
                            addComponentToMolecule(modifiedSpecies, (getTrueTag(dependencyGraph,
                                                                    dependencyGraph[element[0]][0][0])),
reactionProperties[classification[0]][0])
                            addStateToComponent(species,(getTrueTag(dependencyGraph, dependencyGraph[element[0]][0][0])), reactionProperties[classification[0]][0], reactionProperties[classification[0]][1])
                            addStateToComponent(modifiedSpecies, (getTrueTag(dependencyGraph, dependencyGraph[element[0]][0][0])), reactionProperties[classification[0]][0], reactionProperties[classification[0]][1])
                            addStateToComponent(species, (getTrueTag(dependencyGraph, dependencyGraph[element[0]][0][0])), reactionProperties[classification[0]][0], 'U')
                            
                    #update the base species
                    if len(classificationArray) > 0 and classificationArray[0][0] is not None:
                        translator[(getTrueTag(dependencyGraph, dependencyGraph[element[0]][0][0]))] = \
                        deepcopy(species)
                        translator[element[0]] = modifiedSpecies
                    else:
                        print 'ALERT', element[0], str(modifiedSpecies), str(species)
                        #print equivalenceDictionary
            else:
                #binding
                #print '---',dependencyGraph[element[0]],element
                '''
                if element[0] not in database:
                    species = st.Species()
                else:
                    species = database[element[0]]
                '''
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
                            translator[molecule[1].name].molecules[0].components.append(
                            deepcopy(newComponent2))
                        molecule[1].components[-1].bonds.append(idx)

                #update the translator
                translator[element[0]] = species


def updateSpecies(species, referenceMolecule):
    flag = False
    #print '--',str(species),str(referenceMolecule)
    for moleculeStructure in species.molecules:
        if moleculeStructure.name == referenceMolecule.name:
            for component in referenceMolecule.components:
                count = [x.name for x in referenceMolecule.components].count(component.name)
                count -= [x.name for x in moleculeStructure.components].count(component.name)
                newComponent = st.Component(component.name)
                if len(component.states) > 0:
                    newComponent.addState('U')
                if count > 0:
                    for _ in range(0, count):
                        moleculeStructure.addComponent(deepcopy(newComponent))
                elif count < 0:
                    for _ in range(0, -count):
                        referenceMolecule.addComponent(deepcopy(newComponent))
                        flag = True
            for component in moleculeStructure.components:
                count = [x.name for x in referenceMolecule.components].count(component.name)
                count -= [x.name for x in moleculeStructure.components].count(component.name)
                newComponent = st.Component(component.name)
                if len(component.states) > 0:
                    newComponent.addState(component.states[0])
                    newComponent.addState('U')
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
                if updateSpecies(translator[dependency],
                                 translator[getTrueTag(dependencyGraph,
                                                      molecule)].molecules[0]):
                    flag = True

#TODO:bm19:Rafi_Rasi_GTP
def transformMolecules(parser, database, configurationFile,namingConventions,
                       speciesEquivalences=None,bioGridFlag=False):
    '''
    main method. Receives a parser configuration, a configurationFile and a
    list of user defined species equivalences and returns a dictionary
    containing an atomized version of the model
    Keywords:
        ---parser: data structure containing the reactions and species we will use
        ---database:data structure containing the result of the outgoing translation
        ---configurationFile
        ---speciesEquivalences:predefined species
    '''
    
    _, rules, _ = parser.getReactions()
    molecules, _, _,_ = parser.getSpecies()

    sbmlAnalyzer = \
    analyzeSBML.SBMLAnalyzer(configurationFile, namingConventions,speciesEquivalences)
    #classify reactions
    classifications, equivalenceTranslator, eequivalenceTranslator,\
    indirectEquivalenceTranslator=  sbmlAnalyzer.classifyReactions(rules, molecules)
    #####input processing
    #states,components,other user options
    database.reactionProperties = sbmlAnalyzer.getReactionProperties()
    database.translator, database.labelDictionary = sbmlAnalyzer.getUserDefinedComplexes()
    database.dependencyGraph = {}
    #analyzeSBML.analyzeNamingConventions(molecules)
    rdfAnnotations = analyzeRDF.getAnnotations(parser,'uniprot')

    ####dependency graph
    print '---',indirectEquivalenceTranslator
    #binding reactions
    for reaction, classification in zip(rules, classifications):
        dependencyGraph(database.dependencyGraph,
                        list(parseReactions(reaction)),classification)
    
    #catalysis reactions
    for key in eequivalenceTranslator:
        for namingEquivalence in eequivalenceTranslator[key]:
            baseElement = min(namingEquivalence, key=len)
            modElement = max(namingEquivalence, key=len)
            if key != 'Binding':
                if baseElement not in database.dependencyGraph or database.dependencyGraph[baseElement] == []:
                    if modElement not in database.dependencyGraph or database.dependencyGraph[modElement] == []:
                        database.dependencyGraph[baseElement] = []
                    elif [baseElement] not in database.dependencyGraph[modElement]:
                        addToDependencyGraph(database.dependencyGraph,baseElement,[modElement])
                        continue
                addToDependencyGraph(database.dependencyGraph, modElement,
                                     [baseElement])
    
    #complex catalysis reactions
    for key in indirectEquivalenceTranslator:
        #first remove these entries from the dependencyGraph since 
        #they are not true bindingReactions
        for namingEquivalence in indirectEquivalenceTranslator[key]:
            tmp = deepcopy(namingEquivalence[1])
            if tmp in database.dependencyGraph[namingEquivalence[0][0]]:
                database.dependencyGraph[namingEquivalence[0][0]].remove(tmp)
            elif tmp in database.dependencyGraph[namingEquivalence[0][1]]:
                database.dependencyGraph[namingEquivalence[0][1]].remove(tmp)
            else:
                tmp.reverse()
                if tmp in database.dependencyGraph[namingEquivalence[0][0]]:
                    database.dependencyGraph[namingEquivalence[0][0]].remove(tmp)
                elif tmp in database.dependencyGraph[namingEquivalence[0][1]]:
                    database.dependencyGraph[namingEquivalence[0][1]].remove(tmp)
            #then add the new, true dependencies
            #if its not supposed to be a basic element
            tmp = [x for x in namingEquivalence[1] if x not in namingEquivalence[2]]
            tmp.extend([x for x in namingEquivalence[2] if x not in namingEquivalence[1]])
            tmp2 = deepcopy(tmp)
            tmp2.reverse()
            
            ##TODO: map back for the elements in namingEquivalence[2]
            if tmp not in database.dependencyGraph[namingEquivalence[3][0]] \
                and tmp2 not in database.dependencyGraph[namingEquivalence[3][0]]:
                if all(x in database.dependencyGraph for x in tmp):
                    database.dependencyGraph[namingEquivalence[3][0]] = [tmp]
    #user defined stuff
    for element in database.labelDictionary:
        if len(database.labelDictionary[element][0]) == 0 or element == \
        database.labelDictionary[element][0][0]:
            addToDependencyGraph(database.dependencyGraph, element, [])
        else:
            database.dependencyGraph[element] = [list(
            database.labelDictionary[element][0])]
    #####sct
    prunnedDependencyGraph, weights, unevenElementDict = \
    consolidateDependencyGraph(database.dependencyGraph, equivalenceTranslator)
    #FIXME: I'm conatminating these data structures somewhere. In here
    #im just calling the original generator to recover them.
    classifications, equivalenceTranslator, eequivalenceTranslator, \
        indirectEquivalenceTranslator = sbmlAnalyzer.classifyReactions(rules,molecules)
        
    weights = sorted(weights, key=lambda rule: rule[1])
    #print {x:str(database.translator[x]) for x in database.translator}
    atomize(prunnedDependencyGraph, weights, database.translator, database.reactionProperties, 
                                                                eequivalenceTranslator,bioGridFlag)
    propagateChanges(database.translator, prunnedDependencyGraph)
    return database.translator