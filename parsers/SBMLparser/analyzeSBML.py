# -*- coding: utf-8 -*-
"""
Created on Thu Mar 22 13:11:38 2012

@author: proto
"""

from pyparsing import Word, Suppress,Optional,alphanums,Group,ZeroOrMore
import numpy as np
import json
import itertools
import structures as st
from copy import deepcopy
import detectOntology
import re
import difflib
from util import logMess
from collections import defaultdict


'''
This file in general classifies rules according to the information contained in
the json config file for classyfying rules according to their reactants/products
'''
import functools
def memoize(obj):
    cache = obj.cache = {}

    @functools.wraps(obj)
    def memoizer(*args, **kwargs):
        key = str(args) + str(kwargs)
        if key not in cache:
            cache[key] = obj(*args, **kwargs)
        return cache[key]
    return memoizer


def addToDependencyGraph(dependencyGraph, label, value):
    if label not in dependencyGraph:
        dependencyGraph[label] = []
    if value not in dependencyGraph[label] and value != []:
        dependencyGraph[label].append(value)

class SBMLAnalyzer:
    
    def __init__(self,modelParser,configurationFile,namingConventions,speciesEquivalences=None):
        self.modelParser = modelParser        
        self.configurationFile = configurationFile
        self.namingConventions = detectOntology.loadOntology(namingConventions)
        self.speciesEquivalences= speciesEquivalences
        self.userEquivalencesDict = None
        self.lexicalSpecies= []
        
    def distanceToModification(self,particle,modifiedElement,translationKeys):
        posparticlePos = [m.start()+len(particle) for m in re.finditer(particle,modifiedElement)]
        preparticlePos = [m.start() for m in re.finditer(particle,modifiedElement)]
        keyPos = [m.start() for m in re.finditer(translationKeys,modifiedElement)]
        distance = [abs(y-x) for x in posparticlePos for y in keyPos]
        distance.extend([abs(y-x) for x in preparticlePos for y in keyPos])
        distance.append(9999)
        return min(distance)

    
    def fuzzyArtificialReaction(self,baseElements,modifiedElement,molecules):
        '''
        in case we don't know how a species is composed but we know its base
        elements, try to get it by concatenating its basic reactants
        '''
        import collections
        compare = lambda x, y: collections.Counter(x) == collections.Counter(y)
        equivalenceTranslator,translationKeys,conventionDict = self.processNamingConventions2(molecules)
        indirectEquivalenceTranslator= {x:[] for x in equivalenceTranslator}
        self.processFuzzyReaction([baseElements,modifiedElement],translationKeys,conventionDict,indirectEquivalenceTranslator)
        newBaseElements = baseElements
        for modification in indirectEquivalenceTranslator:
            for element in indirectEquivalenceTranslator[modification]:
                newBaseElements = [element[2][1] if x==element[2][0] else x for x in newBaseElements]
        if compare(baseElements,newBaseElements):
            return None
        return newBaseElements
        
    def analyzeSpeciesModification(self,baseElement, modifiedElement,partialAnalysis):
        '''
        a method for trying to read modifications within complexes
        This is only possible once we know their internal structure 
        (this method is called after the creation and resolving of the dependency
        graph)
        '''
        equivalenceTranslator,translationKeys,conventionDict =  self.processNamingConventions2([baseElement,modifiedElement])
        scores = []
        if len(translationKeys) == 0:
            '''
            there's no clear lexical path between reactant and product
            '''
            return None,None,None
        for particle in partialAnalysis:
            distance = 9999
            comparisonElement = max(baseElement,modifiedElement,key=len)
            if re.search('(_|^){0}(_|$)'.format(particle),comparisonElement) == None:
                distance = self.distanceToModification(particle,comparisonElement,translationKeys[0])
                score = difflib.ndiff(particle,modifiedElement)
            else:
                #FIXME: make sure we only do a search on those variables that are viable
                #candidates. this is once again fuzzy string matchign. there should
                #be a better way of doing this with difflib
                permutations = set(['_'.join(x) for x in itertools.permutations(partialAnalysis,2) if x[0] == particle])
                if all([x not in modifiedElement for x in permutations]):
                    distance = self.distanceToModification(particle,comparisonElement,translationKeys[0])
                    score = difflib.ndiff(particle,modifiedElement)
                #FIXME:tis is just an ad-hoc parameter in terms of how far a mod is from a species name
                #use something better
            if distance < 4:
                scores.append([particle,distance])
        if len(scores)>0:
            winner = scores[[x[1] for x in scores].index(min([x[1] for x in scores]))][0]   
        else:
            winner = None
        if winner:
            return winner,translationKeys,equivalenceTranslator
        return None,None,None

    def findClosestModification(self,particles,species):
        equivalenceTranslator = {}
        dependencyGraph = {}
        localSpeciesDict = defaultdict(lambda : defaultdict(list))

        def analyzeByParticle(splitparticle,species,
                              equivalenceTranslator=equivalenceTranslator,
                              dependencyGraph=dependencyGraph):
            basicElements = []
            composingElements = []
            splitpindex = -1
            #for splitpindex in range(0,len(splitparticle)):
            while (splitpindex + 1)< len(splitparticle):
                splitpindex += 1
                splitp = splitparticle[splitpindex]
                if splitp in species:
                    closestList = [splitp]
                    similarList = difflib.get_close_matches(splitp,species)
                    similarList = [x for x in similarList if x != splitp and len(x) < len(splitp)]
                    similarList = [[x,splitp] for x in similarList]
                    
                    if len(similarList) > 0:
                        for similarity in similarList:
                            tag,modifier = self.processAdHocNamingConventions(similarity[0],
                                            similarity[1],localSpeciesDict,False) 
                            if modifier != None and all(['-' not in x for x in modifier]):
                                logMess('INFO:Atomization','Lexical relationship inferred between \
                                {0}, user information confirming it is required'.format(similarity))
                    
                else:
                    closestList = difflib.get_close_matches(splitp,species)
                    closestList = [x for x in closestList if len(x) < len(splitp)]
                #if theres nothing in the species list i can find a lexical
                #neighbor from, then try to create one based on my two
                #positional neighbors
                if closestList == []:
                    flag= True
                    #do i get something by merging with the previous component?
                    if len(composingElements) > 0:
                        tmp,tmp2 = analyzeByParticle([composingElements[-1] + '_' + splitp], species)
                        if tmp != [] and tmp2 != []:
                            flag = False
                            splitp = composingElements[-1] + '_' + splitp
                            composingElements.pop()
                            closestList = tmp
                            localEquivalenceTranslator,_,_ =  self.processNamingConventions2([tmp[0],tmp2[0]])
                            for element in localEquivalenceTranslator:
                                if element not in equivalenceTranslator:
                                    equivalenceTranslator[element] = []
                                equivalenceTranslator[element].extend(localEquivalenceTranslator[element])
                                for instance in localEquivalenceTranslator[element]:
                                    addToDependencyGraph(dependencyGraph,instance[1],[instance[0]])
                    #do i get something by merging with the next component?
                    if flag and splitpindex + 1 != len(splitparticle):
                        tmp,tmp2 = analyzeByParticle([splitp+ '_' + splitparticle[splitpindex+1]],species)
                        if tmp!= [] and tmp2 != []:
                            splitp = splitp+ '_' + splitparticle[splitpindex+1]
                            splitpindex += 1
                            closestList = tmp
                            localEquivalenceTranslator,_,_ =  self.processNamingConventions2([tmp[0],tmp2[0]])
                            for element in localEquivalenceTranslator:
                                if element not in equivalenceTranslator:
                                    equivalenceTranslator[element] = []
                                equivalenceTranslator[element].append(localEquivalenceTranslator[element])
                                for instance in localEquivalenceTranslator[element]:
                                    addToDependencyGraph(dependencyGraph,instance[1],[instance[0]])

                        else:
                            return [],[]
                    elif flag:
                        return [],[]
                basicElements.append(min(closestList,key=len))
                #if what i have is a known compound just add it
                if splitp in species:
                    composingElements.append(splitp)
                #if not create it
                else:
                    closestList = difflib.get_close_matches(splitp,species)
                    closestList = [x for x in closestList if len(x) < len(splitp)]
                    flag = False
                    for element in closestList:
                        localEquivalenceTranslator,_,_ =  self.processNamingConventions2([element,splitp])
                        if len(localEquivalenceTranslator.keys()) == 0:
                            basicElements = []
                            composingElements = []
                        for element in localEquivalenceTranslator:
                            if element not in equivalenceTranslator:
                                equivalenceTranslator[element] = []
                            equivalenceTranslator[element].append(localEquivalenceTranslator[element])
                            for instance in localEquivalenceTranslator[element]:
                                addToDependencyGraph(dependencyGraph,instance[1],[instance[0]])
                            flag = True
                    if flag:
                        composingElements.append(splitp)
            return basicElements,composingElements
        additionalHandling = []
        for particle in particles:
            composingElements = []
            basicElements = []
            #break it down into small bites
            #TODO: take into account modifiers like _P
            splitparticle = particle.split('_')
            #print '---',splitparticle
            splitparticle = [x for x in splitparticle if x]
            #print splitparticle
            if particle == 'MisP_Ub8':
                pass
            basicElements,composingElements = analyzeByParticle(splitparticle,species)
            if particle not in composingElements and composingElements != []:
                addToDependencyGraph(dependencyGraph,particle,composingElements)
                for element in composingElements:
                    if element not in dependencyGraph:
                        addToDependencyGraph(dependencyGraph,element,[])
                    if element not in particles:
                        additionalHandling.append(element)
        #if len(additionalHandling) > 0:
            #print self.findClosestModification(set(additionalHandling),species)
        return dependencyGraph,equivalenceTranslator
    def parseReactions(self,reaction,specialSymbols=''):
        name = Word(alphanums + '_-') + ':'
        species =  (Word(alphanums+"_"+":#-") 
        + Suppress('()')) + ZeroOrMore(Suppress('+') + Word(alphanums+"_"+":#-") 
        + Suppress("()"))
        rate = Word(alphanums +     "()")
        grammar = Suppress(Optional(name)) + ((Group(species) | '0') + Suppress(Optional("<") + "->") + (Group(species) | '0') + Suppress(rate))  
        
        result =  grammar.parseString(reaction).asList()
        if len(result) < 2:    
            result = [result,[]]
        if '<->' in reaction and len(result[0]) == 1 and len(result[1]) == 2:
            result2 = [result[1],result[0]]
            result = result2
        return result
    
    
    
    def loadConfigFiles(self,fileName):
        '''
        the reactionDefinition file must contain the definitions of the basic reaction types 
        we wnat to parse and what are the requirements of a given reaction type to be considered
        as such
        '''
        reactionDefinition = ''
        if fileName == '':
            return []
        with open(fileName,'r') as fp:
            reactionDefinition = json.load(fp)
        return reactionDefinition
        

    def identifyReactions2(self,rule,reactionDefinition):
        '''
        This method goes through the list of common reactions listed in ruleDictionary
        and tries to find how are they related according to the information in reactionDefinition
        '''  
        result = []
        for idx,element in enumerate(reactionDefinition['reactions']):
            tmp1 = rule[0] if rule[0] not in  ['0',['0']] else []
            tmp2 = rule[1] if rule[1] not in  ['0',['0']] else []
            if(len(tmp1) == len(element[0]) and len(tmp2) == len(element[1])):
                result.append(1)           
    #            for (el1,el2) in (element[0],rule[0]):
    #                if element[0].count(el1) == element[]
            else:
                result.append(0)
        return result
    
      
        
    def species2Rules(self,rules):
        '''
        This method goes through the rule list and classifies species tuples in a dictionary
        according to the reactions they appear in.
        '''
        ruleDictionary = {}
        for idx,rule in enumerate(rules):
            reaction2 = rule #list(parseReactions(rule))
            totalElements =  [item for sublist in reaction2 for item in sublist]
            if tuple(totalElements) in ruleDictionary:
                ruleDictionary[tuple(totalElements)].append(idx)
            else:
                ruleDictionary[tuple(totalElements)] = [idx]
        return ruleDictionary
    
    def checkCompliance(self,ruleCompliance,tupleCompliance,ruleBook):
        '''
        This method is mainly useful when a single rule can be possibly classified
        in different ways, but in the context of its tuple partners it can only be classified
        as one
        '''
        
        ruleResult = np.zeros(len(ruleBook))
        for validTupleIndex in np.nonzero(tupleCompliance):
            for index in validTupleIndex:
                for alternative in ruleBook[index]:
                    if 'r' in alternative and np.any([ruleCompliance[temp] for temp in alternative['r']]):
                        ruleResult[index] = 1
                        break
                    #check if just this is enough
                    if 'n' in alternative:
                        ruleResult[index] = 1
                        break
        return ruleResult
        
    def levenshtein(self,s1, s2):
            l1 = len(s1)
            l2 = len(s2)
        
            matrix = [range(l1 + 1)] * (l2 + 1)
            for zz in range(l2 + 1):
              matrix[zz] = range(zz,zz + l1 + 1)
            for zz in range(0,l2):
              for sz in range(0,l1):
                if s1[sz] == s2[zz]:
                  matrix[zz+1][sz+1] = min(matrix[zz+1][sz] + 1, matrix[zz][sz+1] + 1, matrix[zz][sz])
                else:
                  matrix[zz+1][sz+1] = min(matrix[zz+1][sz] + 1, matrix[zz][sz+1] + 1, matrix[zz][sz] + 1)
            return matrix[l2][l1]
            
    def analyzeUserDefinedEquivalences(self,molecules,conventions):
        equivalences = {}
        smolecules = [x.strip('()') for x in molecules]
        modifiedElement = {}
        for convention in conventions:
            baseMol = []
            modMol = []
            for molecule in smolecules:
                if convention[0] in molecule and convention[1] not in molecule:
                    baseMol.append(molecule)
                elif convention[1] in molecule:
                    modMol.append(molecule)
            if convention[2] not in equivalences:
                equivalences[convention[2]] = []
            equivalences[convention[2]].append((convention[0],convention[1]))
            if convention[0] not in modifiedElement:            
                modifiedElement[convention[0]] = []
            modifiedElement[convention[0]].append((convention[0],convention[1]))
            '''
            for mol1 in baseMol:
                for mol2 in modMol:
                    score = self.levenshtein(mol1,mol2)
                    if score == self.levenshtein(convention[0],convention[1]):
                        equivalences[convention[2]].append((mol1,mol2))
                        modifiedElement[convention[0]].append((mol1,mol2))
                        break
            '''
        return equivalences,modifiedElement        
     
    
    def processNamingConventions2(self,molecules,threshold=4):
            
        #normal naming conventions
        strippedMolecules = [x.strip('()') for x in molecules]
        tmpTranslator,translationKeys,conventionDict =  detectOntology.analyzeNamingConventions(strippedMolecules,
                                                                                      self.namingConventions,similarityThreshold=threshold)
        
        #user defined naming convention
        if self.userEquivalencesDict == None and hasattr(self,'userEquivalences'):
            self.userEquivalencesDict,self.modifiedElementDictionary = self.analyzeUserDefinedEquivalences(molecules,self.userEquivalences)
        else: 
            if self.userEquivalencesDict ==None:            
                self.userEquivalencesDict = {}
        
        #for name in self.userEquivalencesDict:
        #    equivalenceTranslator[name] = self.userEquivalencesDict[name]

        #add stuff to the main translator
        for element in self.userEquivalencesDict:
            if element not in tmpTranslator:
                tmpTranslator[element] = []
            tmpTranslator[element].extend(self.userEquivalencesDict[element])
        return tmpTranslator,translationKeys,conventionDict
    
    
    def processAdHocNamingConventions(self,reactant,product,
                                      localSpeciesDict,compartmentChangeFlag):
        #strippedMolecules = [x.strip('()') for x in molecules]
        molecules = [reactant,product] if len(reactant) < len(product) else [product,reactant]
        similarityThreshold = 10
        namePairs,differenceList,_ = detectOntology.defineEditDistanceMatrix(molecules,similarityThreshold=similarityThreshold)
        #FIXME:in here we need a smarter heuristic to detect actual modifications
        #for now im just going with a simple heuristic that if the species name
        #is long enough, and the changes from a to be are all about modification
        longEnough = 3
        if len(reactant) >= longEnough and len(differenceList) > 0 and len(reactant) >= len(differenceList[0]):
            #one is strictly a subset of the other a,a_b
            if len([x for x in differenceList[0] if '-' in x]) == 0:
                return ''.join([x[-1] for x in differenceList[0]]),differenceList[0]
            #string share a common subset but they contain mutually exclusive appendixes: a_b,a_c
            else:
                commonRoot = detectOntology.findLongestSubstring(reactant,product)
                if len(commonRoot) > longEnough:
                    molecules = [commonRoot,reactant,product]
                    namePairs,differenceList,_ = detectOntology.defineEditDistanceMatrix(molecules,similarityThreshold=10)  
                    #obtain the name of the component from an anagram using the modification letters
                    validDifferences = [''.join([x[-1] 
                        for x in difference]) 
                        for difference in differenceList if '-' not in [y[0] 
                        for y in difference]]
                    validDifferences.sort()
                    #avoid trivial differences
                    if len(validDifferences) < 2:
                        return None,None

                    #FIXME:here it'd be helpful to come up with a better heuristic
                    #for infered component names
                    componentName =  ''.join([x[0:max(1,len(x)/2)] for x in validDifferences])
                    
                    for namePair,difference in zip(namePairs,differenceList):
                        if len([x for x in difference if '-' in x]) == 0:
                            tag = ''.join([x[-1] for x in difference])
                            if [namePair[0],tag] not in localSpeciesDict[commonRoot][componentName]:
                                localSpeciesDict[namePair[0]][componentName].append([namePair[0],tag,compartmentChangeFlag])
                                localSpeciesDict[namePair[1]][componentName].append([namePair[0],tag,compartmentChangeFlag])
                                
        return None,None
    
    import itertools
    def approximateMatching(self,ruleList,differenceParameter=[]):
        def curateString(element,differences,symbolList = ['#','&',';','@','!','?'],equivalenceDict={}):
            '''
            remove compound differencese (>2 characters) and instead represent them with symbols
            returns transformed string,an equivalence dictionary and unused symbols
            '''
            tmp = element
            for difference in differences:
                if difference in element:
                    if difference.startswith('_'):
                        if difference not in equivalenceDict:
                            symbol = symbolList.pop()
                            equivalenceDict[difference] = symbol
                        else:
                            symbol = equivalenceDict[difference]
                        tmp = re.sub(r'{0}(_|$)'.format(difference),r'{0}\1'.format(symbol),tmp)
                    elif difference.endswith('_'):
                        if difference not in equivalenceDict:
                            symbol = symbolList.pop()
                            equivalenceDict[difference] = symbol
                        else:
                            symbol = equivalenceDict[difference]
    
                        tmp = re.sub(r'(_|^){0}'.format(difference),r'{0}\1'.format(symbol),tmp)
            return tmp,symbolList,equivalenceDict
        '''
        given a transformation of the kind a+ b -> ~a_~b, where ~a and ~b are some
        slightly modified version of a and b, this function will return a list of 
        lexical changes that a and b must undergo to become ~a and ~b.
        '''
        differences = deepcopy(differenceParameter)
        tmpRuleList = deepcopy(ruleList)
        flag = True
        if len(ruleList[1]) == 1 and ruleList[1] != '0':
            while flag:
                flag = False
                sym = ['#','&',';','@','!','?']
                dic = {}
                for idx,_ in enumerate(tmpRuleList[0]):
                    tmpRuleList[0][idx],sym,dic = curateString(ruleList[0][idx],differences,sym,dic)
                    
                tmpRuleList[1][0],sym,dic =  curateString(ruleList[1][0],differences,sym,dic)
                permutations = [x for x in itertools.permutations(ruleList[0])]
                tpermutations = [x for x in itertools.permutations(tmpRuleList[0])]
                score = [difflib.SequenceMatcher(None,'_'.join(x),ruleList[1][0]).ratio() \
                    for x in permutations]
                maxindex = score.index(max(score))
                ruleList[0] = list(permutations[maxindex])
                tmpRuleList[0] = list(tpermutations[maxindex])

                sym = [dic[x] for x in dic]
                sym.extend(differences)
                sym = [x for x in sym if '_' not in x]
                simplifiedDifference = difflib.SequenceMatcher(lambda x: x in sym,'-'.join(tmpRuleList[0]),tmpRuleList[1][0])
                            
                matches =  simplifiedDifference.get_matching_blocks()
                if len(matches) != len(ruleList[0]) + 1:
                    return [[],[]],[[],[]]
                
                if len(matches) != 3:
                    pass
                productPartitions = []
                for idx,match in enumerate(matches):
                    if matches[idx][2] != 0:
                        productPartitions.append(tmpRuleList[1][0][
                                matches[idx][1]:matches[idx][1]+matches[idx][2]])
                reactantPartitions = tmpRuleList[0]
                
                
                
                #Don't count trailing underscores as part of the species name
                for idx,_ in enumerate(reactantPartitions):
                    reactantPartitions[idx] = reactantPartitions[idx].strip('_')                    
                for idx,_ in enumerate(productPartitions):
                    productPartitions[idx] = productPartitions[idx].strip('_')                    

                #greedymatching
                
                acc=0
                #FIXME:its not properly copying all the string
                for idx in range(0,len(matches)-1):
                    while matches[idx][2]+ acc < len(tmpRuleList[1][0]) \
                    and tmpRuleList[1][0][matches[idx][2]+ acc]  in sym:
                        productPartitions[idx] += tmpRuleList[1][0][matches[idx][2] + acc]
                        acc += 1
                    
                #idx = 0
                #while(tmpString[matches[0][2]+ idx]  in sym):
                #    reactantfirstHalf += tmpString[matches[0][2] + idx]
                #    idx += 1
                
                
                for element in dic:
                    for idx in range(len(productPartitions)):
                        productPartitions[idx] = productPartitions[idx].replace(dic[element],element)
                        reactantPartitions[idx] = reactantPartitions[idx].replace(dic[element],element)
                        
                zippedPartitions = zip(reactantPartitions,productPartitions)
                zippedPartitions = [sorted(x,key=len) for x in zippedPartitions]
                bdifferences = [[z for z in y if '+ ' in z or '- ' in z] for y in \
                             [difflib.ndiff(*x) for x in zippedPartitions]]
            
                
                
                processedDifferences = [''.join([y.strip('+ ') for y in x]) for x in bdifferences]
                
                for idx,processedDifference in enumerate(processedDifferences):
                    if processedDifference not in differences and \
                    '- ' not in processedDifference and bdifferences[idx] != []:
                        flag = True
                        differences.append(processedDifference)
                        
        else:
            #TODO: dea with reactions of the kindd a+b ->  c + d
            return [[],[]],[[],[]]
        return bdifferences,zippedPartitions
             
    def getReactionClassification(self,reactionDefinition,rules,equivalenceTranslator,
                                  indirectEquivalenceTranslator,
                                  translationKeys=[]):
        '''
        *reactionDefinition* is a list of conditions that must be met for a reaction
        to be classified a certain way
        *rules* is the list of reactions
        *equivalenceTranslator* is a dictinary containing all complexes that have been
        determined to be the same through naming conventions
        This method will go through the list of rules and the list of rule definitions
        and tell us which rules it can classify according to the rule definitions list
        provided
        '''
        ruleDictionary = self.species2Rules(rules)
        #determines a reaction's reactionStructure aka stoichoimetry
        ruleComplianceMatrix = np.zeros((len(rules),len(reactionDefinition['reactions'])))
        for (idx,rule) in enumerate(rules):
            reaction2 = rule #list(parseReactions(rule))
            ruleComplianceMatrix[idx] = self.identifyReactions2(reaction2,reactionDefinition)
        #initialize the tupleComplianceMatrix array with the same keys as ruleDictionary
        #the tuple complianceMatrix is basically there to make sure we evaluate
        #bidirectional reactions as one reaction        
        tupleComplianceMatrix = {key:np.zeros((len(reactionDefinition['reactions']))) for key in ruleDictionary}
        #check which reaction conditions each tuple satisfies
        for element in ruleDictionary:
            for rule in ruleDictionary[element]:
                tupleComplianceMatrix[element] += ruleComplianceMatrix[rule]     

        #now we will check for the nameConventionMatrix (same thing as before but for naming conventions)
        tupleNameComplianceMatrix = {key:{key2:0 for key2 in equivalenceTranslator} \
                                        for key in ruleDictionary}
        for rule in ruleDictionary:
            for namingConvention in equivalenceTranslator:
                for equivalence in equivalenceTranslator[namingConvention]:
                    if all(element in rule for element in equivalence):
                        tupleNameComplianceMatrix[rule][namingConvention] +=1
                        break
                for equivalence in indirectEquivalenceTranslator[namingConvention]:
                    if all(element in rule for element in equivalence[0]):
                        tupleNameComplianceMatrix[rule][namingConvention] +=1
                        break
                        #we can have more than one
                    #elif appro
        #check if the reaction conditions each tuple satisfies are enough to get classified
        #as an specific named reaction type
        tupleDefinitionMatrix = {key:np.zeros((len(reactionDefinition['definitions']))) for key in ruleDictionary}
        for key,element in tupleComplianceMatrix.items():
            for idx,member in enumerate(reactionDefinition['definitions']):
                for alternative in member:
                    if 'r' in alternative:            
                        tupleDefinitionMatrix[key][idx] += np.all([element[reaction] for reaction in alternative[u'r']])
                    if 'n' in alternative and reactionDefinition['reactionsNames'][idx] in equivalenceTranslator:
                        tupleDefinitionMatrix[key][idx] += np.all([tupleNameComplianceMatrix[key][reactionDefinition['reactionsNames'][idx]]])
        #cotains which rules are equal to reactions defined in reactionDefinitions['definitions']
        #use the per tuple classification to obtain a per reaction classification
        ruleDefinitionMatrix = np.zeros((len(rules),len(reactionDefinition['definitions'])))

        for key,element in ruleDictionary.items():
            for rule in element:
                ruleDefinitionMatrix[rule] = self.checkCompliance(ruleComplianceMatrix[rule],
    tupleDefinitionMatrix[key],reactionDefinition['definitions'])
        #use reactionDefinitions reactionNames field to actually tell us what reaction
        #type each reaction is
        results = []    
        for idx,element in enumerate(ruleDefinitionMatrix):
            nonZero = np.nonzero(element)[0]
            if(len(nonZero) == 0):
                results.append('None')
            #todo: need to do something if it matches more than one reaction
            else:
                classifications = [reactionDefinition['reactionsNames'][x] for x in nonZero]
                #FIXME: we should be able to support more than one transformation
                results.append(classifications[0])
        return  results
    
    def setConfigurationFile(self,configurationFile):
        self.configurationFile = configurationFile
        
    def getReactionProperties(self):
        '''
        if we are using a naming convention definition in the json file
        this method will return the component and state names that this 
        reaction uses
        '''
        
        #TODO: once we transition completely to a naming convention delete 
        #this ----
        reactionTypeProperties = {}
        reactionDefinition = self.loadConfigFiles(self.configurationFile)
        if self.speciesEquivalences != None:
            self.userEquivalences = self.loadConfigFiles(self.speciesEquivalences)['reactionDefinition']
        for reactionType,properties in zip(reactionDefinition['reactionsNames'],reactionDefinition['definitions']):
            #if its a reaction defined by its naming convention   
            #xxxxxxxxxxxxxxxxxxx
            for alternative in properties:
                if 'n' in alternative.keys():
                    try:
                        site = reactionDefinition['reactionSite'][alternative['rsi']]
                        state = reactionDefinition['reactionState'][alternative['rst']]
                    except:
                        site = reactionType
                        state = reactionType[0]
                    reactionTypeProperties[reactionType] = [site,state]
        #TODO: end of delete
        reactionDefinition = self.namingConventions
        for idx,reactionType in enumerate(reactionDefinition['modificationList']):
            site = reactionDefinition['reactionSite'][reactionDefinition['definitions'][idx]['rsi']]
            state = reactionDefinition['reactionState'][reactionDefinition['definitions'][idx]['rst']]
            reactionTypeProperties[reactionType] = [site,state]
        return reactionTypeProperties


        
    def processFuzzyReaction(self,reaction,translationKeys,conventionDict,indirectEquivalenceTranslator):
        
        differences,pairedChemicals= self.approximateMatching(reaction,
                                                    translationKeys)
        d1,d2 = differences[0],differences[1]
        firstMatch,secondMatch = pairedChemicals[0],pairedChemicals[1]
        matches = [firstMatch,secondMatch]
        for index,element in enumerate([d1,d2]):
            idx1=0
            idx2 = 1
            while idx2 <= len(element):
                if (element[idx1],) in conventionDict.keys():
                    pattern = conventionDict[(element[idx1],)]
                    indirectEquivalenceTranslator[pattern].append([[reaction[0][index],reaction[1][0]],reaction[0],matches[index],reaction[1]])
                elif (element[idx1].replace('-','+'),) in conventionDict.keys():
                    matches[index].reverse()
                    transformedPattern = conventionDict[(element[idx1].replace('-','+'),) ]
                    indirectEquivalenceTranslator[transformedPattern].append([[reaction[1][0],reaction[0][index]],reaction[0],matches[index],reaction[1]])
                elif idx2 < len(element):
                    if tuple([element[idx1],element[idx2]]) in conventionDict.keys():    
                        pattern = conventionDict[tuple([element[idx1],element[idx2]])]
                        indirectEquivalenceTranslator[pattern].append([[reaction[0][index],reaction[1][0]],reaction[0],matches[index],reaction[1]])
                        idx1 += 1
                        idx2 += 1
                    elif '-' in element[idx1] and '-' in element[idx2]:
                        if tuple([element[idx1].replace('-','+'),element[idx2].replace('-','+')]) in conventionDict.keys():  
                            matches[index].reverse()
                            transformedPattern = conventionDict[tuple([element[idx1].replace('-','+'),element[idx2].replace('-','+')])]
                            indirectEquivalenceTranslator[transformedPattern].append([[reaction[1][0],reaction[0][index]],reaction[0],matches[index],reaction[1]])
                            idx1 += 1
                            idx2 += 1

                idx1+=1
                idx2+=1
        

        
    def classifyReactions(self,reactions,molecules):
        '''
        classifies a group of reaction according to the information in the json
        config file
        '''
        def createArtificialNamingConvention(reaction,fuzzyKey,fuzzyDifference):
            '''
            Does the actual data-structure filling if
            a 1-1 reaction shows sign of modification. Returns True if 
            a change was performed
            '''
            #fuzzyKey,fuzzyDifference = self.processAdHocNamingConventions(reaction[0][0],reaction[1][0],localSpeciesDict,compartmentChangeFlag)
            
            if fuzzyKey and fuzzyKey.strip('_') not in strippedMolecules:
                logMess('INFO:Atomization','added induced naming convention {0}'.format(str(reaction)))
                #if our state isnt yet on the dependency graph preliminary data structures
                if '{0}mod'.format(fuzzyKey) not in equivalenceTranslator:
                    equivalenceTranslator['{0}mod'.format(fuzzyKey)] = []
                    adhocLabelDictionary['{0}mod'.format(fuzzyKey)] = ['{0}mod'.format(fuzzyKey),fuzzyKey.upper()]
                    #fill main naming convention data structure                    
                    self.namingConventions['modificationList'].append('{0}mod'.format(fuzzyKey))
                    self.namingConventions['reactionState'].append(fuzzyKey.upper())
                    self.namingConventions['reactionSite'].append('{0}mod'.format(fuzzyKey))
                    self.namingConventions['patterns'][fuzzyDifference] = '{0}mod'.format(fuzzyKey)
                    self.namingConventions['definitions'].append({'rst':len(self.namingConventions['reactionState'])-1,
                            'rsi':len(self.namingConventions['reactionSite'])-1})
                    if fuzzyKey not in translationKeys:
                        translationKeys.append(fuzzyKey)
                #if this same definition doesnt already exist. this is to avoid cycles
                if tuple(sorted([x[0] for x in reaction],key=len)) not in equivalenceTranslator['{0}mod'.format(fuzzyKey)]:
                    equivalenceTranslator['{0}mod'.format(fuzzyKey)].append(tuple(sorted([x[0] for x in reaction],key=len)))
                    newTranslationKeys.append(fuzzyKey)
                conventionDict[fuzzyDifference] = '{0}mod'.format(fuzzyKey)
                if '{0}mod'.format(fuzzyKey) not in indirectEquivalenceTranslator:
                    indirectEquivalenceTranslator['{0}mod'.format(fuzzyKey)] = []
                return True
            return False
        
        #load the json config file
        reactionDefinition = self.loadConfigFiles(self.configurationFile)
        strippedMolecules = [x.strip('()') for x in molecules]
        #load user defined complexes        
        if self.speciesEquivalences != None:
            self.userEquivalences = self.loadConfigFiles(self.speciesEquivalences)['reactionDefinition']
        #determines if two molecules have a relationship according to the naming convention section
        #equivalenceTranslator is a dictionary of actual modifications
        #example {'Phosporylation':[['A','A_p'],['B','B_p']]}
        
        #process straightforward naming conventions
        equivalenceTranslator,translationKeys,conventionDict = self.processNamingConventions2(molecules)
        newTranslationKeys = []
        adhocLabelDictionary = {}
        #lists of plain reactions
        rawReactions = [self.parseReactions(x) for x in reactions]
        #process fuzzy naming conventions based on reaction information
        indirectEquivalenceTranslator= {x:[] for x in equivalenceTranslator}
        localSpeciesDict = defaultdict(lambda : defaultdict(list))

        trueBindingReactions = []
        for idx,reaction in enumerate(rawReactions):
            if len(reaction[0]) == 2:
                #complex naming conventions, first we need to know how to match them
                differences,pairedChemicals = self.approximateMatching(reaction,
                                                    translationKeys)
                
                d1,d2 = differences[0],differences[1]
                firstMatch,secondMatch = pairedChemicals[0],pairedChemicals[1]
                #then how do they differ
                
                for pair in pairedChemicals:
                    if len(pair) == 2:
                        fuzzyKey,fuzzyDifference = self.processAdHocNamingConventions(pair[0],
                                        pair[1],localSpeciesDict,False) 
                        #fill in 1-1 naming structures
                        createArtificialNamingConvention([[pair[0]],[pair[1]]],
                                                         fuzzyKey,fuzzyDifference)

                #basic processing vs known naming conventions
                self.processFuzzyReaction(reaction,translationKeys,
                               conventionDict,indirectEquivalenceTranslator)

            elif len(reaction[1]) == 2 and len(reaction[0]) == 1:
                differences,pairedChemicals= self.approximateMatching([reaction[1],reaction[0]],
                                                    translationKeys)
                d1,d2 = differences[0],differences[1]
          
                for pair in pairedChemicals:
                    if len(pair) == 2:
                        fuzzyKey,fuzzyDifference = self.processAdHocNamingConventions(pair[0],
                                        pair[1],localSpeciesDict,False) 
                        #fill in 1-1 naming structures
                        createArtificialNamingConvention([[pair[0]],[pair[1]]],
                                                         fuzzyKey,fuzzyDifference)
 
    
                #self.processFuzzyReaction([reaction[1],reaction[0]],translationKeys,conventionDict,indirectEquivalenceTranslator)

            elif len(reaction[0]) == 1 and len(reaction[1]) == 1 and '0' not in reaction:
                #check if this is a change compartment reaction
                sbmlreactants =  self.modelParser.model.getReaction(long(idx)).getListOfReactants()
                sbmlproducts = self.modelParser.model.getReaction(long(idx)).getListOfProducts()
                rcomp = pcomp = ''
                counter = 0
                for sbmlr,sbmlp in zip(sbmlreactants,sbmlproducts): 
                    rid,pid =sbmlr.getSpecies(),sbmlp.getSpecies()
                    rspec = self.modelParser.model.getSpecies(rid)
                    pspec = self.modelParser.model.getSpecies(pid)
                    rcomp = rspec.getCompartment() if not rspec.getBoundaryCondition() else None
                    pcomp = pspec.getCompartment() if not pspec.getBoundaryCondition() else None
                    
                    counter+=1
                assert(counter==1)
                compartmentChangeFlag = False
                if rcomp != pcomp and rcomp and pcomp:
                    root = detectOntology.findLongestSubstring(reaction[0][0],reaction[1][0])
                    #self.lexicalSpecies.append([reaction[0][0],[[root]]])     
                    #self.lexicalSpecies.append([reaction[1][0],[[root]]])
                    compartmentChangeFlag = True
                    
                #check if reaction->product shares the same reactant root
                fuzzyKey,fuzzyDifference = self.processAdHocNamingConventions(reaction[0][0],reaction[1][0],localSpeciesDict,compartmentChangeFlag)
                createArtificialNamingConvention(reaction,fuzzyKey,fuzzyDifference)
            #    self.processFuzzyReaction([[reaction[0][0],''],reaction[1]],translationKeys,conventionDict,indirectEquivalenceTranslator)
            elif (len(reaction[1]) == 1 or len(reaction[0]) == 1) and '0' not in reaction:
                if len(reaction[1]) == 1 and len(reaction[0]) <= 4:
                    difference,pairedChemicals =  self.approximateMatching(reaction,translationKeys)
                elif len(reaction[0]) == 1 and len(reaction[1]) <= 4:
                    difference,pairedChemicals =  self.approximateMatching([reaction[1],reaction[0]],translationKeys)
                else:
                    continue
                #its a simple binding reaction
                if difference == [y for y in itertools.repeat([],len(reaction[0]))] \
                and pairedChemicals != [y for y in itertools.repeat([],len(reaction[0]))]:
                    trueBindingReactions.append(idx)
                else:
                    #n-order modification reaction
                    #print '--',reaction
                    pass
                    '''
                    for pair in pairedChemicals:
                        if len(pair) == 2:
                            fuzzyKey,fuzzyDifference = self.processAdHocNamingConventions(pair[0],
                                            pair[1],localSpeciesDict,False) 
                            #fill in 1-1 naming structures
                            createArtificialNamingConvention([[pair[0]],[pair[1]]],
                                                             fuzzyKey,fuzzyDifference)
                    '''
            #else:
                #print '+++',reaction
        translationKeys.extend(newTranslationKeys)
        for species in localSpeciesDict:
            speciesName =  localSpeciesDict[species][localSpeciesDict[species].keys()[0]][0][0]
            definition = [species]
            sdefinition = [speciesName]
            for component in localSpeciesDict[species]:
                cdefinition = []
                states = [["s",state[1]] for state in localSpeciesDict[species][component]]
                for state in states:
                    cdefinition.extend(state)
                cdefinition = [component,cdefinition]
                sdefinition.extend(cdefinition)
            definition.append([sdefinition])
            self.lexicalSpecies.append(definition)
                #definition = [commonRoot,[[commonRoot,componentName,["s",tag]]]]
        
        reactionClassification = self.getReactionClassification(reactionDefinition,
                                            rawReactions,equivalenceTranslator,
                                            indirectEquivalenceTranslator,
                                            translationKeys)
        for element in trueBindingReactions:
            reactionClassification[element] = 'Binding'
        listOfEquivalences = []
        
        for element in equivalenceTranslator:
            listOfEquivalences.extend(equivalenceTranslator[element])
        return reactionClassification,listOfEquivalences,equivalenceTranslator, \
                indirectEquivalenceTranslator,adhocLabelDictionary
        
    
 
    
    def processAnnotations(self,molecules,annotations):
        processedAnnotations = []
        for element in annotations:
            if len(annotations[element]) > 1:
                pro = [list(x) for x in itertools.combinations([y for y in annotations[element]],2)]
                processedAnnotations.extend(pro)
            
        return {-1:processedAnnotations}
    

    
    def classifyReactionsWithAnnotations(self,reactions,molecules,annotations,labelDictionary):        
        '''
        this model will go through the list of reactions and assign a 'modification' tag to those reactions where
        some kind of modification goes on aided through annotation information
        '''
        rawReactions = [self.parseReactions(x) for x in reactions]
        equivalenceTranslator = self.processAnnotations(molecules,annotations)     
        for reactionIndex in range(0,len(rawReactions)):
            for reactantIndex in range(0,len(rawReactions[reactionIndex])):
                tmp = []
                for chemicalIndex in range(0,len(rawReactions[reactionIndex][reactantIndex])):
                    tmp.extend(list(labelDictionary[rawReactions[reactionIndex][reactantIndex][chemicalIndex]]))
                rawReactions[reactionIndex][reactantIndex] = tmp
        #self.annotationClassificationHelper(rawReactions,equivalenceTranslator[-1])         
    
    def userJsonToDataStructure(self,userEquivalence,dictionary,
                                labelDictionary,equivalencesList):
        '''
        converts a user defined species to an internal representation
        '''
        tmp = st.Species()
        label = []
        for molecule in userEquivalence[1]:
            tmp2 = st.Molecule(molecule[0])
            for componentIdx in range(1,len(molecule),2):
                tmp3 = st.Component(molecule[componentIdx])
                for bindStateIdx in range(0,len(molecule[componentIdx+1]),2):
                    if molecule[componentIdx+1][bindStateIdx] == "b":
                        tmp3.addBond(molecule[componentIdx+1][bindStateIdx+1])
                    elif molecule[componentIdx+1][bindStateIdx] == "s":
                        tmp3.addState('U')
                        tmp3.addState(molecule[componentIdx+1][bindStateIdx+1])
                        equivalencesList.append([userEquivalence[0],molecule[0]])
                
                #tmp3.addState(molecule[2][2])
            
                tmp2.addComponent(tmp3)
            stmp = st.Species()
            stmp.addMolecule(deepcopy(tmp2))
            stmp.reset()
            #in case one definition overlaps another
            if molecule[0] in dictionary:
                dictionary[molecule[0]].extend(deepcopy(stmp))
            else:
                dictionary[molecule[0]] = deepcopy(stmp)
            labelDictionary[molecule[0]] = [(molecule[0],)]
            label.append(molecule[0])
            
            #for component in tmp2.components:
            #    if component.name == molecule[1]:
            #        component.setActiveState(molecule[2][1])
            tmp.addMolecule(tmp2)
        if userEquivalence[0] in dictionary:
            dictionary[userEquivalence[0]].extend(deepcopy(tmp))
        else:
            dictionary[userEquivalence[0]] = deepcopy(tmp)
        labelDictionary[userEquivalence[0]] = [tuple(label)]
    def getUserDefinedComplexes(self):
        dictionary = {}
        userLabelDictionary = {}
        equivalencesList = []
        lexicalLabelDictionary = {}
        if self.speciesEquivalences != None:
            speciesdictionary =self.loadConfigFiles(self.speciesEquivalences)
            userEquivalences = speciesdictionary['complexDefinition'] \
                if 'complexDefinition' in speciesdictionary else None
            for element in userEquivalences:
                self.userJsonToDataStructure(element,dictionary,
                                             userLabelDictionary,equivalencesList)
                                             
            complexEquivalences = speciesdictionary['modificationDefinition']
            for element in complexEquivalences:
                userLabelDictionary[element] = [tuple(complexEquivalences[element])]
                
        #stuff we got from string similarity
        for element in self.lexicalSpecies:
            self.userJsonToDataStructure(element,dictionary,lexicalLabelDictionary,
                                         equivalencesList)
                                         
        return dictionary,userLabelDictionary,lexicalLabelDictionary
        
