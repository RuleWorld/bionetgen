import componentGroups
import argparse
import pprint
from collections import defaultdict
import itertools
from copy import copy
from utils import readBNGXML




class ModelLearning:
    def __init__(self, fileName,rawFileName=None):
        self.molecules, self.rules, _ = readBNGXML.parseXML(fileName)
        self.dependencies, self.patternXreactions = componentGroups.getContextRequirements(fileName, collapse=False)
        self.transposePatternsReactions()
        self.reverseDependencies = componentGroups.reverseContextDict(self.dependencies)
        self.moleculeMotifDict, self.motifMoleculeDict = self.classifyPairsByMotif(self.reverseDependencies)

        if rawFileName:
            self.processRawSBML(rawFileName)
        else:
            self.rawRules = {}

    def processRawSBML(self,inputfile):
        _, rawrules, _ = readBNGXML.parseXML(inputfile)
        self.rawRules = {x[0].label: x[0] for x in rawrules}

    def transposePatternsReactions(self):
        self.reactionsXpatterns = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
        for product in self.patternXreactions:
            for reactionCenter in self.patternXreactions[product]:
                for contextcomponent in self.patternXreactions[product][reactionCenter]:
                    for contextState in self.patternXreactions[product][reactionCenter][contextcomponent]:
                        for reaction in self.patternXreactions[product][reactionCenter][contextcomponent][contextState]:
                            self.reactionsXpatterns[reaction][product][reactionCenter].append((contextcomponent, contextState))

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
        complexRules = []
        for rule in self.rules:
            if len([x for x in rule[0].actions if x.action not in ['ChangeCompartment']]) >= threshold:
                complexRules.append(rule)
        return complexRules

    def analyzeComplexReactions(self, threshold=2):
        def getActionableComponentPartners(actions, molecule):

            actionableComponents = []
            for action in actions:
                if action[1] and action[1] in molecule.lower():
                    actionableComponents.append(action[2])
                if action[2] and action[2] in molecule.lower():
                    actionableComponents.append(action[1])
            return actionableComponents
        for reaction in self.getComplexReactions():
            #analyze reactions with cis-allostery (e.g. two actions sites are on the same molecule)
            if len([x for x in self.reactionsXpatterns[reaction[0].label] if len(self.reactionsXpatterns[reaction[0].label][x]) > 1]) ==0:
                continue
            print '----------'
            if reaction[0].label in self.rawRules:
                print str(self.rawRules[reaction[0].label])
                print '>>>>>>'
                print str(reaction[0])
            else:
                print str(reaction[0].label)
            #print str(reaction[0])
            resolvedActions = []
            print 'Actions:'
            changeFlag = 0
            for action in reaction[0].actions:
                molecule1 = reaction[-3]['_'.join(action.site1.split('_')[:-1])] if action.site1 else ''
                molecule2 = reaction[-3]['_'.join(action.site2.split('_')[:-1])] if action.site2 else ''
                site1 = reaction[-3][action.site1] if action.site1 else ''
                site2 = reaction[-3][action.site2] if action.site2 else ''
                print '\t{0}= {1}({2}), {3}({4})'.format(action.action,molecule1,site1,molecule2,site2)
                if action.action == 'DeleteBond':
                    changeFlag = 1
                resolvedActions.append([action.action,site1,site2])

            print 'Context:'
            for reactionCenter in self.reactionsXpatterns[reaction[0].label]:
                #cys rules

                if len(self.reactionsXpatterns[reaction[0].label][reactionCenter]) > 1:
                    for state in self.reactionsXpatterns[reaction[0].label][reactionCenter]:
                        #we will focus on statechange actions for now
                        if state[2] not in ['']:
                            #print self.patternXreactions[reactionCenter]
                            actionableComponents = getActionableComponentPartners([x for x in resolvedActions if x[0] in ['AddBond', 'DeleteBond']],reactionCenter)
                            for component in actionableComponents:
                                print '\treaction center <{0}>, context <{1}> in molecule <{2}>:'.format(component,state[0],reactionCenter)
                                print '\t', {x: dict(self.patternXreactions[reactionCenter][(component, changeFlag, '')][x]) \
                                for x in self.patternXreactions[reactionCenter][(component, changeFlag, '')] if x in [state[0],state[0].lower()]}
                            
            print '+++++++++'

def defineConsole():
    """
    defines the program console line commands
    """
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i', '--input', type=str, help='sbml file', required=True)
    parser.add_argument('-r', '--raw', type=str, help='raw sbml file')
    return parser


if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    inputFile = namespace.input

    modelLearning = ModelLearning(namespace.input, namespace.raw)
    #print modelLearning.getMotifFromPair('EGFR','grb2','shc')
    #print modelLearning.getMotifFromPair('Shc','grb2','egfr')
    modelLearning.analyzeComplexReactions()

    #for rule in complexRules:
    #    print str(rule[0])
    #pprint.pprint(modelLearning.getPairsFromMotif('independent', 'nullrequirement'))
