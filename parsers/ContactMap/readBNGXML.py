# -*- coding: utf-8 -*-
"""
Created on Mon Nov 19 14:28:16 2012

@author: proto
"""
from lxml import etree
from lxml import objectify
import structures as st
 #http://igraph.sourceforge.net/documentation.html
#----------------------------------------------------------------------


def findBond(bondDefinitions,component):
    for idx,bond in enumerate(bondDefinitions.getchildren()):
        if component in [bond.get('site1'),bond.get('site2')]: 
            return str(idx+1)
    
def createMolecule(molecule,bonds):
    nameDict = {}
    mol = st.Molecule(molecule.get('id'))
    nameDict[molecule.get('id')] = molecule.get('name')
    for element in molecule.find('.//{http://www.sbml.org/sbml/level3}ListOfComponents'):
        component = st.Component(element.get('id'))
        nameDict[element.get('id')] = element.get('name')
        if element.get('numberOfBonds') in ['+','?']:
            component.addBond(element.get('numberOfBonds'))
        elif element.get('numberOfBonds') != '0':
            component.addBond(findBond(bonds,element.get('id')))
        mol.addComponent(component)
    return mol,nameDict
    

def generateGraph(molecules, rules):
    pass
    
def createSpecies(pattern):
    tmpDict = {}
    species = st.Species()
    mol = pattern.find('.//{http://www.sbml.org/sbml/level3}ListOfMolecules')
    bonds = pattern.find('.//{http://www.sbml.org/sbml/level3}ListOfBonds')
    for molecule in mol.getchildren():
        molecule ,nameDict = createMolecule(molecule,bonds)
        species.addMolecule(molecule)
        tmpDict.update(nameDict)
    return species,tmpDict
    
    

def parseRule(rule):
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
        elm,tmpDict = createSpecies(pattern)
        reactants.append(elm)
        nameDict.update(tmpDict)
    for pattern in pp:
        elm,tmpDict = createSpecies(pattern)
        products.append(elm)
        nameDict.update(tmpDict)
    for operation in op:
        action = st.Action()
        tag = operation.tag
        tag = tag.replace('{http://www.sbml.org/sbml/level3}','')
        action.setAction(tag,operation.get('site1'),operation.get('site2'))
        actions.append(action)
    for mapping in mp:
        tmpMap = (mapping.get('sourceID'),mapping.get('targetID'))
        mappings.append(tmpMap)
    return reactants,products,actions,mappings,nameDict
    
def parseMolecules(molecules):
   mol = st.Molecule(molecules.get('id'))
   components = molecules.find('.//{http://www.sbml.org/sbml/level3}ListOfComponentTypes')
   if components != None:
       for component in components.getchildren():
           mol.addComponent(component.get('id'))
   return mol       
        

def parseXML(xmlFile):
    doc = context = etree.parse(xmlFile)
    molecules = doc.findall('.//{http://www.sbml.org/sbml/level3}MoleculeType')
    rules = doc.findall('.//{http://www.sbml.org/sbml/level3}ReactionRule')
    ruleDescription = []
    moleculeList = []
    for molecule in molecules:
        moleculeList.append(parseMolecules(molecule))
        
    for rule in rules:
        ruleDescription.append(parseRule(rule))

        
if __name__ == "__main__":
    parseXML("BM48.xml")