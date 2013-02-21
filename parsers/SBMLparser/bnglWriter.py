#!/usr/bin/env python

import re
from copy import deepcopy
from util import logMess
import string
from pyparsing import commaSeparatedList as csl
def evaluatePiecewiseFunction(function):
    pass

def bnglReaction(reactant,product,rate,tags,translator=[],isCompartments=False,reversible=True):
    finalString = ''
    #if translator != []:
    #    translator = balanceTranslator(reactant,product,translator)
    if len(reactant) == 0 or (len(reactant) == 1 and reactant[0][1] == 0):
        finalString += 'NullSpecies() '
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
        finalString += 'NullSpecies() '
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


def parsePieceWiseFunction(parameters):
    if len(parameters) == 3:
        return 'if({0},{1},{2})'.format(parameters[1],parameters[0],parameters[2])
    else:
        return 'if({0},{1},{2})'.format(parameters[1],parameters[0],parsePieceWiseFunction(parameters[2:]))
    
def bnglFunction(rule,functionTitle,compartments=[]):
    def powParse(match):
        if match.group(1) == 'root':
            exponent = '(1/%s)' % match.group(3)
        else:
            exponent = match.group(3)
        if match.group(1) in ['root','pow']:
            operator = '^'
        return '({0}){1}({2})'.format(match.group(2),operator,exponent)
    def compParse(match):

        translator = {'gt':'>','lt':'<','and':'AND','or':'OR','geq':'>=','leq':'<=','eq':'=='}
        exponent = match.group(3)
        operator = translator[match.group(1)]
        return '{0} {1} {2}'.format(match.group(2),operator,exponent)
      
    def parameterRewrite(match):
        return 'p' + match.group(1)
        
    def findClosure(rule):
        stackCount = 1
        init = 0
        while stackCount != 0:
            lparen = string.find(rule[init:],'(')
            rparen = string.find(rule[init:],')')
            if lparen == -1 or rparen < lparen:
                stackCount -= 1
                init += rparen + 1
            else:
                stackCount += 1
                init += lparen + 1
            
        return init
        
    def changeToBNGL(functionList,rule,function):
        oldrule = ''
        #if the rule contains any mathematical function we need to reformat
        while any([re.search(r'(\W)({0})(\W)'.format(x),rule) != None for x in functionList]) and (oldrule != rule):
            oldrule = rule
            for x in functionList:
                rule  = re.sub('({0})\(([^,]+),([^)]+)\)'.format(x),function,rule)
            if rule == oldrule:
                logMess('ERROR','Malformed pow or root function %s' % rule)
                print 'meep'
        return rule
    
    def piecewiseToIf(rule):
        init = string.find(rule,'piecewise(')+len('piecewise(')
        end = findClosure(rule[init:])
        mrule = rule[init:init+end]
        while 'piecewise' in mrule:
            mrule = piecewiseToIf(mrule)
        parameters = csl.parseString(mrule)
        result =  parsePieceWiseFunction(parameters)
        return rule[0:init-len('piecewise(')] + result + rule[init+end:]        
    
    rule = changeToBNGL(['pow','root'],rule,powParse)
    rule = changeToBNGL(['gt','lt','leq','geq','eq'],rule,compParse)
    rule = changeToBNGL(['and','or'],rule,compParse)

    while 'piecewise' in rule:
        rule = piecewiseToIf(rule)
    #change function commas for semicolons
    if 'lambda(' in rule:
        parameters =  csl.parseString(rule[string.find(rule,'(')+1:-1])
        param = []
        for idx,element in enumerate(parameters[0:-1]):
            param.append('p' + element.strip())
            #print '(%s(\W|$))' % element.strip(),parameterRewrite,parameters[-1]
            try:
                parameters[-1] = re.sub('(%s(\W|$))' % element.strip(),parameterRewrite,parameters[-1])
            except:
                logMess('ERROR','Cannot parse function %s-%s' % (functionTitle,rule))
                parameters[-1] = ''
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
    
    #change references to time for time()    
    tmp =re.sub(r'(\W|^)(time)(\W|$)',r'\1 time() \3',tmp)
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
    output.write('simulate({method=>ode,t_end=>100,n_steps=>100,print_functions=>1});')
    #output.write('writeXML()\n')
    
def sectionTemplate(name,content):
    section = 'begin %s\n' % name
    temp = ['\t%s\n' % line for line in content]
    section += ''.join(temp)
    section += 'end %s\n' % name
    return section

