import argparse
import componentGroups
from utils import readBNGXML
import utils.structures as st
from collections import defaultdict
import pprint

moleculemapping = {'Epidermal_Growth_Factor': 'EGF'}
componentmapping = {'Raf': {'mmod': 'astMod'}, 'JAK': {'mmod': 'genericMod'}}


from collections import defaultdict

class prettyDict(defaultdict):
    __repr__ = dict.__repr__


def moleculeIntersection(nameSet1, nameSet2, mapping):
    intersection = {}
    for name1 in nameSet1:
        if name1 in nameSet2:
            intersection[name1] = name1
        elif name1 in mapping:
            intersection[name1] = mapping[name1]
    return intersection


def compareModelsStructurally(fileName1, fileName2, mapping,componentMapping):
    molecules, _, _ = readBNGXML.parseXML(fileName1)
    molecules2, _, _ = readBNGXML.parseXML(fileName2)

    moleculeName1 = {x.name: x for x in molecules}
    moleculeName2 = {x.name: x for x in molecules2}
    intersectionDict = {}

    nameIntersection = moleculeIntersection(moleculeName1.keys(), moleculeName2.keys(), mapping)
    componentNameIntersection = defaultdict(lambda: defaultdict(str))
    for molecule in nameIntersection:
        
        for component in moleculeName1[molecule].components:
            if component.name in [x.name for x in moleculeName2[nameIntersection[molecule]].components]:
                    componentNameIntersection[nameIntersection[molecule]][component.name] = component.name
            elif molecule in componentMapping and component.name in componentMapping[molecule]:
                componentNameIntersection[nameIntersection[molecule]][component.name.lower()] = componentMapping[molecule][component.name].lower()
    return nameIntersection,componentNameIntersection

def discoverDifferences(contextMatrix, fileName1, fileName2):
    criticalComponents = prettyDict(list)
    for molecule in contextMatrix:
        for component1 in contextMatrix[molecule]:
            for component2 in contextMatrix[molecule][component1]:
                if len(contextMatrix[molecule][component1][component2]) != 2 or contextMatrix[molecule][component1][component2][fileName1] != contextMatrix[molecule][component1][component2][fileName2]:
                    criticalComponents[molecule].append([(component1,component2),contextMatrix[molecule][component1][component2]])
    return criticalComponents


def hypothesisGenerator(premises, fileName1, fileName2):
    experiments = []
    serializedPremise = premises[1].items()
    if len(premises[1]) == 1:
        targetFileName = fileName2 if fileName1 in premises[1] else fileName1

        if serializedPremise[0][1] == 'requirement':
            sourceMolecule = [premises[0][0], 1]
            targetMolecule = [premises[0][1], 0]
            experiments.append([targetFileName, sourceMolecule, targetMolecule])
        elif serializedPremise[0][1] == 'nullrequirement':
            sourceMolecule = [premises[0][0], 1]
            targetMolecule = [premises[0][1], 1]
            experiments.append([targetFileName, sourceMolecule, targetMolecule])
        elif serializedPremise[0][1] == 'exclusion':
            sourceMolecule = [premises[0][0], 1]
            targetMolecule = [premises[0][1], 1]
            experiments.append([targetFileName, sourceMolecule, targetMolecule])
            sourceMolecule = [premises[0][1], 1]
            targetMolecule = [premises[0][0], 1]
            experiments.append([targetFileName, sourceMolecule, targetMolecule])

    elif len(premises[1]) == 2:
        if serializedPremise[0][1] == 'nullrequirement':
            if serializedPremise[1][1] == 'exclusion':
                targetFileName = serializedPremise[1][0]
                sourceMolecule = [premises[0][1], 1]
                targetMolecule = [premises[0][0], 0]
                experiments.append([targetFileName, sourceMolecule, targetMolecule])
        elif serializedPremise[0][1] == 'exclusion':
            if serializedPremise[1][1] == 'nullrequirement':
                targetFileName = serializedPremise[0][0]
                sourceMolecule = [premises[0][0], 1]
                targetMolecule = [premises[0][1], 0]
                experiments.append([targetFileName, sourceMolecule, targetMolecule])

    return experiments


def lookupPatterns(graph, center, context, molecule, componentNameIntersection):
    counter = 0
    relevantRules = set([])
    while '{0}%{1}'.format(molecule, counter) in graph:
        for component in graph['{0}%{1}'.format(molecule, counter)]:
            if center[0] in component:
                for component2 in graph['{0}%{1}'.format(molecule, counter)][component]:
                    if component2 == context[0]:
                        for activationStatus in graph['{0}%{1}'.format(molecule, counter)][component][component2]:
                            if context[1] == 1 and (activationStatus[0] == 1 or activationStatus[1] not in ['', '0']):
                                relevantRules = relevantRules.union(set(graph['{0}%{1}'.format(molecule, counter)][component][component2][activationStatus]))
                            elif context[1] == 0 and (activationStatus[0] == 0 and activationStatus[1] in ['', '0']):
                                relevantRules = relevantRules.union(set(graph['{0}%{1}'.format(molecule, counter)][component][component2][activationStatus]))

        counter += 1
    return relevantRules


def convertExperimentNames(componentNameIntersection, molecule, experiment, firstFileFlag):
    trueExperiment = [experiment[0]]

    for element in experiment[1:]:
        if not firstFileFlag:
            trueExperiment.append(element)
        else:
            for component in componentNameIntersection[molecule]:
                if componentNameIntersection[molecule][component] == element[0]:
                    trueExperiment.append([component, element[1]])
                    break

    return trueExperiment

def obtainContextDifferences(fileName1, fileName2, moleculeNameIntersection, componentNameIntersection):
    file1Context, relationshipGraph1 = componentGroups.getContextRequirements(fileName1,False)
    file2Context, relationshipGraph2 = componentGroups.getContextRequirements(fileName2,False)   

    #contextMatrix1 = {molecule:[[0]*len(componentNameIntersection[molecule]) for x in componentNameIntersection[molecule]]  for molecule in moleculeNameIntersection}
    contextMatrix1 = prettyDict(lambda: prettyDict(lambda: prettyDict(dict)))
    #contextMatrix2 = prettyDict(lambda: prettyDict(lambda: prettyDict(list)))

    for key in moleculeNameIntersection:
        for relationship in file1Context[key]:
            for element in file1Context[key][relationship]:
                if relationship == 'independent':
                    continue
                    #moleculePair = [element[0][0],element[1]]
                else:
                    moleculePair = [element[0][0],element[1][0]]
                    #contextMatrix[element[0]][element[1]] = relationship
                    #continue

                if moleculePair[0].lower() in componentNameIntersection[moleculeNameIntersection[key]] and moleculePair[1].lower() in componentNameIntersection[moleculeNameIntersection[key]]:
                    contextMatrix1[moleculeNameIntersection[key]][componentNameIntersection[moleculeNameIntersection[key]][moleculePair[1].lower()]][componentNameIntersection[moleculeNameIntersection[key]][moleculePair[0].lower()]][fileName1] = relationship

        for relationship in file2Context[moleculeNameIntersection[key]]:
            for element in file2Context[moleculeNameIntersection[key]][relationship]:
                if relationship == 'independent':
                    continue
                    #moleculePair = [element[0][0],element[1]]
                else:
                    moleculePair = [element[0][0], element[1][0]]

                if moleculePair[0].lower() in [componentNameIntersection[moleculeNameIntersection[key]][x] for x in componentNameIntersection[moleculeNameIntersection[key]]] and \
                moleculePair[1].lower() in [componentNameIntersection[moleculeNameIntersection[key]][x] for x in componentNameIntersection[moleculeNameIntersection[key]]]:
                    contextMatrix1[moleculeNameIntersection[key]][moleculePair[1].lower()][moleculePair[0].lower()][fileName2] = relationship

    differences = discoverDifferences(contextMatrix1, fileName1, fileName2)
    relationshipGraphDict = {fileName1: relationshipGraph1, fileName2: relationshipGraph2}
    fileContextDict = {fileName1: file1Context,fileName2: file2Context}


    for molecule in differences:
        for componentPair in differences[molecule]:
            counter = 0
            hypothesis =  hypothesisGenerator(componentPair, fileName1, fileName2)
            experimentResults = prettyDict(list)
            for experiment in hypothesis:
                trueExperiment = convertExperimentNames(componentNameIntersection, molecule, experiment, experiment[0] == fileName1)
                rules = lookupPatterns(relationshipGraphDict[trueExperiment[0]], trueExperiment[1], trueExperiment[2] ,molecule, componentNameIntersection)
                experimentResults[trueExperiment[0]].append([trueExperiment[1:],rules])
            componentPair.append(experimentResults)
    return differences


def evaluateDifferences(differences, fileName1, fileName2, fileName1raw, fileName2raw):
    for molecule in differences:
        for differenceInstance in differences[molecule]:
            pass


def main(fileName1, fileName2, fileName1raw, fileName2raw):

    moleculeNameIntersection, componentNameIntersection = compareModelsStructurally(fileName1, fileName2, moleculemapping, componentmapping)
    
    differences = obtainContextDifferences(fileName1, fileName2, moleculeNameIntersection, componentNameIntersection)

    #evaluateDifferences(differences, fileName1, fileName2, fileName1raw, fileName2raw)
    pprint.pprint(differences)


def defineConsole():
    """
    defines the program console line commands
    """
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-f1', '--file1', type=str, help='reference file', required=True)
    parser.add_argument('-f2', '--file2', type=str, help='comparison file', required=True)
    parser.add_argument('-f1aux', '--file1-aux', type=str, help='reference file', required=True)
    parser.add_argument('-f2aux', '--file2-aux', type=str, help='comparison file', required=True)

    return parser



if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()

    main(namespace.file1, namespace.file2,namespace.file1_aux,namespace.file2_aux)
