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

'''
This file in general classifies rules according to the information contained in
the json config file for classyfying rules according to their reactants/products
'''

class SBMLAnalyzer:
    
    def __init__(self,configurationFile,namingConventions,speciesEquivalences=None):
        self.configurationFile = configurationFile
        self.namingConventions = namingConventions
        self.speciesEquivalences= speciesEquivalences
        self.userEquivalencesDict = None
        
        
    def parseReactions(self,reaction,specialSymbols=''):
        species =  (Word(alphanums+"_"+":#-") 
        + Suppress('()')) + ZeroOrMore(Suppress('+') + Word(alphanums+"_"+":#-") 
        + Suppress("()"))
        rate = Word(alphanums + "()")
        grammar = ((Group(species) | '0') + Suppress(Optional("<") + "->") + (Group(species) | '0') + Suppress(rate))  
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
        #print reactionDefinition
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
            #print reaction2
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
     
    def analyzeNamingConventions(self,molecules,originalPattern='',modifiedPattern='',totalPatterns=''):
        '''
        *originalPattern* and *modifiedPattern* are regular expressions containing
        the patterns we wish to compare and see if they are the same.
        We will go through the list of molecules and check for names that match those
        patterns
        '''
        #original = originalPattern[0].replace('\\\\', '\\')
        #modified = modifiedPattern[0].replace('\\\\', '\\')
        #pOriginal = re.compile(original)
        #pModified = re.compile(modified)
        #oMolecules = []
        patternType = originalPattern
        pattern = modifiedPattern
        oMolecules = []
        results = []
        if patternType == 'prefix':
            comparisonMethod = str.startswith
        elif patternType == 'suffix':
            comparisonMethod = str.endswith
        elif patternType == 'infix':
            comparisonMethod = str.count
            
        #comparisonMethod = str.startswith if patternType == 'prefix' else str.endswith
        for molecule in [x.strip('()') for x in molecules]:
            if comparisonMethod(molecule,pattern) or comparisonMethod(molecule,pattern.lower()) or comparisonMethod(molecule,pattern.upper()):
                oMolecules.append(molecule)
        
        
        
        if patternType == 'infix':
            for superMolecule in oMolecules:
                exceptions = [x[1] for x in totalPatterns if pattern in x[1] and x[1] != pattern]
                #FIXME: We do not need to totally reject it, just to properly handle it
                if not True in [x in superMolecule for x in exceptions]:
                    moleculeArray = tuple(superMolecule.split(pattern))
                    results.append([superMolecule,moleculeArray])
            return results
                
        for molecule in [x.strip('()') for x in molecules]:
            if molecule in oMolecules:
                continue
            for superMolecule in oMolecules:
                if patternType == 'prefix':
                    comparison = superMolecule[len(pattern):]
                elif patternType == 'suffix':
                    comparison = superMolecule[:len(superMolecule)- len(pattern)]

                #elif patternType == 'infix':
                #    comparison = superMolecule.replace(pattern,'',1)
                    #if 'EGF_EGFR2' in comparison:
                    #    print comparison,molecule
                if comparison == molecule:
                    results.append((molecule,superMolecule))
                
#        for molecule in molecules:
#            mmatch = pModified.match(molecule)        
#            if mmatch and mmatch.group('key') in oMolecules:
#                results.append((mmatch.group('key'),molecule[0:-2]))
    
        return results
     
     
    
    def processNamingConventions2(self,molecules):
            
        
        #normal naming conventions
        tmpTranslator,translationKeys,conventionDict =  detectOntology.analyzeNamingConventions([x.strip('()') for x in molecules],
                                                                                      self.namingConventions)
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
            tmpTranslator[element].extend(self.userEquivalencesDict[element])
        
        return tmpTranslator,translationKeys,conventionDict
        
    
    def processNamingConventions(self,molecules,reactionDefinition):
        equivalenceTranslator = {}
        index = 0
        
        if self.userEquivalencesDict == None and hasattr(self,'userEquivalences'):
            self.userEquivalencesDict,self.modifiedElementDictionary = self.analyzeUserDefinedEquivalences(molecules,self.userEquivalences)
        else: 
            if self.userEquivalencesDict ==None:            
                self.userEquivalencesDict = {}
        #TODO: user defined naming conventions are not being added
        
        tmpTranslator,translationKeys,conventionDict =  detectOntology.analyzeNamingConventions([x.strip('()') for x in molecules],self.namingConventions)
        
        print translationKeys
        print conventionDict
        #for name in self.userEquivalencesDict:
        #    equivalenceTranslator[name] = self.userEquivalencesDict[name]
        #print '---',equivalenceTranslator            
        
        for name,prop in zip(reactionDefinition['reactionsNames'],reactionDefinition['definitions']):
            #xxxxxxxxxxxxxxxxxxxxxxx
            
            for alternative in prop:
                if 'n' in alternative.keys():
                    convention = reactionDefinition['namingConvention'][alternative['n'][0]]
                    temp = self.analyzeNamingConventions(molecules,convention[0],convention[1],reactionDefinition['namingConvention'])
                    if name in self.userEquivalencesDict:
                        temp.extend(self.userEquivalencesDict[name])
                    equivalenceTranslator[name] = temp
                    index += 1
        #now we want to fill in all intermediate relationships
        newTranslator = equivalenceTranslator.copy()
        #FIXME: the only thing we want to copy over from the old 
        #naming convention detection is the binding information
        for element in newTranslator:
            if element not in tmpTranslator and len(newTranslator[element]) > 0:
                tmpTranslator[element] = newTranslator[element]
        for element in tmpTranslator:
            if element in self.userEquivalencesDict:
                tmpTranslator[element].extend(self.userEquivalencesDict[element])
                
        return tmpTranslator,translationKeys,conventionDict
    
    def approximateMatching(self,ruleList,differences=[]):
        '''
        remove compound differencese (>2 characters) and instead represent them with symbols
        returns transformed string,an equivalence dictionary and unused symbols
        
        '''
        def curateString(element,differences,symbolList = ['#','&',';','@','!','?'],equivalenceDict={}):
            tmp = element
            for difference in differences:
                if difference in element:
                    if difference not in equivalenceDict:
                        symbol = symbolList.pop()
                        equivalenceDict[difference] = symbol
                    else:
                        symbol = equivalenceDict[difference]
                    if difference.startswith('_'):
                        tmp = re.sub(r'{0}(_|$)'.format(difference),r'{0}\1'.format(symbol),tmp)
                    elif difference.endswith('_'):
                        tmp = re.sub(r'(_|^){0}'.format(difference),r'{0}\1'.format(symbol),tmp)
            return tmp,symbolList,equivalenceDict
        '''
        given a transformation of the kind a+ b -> ~a_~b, where ~a and ~b are some
        slightly modified version of a and b, this function will return a list of 
        lexical changes that a and b must undergo to become ~a and ~b.
        '''
        tmpRuleList = deepcopy(ruleList)
        if len(ruleList[1]) == 1 and ruleList[1] != '0':
            tmpRuleList[0][0],sym,dic =  curateString(ruleList[0][0],differences)
            tmpRuleList[0][1],sym,dic = curateString(ruleList[0][1],differences,sym,dic)
            tmpRuleList[1][0],sym,dic =  curateString(ruleList[1][0],differences,sym,dic)
            
            alt1 = ruleList[0][0] + ruleList[0][1]
            alt2 = ruleList[0][1] + ruleList[0][0]
            r1 = difflib.SequenceMatcher(None,alt1,ruleList[1][0]).ratio()
            r2 =  difflib.SequenceMatcher(None,alt2,ruleList[1][0]).ratio()
            #alt = alt1
            if r2>r1:
                ruleList[0].reverse()
                tmpRuleList[0].reverse()
                #alt = alt2
            #salt = ruleList[0][0] + '-' + ruleList[0][1]
            sym = [dic[x] for x in dic]
            sym.extend(differences)
            sym = [x for x in sym if '_' not in x]
            simplifiedDifference = difflib.SequenceMatcher(lambda x: x in sym,tmpRuleList[0][0] + '-' + tmpRuleList[0][1],tmpRuleList[1][0])
                        
            matches =  simplifiedDifference.get_matching_blocks()
            if len(matches) != 3:
                return [],[],[],[]
            productfirstHalf = tmpRuleList[1][0][matches[0][1]:matches[0][2]]
            productsecondHalf = tmpRuleList[1][0][matches[1][1]:]
            tmpString = tmpRuleList[0][0] + '-' + tmpRuleList[0][1]
            reactantfirstHalf = tmpString[matches[0][0]:matches[0][2]]
            reactantsecondHalf = tmpString[matches[1][0]:]
            #greedymatching
            idx = 0
            while(tmpRuleList[1][0][matches[0][2]+ idx]  in sym):
                productfirstHalf += tmpRuleList[1][0][matches[0][2] + idx]
                idx += 1
            idx = 0
            while(tmpString[matches[0][2]+ idx]  in sym):
                reactantfirstHalf += tmpString[matches[0][2] + idx]
                idx += 1
            
            
            for element in dic:
                reactantfirstHalf = reactantfirstHalf.replace(dic[element],element)
                reactantsecondHalf = reactantsecondHalf.replace(dic[element],element)
                productfirstHalf = productfirstHalf.replace(dic[element],element)
                productsecondHalf = productsecondHalf.replace(dic[element],element)

            difference = difflib.ndiff(reactantfirstHalf,productfirstHalf)
            difference2 = difflib.ndiff(reactantsecondHalf,productsecondHalf)
            difference1 =  [x for x in difference if '+' in x or '-' in x]
            difference2 =  [x for x in difference2 if '+' in x or '-' in x]
        else:
            #TODO: dea with reactions of the kindd a+b ->  c + d
            return [],[],[],[],
        return difference1,difference2,[reactantfirstHalf,productfirstHalf], \
                [reactantsecondHalf,productsecondHalf]
             
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
        #print tupleC

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
                        #print 'malformed json file in the definitions section, using defaults'
                        site = reactionType
                        state = reactionType[0]
                    reactionTypeProperties[reactionType] = [site,state]
        #TODO: end of delete
        reactionDefinition = detectOntology.loadOntology(self.namingConventions)
        for idx,reactionType in enumerate(reactionDefinition['modificationList']):
            site = reactionDefinition['reactionSite'][reactionDefinition['definitions'][idx]['rsi']]
            state = reactionDefinition['reactionState'][reactionDefinition['definitions'][idx]['rst']]
            reactionTypeProperties[reactionType] = [site,state]
        return reactionTypeProperties


    def classifyReactions(self,reactions,molecules):
        '''
        classifies a group of reaction according to the information in the json
        config file
        '''
        
        #load the json config file
        reactionDefinition = self.loadConfigFiles(self.configurationFile)
        
        #load user defined complexes        
        if self.speciesEquivalences != None:
            self.userEquivalences = self.loadConfigFiles(self.speciesEquivalences)['reactionDefinition']
        #determines if two molecules have a relationship according to the naming convention section
        #equivalenceTranslator is a dictionary of actual modifications
        #example {'Phosporylation':[['A','A_p'],['B','B_p']]}
        
        #process straightforward naming conventions
        #equivalenceTranslator,translationKeys,conventionDict = self.processNamingConventions(molecules,reactionDefinition)
                    
        equivalenceTranslator,translationKeys,conventionDict = self.processNamingConventions2(molecules)
    
        
        
        #lists of plain reactions
        rawReactions = [self.parseReactions(x) for x in reactions]
        #process fuzzy naming conventions based on reaction information
        indirectEquivalenceTranslator= {x:[] for x in equivalenceTranslator}
        for reaction in rawReactions:
            
            if len(reaction[0]) == 2:
                d1,d2,firstMatch,secondMatch= self.approximateMatching(reaction,
                                                            translationKeys)
                #print reaction,d1,d2
                idx1=0
                idx2 = 1
                matches = [firstMatch,secondMatch]
                for index,element in enumerate([d1,d2]):
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

        reactionClassification = self.getReactionClassification(reactionDefinition,
                                            rawReactions,equivalenceTranslator,
                                            indirectEquivalenceTranslator,
                                            translationKeys)
        listOfEquivalences = []
        
        
        for element in equivalenceTranslator:
            listOfEquivalences.extend(equivalenceTranslator[element])
        return reactionClassification,listOfEquivalences,equivalenceTranslator, \
                indirectEquivalenceTranslator
    
    
 
    
    def processAnnotations(self,molecules,annotations):
        processedAnnotations = []
        for element in annotations:
            if len(annotations[element]) > 1:
                pro = [list(x) for x in itertools.combinations([y for y in annotations[element]],2)]
                processedAnnotations.extend(pro)
            
        return {-1:processedAnnotations}
    
    def annotationClassificationHelper(self,reactions,annotations):
        for reaction in reactions:
           for annotation in annotations:
               
               if ((annotation[0],) in reaction[0] and (annotation[1],) in reaction[1]) or ((annotation[0],) in reaction[1] and (annotation[1],) in reaction[0]):
                       print reaction,annotation
    
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
    
    def getUserDefinedComplexes(self):
        dictionary = {}
        labelDictionary = {}
        equivalencesList = []
        if self.speciesEquivalences != None:
            speciesdictionary =self.loadConfigFiles(self.speciesEquivalences)
            userEquivalences = speciesdictionary['complexDefinition'] \
                if 'complexDefinition' in speciesdictionary else None
            for element in userEquivalences:
                tmp = st.Species()
                label = []
                for molecule in element[1]:
                    tmp2 = st.Molecule(molecule[0])
                    for componentIdx in range(1,len(molecule),2):
                        tmp3 = st.Component(molecule[componentIdx])
                        if len(molecule[componentIdx+1])>0:
                            if molecule[componentIdx+1][0] == "b":
                                tmp3.addBond(molecule[componentIdx+1][1])
                            elif molecule[componentIdx+1][0] == "s":
                                tmp3.addState('U')
                                tmp3.addState(molecule[componentIdx+1][1])
                                equivalencesList.append([element[0],molecule[0]])
                        
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
                    
                dictionary[element[0]] = deepcopy(tmp)
                labelDictionary[element[0]] = [tuple(label)]
            complexEquivalences = speciesdictionary['modificationDefinition']
            for element in complexEquivalences:
                labelDictionary[element] = [tuple(complexEquivalences[element])]
        return dictionary,labelDictionary
        
