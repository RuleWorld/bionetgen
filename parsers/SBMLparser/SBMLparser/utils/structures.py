# -*- coding: utf-8 -*-
"""
Created on Wed May 30 11:44:17 2012

@author: proto
"""
from copy import deepcopy
import difflib
import hashlib
import numpy
from collections import Counter, defaultdict


def compareLists(list1, list2):
    return Counter(list1) == Counter(list2)


class Species:
    def __init__(self):
        self.molecules = []
        self.bondNumbers = []

    def __eq__(self, other):
        return str(self) == str(other)

    def getBondNumbers(self):
        bondNumbers = [0]
        for element in self.molecules:
            bondNumbers.extend(element.getBondNumbers())
        return bondNumbers

    def copy(self):
        species = Species()
        for molecule in self.molecules:
            species.molecules.append(molecule.copy())
        return species

    def addMolecule(self, molecule, concatenate=False, iteration=1):
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
                        counter += 1
            self.molecules.append(molecule)
            #self.molecules.append(molecule)
            #for element in self.molecules:
            #    if element.name == molecule.name:
                
    def addCompartment(self, tags):
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
            
            list1 = sorted(self.molecules,key=lambda x:len(x.components))
            list1 = sorted(list1,key=lambda x:x.name)
            list2 = sorted(species.molecules,key=lambda x:len(x.components))
            list2 = sorted(list2,key=lambda x:x.name)
            
            for (selement,oelement) in zip(list1,list2):
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
                     
                    bond1 = sum([x.bonds for x in element.components],[])
                    bondList = []
                    for x in [z for z in self.molecules if z.name == element.name]:
                        bondList.append((x,sum([y.bonds for y in x.components],[])))
                    score = 0
                    for x in bondList:
                        if difflib.SequenceMatcher(None,x[1],bond1).ratio() >= score:
                            score = difflib.SequenceMatcher(None,x[1],bond1).ratio()
                            molecule = x[0]
                    #sortedArray = sorted(bondList,
                    #    key=lambda y:difflib.SequenceMatcher(None,y[1],bond1),reverse=True)
                    #molecule = sortedArray[0][0]
                        
                    for component in element.components:
                        if component.name not in [x.name for x in molecule.components]:
                            molecule.addComponent(deepcopy(component),update)
                        else:
                            comp = molecule.getComponent(component.name)
                            for state in component.states:
                                comp.addState(state,update)
                    
    
    def updateBonds(self, bondNumbers):
        newBondNumbers = deepcopy(bondNumbers)
        correspondence = {}
        intersection = [int(x) for x in newBondNumbers if x in self.getBondNumbers()]
        newBase = max(bondNumbers) + 1
        for element in self.molecules:
            for component in element.components:
                component.bonds = [int(x) + newBase for x in component.bonds]
                '''
                for index in range(0,len(component.bonds)):
                    if int(component.bonds[index]) in intersection:

                        if component.bonds[index] in correspondence:
                            component.bonds[index] = correspondence[component.bonds[index]]
                        else:
                            correspondence[component.bonds[index]] = max(intersection) + 1
                            component.bonds[index] = max(intersection) + 1
                '''
                        #intersection = [int(x) for x in newBondNumbers if x in self.getBondNumbers()]

    def deleteBond(self, moleculePair):
        for molecule in self.molecules:
            if molecule.name in moleculePair:
                moleculePairCopy = deepcopy(moleculePair)
                moleculePairCopy.remove(molecule.name)
                for component in molecule.components:
                    if component.name in [x.lower() for x in moleculePairCopy]:
                        component.bonds = []

    def append(self, species):
        newSpecies = (deepcopy(species))
        newSpecies.updateBonds(self.getBondNumbers())

        for element in newSpecies.molecules:
            self.molecules.append(deepcopy(element))

    def sort(self):
        """
        Sort molecules by number of components, then number of bonded components, then lowest numbered bond,
        then the negative sum of the bond index, then number of active states, then string length
        """

        self.molecules.sort(key=lambda molecule: (len(molecule.components),
                                                      -min([int(y) if y not in ['?','+'] else 999 for x in molecule.components for y in x.bonds] + [999]),
                                                      -len([x for x in molecule.components if len(x.bonds) > 0]),
                                                      -len([x for x in molecule.components if x.activeState not in [0, '0']]),
                                                      len(str(molecule)),
                                                      str(molecule)),
                                reverse=True)
        #self.molecules.sort(key=lambda x:(-len(x.components),x.evaluateMolecule(),x.name))

    def __str__(self):
        self.sort()
        return '.'.join([x.toString().replace('-', '_') for x in self.molecules])

    def str2(self):
        self.sort()
        return '.'.join([x.str2().replace('-', '_') for x in self.molecules])

    def reset(self):
        for element in self.molecules:
            element.reset()

    def toString(self):
        return self.__str__()


import pickle
class Molecule:
    def __init__(self, name):
        self.components = []
        self.name = name
        self.compartment = ''
        
        a = numpy.random.rand(10, 100)
        self.hash = hashlib.sha1(a).digest()
        
        
    def copy(self):
        molecule = Molecule(self.name)
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
        self.components = sorted(self.components,key=lambda x:x.name)
    
    def setCompartment(self, compartment):
        self.compartment = compartment
             
    def getBondNumbers(self):
        bondNumbers = []
        for element in self.components:
                bondNumbers.extend([int(x) for x in element.bonds])
        return bondNumbers
        
    def getComponent(self, componentName):
        for component in self.components:
            if componentName == component.getName():
                return component
                
    def removeComponent(self, componentName):
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
        return [x.name for x in self.components if x.bonds != []]
        
    def contains(self,componentName):
        return componentName in [x.name for x in self.components]
        
        
    def evaluateMolecule(self):
        try:
            return min([self.evaluateBonds(x.bonds) for x in self.components])
        except ValueError:
            return 999999

    def evaluateBonds(self,bonds):
        if len(bonds) == 0:
            return 9999999
        return bonds[0]

    def sort(self):
        self.components.sort(key=lambda x:(x.name,self.evaluateBonds(x.bonds)))
    def __str__(self):

        self.sort()
        tmp = self.name.replace('-','_')
        if tmp == '0':
            return tmp
        if tmp[0].isdigit():
            tmp = 'm__' + tmp
        return tmp + '(' + ','.join([str(x) for x in self.components]) + ')' + self.compartment
      
    def signature(self,component):
        #tmpComponents = deepcopy(self.components)
        #tmpComponents.sort(key=lambda x:x.name)
        tmp = self.name.replace('-','_')
        return tmp + '(' + ','.join([x.signature() for x in self.components if (self.name,x.name) not in component]) + ')' + self.compartment

    def toString(self):
        return self.__str__()
        
    def str2(self):
        self.sort()
        self.components = sorted(self.components,key=lambda x:x.name)
        tmp = self.name.replace('-','_')
        if tmp[0].isdigit():
            tmp = 'm__' + tmp
        
        return tmp + '(' + ','.join([x.str2() for x in self.components]) + ')'
        
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
                
    
class Component:
    def __init__(self,name,bonds = [],states=[]):
        self.name = name
        self.states = []
        self.bonds = []
        self.activeState = ''
        
        
    def copy(self):
        component = Component(self.name,deepcopy(self.bonds),deepcopy(self.states))
        component.activeState = deepcopy(self.activeState)     
        return component
        
    def addState(self,state,update=True):
        if not state in self.states:
            self.states.append(state)
        if update:
            self.setActiveState(state)
        if not '0' in self.states:
            self.states.append('0')
        #print 'LALALA',state
    def addStates(self,states,update=True):
        for state in states:
            if state not in self.states:
                self.addState(state,update)
        
    def addBond(self,bondName):
        #if len(self.bonds) == 0:
        #    self.bonds.append('U')
        if not bondName in self.bonds:
            self.bonds.append(bondName)
            return True
        return False
        
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
        tmp = self.getRuleStr()
        #edit to meet bng string requirements
        if tmp[0].isdigit():
            tmp = '_' + tmp
        tmp = tmp.replace('-','_')
        return tmp
        
    def str2(self):
        tmp = self.name

        if len(self.bonds) > 0:
            tmp += '!' + '!'.join([str(x) for x in self.bonds])
        if len(self.states) > 0:
            tmp += '~' + '~'.join([str(x) for x in self.states])

        if tmp[0].isdigit():
            tmp = '_' + tmp

        return tmp        
        
    def signature(self):
        tmp = self.name
        if len(self.bonds) > 0:
            tmp += '!+'
        if self.activeState != '':
            tmp += '~{0}'.format(self.activeState)
        return tmp        
        
    def __hash__(self):
        return self.name
        
    def reset(self):
        self.bonds = []
        if '0' in self.states:
            self.activeState = '0'
        
        
class Databases:
    def __init__(self):
        # dictionary contains molecule vs bionetgen structure defitions
        self.translator = {}
        self.synthesisDatabase = {}
        self.catalysisDatabase = {}
        self.rawDatabase = {}
        self.labelDictionary = {}
        self.synthesisDatabase2 = {}
        self.assumptions = defaultdict(list)
        
    def getRawDatabase(self):
        return self.rawDatabase
        
    def getLabelDictionary(self):
        return self.labelDictionary
        
    def add2LabelDictionary(self, key, value):
        temp = tuple(key)
        temp = temp.sort()
        self.labelDictionary[temp] = value

    def add2RawDatabase(self, rawDatabase):
        pass
    
    def getTranslator(self):
        return self.translator
    