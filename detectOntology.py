# -*- coding: utf-8 -*-
"""
Created on Sat Oct 19 15:19:35 2013

@author: proto
"""

import libsbml
import numpy as np
import difflib
from collections import Counter
from itertools import dropwhile
import json
import ast

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
    differenceList = []
    threshold = 3
    namePairs = []

    for idx,element in enumerate(scoreMatrix):
        for idx2,element2 in enumerate(scoreMatrix[idx]):
            if scoreMatrix[idx][idx2] <= threshold and  idx < idx2:
                namePairs.append([speciesName[idx],speciesName[idx2]])
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
  
  
def main(fileName):
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile(fileName)
    model = document.getModel()
    speciesName = []
    threshold = 2
    ontology =  loadOntology('reactionDefinitions/namingConventions.json')
    differenceCounter = Counter()
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
    
    for element in differenceCounter:
        if element in ontology['patterns']:
            print element, ontology['patterns'][element]
        
            
if __name__ == "__main__":
    bioNumber= 19
    main('XMLExamples/curated/BIOMD%010i.xml' % bioNumber)