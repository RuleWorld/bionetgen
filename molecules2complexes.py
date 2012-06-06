# -*- coding: utf-8 -*-
"""
Created on Tue Dec  6 17:42:31 2011

@author: proto
"""
from libsbml2bngl import SBML2BNGL
from pyparsing import Word, Suppress, Optional, alphanums, Group
import libsbml
from numpy import sort
from copy import deepcopy
import reactionTransformations
import analyzeSBML
import analyzeRDF

    
def parseReactions(reaction):
    species = Optional(Word(alphanums+"_") + Suppress('()')) +  \
    Optional(Suppress('+') + Word(alphanums+"_") + Suppress("()")) + \
    Optional(Suppress('+') + Word(alphanums+"_") + Suppress("()"))
    rate = Word(alphanums + "()")
    grammar = (Group(species) + Suppress("->") + Group(species) + Suppress(rate)) \
    ^ (species + Suppress("->") + Suppress(rate))  
    result =  grammar.parseString(reaction).asList()
    if len(result) < 2:
        result = [result,[]]
    return result

def identifyReaction(reaction,element):
    '''
    this method only uses immediate context information to determine
    the type a reaction belongs to
    '''
    identifier = -1
    if len(reaction) == 2: 
        if len(reaction[0]) == 2 and len(reaction[1]) == 1:
            identifier = 1
        elif len(reaction[0]) == 2 and len(reaction[1]) == 2:
            identifier = 2
        elif len(reaction[0])==1 and len(reaction[1])==2:
            identifier = 3
        elif len(reaction[0])==0 and len(reaction[1])==1:
            identifier = 4
        elif len(reaction[0])==1 and len(reaction[1])==0:
            identifier = 5
        elif len(reaction[0])==1 and len(reaction[1]) == 1:
            identifier = 6
        return identifier

        
def issubset(possible_sub, superset):
    return all(element in superset for element in possible_sub)


    
#this method is used during a catalysis reaction
def updateState(original):
    pass

def addToLabelDictionary(dictionary,label,value):
    #dictionary[label] = value
    if label not in dictionary:
        dictionary[label] = [value]
    else:
        dictionary[label].append(value)
       # dictionary[label].sort()
def defineCorrespondence(reaction2, totalElements,database,
    classification,rdfAnnotations):
    '''
    this method goes through the list of reactions and determines what kind of 
    reaction each one is (eg. catalysis, synthesis etc). It also fills the database
    with information about what each sbml molecule is equal to in bngl lingo._
    eg S1 + s2 -> s3 would return s3 == s1.s2
    '''
    for element in totalElements:
        #if it's already in the database
        #if element in database.labelDictionary:
        #    continue
        #if it's in the list of elements provided by the user
        if (element,) in database.rawDatabase:
            #labelDictionary[element] = []
            #database.labelDictionary[element] = (element,)
            addToLabelDictionary(database.labelDictionary,element,(element,))
        #if we can obtain what it is according to what appears in the annotations
        elif classification == 'Binding' and (element not in database.labelDictionary
        or (len(database.labelDictionary[element]) < 2)):
                if len(reaction2[0]) == 2 and element not in reaction2[0]:
                    addToLabelDictionary(database.labelDictionary,element,tuple([k for k in reaction2[0]]))
                    #database.labelDictionary[element] = tuple([k for k in reaction2[0]])
                elif len(reaction2[1]) == 2 and element not in reaction2[1]:
                    addToLabelDictionary(database.labelDictionary,element,tuple([k for k in reaction2[1]]))
                    #database.labelDictionary[element] = tuple([k for k in reaction2[1]])
                else:
                    addToLabelDictionary(database.labelDictionary,element,(element,))
                    #database.labelDictionary[element] = (element,)
        elif classification == 'Phosporylation' or classification == 'Double-Phosporylation':
            #print element
            #addToLabelDictionary(database.labelDictionary,element,(element,))
            pass
        else:
            equivalence = analyzeRDF.getEquivalence(element,rdfAnnotations)
            if equivalence != []:
                for equivalentElement in equivalence:
                    if equivalentElement in database.labelDictionary:
                        addToLabelDictionary(database.labelDictionary,element,(equivalentElement,))
                        #database.labelDictionary[element] = (equivalentElement,)
            else:
                addToLabelDictionary(database.labelDictionary,element,(element,))
    return database.labelDictionary

##TODO: i introduced arrays in the labelDictionary, have to resolve correspondences                  
def resolveCorrespondence(database):
    temp = database.labelDictionary.copy()
    for element in database.labelDictionary:
       # for idx,alternative in enumerate(database.labelDictionary[element]):
        for member in [x for x in database.labelDictionary[element]]:
            tmpLabel= member
            while True:
                oldTemp = deepcopy(temp[element])
                temp[element] = list(temp[element])
                temp[element].extend(database.labelDictionary[tmpLabel])
                temp[element].remove(tmpLabel)
                temp[element] = tuple(sort(temp[element]))
                tmpLabel = database.labelDictionary[tmpLabel][0]
                if oldTemp == temp[element]:
                    break
                
        #temp[element] = list(set(temp[element]))
    labelDictionary = temp.copy()
    return labelDictionary
               
                    
def printReactions(history):
    temp = []
    for element in history:
        for reactant in element:
            temp.append(printReactants(reactant))
        print ' -> '.join(temp)
        temp = []

def printReactionsWithDictionary(originalRules,dictionary):
    #print originalRules,dictionary
    for rule in originalRules:
        reactionList = []
        parsedReaction = list(parseReactions(rule))
        for reactionSide in parsedReaction:
            reactionSideList= []
            if reactionSide == []:
                reactionSideList.append('0')
                
            for reactant in reactionSide:
                if reactant in translator:
                    reactionSideList.append(printReactants([translator[reactant]]))
                else:
                    reactionSideList.append(reactant + '()')
            reactionList.append(' + '.join(reactionSideList))
        print ' -> '.join(reactionList)
            

def printReactants(reactants):
    temp = []
    if isinstance(reactants[0],int):
        return str(reactants[0])
    elif isinstance(reactants[0],str):
        return reactants[0] + '()'
    else:
        for element in reactants:
            temp.append(reactionTransformations.printSpecies(element[0],
                                                             element[1]))
        return ' + '.join(temp)

def getPairIntersection(set1, set2):
    acc = []
   # print set1,'+++',set2
    for el1,el2 in zip(set1, set2):
        temp2 = []
        temp2.append(tuple([val for val in el1[0] if val in el2[0]]))
        for el1x,el2x in zip(el1[1],el2[1]):
            temp2.append(([val for val in el1x if val in el2x],))
        acc.append(tuple(temp2))
    return acc

def getNewProduct(factor, temp):
    if temp == [0]:
        return factor[1]
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
            if factorMember == 0:
                continue
            for factorGeneral in [x for x in reactantGeneral if x not in temp]:
                if issubset(factorMember[0],factorGeneral[0]):
                    temp.append(factorGeneral)
                    continue
        if len(temp) > 1:
            #print temp,element[0],temp==element[0]
            tempArray.append(temp)
            finalArray.append(element)
    #print '----'
    temp = factor[0]
    for element in tempArray:
        temp = getPairIntersection(temp,element)
    newFactor =  [temp,getNewProduct(factor,temp)]
        #temp2 = getPairIntersection(temp)
    return newFactor,finalArray        

def transformRawType(originalRule,translator):
    '''
    this method grabs a rule that has a standard sbml data type and transforms
    it to the data type we use internally, with no components
    '''
    newRule = []
    for element in originalRule:
        acc = []
        for temp in element:
            if temp in translator:
                acc.append(translator[temp])
            else:
                acc.append(((temp,),([],),))
        newRule.append(acc)
    return newRule

def getPertinentNamingEquivalence(original, database):
    temp = [item for element in original for item in element]
    temp2 = [database.labelDictionary[x] for x in temp]
    temp3 = [x for x in temp2 if temp2.count(x) == 1]
    temp4 = [x for x in temp if (x,) not in temp3]
    temp4.sort(lambda x,y: cmp(len(x), len(y)))
    return temp4

def processRule(original,database,
                classification,equivalenceTranslator):
    '''
    '''
    #print (identifyReaction(original,0))
    if identifyReaction(original,0) == 1 and classification == 'Binding':
        return reactionTransformations.synthesis(original,database.labelDictionary,
        database.rawDatabase,database.synthesisDatabase,database.translator)
    elif classification in ['Phosporylation','Double-Phosporylation','Generic-Catalysis']:
        pertinentEquivalence = getPertinentNamingEquivalence(original,database)
        print 'aaaaaa',pertinentEquivalence
        return reactionTransformations.catalysis(original,database.labelDictionary,database.rawDatabase,
                                                  None,
                                                  database.translator,pertinentEquivalence,
                                                  classification)

    
    elif identifyReaction(original,0) == 4:
        #return reactionTransformations.creation(original,dictionary,
        #rawDatabase,translator)
        return ''
    elif identifyReaction(original,0) == 5:
        #return reactionTransformations.decay(original,dictionary,
        #rawDatabase,translator)
        return ''
    else:
        #return transformRawType(original,translator)
        return ''
        
def reduceReactions(history):
    newHistory = deepcopy(history)
    for x in range(0,3):
        (factor,eliminate) = factorize(newHistory[0],newHistory)
        for element in eliminate:
            newHistory.remove(element)
        newHistory.append(factor)
    return newHistory
    
def simplify(labelDatabase):
    for element in labelDatabase:
        if len(labelDatabase[element]) == 1:
            labelDatabase[element] = labelDatabase[element][0]
        #placeholder
        else:
             labelDatabase[element] = max(labelDatabase[element],key=len)
        #else:
        #    labelDatabase[element] = labelDatabase[element][0]

#TODO: not all the reactions im defining as catalysis are actually catalysis
def correctClassifications(rules,classifications,labelDatabase):
    print labelDatabase
    for idx in range(0,len(rules)):
        if classifications[idx] == 'Binding':
            reaction2 = list(parseReactions(rules[idx]))
            if len(labelDatabase[reaction2[1][0]]) >1:
                #if we found that a binding is actually a catalysis
                classifications[idx] = 'Generic-Catalysis'
                # we will assume that they smallest element
                print reaction2
                print labelDatabase[reaction2[1][0]]
                labelDatabase[reaction2[1][0]] = [min(labelDatabase[reaction2[1][0]],key=len)]
                
            
            
def transformMolecules(parser,database):
    #labelDictionary = {}
    _,rules,_ = parser.getReactions()
    molecules,_,_ = parser.getSpecies()
    #synthesisdatabase = {}
    #translator = {}
    
    classifications,equivalenceTranslator = analyzeSBML.classifyReactions(rules,molecules)
    #analyzeSBML.analyzeNamingConventions(molecules)
    rdfAnnotations = analyzeRDF.getAnnotations(parser,'uniprot')
    #print rdfAnnotations
    print 'starting'
    
    for element in equivalenceTranslator:
        database.labelDictionary[element[0]] = [(min(element,key=len),)]
        database.labelDictionary[element[1]] = [(min(element,key=len),)]
    for rule,classification in zip(rules,classifications): 
        #print rule 
        reaction2 = list(parseReactions(rule))
        totalElements =  [item for sublist in reaction2 for item in sublist]
        totalElements = list(set(totalElements))
        database.labelDictionary = defineCorrespondence(reaction2,totalElements,
                                               database,
                                               classification,rdfAnnotations)
        #database.labelDictionary = resolveCorrespondence(database)
    correctClassifications(rules,classifications,database.labelDictionary)
    simplify(database.labelDictionary)
    for _ in range(0,5):
        database.labelDictionary = resolveCorrespondence(database)
    #print database.labelDictionary
    for rule,classification in zip(rules,classifications):
        reaction2 = list(parseReactions(rule))
        processRule(reaction2,database,classification,equivalenceTranslator)
    #update all equivalences
    for element in database.labelDictionary:
        if not isinstance(database.labelDictionary[element],tuple):
            database.translator[element] = database.translator[database.labelDictionary[element]]
    #print translator
   # print labelDictionary
    return database.translator

if __name__ == "__main__":

#    database = {('S1',):(["r","l"],),("S2",):(["s"],),}    
    #database = {('S1',):("r","l"),("S2",):("s"),
                 #('S1','S2'):([('r','1'),('l')],[('s','1')]),
                #('S1','S2','S2'):([('r','1'),('l','2')],[('s','1')],[('s','2')])}
    #database = {('S1',):(["a","b"],),("S2",):(["r"],),('S3',):(['l'],),('S1','S2'):([('a','1')],[('r','1')]),('S1','S3'):([('b','2')],[('l','2')])}
    #database = {('S1',):(["a","b"],),("S2",):(["r"],),('S3',):(['l'],),('S4',):(['t'],),('S1','S2'):([('a','1')],[('r','1')]),('S1','S3'):([('b','2')],[('l','2')]),('S1','S4'):([('c','3')],[('t','3')])}
    #rawDatabase = {('S1',):([("a",),("b",),("c",)],),("S2",):([("r",)],),
    #              ('S3',):([("l",)],),('S4',):([('t',)],)}  
    #catalysisDatabase = {(('S1',),'P'):(([("a",'','U')]),([("a",'','P')]))}
    catalysisDatabase = {}    
    #rawDatabase = {('EpoR',):(['r','U','I'],),('SAv',):(['l'],)}    
    rawDatabase={}    
    #synthesisdatabase = {('S1','S2'):([('b','1')],[('r','1')])}
    synthesisdatabase = {}
    history = []
    translator = {}
    reader = libsbml.SBMLReader()
    #BIOMD0000000272
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000272.xml')
    #document = reader.readSBMLFromFile('XMLExamples/simple4.xml')
    model = document.getModel()        
    parser = SBML2BNGL(model)
    _,rules,_ = parser.getReactions()
    
    translator = transformMolecules(rules,rawDatabase)
    #print translator
    printReactionsWithDictionary(rules,translator)
    #print 'reducing...'  
    #newHistory = reduceReactions(history)
    #printReactions(newHistory)
    #print history
    #for x in range(0,3)
    #    factorize(history[0],history)
        
    
   # print history
#    print translator
    #for rule in rules:
    #    print parseReactions(reaction)
    
        

    