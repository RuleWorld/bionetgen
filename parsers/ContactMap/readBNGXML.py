# -*- coding: utf-8 -*-
"""
Created on Mon Nov 19 14:28:16 2012

@author: proto
"""
from lxml import etree
import sys
sys.path.insert(0, '../utils/')

import smallStructures as st
 #http://igraph.sourceforge.net/documentation.html
#----------------------------------------------------------------------


def findBond(bondDefinitions, component):
    '''
    Returns an appropiate bond number when veryfying how 
    to molecules connect in a species
    '''
    for idx, bond in enumerate(bondDefinitions.getchildren()):
        if component in [bond.get('site1'), bond.get('site2')]: 
            return str(idx+1)
    
def createMolecule(molecule, bonds):
    nameDict = {}
    mol = st.Molecule(molecule.get('name'),molecule.get('id'))
    nameDict[molecule.get('id')] = molecule.get('name')
    listOfComponents =  molecule.find('.//{http://www.sbml.org/sbml/level3}ListOfComponents')
    if listOfComponents != None:
        for element in listOfComponents:
            component = st.Component(element.get('name'),element.get('id'))
            nameDict[element.get('id')] = element.get('name')
            if element.get('numberOfBonds') in ['+','?']:
                component.addBond(element.get('numberOfBonds'))
            elif element.get('numberOfBonds') != '0':
                component.addBond(findBond(bonds, element.get('id')))
            state = element.get('state') if element.get('state') != None else ''
            component.states.append(state)
            component.activeState = state
            mol.addComponent(component)
    return mol, nameDict
    

    
def createSpecies(pattern):
    tmpDict = {}
    species = st.Species()
    species.idx = pattern.get('id')
    mol = pattern.find('.//{http://www.sbml.org/sbml/level3}ListOfMolecules')
    bonds = pattern.find('.//{http://www.sbml.org/sbml/level3}ListOfBonds')
    for molecule in mol.getchildren():
        molecule, nameDict = createMolecule(molecule, bonds)
        tmpDict.update(nameDict)
        species.addMolecule(molecule)
        if bonds != None:
            species.bonds = [(bond.get('site1'),bond.get('site2')) for bond in bonds]
        tmpDict.update(nameDict)
    return species, tmpDict
    
    

def parseRule(rule):
    '''
    Parses a rule XML section
    Returns: a list of the reactants and products used, followed by the mapping
    between the two and the list of operations that were performed
    '''
    rp = rule.find('.//{http://www.sbml.org/sbml/level3}ListOfReactantPatterns')
    pp = rule.find('.//{http://www.sbml.org/sbml/level3}ListOfProductPatterns')
    mp = rule.find('.//{http://www.sbml.org/sbml/level3}Map')
    op = rule.find('.//{http://www.sbml.org/sbml/level3}ListOfOperations')
    nameDict = {}
    reactants = []
    products = []
    actions = []
    mappings = []
    for pattern in rp:
        elm, tmpDict = createSpecies(pattern)
        reactants.append(elm)
        nameDict.update(tmpDict)
    for pattern in pp:
        elm, tmpDict = createSpecies(pattern)
        products.append(elm)
        nameDict.update(tmpDict)
    for operation in op:
        action = st.Action()
        tag = operation.tag
        tag = tag.replace('{http://www.sbml.org/sbml/level3}','')
        if operation.get('site1') != None:
            action.setAction(tag, operation.get('site1'), operation.get('site2'))
        else:
            action.setAction(tag, operation.get('site'), None)
        actions.append(action)
    for mapping in mp:
        tmpMap = (mapping.get('sourceID'), mapping.get('targetID'))
        mappings.append(tmpMap)
    return reactants, products, actions, mappings, nameDict
    
def parseMolecules(molecules):
    '''
    Parses an XML molecule section
    Returns: a molecule structure
    '''
    mol = st.Molecule(molecules.get('name'),molecules.get('id'))
    components = \
      molecules.find('.//{http://www.sbml.org/sbml/level3}ListOfComponentTypes')
    if components != None:
        for component in components.getchildren():
            comp = st.Component(component.get('name'),component.get('id'))
            mol.addComponent(comp)
    return mol       
        

def parseXML(xmlFile):
    doc = etree.parse(xmlFile)
    molecules = doc.findall('.//{http://www.sbml.org/sbml/level3}MoleculeType')
    rules = doc.findall('.//{http://www.sbml.org/sbml/level3}ReactionRule')
    ruleDescription = []
    moleculeList = []
    for molecule in molecules:
        moleculeList.append(parseMolecules(molecule))
        
    for rule in rules:
        ruleDescription.append(parseRule(rule))
    
    return moleculeList, ruleDescription
        
if __name__ == "__main__":
    parseXML("fceri.xml")