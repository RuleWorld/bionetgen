# -*- coding: utf-8 -*-
"""
Created on Mon Nov 19 14:28:16 2012

@author: proto
"""
from lxml import etree
import structures as st
 #http://igraph.sourceforge.net/documentation.html
#----------------------------------------------------------------------

def _fast_find(element, tag_name):
    """
    Performance optimization: attempting direct child lookup first is O(children)
    compared to the recursive .// search which is O(tree).
    """
    res = element.find('{http://www.sbml.org/sbml/level3}' + tag_name)
    if res is None:
        res = element.find('.//{http://www.sbml.org/sbml/level3}' + tag_name)
    return res



def findBond(bondDefinitions, component):
    '''
    Returns an appropiate bond number when veryfying how 
    to molecules connect in a species
    '''
    if isinstance(bondDefinitions, dict):
        return bondDefinitions.get(component)
    for idx, bond in enumerate(bondDefinitions):
        if component in [bond.get('site1'), bond.get('site2')]: 
            return str(idx+1)
    
def createMolecule(molecule, bonds):
    nameDict = {}
    mol = st.Molecule(molecule.get('name'),molecule.get('id'))
    nameDict[molecule.get('id')] = molecule.get('name')
    listOfComponents =  _fast_find(molecule, 'ListOfComponents')
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
    mol = _fast_find(pattern, 'ListOfMolecules')
    bonds = _fast_find(pattern, 'ListOfBonds')

    bond_map = {}
    if bonds is not None:
        for idx, bond in enumerate(bonds):
            bond_map[bond.get('site1')] = str(idx + 1)
            bond_map[bond.get('site2')] = str(idx + 1)
    else:
        bond_map = None

    for molecule in mol:
        molecule, nameDict = createMolecule(molecule, bond_map)
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
    rp = _fast_find(rule, 'ListOfReactantPatterns')
    pp = _fast_find(rule, 'ListOfProductPatterns')
    mp = _fast_find(rule, 'Map')
    op = _fast_find(rule, 'ListOfOperations')
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
        if tag in ['Add','Delete']:
                action.setAction(tag,operation.get('id'),None)
        elif 'Bond' in tag:
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
      _fast_find(molecules, 'ListOfComponentTypes')
    if components != None:
        for component in components:
            comp = st.Component(component.get('name'),component.get('id'))
            mol.addComponent(comp)
    return mol       
        

def parseXML(xmlFile):
    parser = etree.XMLParser(resolve_entities=False, no_network=True)
    doc = etree.parse(xmlFile, parser)

    model = doc.getroot().find('{http://www.sbml.org/sbml/level3}model')
    molecules = []
    rules = []
    parameters = []
    observables = []
    if model is not None:
        lom = model.find('{http://www.sbml.org/sbml/level3}ListOfMoleculeTypes')
        if lom is not None:
            molecules = lom.findall('{http://www.sbml.org/sbml/level3}MoleculeType')
        lor = model.find('{http://www.sbml.org/sbml/level3}ListOfReactionRules')
        if lor is not None:
            rules = lor.findall('{http://www.sbml.org/sbml/level3}ReactionRule')

    ruleDescription = []
    moleculeList = []
    for molecule in molecules:
        moleculeList.append(parseMolecules(molecule))
        
    for rule in rules:
        ruleDescription.append(parseRule(rule))
    
    return moleculeList, ruleDescription
        
if __name__ == "__main__":
    parseXML("fceri.xml")
