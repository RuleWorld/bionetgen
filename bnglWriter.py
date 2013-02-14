#!/usr/bin/env python

import re
from copy import deepcopy
from util import logMess
import string

def bnglReaction(reactant,product,rate,tags,translator=[],isCompartments=False,reversible=True):
    finalString = ''
    #if translator != []:
    #    translator = balanceTranslator(reactant,product,translator)
    if len(reactant) == 0:
        finalString += 'Null() '
    for index in range(0,len(reactant)):
        tag = ''
        if reactant[index][0] in tags and isCompartments:
            tag = tags[reactant[index][0]]
        finalString += printTranslate(reactant[index],tag,translator)
        if index < len(reactant) -1:
            finalString += ' + '
    if reversible:
        finalString += ' <-> '
    else:
        finalString += ' -> '
    if len(product) == 0:
        finalString += 'Null() '
    for index in range(0,len(product)):
        tag = ''
        if product[index][0] in tags and isCompartments:
            tag = tags[product[index][0]]
        finalString +=  printTranslate(product[index],tag,translator) 
        if index < len(product) -1:
            finalString += ' + '
    finalString += ' ' + rate
    return finalString
    


def printTranslate(chemical,tags,translator={}):
    tmp = []
    if chemical[0] not in translator:
        app = chemical[0] + '()' + tags
    else:
        translator[chemical[0]].addCompartment(tags)
        app = str(translator[chemical[0]])
    if float(int(chemical[1])) == chemical[1]:
        for item in range(0,int(chemical[1])):
            tmp.append(app)
    else:
        idx = logMess("ERROR","Cannot deal with non integer stoicheometries: {0}* {1}".format(chemical[1],chemical[0]))
        tmp.append('ERROR CHECK LOG {0}'.format(idx))
    return ' + '.join(tmp)

def balanceTranslator(reactant,product,translator):
    rMolecules = []
    pMolecules = []
    newTranslator = {}
    for species in reactant:
        if species[0] in translator:
            newTranslator[species[0]] = deepcopy(translator[species[0]]) 
            rMolecules.extend(newTranslator[species[0]].molecules)
    for species in product:
        if species[0] in translator:
            newTranslator[species[0]] = deepcopy(translator[species[0]])
            pMolecules.extend(newTranslator[species[0]].molecules)
        
    for rMolecule in rMolecules:
        for pMolecule in pMolecules:
            if rMolecule.name == pMolecule.name:
                overFlowingComponents = [x for x in rMolecule.components if x.name not in [y .name for y in pMolecule.components]]
                overFlowingComponents.extend([x for x in pMolecule.components if x.name not in [y .name for y in rMolecule.components]])                
                rMolecule.removeComponents(overFlowingComponents)
                pMolecule.removeComponents(overFlowingComponents)
    return newTranslator


    
def bnglFunction(rule,functionTitle,compartments=[]):
    def powParse(match):
        if match.group(1) == 'root':
            exponent = '(1/%s)' % match.group(3)
        else:
            exponent = match.group(3)
        return '({0})^({1})'.format(match.group(2),exponent)
    
    def parameterRewrite(match):
        return 'p' + match.group(1)
    oldrule = ''
    while (('pow' in rule) or ('root' in rule)) and (oldrule != rule):
        oldrule = rule
        rule  = re.sub('(pow)\(([^,]+),([^)]+)\)',powParse,rule)
        rule  = re.sub('(root)\(([^,]+),([^)]+)\)',powParse,rule)
        if rule == oldrule:
            logMess('ERROR','Malformed pow or root function %s' % rule)
            print 'meep'
    if 'piecewise' in rule:
        logMess('BUG','We cannot deal with piecewise functions for the time being %s' %rule)
        return                

    if 'lambda' in rule:
        parameters =  rule[string.find(rule,'(')+1:-1].split(',')
        param = []
        for idx,element in enumerate(parameters[0:-1]):
            param.append('p' + element.strip())
            parameters[-1] = re.sub('(%s(\W|$))' % element.strip(),parameterRewrite,parameters[-1])
        param.append(parameters[-1])
        return '{0}({1}) = {2}'.format(functionTitle,','.join(param[0:-1]),param[-1])
        
    tmp = rule
    #delete the compartment from the rate function since cBNGL already does it
    for compartment in compartments:
        tmp = re.sub('^{0}\s*[*]'.format(compartment[0]),'',tmp)
        tmp = re.sub('([*]\s*{0})$'.format(compartment[0]),'',tmp)
        if compartment[0] in tmp:
            tmp =re.sub(r'(\W)({0})(\W)'.format(compartment[0]),r'\1 {0} \3'.format(str(compartment[1])),tmp)
            #tmp = re.sub(r'(\W)({0})(\W)'.format(compartment[0]),r'\1%s\3' % str(compartment[1]),tmp)
            logMess('WARNING','Exchanging reference to compartment %s for its dimensions' % compartment[0])
    #BNGL has ^ for power. 
    
    finalString = '%s = %s' % (functionTitle,tmp)
    return finalString

    
def finalText(param,molecules,species,observables,rules,functions,compartments,fileName):
    output = open(fileName,'w')
    output.write('begin model\n')
    output.write(sectionTemplate('parameters',param))
    output.write(sectionTemplate('compartments',compartments))          
    output.write(sectionTemplate('molecule types',molecules))
    output.write(sectionTemplate('seed species',species))
    output.write(sectionTemplate('observables',observables))
    output.write(sectionTemplate('functions',functions))
    output.write(sectionTemplate('reaction rules',rules))
    output.write('end model\n')
    output.write('generate_network();\n')
    output.write('simulate_ode({t_end=>100,n_steps=>100});')
    #output.write('writeXML()\n')
    
def sectionTemplate(name,content):
    section = 'begin %s\n' % name
    temp = ['\t%s\n' % line for line in content]
    section += ''.join(temp)
    section += 'end %s\n' % name
    return section

