from utils import readBNGXML
import argparse
from collections import defaultdict
from cStringIO import StringIO

def molecule2stateTuples(molecule):
    """
    Receives a molecule structure, returns a tuple detailing the state of the contained states
    """
    tupleList = []
    for component in molecule.components:
        componentDefinition = [component.name]
        componentDefinition.append(1 if len(component.bonds) > 0 else 0)
        componentDefinition.append('' if len(component.states) == 0 else component.activeState)
        tupleList.append(tuple(componentDefinition))
    return tupleList

def getChemicalStates(rules):
    """
    goes through a list of rules and creates a dictionary (molecules) of possible chemical states of its components
    """
    chemicalStates = defaultdict(list)
    for rule in rules:
        for chemicalSet in [rule[0].reactants,rule[0].products]:
            for chemical in chemicalSet:
                for molecule in chemical.molecules:
                    stateTuple =  molecule2stateTuples(molecule)
                    #if ('grb2',1,'') in stateTuple and molecule.name == 'EGFR' and ('shc',1,'') in stateTuple:
                    #    print '---',str(rule[0])
                    chemicalStates[molecule.name].append(stateTuple)

    return chemicalStates 

def sortChemicalStates(chemicalStates):
    """
    sorts a dictionary of chemicalStates into a dictionary of molecules of chemicalStates 
    of their associated-same-molecule-components (and their states)
    """
    sortedChemicalStates = defaultdict(lambda : defaultdict(lambda :defaultdict(set)))
    for molecule in chemicalStates:
        for stateInstance in chemicalStates[molecule]:
            for componentState in stateInstance:
                for componentState2 in stateInstance:
                    if componentState == componentState2:
                        continue
                    sortedChemicalStates[molecule][componentState][componentState2[0]].add(componentState2[1:])
    return sortedChemicalStates

def isActive(state):
    if state[0] == 1 or state[1] not in ['','0']:
        return True
    return False

def componentStateSize(molecules,moleculeName,componentName):
    for molecule in molecules:
        if molecule.name == moleculeName:
            for component in molecule.components:
                if component.name == componentName:
                    return max(2,len(component.states))

def analyzeDependencies(componentStateCollection,state,moleculeName,molecules,dependencies):
    for componentName in componentStateCollection:
        stateSize = componentStateSize(molecules,moleculeName,componentName)

        if stateSize == len(componentStateCollection[componentName]):
            dependencies[moleculeName]['independent'].add((state,componentName))
        elif len(componentStateCollection[componentName]) == 1:
            activeState = list(componentStateCollection[componentName])[0]
            if isActive((state[1],state[2])) and isActive(activeState):
                dependencies[moleculeName]['requirement'].add(((componentName,activeState[0],activeState[1]),state))
            elif isActive((state[1],state[2])) and not isActive(activeState):
                dependencies[moleculeName]['exclusion'].add(((state,(componentName,activeState[0],activeState[1]))))
            elif (not isActive((state[1],state[2]))) and not isActive(activeState):
                dependencies[moleculeName]['nullrequirement'].add(((state,(componentName,activeState[0],activeState[1]))))
            elif (not isActive((state[1],state[2]))) and isActive(activeState):
                dependencies[moleculeName]['exclusion'].add((((componentName,activeState[0],activeState[1])),state))
def detectDependencies(stateDictionary,molecules):
    dependencies = defaultdict(lambda: defaultdict(set))
    for moleculeName in stateDictionary:
        for state in stateDictionary[moleculeName]:
            analyzeDependencies(stateDictionary[moleculeName][state],state,moleculeName,molecules,dependencies)
    return dependencies

def getMutualExclusions(stateDictionary):
    mutualExclusion = []
    for molecule in stateDictionary:
        for partner in stateDictionary[molecule]['exclusion']:
                print molecule,partner

def removeIndirectDependencies(dependencies,stateSpace):
    """
    goes through the list of dependencies and search for those states that requires more than 1 condition.
    If true, it will check if any of the set of prerequirements is a sufficient conditions for any of the other prerequirements
    to be valid. If true it will remove those redundant prerequirements them
    """
    indirect = defaultdict(set)

    for molecule in dependencies:
        for requirement in set([x[1] for x in dependencies[molecule]['requirement']]):
            prerequirements = [x[0] for x in dependencies[molecule]['requirement'] if requirement == x[1]]
            #if a particular state has more than one requirement 
            if len(prerequirements) > 1:
                relevantStateSpace ={x:stateSpace[molecule][x] for x in prerequirements} 
                for rsp in relevantStateSpace:
                    for partner in relevantStateSpace[rsp]:
                        partnerDefinition = list(relevantStateSpace[rsp][partner])
                        #is any single prerequirement a sufficient condition for any other other ones?
                        if len(partnerDefinition) == 1 and (partner,partnerDefinition[0][0],partnerDefinition[0][1]) in prerequirements:
                            #if so mark for deletion
                            indirect[molecule].add(((partner,partnerDefinition[0][0],partnerDefinition[0][1]),requirement))
    for molecule in indirect:
        for indirectDependency in indirect[molecule]:
            dependencies[molecule]['requirement'].remove(indirectDependency)

def formatComponent(state,tense):
    if state[0] == 1:
        if tense == 'past':
            compStr = 'be bound'
        else:
            compStr= 'bind'
    else:
        compStr = 'be in state {0}'.format(state[1])
    return compStr


def printDependencyLog(dependencies):
    log = StringIO()
    for molecule in dependencies:
        for requirementType in dependencies[molecule]:
            if requirementType in ['independent','nullrequirement']:
                continue
            for baseMolecule in dependencies[molecule][requirementType]:
                if requirementType == 'requirement':
                    log.write('Molecule {0} needs component {1} to {2} for component {3} to {4}\n'.format(molecule,baseMolecule[0][0],
                       formatComponent(baseMolecule[0][1:],'past'),baseMolecule[1][0],formatComponent(baseMolecule[1][1:],'present')))
                elif requirementType == 'exclusion':
                    log.write('In molecule {0} component {1} is exclusive of component {2}\n'.format(molecule,baseMolecule[0][0],
                        baseMolecule[1][0]))
    return log.getvalue()

def getContextRequirements(inputfile):
    """
    Receives a BNG-XML file and returns the contextual dependencies implied by this file
    """
    molecules, rules, _ = readBNGXML.parseXML(inputfile)

    chemicalStates = getChemicalStates(rules)
    stateDictionary = sortChemicalStates(chemicalStates)
    
    dependencies = detectDependencies(stateDictionary,molecules)
    #getMutualExclusions(dependencies)
    removeIndirectDependencies(dependencies,stateDictionary)
    return dependencies

def defineConsole():
    """
    defines the program console line commands
    """
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i','--input',type=str,help='settings file',required=True)
    return parser    



if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    inputFile = namespace.input

    dependencies = getContextRequirements(inputFile)
    print printDependencyLog(dependencies)
    

