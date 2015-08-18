import componentGroups
import argparse
import pprint
from collections import defaultdict
import itertools
from copy import copy


def defineConsole():
    """
    defines the program console line commands
    """
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i', '--input', type=str, help='settings file', required=True)
    return parser


class ModelLearning:
    def __init__(self, fileName):
        self.dependencies, self.patternXreactions = componentGroups.getContextRequirements(fileName, collapse=False)
        self.reverseDependencies = componentGroups.reverseContextDict(self.dependencies)
        self.moleculeMotifDict, self.motifMoleculeDict = self.classifyPairsByMotif(self.reverseDependencies)

    def classifyPairsByMotif(self, reverseDependencies):
        """
        recives a dict arranged by molecule->componentPair->dependency

        organizes molecule pairs by a concept called 'motif'
        """
        motifDependencies = defaultdict(lambda: defaultdict(list))
        motifDependencies2 = defaultdict(lambda: defaultdict(list))

        for molecule in reverseDependencies:
            for moleculePair in reverseDependencies[molecule]:
                motifDependencies[molecule][frozenset(moleculePair)].append(reverseDependencies[molecule][moleculePair])

        for molecule in motifDependencies:
            for moleculePair in motifDependencies[molecule]:
                requirementPair = tuple(sorted(motifDependencies[molecule][moleculePair]))

                motifDependencies2[requirementPair][molecule].append(moleculePair)

        return motifDependencies, motifDependencies2

    def getMotifFromPair(self, molecule, component1, component2):
        return self.moleculeMotifDict[molecule][frozenset([component1, component2])]

    def getParticipatingReactions(self, molecule, componentPair, reactionDictionary):
        correlationList = {}
        for moleculeName in reactionDictionary:
            if moleculeName.startswith(molecule + '%'):
                for component in reactionDictionary[moleculeName]:
                    if component[0] in componentPair and (component[1] == 1 or component[2] not in ['0', 0, '']):
                        for componentComplement in [x for x in reactionDictionary[moleculeName][component] if x in componentPair]:
                            correlationList[(component[0], componentComplement)] = (reactionDictionary[moleculeName][component][componentComplement])
        return correlationList

    def getPairsFromMotif(self, motif1, motif2):
        moleculeCorrelationList = defaultdict(dict)
        for element in self.motifMoleculeDict:
            if motif1 in element and motif2 in element:
                for molecule in self.motifMoleculeDict[element]:
                    correlationList = {}
                    if len(self.motifMoleculeDict[element][molecule]) > 0:
                        for componentPair in self.motifMoleculeDict[element][molecule]:
                            correlationList[componentPair] = self.getParticipatingReactions(molecule, componentPair, self.patternXreactions)
                    moleculeCorrelationList[molecule].update(correlationList)
        return dict(moleculeCorrelationList)

    def analyzeRedundantBonds(self, assumptions):
        """
        Analyzes a system of molecules with redundant bonds between them (more than one path between any two nodes in the system). The function
        attemps to score the bonds by looking out for partial competition relationships (e.g. the presence of one component
        excludes the activation of another, but in the other direction we see independence) which are less likely than a fully independence
        relationship. The method will thus nominate such edges for deletion if the nodes still form a fully connected graph after their removal
        """

        def fullyConnectedGraph(nodes, edges):
            """
            Lazy implementation. This only works if there is one common elements to all subgraphs
            """
            if edges == []:
                return False
            tmpNodeList = [set(x) for x in edges]
            superGraph = set.intersection(*map(set, edges))
            if len(superGraph) > 0:
                return True
            return False

        conserveBonds = []

        deleteBonds = {}
        for redundantBondSet in assumptions:
            allBonds = [sorted(x) for x in itertools.combinations(redundantBondSet, 2)]
            conserveBonds = []
            for molecule in redundantBondSet:
                for x in itertools.combinations([x for x in redundantBondSet if x != molecule], 2):
                    contextMotif = self.getMotifFromPair(molecule, x[0].lower(), x[1].lower())
                    if 'independent' in contextMotif and not ('requirement' in contextMotif or 'nullrequirement' in contextMotif):
                        conserveBonds.append(sorted([molecule, x[0]]))
                        conserveBonds.append(sorted([molecule, x[1]]))
            if fullyConnectedGraph(redundantBondSet,conserveBonds):
                deleteBonds[redundantBondSet] = [x for x in allBonds if x not in conserveBonds]
        return deleteBonds

    def scoreHypotheticalBonds(self, assumptions):
        """
        TODO: we need some way to evaluate the confidence in a bond based on context information
        """
        pass

    def getComplexReactions(self, threshold=2):
        pass


if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    inputFile = namespace.input

    modelLearning = ModelLearning(namespace.input)
    #print modelLearning.getMotifFromPair('EGFR','grb2','shc')
    #print modelLearning.getMotifFromPair('Shc','grb2','egfr')
    pprint.pprint(modelLearning.getPairsFromMotif('independent', 'nullrequirement'))
