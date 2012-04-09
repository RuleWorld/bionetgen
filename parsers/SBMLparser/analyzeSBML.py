# -*- coding: utf-8 -*-
"""
Created on Thu Mar 22 13:11:38 2012

@author: proto
"""

from libsbml2bngl import SBML2BNGL
from pyparsing import Word, Suppress,Optional,alphanums,Group
import libsbml
from numpy import sort,zeros
import json

def identifyReaction(reaction,element):
    if len(reaction) == 2: 
        if len(reaction[0]) == 2 and len(reaction[1]) == 1:
            return 1
        elif len(reaction[0]) == 2 and len(reaction[1]) == 2:
            return 2
        elif len(reaction[0])==1 and len(reaction[1])==2:
            return 3
        elif len(reaction[0])==0 and len(reaction[1])==1:
            return 4
        elif len(reaction[0])==1 and len(reaction[1])==0:
            return 5
        elif len(reaction[0])==1 and len(reaction[1]) == 1:
            return 6
        else:
            return -1




def parseReactions(reaction):
    
    species = Optional(Word(alphanums+"_") + Suppress('()')) + Optional(Suppress('+') + Word(alphanums+"_") 
    + Suppress("()"))
    rate = Word(alphanums + "()")
    grammar = (Group(species) + Suppress("->") + Group(species) + Suppress(rate)) \
    ^ (species + Suppress("->") + Suppress(rate))  
    result =  grammar.parseString(reaction).asList()
    if len(result) < 2:
        result = [result,[]]
    return result

        
def defineCorrespondence(reaction2, totalElements,labelDictionary,rawDatabase, synthesisDatabase,catalysisDatabase):
    for element in totalElements:
        if (element,) in rawDatabase:
            #labelDictionary[element] = []
            labelDictionary[element] = (element,)
        else:
            typeOfReaction = identifyReaction(reaction2,element)
            if typeOfReaction == 1:
                if element in reaction2[1]:
                    labelDictionary[element] = tuple([k for k in reaction2[0]])

            elif typeOfReaction == 2:
                labelDictionary[element] = (tuple([k for k in reaction2[0] if k in reaction2[1]]),
                    tuple([k for k in reaction2[0] if k in reaction2[1]]))
            #if element in reaction2[0] and reaction[1] then theres a state change reaction
            elif typeOfReaction == 3:
                #print reaction2,element
                #print labelDictionary
                print 'hola'
            elif typeOfReaction == 4 or typeOfReaction == 5:
                pass
            elif typeOfReaction == 6:
                #print reaction2
                if tuple(reaction2[1]) in rawDatabase:
                    print 'decay'
                else:
                    print 'conversion'
            else:
                print 'Ive no idea what youre talking about'
    return labelDictionary

def inferReactants():
    pass


def classifyRules(rules):
    '''
    This method is designed to go through all rules and get them together in a 
    dictonary depending on which elements are involved.
    '''
    ruleDictionary = {}
    
    
##TODO: i introduced arrays in the labelDictionary, have to resolve correspondences                  
def resolveCorrespondence(labelDictionary):
    temp = labelDictionary.copy()
    for element in [x for x in labelDictionary if len(labelDictionary[x]) > 1]:
        for member in [x for x in labelDictionary[element] if len(labelDictionary[x]) > 1]:
            temp[element] = list(temp[element])
            temp[element].extend(labelDictionary[member])
            temp[element].remove(member)
            temp[element] = tuple(sort(temp[element]))
    labelDictionary = temp.copy()
    return labelDictionary

def loadConfigFiles():
    with open('reactionDefinition.json','r') as fp:
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
        reaction2 = list(parseReactions(rule))
        #print reaction2
        totalElements =  [item for sublist in reaction2 for item in sublist]
        if frozenset(totalElements) in ruleDictionary:
            ruleDictionary[frozenset(totalElements)].append(rules.index(rule))
        else:
            ruleDictionary[frozenset(totalElements)] = [rules.index(rule)]
    return ruleDictionary

def getDescription(species,rules):
    '''
    This method will go through the list of rules and the list of rule definitions
    and tell us which rules it can classify according to the rule definitions list
    provided
    '''
    ruleDictionary = species2Rules(rules)
    reactionDefinition = loadConfigFiles()
    ruleComplianceMatrix = zeros((len(rules),len(reactionDefinition['reactions'])))
    for (idx,rule) in enumerate(rules):
        reaction2 = list(parseReactions(rule))
        ruleComplianceMatrix[idx] = identifyReactions2(reaction2,reactionDefinition)
    
    tupleComplianceMatrix = {key:zeros((len(reactionDefinition['reactions']))) for key in ruleDictionary}
    for element in ruleDictionary:
        for rule in ruleDictionary[element]:
            tupleComplianceMatrix[element] += ruleComplianceMatrix[rule]     
    #labelDictionary = resolveCorrespondence(labelDictionary)
    #print labelDictionary
    print tupleComplianceMatrix
    
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
    print rules    
    getDescription([],rules)