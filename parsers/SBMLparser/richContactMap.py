# -*- coding: utf-8 -*-
"""
Created on Sun Oct 13 20:10:48 2013

@author: proto
"""

import sys
import contextAnalyzer
import subprocess
from collections import Counter
sys.path.insert(0, '../utils/')
import consoleCommands as console
import readBNGXML
import extractAtomic
import pygraphviz as pgv
import progressbar
import glob
import os
import shutil
import csv
import radarChart
import matplotlib.patches as mpatches
sys.path.insert(0, '../ContactMap')
import createGraph
from copy import copy, deepcopy
import scipy.stats
from scipy import linalg


def createRuleBiPartite(rule, transformationCenter, transformationContext,
                        transformationProduct, actionNames, atomicArray, fileName, redundantPattern):
    graph = pgv.AGraph(directed=True, strict=True, ranksep='2.5', compound='true',
                       rankdir='LR')

    gReactants = graph.subgraph(
        name='clusterReactants', label='Chemicals', rankdir='TB')
    gRules = graph.subgraph(
        name='clusterRules', label='Transformations', rankdir='TB', ranksetp='2.5 ')
    gProducts = graph.subgraph(
        name='clusterProducts', label='Products', rankdir='TB')

    gReactants.node_attr.update(shape='square')
    gProducts.node_attr.update(shape='square')

    edgeIdx = 0
    reactantDictionary = {}
    productDictionary = {}
    edgeIdx = 1
    reactantbuiltlist = {}
    productbuildlist = {}

    transformationRange = range(0, len(transformationCenter))
    skipArray = []

    for element in transformationRange:
        options = [{'style': 'filled', 'color': 'pink'}
                   for x in range(0, len(transformationCenter[element - 1]))]
        if len(options) == 0:
            skipArray.append(element)
            continue
        edgeArray = createGraph.createSubGraph(transformationCenter[element - 1],
                                               gReactants, reactantDictionary, reactantbuiltlist, atomicArray,
                                               gReactants, edgeIdx, 'clusterr', options)
        for edge in edgeArray:
            graph.add_edge(edge + '_dummy', actionNames[element - 1],
                           key='key%i' % edgeIdx, ltail=edge)
            edgeIdx += 1

    for idx, element in enumerate(transformationRange):
        if element in skipArray:
            continue
        tmp = copy(transformationContext[element - 1])
        # if(len(redundantPattern) > 0):
        tmp.subtract(redundantPattern)

        negativeCounts = Counter(el for el in tmp.elements() if tmp[el] <= 0)
        importantContext = Counter(el for el in tmp.elements() if tmp[el] > 0)
        redundantContext = Counter(el for el in redundantPattern.elements(
        ) if el not in negativeCounts and el in transformationContext[element - 1])

        options = [{'style': 'filled', 'color': 'green'}
                   for x in range(0, len(transformationContext[element - 1]))]
        options2 = [{'style': 'filled', 'color': 'cyan'}
                    for x in range(0, len(transformationContext[element - 1]))]

        edgeArray = createGraph.createSubGraph(importantContext,
                                               gReactants, reactantDictionary, reactantbuiltlist, atomicArray,
                                               gReactants, edgeIdx, 'clusterr', options)
        edgeArray2 = createGraph.createSubGraph(redundantContext,
                                                gReactants, reactantDictionary, reactantbuiltlist, atomicArray,
                                                gReactants, edgeIdx, 'clusterr', options2)

        edgeArray.extend(edgeArray2)
        for edge in edgeArray:
            graph.add_edge(edge + '_dummy', actionNames[element - 1],
                           key='key%i' % edgeIdx, ltail=edge, color='red')
            edgeIdx += 1
    for element in transformationRange:
        if element in skipArray:
            continue
        options = [{'style': 'filled', 'color': 'pink'}
                   for x in range(0, len(transformationProduct[element - 1]))]
        edgeArray = createGraph.createSubGraph(transformationProduct[element - 1],
                                               gProducts, productDictionary, productbuildlist, atomicArray,
                                               gProducts, edgeIdx, 'clusterp', options)
        for edge in edgeArray:
            graph.add_edge(actionNames[element - 1], edge + '_dummy',
                           key='key%i' % edgeIdx, lhead=edge)
            edgeIdx += 1

    graph.write('%s.dot' % fileName)
    subprocess.call(
        ['dot', '-Tsvg', '{0}.dot'.format(fileName), '-o{0}.svg'.format(fileName)])
    subprocess.call(
        ['dot', '-Tpng', '{0}.dot'.format(fileName), '-o{0}.png'.format(fileName)])


def reverseLookup(redundantDict):
    '''
    grab a dictioanry of reaction centers and associate them by reaction instead
    '''
    reverseDictionary = {}
    for center in redundantDict:
        for action in redundantDict[center]:
            for reaction in redundantDict[center][action]:
                if reaction not in reverseDictionary:
                    reverseDictionary[reaction] = []
                reverseDictionary[reaction].append([center, action])
    return reverseDictionary


def dictionaryToCounter(redundantPattern):
    tmp = Counter()
    for element in redundantPattern:
        for key in element:
            tmp.update(element[key])
    return tmp


def extractCenterContext(rules):
    transformationCenter = []
    transformationContext = []
    transformationProduct = []
    atomicArray = []
    actionNames = []
    for idx, rule in enumerate(rules):
        tatomicArray, ttransformationCenter, ttransformationContext, \
            tproductElements, tactionNames, tlabelArray = extractAtomic.extractTransformations(
                [rule])

        transformationCenter.append(ttransformationCenter)
        transformationContext.append(ttransformationContext)
        actionNames.append(tactionNames)
        atomicArray.append(tatomicArray)
        transformationProduct.append(tproductElements)
    return transformationCenter, transformationContext, \
        transformationProduct, atomicArray, actionNames


def createBipartiteGraph(fileName):

    rules = extractRulesfromBNGL(fileName)

    transformationCenter, transformationContext, \
        transformationProduct, atomicArray, actionNames = extractCenterContext(
            rules)

    redundantDict, patternDictList = contextAnalyzer.extractRedundantContext(
        rules, transformationCenter, transformationContext)
    reverseRedundantDict = reverseLookup(redundantDict)

    idx = 0
    for rule, center, context, product, action, atom in zip(rules, transformationCenter, transformationContext, transformationProduct, actionNames, atomicArray):
        redundantPattern = []
        if idx in reverseRedundantDict:
            for key in reverseRedundantDict[idx]:
                redundantPattern.append(patternDictList[key[0]][key[1]])
        redundantPatternCounter = dictionaryToCounter(redundantPattern)
        createRuleBiPartite(rule, center, context, product, action,
                            atom, 'img/bipartite{0}'.format(idx), redundantPatternCounter)
        idx += 1


def matchName(rmolecule, rcomponent, componentList):

    for component in componentList:
        if rmolecule in component and rcomponent in component:
            return True
    return False
    # for idx,rule in enumerate(rules):


def createMap(rules, center, redundantContext, fileName):
    graph = pgv.AGraph(directed=True, strict=True, ranksep='2.5', compound='true',
                       rankdir='LR')
    subgraphs = {}
    center = [[x.site1, x.site2] for x in rules[0].actions]
    center = set([y for x in center for y in x if y != None])
    newCenter = []
    for element in rules[0].mapping:
        if element[0] in center:
            newCenter.append(element[1])
    if len(newCenter) == 0:
        newCenter = center
    redundantKeys = list(redundantContext.elements())
    for product in rules[0].products:
        for molecule in product.molecules:
            if molecule not in subgraphs:
                newSubgraph = graph.subgraph(name='cluster_{0}'.format(molecule.idx),
                                             label='{0}'.format(molecule.name),
                                             rankdir='TB', shape='square', style='rounded',
                                             pendwidth=2)
                subgraphs[molecule.idx] = newSubgraph
            else:
                newSubgraph = subgraphs[molecule.idx]
            for component in molecule.components:
                if component.idx in newCenter:

                    color = 'red'
                elif matchName(molecule.name, component.name, redundantKeys):
                    color = 'green'
                else:
                    color = 'blue'
                newSubgraph.add_node(
                    component.idx, label=component.name, color=color, penwidth=2, shape='circle', fontcolor=color)
        for bond in product.bonds:
            graph.add_edge(
                bond[0], bond[1], dir='none', len=0.1, weight=100, penwidth=2)

    graph.write('%s.dot' % fileName)
    subprocess.call(
        ['dot', '-Tsvg', '{0}.dot'.format(fileName), '-o{0}.svg'.format(fileName)])


def createContactMap(fileName):
    rules = extractRulesfromBNGL(fileName)
    transformationCenter = []
    transformationContext = []
    transformationProduct = []
    actionNames = []
    atomicArray = []

    for idx, rule in enumerate(rules):
        tatomicArray, ttransformationCenter, ttransformationContext, \
            tproductElements, tactionNames, tlabelArray = extractAtomic.extractTransformations(
                [rule])

        transformationCenter.append(ttransformationCenter)
        transformationContext.append(ttransformationContext)
        actionNames.append(tactionNames)
        atomicArray.append(tatomicArray)
        transformationProduct.append(tproductElements)

    #tatomicArray, ttransformationCenter, ttransformationContext, \
    #            tproductElements,tactionNames,tlabelArray = extractAtomic.extractTransformations(rules)
    redundantDict, patternDictList = contextAnalyzer.extractRedundantContext(
        rules, transformationCenter, transformationContext)
    reverseRedundantDict = reverseLookup(redundantDict)

    for idx in range(0, len(rules)):
        redundantPattern = []
        if idx in reverseRedundantDict:
            for key in reverseRedundantDict[idx]:
                redundantPattern.append(patternDictList[key[0]][key[1]])
        redundantPatternCounter = dictionaryToCounter(redundantPattern)

        createMap(rules[idx], transformationProduct[
                  idx], redundantPatternCounter, 'conImg/contatMap{0}'.format(idx))


def constructHistogram(data, fileName, legend, weights, normed=True):
    import matplotlib.pyplot as plt
    plt.clf()
    plt.hist(data, weights=weights, normed=normed)
    plt.xlabel(legend, fontsize=18)
    plt.savefig('{0}.png'.format(fileName))


def analyzeSpace(centerString, molecules):
    # spaceCoveredHelper()
    totalSpace = 1
    excludeMolecules = []
    operators = ['Add', 'Delete', 'AddBond',
                 'DeleteBond', 'StateChange', 'ChangeCompartment']
    try:
        if 'AddBond' in centerString or 'StateChange' in centerString:
            for centerCandidate in [x for x in centerString if x not in operators]:
                if len(centerCandidate) >= 2:
                    moleculeName = [x.split('(')[0] for x in centerCandidate]
                    moleculeInstance = [
                        x for x in molecules if x.name in moleculeName]
                    if len(moleculeInstance) == 1:  # symmetry
                        moleculeInstance.append(moleculeInstance[0])
                    totalSpace *= spaceCoveredHelper(
                        moleculeInstance[0], '', [moleculeInstance[1]], [], molecules)
                    totalSpace *= spaceCoveredHelper(moleculeInstance[1], '',
                                                     [moleculeInstance[0]], [], molecules)

                if len(centerCandidate) == 1:
                    moleculeName = [x.split('(')[0] for x in centerCandidate]
                    moleculeInstance = [
                        x for x in molecules if x.name in moleculeName]
                    totalSpace *= spaceCoveredHelper(moleculeInstance[0], '',
                                                     [], [], molecules) / 2

        if 'DeleteBond' in centerString:
            for moleculeString in [x for x in centerString if x not in operators]:
                if len(moleculeString) == 1 and '.' in moleculeString[0]:
                    moleculeName = [x.split('(')[0]
                                    for x in moleculeString[0].split('.')]
                    moleculeInstance = [
                        x for x in molecules if x.name in moleculeName]
                    if len(moleculeInstance) == 1:  # symmetry
                        moleculeInstance.append(moleculeInstance[0])
                    totalSpace *= spaceCoveredHelper(
                        moleculeInstance[0], '', [moleculeName[1]], [], molecules)
                    totalSpace *= spaceCoveredHelper(moleculeInstance[1], '',
                                                     [moleculeName[0]], [], molecules)
    except CycleError:
        return -1
    return totalSpace


def cooperativityAnalysis2(fileName):
    species, rules, par = extractRulesfromBNGL(fileName)
    transformationCenter, transformationContext, \
        transformationProduct, atomicArray, actionNames = extractCenterContext(
            rules)
    # group by reaction center
    redundantDict, patternDictList = contextAnalyzer.extractRedundantContext(
        rules, transformationCenter, transformationContext)
    totalProcesses = 0
    groupedProcesses = 0
    redundantProcesses = 0
    reverseRedundantDict = reverseLookup(redundantDict)
    totalSpaceCovered = []
    both = hetero = homo = 0
    for center in redundantDict:
        processCount = []
        reactionInCenter = len([y for x in redundantDict[center]
                                for y in redundantDict[center][x]])
        if reactionInCenter > 1 and len([x for x in center if x not in ['Add', 'Delete']]) > 1:
            totalSpace = analyzeSpace(center, species)
            totalSpaceCovered.append((totalSpace, reactionInCenter))

        for rateKey in redundantDict[center]:
            if len([x for x in center if x not in ['Add', 'Delete']]) > 1:
                processCount.append(len(redundantDict[center][rateKey]))

            totalProcesses += len(redundantDict[center][rateKey])

        if len(processCount) > 1 and any([x > 1 for x in processCount]):
            both += sum(processCount)
        elif len(processCount) > 1:
            hetero += sum(processCount)
        elif sum(processCount) > 1:
            homo += sum(processCount)
    #totalhetero = hetero*1.0/totalProcesses if totalProcesses > 0 else 0
    #totalhomo = homo*1.0/totalProcesses if totalProcesses > 0 else 0
    #totalboth = both*1.0/totalProcesses if totalProcesses > 0 else 0
    #totalall = (hetero+homo+both)*1.0/totalProcesses if totalProcesses > 0 else 0
    return hetero, homo, both, totalProcesses, totalSpaceCovered


def extractRulesfromBNGL(fileName):
    # console.bngl2xml('complex/{0}.bngl'.format(fileName),timeout=10)
    species, rules, par = readBNGXML.parseXML(fileName)
    return species, rules, par


import numpy as np


import matplotlib.pyplot as plt
import cPickle as pickle


def plotresults(results, yvals, fileName, legend=''):
    plt.clf()
    plt.plot(results, yvals)
    plt.axis([0, 1, 0, 1])
    plt.xlabel(legend, fontsize=18)
    plt.savefig('{0}.png'.format(fileName))


class CycleError(Exception):

    def __init__(self, moleculeName, visitedMolecules):
        self.moleculeName = moleculeName
        self.visitedMolecules = visitedMolecules

    def __str__(self):
        return repr(self.moleculeName) + ' ' + repr(self.visitedMolecules)


def spaceCovered(molecules, observablesLen):

    # print [str(x) for x in molecules]
    speciesCount = 0
    excludedMolecules = []
    for individualMolecule in molecules:
        # print '++++'
        visitedMolecules = []
        tmp = spaceCoveredHelper(
            individualMolecule, '', excludedMolecules, visitedMolecules, molecules)
        # print
        # '--',individualMolecule.name,visitedMolecules,tmp,excludedMolecules
        excludedMolecules.append(individualMolecule.name)

        speciesCount += tmp
    return min(observablesLen * 1.0 / speciesCount, 1.0)


def spaceCoveredHelper(moleculeInstance, comeFromMolecule, excludedMolecules,
                       visitedMolecules, molecules):
    multiplier = 1
    for component in moleculeInstance.components:
        if len(component.states) > 0:
            # account for state complexity
            multiplier *= len(component.states)
        else:
            # account for binding complexity
            for molecule in molecules:
                # for all molecules which match the component name
                if molecule.name.lower() == component.name:
                    # skip those molecules we have considered already in previous complexity interations
                    # todo: this may be skipping over other complexity considerations
                    # eg exmol:[b] A-B is not the same as A-C-B
                    if molecule.name not in excludedMolecules:
                        # exclude molecules we already visited in this session or
                        # ask if theres more than one way to connect to the
                        # root molecule
                        if len([x for x in visitedMolecules if x == moleculeInstance.name]) > 3:
                            raise CycleError(molecule.name, visitedMolecules)
                        else:
                            tmp = copy(visitedMolecules)
                            tmp.append(moleculeInstance.name)
                            newMolecule = deepcopy(molecule)
                            for x in newMolecule.components:
                                if x.name == moleculeInstance.name.lower():
                                    newMolecule.components.remove(x)
                                    break
                            multiplier *= 2 * spaceCoveredHelper(newMolecule,
                                                                 moleculeInstance.name, excludedMolecules, tmp, molecules)
                        # the only way this will go on is if we are checking for
                        # dimerization otherwise crash.
                        # elif molecule.name != comeFromMolecule:
                        #    raise CycleError(molecule.name,comeFromMolecule)
                        # if we already passed through the same points more than 3 times
                        # elif len([x for x in visitedMolecules if x == moleculeInstance.name]) > 3:
                        #    raise CycleError(molecule.name,visitedMolecules)
                    else:
                        multiplier += 0
    return multiplier


def getReactionTypeProperties(reactions):
    totalActions = 0
    actionCounter = []
    for reaction in reactions:
        #if len(reaction[0].actions) ==2 and 'Add' in reaction[0].actions and \
        #    'Delete' in reaction[0].actions:
        #        actionCounter.append('Add/Delete')
        if '0' in  [str(x) for x in reaction[0].reactants] or '0' in \
                [str(x) for x in reaction[0].products]:
            continue

        for action in reaction[0].actions:
            
            if action.action in ['AddBond', 'DeleteBond']:
                actionCounter.append('AddBond')
            elif action.action in ['Add', 'Delete']:
                actionCounter.append('Add')
            elif action.action in ['StateChange']:
                actionCounter.append('StateChange')
            elif action.action in ['ChangeCompartment']:
                actionCounter.append('ChangeCompartment')
            else:
                print action
                raise Exception
        #actionCounter.extend([x.action for x in reaction[0].actions])
    actionCounter = Counter(actionCounter)
    return actionCounter


def partial_corr(C):
    """
    Returns the sample linear partial correlation coefficients between pairs of variables in C, controlling 
    for the remaining variables in C.


    Parameters
    ----------
    C : array-like, shape (n, p)
        Array with the different variables. Each column of C is taken as a variable


    Returns
    -------
    P : array-like, shape (p, p)
        P[i, j] contains the partial correlation of C[:, i] and C[:, j] controlling
        for the remaining variables in C.
    """

    C = np.asarray(C)
    p = C.shape[1]
    P_corr = np.zeros((p, p), dtype=np.float)
    for i in range(p):
        P_corr[i, i] = 1
        for j in range(i + 1, p):
            idx = np.ones(p, dtype=np.bool)
            idx[i] = False
            idx[j] = False
            beta_i = linalg.lstsq(C[:, idx], C[:, j])[0]
            beta_j = linalg.lstsq(C[:, idx], C[:, i])[0]

            res_j = C[:, j] - C[:, idx].dot(beta_i)
            res_i = C[:, i] - C[:, idx].dot(beta_j)

            corr = scipy.stats.pearsonr(res_i, res_j)[0]
            P_corr[i, j] = corr
            P_corr[j, i] = corr

    return P_corr


def modelCompositionCDF(directory):
    '''
    '''
    with open('spaceCovered.dump', 'rb') as f:
        spaceCoveredArray = pickle.load(f)
    index, atomization, observables, space, actionCounter = zip(
        *spaceCoveredArray)
    data = {
        'labels':['Add','AddBond',\
                'StateChange']
            }
    
    generalCounter = defaultdict(lambda : defaultdict(list))
    counter = 0
    for iindex,modelSpace,atom,observable,model in zip(index,space,atomization,observables,actionCounter):
        totalActions = len([x for x in model.elements()])
        counter += 1
        for label in data['labels']:
            
            #generalCounter[label].append(model[label]*1.0/totalActions)
            if modelSpace < 0.4:
                generalCounter['l4'][label].append(model[label]*1.0/totalActions)
            elif modelSpace >=0.4 and modelSpace < 1:
                generalCounter['l10'][label].append(model[label]*1.0/totalActions)
            elif modelSpace == 1:
                generalCounter['Z10'][label].append(model[label]*1.0/totalActions)
        if modelSpace == 1:
            print iindex,atom,
            print generalCounter['Z10']['Add'][-1]
    print counter
    data['Mean process ratio'] = []
    for counter in generalCounter:
        data['Mean process ratio'].append([np.mean(generalCounter[counter][x]) for x in generalCounter[counter]])
    data['labels'] = [x for x in generalCounter['l4']]
    spoke_labels = data.pop('labels')
    print spoke_labels
    N = len(spoke_labels)
    theta = radarChart.radar_factory(N, frame='polygon')



    fig = plt.figure(figsize=(9, 9))
    fig.subplots_adjust(hspace=0, top=0.85, bottom=0.05)

    colors = ['b', 'r', 'g', 'm', 'y']
    # Plot the four cases from the example data on separate axes
    for n, title in enumerate(data.keys()):
        ax = fig.add_subplot(1, 1, n+1, projection='radar')
        plt.rgrids([0.2, 0.4, 0.6, 0.8])
        ax.set_title(title, weight='bold', size=20, position=(0.5, 1.1),
                     horizontalalignment='center', verticalalignment='center')
        for d, color in zip(data[title], colors):
            ax.plot(theta, d, color=color)
            ax.fill(theta, d, facecolor=color, alpha=0.25)
        ax.set_varlabels(spoke_labels)

    
    plt.subplot(1, 1, 1)
    labels = ('space coverage < 0.4','space coverage < 1', 'space coverage = 1')
    legend = plt.legend(labels, loc=(0.65, .85), labelspacing=0.1)
    plt.setp(legend.get_texts(), fontsize=17)
    plt.savefig('actionspaceratio.png')
    #plt.figtext(0.5, 0.965, '5-Factor Solution Profiles Across Four Scenarios',
    #            ha='center', color='black', weight='bold', size='large')
    

def spaceCoveredCDF(directory):

    spaceCoveredArray = []
    atomizationDict = {}
    with open(os.path.join(directory, 'sortedD.dump'), 'rb') as f:
        atomizationStats = pickle.load(f)
    for element in atomizationStats:
        try:
            atomizationDict[element['index']] = element['atomization']
        except:
            continue
    progress = progressbar.ProgressBar()
    cycles = []

    # generateBNGXML(directory)
    errors = []
    skippedModels = []
    with open('ratomizationp2m.dump', 'rb') as f:
        ratomizationDict = pickle.load(f)
    '''
    for element in progress(range(1,549)):
        try:
            if element in atomizationDict and atomizationDict[element] > 0:
                #console.bngl2xml('complex/BIOMD%010i.xml.bngl' %element,timeout=10)
                #print ratomizationDict['complex2/BIOMD%010i.xml.xml' %element],atomizationDict[element]
                species,reactions,_= readBNGXML.parseXML(os.path.join(directory,
                                            'BIOMD%010i.xml.xml' %element))
                actionCounter = getReactionTypeProperties(reactions)
                observablesLen = readBNGXML.getNumObservablesXML(os.path.join(directory,
                                                'BIOMD%010i.xml.xml' %element))
                try:
                    spaceCoveredArray.append([element,atomizationDict[element],observablesLen,spaceCovered(species,observablesLen),actionCounter])
                    #print element,spaceCoveredArray[-1]
                except CycleError as e:
                    #print element,-1,e
                    cycles.append(element)
                    continue
                    #spaceCoveredArray.append([atomizationDict[element],0])
            else:
                skippedModels.append(element)
                
        except (IOError,IndexError):
            errors.append(element)
            continue
    print 'molecule cycles encountered',len(cycles), cycles
    print 'errors encountered',len(errors),errors
    print 'skipped elements',len(skippedModels),skippedModels
    #return
    with open('spaceCovered.dump','wb') as f:
        pickle.dump(spaceCoveredArray,f)
    '''
    with open('spaceCovered.dump', 'rb') as f:
        spaceCoveredArray = pickle.load(f)
    index, atomization, observables, space, actionCounter = zip(
        *spaceCoveredArray)
    print 'models to analyze', len(spaceCoveredArray)
    # print spaceCoveredArray[0:10]
    addAction = []
    modifyAction = []
    paddAction = []
    pmodifyAction = []

    for element in actionCounter:
        totalActions = len([x for x in element.elements()])
        atomizedActions = len(
            [x for x in element.elements() if x in ['AddBond', 'StateChange']])
        addAction.append(element['AddBond'] * 1.0 / totalActions)
        modifyAction.append(element['StateChange'] * 1.0 / totalActions)
        if atomizedActions > 0:
            paddAction.append(element['AddBond'] * 1.0 / atomizedActions)
            pmodifyAction.append(
                element['StateChange'] * 1.0 / atomizedActions)
        else:
            paddAction.append(0)
            pmodifyAction.append(0)
    # for i,a,o,s,ac in spaceCoveredArray:
    #    if s > 0.5:
    #        print i,a,o,s,ac
    #heatmap, xedges, yedges = np.histogram2d(space, atomization, bins=8)
    #extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]

    # building 2.5plots
    '''
    plt.clf()
    cm = plt.cm.get_cmap('YlOrRd')
    cax= plt.scatter(addAction, observables, s=40, 
                c=space,cmap=cm)
    plt.yscale('log')
    plt.xlabel('Fraction of AddBond/DeleteBond over total number of actions',fontsize=16)
    plt.ylabel('Model size (species)',fontsize=18)
    cb = plt.colorbar(cax)
    cb.set_label('Coverage')
    #plt.xlim([-0.01,1.01])
    #plt.ylim([-0.01,1.01])
    plt.savefig('addbondvssizecoverage.png')

    plt.clf()
    cm = plt.cm.get_cmap('YlOrRd')
    cax= plt.scatter(paddAction, observables, s=40, 
                c=space,cmap=cm)
    plt.yscale('log')
    plt.xlabel('Fraction of AddBond/DeleteBond over atomized actions',fontsize=16)
    plt.ylabel('Model size (species)',fontsize=18)
    cb = plt.colorbar(cax)
    cb.set_label('Coverage')
    #plt.xlim([-0.01,1.01])
    #plt.ylim([-0.01,1.01])
    plt.savefig('partial_addbondvssizecoverage.png')


    plt.clf()
    #plt.imshow(heatmap, extent=extent)
    cm = plt.cm.get_cmap('YlOrRd')
    cax= plt.scatter(atomization, observables, s=40, 
                c=space,cmap=cm)
    plt.yscale('log')


    plt.xlabel('atomization',fontsize=18)
    plt.ylabel('model size (species)',fontsize=18)
    cb = plt.colorbar(cax)
    cb.set_label('Coverage')
    

    #plt.show()  
    
    plt.savefig('atomizationvsspeciesCov.png')
    '''

    # build histogram
    plt.clf()
    plt.hist(space)
    plt.xlabel('Space covered', fontsize=24)
    plt.ylabel('Number of models', fontsize=24)
    plt.savefig('spaceHistogram.png')
    # simple scatter plots
    plt.clf()
    cax = plt.scatter(observables, space, s=40)
    plt.xscale('log')
    plt.xlabel('Model Size (species)', fontsize=18)
    plt.ylabel('Coverage', fontsize=18)
    plt.savefig('speciesvscoverage.png')

    plt.clf()
    cax = plt.scatter(atomization, space, s=40)
    plt.xlabel('Atomization', fontsize=18)
    plt.ylabel('Coverage', fontsize=18)
    plt.savefig('atomizationvscoverage.png')

    plt.clf()
    cax = plt.scatter(addAction, space, s=40)
    plt.xlabel('Ratio of AddBond actions', fontsize=18)
    plt.ylabel('Coverage', fontsize=18)
    plt.savefig('addbondvscoverage.png')

    # now building heatmaps
    heatmap, xedges, yedges = np.histogram2d(
        space, np.log10(observables), bins=12)
    heatmap = np.log2(heatmap)
    extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
    heatmap[heatmap < 0] = 0
    plt.clf()
    cm = plt.cm.get_cmap('YlOrRd')

    plt.imshow(heatmap, extent=extent, aspect='auto',
               origin='lower', interpolation='nearest', cmap=cm)
    plt.xlabel('log10(Model size (species))', fontsize=18)
    plt.ylabel('Space coverage', fontsize=18)
    cb = plt.colorbar()
    cb.set_label('Number of models', fontsize=18)
    # cb.set_ticklabels(['0',''])
    # plt.show()
    plt.savefig('sizevscoveragemap.png')

    heatmap, xedges, yedges = np.histogram2d(space, atomization, bins=12)
    #heatmap = np.log2(heatmap)
    extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
    heatmap[heatmap < 0] = 0
    plt.clf()
    cm = plt.cm.get_cmap('YlOrRd')

    plt.imshow(heatmap, extent=extent, aspect='auto',
               origin='lower', interpolation='nearest', cmap=cm)
    plt.xlabel('Atomization', fontsize=18)
    plt.ylabel('Space coverage', fontsize=18)
    cb = plt.colorbar()
    cb.set_label('Number of models', fontsize=18)
    # cb.set_ticklabels(['0',''])
    # plt.show()
    plt.savefig('atomizationvscoveragemap.png')

    heatmap, xedges, yedges = np.histogram2d(space, addAction, bins=12)
    heatmap = np.log10(heatmap)
    extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
    heatmap[heatmap < 0] = 0
    plt.clf()
    cm = plt.cm.get_cmap('YlOrRd')

    plt.imshow(heatmap, extent=extent, aspect='auto',
               origin='lower', interpolation='nearest', cmap=cm)
    plt.xlabel('Fraction of AddBond reactions', fontsize=18)
    plt.ylabel('Space coverage', fontsize=18)
    cb = plt.colorbar()
    cb.set_label('log10(Number of models)', fontsize=18)
    # cb.set_ticklabels(['0',''])
    # plt.show()
    plt.savefig('addbondvscoveragemap.png')
    nobservables = [min(1, x * 1.0 / 50) for x in observables]

    tmp = np.asarray([nobservables, addAction, atomization, space])
    tmp = np.transpose(tmp)
    p_corr = partial_corr(tmp)
    print p_corr
    print 'observable-space correlation', scipy.stats.pearsonr(nobservables, space)
    print 'atomization-space correlation', scipy.stats.pearsonr(atomization, space)
    print 'addBond-space correlation', scipy.stats.pearsonr(addAction, space)
    print '--- only those with addbonds'

    tobservables, tatom, taddAction, tspacep = zip(
        *[(w, z, x, y) for w, z, x, y in zip(nobservables, atomization, addAction, space) if x > 0])
    print len(tobservables)
    print 'observable-space correlation', scipy.stats.pearsonr(tobservables, tspacep)
    print 'atomization-space correlation', scipy.stats.pearsonr(tatom, tspacep)
    print 'addBond-space correlation', scipy.stats.pearsonr(taddAction, tspacep)
    print 'space stats, mean,std',np.mean(tspacep),np.std(tspacep)
    tmp = np.asarray([tobservables, taddAction, tatom, tspacep])
    tmp = np.transpose(tmp)
    p_corr = partial_corr(tmp)
    print 'partial correlation'
    print p_corr

    plt.clf()
    plt.hist(tspacep)
    plt.xlabel('Space covered ({0} models)'.format(len(tspacep)), fontsize=18)
    plt.ylabel('Number of models', fontsize=18)
    plt.savefig('spaceHistogramAddBond.png')
    #spaceCoveredArray=np.sort( spaceCoveredArray )
    # yvals=np.arange(len(spaceCoveredArray))/float(len(spaceCoveredArray))
    # plotresults(spaceCoveredArray,yvals,'spaceCovered')


def reactionBasedAtomization(reactions):
    atomizedProcesses = 0
    trueProcessesCounter = 0
    for reaction in reactions:
        if '0' in  [str(x) for x in reaction[0].reactants] or '0' in \
                [str(x) for x in reaction[0].products]:
            continue
        trueProcessesCounter += 1
        #if len([x for x in action.action if x in ['Add','Delete']]) == 0:
        #    atomizedProcesses +=1
        
        for action in reaction[0].actions:
            if action.action in ['AddBond', 'DeleteBond', 'StateChange', 'ChangeCompartment']:
                atomizedProcesses += 1
                break
        
    if trueProcessesCounter == 0:
        return 0, 0
    return atomizedProcesses, trueProcessesCounter

from collections import defaultdict
generalSignature = defaultdict(list)


def stoichiometryAnalysis(reactions):
    ato = []
    nonato = []
    for reaction in reactions:
        if '0' in  [str(x) for x in reaction[0].reactants] or '0' in \
                [str(x) for x in reaction[0].products]:
            continue
        signature = ((len(reaction[0].reactants), len(reaction[0].products)))
        if len([x for x in reaction[0].actions if x.action in ['AddBond', 'DeleteBond', 'StateChange']]) > 0:
            ato.append(signature)
        else:
            generalSignature[signature].append(reaction)
            nonato.append(signature)
    return ato, nonato


import fnmatch


def getValidFiles(directory, extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append(os.path.join(root, filename))
    return matches


def reactionBasedAtomizationFile(xml):
    ratomizationList = []
    ratomizationDict = defaultdict(dict)
    ratomizationListm10 = []
    ratomizationListl10 = []
    largeUseless = []
    syndelArray = []
    syndel = 0
    totalRatomizedProcesses = 0
    totalReactions = 0
    totalProcesses = 0
    validFiles = 0
    atomizedDistro = []
    nonAtomizedDistro = []
    interesting = []

    try:
            # console.bngl2xml('complex/output{0}.bngl'.format(element),timeout=10)
        try:

            _, rules, _ = readBNGXML.parseXML(xml)
        except:
            print xml
        atomizedProcesses, weight = reactionBasedAtomization(rules)
        ato, nonato = stoichiometryAnalysis(rules)
        atomizedDistro.extend(ato)
        nonAtomizedDistro.extend(nonato)
        # if (2,1) in nonato:
        #    interesting.append(element)
        score = atomizedProcesses * 1.0 / weight if weight != 0 else 0
        totalRatomizedProcesses += atomizedProcesses
        totalReactions += len(rules)
        totalProcesses += weight
        ratomizationDict[xml]['score'] = score
        ratomizationDict[xml]['weight'] = weight
        ratomizationDict[xml]['length'] = len(rules)
        if len(rules) == 0:
            ruleslen0 += 1
        syndelArray.append((len(rules) - weight) * 1.0 / len(rules))
        if score == -1:
            syndel += 1
            # ratomizationList.append([0,0,len(rules)])
        ratomizationList.append([score, weight, len(rules)])
        if len(rules) > 10:
            if weight * 1.0 / len(rules) >= 0.1 and score < 0.1:
                largeUseless.append(xml)
            ratomizationListm10.append([score, weight, len(rules)])
        else:
            ratomizationListl10.append([score, weight, len(rules)])
        # print xml,ratomizationList[-1]
        validFiles += 1
    # except (IndexError,ZeroDivisionError):
    #    syndel += 1
    #    print 'iz'
    #    continue
    except IOError:
        print 'io'
    return ratomizationDict

import os.path


def generateBNGXML(directory):

    bnglFiles = getValidFiles(directory, 'bngl')
    print 'converting {0} bnglfiles'.format(len(bnglFiles))
    progress = progressbar.ProgressBar()
    for i in progress(range(len(bnglFiles))):
        xmlName = '.'.join(bnglFiles[i].split('.')[:-1]) + '.xml'
        print xmlName

        if os.path.exists(xmlName):
            continue
        console.bngl2xml(bnglFiles[i], timeout=10)

    print 'moving xml files'
    files = glob.iglob(os.path.join('.', "*.xml"))
    for xmlfile in files:
        if os.path.isfile(xmlfile):
            shutil.move(xmlfile, directory)


from collections import defaultdict


def reactionBasedAtomizationDistro(directory):
    '''
    calculates a rection atomization based metric:
    ration of atomized reactions (non syndeg) in a model
    '''
    ratomizationList = []
    ratomizationDict = defaultdict(dict)
    ratomizationListm10 = []
    ratomizationListl10 = []
    largeUseless = []
    syndelArray = []
    syndel = 0
    totalRatomizedProcesses = 0
    totalReactions = 0
    totalProcesses = 0
    validFiles = 0
    atomizedDistro = []
    nonAtomizedDistro = []
    interesting = []

    # generate bng-xml
    # generateBNGXML(directory)

    print 'reading bng-xml files'
    xmlFiles = getValidFiles(directory, 'xml')

    print 'analyzing {0} bng-xml files'.format(len(xmlFiles))
    progress = progressbar.ProgressBar()
    ruleslen0 = 0
    for i in progress(range(len(xmlFiles))):

        xml = xmlFiles[i]
    # for xml in xmlFiles:
        try:
            # console.bngl2xml('complex/output{0}.bngl'.format(element),timeout=10)
            try:

                _, rules, _ = readBNGXML.parseXML(xml)
            except IOError:
                print xml
                continue
            atomizedProcesses, weight = reactionBasedAtomization(rules)
            ato, nonato = stoichiometryAnalysis(rules)
            atomizedDistro.extend(ato)
            nonAtomizedDistro.extend(nonato)
            # if (2,1) in nonato:
            #    interesting.append(element)
            score = atomizedProcesses * 1.0 / weight if weight != 0 else 0
            totalRatomizedProcesses += atomizedProcesses
            totalReactions += len(rules)
            totalProcesses += weight
            ratomizationDict[xml]['score'] = score
            ratomizationDict[xml]['weight'] = weight
            ratomizationDict[xml]['length'] = len(rules)
            if len(rules) == 0:
                ruleslen0 += 1
                continue
            syndelArray.append((len(rules) - weight) * 1.0 / len(rules))
            if score == -1:
                syndel += 1
                # ratomizationList.append([0,0,len(rules)])
                continue
            ratomizationList.append([score, weight, len(rules)])
            if len(rules) > 10:
                if weight * 1.0 / len(rules) >= 0.1 and score < 0.1:
                    largeUseless.append(xml)
                ratomizationListm10.append([score, weight, len(rules)])
            else:
                ratomizationListl10.append([score, weight, len(rules)])
            # print xml,ratomizationList[-1]
            validFiles += 1
        # except (IndexError,ZeroDivisionError):
        #    syndel += 1
        #    print 'iz'
        #    continue
        except IOError:
            print 'io'
            continue
    with open('ratomizationp2m.dump', 'wb') as f:
        pickle.dump(ratomizationDict, f)

    print '{0} models with 0 rules'.format(ruleslen0)
    print 'generating figures for {0} models'.format(len(ratomizationList))
    print '-----'
    print 'atomized', Counter(atomizedDistro)
    print 'nonatomized', Counter(nonAtomizedDistro)
    print 'models with 2->1 non atomized reactions', interesting
    with open('nonatomizedreactions.dump', 'wb') as f:
        pickle.dump(generalSignature, f)
    ratomization, weights, length = zip(*ratomizationList)

    ratomizationm10, weightsm10, lengthm10 = zip(*ratomizationListm10)
    ratomizationl10, weightsl10, lengthl10 = zip(*ratomizationListl10)

    constructHistogram(syndelArray, 'syndelHist', 'Fraction of synthesis and degradation reactions', np.ones(
        len(syndelArray)), normed=False)
    plt.clf()
    plt.hist(ratomization)
    plt.xlabel('Reaction atomization level',fontsize=18)
    plt.ylabel('Number of models',fontsize=18)
    plt.savefig('ratomizationHist.png')

    weights = np.array(weights)
    length = np.array(length)
    tmp = weights * 1.0 / length

    #ratomizationWP1 = [x for x,y in zip(ratomization,tmp) if y < 0.1]
    #ratomizationWP10 = [x for x,y,z in zip(ratomization,tmp,length) if y >= 0.1 and z<10]
    #ratomizationWP11 = [x for x,y,z in zip(ratomization,tmp,length) if y >= 0.1 and z>=10]

    #constructHistogram(ratomizationWP1,'ratomizationWP1','Reaction atomization level',np.ones(len(ratomizationWP1)),normed=False)
    #constructHistogram(ratomizationWP10,'ratomizationWP10','Reaction atomization level',np.ones(len(ratomizationWP10)),normed=False)
    #constructHistogram(ratomizationWP11,'ratomizationWP11','Reaction atomization level',np.ones(len(ratomizationWP11)),normed=False)

    print 'process={0}, rprocess={1}, reactions = {2},syndel={3},valid={4}'.format(totalProcesses,
                                                                                   totalRatomizedProcesses, totalReactions, syndel, validFiles)

    tmp2 = zip(ratomization, tmp)
    tmp2.sort(key=lambda x: x[0])

    # heatmap showing histogram of atomization vs non syn-deg
    heatmap, xedges, yedges = np.histogram2d(tmp, ratomization, bins=12)
    #heatmap = np.log2(heatmap)

    print 'correlation syndel-atomization', scipy.stats.pearsonr(tmp, ratomization)
    extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
    heatmap[heatmap < 0] = 0
    plt.clf()
    cm = plt.cm.get_cmap('YlOrRd')

    plt.imshow(heatmap, extent=extent, aspect='auto',
               origin='lower', interpolation='nearest', cmap=cm)
    plt.xlabel('Atomization level', fontsize=18)
    plt.ylabel('Percentage of non syn-del reactions', fontsize=18)
    cb = plt.colorbar()
    cb.set_label('Number of models', fontsize=18)
    # cb.set_ticklabels(['0',''])
    # plt.show()
    plt.savefig('atomizationvssyndelHeatmap.png')

    plt.clf()
    plt.scatter(tmp, ratomization)
    plt.ylabel('Atomization level', fontsize=24)
    plt.xlabel('Percentage of non syn=del reactions', fontsize=24)
    plt.savefig('atomizationScatterplot.png')

    # heatmap showing average atomization of %syn-def vs model size
    ratomizationHeatmapCounter = defaultdict(lambda: defaultdict(list))
    _, bin_edges = np.histogram(length, 4)
    digitizedLength = np.digitize(length, bin_edges)
    _, bin_edges2 = np.histogram(tmp, 4)
    digitizedSynDeg = np.digitize(tmp, bin_edges2)
    for rat, wei, leng in zip(ratomization, digitizedLength, digitizedSynDeg):
        ratomizationHeatmapCounter[wei][leng].append(rat)
    ratomizationHeatmap = np.zeros([5, 5])
    for xelement in ratomizationHeatmapCounter:
        for yelement in ratomizationHeatmapCounter[xelement]:

            ratomizationHeatmap[
                xelement - 1][yelement - 1] = np.mean(ratomizationHeatmapCounter[xelement][yelement])
            ratomizationHeatmapCounter[xelement][yelement] = np.mean(
                ratomizationHeatmapCounter[xelement][yelement])

    heatmap, xedges, yedges = np.histogram2d(length, tmp, bins=5)
    heatmap = np.log2(heatmap)

    plt.clf()
    plt.hist(length, bins=30 ** np.linspace(np.log10(1), np.log10(1000), 40))
    plt.xscale('log')
    plt.xlabel(
        'Number of reactions ({0} models)'.format(len(length)), fontsize=18)
    plt.savefig('numberOfReactionsHist.png')

    plt.clf()
    ax = plt.gca()
    ax.scatter(1 - tmp, [max(x, 0) for x in ratomization])
    plt.ylabel('Atomization', fontsize=24)
    plt.xlabel('Fraction of syndel reactions', fontsize=24)
    ax.grid(True)
    # ax.set_xscale('log')
    # ax.set_autoscale_on(False)
    plt.ylim([-0.1, 1.1])
    # ax.set_yscale('log')
    plt.savefig('atomizationvsnsyndel.png')

    plt.clf()
    plt.scatter(length, 1 - tmp, s=40,
                c=[max(x, 0) for x in ratomization])
    plt.xlabel('Number of reactions', fontsize=24)
    plt.ylabel('Fracion of syndel reactions', fontsize=24)

    ax = plt.gca()
    # ax.set_xscale('log')
    ax.grid(True)
    plt.xlim(xmin=0, xmax=100)
    plt.gray()
    cb = plt.colorbar()
    cb.set_label('Atomization level')

    plt.savefig('reactionsvssyndelwlinear.png')

    plt.clf()
    plt.imshow(ratomizationHeatmap,
               extent=extent, aspect='auto', origin='lower', interpolation='nearest')
    plt.xlabel('Fraction of non syn-del reactionsModel size')
    plt.ylabel('Model Size (reactions)')
    cb = plt.colorbar()
    cb.set_label('Atomization level')
    # plt.show()
    plt.savefig('atomizationHeatMap2.png')

    heatmap[heatmap < 0] = 0
    plt.clf()
    plt.imshow(heatmap, extent=extent, aspect='auto',
               origin='lower', interpolation='nearest')
    plt.ylabel('Model size (reactions)')
    plt.xlabel('Percentage of non syn-del reactions')
    cb = plt.colorbar()
    cb.set_label('log2(Number of models)')
    plt.show()
    plt.savefig('atomizationHeatMap3.png')

    plt.clf()
    plt.scatter(length, tmp)
    plt.xlabel('Number of reactions', fontsize=24)
    plt.xscale('log')
    plt.ylabel('Atomization level', fontsize=24)
    plt.savefig('scatterreactionsvslevel.png')

    ratomization = np.sort(ratomization)
    ratomizationm10 = np.sort(ratomizationm10)
    ratomizationl10 = np.sort(ratomizationl10)

    yvals = np.arange(len(ratomization)) / float(len(ratomization))
    plotresults(ratomization, yvals, 'ratomizationw',
                'Reaction atomization level ({0} models)'.format(len(ratomizationList)))
    yvals = np.arange(len(ratomizationm10)) / float(len(ratomizationm10))
    plotresults(ratomizationm10, yvals, 'ratomization_m10w',
                'Reaction atomization level >10 reactions({0} models)'.format(len(ratomizationListm10)))
    yvals = np.arange(len(ratomizationl10)) / float(len(ratomizationl10))
    plotresults(ratomizationl10, yvals, 'ratomization_l10w',
                'Reaction atomization level <=10 reactions({0} models)'.format(len(ratomizationListl10)))
    print 'syndel', syndel
    print '>10 with 0', largeUseless


def createGroupingCDF():
    '''
    analyzes a group ofatomized files with species atomization > 0
    and groups them according to whether they have groups of reaction with 
    same reaction center different context/reaction rates
    '''
    processList = []
    '''
    with open('ratomizationp2m.dump','rb') as f:
        atomizationStats = pickle.load(f)
    atomizationDict = {}
    atomizationList = []
    groupedDict = []
    spaceCoveredDict = {}
    for element in atomizationStats:
        try:
            print element,
            analysis = cooperativityAnalysis2(element)
            spaceCoveredDict[element] = analysis[-1]
            print analysis[-1]
            #grouping analysis
            processList.append(list(analysis[:-1]))
            atomizationList.append(atomizationStats[element])
            tmp = [element]
            tmp.extend(processList[-1])
            groupedDict.append(tmp)
        except (IOError,IndexError):
            print
            continue

      
    
    with open('groupedProcessList.dump','wb') as f:
        pickle.dump(processList,f)
    with open('spaceCovered.dump','wb') as f:
        pickle.dump(spaceCoveredDict,f)
    with open('groupedProcessList.dump','wb') as f:
        pickle.dump(groupedDict,f)

    processList = sorted(processList, key=lambda x: x[0])

    with open('groupedProcessList.csv','wb') as f:
        writer = csv.writer(f, delimiter=' ',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)
        writer.writerow(['file','heterogeneous','homogeneous','mixed','total-processes'])
        writer.writerows(processList)

    '''

    with open('groupedProcessList.dump', 'rb') as f:
        processList = pickle.load(f)

    _, groupedProcess, redundantProcess, mixedProcess, totalProcess = zip(
        *processList)

    processList = [x for x in processList if sum(x[1:4]) > 0]

    processList = [[float(y) / sum(x[1:4]) for y in x[1:4]]
                   for x in processList]
    print processList
    x = np.arange(len(processList))

    processList = sorted(
        processList, key=lambda x: (float(x[2]), float(x[1]), float(x[0])))
    processList = np.column_stack(processList)
    processList = [map(float, y) for y in processList]

    print processList
    # plt.clf()
    # plt.scatter(groupedProcess,atomizationList)
    # plt.show()
    # print groupedDict

    totalProcess = np.sort(totalProcess)
    #redundantProcess=np.sort( redundantProcess )
    yvals = np.arange(len(totalProcess)) / float(len(totalProcess))
    plotresults(totalProcess, yvals, 'totalGroups',
                'Ratio of reaction groups with more than one member')
    # plotresults(redundantProcess,yvals,'redundant>0')

    # this call to 'cumsum' (cumulative sum), passing in your y data,
    # is necessary to avoid having to manually order the datasets

    processList = np.array(processList)
    y_stack = np.cumsum(processList, axis=0)   # a 3x10 array
    fig = plt.figure()
    #ax1 = fig.add_subplot(111)

    plt.fill_between(x, 0, y_stack[0], facecolor="#CC6666", alpha=.7)
    plt.fill_between(x, y_stack[0], y_stack[1], facecolor="#1DACD6", alpha=.7)
    plt.fill_between(x, y_stack[1], y_stack[2], facecolor="#6E5160")
    hompatch = mpatches.Patch(color='#CC6666', label=' heteogeneous')
    hetpatch = mpatches.Patch(color='#1DACD6', label='homogeneous')
    bothpatch = mpatches.Patch(color='#6E5160', label='mixed')
    plt.legend(handles=[hompatch, hetpatch, bothpatch])

    plt.show()
    plt.savefig('stackplot.png')

import bioservices


def analyzeGroupingCDF():
    bio = bioservices.BioModels()

    with open('groupedProcessList.dump', 'rb') as f:
        groupedDict = pickle.load(f)
    elementList, groupedProcessList, redundantProcessList = zip(*groupedDict)
    hist, bin_edges = np.histogram(redundantProcessList, bins=5)
    binIndexArray = np.digitize(redundantProcessList, bin_edges)
    modelAnnotationBin = defaultdict(list)

    for element, binIndex in zip(elementList, binIndexArray):
        try:
            info = bio.getSimpleModelsByIds('BIOMD%010i' % element)
            parsedInfo = bioservices.Service('name').easyXML(info)
            modelName = stats.removeTags(str(parsedInfo['modelname'][0]))
            modelName = modelName.split(' - ')
            if len(modelName) > 1:
                modelName = modelName[1]
            else:
                modelName = modelName[0].split('_')
                if len(modelName) > 1:
                    modelName = ' '.join(modelName[1:])
                else:
                    modelName = modelName[0]
            print element, binIndex
            modelAnnotationBin[binIndex].append((element, modelName))
        except:
            print parsedInfo
            break
    print modelAnnotationBin
    print hist, bin_edges


def createSpaceDistribution():
    '''
    analyzes groups of files that have grouped reaction and creates a 
    distribution of how much space thse context-sensitive reactions cover
    '''
    with open('spaceCovered.dump', 'rb') as f:
        space = pickle.load(f)
    infiniteCycles = 0
    infiniteCyclesFiles = set([])
    spaceCovered = []
    groupSize = []
    modelsWithGroups = []
    for element in space:
        for entry in space[element]:
            if entry[0] == -1:
                infiniteCycles += 1
                infiniteCyclesFiles.add(element)
                continue
            if entry[1] > entry[0]:
                continue

            spaceCovered.append(entry[1] * 1.0 / entry[0])
            modelsWithGroups.append((element, spaceCovered[-1]))
            groupSize.append(entry[0])
    constructHistogram(spaceCovered, 'spaceCovered', 'Percentage of space covered by\
reaction groups', np.ones(len(spaceCovered)), False)

    # uncomment if we wish to obtain a list of which model belongs
    # to which group
    # analyzeSpaceDistribution(modelsWithGroups)

import bioservices
import stats


def analyzeSpaceDistribution(analyzedModels):
    bio = bioservices.BioModels()
    number, spaceCovered = zip(*analyzedModels)

    hist, bin_edges = np.histogram(spaceCovered, bins=5)
    binIndexArray = np.digitize(spaceCovered, bin_edges)
    modelAnnotationBin = defaultdict(list)

    for element, binIndex in zip(number, binIndexArray):
        try:
            info = bio.getSimpleModelsByIds('BIOMD%010i' % element)
            parsedInfo = bioservices.Service('name').easyXML(info)
            modelName = stats.removeTags(str(parsedInfo['modelname'][0]))
            modelName = modelName.split(' - ')
            if len(modelName) > 1:
                modelName = modelName[1]
            else:
                modelName = modelName[0].split('_')
                if len(modelName) > 1:
                    modelName = ' '.join(modelName[1:])
                else:
                    modelName = modelName[0]
            print element, binIndex
            modelAnnotationBin[binIndex].append(modelName)
        except:
            print parsedInfo
            break
    print modelAnnotationBin

import pprint


def nonAtomizedSpeciesAnalysis():
    with open('nonatomizedreactions.dump', 'rb') as f:
        nonAtomizedList = pickle.load(f)
    f = open('nonatomizedresults.txt', 'wt')
    for signature in nonAtomizedList:
        count = Counter()
        reactionList = defaultdict(list)
        f.write('--------\n')
        f.write(str(signature) + '\n')
        for reaction in nonAtomizedList[signature]:
            count[tuple(sorted([x.action for x in reaction[0].actions]))] += 1
            reactionList[tuple(sorted([x.action for x in reaction[0].actions]))].append(
                str(reaction[0]))
            if signature in [(2, 1), (1, 2), (1, 1)]:
                f.write(str(count) + '\n')
                f.write('+++\n')
                pprint.pprint(dict(reactionList), stream=f)
    f.close()
if __name__ == "__main__":
    spaceCoveredCDF('complex2')
    #modelCompositionCDF('complex2')
    #reactionBasedAtomizationDistro('complex2')
    # nonAtomizedSpeciesAnalysis()
    # createGroupingCDF()
    # print reactionBasedAtomizationFile('complex/BIOMD0000000019.xml.xml')
    # analyzeGroupingCDF()

    # createSpaceDistribution()
    # createBipartiteGraph('complex/output5.bngl')
    #species,rules = extractRulesfromBNGL('output19')

    # print processList
    # cooperativityAnalysis2('egfr_net')
    #createContactMap(  'output/output5.bngl')