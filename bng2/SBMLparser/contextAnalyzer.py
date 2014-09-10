# -*- coding: utf-8 -*-
"""
Created on Wed Aug  7 21:04:16 2013

@author: proto
"""

import pygraphviz as pgv

import sys
sys.path.insert(0, '../utils/')
import operator
import consoleCommands as console
from smallStructures import readFromString
import extractAtomic
import readBNGXML
from collections import Counter
from copy import deepcopy
import numpy as np


def readFile(fileName):
    f = open(fileName)
    r = f.readlines()
    return r

def getPatternDict(patternList):
    '''
    goes through a list of atomic patterns and orders them in a dictionary according to the molecule/
    component pair they contain. This means that molecules with the same molecule/component pair but 
    different number of bonds or state configuration will be grouped together    
    '''
    patternDict = {}
    extendedList = list(patternList.elements())
    stExtendList = [readFromString(x) for x in extendedList]
    
    for species in stExtendList:
        for molecule in species.molecules:
            if molecule.str3() not in patternDict:
                patternDict[molecule.str3()] = []
            patternDict[molecule.str3()].append(str(species))
            
    return patternDict
    
def matchElements(moleculeSet1,moleculeSet2):
    '''
    create a distance matrix set1 vs set2 and choose elements from different rows/columns that minimize 
    distance
    '''
    
    distanceMatrix = [[0 for x in range(0,len(moleculeSet2))] for x in range(0,len(moleculeSet1))]

    for idx,reference in enumerate(moleculeSet1):
        for idx2,comparison in enumerate(moleculeSet2):
            distanceMatrix[idx][idx2] = reference[1].distance(comparison[1])
    elements = []
    for row in distanceMatrix:
        while row.index(min(row)) in elements:
            row[row.index(min(row))] = 100000
        elements.append(row.index(min(row)))
    finalArray = []
    for idx,element in enumerate(elements):
        finalArray.append((moleculeSet1[idx],moleculeSet2[element]))
    return finalArray

def getMetaElement(matchedArray):
    for element in matchedArray:
        element[0][1].compare(element[1][1])

def createMetaRule(ruleSet,differences):
    '''
    Creates a metaRule from an array 'ruleSet' of rules. The differences parameter contains a dictionary
    elaborating on how the rules are different
    '''
    moleculeDict = []
    for ruleDescription in ruleSet:
        #todo:i have to find the way to group together equivalent
        #molecules from different rules and find the metarule
        molList = {}
        for reactant in ruleDescription[0].reactants:
            for key in differences:
                for molecule in reactant.molecules:
                    if molecule.name + '(' in key:
                        for component in molecule.components:
                            if '(' + component.name + ')' in key:
                                #print molecule.name,component.name,key
                                if key not in molList:
                                    molList[key] = []
                                molList[key].append([reactant,molecule,component])
        moleculeDict.append(molList)
        for reactant in ruleDescription[0].products:
            for key in differences:
                for molecule in reactant.molecules:
                    if molecule.name + '(' in key:
                        for component in molecule.components:
                            if '(' + component.name + ')' in key:
                                #print molecule.name,component.name,key
                                if key not in molList:
                                    molList[key] = []
                                molList[key].append([reactant,molecule,component])
        moleculeDict.append(molList)

    metaRule = moleculeDict[0]
    matchedArray = {}
    for idx in range(1,len(moleculeDict)):
        
        for element in metaRule:
            if element in moleculeDict[idx]:
                matchedArray = matchElements(metaRule[element],moleculeDict[idx][element])
                getMetaElement(matchedArray)
            #print metaRule[element],moleculeDict[idx][element]

def groupByReactionCenter(transformationCenter):
    centerDict = {}
    #extract rules with teh same reaction center
    for idx,rule in enumerate(transformationCenter):
        #print rule
        element = [tuple(x.elements()) for x in rule if len(x) > 0]
        #print element
        #for element in rule:
        #    if element != set([]):
        if tuple(element) not in centerDict:
            centerDict[tuple(element)] = set([])
        centerDict[tuple(element)].add(idx)
    return centerDict
  

def groupByReactionCenterAndRateAndActions(rules,centerDict):
    '''
    group those reactions with the same reaction center on the first level
    and reaction rate/ actions on the second level
    '''
    redundantDict = {}
    for center in centerDict:
        tmpDict = {}
        for rnum in centerDict[center]:
            actionString = rules[rnum][0].actions
            key =  [x.action for x in actionString]
            key.sort()
            #append the rate to the tuple to create a key
            key.append(rules[rnum][-1])
            if tuple(key) not in tmpDict:
                tmpDict[tuple(key)] = []
            tmpDict[tuple(key)].append(rnum)
        for rate in tmpDict:
            if len(tmpDict[rate]) > 1:
                if center not in redundantDict:
                    redundantDict[center] = {}
                redundantDict[center][rate] = tmpDict[rate]
    return redundantDict


def obtainDifferences(redundantDict,transformationContext):
    '''
    analize what is different between similar rules (same rc, rate and actions) throw unions 
    and intersections of           their context
    '''
    redundantListDict = {}
    for center in redundantDict:
        for rate in redundantDict[center]:
            tmp = []
            tmp = [transformationContext[x] for x in redundantDict[center][rate]]
            #if center == (('Prot(iMod~U)',), ('Prot(egfr)', 'EGFR(prot)')):
            #    print '--',tmp

            #pair together by traqnsformation the context of all those rules whose reaction center, rate
            #and transformations are the same
            tmp = zip(*tmp)
            #if center == (('Prot(iMod~U)',), ('Prot(egfr)', 'EGFR(prot)')):
            #    print '--------------------------------'
            #    print '--',tmp
            #print '@@@',tmp
            intersections = []     
            unions = []
            for ttmp in tmp:
                tint = ttmp[0]
                tun = ttmp[0]
                for idx in range(1,len(ttmp)):
                    tint &= ttmp[idx]
                    tun |= ttmp[idx]
                intersections.append(tint)
                unions.append(tun)
            differences = []
            #obtain the union -intersection per transformation
            for x,y in zip(intersections,unions):
                differences.append(y-x)
            #obtain those elements that were found to be constantly different across all intersections
            constantDifferences = differences[0]
            for idx in range(1,len(differences)):
                constantDifferences &= differences[idx]
            if center not in redundantListDict:
                redundantListDict[center] = {}
            redundantListDict[center][rate] = constantDifferences
    return redundantListDict


def reactionCenterGraph(species,reactionCenter):
    total = sum(x[1] for x in reactionCenter)
    graph = pgv.AGraph(directed=False,concentrate=True)
    print reactionCenter,
    for element in species:
        graph.add_node(element.name,shape='diamond',style='filled')
        for component in element.components:
            pass            

    
    
def extractStatistics():
    number = 151
    console.bngl2xml('complex/output{0}.bngl'.format(number))
    species,rules,parameterDict= readBNGXML.parseXML('output{0}.xml'.format(number))
    #print rules
    
    
    transformationCenter = []
    transformationContext = []

    k = []
    actions = Counter()
    actionSet = Counter()
    for idx,rule in enumerate(rules):
        tatomicArray, ttransformationCenter, ttransformationContext, \
                tproductElements,tactionNames,tlabelArray = extractAtomic.extractTransformations([rule])
    #atomicArray.append(tatomicArray)
        transformationCenter.append(ttransformationCenter)
        transformationContext.append(ttransformationContext)
        k.append(len(rule[0].actions))
        #if len(rule[0].actions) > 3:
        #    print rule[0].reactants
        actions.update([x.action for x in rule[0].actions])
        tmp = [x.action for x in rule[0].actions]
        tmp.sort()
        actionSet.update([tuple(tmp)])
    
    
    #print actions
    #print actionSet
    print 'number of species',len(species)
    print 'avg number o actions',np.average(k),np.std(k)
    centerDict = groupByReactionCenter(transformationCenter)
    print 'singletons',len({x:centerDict[x] for x in centerDict if len(centerDict[x]) == 1})
    tmp = [[tuple(set(x)),len(centerDict[x])] for x in centerDict]
    #reactionCenterGraph(species,tmp)
    #tmp.sort(key=lambda x:x[1],reverse=True)
    print 'number of reaction centers',len(centerDict.keys())
    print 'number of rules',len(rules)
    
    #print 'unique',[x for x in centerDict[element] if len(centerDict[element]) == 1]
    redundantDict = groupByReactionCenterAndRateAndActions(rules,centerDict)
    #print redundantDict
    tmp2 = [('$\\tt{{{0}}}$'.format(tuple(set(x))),tuple(set(y[:-1])),y[-1],len (redundantDict[x][y])) for x in redundantDict for y in redundantDict[x] if 'kr' not in y[-1]]
    tmp2 = set(tmp2)
    tmp2 = list(tmp2)
    tmp2.sort(key=lambda x:x[3],reverse=True)
    tmp2.sort(key=lambda x:x[0],reverse=True)

    tmp2 = ['{0} & {1} & {2} & {3}\\\\\n'.format(element[0],element[1],element[2],element[3]) for element in tmp2]
    
    with open('table.tex','w') as f:
        f.write('\\begin{tabular}{|cccc|}\n')
        f.write('\\hline\n')
        f.write('Reaction Centers & Action & Score\\\\\\hline\n')
        for element in tmp2:
            f.write(element)
        f.write('\\hline\n')
        f.write('\\end{tabular}\n')

    #print redundantDict
    x = [len(centerDict[x]) for x in centerDict]    
    #122,138
    print 'average number of reactions with same rate and reaction cneter',np.average(x),np.std(x) 
    print 'total number of clusters',len(x)
    print x
        #redundantDict['{0}.{1}'.format(element,element2)] = tmpDict[element2]
 
def findNewParameters(parameters,bngParameters):
    '''
    finds which parameters were added in the xml definition that were not in the original bngl file 
    '''
    tmp = [par.strip().split(' ')[0] for par in parameters]
    newPar = []    
    for bngp in bngParameters:
        if bngp not in tmp:
            newPar.append('\t {0} {1}\n'.format(bngp,bngParameters[bngp]))
    return newPar

def extractRedundantContext(rules,transformationCenter,transformationContext):
    ''''
    
    '''
    centerDict = groupByReactionCenter(transformationCenter)
    redundantDict = groupByReactionCenterAndRateAndActions(rules,centerDict)
                #redundantDict['{0}.{1}'.format(element,element2)] = tmpDict[element2]
    redundantListDict = obtainDifferences(redundantDict,transformationContext)
    #todo: remove redundancies from rules
    #group together equivalent patterns
    patternDictList = {}
    for center in redundantListDict:
        for rate in redundantListDict[center]:
            patternDict = getPatternDict(redundantListDict[center][rate])
            if center not in patternDictList:
                patternDictList[center] = {}
            patternDictList[center][rate] = patternDict
    return redundantDict,patternDictList

def main():
    fileName = 'complex/output19.bngl'
    console.bngl2xml(fileName)
    species,rules,par= readBNGXML.parseXML('output19.xml')
    #print rules
    
    
    transformationCenter = []
    transformationContext = []

    #extract the context of such reactions
    for idx,rule in enumerate(rules):
        tatomicArray, ttransformationCenter, ttransformationContext, \
                tproductElements,tactionNames,tlabelArray = extractAtomic.extractTransformations([rule])
    #atomicArray.append(tatomicArray)
        transformationCenter.append(ttransformationCenter)
        transformationContext.append(ttransformationContext)
    redundantDict,patternDictList = extractRedundantContext(rules,transformationCenter,transformationContext)
    #print redundantDict
    #construct rule based patterns based on the reaction patterns they have to match and
    #the ones they  have to discriminate
    
    for center in patternDictList:
        for rate in patternDictList[center]:
            match = patternDictList[center][rate]
            notContext = []
            for cRate in [x for x in patternDictList[center] if x != rate]:
                notContext.append([transformationContext[x] for x in redundantDict[center][cRate]])
            ruleSet = [rules[x] for x in redundantDict[center][rate]]
            createMetaRule(ruleSet,match)
    
    
    newRules = range(0,len(rules))
    for center in redundantDict:
        for context in redundantDict[center]:
            for element in range(1,len(redundantDict[center][context])):
                newRules.remove(redundantDict[center][context][element])
    
    #for element in newRules:
    #    print str(rules[element][0])
       
    newRulesArray = []
    for element in newRules:
        newRulesArray.append('{0}\n'.format(str(rules[element][0])))
    lines = readFile(fileName)
    startR = lines.index('begin reaction rules\n')
    endR = lines.index('end reaction rules\n')
    startP = lines.index('begin parameters\n')
    endP = lines.index('end parameters\n')
    
    newPar = findNewParameters(lines[startP+1:endP],par)
    newLines = lines[0:endP] + newPar + lines[endP:startR+1] + newRulesArray + lines[endR:len(lines)]
    
    f = open(fileName + 'reduced.bngl','w')
    f.writelines(newLines)
    '''
    for reactionCenter in redundantDict:
        candidateRules = [rules[idx][0] for idx in redundantDict[reactionCenter]]
        print '+++',redundantListDict[reactionCenter]
        print '---',candidateRules
    '''
    
if __name__ == "__main__":
    #extractStatistics()
    main()