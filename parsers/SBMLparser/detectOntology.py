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
try:
    import pandas as pd
except ImportError:
    pd = None
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

def getDifferences(scoreMatrix, speciesName,threshold):
    '''
    given a list of strings and a scoreMatrix, return the list of difference between
    those strings with a levenshtein difference of less than two
    returns: 
        namePairs: list of tuples containing strings with distance <2
        differenceList: list of differences between the tuples in namePairs
    '''
    differenceList = []
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
  
def findLongestSubstring(speciesA,speciesB):
    sm = difflib.SequenceMatcher(a=speciesA,b=speciesB)
    longestMatch = sm.find_longest_match(0,len(speciesA),0,len(speciesB))
    return speciesA[longestMatch[0]:longestMatch[0]+longestMatch[2]]

def defineEditDistanceMatrix(speciesName,similarityThreshold=3):
    '''
    obtains a distance matrix and a pairs of elements that are close 
    in distance, along with the proposed differences
    '''
    differenceCounter = Counter()
    scoreMatrix = np.zeros((len(speciesName),len(speciesName)))
    for idx,species in enumerate(speciesName):
        for idx2,species2 in enumerate(speciesName):
            if species == species2 or scoreMatrix[idx][idx2] != 0:
                continue
            scoreMatrix[idx][idx2] = levenshtein(speciesName[idx],speciesName[idx2])
            scoreMatrix[idx2][idx] = scoreMatrix[idx][idx2]
    namePairs,differenceList = getDifferences(scoreMatrix, speciesName,similarityThreshold)
    differenceCounter.update(differenceList)
    return namePairs,differenceList,differenceCounter
    
def analyzeNamingConventions(speciesName,ontologyFile,ontologyDictionary={},similarityThreshold=3):
    patternClassification = {}
    pairClassification = {}

    ontology =  loadOntology(ontologyFile)
    finalDifferenceCounter = Counter()
    namePairs,differenceList,differenceCounter = defineEditDistanceMatrix(speciesName,similarityThreshold)
    
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
    xmlFiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) and f.endswith('xml')]
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
                
        namePairs,differenceList = getDifferences(scoreMatrix, speciesName,3)
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
        #dictionary = pickle.load(f)
        fileCounter = pickle.load(f)
    totalCounter = Counter()
    for element in counter:
        
        totalCounter[element] = counter[element] * fileCounter[element]/469.0
    keys = totalCounter.most_common(200)
    #keys = keys[1:]
    pp = pprint.PrettyPrinter(indent=4)
    pp.pprint(keys)
    data = pd.DataFrame(keys)
    #print data.to_excel('name.xls')
    '''
    for element in keys:
        print '------------------'
        print element
        pp.pprint(dictionary[element[0]])
    ''' 
    
if __name__ == "__main__":
    bioNumber= 19
    #main('XMLExamples/curated/BIOMD%010i.xml' % bioNumber)
    
    databaseAnalysis('XMLExamples/non_curated/','non_ontologies.dump')    
    #analyzeTrends('ontologies.dump')