# 2013.10.10 11:42:06 EDT
#Embedded file name: ../utils/extractAtomic.py
"""
Created on Wed Aug  7 20:54:17 2013

@author: proto
"""
from collections import Counter

def extractMolecules(action, site1, site2, chemicalArray):
    """
    this method goes through the chemicals in a given array 'chemicalArray'
    and extracts its atomic patterns into two arrays:
        those elements that are contained in [site1,site2] will be put in the
        reactionCenter set. The rest will    be placed in the context set.
        The entire system will be put into the atomicPatterns dictionary
    Keyword arguments:
    site1,site2 -- where the action takes place
    chemicalArray -- the list of species we will be extracting atomic patters 
    from
    """
    atomicPatterns = {}
    reactionCenter = set()
    context = set()
    reactionCenterC = Counter()
    contextC = Counter()
    for reactant in chemicalArray:
        ta, tr, tc = reactant.extractAtomicPatterns(action, site1, site2)
        atomicPatterns.update(ta)
        for element in tr:
            reactionCenter.add(element)

        for element in tc:
            context.add(element)

        reactionCenterC.update(tr)
        contextC.update(tc)

    return (atomicPatterns, reactionCenterC, contextC)


def solveWildcards(atomicArray):
    """
    When you have a wildcard '+' as a bond configuration, this method allows you
    to go through the list of atomic elements and find which patterns the '+'
    can potentially resolve to
    """
    standinArray = {}
    for wildcard in [ x for x in atomicArray if '+' in x ]:
        for atomic in [ x for x in atomicArray if '+' not in x and len(atomicArray[x].molecules) > 1 ]:
            if atomicArray[wildcard].molecules[0].name in [ x.name for x in atomicArray[atomic].molecules ]:
                if wildcard not in standinArray:
                    standinArray[wildcard] = []
                standinArray[wildcard].append(atomicArray[atomic])

    atomicArray.update(standinArray)


def getMapping(mapp, site):
    for mapping in mapp:
        if site in mapping:
            return [ x for x in mapping if x != site ][0]


def extractTransformations(rules):
    """
    goes through the list of rules and extracts its reactioncenter,context and product
    atomic patterns per transformation action
    """
    atomicArray = {}
    transformationCenter = []
    transformationContext = []
    productElements = []
    actionName = []
    index = 0
    label = []
    for rule, _, reationRate, reactionSymbol in rules:
        index += 1
        for action in rule.actions:
            atomic, reactionCenter, context = extractMolecules(action.action, action.site1, action.site2, rule.reactants)
            transformationCenter.append(reactionCenter)
            transformationContext.append(context)
            atomicArray.update(atomic)
            productSites = [getMapping(rule.mapping, action.site1), getMapping(rule.mapping, action.site2)]
            atomic, rc, _ = extractMolecules(action.action, productSites[0], productSites[1], rule.products)
            productElements.append(rc)
            atomicArray.update(atomic)
            actionName.append('%i-%s' % (index, action.action)) 
            r = '+'.join([ str(x) for x in rule.reactants ])
            p = '+'.join([ str(x) for x in rule.products ])
            label.append('->'.join([r, p, '%i-%s' % (index, action.action)]))

    solveWildcards(atomicArray)
    return (atomicArray,
     transformationCenter,
     transformationContext,
     productElements,
     actionName,
     label)

