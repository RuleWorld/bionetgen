# -*- coding: utf-8 -*-
"""
Created on Tue May 21 12:38:21 2013

@author: proto
"""

import libsbml2bngl as ls2b
import argparse



def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i','--input-file',nargs=1,type=str,help='input SBML file')
    parser.add_argument('-o','--output-file',nargs=1,type=str,help='output SBML file')
    parser.add_argument('-c','--convention-file',nargs=1,type=str,help='Conventions file')
    parser.add_argument('-u','--user-structures',nargs=1,type=str,help='User defined species')
    parser.add_argument('-id','--molecule-id',action='store_true')
    return parser    

def checkInput(namespace):
    
    options = {}
    options['inputFile'] = namespace.input_file[0]
    
    conv,useID = ls2b.selectReactionDefinitions(options['inputFile'])    
    options['outputFile'] = namespace.output_file if namespace.output_file != None else options['inputFile'] + '.bngl'
    options['conventionFile'] = namespace.convention_file if namespace.convention_file != None else conv
    options['userStructure'] = namespace.user_structures
    options['useId'] = namespace.molecule_id
    return options

def main():
    parser = defineConsole()
    namespace = parser.parse_args('-i XMLExamples/curated/BIOMD0000000001.xml'.split())
    print namespace

    options = checkInput(namespace)

    inputFile = namespace.input_file
    outputFile = namespace.output_file
    conventionFile = namespace.convention_file
    userStructure = namespace.user_structures    
    useId = namespace.molecule_id
    
    
        
    ls2b.analyzeFile(options['inputFile'],options['conventionFile'],options['useId'],outputFile,userStructure)
    
    
    print namespace
    print parser.print_help()
    

if __name__ == "__main__":
    main()
