# -*- coding: utf-8 -*-
"""
Created on Sat Oct 19 15:19:35 2013

@author: proto
"""
import pprint
import libsbml
import numpy as np
import difflib
from collections import Counter
import json
import ast
import pickle
from os import listdir
from os.path import isfile, join
import re
from copy import deepcopy

def levenshtein(s1, s2):
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

def getDifferences(scoreMatrix, speciesName):
    '''
    given a list of strings and a scoreMatrix, return the list of difference between
    those strings with a levenshtein difference of less than two
    returns: 
        namePairs: list of tuples containing strings with distance <2
        differenceList: list of differences between the tuples in namePairs
    '''
    differenceList = []
    threshold = 3
    namePairs = []

    for idx,element in enumerate(scoreMatrix):
        for idx2,element2 in enumerate(scoreMatrix[idx]):
            if scoreMatrix[idx][idx2] <= threshold and  idx < idx2:
                if len(speciesName[idx])<len(speciesName[idx2]):
                    namePairs.append([speciesName[idx],speciesName[idx2]])
                else:
                    namePairs.append([speciesName[idx2],speciesName[idx]])
    for pair in namePairs:
        if len(pair[1]) < len(pair[0]):
            difference =  difflib.ndiff(pair[1],pair[0])
        else:
            difference =  difflib.ndiff(pair[0],pair[1])
        tmp = []
        for diff in difference:
           if diff[0] == ' ': continue
           tmp.append(diff)
        differenceList.append(tuple(tmp))
    
    return namePairs,differenceList
    
  
def loadOntology(ontologyFile):
    tmp= {}
    with open(ontologyFile,'r') as fp:
         ontology =  json.load(fp)
    for element in ontology['patterns']:
        tmp[ast.literal_eval(element)] = ontology['patterns'][element]
    ontology['patterns'] =  tmp
    return ontology
  
def approximateMatching(ruleList,differences=[]):
    '''
    remove compound differencese (>2 characters) and instead represent them with symbols
    returns transformed string and an equivalence dictionary
    
    '''
    def curateString(element,differences,symbolList = ['#','&',';','@'],equivalenceDict={}):
        tmp = element
        for difference in differences:
            if difference in element:
                if difference.startswith('_'):
                    if difference not in equivalenceDict:
                        symbol = symbolList.pop()
                        equivalenceDict[difference] = symbol
                    else:
                        symbol = equivalenceDict[difference]
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
    if len(ruleList[1]) == 1:
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
            return [],[]
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
    return difference1,difference2


def analyzeNamingConventions(speciesName,ontologyFile,ontologyDictionary={},similarityThreshold=2):
    ontology =  loadOntology(ontologyFile)
    differenceCounter = Counter()
    finalDifferenceCounter=Counter()
    scoreMatrix = np.zeros((len(speciesName),len(speciesName)))
    for idx,species in enumerate(speciesName):
        for idx2,species2 in enumerate(speciesName):
            if species == species2 or scoreMatrix[idx][idx2] != 0:
                continue
            scoreMatrix[idx][idx2] = levenshtein(speciesName[idx],speciesName[idx2])
            scoreMatrix[idx2][idx] = scoreMatrix[idx][idx2]
            
    namePairs,differenceList = getDifferences(scoreMatrix, speciesName)
    differenceCounter.update(differenceList)
    patternClassification = {}
    pairClassification = {}
    for element in differenceCounter:
        if element in ontology['patterns']:
            finalDifferenceCounter[element] = differenceCounter[element]
            patternClassification[element] = ontology['patterns'][element]
    for pair,difference in zip(namePairs,differenceList):
        if difference in patternClassification:
            if patternClassification[difference] not in pairClassification:
                pairClassification[patternClassification[difference]] = []
            pairClassification[patternClassification[difference]].append(tuple(pair))
    keys = finalDifferenceCounter.keys()
    tmp = {key:'' for key in keys}
    for key in tmp:
        tmp[key] = ontology['patterns'][key]
    keys =  [''.join(x).replace('+ ','') for x in keys]
    #print ontology
    return pairClassification,keys,tmp


def main(fileName):
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile(fileName)
    model = document.getModel()
    speciesName = []
    
    for species in model.getListOfSpecies():
        speciesName.append(species.getName())

    return analyzeNamingConventions(speciesName,'reactionDefinitions/namingConventions.json')
            
def databaseAnalysis(directory,outputFile):
    xmlFiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) and 'xml' in f]
    differenceCounter = Counter()
    fileDict = {}
    for xml in xmlFiles:
        print xml
        reader = libsbml.SBMLReader()
        document = reader.readSBMLFromFile(directory + xml)
        model = document.getModel()
        if model == None:
            continue
        speciesName = []
        
        for species in model.getListOfSpecies():
            speciesName.append(species.getName())
        scoreMatrix = np.zeros((len(speciesName),len(speciesName)))
        for idx,species in enumerate(speciesName):
            for idx2,species2 in enumerate(speciesName):
                if species == species2 or scoreMatrix[idx][idx2] != 0:
                    continue
                scoreMatrix[idx][idx2] = levenshtein(speciesName[idx],speciesName[idx2])
                scoreMatrix[idx2][idx] = scoreMatrix[idx][idx2]
                
        namePairs,differenceList = getDifferences(scoreMatrix, speciesName)
        differenceCounter.update(differenceList)
        for key,element in zip(differenceList,namePairs):
            if key == ():
                continue
            if key not in fileDict:
                #differenceDict[tuple(key)] = set()
                fileDict[tuple(key)] = set()
            #differenceDict[tuple(key)].add((xml,tuple(element)))
            fileDict[tuple(key)].add(xml)
            
        fileCounter = Counter()
        for element in fileDict:
            fileCounter[element] = len(fileDict[element])
        with open(outputFile,'wb') as f:
            pickle.dump(differenceCounter,f)
            #pickle.dump(differenceDict,f)
            pickle.dump(fileCounter,f)
        
        
def analyzeTrends(inputFile):
    with open(inputFile,'rb') as f:
        counter = pickle.load(f)
        dictionary = pickle.load(f)
        fileCounter = pickle.load(f)
    totalCounter = Counter()
    for element in counter:
        totalCounter[element] = counter[element] * fileCounter[element]
    keys = totalCounter.most_common(35)
    #keys = keys[1:]
    pp = pprint.PrettyPrinter(indent=4)
    pp.pprint(keys)
    for element in keys:
        print '------------------'
        print element
        pp.pprint(dictionary[element[0]])
        
    
if __name__ == "__main__":
    bioNumber= 19
    #main('XMLExamples/curated/BIOMD%010i.xml' % bioNumber)
    
    databaseAnalysis('XMLExamples/non_curated/','non_contologies.dump')    
    #analyzeTrends('ontologies.dump')