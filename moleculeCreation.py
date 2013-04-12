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
from pyparsing import Word, Suppress, Optional, alphanums, Group,ZeroOrMore
from numpy import sort
from copy import deepcopy
import reactionTransformations
import analyzeSBML
import analyzeRDF
from util import logMess
import re
import structures as st



def parseReactions(reaction):
    species =   (Word(alphanums+"_:#-") 
    + Suppress('()')) + ZeroOrMore(Suppress('+') + Word(alphanums+"_:#-") 
    + Suppress("()"))
    '''
    species = Optional(Suppress(Word(nums+"*"))) +  Optional(Word(alphanums+"_") + Suppress('()')) +  \
    Optional(Suppress('+') + Word(alphanums+"_") + Suppress("()")) + \
    Optional(Suppress('+') + Word(alphanums+"_") + Suppress("()")) + Optional(Suppress('+') + Word(alphanums+"_") 
    + Suppress("()"))
    '''
    rate = Word(alphanums + "()")
    grammar = ((Group(species) | '0') + Suppress(Optional("<") + "->") + (Group(species) | '0') + Suppress(rate)) \
    ^ (species + Suppress(Optional("<") + "->") + Suppress(rate))  
    result =  grammar.parseString(reaction).asList()
    if len(result) < 2:
        result = [result,[]]
    if '<->' in reaction and len(result[0]) == 1 and len(result[1]) == 2:
        result2 = [result[1],result[0]]
        result = result2
    return result


    


class Memoize:
    def __init__(self, f):
        self.f = f
        self.memo = {}
    def __call__(self, *args):
        if not args in self.memo:
            self.memo[args] = self.f(*args)
        return self.memo[args]

    
def resolveDependencyGraph(dependencyGraph,reactant,withModifications=False):
    return resolveDependencyGraphHelper(dependencyGraph,reactant,[],withModifications)


def resolveDependencyGraphHelper(dependencyGraph,reactant,memory,withModifications=False):
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
                    continue
                baseElement = resolveDependencyGraphHelper(dependencyGraph,element,memory + [element],withModifications)
                
                if baseElement != None:
                    tmp.extend(baseElement)
            #if not withModifications:
            result.extend(tmp)
            if len(option) == 1 and withModifications and option[0] != reactant:
                result.append((option[0],reactant))
    return result
                
def addToDependencyGraph(dependencyGraph,label,value):
    if label not in dependencyGraph:
        dependencyGraph[label] = []
    if value not in dependencyGraph[label] and value != []:
        dependencyGraph[label].append(value)
        
def dependencyGraph(dependencyGraph,reaction,classification,equivalenceTranslator):
    totalElements =  [item for sublist in reaction for item in sublist]
    for element in totalElements:
        addToDependencyGraph(dependencyGraph,element,[])
        if classification == 'Binding':
            if len(reaction[0]) == 2 and element not in reaction[0]:
                addToDependencyGraph(dependencyGraph,element,reaction[0])
            elif len(reaction[1]) == 2 and element not in reaction[1]:
                addToDependencyGraph(dependencyGraph,element,reaction[1])


    
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
        path = resolveDependencyGraph(dependencyGraph,element)
        weight = measureGraph(path)
        weights.append([element,weight])
        
    weights = sorted(weights,key=lambda rule: rule[1]) 
    return weights

def consolidateDependencyGraph(dependencyGraph):
    def selectBestCandidate(reactant,candidates,dependencyGraph):
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
                rootChemical = resolveDependencyGraph(dependencyGraph,chemical)
                
                mod = resolveDependencyGraph(dependencyGraph,chemical,True)
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
            return None
 
        #if we have more than one modified element for a single reactant
        #we can try to choose the one that is most similar to the original
        #reactant
        newModifiedElements = {}
        for element in modifiedElements:
            if element[0] not in newModifiedElements or element[1] == reactant:
                newModifiedElements[element[0]] = element[1]
        #check if all candidates are the same
        #print '...',tmpCandidates[0]
        if reactant == 'EGF_EGFR2':
            print 'dollah'
        if tmpCandidates[1:] == tmpCandidates[:-1] or len(tmpCandidates) == 1:
            flag = True
            while flag:
                flag = False
                for idx,chemical in enumerate(tmpCandidates[0]):
                    if chemical in newModifiedElements:
                        tmpCandidates[0][idx] = newModifiedElements[chemical]
                        flag = True
                        break
            #tmpCandidates[0] = candidates[0]
            
        else:
        #temporal solution for defaulting to the first alternative
            print '---','error',reactant,newModifiedElements,tmpCandidates
            totalElements = [y for x in tmpCandidates for y in x]
            elementDict = {}
            namingConvention = []
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
            
            while flag:
                flag = False
                for idx,chemical in enumerate(newTmpCandidates[0]):
                    if chemical in newModifiedElements and newModifiedElements[chemical] in reactant:
                        newTmpCandidates[0][idx] = newModifiedElements[chemical]
                        flag = True
                        break
            #print newTmpCandidates,unevenElements
        #print ';;;',tmpCandidates[0]
        if len(candidates) == 1 and len(candidates[0]) == 1 and candidates[0][0] != reactant and len(tmpCandidates[0]) > 1:
            print '+++','error I dont know how this is modified',candidates[0],reactant
            return [],[]


        return [tmpCandidates[0]],unevenElements
       
    tmp = {}
    prunnedDependencyGraph = deepcopy(dependencyGraph)
    weights = weightDependencyGraph(prunnedDependencyGraph)
    unevenElementDict = {}
    for element in weights:
        #print element
        candidates = [x for x in prunnedDependencyGraph[element[0]]]
        #print '-',candidates
        if len(candidates) == 1 and type(candidates[0][0])  == tuple:
            prunnedDependencyGraph[element[0]] = []
        if len(candidates) >= 1:
            candidates,uneven = selectBestCandidate(element[0],candidates,prunnedDependencyGraph)
            if uneven != []:            
                unevenElementDict[element[0]] = (uneven)
        if candidates == None:
            prunnedDependencyGraph[element[0]] = []
        else:
            prunnedDependencyGraph[element[0]] = candidates
            
    return prunnedDependencyGraph,weights,unevenElementDict
    
def identifyReaction(equivalenceDictionary,originalElement,modifiedElement):
    print originalElement,modifiedElement
    for classification in equivalenceDictionary:
        if (originalElement,modifiedElement) in equivalenceDictionary[classification]:
            return classification
    return None


def createEmptySpecies(name):
    species = st.Species()
    molecule = st.Molecule(name)
    species.addMolecule(molecule)
    return species

def addStateToComponent(species,moleculeName,componentName,state):
    for molecule in species.molecules:
        if moleculeName == molecule.name:
            for component in molecule.components:
                if componentName == component.name and state not in component.states:
                    component.addState(state)
                elif state in component.states:
                    component.setActiveState(state)
    
def addComponentToMolecule(species,moleculeName,componentName):
    for molecule in species.molecules:
        if moleculeName == molecule.name:
            if componentName not in [x.name for x in molecule.components]:
                component = st.Component(componentName)
                molecule.addComponent(component)

def addBondToComponent(species,moleculeName,componentName,bond,priority = 1):
    order = 1
    for molecule in species.molecules:
        if moleculeName == molecule.name:
            if priority == order or len([x for x in species.molecules if x.name==moleculeName]) == order:
                for component in molecule.components:
                    if componentName == component.name:
                            # if we are adding a second bond to the same component
                            #it actually means that we have two components with the same
                            #name
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

def getComplexationComponents2(species):
    def getBiggestMolecule(array):
        sortedMolecules = sorted(array,key=lambda rule: len(rule.components))
        sortedMolecule = sorted(array,key=lambda rule: len(str(rule)))
        return sortedMolecule[-1]
    speciesDict = {}
    #this array will contain all molecules that bind together
    pairedMolecules = []
    for x in species.molecules:
        for y in x.components:
            if y.name not in speciesDict:
                speciesDict[y.name] = []
            speciesDict[y.name].append(x)
    orphanedMolecules = [x.name.lower() for x in species.molecules]
    for x in species.molecules:
        for component in [y for y in x.components if y.name.lower() in speciesDict.keys()]:
            if x.name.lower() in speciesDict:
                if(x in speciesDict[component.name.lower()]) and \
                    component.name in [y.name.lower() for y in speciesDict[x.name.lower()]] \
                    and x != speciesDict[x.name.lower()][0]:
                    for mol in speciesDict[x.name.lower()]:
                        if mol.name.lower() == component.name:
                            pairedMolecules.append([x,mol])
                            speciesDict[x.name.lower()].remove(mol)
                            speciesDict[component.name].remove(x)
                            if x.name.lower() in orphanedMolecules:
                                orphanedMolecules.remove(x.name.lower())
                            if component.name in orphanedMolecules:
                                orphanedMolecules.remove(component.name)
    totalComplex = [set(x) for x in pairedMolecules]
 
    flag = True
    while flag:
        flag = False
        for idx in range(0,len(totalComplex)-1):
            for idx2 in range(idx+1,len(totalComplex)):
                if len([x for x in totalComplex[idx] if x in totalComplex[idx2]]) > 0:
                    totalComplex[idx] = totalComplex[idx].union(totalComplex[idx2])
                    totalComplex.pop(idx2)
                    flag = True
                    break
            if flag:
                break
    #now we process those molecules where we need to create a new component
    newComponentPairs = []
    for element in orphanedMolecules:
        for mol1 in species.molecules:
            if mol1.name.lower() == element:
                totalComplex.append(set([mol1]))
    while len(totalComplex) > 1:
        mol1=  getBiggestMolecule(totalComplex[0])
        mol2 =  getBiggestMolecule(totalComplex[1])
        newComponentPairs.append([mol1,mol2])
        pairedMolecules.append([mol1,mol2])
        totalComplex[0] = totalComplex[0].union(totalComplex[1])
        totalComplex.pop(1)
    #totalComplex.extend(orphanedMolecules)
    return newComponentPairs,pairedMolecules
        #    pass        
        
    
           
def getTrueTag(dependencyGraph,molecule):
    if dependencyGraph[molecule] == []:
        return molecule
    elif dependencyGraph[molecule][0][0] == molecule:
        return molecule
    else:
        return getTrueTag(dependencyGraph,dependencyGraph[molecule][0][0])

def preRuleifyReactions(dependencyGraph,weights,translator,reactionProperties,equivalenceDictionary):
    for element in weights:
        if element[0] == '0':
            continue
        if dependencyGraph[element[0]] == []:
            if element[0] not in translator:
                translator[element[0]] = createEmptySpecies(element[0])
        else:
            if len(dependencyGraph[element[0]][0]) == 1:
                if dependencyGraph[element[0]][0][0] == element[0]:
                    if element[0] not in translator:
                        translator[element[0]] = createEmptySpecies(element[0])
                else:                      
                    classification = identifyReaction(equivalenceDictionary,dependencyGraph[element[0]][0][0],element[0])
                    species = createEmptySpecies(dependencyGraph[element[0]][0][0])
                    if classification != None:
                        if dependencyGraph[element[0]][0][0] in translator:
                            species = translator[dependencyGraph[element[0]][0][0]]

                        addComponentToMolecule(species,dependencyGraph[element[0]][0][0],reactionProperties[classification][0])
                        addStateToComponent(species,dependencyGraph[element[0]][0][0],reactionProperties[classification][0],reactionProperties[classification][1])
                        modifiedSpecies = deepcopy(species) 
                        addStateToComponent(species,dependencyGraph[element[0]][0][0],reactionProperties[classification][0],'U')
                        
                        #update the base species
                        print ';;;;;;;:::::::::;;;;',getTrueTag(dependencyGraph,element[0]),dependencyGraph[element[0]][0][0],str(species)
                        translator[dependencyGraph[element[0]][0][0]] = deepcopy(species)
                        translator[element[0]] = modifiedSpecies
                    else:
                        print 'ALERT',element[0]
            else:
                print '---',dependencyGraph[element[0]],element
                '''
                if element[0] not in database:
                    species = st.Species()
                else:
                    species = database[element[0]]
                '''
                bondNumber = 1
                species =st.Species()
                for molecule in dependencyGraph[element[0]][0]:
                    if molecule in translator:
                        tmpSpecies = translator[molecule]
                        if molecule != getTrueTag(dependencyGraph,molecule):
                            original =translator[getTrueTag(dependencyGraph,molecule)]
                            updateSpecies(tmpSpecies,original.molecules[0])
                        species.addMolecule(deepcopy(tmpSpecies.molecules[0]))
                    else:
                        mol = st.Molecule(molecule)
                        dependencyGraph[molecule] = deepcopy(mol)
                        species.addMolecule(mol)
                #listOfConnectingComponents,unpairedComponent = getComplexationComponents(species,dependencyGraph,dependencyGraph[element[0]][0])
                newComponentList,moleculePairsList = getComplexationComponents2(species)
                for molecule in newComponentList:
                    newComponent1 = st.Component(molecule[1].name.lower())
                    newComponent2 = st.Component(molecule[0].name.lower())
                    
                    molecule[0].components.append(newComponent1)
                    molecule[1].components.append(newComponent2)
                    
                    #TODO: update basic molecules with new components
                    #translator[molecule[0].name].molecules[0].components.append(deepcopy(newComponent1))
                    #translator[molecule[1].name].molecules[0].components.append(deepcopy(newComponent2))
                
                for idx,molecule in enumerate(moleculePairsList):
                    for component in molecule[0].components:
                        if component.name == molecule[1].name.lower() and len(component.bonds) == 0:
                            component.bonds.append(idx)
                    for component in molecule[1].components:
                        if component.name == molecule[0].name.lower() and len(component.bonds) == 0:
                            component.bonds.append(idx)

                            
                print '???!!!',element[0],str(species)
                translator[element[0]] = species

def updateSpecies(species,referenceMolecule):
    flag = False
    for moleculeStructure in species.molecules:
        if moleculeStructure.name == referenceMolecule.name:
            for component in referenceMolecule.components:
                count = [x.name for x in referenceMolecule.components].count(component.name)
                count -= [x.name for x in moleculeStructure.components].count(component.name)
                newComponent= st.Component(component.name)
                if count > 0:
                    for idx in range(0,count):
                        moleculeStructure.addComponent(deepcopy(newComponent))
                elif count < 0:
                    for idx in range(0,-count):
                        referenceMolecule.addComponent(deepcopy(newComponent))
                        flag = True
    return flag
                    
def propagateChanges(translator,dependencyGraph):
    flag = True
    while flag:
        flag = False
        for dependency in dependencyGraph:
            if dependencyGraph[dependency] == []:
                continue
            for molecule in dependencyGraph[dependency][0]:
                if updateSpecies(translator[dependency],translator[getTrueTag(dependencyGraph,molecule)].molecules[0]):
                    flag = True

def preRuleify(reaction,dependencyGraph,ruleifyDictionary):
    moleculeList = []
    for element in reaction[0]:
        if element not in ruleifyDictionary:
            tmp = st.Species()
            if dependencyGraph[element] == []:
                tmpMolecule = st.Molecule(element)
                moleculeList.append(tmpMolecule)
                tmp.addMolecule(tmpMolecule)
            else:
                for molecule in dependencyGraph[element][0]: 
                    print molecule,dependencyGraph[molecule],resolveDependencyGraph(dependencyGraph,molecule)
                    tmpMolecule = st.Molecule(molecule)
                    moleculeList.append(tmpMolecule)
                    tmp.addMolecule(tmpMolecule)
            ruleifyDictionary[element] = tmp
            
            
    for element in reaction[1]:
        if element not in ruleifyDictionary:
            tmp = st.Species()
            if dependencyGraph[element] == []:
                tmpMolecule = st.Molecule(element)
                moleculeList.append(tmpMolecule)
                tmp.addMolecule(tmpMolecule)
            else:
                for molecule in dependencyGraph[element][0]:
                    tmpMolecule = None
                    for idx,reactantMolecule in enumerate(moleculeList):
                        if reactantMolecule.name == molecule:
                            tmpMolecule = deepcopy(reactantMolecule)
                            moleculeList.pop(idx)
                            break
                    if tmpMolecule == None:
                        tmpMolecule = st.Molecule(molecule)
                    tmp.addMolecule(tmpMolecule)
            ruleifyDictionary[element] = tmp
                

            
def transformMolecules(parser,database,configurationFile,speciesEquivalences=None):
    _,rules,_ = parser.getReactions()
    molecules,_,_ = parser.getSpecies()
    sbmlAnalyzer =analyzeSBML.SBMLAnalyzer(configurationFile,speciesEquivalences)
    classifications,equivalenceTranslator,eequivalenceTranslator = sbmlAnalyzer.classifyReactions(rules,molecules)
    
    
    database.reactionProperties = sbmlAnalyzer.getReactionProperties()
    database.translator,database.labelDictionary = sbmlAnalyzer.getUserDefinedComplexes()
    database.dependencyGraph = {}
    #analyzeSBML.analyzeNamingConventions(molecules)
    rdfAnnotations = analyzeRDF.getAnnotations(parser,'uniprot')
  
    
    
    for reaction,classification in zip(rules,classifications):
        dependencyGraph(database.dependencyGraph,list(parseReactions(reaction)),classification,equivalenceTranslator)

    
    for key in eequivalenceTranslator:
        for namingEquivalence in eequivalenceTranslator[key]:
            baseElement = min(namingEquivalence,key=len)
            modElement = max(namingEquivalence,key=len)
            if key!= 'Binding':
                addToDependencyGraph(database.dependencyGraph,modElement,[baseElement])

    for element in database.labelDictionary:
        if element == database.labelDictionary[element][0][0]:
            addToDependencyGraph(database.dependencyGraph,element,[])
        else:
            addToDependencyGraph(database.dependencyGraph,element,list(database.labelDictionary[element][0]))
    
    prunnedDependencyGraph,weights,unevenElementDict = consolidateDependencyGraph(database.dependencyGraph)
    #print prunnedDependencyGraph
    classifications,equivalenceTranslator,eequivalenceTranslator = sbmlAnalyzer.classifyReactions(rules,molecules)

    #print resolveDependencyGraph(database.dependencyGraph,'EGF_EGFRm2_GAP_Shcm_Grb2_Sos_Ras_GTP_Prot',True)
    #print resolveDependencyGraph(prunnedDependencyGraph,'EGF_EGFRm2')
    weightsDict = {x[0]:x[1] for x in weights}
    tempRules = {}
    weights = sorted(weights,key=lambda rule:rule[1])
    preRuleifyReactions(prunnedDependencyGraph,weights,database.translator,database.reactionProperties,eequivalenceTranslator)
    print {x:str(database.translator[x]) for x in database.translator}
    #propagate changes
    propagateChanges(database.translator,prunnedDependencyGraph)    
    #for rule in rules:
    #    reaction = list(parseReactions(rule))
    #    preRuleify(reaction,prunnedDependencyGraph,tempRules)
    return database.translator        