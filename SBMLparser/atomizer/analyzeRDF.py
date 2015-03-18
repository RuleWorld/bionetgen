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
from sbml2bngl import SBML2BNGL

import libsbml
import collections

def getAnnotations(parser,stringKey=None):
    annotation = parser.getSpeciesAnnotation()
    annotationDictionary = collections.defaultdict(set)
    for key,value in annotation.items():
        annotationList = []
        if annotation[key] != None:
            for element in annotation[key]:
                for index in range(0,element.getNumAttributes()):
                    if not stringKey or stringKey in element.getValue(index):
                        annotationList.append(element.getValue(index))
                if annotationList == []:
                    continue
                annotationDictionary[key].add(tuple(annotationList))
                #annotationDictionary[frozenset(annotationList)].sort(lambda x,y: cmp(len(x), len(y)))
    return annotationDictionary


def getSpeciesAnnotationStructure(parser):
    model = parser.model
    for species in model.getListOfSpecies():
        name = species.getName()
        speciesId = species.getId()
        annotation = species.getAnnotation()
        lista = libsbml.CVTermList()
        libsbml.RDFAnnotationParser.parseRDFAnnotation(annotation,lista)
        for idx in range(0,lista.getSize()):
          for idx2 in range(0, lista.get(idx).getResources().getLength()):
              resource = lista.get(idx).getResources().getValue(idx2)
              qualifierType = lista.get(idx).getQualifierType()
              qualifierDescription= bioqual[lista.get(idx).getBiologicalQualifierType()] if qualifierType \
              else modqual[lista.get(idx).getModelQualifierType()]
              #resource = resolveAnnotation(resource)
    
    
def getEquivalence(species,rdf_database):
    '''
    *species* is the species whose equivalence we will go and search
    This method will search through the RDF database and look if param 'species'
    is equal to any other element in the species database
    '''
    
    for element in rdf_database:
        if species in rdf_database[element]:
            if rdf_database[element].index(species) == 0:
                return []
            #return [x for x in rdf_database[element] if x != species]
            
            #well only return the first one by default
            return [rdf_database[element][0]]
    return []

if __name__ == "__main__":
    reader = libsbml.SBMLReader()
    #BIOMD0000000272
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000219.xml')
    #document = reader.readSBMLFromFile('XMLExamples/simple4.xml')
    model = document.getModel()        
    parser = SBML2BNGL(model)
    annotationDictionary =  getAnnotations(parser)
    print annotationDictionary
    #print getEquivalence('SAv_EpoR',annotationDictionary)
    #print annotation
    #print rules    
