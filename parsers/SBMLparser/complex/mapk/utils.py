# -*- coding: utf-8 -*-
"""
Created on Sat May  3 16:06:41 2014

@author: proto
"""

from os import listdir,getcwd
from os.path import isfile, join,dirname

import shutil
import subprocess

def updateDistribution():
    directory = '.'
    onlyfiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) and '.bngl' in f]
    #onlyNotFiles =[ f for f in listdir('./' + directory) if not isfile(join('./' + directory,f)) ]
    originalFiles = listdir('/home/proto/workspace/bionetgen/parsers/SBMLparser/complex')         
    for element in onlyfiles:
        if element in originalFiles:
            try:
                shutil.copy('/home/proto/workspace/bionetgen/parsers/SBMLparser/complex/' + element, '.')
            except:
                continue
def createGML():
    directory = '.'
    bngDistro = '/home/proto/workspace/bionetgen/bng2/Perl2/'
    onlybnglfiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) and '.bngl' in f]
    for bngl in onlybnglfiles:
        print dirname(__file__)
        subprocess.call(['perl',bngDistro +'visualizeBNGL.pl',getcwd()+ '/' + bngl],cwd=bngDistro)
    shutil.move(bngDistro + '*.gml','.')

def createGMLType(graphType):
    directory = '.'
    bngDistro = '/home/proto/workspace/bionetgen/bng2/Perl2/'
    onlybnglfiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) and '.bngl' in f]
    for bngl in onlybnglfiles:
        if '19' in bngl or '250' in bngl:
            continue
        print dirname(__file__)

        subprocess.call(['perl',bngDistro +'visualize.pl',getcwd()+ '/' + bngl,
            graphType,graphType],cwd=bngDistro)
    #shutil.move(bngDistro + '*.gml','.')


    
if __name__ == "__main__":
    #updateDistribution()
    #createGML()
    #createGMLType('rules')
    createGMLType('contact')
    #createGMLType('rules_eqn')

