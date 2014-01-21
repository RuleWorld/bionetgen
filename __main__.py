# -*- coding: utf-8 -*-
"""
Created on Tue May 21 12:38:21 2013

@author: proto
"""

import libsbml2bngl as ls2b
import argparse



def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-i','--input-file',type=str,help='input SBML file',required=True)
    
    parser.add_argument('-o','--output-file',type=str,help='output SBML file')
    parser.add_argument('-c','--convention-file',type=str,help='Conventions file')
    parser.add_argument('-u','--user-structures',type=str,help='User defined species')
    parser.add_argument('-id','--molecule-id',action='store_true',help='use SBML molecule ids instead of names. IDs are less descriptive \
    but more bngl friendly. Use only if the generated BNGL has syntactic errors')
    parser.add_argument('-a','--atomize',action='store_true',help='Infer molecular structure')
    parser.add_argument('-b','--biogrid',action='store_true',help='Use biogrid database to infer molecules')
    return parser    

def checkInput(namespace):
    
    options = {}
    options['inputFile'] = namespace.input_file
    
    conv,useID = ls2b.selectReactionDefinitions(options['inputFile'])    
    options['outputFile'] = namespace.output_file if namespace.output_file != None else options['inputFile'] + '.bngl'
    options['conventionFile'] = namespace.convention_file if namespace.convention_file != None else conv
    options['userStructure'] = namespace.user_structures
    options['useId'] = namespace.molecule_id
    options['atomize'] = namespace.atomize
    options['biogrid'] = namespace.biogrid
    return options

def main():
    parser = defineConsole()
    namespace = parser.parse_args()

    options = checkInput(namespace)
    print options    
    
    ls2b.analyzeFile(options['inputFile'],options['conventionFile'],options['useId'],
                     options['outputFile'],speciesEquivalence=options['userStructure'],
                     atomize=options['atomize'],bioGrid=options['biogrid'])
    
    
    

if __name__ == "__main__":
    main()
