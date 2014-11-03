# -*- coding: utf-8 -*-
"""
Created on Mon May 14 14:59:41 2012

@author: proto
"""

class Sites:
    def __init__(self,name,bond=''):
        self.name= name
        self.bond = bond
        
    def __str__(self):
        if (self.bond == ''):
            return self.name
        else:
            return '%s!%s' % (self.name,self.bond)
    

class Molecules:
    def __init__(self,name,components=[]):
        print name,components
        self.name = name
        self.components = []
        for element in components:
            self.addComponent(element)

    def addComponent(self,component):
        if len(component) ==1:
            self.components.append(Sites(component[0]))
        else:
            self.components.append(Sites(component[0],component[1]))
    def __str__(self):
        printString = '%s(' % (self.name)
        printString += ','.join([str(k) for k in self.components])
        return printString + ')'
        
        
        

class Species:
    
    def __init__(self,molecules):
        self.molecules = []
        self.addMolecules(molecules)
    
        self.compartment = ''
        
    def addMolecules(self,molecules):
        for element in molecules:
            self.addMolecule(element[0],element[1])
        
    def addMolecule(self,name,components):
        molecule = Molecules(name,components)
        self.molecules.append(molecule)
        
    def addCompartment(self,compartment):
        self.compartment = compartment
        
    def __str__(self):
        printString = ''
        if self.compartment is not '':
            printString += '@s:' % (self.compartment) 
        printString += '.'.join(str(k) for k in self.molecules)
        return printString 
            


def main():
    #s = Species((('SAv', 'EpoR'), [('l', 215)], [('U',), ('I',), ('r', 215)]))
    s = Species([['SAv',[('l', 215)]],['EpoR',[('r', 215),('0',),('I',)]]])
    print s
            
if __name__ == "__main__":
    main()
    
    