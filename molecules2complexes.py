# -*- coding: utf-8 -*-
"""
Created on Tue Dec  6 17:42:31 2011

@author: proto
"""
from pyparsing import Word, Suppress, Optional, alphanums, Group
from numpy import sort
from copy import deepcopy
import reactionTransformations
import analyzeSBML
import analyzeRDF
from util import logMess
import re

def parseReactions(reaction):
    species =   (Word(alphanums+"_:#") 
    + Suppress('()')) + Optional(Suppress('+') + Word(alphanums+"_:#") 
    + Suppress("()")) + Optional(Suppress('+') + Word(alphanums+"_") 
    + Suppress("()")) + Optional(Suppress('+') + Word(alphanums+"_") 
    + Suppress("()"))
    '''
    species = Optional(Suppress(Word(nums+"*"))) +  Optional(Word(alphanums+"_") + Suppress('()')) +  \
    Optional(Suppress('+') + Word(alphanums+"_") + Suppress("()")) + \
    Optional(Suppress('+') + Word(alphanums+"_") + Suppress("()")) + Optional(Suppress('+') + Word(alphanums+"_") 
    + Suppress("()"))
    '''
    rate = Word(alphanums + "()")
    grammar = ((Group(species) | '0') + Suppress(Optional("<") + "->") + (Group(species) | '0') + Suppress(rate)) \
    ^ (species + Suppress(Optional("<") + "->") + Suppress(rate))  
    result =  grammar.parseString(reaction).asList()
    if len(result) < 2:
        result = [result,[]]
    if '<->' in reaction and len(result[0]) == 1 and len(result[1]) == 2:
        result2 = [result[1],result[0]]
        result = result2
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
       
def correctClassificationsWithCycleInformation(rules,classifications,cycles):
    for index  in range(0,len(rules)):
        reaction2 = list(parseReactions(rules[index]))
        if (len([x for x in reaction2[0] if x in [y for x in cycles for y in x]]) > 0 and
            len([x for x in reaction2[1] if x in [y for x in cycles for y in x]]) > 0):
                classifications[index] = 'Generic-Catalysis'
                

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
        elif classification == 'Binding':
        #and (element not in database.labelDictionary or (len(database.labelDictionary[element]) < 2)):
                if len(reaction2[0]) == 2 and element not in reaction2[0]:
                    addToLabelDictionary(database.labelDictionary,element,tuple([k for k in reaction2[0]]))
                    #database.labelDictionary[element] = tuple([k for k in reaction2[0]])
                elif len(reaction2[1]) == 2 and element not in reaction2[1]:
                    addToLabelDictionary(database.labelDictionary,element,tuple([k for k in reaction2[1]]))
                    #database.labelDictionary[element] = tuple([k for k in reaction2[1]])
                else:
                    pass
                    addToLabelDictionary(database.labelDictionary,element,(element,))
                    #database.labelDictionary[element] = (element,)
        elif classification == 'Phosporylation' or classification == 'Double-Phosporylation':
            #print element
            addToLabelDictionary(database.labelDictionary,element,(element,))
            #pass
        else:
            #use the naming conventions
            #use information from the RDF database
            #equivalence = analyzeRDF.getEquivalence(element,rdfAnnotations)
            equivalence = []
            if equivalence != []:
                for equivalentElement in equivalence:
                    if equivalentElement in database.labelDictionary:
                        addToLabelDictionary(database.labelDictionary,element,(equivalentElement,))
                        #database.labelDictionary[element] = (equivalentElement,)
            else:
                addToLabelDictionary(database.labelDictionary,element,(element,))
    return database.labelDictionary


def defineComplexCorrespondenceWithNamingConventions(reactant,namingConventions,database,createOnDemand=False):
    if isinstance(namingConventions[0][1],tuple):
        #if len([x for x in namingConventions[0][1] if x not in database.labelDictionary]) == 0:
        if namingConventions[0][0] in database.labelDictionary:
            a1 = (database.labelDictionary[namingConventions[0][0]][0])
            a2 = (namingConventions[0][1])
            evaluation = len([x for x in a1 if x not in a2])+len([x for x in a2 if x not in a1])
            if  evaluation != 0:
                #if there's already something in there
                if a1[0] != namingConventions[0][0]:
                    if not createOnDemand:
                        pass
                        logMess("ERROR","{0}: {1} =/= {2}".format(namingConventions[0][0],[x for x in a1 if x not in a2],[x for x in a2 if x not in a1]))
                        #print 'CONTRADICTORY ',namingConventions[0][0],'(',[x for x in a1 if x not in a2],[x for x in a2 if x not in a1],[x in database.labelDictionary for x in a2 if x not in a1],')'
                    if createOnDemand and len([x for x in a1 if x not in a2])<len([x for x in a2 if x not in a1]):
                        database.labelDictionary[namingConventions[0][0]] =  [a2]
                        print 'CONTRADICTORY ',namingConventions[0][0],'(',[x for x in a1 if x not in a2],[x for x in a2 if x not in a1],[x in database.labelDictionary for x in a2 if x not in a1],')'
                        for element in [x for x in a2 if x not in database.labelDictionary]:
                            database.labelDictionary[element] = [(element,)]
                #if we already know about all the elements in there

                elif len([x for x in a2 if x not in namingConventions]) == 0:
                    print '++++',a1,a2,namingConventions[0][0]
                #database.labelDictionary[namingConventions[0][0]].append(namingConventions[0][1])
        #for element in [x for x in namingConventions[0][1] if x not in database.labelDictionary]:
        #    database.labelDictionary[element] = [(element,)]
            
        #if namingConventions[0][0] in database.labelDictionary:
        #     print namingConventions[0][0],[namingConventions[0][1]],database.labelDictionary[namingConventions[0][0]],namingConventions[0][1]==database.labelDictionary[namingConventions[0][0]]
        return
    
def defineCorrespondenceWithNamingConventions(reactant,namingConventions,database,createOnDemand=False):
    #FIXME: this is just a hack for where there are many naming conventions. Fix
    if isinstance(namingConventions[0][1],tuple):
        return
    if reactant == max(namingConventions[0],key=len):
        if database.labelDictionary[ max(namingConventions[0],key=len)][0] ==  (max(namingConventions[0],key=len),):
             database.labelDictionary[ max(namingConventions[0],key=len)] = [(min(namingConventions[0],key=len),)]
        else:
            addToLabelDictionary(database.labelDictionary, max(namingConventions[0],key=len), (min(namingConventions[0],key=len),))

def defineCorrespondenceWithAnnotations(reactant,annotations,database):
    if database.labelDictionary[reactant][0] == (reactant,):
        tmp = [x for x in annotations[0] if x in database.labelDictionary]
        database.labelDictionary[reactant] = [(min(tmp,key=len),)]
    
##TODO: i introduced arrays in the labelDictionary, have to resolve correspondences

def resolveCycles(database,equivalenceTranslator):
    #print database.labelDictionary
    cycleCandidates = {}
    cycleMembers = []
    cycles = []
    #detect all those elements that are involved in a reaction cycle
    for element in database.labelDictionary:
        temp = []
        #print element,
        recursiveChecking(database.labelDictionary,element,temp)
        if 'CYCLEPROBLEM' in temp:
            temp.remove('CYCLEPROBLEM')
            cycleCandidates[element] = temp
    #detect the mebers that are crucial to the cycle
    for element in cycleCandidates:
        if cycleCandidates[element].count(element) >1:
            cycleMembers.append(element)
    #take away elements that are not strictly part of the cycles
    updatedCycleCandidates = {x:[y for y in set(cycleCandidates[x]) 
        if y in cycleMembers] for x in cycleCandidates if x in cycleMembers}  
    #get the minimum cycles
    for element in updatedCycleCandidates:
        for counterElement in updatedCycleCandidates:
            if element == counterElement:
                pass
            intersection = [x for x in updatedCycleCandidates[element] 
                if x in updatedCycleCandidates[counterElement]]
            if len(intersection) > 2 and intersection not in cycles:
                cycles.append(intersection)
    ##fill in the equivalence translator thing      
    for cycle in cycles:
        shortestTerm = min(cycle,key=len)
        for element in [x for x in cycle if x != shortestTerm]:
            if tuple([shortestTerm,element]) not in equivalenceTranslator:
                equivalenceTranslator.append(tuple([shortestTerm,element]))
    return cycles
        
        

def recursiveChecking(labelDictionary,root,acc,):
    if root in acc:
        acc.extend(['CYCLEPROBLEM',root])
    elif root in labelDictionary[root]:
        acc.append(root)
    else:
        #temp = deepcopy(acc)
        acc.append(root)
        for element in labelDictionary[root]:
            #print 'bbbbbbbbbbbb',element,acc
            recursiveChecking(labelDictionary,element,acc)
        
        return acc
            
            
def resolveCorrespondence(database,cycles):
    
    temp = database.labelDictionary.copy()
    #print temp
    #print database.labelDictionary
    for element in [x for x in database.labelDictionary if x not in [j for i in cycles for j in i]]:
       # for idx,alternative in enumerate(database.labelDictionary[element]):
        history = [element]
        history.extend(database.labelDictionary[element])
        for member in [x for x in database.labelDictionary[element]]:
            tmpLabel= member
            #if member in history:
            #        continue
            #history.append(member)
            backup = deepcopy(temp[element])
            counter = 0
            while True:
                counter +=1
                oldTemp = deepcopy(temp[element])
                #TODO: I dont remember what was this for, probably for avoiding cycles
                #if len([x for x in database.labelDictionary[tmpLabel] if x in history]) > 0:
                #    temp[element] = backup
                temp[element] = list(temp[element])
                
               # print member,counter,history,[x for x in database.labelDictionary[tmpLabel]]
                if tmpLabel in [j for i in cycles for j in i]:
                    break
                temp[element].extend(database.labelDictionary[tmpLabel])
                temp[element].remove(tmpLabel)
                temp[element] = tuple(sort(temp[element]))
                history.append(tmpLabel)
                
                tmpLabel = database.labelDictionary[tmpLabel][0]
                #print oldTemp,temp[element],history
                #print database.labelDictionary[tmpLabel]
                if oldTemp == temp[element]:
                    break
    
    #for element in cycles:
    #    temp[element] = database.labelDictionary[element]          
        #temp[element] = list(set(temp[element]))
    labelDictionary = temp.copy()
    #print labelDictionary
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

def getEquivalenceList(reactant,equivalenceTranslator):
    equivalenceList = []
    for element in equivalenceTranslator:
        if reactant in element:
            equivalenceList.append(element)
    return equivalenceList

def getPertinentNamingEquivalence3(original,labelDatabase, equivalenceTranslator):
    """
    This function receives
    """
    overallEquivalence = []
    for element in original:
        equivalenceList = []
        for reactant in element:
            equivalenceList.extend(getEquivalenceList(reactant,equivalenceTranslator))
        overallEquivalence.append(equivalenceList)
    intersection = [x for x in overallEquivalence[0] if x in overallEquivalence[1]]
    overallEquivalence = []
    
    if intersection == []:
        for element in original:
            equivalenceList = []
            for reactant in element:
                for subreactant in labelDatabase[reactant]:
                    equivalenceList.extend(getEquivalenceList(subreactant,equivalenceTranslator))
            overallEquivalence.append(equivalenceList)
        intersection = [x for x in overallEquivalence[0] if x in overallEquivalence[1]]
    #TODO: we still need some additional checking
    return intersection[0]
def processRule(original,database,
                classification,equivalenceTranslator,outputFlag = False):
    '''
    '''
    #print (identifyReaction(original,0))
    if identifyReaction(original,0) == 1 and classification == 'Binding':
        return reactionTransformations.synthesis(original,database.labelDictionary,
        database.rawDatabase,database.synthesisDatabase,database.translator,outputFlag)
    elif classification in ['Phosporylation','Double-Phosporylation','Generic-Catalysis','Modification','mMod','iMod','modI']:
        equ = equivalenceTranslator[classification]
        pertinentEquivalence = getPertinentNamingEquivalence3(original,database.rawLabelDictionary,equ)
        return reactionTransformations.catalysis(original,database.labelDictionary,database.rawDatabase,
                                                  None,
                                                  database.translator,pertinentEquivalence,
                                                  classification,database.reactionProperties)

    
    elif identifyReaction(original,0) == 4:
        #return reactionTransformations.creation(original,dictionary,
        #rawDatabase,translator)
        return ''
    elif identifyReaction(original,0) == 5: 
        #return reactionTransformations.decay(original,dictionary,
        #rawDatabase,translator)
        return ''
    else:
        #print 'none',original
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
def correctClassifications(rules,classifications,labelDatabase,equivalenceTranslator):
    for idx in range(0,len(rules)):
        #if classifications[idx] == 'Binding':
        reaction2 = list(parseReactions(rules[idx]))
        #if len(labelDatabase[reaction2[1][0]]) >1 and min(labelDatabase[reaction2[1][0]],key=len)[0] != reaction2[1][0]:
        
        #if we found that a binding is actually a catalysis
        classifications[idx] = 'Generic-Catalysis'
        # we will assume that they smallest element
        #print reaction2
        #print labelDatabase[reaction2[1][0]]
        labelDatabase[reaction2[1][0]] = [min(labelDatabase[reaction2[1][0]],key=len)]
                
            
            
def transformMolecules(parser,database,configurationFile,speciesEquivalences=None):
    #labelDictionary = {}
    _,rules,_ = parser.getReactions()
    molecules,_,_ = parser.getSpecies()
    #synthesisdatabase = {}
    #translator = {}
    sbmlAnalyzer =analyzeSBML.SBMLAnalyzer(configurationFile,speciesEquivalences)
    classifications,equivalenceTranslator,eequivalenceTranslator = sbmlAnalyzer.classifyReactions(rules,molecules)
    database.reactionProperties = sbmlAnalyzer.getReactionProperties()
    
    database.translator,database.labelDictionary = sbmlAnalyzer.getUserDefinedComplexes()
    
    #analyzeSBML.analyzeNamingConventions(molecules)
    rdfAnnotations = analyzeRDF.getAnnotations(parser,'uniprot')
    #print rdfAnnotations
    #print classifications
    
    #for element in equivalenceTranslator:
    #    addToLabelDictionary(database.labelDictionary,element[0],min(element,key=len))
    #    addToLabelDictionary(database.labelDictionary,element[1],min(element,key=len))

    #for element in database.labelDictionary:
    #    database.labelDictionary[element] = [(min(database.labelDictionary[element],key=len),)] 
    
    #STEP1: Use reaction information to infer w print zip(rules,classifications)
    
    for rule,classification in zip(rules,classifications): 
        reaction2 = list(parseReactions(rule))
        totalElements =  [item for sublist in reaction2 for item in sublist]
        totalElements = list(set(totalElements))
        #obtain elements from the equivalenceTranstor that contain at least two
        #reactants/products
        
        #equivalences = [x for x in equivalenceTranslator if x[0] in reaction2[0] or x[1] in reaction2[1]]
        database.labelDictionary = defineCorrespondence(reaction2,totalElements,
                                               database,classification,
                                               rdfAnnotations)
        #if 'ERKi_MEKi_PP' in database.labelDictionary:
        #    print database.labelDictionary['ERKi_MEKi_PP'],rule
        #database.labelDictionary = resolveCorrespondence(database)
    #correctClassifications(rules,classifications,database.labelDictionary)
    #print 'step1',database.labelDictionary

    simplify(database.labelDictionary)
    
    #TODO: uncomment this section when we solve the bug on reclassifying
    #print database.labelDictionary 
    cycles = resolveCycles(database,equivalenceTranslator)
    database.rawLabelDictionary = deepcopy(database.labelDictionary)
    
    for _ in range(0,5):
        database.labelDictionary = resolveCorrespondence(database,cycles)
    #print 'after resolving correspondences'
    classifications2,_,eequivalenceTranslator = sbmlAnalyzer.reclassifyReactions(rules,molecules,database.labelDictionary)
    for index in range(0,len(classifications)):
        if classifications[index] in ['None','Binding'] and classifications2[index] != 'None':
            classifications[index] = classifications2[index]
    tmp = {}
    
    tmp = {x:[database.labelDictionary[x]] for x in database.labelDictionary}
    database.labelDictionary = tmp
    
    #print 'step1.5',database.labelDictionary    
    #STEP2: Use naming conventions
    
    for element in set(x for rule in rules for tmp in parseReactions(rule) for x in tmp):
        equivalence = [x for x in equivalenceTranslator if element == max(x,key=len)]
        if equivalence != []:
            defineCorrespondenceWithNamingConventions(element,equivalence,database,createOnDemand=True)
            
    #for _ in range(0,5):
    #    database.labelDictionary = resolveCorrespondence(database,cycles)


    for element in set(x for rule in rules for tmp in parseReactions(rule) for x in tmp):
        equivalence = [x for x in equivalenceTranslator if element == max(x,key=len)]
        if equivalence != []:
            defineComplexCorrespondenceWithNamingConventions(element,equivalence,database,createOnDemand=False)

    #print 'step2',database.labelDictionary    
    #STEP2.5: For those elements that where updated from the naming conventions
    #we also use the chance to update the reaction classification if necessary
    
#    correctClassifications(rules,classifications,database.labelDictionary)
    #STEP3: Use annotations
    sbmlAnalyzer.classifyReactionsWithAnnotations(rules,molecules,rdfAnnotations,database.labelDictionary)
    #for element in set(x for rule in rules for tmp in parseReactions(rule) for x in tmp):
    #    annotation = [rdfAnnotations[x] for x in rdfAnnotations if element in rdfAnnotations[x]]
    #    if annotation != []:
    #        defineCorrespondenceWithAnnotations(element,annotation,database)
    simplify(database.labelDictionary)

    #print 'step3',database.labelDictionary    
    #analyzeSBML.reclassifyReactions(reactions,molecules,labelDictionary,classifications,equivalenceTranslator)
    cycles = resolveCycles(database,equivalenceTranslator)

    #TODO: this is causing errors, check
    for _ in range(0,5):
        database.labelDictionary = resolveCorrespondence(database,cycles)
         
    correctClassificationsWithCycleInformation(rules,classifications,cycles)
    #print database.labelDictionary
    counter = 0
    nonProcessedRules = zip(rules,classifications)
    #TODO:multipass stuff.
    #while len(nonProcessedRules) > 0:
    #tmp = []
    
    ##sort. Primary key: number of predependencies a rule has
    #secondary key: length of the names of the elements involved    
    ruleWeightTable = []
    ruleWeight2Table= []
    for rule in rules:
        flag = False
        weight = 0
        weight2 = 0
        reaction2 = list(parseReactions(rule))
        #sort rules according to the complexity of the reactants (not the products)
        for element in reaction2[0]:
            if element not in database.labelDictionary:
                weight =50
            else:
                weight = max(weight,len(database.labelDictionary[element]))
            weight2 += len(element)
        for element in reaction2[0]:
            weight+= sum([1 for x in equivalenceTranslator if re.search(r'(_|^)({0})(_|$)'.format(x[1]),element) != None])
        ruleWeight2Table.append(weight2)
        ruleWeightTable.append(weight)
    nonProcessedRules = zip(ruleWeightTable,ruleWeight2Table,rules,classifications)
    nonProcessedRules = sorted(nonProcessedRules,key=lambda rule: rule[1])
    nonProcessedRules = sorted(nonProcessedRules,key=lambda rule: rule[0])
    
        
    for idx,(w0,w1,rule,classification) in enumerate(nonProcessedRules):
        outputFlag = False
        #if classification == 'Modification':
        #    outputFlag = True
        counter += 1
        reaction2 = list(parseReactions(rule))
        if outputFlag:
            tmp = deepcopy(database.translator)
            print reaction2
        processRule(reaction2,database,classification,eequivalenceTranslator,outputFlag)
        #if 'EGF_EGFRm2_GAP_Grb2_Prot' in database.translator:
        #    print '++++',rule,difflib.SequenceMatcher(None, 'Grb2(egfr,shc!10,sos).EGF(egfr!5,modI~U,modM~M).EGFR(egf!5,egfr!8,gap!9,grb2!11,modI~U,prot,ras_gdp,shc!10).EGF(egfr!7,modI~U,modM~U).EGFR(egf!7,egfr!8,gap!8,grb2,modI~U,prot,ras_gdp,shc!9).GAP(egfr!9).Prot(egfr!11,modI~U,ras_gdp,ras_gtp)' , str(database.translator['EGF_EGFRm2_GAP_Grb2_Prot'])).ratio()
        if outputFlag:
            print {x:str(database.translator[x]) for x in database.translator if x not in tmp}
        
    for element in database.labelDictionary:
        if not isinstance(database.labelDictionary[element],tuple):
            database.translator[element] = database.translator[database.labelDictionary[element]]
    raw =  [x[0] for x in database.rawDatabase]

    for element in database.translator:
        if element in raw:
            continue
        for mol in database.translator[element].molecules:
            if mol.name in database.translator:
                mol.update(database.translator[mol.name].molecules[0])
    return database.translator,logMess.log

