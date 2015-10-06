# -*- coding: utf-8 -*-

import libsbml
import re
import collections
import os
import fnmatch
import pickle
import progressbar

def standardizeName(name):
    '''
    Remove stuff not used by bngl
    '''

    sbml2BnglTranslationDict = {"^": "",
                                "'": "",
                                "*": "m", " ": "_",
                                "#": "sh",
                                ":": "_", 'α': 'a',
                                'β': 'b',
                                'γ': 'g', "(": "__",
                                ")": "__",
                                " ": "", "+": "pl",
                                "/": "_", ":": "_",
                                "-": "_",
                                ".": "_",
                                '?': "unkn",
                                ',': '_',
                                '[': '__',
                                ']': '__',
                                '>': '_',
                                '<': '_'}

    for element in sbml2BnglTranslationDict:
        name = name.replace(element, sbml2BnglTranslationDict[element])
    name = re.sub('[\W]', '', name)
    return name


def getValidFiles(directory, extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append(os.path.join(root, filename))
    return matches


def getAnnotationsFromFile(fileName):
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile(fileName)
    model = document.getModel()
    annotationDictionary = collections.defaultdict(list)
    for species in model.getListOfSpecies():
        annotationXML = species.getAnnotation()
        lista = libsbml.CVTermList()
        libsbml.RDFAnnotationParser.parseRDFAnnotation(annotationXML, lista)
        if lista.getSize() == 0:
            annotationDictionary[standardizeName(species.getName())] = []
        for index in range(0, lista.getSize()):
            #name = tmpList.getBiologicalQualifier() if tmpList.getQualifierType() == _libsbml.BIOLOGICAL_QUALIFIER else tmpList.getMo
            for index2 in range(0, lista.get(index).getResources().getNumAttributes()):
                annotationDictionary[standardizeName(species.getName())].append(
                    lista.get(index).getResources().getValue(index2))

    return annotationDictionary

bioqual = ['BQB_IS','BQB_HAS_PART','BQB_IS_PART_OF','BQB_IS_VERSION_OF',
          'BQB_HAS_VERSION','BQB_IS_HOMOLOG_TO',
'BQB_IS_DESCRIBED_BY','BQB_IS_ENCODED_BY','BQB_ENCODES','BQB_OCCURS_IN',
'BQB_HAS_PROPERTY','BQB_IS_PROPERTY_OF','BQB_UNKNOWN']

modqual = ['BQM_IS','BQM_IS_DESCRIBED_BY','BQM_IS_DERIVED_FROM','BQM_UNKNOWN']


from collections import defaultdict
def getModelAnnotationFromFile(fileName):
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile(fileName)
    model = document.getModel()
    annotationXML = model.getAnnotation()
    lista = libsbml.CVTermList()
    libsbml.RDFAnnotationParser.parseRDFAnnotation(annotationXML,lista)
    metaDict = []

    for idx in range(lista.getSize()):
      for idx2 in range(0, lista.get(idx).getResources().getLength()):
          resource = lista.get(idx).getResources().getValue(idx2)
          qualifierType = lista.get(idx).getQualifierType()
          qualifierDescription= bioqual[lista.get(idx).getBiologicalQualifierType()] if qualifierType \
          else modqual[lista.get(idx).getModelQualifierType()]
          #resource = resolveAnnotation(resource)
          if 'BQB' in qualifierDescription:
            metaDict.append(resource)
    return metaDict

def getAnnotationFromFolder(directory):
    validFiles = getValidFiles(directory, 'xml')
    annotationDict = {}
    progress  = progressbar.ProgressBar()
    print('Processing annotations from directory {0}'.format(directory))
    for index in progress(range(0,len(validFiles))):
        annotationDict[validFiles[index]] = getAnnotationsFromFile(validFiles[index])

    with open(os.path.join(directory,'annotationDictionary.dump'), 'wb') as f:
        pickle.dump(annotationDict,f)

def getModelAnnotationFromFolder(directory):
    validFiles = getValidFiles(directory, 'xml')
    annotationDict = {}
    progress  = progressbar.ProgressBar()
    print('Processing model annotations from directory {0}'.format(directory))
    for index in progress(range(0,len(validFiles))):
        annotationDict[validFiles[index]] = getModelAnnotationFromFile(validFiles[index])

    with open(os.path.join(directory,'modelAnnotationDictionary.dump'), 'wb') as f:
        pickle.dump(annotationDict,f)

if __name__ == "__main__":
    getModelAnnotationFromFolder('XMLExamples/curated')
    #print getModelAnnotationFromFile('XMLExamples/curated/BIOMD0000000019.xml')
