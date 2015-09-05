# -*- coding: utf-8 -*-
"""
Created on Fri Nov 14 18:17:20 2014

@author: proto
"""

import libsbml
from sbml2bngl import SBML2BNGL as  SBML2BNGL
import structures
import atomizer.moleculeCreation as mc
import os
from subprocess import call        
import tempfile
import sys
#sys.path.insert(0, '../utils/')
import consoleCommands
import readBNGXML
import argparse

bioqual = ['BQB_IS','BQB_HAS_PART','BQB_IS_PART_OF','BQB_IS_VERSION_OF',
          'BQB_HAS_VERSION','BQB_IS_HOMOLOG_TO',
'BQB_IS_DESCRIBED_BY','BQB_IS_ENCODED_BY','BQB_ENCODES','BQB_OCCURS_IN',
'BQB_HAS_PROPERTY','BQB_IS_PROPERTY_OF','BQB_HAS_TAXON','BQB_UNKNOWN']

modqual = ['BQM_IS','BQM_IS_DESCRIBED_BY','BQM_IS_DERIVED_FROM','BQM_UNKNOWN']

from collections import defaultdict
import re


class AnnotationExtractor:
    def __init__(self, fileName, speciesEquivalencesFlag=None):
        self.fileName = fileName
        self.sct, self.database, self.sbmlDocument = self.obtainSCT(self.fileName, 'config/reactionDefinitions.json', False,
                                                                    'config/namingConventions.json',
                                                                    speciesEquivalences=speciesEquivalencesFlag)

    def standardizeName(self, name):
        '''
        Remove stuff not used by bngl
        '''
        name2 = name
        
        sbml2BnglTranslationDict = {"^":"",
                                    "'":"",
                                    "*":"m"," ":"_",
                                    "#":"sh",
                                    ":":"_",'α':'a',
                                    'β':'b',
                                    'γ':'g',"(":"__",
                                    ")":"__",
                                    " ":"","+":"pl",
                                    "/":"_",":":"_",
                                    "-":"_",
                                    ".":"_",
                                    '?':"unkn",
                                    ',':'_',
                                    '[':'__',
                                      ']':'__',
                                      '>':'_',
                                      '<':'_'}
                                    
        for element in sbml2BnglTranslationDict:
            name = name.replace(element,sbml2BnglTranslationDict[element])
        name = re.sub('[\W]', '', name)
        return name


    def parseAnnotation(self, annotation):
        speciesAnnotationDict = defaultdict(list)
        lista = libsbml.CVTermList()
        libsbml.RDFAnnotationParser.parseRDFAnnotation(annotation, lista)
        #print '----',species.getName()
        for idx in range(0,lista.getSize()):
            for idx2 in range(0, lista.get(idx).getResources().getLength()):
                resource = lista.get(idx).getResources().getValue(idx2)
                qualifierType = lista.get(idx).getQualifierType()
                qualifierDescription = bioqual[lista.get(idx).getBiologicalQualifierType()] if qualifierType \
                                                else modqual[lista.get(idx).getModelQualifierType()]
                speciesAnnotationDict[qualifierDescription].append(resource)
        return speciesAnnotationDict

    def buildAnnotationDict(self, document):
        annotationDict = defaultdict(lambda: defaultdict(list))
        speciesNameDict = {}
        for species in document.getModel().getListOfSpecies():
            annotation = species.getAnnotation()
            transformedName = self.standardizeName(species.getName())
            speciesNameDict[transformedName] = species.getName()
            speciesNameDict[species.getName()] = transformedName
            if annotation:
                annotationDict[transformedName] = self.parseAnnotation(annotation)
        return annotationDict, speciesNameDict
        


    def updateFromParent(self, child, parent, annotationDict):
        for annotationLabel in annotationDict[parent]:
            if annotationLabel in ['BQB_IS_VERSION_OF','BQB_IS']:
                annotationDict[child]['BQB_IS_VERSION_OF'] =annotationDict[parent][annotationLabel]
            

    def updateFromChild(self, parent,child,annotationDict):
        for annotationLabel in annotationDict[child]:
            if annotationLabel in ['BQB_IS_VERSION_OF','BQB_IS']:
                annotationDict[parent]['BQB_HAS_VERSION'] =annotationDict[child][annotationLabel]

    def updateFromComplex(self, complexMolecule,sct,annotationDict,annotationToSpeciesDict):
        localSpeciesDict = {}
        unmatchedReactants = []
        unmatchedAnnotations = []
        for constituentElement in sct[complexMolecule][0]:
            flag = False
            if len(annotationDict[constituentElement]) > 0:
                for annotation in annotationDict[constituentElement]:
                    if annotation in ['BQB_IS_VERSION_OF','BQB_IS','BQB_HAS_VERSION']:
                        flag = True
                        for individualAnnotation in annotationDict[constituentElement][annotation]:
                            localSpeciesDict[individualAnnotation] = constituentElement
                            localSpeciesDict[constituentElement] = individualAnnotation
                if flag:
                    continue
                
            if constituentElement in annotationToSpeciesDict:
                localSpeciesDict[constituentElement] = annotationToSpeciesDict[constituentElement]
                localSpeciesDict[annotationToSpeciesDict[constituentElement]] = constituentElement
            else:
                unmatchedReactants.append(constituentElement)
                
        for annotationType in annotationDict[complexMolecule]:
            if annotationType in ['BQB_HAS_VERSION','BQB_HAS_PART']:
                for constituentAnnotation in annotationDict[complexMolecule][annotationType]:
                    if constituentAnnotation not in localSpeciesDict:
                        unmatchedAnnotations.append(constituentAnnotation)
        if len(set(unmatchedReactants)) == 1 and len(set(unmatchedAnnotations)) == 1:
            localSpeciesDict[unmatchedReactants[0]] = unmatchedAnnotations[0]
            localSpeciesDict[unmatchedAnnotations[0]] = unmatchedReactants[0]
            annotationDict[unmatchedReactants[0]]['BQB_IS_VERSION_OF'] = [unmatchedAnnotations[0]]

        elif len(unmatchedReactants) > 0 or len(unmatchedAnnotations) > 0:
            #annotate from database names
            #print '**//',complexMolecule,unmatchedReactants,unmatchedAnnotations
            pass
            
        for element in localSpeciesDict:
            if element not in annotationToSpeciesDict:
                annotationToSpeciesDict[element] = localSpeciesDict[element]

    def updateFromComponents(self, complexMolecule,sct,annotationDict,annotationToSpeciesDict):
        localSpeciesDict = {}
        unmatchedReactants = []
        for constituentElement in sct[complexMolecule][0]:
            flag = False
            if len(annotationDict[constituentElement]) > 0:
                for annotation in annotationDict[constituentElement]:
                    if annotation in ['BQB_IS_VERSION_OF','BQB_IS','BQB_HAS_VERSION','BQB_HAS_PART']:
                        for individualAnnotation in annotationDict[constituentElement][annotation]:
                            #localSpeciesDict[individualAnnotation] = constituentElement
                            localSpeciesDict[constituentElement] = individualAnnotation
                            flag = True
            if not flag:
                unmatchedReactants.append(constituentElement) 
        for element in localSpeciesDict:
            annotationDict[complexMolecule]['BQB_HAS_PART'].append(localSpeciesDict[element])               


    def buildAnnotationTree(self, annotationDict,sct,database):
        annotationToSpeciesDict = {}
        for element in database.weights:
            if len(sct[element[0]]) > 0:
        
                if len(sct[element[0]][0]) == 1:
                    buildingBlock = sct[element[0]][0][0]
                    if len(annotationDict[element[0]]) == 0:
                        if len(annotationDict[buildingBlock]) > 0:
                            self.updateFromParent(element[0],buildingBlock,annotationDict)
                    if len(annotationDict[buildingBlock]) == 0:
                        if len(annotationDict[element[0]]) > 0:
                            self.updateFromChild(buildingBlock,element[0],annotationDict)
                elif len(sct[element[0]][0]) > 1:
                    if len(annotationDict[element[0]]) == 0:
                        self.updateFromComponents(element[0],sct,annotationDict,annotationToSpeciesDict)
                    else:
                        if 'BQB_HAS_VERSION' in annotationDict[element[0]] or 'BQB_HAS_PART' in annotationDict[element[0]]:
                            self.updateFromComplex(element[0],sct,annotationDict,annotationToSpeciesDict)
                        else:
                            self.updateFromComponents(element[0],sct,annotationDict,annotationToSpeciesDict)
                #        annotationdict[element[0]]
                #for buildingBlock in sct[element[0]][0]:
                #    print '\t',buildingBlock,annotationDict[buildingBlock]


    def obtainSCT(self, fileName, reactionDefinitions, useID, namingConventions,speciesEquivalences=None):
        '''
        one of the library's main entry methods. Process data from a file
        to obtain the species composition table, a dictionary describing
        the chemical history of different elements in the system
        '''
        reader = libsbml.SBMLReader()
        document = reader.readSBMLFromFile(fileName)

        parser = SBML2BNGL(document.getModel(),useID)
        database = structures.Databases()
        database.forceModificationFlag = True
        database = mc.createSpeciesCompositionGraph(parser, database, reactionDefinitions,namingConventions,
                           speciesEquivalences=speciesEquivalences,bioGridFlag=False)

        return database.prunnedDependencyGraph, database,document

    def getAnnotationSystem(self):

        annotationDict, speciesNameDict = self.buildAnnotationDict(self.sbmlDocument)
        self.buildAnnotationTree(annotationDict, self.sct, self.database)

        return annotationDict

    def getModelAnnotations(self):
        model = self.sbmlDocument.getModel()
        annotationXML = model.getAnnotation()
        lista = libsbml.CVTermList()
        libsbml.RDFAnnotationParser.parseRDFAnnotation(annotationXML, lista)
        modelAnnotations = []
        for idx in range(lista.getSize()):
            for idx2 in range(lista.get(idx).getResources().getLength()):
                if(lista.get(idx).getQualifierType()):
                    modelAnnotations.append([bioqual[lista.get(idx).getBiologicalQualifierType()],lista.get(idx).getResources().getValue(idx2)])
                else:
                    modelAnnotations.append([modqual[lista.get(idx).getModelQualifierType()],lista.get(idx).getResources().getValue(idx2)])
                    
        return modelAnnotations
    


def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i','--input-file',type=str,help='input SBML file',required=True)
    #parser.add_argument('-o','--output-file',type=str,help='output SBML file',required=True)
    return parser    

 
if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    #input_file = '/home/proto/workspace/bionetgen/parsers/SBMLparser/XMLExamples/curated/BIOMD%010i.xml' % 19
    annotationExtractor = AnnotationExtractor(namespace.input_file)
    annotationExtractor.getModelAnnotations()
    #elementalMolecules = [x for x in annotationExtractor.sct if annotationExtractor.sct[x] == []]
    #print {x:annotationExtractor.getAnnotationSystem()[x] for x in elementalMolecules}

