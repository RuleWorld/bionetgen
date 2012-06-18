# -*- coding: utf-8 -*-
"""
Created on Wed May 30 11:44:17 2012

@author: proto
"""

class Species:
    def __init__(self):
        self.molecules = []
        
    def addMolecule(self,molecule):
        self.molecules.append(molecule)
        
    def getMolecule(self,moleculeName):
        for molecule in self.molecules:
            if moleculeName == molecule.name:
                return molecule
        return None
    def getSize(self):
        return len(self.molecules)
    
    def contains(self,moleculeName):
        for molecule in self.molecules:
            if moleculeName == molecule.name:
                return True
        return False
        
    def addChunk(self,tags,moleculesComponents,precursors):
        '''
        temporary transitional method
        '''
        print 'lllllllllllllllllllll',moleculesComponents
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
                    continue
                tmpCompo = Component(component[0][0])
                
                for index in range(1,len(component[0])):
                    tmpCompo.addState(component[0][index])
                if len(component) > 1:
                    tmpCompo.addBond(component[1])
                tmp.addComponent(tmpCompo)
            if not self.contains(tag):
                self.molecules.append(tmp)
    def __str__(self):
        return '.'.join([x.toString() for x in self.molecules])
        
    def toString(self):
        return self.__str__()
        

class Molecule:
    def __init__(self,name):
        self.components = []
        self.name = name
        
    def addChunk(self,chunk):
        component = Component(chunk[0][0][0][0])
        component.addState(chunk[0][0][0][1])
        self.addComponent(component)
        
    def addComponent(self,component):
        self.components.append(component)
        
    def getComponent(self,componentName):
        for component in self.components:
            if componentName == component.getName():
                return component
                
    def addBond(self,componentName,bondName):
        component = self.getComponent(componentName)
        component.addBond(bondName)
        
    def contains(self,componentName):
        return componentName in [x.name for x in self.components]
        
    def __str__(self):
        return self.name + '(' + ','.join([str(x) for x in self.components]) + ')'
        
    def toString(self):
        return self.__str__()
    
class Component:
    def __init__(self,name,bonds = [],states=[]):
        self.name = name
        self.states = states
        self.bonds = []
        self.activeState = ''
        
    def addState(self,state):
        self.states.append(state)
        self.setActiveState(state)
        
    def addBond(self,bondName):
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
        
        
class Databases:
    def __init__(self):
        self.translator ={}
        self.synthesisDatabase = {}
        self.catalysisDatabase = {}
        self.rawDatabase = {}
        self.labelDictionary = {}
        
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
    