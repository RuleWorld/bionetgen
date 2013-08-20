# -*- coding: utf-8 -*-
"""
Created on Wed Aug  7 21:04:16 2013

@author: proto
"""

import sys
sys.path.insert(0, '../utils/')

import consoleCommands as console
import extractAtomic
import readBNGXML

if __name__ == "__main__":
    console.bngl2xml('complex/output19.bngl')
    species,rules= readBNGXML.parseXML('output19.xml')
    #print rules
    
    
    atomicArray =[]
    transformationCenter = []
    transformationContext = []
    productElements = []
    actionNames = []
    labelArray = []
    for idx,rule in enumerate(rules):
        tatomicArray, ttransformationCenter, ttransformationContext, \
                tproductElements,tactionNames,tlabelArray = extractAtomic.extractTransformations([rule])
        #atomicArray.append(tatomicArray)
        transformationCenter.append(ttransformationCenter)
        transformationContext.append(ttransformationContext)
    centerDict = {}
    
    #extract rules with teh same reaction center
    for idx,rule in enumerate(transformationCenter):
        for element in rule:
            if element != set([]):
                if tuple(element) not in centerDict:
                    centerDict[tuple(element)] = set([])
                centerDict[tuple(element)].add(idx)
    #extract the context of such reactions
    for element in centerDict:
        tmp= []
        if len(centerDict[element]) == 1:
            pass
        else:
            for index in centerDict[element]:
               tmp.append(transformationContext[index])
            tmp2 = tmp[0][0]
            tmp3 = tmp[0][0]
            for ttmp in tmp:
                    for x in ttmp:
                        tmp2 = tmp2.intersection(x)
                        tmp3 = tmp3.union(x)
            #print '###',element
            #print '+++',tmp2
            #print '---',tmp3.difference(tmp2)
    redundantDict = {}
    #group those reactions with the same reaction center and reaction rate
    #TODO: add same actions as a restriction
    for element in centerDict:
        tmpDict = {}
        for rnum in centerDict[element]:
            actionString = rules[rnum][0].actions
            key =  [x.action for x in actionString]
            key.sort()
            key.append(rules[rnum][-2])
            if tuple(key) not in tmpDict:
                tmpDict[tuple(key)] = []
            tmpDict[tuple(key)].append(rnum)
        for element2 in tmpDict:
            if len(tmpDict[element2]) > 1:
                redundantDict['{0}.{1}'.format(element,element2)] = tmpDict[element2]
    #print redundantDict
    
    redundantListDict = {}
    for element in redundantDict:
        tmp = []
        tmp = [transformationContext[x] for x in redundantDict[element]]
        #pair together by traqnsformation the context of all those rules whose reaction center, rate
        #and transformations are the same
        tmp = zip(*tmp)

        #print '@@@',tmp
        intersections = []     
        unions = []
        
        for ttmp in tmp:
            intersections.append(set.intersection(*ttmp))
            unions.append(set.union(*ttmp))
        differences = []
        #obtain the union -intersection per transformation
        for x,y in zip(intersections,unions):
            differences.append(set.difference(y,x))
        #obtain those elements that were found to be constantly different across all intersections
        constantDifferences = set.intersection(*differences)
        redundantListDict[element] = constantDifferences
    #todo: remove redundancies from rules
    
    for reactionCenter in redundantDict:
        print redundantDict[reactionCenter]