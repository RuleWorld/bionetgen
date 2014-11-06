# -*- coding: utf-8 -*-
"""
Created on Tue Nov  4 14:21:07 2014

@author: proto
"""

import networkx
import sys
sys.path.insert(0, '../utils/')
import consoleCommands
import readBNGXML
import libsbgn
from datetime import datetime,timedelta

def openGML(fileName,graphType):
    #consoleCommands.generateGraph(fileName,graphType)
    fileName = '.'.join(fileName.split('.')[:-1]) + '_{0}.gml'.format(graphType)
    model =  networkx.read_gml(fileName)
    return model
    
def readXML(fileName):
    #consoleCommands.bngl2xml(fileName)
    fileName = '.'.join(fileName.split('.')[:-1]) + '.xml'
    return readBNGXML.parseXML(fileName)
    

if __name__ == "__main__":      
    #molecules,reaction,parameters =   readXML('BIOMD0000000019.xml.bngl')
    #print molecules
    contactMap = openGML('BIOMD0000000019.xml.bngl','contactmap')
    print '--',contactMap
    libsbgn.createSBNG_ER_gml(contactMap)
    #regulatory = openGML('BIOMD0000000019.xml.bngl','regulatory')
    #print model