# -*- coding: utf-8 -*-
"""
Created on Thu Mar 22 13:11:38 2012

@author: proto
"""

from libsbml2bngl import SBML2BNGL
from pyparsing import Word, Suppress,Optional,alphanums,Group
import libsbml
from numpy import sort,zeros,nonzero
import numpy as np
import json
import analyzeRDF
import re
'''
This file in general classifies rules according to the information contained in
the json config file for classyfying rules according to their reactants/products
'''

def parseReactions(reaction):
    species = Optional(Word(alphanums+"_") + Suppress('()')) + Optional(Suppress('+') + Word(alphanums+"_") 
    + Suppress("()")) + Optional(Suppress('+') + Word(alphanums+"_") 
    + Suppress("()")) + Optional(Suppress('+') + Word(alphanums+"_") 
    + Suppress("()"))
    rate = Word(alphanums + "()")
    grammar = (Group(species) + Suppress("->") + Group(species) + Suppress(rate)) \
    ^ (species + Suppress("->") + Suppress(rate))  
    result =  grammar.parseString(reaction).asList()
    if len(result) < 2:    
        result = [result,[]] 
        
    return result



def loadConfigFiles():
    '''
    the reactionDefinition file must contain the definitions of the basic reaction types 
    we wnat to parse and what are the requirements of a given reaction type to be considered
    as such
    '''
    with open('reactionDefinition2.json','r') as fp:
        reactionDefinition = json.load(fp)
    return reactionDefinition

def identifyReactions2(rule,reactionDefinition):
    '''
    This method goes through the list of common reactions listed in ruleDictionary
    and tries to find how are they related according to the information in reactionDefinition
    '''  
    #print reactionDefinition
    result = []
    for idx,element in enumerate(reactionDefinition['reactions']):
        if(len(rule[0]) == len(element[0]) and len(rule[1]) == len(element[1])):
            result.append(1)           
#            for (el1,el2) in (element[0],rule[0]):
#                if element[0].count(el1) == element[]
        else:
            result.append(0)
    return result

  
    
def species2Rules(rules):
    '''
    This method goes through the rule list and classifies species tuples in a dictionary
    according to the reactions they appear in.
    '''
    ruleDictionary = {}
    for rule in rules:
        reaction2 = rule #list(parseReactions(rule))
        #print reaction2
        totalElements =  [item for sublist in reaction2 for item in sublist]
        if frozenset(totalElements) in ruleDictionary:
            ruleDictionary[frozenset(totalElements)].append(rules.index(rule))
        else:
            ruleDictionary[frozenset(totalElements)] = [rules.index(rule)]
    return ruleDictionary

def checkCompliance(ruleCompliance,tupleCompliance,ruleBook):
    '''
    This method is mainly useful when a single rule can be possibly classified
    in different ways, but in the context of its tuple partners it can only be classified
    as one
    '''
    ruleResult = np.zeros(len(ruleBook))
    for validTupleIndex in np.nonzero(tupleCompliance):
        for index in validTupleIndex:
            if 'r' in ruleBook[index] and np.any([ruleCompliance[temp] for temp in ruleBook[index]['r']]):
                ruleResult[index] = 1
            #check if just this is enough
            if 'n' in ruleBook[index]:
                ruleResult[index] = 1
    return ruleResult
        
 
def analyzeNamingConventions(molecules,originalPattern='',modifiedPattern=''):
    '''
    *originalPattern* and *modifiedPattern* are regular expressions containing
    the patterns we wish to compare and see if they are the same.
    We will go through the list of molecules and check for names that match those
    patterns
    '''
    original = originalPattern[0].replace('\\\\', '\\')
    modified = modifiedPattern[0].replace('\\\\', '\\')
    pOriginal = re.compile(original)
    pModified = re.compile(modified)
    oMolecules = []
    
    results = []    
    
    for molecule in molecules:
        omatch = pOriginal.match(molecule)        
        if omatch:
            oMolecules.append(omatch.group('key'))
            
    for molecule in molecules:
        mmatch = pModified.match(molecule)        
        if mmatch and mmatch.group('key') in oMolecules:
            results.append((mmatch.group('key'),molecule[0:-2]))
           
    return results
 
def processNamingConventions(molecules,namingConventions):
    equivalenceTranslator = {}
    for idx,convention in enumerate(namingConventions):
        temp = analyzeNamingConventions(molecules,convention[0],convention[1])
        equivalenceTranslator[idx] = temp
    return equivalenceTranslator

def getReactionClassification(reactionDefinition,rules,equivalenceTranslator,useNamingConventions=True):
    '''
    *reactionDefinition* is ....
    *rules*
    This method will go through the list of rules and the list of rule definitions
    and tell us which rules it can classify according to the rule definitions list
    provided
    '''
    ruleDictionary = species2Rules(rules)
    #contains which rules are equal to reactions defined in reactionDefiniotion['reactions]    
    ruleComplianceMatrix = zeros((len(rules),len(reactionDefinition['reactions'])))
    for (idx,rule) in enumerate(rules):
        reaction2 = rule #list(parseReactions(rule))
        ruleComplianceMatrix[idx] = identifyReactions2(reaction2,reactionDefinition)
    #initialize the tupleComplianceMatrix array with the same keys as ruleDictionary
    tupleComplianceMatrix = {key:zeros((len(reactionDefinition['reactions']))) for key in ruleDictionary}
    #check which reaction conditions each tuple satisfies
    for element in ruleDictionary:
        for rule in ruleDictionary[element]:
            tupleComplianceMatrix[element] += ruleComplianceMatrix[rule]     
    #print tupleC
    #now we will check for the nameConventionMatrix
    
    tupleNameComplianceMatrix = {key:zeros((len(reactionDefinition['namingConvention']))) for key in ruleDictionary}
    for rule in ruleDictionary:
        for namingConvention in equivalenceTranslator:
            for equivalence in equivalenceTranslator[namingConvention]:
                if all(element in rule for element in equivalence):
                    tupleNameComplianceMatrix[rule][namingConvention] +=1
    #check if the reaction conditions each tuple satisfies are enough to get classified
    #as an specific named reaction type
    tupleDefinitionMatrix = {key:zeros((len(reactionDefinition['definitions']))) for key in ruleDictionary}
    for key,element in tupleComplianceMatrix.items():
        for idx,member in enumerate(reactionDefinition['definitions']):
            #tupleDefinitionMatrix[key][idx] = True
            if 'r' in member:            
                tupleDefinitionMatrix[key][idx] = np.all([element[reaction] for reaction in member[u'r']])
            if 'n' in member:
                tupleDefinitionMatrix[key][idx] = np.all([tupleNameComplianceMatrix[key][reaction] for reaction in member[u'n']])
           # if 'n' in member:
            #    tupleDefinitionMatrix[key][idx] = tupleDefinitionMatrix[key][idx]  and ruleNameComplianceMatrix
            #if 'n' in member:
    #cotains which rules are equal to reactions defined in reactionDefinitions['definitions']
    #use the per tuple classification to obtain a per reaction classification
    ruleDefinitionMatrix = zeros((len(rules),len(reactionDefinition['definitions'])))
    for key,element in ruleDictionary.items():
        for rule in element:
            ruleDefinitionMatrix[rule] = checkCompliance(ruleComplianceMatrix[rule],
tupleDefinitionMatrix[key],reactionDefinition['definitions'])
    #use reactionDefinitions reactionNames field to actually tell us what reaction
    #type each reaction is
    results = []    
    for element in ruleDefinitionMatrix:
        nonZero = nonzero(element)[0]
        if(len(nonZero) == 0):
            results.append('None')
        #todo: need to do something if it matches more than one reaction
        else:
            results.append(reactionDefinition['reactionsNames'][nonZero[0]])
            
    #now we will process the naming conventions section
    #print results
    return  results
    
def classifyReactions(reactions,molecules):
    '''
    classifies a group of reaction according to the information in the json
    config file
    '''
    reactionDefinition = loadConfigFiles()
    equivalenceTranslator = {}
    #determines if two molecules have a relationship according to the naming convention section
    equivalenceTranslator = processNamingConventions(molecules,reactionDefinition['namingConvention'])
    rawReactions = [parseReactions(x) for x in reactions]
    reactionClassification = getReactionClassification(reactionDefinition,rawReactions,equivalenceTranslator)
    listOfEquivalences = []
    for element in equivalenceTranslator:
        listOfEquivalences.extend(equivalenceTranslator[element])
    return reactionClassification,listOfEquivalences


def reclassifyReactions(reactions,molecules,labelDictionary):
    reactionDefinition = loadConfigFiles()

    rawReactions = [parseReactions(x) for x in reactions]
    #print labelDictionary
    reactionDefinition = loadConfigFiles()
    equivalenceTranslator = processNamingConventions(molecules,reactionDefinition['namingConvention'])
    for reactionIndex in range(0,len(rawReactions)):
        for reactantIndex in range(0,len(rawReactions[reactionIndex])):
            tmp = []
            for chemicalIndex in range(0,len(rawReactions[reactionIndex][reactantIndex])):
                tmp.extend(list(labelDictionary[rawReactions[reactionIndex][reactantIndex][chemicalIndex]]))
            rawReactions[reactionIndex][reactantIndex] = tmp
    reactionClassification = getReactionClassification(reactionDefinition,rawReactions,equivalenceTranslator)
    return reactionClassification           

if __name__ == "__main__":
    reader = libsbml.SBMLReader()
    #BIOMD0000000272
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000272.xml')
    #document = reader.readSBMLFromFile('XMLExamples/simple4.xml')
    model = document.getModel()        
    parser = SBML2BNGL(model)
#    print parser.getReactions()
    #parser.processFile('flat.bngl')
    _,rules,_ = parser.getReactions()
    molecules,_,_ = parser.getSpecies(translator)
    #print rules    
    #print classifyReactions(rules)
    analyzeNamingConventions(molecules)