# -*- coding: utf-8 -*-
"""
Created on Tue Oct  9 16:02:53 2012

@author: proto
"""

import libsbml
import libsbml2bngl
import analyzeRDF
import numpy as np
import structures
import molecules2complexes as m2c

def loadAnnotations(fileName):
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile(fileName)
    parser = libsbml2bngl.SBML2BNGL(document.getModel())    
    rdfAnnotations = analyzeRDF.getAnnotations(parser,'miriam')
    return rdfAnnotations    

def loadTranslator(fileName,jsonFile):
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000' + fileName + '.xml')
    parser = libsbml2bngl.SBML2BNGL(document.getModel())    
    database = structures.Databases()
    translator = m2c.transformMolecules(parser,database,'reactionDefinitions/reactionDefinition' + str(jsonFile) + '.json')
    return translator    
    


def mergeModels(fileName1,fileName2):
    
    
    annotations1 = loadAnnotations('XMLExamples/curated/BIOMD0000000' + fileName1 + '.xml')
    annotations2 = loadAnnotations('XMLExamples/curated/BIOMD0000000' + fileName2 + '.xml')
    intersection = []
    for annotation in [x for x in annotations1 if x in annotations2]:
        intersection.append(annotation)
    print intersection
    
    print annotations1[intersection[0]],annotations2[intersection[0]]
    
    history = np.load('stats3.npy')
    jsonArray = []
    for element in history:
        if element[0] in (int(fileName1),int(fileName2)):
            jsonArray.append(element[1])
    print jsonArray
    #loadTranslator('XMLExamples/curated/BIOMD0000000' + fileName1 + '.xml','reactionDefinition'+str(int(jsonArray[0])) + '.json')
    if len(jsonArray) < 2:
        print 'We found no meta information related to the input models'
        
    translator1 = loadTranslator(fileName1,int(jsonArray[0]))
    translator2 = loadTranslator(fileName2,int(jsonArray[1]))
    
    print {x:str(translator1[x]) for x in translator1}
    print '---'
    print {x:str(translator2[x]) for x in translator2}
def main():
    mergeModels('336','365')
    
if __name__ == "__main__":
    main()
    
    
