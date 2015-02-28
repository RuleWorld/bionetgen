# -*- coding: utf-8 -*-
"""
Created on Thu Dec 11 18:40:26 2014

@author: proto
"""
import sys
sys.path.insert(0, '../utils/')

import readBNGXML
import consoleCommands as console


def calculateSpeciesInformationContent(speciesList):
    for species in speciesList:
        print str(species)

def getInformationContent(fileName):
    console.bngl2xml(fileName,timeout=30)
    xmlFileName = '.'.join(fileName.split('.')[:-1])+'.xml'
    xmlFileName =xmlFileName.split('/')[-1]
    species,reactions,_= readBNGXML.parseXML(xmlFileName)
    calculateSpeciesInformationContent(species)
    
def getGraphMeasures(fileName):
    contactmapName = console.generateGraph(fileName,'contactmap')
    processName = console.generateGraph(fileName,'process')
    

def main():
    fileName = 'complex/output{0}.bngl'.format(19)
    getInformationContent(fileName)

if __name__ == "__main__":
    main()