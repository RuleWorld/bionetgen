# -*- coding: utf-8 -*-
"""
Created on Wed May 30 11:44:17 2012

@author: proto
"""
from copy import deepcopy
class Species:
    def __init__(self):
        self.molecules = []
        self.bondNumbers = []
    
    def copy(self):
        species = Species()
        for molecule in self.molecules:
            species.molecules.append(molecule.copy())
        return species
    
    def addMolecule(self,molecule,concatenate=False,iteration = 1):
        if not concatenate:
            self.molecules.append(molecule)
        else:
            counter = 1
            for element in self.molecules:
                
                if element.name == molecule.name:
                    if iteration == counter:
                        element.extend(molecule)
                        break
                    else:
                        counter +=1
            #self.molecules.append(molecule)
            #for element in self.molecules:
            #    if element.name == molecule.name:
    
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
                    
    
        
    def append(self,species):
        for element in species.molecules:
            self.molecules.append(deepcopy(element))              
        
    def __str__(self):
        return '.'.join([x.toString() for x in self.molecules])
        
    def reset(self):
        for element in self.molecules:
            element.reset()
            
    def toString(self):
        return self.__str__()
        

class Molecule:
    def __init__(self,name):
        self.components = []
        self.name = name
        
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
        component = self.getComponent(componentName)
        component.addBond(bondName)
        
    def getComponentWithBonds(self):
        return [x.name for x in self.components if x.bonds != []]
        
    def contains(self,componentName):
        return componentName in [x.name for x in self.components]
        
    def __str__(self):
        self.components.sort()
        return self.name + '(' + ','.join([str(x) for x in self.components]) + ')'
        
    def toString(self):
        return self.__str__()
        
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
        
    def __hash__(self):
        return self.name
        
    def reset(self):
        self.bonds = []
        if 'U' in self.states:
            self.activeState = 'U'
        
        
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
    