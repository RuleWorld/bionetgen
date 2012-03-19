# -*- coding: utf-8 -*-
"""
Created on Tue Dec  6 17:42:31 2011

@author: proto
"""
from libsbml2bngl import SBML2BNGL
from pyparsing import Word, Suppress,Optional,alphanums,Group
import libsbml
from numpy import sort

import reactionTransformations
    
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
            
def issubset(possible_sub, superset):
    return all(element in superset for element in possible_sub)


    
#this method is used during a catalysis reaction
def updateState(original):
    pass

##this method goes through the list of reactions and determines what kind of 
#reaction each one is (eg. catalysis, synthesis etc). It also fills the database
#with information about what each sbml molecule is equal to in bngl lingo._
#eg S1 + s2 -> s3 would return s3 == s1.s2
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
                print reaction2,element
                print labelDictionary
            elif typeOfReaction == 4 or typeOfReaction == 5:
                pass
            elif typeOfReaction == 6:
                print reaction2
                if tuple(reaction2[1]) in rawDatabase:
                    print 'decay'
                else:
                    print 'conversion'
            else:
                
                print 'Ive no idea what youre talking about'
    return labelDictionary

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
               
                    
def printReactions(history):
    temp = []
    for element in history:
        for reactant in element:
            temp.append(printReactants(reactant))
        print ' -> '.join(temp)
        temp = []

def printReactants(reactants):
    temp = []
    for element in reactants:
        temp.append(reactionTransformations.printSpecies(element[0],element[1]))
    return ' + '.join(temp)

def getPairIntersection(set1,set2):
    acc = []
   # print set1,'+++',set2
    for el1,el2 in zip(set1,set2):
        temp2 = []
        temp2.append(tuple([val for val in el1[0] if val in el2[0]]))
        for el1x,el2x in zip(el1[1],el2[1]):
            temp2.append(([val for val in el1x if val in el2x],))
        acc.append(tuple(temp2))
    return acc

def getNewProduct(factor,temp):
    tempTag = []
    tempComponents = []
    for tag,components,reference in zip(factor[1][0][0],factor[1][0][1],temp):
        tempTag.append(tag)
        tempCompo = []
        for component in components:
            if component[0] in reference[1][0][0]:
                tempCompo.append(component)
        tempComponents.append(tempCompo)
    return [((tuple(tempTag),tempComponents))]
    
def factorize(factor,history):
    finalArray = []
    tempArray = []
    for element in history:
        reactantFactor,reactantGeneral = (factor[0],element[0])
        temp = []
        for factorMember in reactantFactor:
            for factorGeneral in [x for x in reactantGeneral if x not in temp]:
                if issubset(factorMember[0],factorGeneral[0]):
                    temp.append(factorGeneral)
                    continue
        if len(temp) > 1:
            #print temp,element[0],temp==element[0]
            tempArray.append(temp)
            finalArray.append(element)
    #print '----'
    #print factor[0],'---',finalArray
    temp = factor[0]
    for element in tempArray:
        temp = getPairIntersection(temp,element)
    temp =  [temp,getNewProduct(factor,temp)]
        #temp2 = getPairIntersection(temp)
    #print finalArray
    
    return temp,finalArray        


def processRule(original,dictionary,rawDatabase,synthesisDatabase,translator):
    print (identifyReaction(original,0))
    if identifyReaction(original,0) == 1:
        return reactionTransformations.synthesis(original,dictionary,rawDatabase,synthesisdatabase,translator)
    elif identifyReaction(original,0) == 4:
        return reactionTransformations.creation(original,dictionary,rawDatabase,translator)
    elif identifyReaction(original,0) == 5:
        return reactionTransformations.decay(original,dictionary,rawDatabase,translator)
    
        

if __name__ == "__main__":

#    database = {('S1',):(["r","l"],),("S2",):(["s"],),}    
    #database = {('S1',):("r","l"),("S2",):("s"),('S1','S2'):([('r','1'),('l')],[('s','1')]),('S1','S2','S2'):([('r','1'),('l','2')],[('s','1')],[('s','2')])}
    #database = {('S1',):(["a","b"],),("S2",):(["r"],),('S3',):(['l'],),('S1','S2'):([('a','1')],[('r','1')]),('S1','S3'):([('b','2')],[('l','2')])}
    #database = {('S1',):(["a","b"],),("S2",):(["r"],),('S3',):(['l'],),('S4',):(['t'],),('S1','S2'):([('a','1')],[('r','1')]),('S1','S3'):([('b','2')],[('l','2')]),('S1','S4'):([('c','3')],[('t','3')])}
    #rawDatabase = {('S1',):([("a",),("b",),("c",)],),("S2",):([("r",)],),('S3',):([("l",)],),('S4',):([('t',)],)}  
    #catalysisDatabase = {(('S1',),'P'):(([("a",'','U')]),([("a",'','P')]))}
    catalysisDatabase = {}    
    rawDatabase = {('EpoR',):(['r','U','I'],),('SAv',):(['l'],)}    
    #synthesisdatabase = {('S1','S2'):([('b','1')],[('r','1')])}
    synthesisdatabase = {}
    history = []
    labelDictionary = {}
    translator = {}
    reader = libsbml.SBMLReader()
    #BIOMD0000000272
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000272.xml')
    #document = reader.readSBMLFromFile('XMLExamples/simple4.xml')
    
    model = document.getModel()        
    parser = SBML2BNGL(model)
#    print parser.getReactions()
    _,rules,_ = parser.getReactions()
    
    print rules
    #print rules
    print 'preparing database...'
    for rule in rules:   
        #print rule
        reaction2 = list(parseReactions(rule))
        #print reaction2
        totalElements =  [item for sublist in reaction2 for item in sublist]
        totalElements = list(set(totalElements))
        labelDictionary = defineCorrespondence(reaction2,totalElements,labelDictionary,rawDatabase,synthesisdatabase,catalysisDatabase)
        #print labelDictionary        
        labelDictionary = resolveCorrespondence(labelDictionary)

    labelDictionary = resolveCorrespondence(labelDictionary)
    print labelDictionary
    #print labelDictionary
    print 'translating...'
    for rule in rules:
        print rule
        reaction2 = list(parseReactions(rule))
        reaction = processRule(reaction2,labelDictionary,rawDatabase,synthesisdatabase,translator)
        print reaction
        history.append(reaction)
    printReactions(history)
    #print history[0]
   # print history
    print 'reducing...'  
    for x in range(0,3):
        (factor,eliminate) = factorize(history[0],history)
        for element in eliminate:
            history.remove(element)
        history.append(factor)
    #print synthesisdatabase
    printReactions(history)
    #print history
    #for x in range(0,3)
    #    factorize(history[0],history)
        
    
   # print history
#    print translator
    #for rule in rules:
    #    print parseReactions(reaction)
    
        

    