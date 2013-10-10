# -*- coding: utf-8 -*-
"""
Created on Wed May 30 11:44:17 2012

@author: proto
"""
from copy import deepcopy
from lxml import etree
import pygraphviz as pgv
import re
from random import randint
from pyparsing import Word, Suppress, Optional, alphanums, Group, ZeroOrMore
from collections import Counter

def parseReactions(reaction):
    components = (Word(alphanums + "_") + Optional(Group('~' + Word(alphanums)))
    + Optional(Group('!' + Word(alphanums))))
    molecule = (Word(alphanums + "_")
    + Optional(Suppress('(')) + Group(components) + ZeroOrMore(Suppress(',') + Group(components))    
    +Suppress(')'))
    
    species = Group(molecule) + ZeroOrMore(Suppress('.') + Group(molecule))

    result = species.parseString(reaction).asList()
    
    return result

def readFromString(string):
    sp = Species()
    reactionList = parseReactions(string)
    for molecule in reactionList:
        mol = Molecule(molecule[0],'')
        if len(molecule) >1:
            for idx in range(1,len(molecule)):
                comp = Component(molecule[idx][0],'')
                if len(molecule[idx]) > 1:
                    for idx2 in range(1,len(molecule[idx])):
                        if molecule[idx][idx2][0] == '~':
                            comp.addState(molecule[idx][idx2][1])
                        elif molecule[idx][idx2][0] == '!':
                            comp.addBond(molecule[idx][idx2][1])
                mol.addComponent(comp)
        sp.addMolecule(mol)
    return sp

class Rule:
    def __init__(self):
        self.reactants = []
        self.products = []
        self.rates = []
        self.bidirectional = False
        self.actions = []
        self.mapping = []
        
    def addReactant(self,reactant):
        self.reactants.append(reactant)

    def addProduct(self,product):
        self.products.append(product)

    def addReactantList(self,reactants):
        self.reactants.extend(reactants)

    def addProductList(self,products):
        self.products.extend(products)
        
    def addRate(self,rate):
        self.rates.append(rate)
        
    def addMapping(self,mapping):
        self.mapping.add(mapping)
        
    def addMappingList(self,mappingList):
        self.mapping.extend(mappingList)
        
    def addActionList(self,actionList):
        self.actions.extend(actionList)
        
    def __str__(self):
        return ' + '.join([str(x) for x in self.reactants]) + ' -> ' + ' + '.join([str(x) for x in self.products]) + ' ' + ','.join(self.rates)
            
class Species:
    def __init__(self):
        self.molecules = []
        self.bondNumbers = []
        self.bonds = []
        self.identifier = randint(0,100000)
        self.idx = ''
    


        
    def getBondNumbers(self):
        bondNumbers = [0]
        for element in self.molecules:
            bondNumbers.extend(element.getBondNumbers())
        return bondNumbers        
        
    def copy(self):
        species = Species()
        species.identifier = randint(0,1000000)
        for molecule in self.molecules:
            species.molecules.append(molecule.copy())
        return species
        
    def getMoleculeById(self,idx):
        for molecule in self.molecules:
            if molecule.idx == idx:
                return molecule
    
    def addMolecule(self,molecule,concatenate=False,iteration = 1):
        if not concatenate:
            self.molecules.append(molecule)
        else:
            counter = 1
            for element in self.molecules:
                
                if element.name == molecule.name:
                    if iteration == counter:
                        element.extend(molecule)
                        return
                    else:
                        counter +=1
            self.molecules.append(molecule)
            #self.molecules.append(molecule)
            #for element in self.molecules:
            #    if element.name == molecule.name:
                
    def addCompartment(self,tags):
        for molecule in self.molecules:
            molecule.setCompartment(tags)
    
    def deleteMolecule(self,moleculeName):
        deadMolecule = None
        for element in self.molecules:
            if element.name == moleculeName:
                deadMolecule = element
        if deadMolecule == None:
            return
        bondNumbers = deadMolecule.getBondNumbers()
        self.molecules.remove(deadMolecule)
        for element in self.molecules:
            for component in element.components:
                for number in bondNumbers:
                    if str(number) in component.bonds:
                        component.bonds.remove(str(number))
    
    def getMolecule(self,moleculeName):
        for molecule in self.molecules:
            if moleculeName == molecule.name:
                return molecule
        return None
    def getSize(self):
        return len(self.molecules)
        
    def getMoleculeNames(self):
        return [x.name for x in self.molecules]
    
    def contains(self,moleculeName):
        for molecule in self.molecules:
            if moleculeName == molecule.name:
                return True
        return False
        
    def addChunk(self,tags,moleculesComponents,precursors):
        '''
        temporary transitional method
        '''
        for (tag,components) in zip (tags,moleculesComponents):
            if self.contains(tag):
                tmp = self.getMolecule(tag)
            else:
                tmp = Molecule(tag)
                #for element in precursors:
                #    if element.getMolecule(tag) != None:
                #        tmp = element.getMolecule(tag)
            
            for component in components:
                if tmp.contains(component[0][0]):
                    tmpCompo = tmp.getComponent(component[0][0])
                    #continue
                else:
                    tmpCompo = Component(component[0][0])
                
                for index in range(1,len(component[0])):
                    tmpCompo.addState(component[0][index])
                if len(component) > 1:
                    tmpCompo.addBond(component[1])
                if not tmp.contains(component[0][0]):   
                    tmp.addComponent(tmpCompo)
            if not self.contains(tag):
                self.molecules.append(tmp)
                
    def extend(self,species,update=True):
        if(len(self.molecules) == len(species.molecules)):
            for (selement,oelement) in zip(self.molecules,species.molecules):
                for component in oelement.components:
                    if component.name not in [x.name for x in selement.components]:
                        selement.components.append(component)
                    else:
                        for element in selement.components:
                            if element.name == component.name:
                                element.addStates(component.states,update)
                                
        else:
            for element in species.molecules:
                if element.name not in [x.name for x in self.molecules]:
                    
                    self.addMolecule(deepcopy(element),update)
                else:
                    for molecule in self.molecules:
                        if molecule.name == element.name:
                            for component in element.components:
                                if component.name not in [x.name for x in molecule.components]:
                                    molecule.addComponent(deepcopy(component),update)
                                else:
                                    comp = molecule.getComponent(component.name)
                                    for state in component.states:
                                        comp.addState(state,update)
                    
    
    def updateBonds(self,bondNumbers):
        newBondNumbers = deepcopy(bondNumbers)
        correspondence = {}
        intersection = [int(x) for x in newBondNumbers if x in self.getBondNumbers()]
        for element in self.molecules:
            for component in element.components:
                for index in range(0,len(component.bonds)):
                    if int(component.bonds[index]) in intersection:
                        
                        if component.bonds[index] in correspondence:
                            component.bonds[index] = correspondence[component.bonds[index]]
                        else:
                            correspondence[component.bonds[index]] = max(intersection) + 1
                            component.bonds[index] = max(intersection) + 1
                        #intersection = [int(x) for x in newBondNumbers if x in self.getBondNumbers()]
    
    def append(self,species):
        newSpecies = (deepcopy(species))
        newSpecies.updateBonds(self.getBondNumbers())
        
        for element in newSpecies.molecules:
            self.molecules.append(deepcopy(element))              
        
    def __str__(self):
        self.molecules.sort(key= lambda molecule: molecule.name)
        name= '.'.join([x.toString() for x in self.molecules])
        '''
        name = name.replace('~','')
        
        name = name.replace('~','')
        name = name.replace(',','')
        name = name.replace('.','')
        name = name.replace('(','')
        name = name.replace(')','')
        name = name.replace('!','')
        name = name.replace('_','')
        '''
        return name
        
    def str2(self):
        return '.'.join([x.str2() for x in self.molecules])
        
    def reset(self):
        for element in self.molecules:
            element.reset()
            
    def toString(self):
        return self.__str__()
    
    def extractAtomicPatterns(self,action,site1,site2):
        atomicPatterns = {}
        bondedPatterns = {}
        reactionCenter = []
        context = []
        #one atomic pattern for the state, one for the bond
        for molecule in self.molecules:
            for component in molecule.components:
                if component.activeState != '':
                    speciesStructure = Species()
                    #one atomic pattern for the states
                    speciesStructure.bonds = self.bonds
                    moleculeStructure = Molecule(molecule.name,molecule.idx)
                    componentStructure = Component(component.name,component.idx)

                    componentStructure.addState(component.activeState)
                    componentStructure.activeState = component.activeState
                    moleculeStructure.addComponent(componentStructure)
                    speciesStructure.addMolecule(moleculeStructure)
                    if componentStructure.idx in [site1,site2] and action == 'StateChange':
                        reactionCenter.append((speciesStructure))
                    else:
                        context.append((speciesStructure))
                    atomicPatterns[str(speciesStructure)] = speciesStructure   
                speciesStructure = Species()
                #one atomic pattern for the bonds
                speciesStructure.bonds = self.bonds
                moleculeStructure = Molecule(molecule.name,molecule.idx)
                componentStructure = Component(component.name,component.idx)
                moleculeStructure.addComponent(componentStructure)
                speciesStructure.addMolecule(moleculeStructure)
                #atomicPatterns[str(speciesStructure)] = speciesStructure
                if len(component.bonds) == 0:
                    if component.activeState == '':
                        atomicPatterns[str(speciesStructure)] = speciesStructure
                else:
                    if component.bonds[0] != '+':
                        componentStructure.addBond(1)
                    else:
                        componentStructure.addBond('+')
                    if component.bonds[0] not in bondedPatterns:
                        bondedPatterns[component.bonds[0]] = speciesStructure
                    elif '+' not in component.bonds[0] or \
                      len(bondedPatterns[component.bonds[0]].molecules) == 0: 
                        bondedPatterns[component.bonds[0]].addMolecule(moleculeStructure)
                if componentStructure.idx in [site1,site2] and action != 'StateChange':
                    reactionCenter.append((speciesStructure))
                elif len(component.bonds) > 0 or component.activeState == '':
                    context.append((speciesStructure))      
        for element in bondedPatterns:
            atomicPatterns[str(bondedPatterns[element])] = bondedPatterns[element]
            
        reactionCenter = [str(x) for x in reactionCenter 
            if str(x) in atomicPatterns]
        context =  [str(x) for x in context if str(x) in atomicPatterns]
        
        return atomicPatterns,reactionCenter,context
                
                    
                    
        
    def graphVizGraph(self,graph,identifier):
        speciesDictionary = {}
        graphName = "%s_%s" % (identifier,str(self))
        
        for idx,molecule in enumerate(self.molecules):
            ident = "%s_m%i" %(graphName,idx)
            speciesDictionary[molecule.idx] = ident
            if len(self.molecules) == 1:
                compDictionary = molecule.graphVizGraph(graph,ident,flag=False)
            else:
                #s1 = graph.subgraph(name = graphName,label=' ')
                compDictionary = molecule.graphVizGraph(graph,ident,flag=False)
            speciesDictionary.update(compDictionary)
            
        for bond in self.bonds:
            if bond[0] in speciesDictionary and bond[1] in speciesDictionary:
                graph.add_edge(speciesDictionary[bond[0]],speciesDictionary[bond[1]],dir='none')
        return speciesDictionary
        
    
        
    def containsComponentIdx(self,idx,dictionary):
        for molecule in self.molecules:
            for component in molecule.components:
                if component.idx == idx:
                    return dictionary[idx]
        return None
        
    def notContainsComponentIdx(self,idx):
        context = []
        for molecule in self.molecules:
            for component in molecule.components:
                if component.idx not in idx:
                    context.append(component)
        return context
        
    def hasWildCardBonds(self):
        for molecule in self.molecules:
            if molecule.hasWildcardBonds():
                return True
        return False
            
    def listOfBonds(self,nameDict):
        listofbonds = {}
        for bond in self.bonds:
            mol1 = re.sub('_C[^_]*$', '', bond[0])
            mol2 = re.sub('_C[^_]*$', '', bond[1])
            if nameDict[mol1] not in listofbonds:
                listofbonds[nameDict[mol1]] = {}
            listofbonds[nameDict[mol1]][nameDict[bond[0]]] = [(nameDict[mol2],nameDict[bond[1]])]
            if nameDict[mol2] not in listofbonds:
                listofbonds[nameDict[mol2]] = {}
            listofbonds[nameDict[mol2]][nameDict[bond[1]]] = [(nameDict[mol1],nameDict[bond[0]])]

        return listofbonds
        

class Molecule:
    def __init__(self,name,idx):
        self.components = []
        self.name = name
        self.compartment = ''
        self.idx = idx
        self.uniqueIdentifier = randint(0,100000)
        
    def copy(self):
        molecule = Molecule(self.name,self.idx)
        for element in self.components:
            molecule.components.append(element.copy())
        return molecule 
        
    def addChunk(self,chunk):
        component = Component(chunk[0][0][0][0])
        component.addState(chunk[0][0][0][1])
        self.addComponent(component)
        
    def addComponent(self,component,overlap=0):
        if not overlap:
            self.components.append(component)
        else:
            if not component.name in [x.name for x in self.components]:
                self.components.append(component)
            else:
                compo = self.getComponent(component.name)
                for state in component.states:
                    compo.addState(state)
    
    def setCompartment(self,compartment):
        self.compartment = compartment
    
    def getComponentById(self,idx):
        for component in self.components:
            if component.idx == idx:
                return component
             
    def getBondNumbers(self):
        bondNumbers = []
        for element in self.components:
                bondNumbers.extend([int(x) for x in element.bonds if x != '+'])
        return bondNumbers
        
    def getComponent(self,componentName):
        for component in self.components:
            if componentName == component.getName():
                return component
                
    def removeComponent(self,componentName):
        x = [x for x in self.components if x.name == componentName]
        if x != []:
            self.components.remove(x[0])
            
    def removeComponents(self,components):
        for element in components:
            if element in self.components:
                self.components.remove(element)
                
    def addBond(self,componentName,bondName):
        bondNumbers = self.getBondNumbers()
        while bondName in bondNumbers:
            bondName += 1
        component = self.getComponent(componentName)
        component.addBond(bondName)
        
    def getComponentWithBonds(self):
        return [x for x in self.components if x.bonds != []]
        
    def contains(self,componentName):
        return componentName in [x.name for x in self.components]
        
    def __str__(self):
        self.components = sorted(self.components,key = lambda st:st.name)
        finalStr =  self.name
        if len(self.components) > 0:
            finalStr += '(' + ','.join([str(x) for x in self.components]) + ')' 
        if self.compartment != '':
            finalStr += '@' + self.compartment
        return finalStr
        
    def toString(self):
        return self.__str__()
        
    def str2(self):
        self.components.sort()
        return self.name + '(' + ','.join([x.str2() for x in self.components]) + ')'
        
    def str3(self):
        return self.name + '(' + self.components[0].name + ')'

    def extend(self,molecule):
        for element in molecule.components:
            comp = [x for x in self.components if x.name == element.name]
            if len(comp) == 0:
                self.components.append(deepcopy(element))
            else:
                for bond in element.bonds:
                    comp[0].addBond(bond)
                for state in element.states:
                    comp[0].addState(state)
                    
    def reset(self):
        for element in self.components:
            element.reset()
            
    def update(self,molecule):
        for comp in molecule.components:
            if comp.name not in [x.name for x in self.components]:
                self.components.append(deepcopy(comp))
                
    def graphVizGraph(self,graph,identifier,components=None,flag=False):
        moleculeDictionary = {}
        flag = False
        '''
        if flag:
            graph.add_node(identifier,label=self.__str__(),name="cluster%s_%s" % (identifier,self.idx))
            print "cluster%s_%s" % (identifier,self.idx)
        else:
            print identifier
            graph.add_node(identifier,label=self.__str__(),name=identifier)
        
        moleculeDictionary[self.idx] = identifier
        return moleculeDictionary
        '''
        if len(self.components) == 0:
            graph.add_node(identifier,label=self.name)
            moleculeDictionary[self.idx] = identifier
        else:
            if not flag:
                s1 = graph.subgraph(name = "cluster%s_%s" % (identifier,self.idx),label=self.name)
            else:
                s1 = graph.subgraph(name = identifier,label=self.name)

            if components == None:
                tmpComponents = self.components
            else:
                tmpComponents = components
            for idx,component in enumerate(tmpComponents):
                ident = "%s_c%i" %(identifier,idx)
                moleculeDictionary[component.idx] = ident
                compDictionary = component.graphVizGraph(s1,ident)
                moleculeDictionary.update(compDictionary)
        return moleculeDictionary
                
        
    def hasWildcardBonds(self):
        for component in self.components:
            if component.hasWilcardBonds():
                return True
        return False
        
    def distance(self,cMolecule):
        distance = 0
        distance += 10000 if self.name != cMolecule.name else 0
        for component1,component2 in zip(self.components,cMolecule.components):
            distance += not component1.bonds == component2.bonds
            distance += not component1.activeState == component2.activeState
        return distance
    
    def compare(self,cMolecule):
        self.components = sorted(self.components,key=lambda st:st.name)
        cMolecule.components = sorted(cMolecule.components,key=lambda st:st.name)
        for c1,c2 in zip(self.components,cMolecule.components):
            if c1.activeState != c2.activeState:
                c1.activeState = ''
            
            if c1.bonds != c2.bonds:
                '''
                if len(c1.bonds) != len(c2.bonds) or '?' in c1.bonds or '?' in c2.bonds:
                    c1.bonds = ['?']
            
                else:
                    c1.bonds = ['+']
            '''
            
class Component:
    def __init__(self,name,idx,bonds = [],states=[]):
        self.name = name
        self.states = []
        self.bonds = []
        self.activeState = ''
        self.idx = idx
        
    def copy(self):
        component = Component(self.name,self.idx,deepcopy(self.bonds),deepcopy(self.states))
        component.activeState = deepcopy(self.activeState)     
        return component
        
    def addState(self,state,update=True):
        if not state in self.states:
            self.states.append(state)
        if update:
            self.setActiveState(state)
        #print 'LALALA',state
    def addStates(self,states,update=True):
        for state in states:
            if state not in self.states:
                self.addState(state,update)
        
    def addBond(self,bondName):
        if not bondName in self.bonds:
            self.bonds.append(bondName)
        
    def setActiveState(self,state):
        if state not in self.states:
            return False
        self.activeState = state
        return True
        
    def getRuleStr(self):
        tmp = self.name
        if len(self.bonds) > 0:
            tmp += '!' + '!'.join([str(x) for x in self.bonds])
        if self.activeState != '':
            tmp += '~' + self.activeState
        return tmp
        
    def getTotalStr(self):
        return self.name + '~'.join(self.states)
    
    def getName(self):
        return self.name 
        
    def __str__(self):
        return self.getRuleStr()
        
    def str2(self):
        tmp = self.name
        if len(self.bonds) > 0:
            tmp += '!' + '!'.join([str(x) for x in self.bonds])
        if len(self.states) > 0:
            tmp += '~' + '~'.join([str(x) for x in self.states])
        return tmp        
        
    def __hash__(self):
        return self.name
        
    def reset(self):
        self.bonds = []
        if 'U' in self.states:
            self.activeState = 'U'
            
    def hasWilcardBonds(self):
        if '+' in self.bonds:
            return True
        return False
    
    def graphVizGraph(self,graph,identifier):
        compDictionary = {}
        
        if len(self.states) == 0:
            graph.add_node(identifier,label=self.name)
        else:
            s1 = graph.subgraph(name="cluster%s_%s" % (identifier,self.idx),label=self.name)
            if self.activeState != '':
                ident = "%s" % (identifier)
                compDictionary[self.activeState] = ident
                s1.add_node(ident,label = self.activeState)
            else:
                for idx,state in enumerate(self.state):
                    ident = "%s_s%i" % (identifier,idx)
                    s1.add_node(ident,label = state)
                    compDictionary[state] = ident
        return compDictionary
            
    def createGraph(self,identifier):

        
        return component,compDictionary
 

class States:
    def __init__(self,name='',idx=''):
        self.name = name
        self.idx = idx
        
class Action:
    def __init__(self):
        self.action = ''
        self.site1 = ''
        self.site2 = ''
    
    def setAction(self,action,site1,site2=''):
        self.action = action
        self.site1 = site1
        self.site2 = site2
        
    def __str__(self):
        return '%s, %s, %s' %(self.action,self.site1,self.site2)
        
class Databases:
    def __init__(self):
        self.translator ={}
        self.synthesisDatabase = {}
        self.catalysisDatabase = {}
        self.rawDatabase = {}
        self.labelDictionary = {}
        self.synthesisDatabase2 = {}
        
    def getRawDatabase(self):
        return self.rawDatabase
        
    def getLabelDictionary(self):
        return self.labelDictionary
        
    def add2LabelDictionary(self,key,value):
        temp = tuple(key)
        temp = temp.sort()
        self.labelDictionary[temp] = value

    def add2RawDatabase(self,rawDatabase):
        pass
    
    def getTranslator(self):
        return self.translator
    
if __name__ == "__main__":
    sp = readFromString('A(b!1,p~P).B(a!1)')
