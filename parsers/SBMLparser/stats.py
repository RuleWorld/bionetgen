# -*- coding: utf-8 -*-
"""
Created on Mon Oct  8 14:16:25 2012

@author: proto
"""
import numpy as np
import libsbml2bngl
import libsbml
import analyzeRDF

def main():
    history = np.load('stats3b.npy')
    history2 = np.load('stats3.npy')
    dictionary = {}
    
    for index in range (0,len(history)):
        flag = False
        if history[index] == []:
            continue
        else:
            for index2 in range(0,len(history2)):
                element = history2[index2]
                
                if element[0] == index+1:
                    if element[2] == 0.0:
                        flag = True
                        break
                    else:
                        break
                elif element[0] > index+1:
                    continue
            if flag:
                continue
            for molecule in history[index]:
                if molecule == []:
                    continue
                
                for uniprot in molecule[1][1]:
                    
                    if uniprot not in dictionary:
                        dictionary[uniprot] = []
                    if index+1 not in [x[0] for x in dictionary[uniprot]]:
                        dictionary[uniprot].append([index+1,molecule[0]])
    
    np.save('statsFinal.npy',np.array(dictionary))
    for element in dictionary:
        if len(dictionary[element]) > 1:
            print element,dictionary[element]
            
            
def main2():
    #336,365
    pair = [336,365]
    history = np.load('stats3.npy')
    extracts = []
    for element in history:
        if element[0] in pair:
            extracts.append(element)
        if len(extracts) == 2:
            break
    print extracts
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000336.xml')
    parser = libsbml2bngl.SBML2BNGL(document.getModel())    
    rdfAnnotations = analyzeRDF.getAnnotations(parser,'miriam')
    print rdfAnnotations
    
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD0000000365.xml')
    parser = libsbml2bngl.SBML2BNGL(document.getModel())    
    rdfAnnotations2 = analyzeRDF.getAnnotations(parser,'miriam')
    print rdfAnnotations2
    
    for element in rdfAnnotations:
        if element in rdfAnnotations2:
            print element,rdfAnnotations[element],rdfAnnotations2[element]
                        
            
          
if __name__ == "__main__":
    main2()