# -*- coding: utf-8 -*-
"""
Created on Thu Mar 22 13:11:38 2012

@author: proto
"""

from libsbml2bngl import SBML2BNGL
from pyparsing import Word, Suppress,Optional,alphanums,Group
import libsbml
from numpy import sort
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

def identifyReactions2():
    '''
    This method goes through the list of common reactions listed in ruleDictionary
    and tries to find how are they related according to the information on reactionDefinition
    '''    

def getDescription(species,rules):
    #rawDatabase = {}
    #rawDatabase = {('S1',):([("a",),("b",),("c",)],),("S2",):([("r",)],),('S3',):([("l",)],),('S4',):([('t',)],)}  
    rawDatabase = {}
    synthesisDatabase = {}
    labelDictionary = {}
    catalysisDatabase = {}
    
    ruleDictionary = {}
    reactionDefinition = loadConfigFiles()
    for rule in rules:   
        reaction2 = list(parseReactions(rule))
        #print reaction2
        totalElements =  [item for sublist in reaction2 for item in sublist]
        if tuple(totalElements) in ruleDictionary:
            ruleDictionary[tuple(totalElements)].append(rules.index(rule))
        else:
            ruleDictionary[tuple(totalElements)] = [rules.index(rule)]
        totalElements = list(set(totalElements))
        #labelDictionary = defineCorrespondence(reaction2,totalElements,labelDictionary,rawDatabase,synthesisDatabase,catalysisDatabase)
        #print labelDictionary        
        #labelDictionary = resolveCorrespondence(labelDictionary)

    print ruleDictionary    
    #labelDictionary = resolveCorrespondence(labelDictionary)
    #print labelDictionary
    print json.dumps(labelDictionary)
    
if __name__ == "__main__":
    reader = libsbml.SBMLReader()
    #BIOMD0000000272
    #document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000272.xml')
    document = reader.readSBMLFromFile('XMLExamples/simple4.xml')
    reactionDefinition = loadConfigFiles()
    print reactionDefinition
    model = document.getModel()        
    parser = SBML2BNGL(model)
#    print parser.getReactions()
    #parser.processFile('flat.bngl')
    _,rules,_ = parser.getReactions()
    print rules    
    getDescription([],rules)