# -*- coding: utf-8 -*-
"""
Created on Mon Nov 19 14:28:16 2012

@author: proto
"""
from lxml import etree
import smallStructures as st
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
            return str(idx)


    
def createMolecule(molecule, bonds):
    nameDict = {}
    mol_id = molecule.get('id')
    mol_name = molecule.get('name')
    mol = st.Molecule(mol_name, mol_id)
    mol_comp = molecule.get('compartment')
    if mol_comp not in ['', None]:
        mol.setCompartment(mol_comp)
    nameDict[mol_id] = mol_name
    listOfComponents =  _fast_find(molecule, 'ListOfComponents')
    if listOfComponents != None:
        for element in listOfComponents:
            elem_id = element.get('id')
            elem_name = element.get('name')
            elem_bonds = element.get('numberOfBonds')
            elem_state = element.get('state')

            component = st.Component(elem_name, elem_id)
            nameDict[elem_id] = elem_name

            if elem_bonds in ['+', '?']:
                component.addBond(elem_bonds)
            elif elem_bonds != '0':
                component.addBond(findBond(bonds, elem_id))

            state = elem_state if elem_state != None else ''
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
            bond_map[bond.get('site1')] = str(idx)
            bond_map[bond.get('site2')] = str(idx)
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
    
    

def parseRule(rule,parameterDict):
    '''
    Parses a rule XML section
    Returns: a list of the reactants and products used, followed by the mapping
    between the two and the list of operations that were performed
    '''
    rp = _fast_find(rule, 'ListOfReactantPatterns')
    pp = _fast_find(rule, 'ListOfProductPatterns')
    mp = _fast_find(rule, 'Map')
    op = _fast_find(rule, 'ListOfOperations')
    rt = _fast_find(rule, 'RateLaw')
    nameDict = {}
    reactants = []
    products = []
    actions = []
    mappings = []
    
    if len(rp) == 0:
        sp = st.Species()
        ml = st.Molecule('0','')
        sp.addMolecule(ml)
        reactants.append(sp)
    if len(pp) == 0:
        sp = st.Species()
        ml = st.Molecule('0','')
        sp.addMolecule(ml)
        products.append(sp)
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
    rateConstants = _fast_find(rt, 'ListOfRateConstants')
    if rateConstants == None:
        rateConstants = rt.get('name')
    else:
        for constant in rateConstants:
            tmp = constant.get('value')
        rateConstants = tmp
    rateConstantsValue = parameterDict[rateConstants] if rateConstants in parameterDict else rateConstants
    #rule = st.Rule()   
    label = rule.get('name')
    label = label.replace('(','_').replace(')','_')
    rule = st.Rule(label)
    rule.addReactantList(reactants)
    rule.addProductList(products)
    rule.addActionList(actions)
    rule.addMappingList(mappings)
    rule.addRate(rateConstants)
    
    #return reactants, products, actions, mappings, nameDict,rateConstantsValue,rateConstants
    return rule,nameDict,rateConstantsValue,rateConstants
    
def parseMolecules(molecules):
    '''
    Parses an XML molecule section
    Returns: a molecule structure
    '''

    mol = st.Molecule(molecules.get('id'),molecules.get('id'))
    components = \
      _fast_find(molecules, 'ListOfComponentTypes')
    if components != None:
        for component in components:
            comp = parseComponent(component)
            mol.addComponent(comp)
    return mol       
        
def parseComponent(component):
    '''
    parses  a bngxml molecule types section
    '''
    comp = st.Component(component.get('id'),component.get('id'))
    states = _fast_find(component, 'ListOfAllowedStates')
    if states != None:
        for state in states:
            comp.addState(state.get('id'))
    return comp
    
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
        lop = model.find('{http://www.sbml.org/sbml/level3}ListOfParameters')
        if lop is not None:
            parameters = lop.findall('{http://www.sbml.org/sbml/level3}Parameter')
        loo = model.find('{http://www.sbml.org/sbml/level3}ListOfObservables')
        if loo is not None:
            observables = loo.findall('{http://www.sbml.org/sbml/level3}Observable')

    ruleDescription = []
    moleculeList = []
    parameterDict = {}
    for parameter in parameters:
        parameterDict[parameter.get('id')] = parameter.get('value')

    for molecule in molecules:
        moleculeList.append(parseMolecules(molecule))
        
    for rule in rules:
        description = parseRule(rule,parameterDict)
        #if 'reverse' in description[0].label:
        #    ruleDescription[-1][0].bidirectional= True
        #    ruleDescription[-1][0].rates.append(description[0].rates[0])
        #else:
        ruleDescription.append(parseRule(rule,parameterDict))
    return moleculeList, ruleDescription,parameterDict
        
def getNumObservablesXML(xmlFile):
    parser = etree.XMLParser(resolve_entities=False, no_network=True)
    doc = etree.parse(xmlFile, parser)
    observables = []
    model = doc.getroot().find('{http://www.sbml.org/sbml/level3}model')
    if model is not None:
        loo = model.find('{http://www.sbml.org/sbml/level3}ListOfObservables')
        if loo is not None:
            observables = loo.findall('{http://www.sbml.org/sbml/level3}Observable')
    return len(observables)
    
if __name__ == "__main__":
    #mol,rule,par = parseXML("output19.xml")
    #print [str(x) for x in mol]
    print(getNumObservablesXML('output19.xml'))