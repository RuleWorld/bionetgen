# -*- coding: utf-8 -*-
"""
Created on Fri Apr 27 14:45:24 2012

@author: proto
"""

'''
this method classifies reactants according to the rdf information, and gives
us information on which reactants are the same, and how do they differ
(compartment etc)
'''

from libsbml2bngl import SBML2BNGL
import libsbml

def getAnnotations(parser,stringKey):
    annotation = parser.getSpeciesAnnotation()
    annotationDictionary = {}
    for key,value in annotation.items():
        annotationList = []
        if annotation[key] != None:
            for index in range(0,annotation[key].getNumAttributes()):
                if stringKey in annotation[key].getValue(index):
                    annotationList.append(annotation[key].getValue(index))
            if frozenset(annotationList) in annotationDictionary:
                annotationDictionary[frozenset(annotationList)].append(key)
            else:
                annotationDictionary[frozenset(annotationList)] = [key]
    return annotationDictionary

if __name__ == "__main__":
    reader = libsbml.SBMLReader()
    #BIOMD0000000272
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000272.xml')
    #document = reader.readSBMLFromFile('XMLExamples/simple4.xml')
    model = document.getModel()        
    parser = SBML2BNGL(model)
    print getAnnotations(parser,'uniprot')
    #print annotation
    #print rules    
