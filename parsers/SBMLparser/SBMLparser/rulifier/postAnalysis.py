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
        self.dependencies, self.patternXreactions,_ = componentGroups.getContextRequirements(fileName, collapse=False)
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

    def getPairsFromMotif(self, motif1, motif2, excludedComponents):
        moleculeCorrelationList = defaultdict(dict)
        for element in self.motifMoleculeDict:
            if motif1 in element and motif2 in element:
                if motif1 == motif2 and len([x for x in element if x == motif1]) < 2:
                    continue
                for molecule in self.motifMoleculeDict[element]:
                    correlationList = {}
                    if len(self.motifMoleculeDict[element][molecule]) > 0:
                        for componentPair in self.motifMoleculeDict[element][molecule]:
                            if not any(x in excludedComponents for x in componentPair):
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
            if fullyConnectedGraph(redundantBondSet, conserveBonds):
                deleteBonds[redundantBondSet] = [x for x in allBonds if x not in conserveBonds]
        return deleteBonds

    def scoreHypotheticalBonds(self, assumptions):
        """
        TODO: we need some way to evaluate the confidence in a bond based on context information
        """
        pass

    def resolveEntry(self, dependencyGraph, moleculeSet):
        """
        resolve an entry to its basic components according to dependency graph
        """
        if type(moleculeSet) == str:
            return [moleculeSet]
        if len(moleculeSet) == 1 and dependencyGraph[moleculeSet[0]] == []:
            return moleculeSet
        compositionList = []
        for molecule in moleculeSet:
            compositionList.extend(self.resolveEntry(dependencyGraph, dependencyGraph[molecule][0]))
        return compositionList

    def getDifference(self, pattern1, pattern2, translator):
        if pattern1 not in translator or pattern2 not in translator:
            return None

        species1 = translator[pattern1]
        species2 = translator[pattern2]
        species1.sort()
        species2.sort()
        componentDifference = []
        for molecule1,molecule2 in zip(species1.molecules,species2.molecules):
            for component1,component2 in zip(molecule1.components,molecule2.components):
                if len(component1.bonds) != len(component2.bonds) or component1.activeState != component2.activeState:
                    componentDifference.append(component1.name)
        return componentDifference

    def processContextMotifInformation(self, assumptionList, database):
        def getClassification(keys, translator):
            flags = [key in [x.lower() for x in translator.keys()] for key in keys]
            if all(flags):
                return 'binding-binding'
            elif any(flags):
                return 'modification-binding'
            return 'modification-modification'

        motifInformationDict = self.getContextMotifInformation()

        motifFinalLog = defaultdict(set)
        motifReactionDefinitions = {}
        for motifInformation in motifInformationDict:
            # iterate over motifs that are known to be problematic
            if motifInformation in [frozenset(['nullrequirement', 'independent']), frozenset(['requirement', 'independent'])]:

                for molecule in motifInformationDict[motifInformation]:
                    if len(motifInformationDict[motifInformation][molecule]) == 0:
                        continue
                    # if the candidate definitions for a given compound are related to a molecule with problematic motifs
                    for assumption in (x for x in assumptionList for y in eval(x[3][1]) for z in y if molecule in z):
                        candidates = eval(assumption[1][1])
                        alternativeCandidates = eval(assumption[2][1])
                        original = eval(assumption[3][1])
                        # further confirm that the change is about the pair of interest
                        # by iterating over all candidates and comparing one by one
                        for candidate in candidates:
                            for alternativeCandidate in alternativeCandidates:
                                difference = [x for x in candidate if x not in alternativeCandidate]
                                difference.extend([x for x in alternativeCandidate if x not in candidate])
                                # if we are referencing a molecule that is not about this particular context change 
                                # dont store it in the motif/species table, just keep information about the motif alone
                                localAnalysisFlag = True
                                if not any([molecule in database.prunnedDependencyGraph[x][0] if
                                            len(database.prunnedDependencyGraph[x]) > 0 else molecule
                                            in x for x in difference]):
                                    localAnalysisFlag = False
                                #    continue
                                if localAnalysisFlag:
                                    # get those elements that differ between the two candidates and that correspond to the current <molecule> being analyzed
                                    difference = [x for x in candidate if x not in alternativeCandidate and self.resolveEntry(database.prunnedDependencyGraph, [x])[0] == molecule]
                                    alternativeDifference = [x for x in alternativeCandidate if x not in candidate and molecule in self.resolveEntry(database.prunnedDependencyGraph, [x])[0]]

                                    # get the difference patterns for the two species
                                    if not difference or not alternativeDifference:
                                        continue
                                    componentDifference = self.getDifference(difference[0], alternativeDifference[0], database.translator)

                                # make sure that the current motif candidate intersects with the difference pattern
                                for keys in motifInformationDict[motifInformation][molecule]:
                                    if localAnalysisFlag and any(key in componentDifference for key in keys):
                                        motifFinalLog['{0}({1})'.format(molecule, ', '.join(keys))].add(assumption[0])
                                    classification = getClassification(keys, database.translator)
                                    if classification not in motifReactionDefinitions:
                                        motifReactionDefinitions[classification] = {}
                                    motifReactionDefinitions[classification]['{0}({1})'.format(molecule, ', '.join(keys))] = (motifInformation, motifInformationDict[motifInformation][molecule][keys])
                                    
        #pprint.pprint(dict(motifFinalLog))
        #pprint.pprint(motifReactionDefinitions)
        return motifFinalLog, motifReactionDefinitions

    def getContextMotifInformation(self):
        '''
        returns the reactions in the system classified by context-component motif pairs. e.g. a requirement,nullrequirement
        motif pair is a pair of components A->B such that B needs for A to be activated to activate, whereas A needs for B
        to be inactivated to activate.
        '''
        relationshipCombinations = itertools.combinations(['independent', 'requirement', 'nullrequirement', 'exclusion'], 2)  
        motifDictionary = {}

        for relCombi in relationshipCombinations:
            motifDictionary[frozenset(relCombi)] = self.getPairsFromMotif(relCombi[0], relCombi[1], [])

        for requirementClass in ['independent', 'requirement', 'nullrequirement', 'exclusion']:
            motifDictionary[frozenset([requirementClass, requirementClass])] = self.getPairsFromMotif(requirementClass, requirementClass, [])
        return motifDictionary

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
    #modelLearning.analyzeComplexReactions()

    #for rule in complexRules:
    #    print str(rule[0])
    
    relationshipCombinations = itertools.combinations(['independent', 'requirement', 'nullrequirement', 'exclusion'], 2)    
    motifDictionary = {}
    for relCombi in relationshipCombinations:
        motifDictionary[relCombi] = modelLearning.getPairsFromMotif(relCombi[0], relCombi[1],['imod'])
        if len(motifDictionary[relCombi]) > 0:
            print relCombi, {x:len(motifDictionary[relCombi][x]) for x in motifDictionary[relCombi]}
    for requirementClass in ['independent', 'requirement', 'nullrequirement', 'exclusion']:
        motifDictionary[(requirementClass,requirementClass)] = modelLearning.getPairsFromMotif(requirementClass, requirementClass, ['imod'])
        if len(motifDictionary[(requirementClass, requirementClass)]) > 0:
            print (requirementClass, requirementClass), {x:len(motifDictionary[(requirementClass,requirementClass)][x]) for x in motifDictionary[(requirementClass,requirementClass)]}
    print modelLearning.getPairsFromMotif('independent','requirement',['imod'])
    print '---'
    #print modelLearning.getPairsFromMotif('independent','nullrequirement',['imod'])


