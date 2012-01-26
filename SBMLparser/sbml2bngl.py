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
    
def parseReactions(reaction):
    
    grammar = Group(Word(alphanums) + Suppress('()') + Optional(Suppress('+') + Word(alphanums) 
    + Suppress("()"))) + Suppress("->") + Group(Word(alphanums) + Suppress("()") + Optional(Suppress('+') 
    + Word(alphanums) + Suppress("()"))) + Suppress(Word(alphanums + "()")) 
   
    return grammar.parseString(reaction).asList()

def issubset(possible_sub, superset):
    return all(element in superset for element in possible_sub)

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
            if len(member) == 2:
                components.append('{0}!{1}'.format(member[0],member[1]))
            else:
                components.append('{0}'.format(member[0]))
        molecules.append('{0}({1})'.format(tag,",".join(components)))
    return ".".join(molecules)

def synthesis(original,dictionary,rawDatabase,synthesisDatabase,translator):
    chemicals = []
    rawChemicals = []
    reaction = []
    #print original
    for elements in original:
        species = []
        temp = []
        for molecule in elements:
            ## If we have translated it before and its in mem   ory
 #           if molecule in translator:
 #               species.append(translator[molecule])
 #           else:
                output = ''
                tags,molecules = findCorrespondence(original[0],original[1],dictionary,molecule,rawDatabase,synthesisDatabase)
                output = printSpecies(tags,molecules)
                temp.append((tags,molecules))
                rawChemicals.append((tags,molecules))
                species.append(output)
                translator[molecule] = output
                if tags not in synthesisDatabase and tags not in rawDatabase:
                    synthesisDatabase[tags] = tuple(molecules)
        chemicals.append(' + '.join(species) )
        reaction.append(temp)
   # print rawChemicals
    print ' -> '.join(chemicals)
    return reaction


#this method reconstructs the component structure for a sbml molecule
#using context and history information    
def findCorrespondence(reactants,products,dictionary,molecule,rawDatabase,synthesisDatabase):
    
    species = dictionary[molecule]
    product = dictionary[products[0]]
    if species in synthesisDatabase:
        return species,synthesisDatabase[species]
    elif species in rawDatabase:
        components = ()
        if product in synthesisDatabase:
            index = product.index(species[0])
            components = synthesisDatabase[product][index]
            temp = ([x[0] for x in components],)
            return species,temp
        else:
            r1 = getFreeRadical(species,rawDatabase,synthesisDatabase,product)
            return species,r1
        return species,rawDatabase[species] 
    
    extended1 = (copy(dictionary[reactants[0]]))
    extended2 = (copy(dictionary[reactants[1]]))
    intersection = findIntersection(extended1,extended2,synthesisDatabase)
    
    if not intersection:
        r1 = getFreeRadical(extended1,rawDatabase,synthesisDatabase,product)
        r2 = getFreeRadical(extended2,rawDatabase,synthesisDatabase,product)
        if not r1 or not r2:
            print 'Cannot infer how',extended1,'binds to',extended2
            sys.exit(1)
        extended1 = list(extended1)
        extended1.extend(extended2)
        rnd = random.randint(0,1000)
        synthesisDatabase[tuple(extended1)] = ([(r1,rnd)],[(r2,rnd)])
        intersection = (tuple(extended1),)
        extended1 = []
        extended2 = []
        
    constructed = [[] for element in species]
    for element in [extended1,extended2,intersection[0]]:
        if len(element) > 1:
            for tag,molecule in zip(element,synthesisDatabase[element]):
                for element in [x for x in molecule if x not in constructed[species.index(tag)]]:
                    for member in constructed[species.index(tag)]:
                        if element[0] in member:
                            print 'INVALID REACTION, may cause errors'
                    constructed[species.index(tag)].append(tuple(element))
    return species,constructed

##this method is used when the user does not provide a full binding specification
##it searches a molecule for a component that has not been used before in another reaction
# in case it cannot find one it returns an empty list
def getFreeRadical(element,rawDatabase,synthesisDatabase,product):
    components = copy(rawDatabase[element][0])
    for member in synthesisDatabase:
        ##the last condition issubset is in there because knowing that S1 + s2 -> s1.s2 
        #does not preclude s2 from using the same bnding site in s1.s3 + s2 -> s1.s2.s3
        if element[0] in member and len(member) > 1 and not issubset(member,product):
            index = member.index(element[0])
            components.remove(synthesisDatabase[member][index][0][0])
    
    if not components:
        return []
    return components[0]

##this method receives a list of components and returns an element from the database
##that is a subset of the union of both sets
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

##this method goes through the list of reactions and determines what kind of 
#reaction each one is (eg. catalysis, synthesis etc). It also fills the database
#with information about what each sbml molecule is equal to in bngl lingo._
#eg S1 + s2 -> s3 would return s3 == s1.s2
def defineCorrespondence(reaction2, totalElements,labelDictionary,rawDatabase, synthesisDatabase):
    for element in totalElements:
        if (element,) in rawDatabase:
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
    rawDatabase = {('S1',):(["a","b","c"],),("S2",):(["r"],),('S3',):(['l'],),('S4',):(['t'],)}   
    synthesisdatabase = {('S1','S2'):([('a','1')],[('r','1')])}
    history = []
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
        labelDictionary = defineCorrespondence(reaction2,totalElements,labelDictionary,rawDatabase,synthesisdatabase)
        labelDictionary = resolveCorrespondence(labelDictionary)

    labelDictionary = resolveCorrespondence(labelDictionary)
    for rule in rules:
        reaction2 = list(parseReactions(rule))
        reaction = synthesis(reaction2,labelDictionary,rawDatabase,synthesisdatabase,translator)
        history.append(reaction)
    #print history
#    print translator
    #for rule in rules:
    #    print parseReactions(reaction)
    
        

    
