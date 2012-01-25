# -*- coding: utf-8 -*-
"""
Created on Tue Dec  6 17:42:31 2011

@author: proto
"""
from libsbml2bngl import SBML2BNGL
from pyparsing import Word, Suppress,Optional,alphanums,Group
import libsbml
from numpy import sort
from copy import copy
import random
import sys

def translate(reaction,bond):
    ##change to contains
    newReaction = [[],[]]
    #print reaction
    if set(reaction[0]) == set(bond.keys()):
        newReaction[0] = reaction[0]
        newReaction[1] = [bond]
    return newReaction

def issubset(possible_sub, superset):
    return all(element in superset for element in possible_sub)
    
def parseReactions(reaction):
    
    grammar = Group(Word(alphanums) + Suppress('()') + Optional(Suppress('+') + Word(alphanums) 
    + Suppress("()"))) + Suppress("->") + Group(Word(alphanums) + Suppress("()") + Optional(Suppress('+') 
    + Word(alphanums) + Suppress("()"))) + Suppress(Word(alphanums + "()")) 
   
    return grammar.parseString(reaction).asList()

def identifyReaction(reaction,element):
    if len(reaction) == 2: 
        if len(reaction[0]) == 2 and len(reaction[1]) == 1:
            return 1
        elif len(reaction[0]) == 2 and len(reaction[1]) == 1:
            return 2

def printSpecies(label,definition):
    molecules = []
    for tag,species in zip(label,definition):
        components = []
        for member in species:
            if(len(member) == 2):
                components.append('{0}!{1}'.format(member[0],member[1]))
            else:
                components.append('{0}'.format(member[0]))
        molecules.append('{0}({1})'.format(tag,",".join(components)))
    return ".".join(molecules)

def synthesis(original,dictionary,database,translator):
    chemicals = []
    rawChemicals = []
    for elements in original:
        species = []
        for molecule in elements:
            ## If we have translated it before and its in mem   ory
 #           if molecule in translator:
 #               species.append(translator[molecule])
 #           else:
                output = ''
                tags,molecules = findCorrespondence(original[0],dictionary,molecule,database)
                output = printSpecies(tags,molecules)
                rawChemicals.append((tags,molecules))
                species.append(output)
                translator[molecule] = output
                if tags not in database:
                    database[tags] = tuple(molecules)
        chemicals.append(' + '.join(species) )
   # print rawChemicals
    print ' -> '.join(chemicals)
    
def findCorrespondence(original,dictionary,molecule,database):
    
    species = dictionary[molecule]
    if tuple(species) in database:
        return species,database[species]
    
    extended1 = (copy(dictionary[original[0]]))
    extended2 = (copy(dictionary[original[1]]))
    intersection = findIntersection(extended1,extended2,database)
    
    if not intersection:
        r1 = getFreeRadical(extended1,database)
        r2 = getFreeRadical(extended2,database)
        if not r1 or not r2:
            print 'Cannot infer how',extended1,'binds to',extended2
            sys.exit(1)
        extended1 = list(extended1)
        extended1.extend(extended2)
        rnd = random.randint(0,1000)
        database[tuple(extended1)] = ([(r1,rnd)],[(r2,rnd)])
        intersection = (tuple(extended1),)
        extended1 = []
        extended2 = []
        
    constructed = [[] for element in species]

    for element in [extended1,extended2,intersection[0]]:
        if len(element) > 1:
            for tag,molecule in zip(element,database[element]):
                for element in [x for x in molecule if x not in constructed[species.index(tag)]]:
                    constructed[species.index(tag)].append(tuple(element))
    return species,constructed
    
def getFreeRadical(element,database):
    components = copy(database[element][0])
    for member in database:
        if element[0] in member and len(member) > 1:
            index = member.index(element[0])
            components.remove(database[member][index][0][0])
    
    if not components:
        return []
    return components[0]

def findIntersection(set1,set2,database):
    intersections = []
    for element in [x for x in database.keys() if len(x) > 1]:
        intersection1 = [x for x in element if x in set1]
        intersection2 = [x for x in element if x in set2]
        notInEither = [x for x in element if x not in set1 and x not in set2]
        if len(intersection1)>0 and len(intersection2)>0 and len(notInEither) == 0:
            intersections.append(element)
            
    return intersections
        
def catalysis(original,dictionary,database):
    pass

def defineCorrespondence(reaction2, totalElements,labelDictionary,database):
    for element in totalElements:
        if (element,) in database:
            labelDictionary[element] = (element,)
        else:
            typeOfReaction = identifyReaction(reaction2,element)
            if typeOfReaction == 1:
                if element in reaction2[1]:
                    labelDictionary[element] = tuple([k for k in reaction2[0]])
            else:
                print 'Ive no idea what youre talking about'
    return labelDictionary
                    
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
               
                    
    

if __name__ == "__main__":

#    database = {('S1',):(["r","l"],),("S2",):(["s"],),}    
    #database = {('S1',):("r","l"),("S2",):("s"),('S1','S2'):([('r','1'),('l')],[('s','1')]),('S1','S2','S2'):([('r','1'),('l','2')],[('s','1')],[('s','2')])}
    #database = {('S1',):(["a","b"],),("S2",):(["r"],),('S3',):(['l'],),('S1','S2'):([('a','1')],[('r','1')]),('S1','S3'):([('b','2')],[('l','2')])}
    #database = {('S1',):(["a","b"],),("S2",):(["r"],),('S3',):(['l'],),('S4',):(['t'],),('S1','S2'):([('a','1')],[('r','1')]),('S1','S3'):([('b','2')],[('l','2')]),('S1','S4'):([('c','3')],[('t','3')])}
    database = {('S1',):(["a","b","c"],),("S2",):(["r"],),('S3',):(['l'],),('S4',):(['t'],)}
    
    labelDictionary = {}
    translator = {}
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile('XMLExamples/simple4.xml')
    model = document.getModel()        
    parser = SBML2BNGL(model)
#    print parser.getReactions()
    _,rules,_ = parser.getReactions()
    print rules
    for rule in rules:   
        reaction2 = list(parseReactions(rule))
        totalElements =  [item for sublist in reaction2 for item in sublist]
        totalElements = list(set(totalElements))
        labelDictionary = defineCorrespondence(reaction2,totalElements,labelDictionary,database)
        labelDictionary = resolveCorrespondence(labelDictionary)

    labelDictionary = resolveCorrespondence(labelDictionary)
    for rule in rules:
        reaction2 = list(parseReactions(rule))
        synthesis(reaction2,labelDictionary,database,translator)
#    print translator
    #for rule in rules:
    #    print parseReactions(reaction)
    
        

    