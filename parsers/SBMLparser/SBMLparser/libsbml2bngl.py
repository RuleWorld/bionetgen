# -*- coding: utf-8 -*-
"""
Created on Fri Mar  1 16:14:42 2013

@author: proto
"""

#!/usr/bin/env python
from collections import OrderedDict
import time
#import matplotlib.pyplot as plt5
import libsbml
import writer.bnglWriter as writer
from optparse import OptionParser
import atomizer.moleculeCreation as mc
import sys
from os import listdir
import numpy as np
import re
import pickle
from copy import copy
log = {'species': [], 'reactions': []}
import signal
from collections import Counter

import utils.structures as structures
import atomizer.analyzeRDF
from utils.util import logMess
from sbml2bngl import SBML2BNGL
from biogrid import loadBioGridDict as loadBioGrid

def handler(signum, frame):
    print "Forever is over!"
    raise Exception("end of time")


import os.path        
def resource_path(relative_path):
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")

    return os.path.join(base_path, relative_path)
    
    


def evaluation(numMolecules,translator):
    originalElements = (numMolecules)
    nonStructuredElements = len([1 for x in translator if '()' in str(translator[x])])
    if originalElements > 0:
        ruleElements = (len(translator) - nonStructuredElements)*1.0/originalElements
        if ruleElements> 1:
            ruleElements = (len(translator) - nonStructuredElements)*1.0/len(translator.keys())
        
    else:
        ruleElements= 0
    return ruleElements


    #print rules
#14,18,56,19,49.87.88.107,109,111,120,139,140,145,151,153,171,175,182,202,205
#230,253,255,256,268,269,288,313,332,333,334,335,336,362,396,397,399,406


def selectReactionDefinitions(bioNumber):
    '''
    This method rrough the stats-biomodels database looking for the 
    best reactionDefinitions definition available
    '''
    #with open('stats4.npy') as f:
    #    db = pickle.load(f)
    fileName = resource_path('config/reactionDefinitions.json')
    useID = True
    naming = resource_path('config/namingConventions.json')
    '''
    for element in db:    
        if element[0] == bioNumber and element[1] != '0':
            fileName = 'reactionDefinitions/reactionDefinition' + element[1] + '.json'
            useID = element[5]
        elif element[0] > bioNumber:
            break
    '''
    return fileName,useID,naming


def resolveDependencies(dictionary,key,idx):
    counter = 0
    for element in dictionary[key]:
        if idx < 20:
            counter += resolveDependencies(dictionary,element,idx+1)
        else:
            counter += 1
    return len(dictionary[key]) + counter    
    
def validateReactionUsage(reactant,reactions):
    for element in reactions:
        if reactant in element:
            return element
    return None


def readFromString(inputString,reactionDefinitions,useID,speciesEquivalence=None,atomize=False):
    '''
    one of the library's main entry methods. Process data from a string
    '''
    try:
        reader = libsbml.SBMLReader()
        document = reader.readSBMLFromString(inputString)
        parser =SBML2BNGL(document.getModel(),useID)
        
        bioGrid = False
        if bioGrid:
            loadBioGrid()
        database = structures.Databases()
        database.forceModificationFlag = True

        namingConventions = resource_path('config/namingConventions.json')
        
        if atomize:
            translator,onlySynDec = mc.transformMolecules(parser,database,reactionDefinitions,namingConventions,speciesEquivalence,bioGrid)
        else:    
            translator={} 
        
        return analyzeHelper(document,reactionDefinitions,
                             useID,'',speciesEquivalence,atomize,translator)[-2]
    except:
        return -5
def processFunctions(functions,sbmlfunctions,artificialObservables,tfunc):
    '''
    this method goes through the list of functions and removes all
    sbml elements that are extraneous to bngl
    '''
    
    for idx in range(0,len(functions)):
        for sbml in sbmlfunctions:
            if sbml in functions[idx]:
                functions[idx] = writer.extendFunction(functions[idx],sbml,sbmlfunctions[sbml])
        functions[idx] =re.sub(r'(\W|^)(time)(\W|$)',r'\1time()\3',functions[idx])
        functions[idx] =re.sub(r'(\W|^)(Time)(\W|$)',r'\1time()\3',functions[idx])
        functions[idx] =re.sub(r'(\W|^)(t)(\W|$)',r'\1time()\3',functions[idx])
    #functions.extend(sbmlfunctions)
    dependencies2 = {}
    for idx in range(0,len(functions)):
        dependencies2[functions[idx].split(' = ')[0].split('(')[0].strip()] = []
        for key in artificialObservables:
            oldfunc = functions[idx]
            functions[idx] = (re.sub(r'(\W|^)({0})([^\w(]|$)'.format(key),r'\1\2()\3',functions[idx]))
            if oldfunc != functions[idx]:
                dependencies2[functions[idx].split(' = ')[0].split('(')[0]].append(key)
        for element in sbmlfunctions:
            oldfunc = functions[idx]
            key = element.split(' = ')[0].split('(')[0]
            if re.search('(\W|^){0}(\W|$)'.format(key),functions[idx].split(' = ')[1]) != None:
                dependencies2[functions[idx].split(' = ')[0].split('(')[0]].append(key)
        for element in tfunc:
            key = element.split(' = ')[0].split('(')[0]
            if key in functions[idx].split(' = ')[1]:
                dependencies2[functions[idx].split( ' = ')[0].split('(')[0]].append(key)
    '''           
    for counter in range(0,3):
        for element in dependencies2:
            if len(dependencies2[element]) > counter:
                dependencies2[element].extend(dependencies2[dependencies2[element][counter]])
    '''
    fd = []
    for function in functions:
        #print function,dependencies2[function.split(' = ' )[0].split('(')[0]],function.split(' = ' )[0].split('(')[0],0
        fd.append([function,resolveDependencies(dependencies2,function.split(' = ' )[0].split('(')[0],0)])
    fd = sorted(fd,key= lambda rule:rule[1])
    functions = [x[0] for x in fd]
    return functions


def extractAtoms(species):
    '''
    given a list of structures, returns a list
    of individual molecules/compartment pairs
    appends a number for 
    '''
    listOfAtoms = set()
    for molecule in species.molecules:
        for component in molecule.components:
            listOfAtoms.add(tuple([molecule.name,component.name]))
    return listOfAtoms


def bondPartners(species,bondNumber):
    relevantComponents = []
    for molecule in species.molecules:
        for component in molecule.components:
            if bondNumber in component.bonds:
                relevantComponents.append(tuple([molecule.name,component.name]))
    return relevantComponents
    
def getMoleculeByName(species,atom):
    '''
    returns the state of molecule-component contained in atom
    '''
    
    stateVectorVector = []
    for molecule in species.molecules:
        if molecule.name == atom[0]:
            stateVector = []
            for component in molecule.components:
                if component.name == atom[1]:
                    
                    #get whatever species this atom is bound to
                    if len(component.bonds) > 0:
                        comp = bondPartners(species,component.bonds[0])
                        comp.remove(atom)
                        if len(comp) > 0:
                            stateVector.append(comp[0])
                        else:
                            stateVector.append('')
                    else:
                        stateVector.append('')
                    if len(component.states) > 0:
                        stateVector.append(component.activeState)
                    else:
                        stateVector.append('')
            stateVectorVector.append(stateVector)
    return tuple(stateVectorVector[0])
        
    
    
def extractCompartmentCoIncidence(species):
    atomPairDictionary = {}
    if [x.name for x in species.molecules] == ['EGF','EGF','EGFR','EGFR']:
        pass
    for molecule in species.molecules:
        for component in molecule.components:
            for component2 in molecule.components:
                if component == component2:
                    continue
                atom = tuple([molecule.name,component.name])
                atom2 = tuple([molecule.name,component2.name])
                molId1 = getMoleculeByName(species,atom)
                molId2 = getMoleculeByName(species,atom2)
                key = tuple([atom,atom2])
                #print key,(molId1,molId2)
                if key not in atomPairDictionary:
                    atomPairDictionary[key] = Counter()
                atomPairDictionary[key].update([tuple([molId1,molId2])])

    return atomPairDictionary    
    
def extractCompartmentStatistics(bioNumber,useID,reactionDefinitions,speciesEquivalence):
    '''
    Iterate over the translated species and check which compartments
    are used together, and how. 
    '''
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile(bioNumber)
    
    
    parser =SBML2BNGL(document.getModel(),useID)
    database = structures.Databases()
    
    #call the atomizer (or not)
    #if atomize:
    translator,onlySynDec = mc.transformMolecules(parser,database,reactionDefinitions,speciesEquivalence)
    #else:    
    #    translator={} 


    compartmentPairs = {}
    for element in translator:
        temp = extractCompartmentCoIncidence(translator[element])
        for element in temp:
            if element not in compartmentPairs:
                compartmentPairs[element] = temp[element]
            else:
                compartmentPairs[element].update(temp[element])
    finalCompartmentPairs = {}
    print '-----'
    for element in compartmentPairs:
        if element[0][0] not in finalCompartmentPairs:
            finalCompartmentPairs[element[0][0]] = {}
        finalCompartmentPairs[element[0][0]][tuple([element[0][1],element[1][1]])] = compartmentPairs[element]
    return finalCompartmentPairs
    
def recursiveSearch(dictionary,element,visitedFunctions=[]):
    tmp = 0
    for item in dictionary[element]:
        if dictionary[item] == []:
            tmp +=1
        else:
            if item in visitedFunctions:
                raise Exception
            tmp += 1
            tmp += (recursiveSearch(dictionary,item,[item] + visitedFunctions))
    return tmp

def reorderFunctions(functions):
    ''''
    Analyze a list of sbml functions and make sure there are no forward dependencies. 
    Reorder if necessary
    '''    
    functionNames = []
    tmp = []
    for function in functions:
        m = re.split('(?<=\()[\w)]', function)
        functionNames.append(m[0])
    functionNamesDict = {x:[] for x in functionNames}
    for idx,function in enumerate(functions):
        tmp = [x for x in functionNames if x in function.split('=')[1] and x!= functionNames[idx]]
        functionNamesDict[functionNames[idx]].extend(tmp)
    newFunctionNamesDict = {}
    for name in functionNamesDict:
        newFunctionNamesDict[name] = recursiveSearch(functionNamesDict,name,[])
    functionWeightsDict = {x:newFunctionNamesDict[x] for x in newFunctionNamesDict}
    functionWeights = []
    for name in functionNames:
        functionWeights.append(functionWeightsDict[name])
    tmp = zip(functions,functionWeights)
    idx = sorted(tmp,key=lambda x:x[1])
    return [x[0] for x in idx]
    
    
def analyzeFile(bioNumber,reactionDefinitions,useID,namingConventions,outputFile,
                speciesEquivalence=None,atomize=False,bioGrid=False):
    '''
    one of the library's main entry methods. Process data from a file
    '''
    logMess.log = []
    logMess.counter = -1
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile(bioNumber)
    
    parser =SBML2BNGL(document.getModel(),useID)
    database = structures.Databases()
    database.forceModificationFlag = True
    
    bioGridDict = {}
    if bioGrid:
        bioGridDict = loadBioGrid()
    
    #call the atomizer (or not). structured molecules are contained in translator
    #onlysyndec is a boolean saying if a model is just synthesis of decay reactions
    if atomize:
        translator,onlySynDec = mc.transformMolecules(parser,database,reactionDefinitions,namingConventions,speciesEquivalence,bioGrid)
    else:    
        translator={} 

    #process other sections of the sbml file (functions reactions etc.)    
    returnArray= analyzeHelper(document,reactionDefinitions,useID,outputFile,speciesEquivalence,atomize,translator)
    
    with open(outputFile,'w') as f:
        f.write(returnArray[-2])
    #with open('{0}.dict'.format(outputFile),'wb') as f:
    #    pickle.dump(returnArray[-1],f)
    if atomize and onlySynDec:
        returnArray = list(returnArray)
        returnArray[0] = -1
    return tuple(returnArray[0:-2])

def correctRulesWithParenthesis(rules,parameters):
    '''
    helper function. Goes through a list of rules and adds a parenthesis
    to the reaction rates of those functions whose rate is in list 
    'parameters'
    '''
    for idx in range(len(rules)):
        tmp = [x for x in parameters if x + ' ' in rules[idx]]
        if len(tmp) > 0:
            rules[idx].strip()
            rules[idx] += '()'
    
def changeNames(functions,dictionary):
    '''
    changes instances of keys in dictionary appeareing in functions to their corresponding
    alternatives
    '''
    tmpArray = []
    for function in functions:
        tmp = function.split(' = ')
        #hack to avoid problems with less than equal or more than equal
        #in equations
        tmp = [tmp[0],''.join(tmp[1:])] 
        for key in [x for x in dictionary if x in tmp[1]]:
            tmp[1] = re.sub(r'(\W|^){0}(\W|$)'.format(key),r'\1{0}\2'.format(dictionary[key]),tmp[1])
        tmpArray.append('{0} = {1}'.format(tmp[0],tmp[1]))
    return tmpArray
    
def changeRates(reactions,dictionary):
    tmpArray = []
    tmp = None
    for reaction in reactions:
        tmp = reaction.strip().split(' ')
        for key in [x for x in dictionary if x in tmp[-1]]:
            tmp[-1] = re.sub(r'(\W|^){0}(\W|$)'.format(key),r'\1{0}\2'.format(dictionary[key]),tmp[-1])
        tmpArray.append(' '.join(tmp))
    if tmp:
        tmpArray.append(' '.join(tmp))
    return tmpArray
    
def unrollFunctions(functions):
    flag = True
    #bngl doesnt accept nested function calling
    while(flag):
        dictionary = OrderedDict()
        flag = False
        for function in functions:
            tmp = function.split(' = ')
            for key in dictionary:
                if key in tmp[1]:
                    tmp[1] = re.sub(r'(\W|^){0}\(\)(\W|$)'.format(key),r'\1({0})\2'.format(dictionary[key]),tmp[1])
                    flag = False
            dictionary[tmp[0].split('()')[0]] = tmp[1]
        tmp = []
        for key in dictionary:
            tmp.append('{0}() = {1}'.format(key,dictionary[key]))
        functions = tmp
    return functions
            
        
            
    
def analyzeHelper(document,reactionDefinitions,useID,outputFile,speciesEquivalence,atomize,translator,bioGrid = False):
    '''
    taking the atomized dictionary and a series of data structure, this method
    does the actual string output.
    '''
    useArtificialRules = False
    parser =SBML2BNGL(document.getModel(),useID)
    database = structures.Databases()
    #translator,log,rdf = m2c.transformMolecules(parser,database,reactionDefinitions,speciesEquivalence)
        
    #try:
    #bioGridDict = {}
    #if biogrid:
    #    bioGridDict = biogrid()
    #if atomize:
    #    translator = mc.transformMolecules(parser,database,reactionDefinitions,speciesEquivalence,bioGridDict)
    #else:    
    #    translator={} 
    
    parser =SBML2BNGL(document.getModel(),useID)
    #except:
    #    print 'failure'
    #    return None,None,None,None
    
    #translator = {}
    param,zparam = parser.getParameters()
    rawSpecies = {}
    for species in parser.model.getListOfSpecies():
            rawtemp = parser.getRawSpecies(species,[x.split(' ')[0] for x in param])
            rawSpecies[rawtemp['identifier']] = rawtemp
    parser.reset()
    
    molecules,initialConditions,observables,speciesDict = parser.getSpecies(translator,[x.split(' ')[0] for x in param])
    #finally, adjust parameters and initial concentrations according to whatever  initialassignments say

    param,zparam,initialConditions = parser.getInitialAssignments(translator,param,zparam,molecules,initialConditions)
    compartments = parser.getCompartments()
    functions = []
    assigmentRuleDefinedParameters = []
    reactionParameters,rules,rateFunctions = parser.getReactions(translator,len(compartments)>1,atomize=atomize)
    functions.extend(rateFunctions)
    aParameters,aRules,nonzparam,artificialRules,removeParams,artificialObservables = parser.getAssignmentRules(zparam,param,rawSpecies,observables)
    for element in nonzparam:
        param.append('{0} 0'.format(element))
    param = [x for x in param if x not in removeParams]
    tags = '@{0}'.format(compartments[0].split(' ')[0]) if len(compartments) == 1 else '@cell'
    molecules.extend([x.split(' ')[0] for x in removeParams])
    if len(molecules) == 0:
        compartments = []
    observables.extend('Species {0} {0}'.format(x.split(' ')[0]) for x in removeParams)
    for x in removeParams:
        initialConditions.append(x.split(' ')[0] + tags + ' ' + x.split(' ')[1])
    ##Comment out those parameters that are defined with assignment rules
    ##TODO: I think this is correct, but it may need to be checked
    tmpParams = []
    for idx,parameter in enumerate(param):
        for key in artificialObservables:
            
            if re.search('^{0}\s'.format(key),parameter)!= None:
                assigmentRuleDefinedParameters.append(idx)
    tmpParams.extend(artificialObservables)
    tmpParams.extend(removeParams)
    tmpParams = set(tmpParams)
    correctRulesWithParenthesis(rules,tmpParams)
    for element in assigmentRuleDefinedParameters:
        param[element] = '#' + param[element]
    
    deleteMolecules = []
    deleteMoleculesFlag = True 
    for key in artificialObservables:
        flag = -1
        for idx,observable in enumerate(observables):
            if 'Species {0} {0}()'.format(key) in observable:
                flag = idx
        if flag != -1:
            observables.pop(flag)
        functions.append(artificialObservables[key])
        flag = -1
        
        if '{0}()'.format(key) in molecules:
            flag = molecules.index('{0}()'.format(key))
        
        if flag != -1:
            if deleteMoleculesFlag:
                deleteMolecules.append(flag)
            else:
                deleteMolecules.append(key)
            #result =validateReactionUsage(molecules[flag],rules)
            #if result != None:
            #    logMess('ERROR','Pseudo observable {0} in reaction {1}'.format(molecules[flag],result))
            #molecules.pop(flag)
            
        flag = -1
        for idx,specie in enumerate(initialConditions):
            if ':{0}('.format(key) in specie:
                flag = idx
        if flag != -1:
            initialConditions[flag] = '#' + initialConditions[flag]

    for flag in sorted(deleteMolecules,reverse=True):
        
        if deleteMoleculesFlag:
            logMess('WARNING:Simulation','{0} reported as function, but usage is ambiguous'.format(molecules[flag]) )
            result =validateReactionUsage(molecules[flag],rules)
            if result != None:
                logMess('ERROR:Simulation','Pseudo observable {0} in reaction {1}'.format(molecules[flag],result))

            #since we are considering it an observable delete it from the molecule and
            #initial conditions list
            #s = molecules.pop(flag)
            #initialConditions = [x for x in initialConditions if '$' + s not in x]
        else:
            logMess('WARNING:Simulation','{0} reported as species, but usage is ambiguous.'.format(flag) )
            artificialObservables.pop(flag)
            

    functions.extend(aRules)
    sbmlfunctions = parser.getSBMLFunctions()
    processFunctions(functions,sbmlfunctions,artificialObservables,rateFunctions)
    for interation in range(0,3):
        for sbml2 in sbmlfunctions:
            for sbml in sbmlfunctions:
                if sbml == sbml2:
                    continue
                if sbml in sbmlfunctions[sbml2]:
                    sbmlfunctions[sbml2] = writer.extendFunction(sbmlfunctions[sbml2],sbml,sbmlfunctions[sbml])
    functions = reorderFunctions(functions)

    functions = changeNames(functions,aParameters)
#     print [x for x in functions if 'functionRate60' in x]

    functions = unrollFunctions(functions)
    rules = changeRates(rules,aParameters)
    
    if len(compartments) > 1 and 'cell 3 1.0' not in compartments:
        compartments.append('cell 3 1.0')

    #sbml always has the 'cell' default compartment, even when it
    #doesn't declare it
    elif len(compartments) == 0 and len(molecules) != 0:
        compartments.append('cell 3 1.0')
    
    
    if len(artificialRules) + len(rules) == 0:
        logMess('ERROR:Simulation','The file contains no reactions')
    if useArtificialRules or len(rules) == 0:
        rules =['#{0}'.format(x) for x in rules]
        evaluate =  evaluation(len(observables),translator)

        artificialRules.extend(rules)
        rules = artificialRules
        


    else:
        artificialRules =['#{0}'.format(x) for x in artificialRules]
        evaluate =  evaluation(len(observables),translator)

        rules.extend(artificialRules)
    commentDictionary = {}
    
    if atomize:
        commentDictionary['notes'] = "'This is an atomized translation of an SBML model created on {0}.".format(time.strftime("%d/%m/%Y"))
    else:
        commentDictionary['notes'] = "'This is a plain translation of an SBML model created on {0}.".format(time.strftime("%d/%m/%Y"))
    commentDictionary['notes'] += " The original model has {0} molecules and {1} reactions. The translated model has {2} molecules and {3} rules'".format(parser.model.getNumSpecies(),parser.model.getNumReactions(),len(molecules),len(set(rules)))
    meta = parser.getMetaInformation(commentDictionary)

    
    from collections import OrderedDict
    finalString = writer.finalText(meta,param+reactionParameters,molecules,initialConditions,list(OrderedDict.fromkeys(observables)),list(OrderedDict.fromkeys(rules)),functions,compartments,outputFile)
    
    #print outputFile
    
    logMess('INFO:Summary','File contains {0} molecules out of {1} original SBML species'.format(len(molecules),len(observables)))
    #store a logfile
    try:
        if len(logMess.log) > 0:
            with open(outputFile + '.log', 'w') as f:
                for element in logMess.log:
                    f.write(element + '\n')
    except AttributeError:
        print "error"
    except IOError:
        pass
        #print ""
    
    #rate of each classified rule
    evaluate2 = 0 if len(observables) == 0 else len(molecules)*1.0/len(observables)
    
        
    return len(rules),len(observables),evaluate,evaluate2,len(compartments), parser.getSpeciesAnnotation(),finalString,speciesDict
    
    '''
    if translator != {}:
        for element in database.classifications:
            if element not in classificationDict:
                classificationDict[element] = 0.0
            classificationDict[element] += 1.0/len(database.classifications)
        return len(rules), evaluate,parser.getModelAnnotation(),classificationDict
    '''
    #return None,None,None,None

def processFile(translator, parser, outputFile):
    param2 = parser.getParameters()
    molecules, species, observables = parser.getSpecies(translator)
    compartments = parser.getCompartments()
    param, rules, functions = parser.getReactions(translator, True)
    param += param2
    writer.finalText(param, molecules, species, observables, rules,
                     functions, compartments, outputFile)

   
def BNGL2XML():
    pass

def getAnnotations(annotation):
    annotationDictionary = []
    if annotation == [] or annotation is None:
        return []
    for index in range(0, annotation.getNumAttributes()):
        annotationDictionary.append(annotation.getValue(index))
    return annotationDictionary

def getAnnotationsDict(annotation):
    annotationDict = {}
    for element in annotation:
        annotationDict[element] = getAnnotations(annotation[element])
    return annotationDict

def processFile2():
    for bioNumber in [19]:
        #if bioNumber in [398]:
        #    continue
    #bioNumber = 175
        logMess.log = []
        logMess.counter = -1
        reactionDefinitions,useID,naming = selectReactionDefinitions('BIOMD%010i.xml' %bioNumber)
        print reactionDefinitions, useID
        #reactionDefinitions = 'reactionDefinitions/reactionDefinition7.json'
        #spEquivalence = 'reactionDefinitions/speciesEquivalence19.json'
        spEquivalence = detectCustomDefinitions(bioNumber)
        print spEquivalence
        useID = False
        #reactionDefinitions = 'reactionDefinitions/reactionDefinition9.json'
        outputFile = 'complex/output' + str(bioNumber) + '.bngl'
        analyzeFile('XMLExamples/curated/BIOMD%010i.xml' % bioNumber, reactionDefinitions,
                    useID,naming,outputFile,speciesEquivalence=spEquivalence,atomize=True,bioGrid=True)

        if len(logMess.log) > 0:
            with open(outputFile + '.log', 'w') as f:
                for element in logMess.log:
                    f.write(element + '\n')

def detectCustomDefinitions(bioNumber):
    '''
    returns a speciesDefinition<bioNumber>.json fileName if it exist
    for the current bioModels. None otherwise
    '''
    directory = 'reactionDefinitions'
    onlyfiles = [ f for f in listdir('./' + directory)]
    if 'speciesEquivalence{0}.json'.format(bioNumber) in onlyfiles:
        return '{0}/speciesEquivalence{1}.json'.format(directory,bioNumber)
    return None

import pyparsing
def main():
    jsonFiles = [ f for f in listdir('./reactionDefinitions') if f[-4:-1] == 'jso']
    jsonFiles.sort()
    parser = OptionParser()
    rulesLength = []
    evaluation = []
    evaluation2 = []
    compartmentLength = []
    parser.add_option("-i","--input",dest="input",
        default='XMLExamples/curated/BIOMD0000000272.xml',type="string",
        help="The input SBML file in xml format. Default = 'input.xml'",metavar="FILE")
    parser.add_option("-o","--output",dest="output",
        default='output.bngl',type="string",    
        help="the output file where we will store our matrix. Default = output.bngl",metavar="FILE")

    (options, _) = parser.parse_args()
    #144
    rdfArray = []
    #classificationArray = []
    #18,32,87,88,91,109,253,255,268,338,330
    #normal:51,353
    #cycles 18,108,109,255,268,392
    for bioNumber in range(1,549):
        
        #if bioNumber in [81,151,175,205,212,223,235,255,326,328,347,370,404,428,430,431,443,444,452,453,465,474]:
        #    continue
    #bioNumber = 175
        logMess.log = []
        logMess.counter = -1
        #reactionDefinitions,useID,naming = selectReactionDefinitions('BIOMD%010i.xml' %bioNumber)
        #print reactionDefinitions, useID
        #reactionDefinitions = 'reactionDefinitions/reactionDefinition7.json'
        #spEquivalence = 'reactionDefinitions/speciesEquivalence19.json'
        #spEquivalence = naming
        #reactionDefinitions = 'reactionDefinitions/reactionDefinition8.json'
        #rlength, reval, reval2, clength,rdf = analyzeFile('XMLExamples/curated/BIOMD%010i.xml' % bioNumber, 
        #                                                  reactionDefinitions,False,'complex/output' + str(bioNumber) + '.bngl',
        #                                                    speciesEquivalence=spEquivalence,atomize=True)

        try:
            rlength = reval = reval2 = slength = None
            rlength, slength,reval, reval2, clength,rdf  = analyzeFile('XMLExamples/curated/BIOMD%010i.xml' % bioNumber, resource_path('config/reactionDefinitions.json'),
                False,resource_path('config/namingConventions.json'),
                #'/dev/null',
                'complex2/' + 'BIOMD%010i.xml' % bioNumber + '.bngl',
                speciesEquivalence=None,atomize=True,bioGrid=False)

    
            #print '++++',bioNumber,rlength,reval,reval2,clength
                                                                
        except IOError:
            continue
        
        except KeyError:
            print 'keyErrorerror--------------',bioNumber
            continue
        except OverflowError:
            print 'overFlowerror--------------',bioNumber
            continue
        except ValueError:
            print 'valueError',bioNumber
        except pyparsing.ParseException:
            print 'pyparsing',bioNumber
        finally:  
            if rlength != None:        
                rulesLength.append({'index':bioNumber,'nreactions':rlength,
                'atomization':reval,'compression':reval2,
                'nspecies':slength})
                compartmentLength.append(clength)
                rdfArray.append(getAnnotationsDict(rdf))
            
            else:
                rulesLength.append([bioNumber,-1,0,0])
                compartmentLength.append(0)
                rdfArray.append({})
        
            #classificationArray.append({})
    #print evaluation
    #print evaluation2
    #sortedCurated = [i for i in enumerate(evaluation), key=lambda x:x[1]]
    print [(idx+1,x) for idx,x in enumerate(rulesLength) if  x > 50]
    with open('sortedD.dump','wb') as f:
        pickle.dump(rulesLength,f)
    with open('annotations.dump','wb') as f:
        pickle.dump(rdfArray,f)
    #with open('classificationDict.dump','wb') as f:
    #    pickle.dump(classificationArray,f)
    '''
    plt.hist(rulesLength,bins=[10,30,50,70,90,110,140,180,250,400])
    plt.xlabel('Number of reactions',fontsize=18)
    plt.savefig('lengthDistro.png')
    plt.clf()
    plt.hist(evaluation, bins=[0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7,
                                0.8, 0.9, 1.0])
    plt.xlabel('Atomization Degree',fontsize=18)    
    plt.savefig('ruleifyDistro.png')
    plt.clf()
    plt.hist(evaluation2, bins=[0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7,
                                0.8, 0.9, 1.0])
    plt.xlabel('Atomization Degree', fontsize=18)
    plt.savefig('ruleifyDistro2.png')
    plt.clf()
    ev = []
    idx = 1
    for x, y, z in zip(rulesLength, evaluation, compartmentLength):
        
        if idx in [18, 51, 353, 108, 109, 255, 268, 392]:
            idx+=1

        if x < 15 and y > 0.7 and z>1:
            print '---',idx,x,y
        idx+=1
    #plt.hist(ev,bins =[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0])
    #plt.xlabel('Atomization Degree',fontsize=18)    
    #plt.savefig('ruleifyDistro3.png')
    '''
            
def main2():
    with open('XMLExamples/curated/BIOMD0000000019.xml','r') as f:
        st = f.read()
        print readFromString(st,
              resource_path('config/reactionDefinitions.json'),False,None,True)        



def isActivated(statusVector):
    if statusVector[0] != '' or statusVector[1] not in ['','0']:
        return True
    return False
    

def flatStatusVector(statusVector):
    if statusVector[0] != '':
        return '!'
    return statusVector[1]
 
def xorBox(status1,status2):
    return not(status1 & status2)
    
def orBox(status1,status2):
    return (status1,status2)
    
def totalEnumerations(pairList):
    xCoordinate = set()
    yCoordinate = set()
    for element in pairList:
        xCoordinate.add(element[0])
        yCoordinate.add(element[1])
    xCoordinate = list(xCoordinate)
    yCoordinate = list(yCoordinate)
    matrix = np.zeros((len(xCoordinate),len(yCoordinate)))
    for element in pairList:
        matrix[xCoordinate.index(element[0])][yCoordinate.index(element[1])] = 1
    return np.all(np.all(matrix))
   
def getRelationshipDegree(componentPair,statusQueryFunction,comparisonFunction,finalComparison):
    componentPairRelationshipDict = {}    
    for pair in componentPair:
        stats = []
        for state in componentPair[pair]:
            status1 = statusQueryFunction(state[0])
            status2 = statusQueryFunction(state[1])
            comparison = comparisonFunction(status1,status2)
            stats.append(comparison)
        if finalComparison(stats):
            print pair,componentPair[pair]
        componentPairRelationshipDict[pair] = finalComparison(stats)
    return componentPairRelationshipDict

'''
def createPlot(labelDict):
    #f, ax = plt.subplots(int(math.ceil(len(labelDict)/4)),4)
    for idx,element in enumerate(labelDict):
        plt.cla()
        tmp = list(set([y for x in labelDict[element] for y in x]))
        xaxis = [tmp.index(x[0]) for x in labelDict[element] if  labelDict[element][x]== True]
        yaxis = [tmp.index(x[1]) for x in labelDict[element] if labelDict[element][x] == True]
        #6print tmp,xaxis,yaxis
        plt.scatter(xaxis,yaxis)
        plt.xticks(range(len(tmp)),tmp)
        plt.yticks(range(len(tmp)),tmp)
        plt.title(element)
        #ax[math.floor(idx/4)][idx%4].scatter(xaxis,yaxis)
        #ax[math.floor(idx/4)][idx%4].xticks(range(len(tmp)),tmp)
        #ax[math.floor(idx/4)][idx%4].yticks(range(len(tmp)),tmp)
        #ax[math.floor(idx/4)][idx%4].title(element)
        plt.savefig('{0}.png'.format(element))
        print '{0}.png'.format(element)
'''

def statFiles():
    
    for bioNumber in [19]:
        reactionDefinitions,useID = selectReactionDefinitions('BIOMD%010i.xml' %bioNumber)
        #speciesEquivalence = None
        speciesEquivalence = 'reactionDefinitions/speciesEquivalence19.json'
                
        componentPairs =  extractCompartmentStatistics('XMLExamples/curated/BIOMD%010i.xml' % bioNumber,useID,reactionDefinitions,speciesEquivalence)
        #analyze the relationship degree betweeen the components of each molecule
        
        #in this case we are analyzing for orBoxes, or components
        #that completely exclude each other
        xorBoxDict = {}
        orBoxDict = {}
        for molecule in componentPairs:
            xorBoxDict[molecule] = getRelationshipDegree(componentPairs[molecule],isActivated,xorBox,all)
            #print '----------------------',molecule,'---------'            
            orBoxDict[molecule] =  getRelationshipDegree(componentPairs[molecule],flatStatusVector,orBox,totalEnumerations)

        #createPlot(orBoxDict)
        box = []
        box.append(xorBoxDict)
        #box.append(orBoxDict)
        with open('orBox{0}.dump'.format(bioNumber),'wb') as f:
            pickle.dump(box,f)

def processDir(directory,atomize=True):
    from os import listdir
    from os.path import isfile, join
    resultDir = {}
    xmlFiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) and f.endswith('xml')]
    blackList = [175,205,212,223,235,255,328,370,428,430,431,443,444,452,453,465]

    for xml in xmlFiles:
        #try:
        if xml not in ['MODEL1310110034.xml'] and len([x for x in blackList if str(x) in xml]) == 0:
            print xml
            try:
                rlength,slength,reval,reval2,_,_ = analyzeFile(directory + xml,'reactionDefinitions/reactionDefinition7.json',
                        False, resource_path('config/namingConventions.json'),
                        '/dev/null', speciesEquivalence=None,atomize=True,bioGrid=False)  
                resultDir[xml] = [rlength,reval,reval2]
            except:
                resultDir[xml] = [-1,0,0]
    with open('evalResults.dump','wb') as f:
        pickle.dump(resultDir,f)
        #except:
                #continue'
    
def processFile3(fileName,customDefinitions=None,atomize=True,bioGrid=False,output=None):
    '''
    processes a file. derp.
    '''
    logMess.log = []
    logMess.counter = -1
    reactionDefinitions = resource_path('config/reactionDefinitions.json')
    spEquivalence = customDefinitions
    namingConventions = resource_path('config/namingConventions.json')
    #spEquivalence = None
    useID = False
    #reactionDefinitions = 'reactionDefinitions/reactionDefinition9.json'
    rlength = -1
    reval = -1
    reval2 = -1
    if output:
        outputFile = output
    else:
        outputFile = '{0}.bngl'.format(fileName)
    rlength, slength,reval, reval2, clength,rdf  = analyzeFile(fileName, reactionDefinitions,
                useID,namingConventions,outputFile,speciesEquivalence=spEquivalence,atomize=atomize,bioGrid=bioGrid)

    if len(logMess.log) > 0:
        with open(fileName + '.log', 'w') as f:
            for element in logMess.log:
                f.write(element + '\n')
    return rlength,reval,reval2
    
    
def listFiles(minReactions,directory):
    '''
    List of SBML files that meet a given condition
    '''
    from os import listdir
    from os.path import isfile, join
    
    xmlFiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) and 'xml' in f]
    outputList = []
    for xml in xmlFiles:
        print '.',
        reader = libsbml.SBMLReader()
        document = reader.readSBMLFromFile(directory + xml)
        model = document.getModel()
        if model == None:
            continue
        if len(model.getListOfReactions()) > minReactions:
            outputList.append(xml)
    print len(outputList)
    
if __name__ == "__main__":
    #identifyNamingConvention()
    #processDatabase()
    
    #main2()
    
    analyzeFile('XMLExamples/curated/BIOMD0000000019.xml', resource_path('config/reactionDefinitions.json'),
                    False, resource_path('config/namingConventions.json'),
                    'BIOMD0000000027.xml' + '.bngl', 
                    speciesEquivalence=None,atomize=True,bioGrid=False)
    
    #main()
    #processFile3('XMLExamples/noncurated/MODEL2463576061.x5ml')
    #processFile3('XMLExamples/jws/dupreez2.xml')
    #processFile3('XMLExamples/non_curated/MODEL1012220002.xml') 
    #output=48
    #processFile3('XMLExamples/curated/BIOMD00000000151.xml',bioGrid=False) 
    
    #param  = [452]
    '''
    param = 2
    #use 105 as an example for (2,2) reactions
    #527

    
    analyzeFile('XMLExamples/curated/BIOMD%010i.xml' % param, resource_path('config/reactionDefinitions.json'),
                    False, resource_path('config/namingConventions.json'),
                    'complex2/output' + str(param) + '.bngl', speciesEquivalence=None,atomize=True,bioGrid=False)

    '''
    '''    
    analyzeFile('plain2_sbml.xml', resource_path('config/reactionDefinitions.json'),
                    False, resource_path('config/namingConventions.json'),

    '''
    '''
    analyzeFile('XMLExamples/BMID000000142971.xml', resource_path('config/reactionDefinitions.json'),
                    False, resource_path('config/namingConventions.json'),
                    'complex/BMID000000142971.xml' + '.bngl', speciesEquivalence=None,atomize=True,bioGrid=False)
    
    '''
    '''
    param = '00870'
    analyzeFile('test/testl2v4/{0}/{0}-sbml-l2v4.xml'.format(param), 'reactionDefinitions/reactionDefinition7.json',
                    False, resource_path('config/namingConventions.json'),
                    'complex/output' + str(param) + '.bngl', speciesEquivalence=None,atomize=True,bioGrid=False)
    '''
    #processFile3('XMLExamples/curated/BIOMD0000000048.xml',customDefinitions=None,atomize=True)    
    #processFile3('/home/proto/Downloads/compartment_test_sbml.xml',customDefinitions=None,atomize=True)    
    #processDir('XMLExamples/curated/')
    #processFile3('hexamer.xml')
    #with open('dimer.xml','r') as f:
    #    r = f.read()
    #print readFromString(r,resource_path('config/reactionDefinitions.json'),False,None,True)
    #statFiles()
    #main2()
    #print readFromString('dsfsdf',resource_path('config/reactionDefinitions.json'),False)
    #processFile2()
    #listFiles(50,'./XMLExamples/curated/')
#todo: some of the assignmentRules defined must be used instead of parameters. remove from the paraemter
#definitions those that are defined as 0'
#2:figure out which assignment rules are being used in reactions. Done before the substitution for id;s
#http://nullege.com/codes/show/src@s@e@semanticsbml-HEAD@semanticSBML@annotate.py
#http://wiki.geneontology.org/index.php/Example_Queries#Find_terms_by_GO_ID
#http://www.geneontology.org/GO.database.shtml  
