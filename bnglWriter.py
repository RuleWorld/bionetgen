#!/usr/bin/env python

import re
from copy import deepcopy
from util import logMess
import string
from pyparsing import commaSeparatedList as csl
import pyparsing
from itertools import dropwhile
import StringIO
import pyparsing as pyp

def evaluatePiecewiseFunction(function):
    pass

def bnglReaction(reactant,product,rate,tags,translator=[],isCompartments=False,reversible=True,comment='',reactionName=None):
    finalString = ''
    #if translator != []:
    #    translator = balanceTranslator(reactant,product,translator)
    if len(reactant) == 0 or (len(reactant) == 1 and reactant[0][1] == 0):
        finalString += '0 '
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
        finalString += '0 '
    for index in range(0,len(product)):
        tag = ''
        if isCompartments:
            if product[index][0] in tags:
                tag = tags[product[index][0]]
        finalString +=  printTranslate(product[index],tag,translator) 
        if index < len(product) -1:
            finalString += ' + '
    finalString += ' ' + rate + ' ' + comment
    finalString = re.sub(r'(\W|^)0\(\)','0',finalString)
    if reactionName:
        finalString = '{0}: {1}'.format(reactionName,finalString)
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
        idx = logMess("ERROR:Simulation","Cannot deal with non integer stoicheometries: {0}* {1}".format(chemical[1],chemical[0]))
        tmp.append(app)
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


def rindex(lst, item):
    '''
    returns the last ocurrence of an element in alist
    '''
    try:
        return dropwhile(lambda x: lst[x] != item, reversed(xrange(len(lst)))).next()
    except StopIteration:
        raise ValueError, "rindex(lst, item): item not in list"
        
        
    
def bnglFunction(rule,functionTitle,reactants,compartments=[],parameterDict={},reactionDict={}):
    def powParse(match):
        if match.group(1) == 'root':
            exponent = '(1/%s)' % match.group(3)
        else:
            exponent = match.group(3)
        if match.group(1) in ['root','pow']:
            operator = '^'
        return '({0}){1}({2})'.format(match.group(2),operator,exponent)
    def compParse(match):

        translator = {'gt':'>','lt':'<','and':'&&','or':'||','geq':'>=','leq':'<=','eq':'=='}
        exponent = match.group(3)
        operator = translator[match.group(1)]
        return '{0} {1} {2}'.format(match.group(2),operator,exponent)
      
    def ceilfloorParse(math):
        flag = False
        if math.group(1) == 'ceil':
             flag = True
        if flag:
            return 'min(rint({0}+0.5),rint({0} + 1))'.format(math.group(2))
        else:
            return 'min(rint({0}-0.5),rint({0}+0.5))'.format(math.group(2))
    
    
    def parameterRewrite(match):
        return match.group(1) + 'param_' + match.group(2) + match.group(3)
    
    def constructFromList(argList,optionList):
        parsedString = ''
        idx = 0
        translator = {'gt':'>','lt':'<','and':'&&','or':'||','geq':'>=','leq':'<=','eq':'=='}
        while idx < len(argList):
            if type(argList[idx]) is list:
                parsedString += '(' + constructFromList(argList[idx],optionList) + ')'
            elif argList[idx] in optionList:
                if argList[idx] == 'ceil':
                    parsedString += 'min(rint(({0}) + 0.5),rint(({0}) + 1))'.format(constructFromList(argList[idx+1],optionList))
                    idx += 1
                elif argList[idx] == 'floor':
                    parsedString += 'min(rint(({0}) -0.5),rint(({0}) + 0.5))'.format(constructFromList(argList[idx+1],optionList))
                    idx += 1
                elif argList[idx] in ['pow']:
                    index = rindex(argList[idx+1],',')
                    parsedString += '(('+ constructFromList(argList[idx+1][0:index],optionList) + ')' 
                    parsedString += ' ^ '  + '(' + constructFromList(argList[idx+1][index+1:] ,optionList) + '))'
                    idx += 1
                elif argList[idx] in ['sqr','sqrt']:
                    tag = '1/' if argList[idx] == 'sqrt' else ''
                    parsedString += '((' + constructFromList(argList[idx+1],optionList) + ') ^ ({0}2))'.format(tag)
                    idx += 1
                elif argList[idx] == 'root':
                    index = rindex(argList[idx+1],',')
                    tmp =  '1/('+ constructFromList(argList[idx+1][0:index],optionList) + '))' 
                    parsedString += '((' + constructFromList(argList[idx+1][index+1:] ,optionList) + ') ^ ' + tmp
                    idx += 1
                elif argList[idx] == 'piecewise':
                    index1 = argList[idx+1].index(',')
                    try:
                        index2 = argList[idx+1][index1+1:].index(',') + index1+1
                        try:
                            index3 = argList[idx+1][index2+1:].index(',') + index2+1
                        except ValueError:
                            index3 = -1
                    except ValueError:
                        parsedString += constructFromList([argList[idx+1][index1+1:]],optionList)
                        index2 = -1
                    if index2 != -1:
                        condition = constructFromList([argList[idx+1][index1+1:index2]],optionList)
                        result = constructFromList([argList[idx+1][:index1]],optionList)
                        if index3 == -1:
                            result2 = constructFromList([argList[idx+1][index2+1:]],optionList)
                        else:
                            result2 = constructFromList(['piecewise', argList[idx+1][index2+1:]],optionList)
                        parsedString += 'if({0},{1},{2})'.format(condition,result,result2)
                    idx+=1
                elif argList[idx] in ['and', 'or']:
                    symbolDict = {'and':' && ','or':' || '}
                    indexArray = [-1]
                    elementArray = []
                    for idx2,element in enumerate(argList[idx+1]):
                        if element ==',':
                            indexArray.append(idx2)
                    indexArray.append(len(argList[idx+1]))
                    tmpStr = argList[idx+1]
                    for idx2,_ in enumerate(indexArray[0:-1]):
                        elementArray.append(constructFromList(tmpStr[indexArray[idx2]+1:indexArray[idx2+1]],optionList))
                    parsedString += symbolDict[argList[idx]].join(elementArray)
                    idx+=1
                elif argList[idx] == 'lambda':
                    
                    tmp = '('
                    upperLimit = rindex(argList[idx+1],',')
                    parsedParams = []
                    for x in argList[idx+1][0:upperLimit]:
                        if x == ',':
                            tmp += ', '
                        else:
                            tmp += 'param_' + x
                            parsedParams.append(x)
                            
                    #tmp = ''.join([x for x in constructFromList(argList[idx+1][0:upperLimit])]) 
                    tmp2 = ') = ' + constructFromList(argList[idx+1][rindex(argList[idx+1],',')+1:],optionList)
                    for x in parsedParams:
                        while re.search(r'(\W|^)({0})(\W|$)'.format(x),tmp2) != None:
                            tmp2 = re.sub(r'(\W|^)({0})(\W|$)'.format(x),r'\1param_\2 \3',tmp2)
                    idx+= 1
                    parsedString += tmp + tmp2
            else:
                parsedString += argList[idx]
            idx += 1
        return parsedString
            
        
        
    def changeToBNGL(functionList,rule,function):
        oldrule = ''
        #if the rule contains any mathematical function we need to reformat
        while any([re.search(r'(\W|^)({0})(\W|$)'.format(x),rule) != None for x in functionList]) and (oldrule != rule):
            oldrule = rule
            for x in functionList:
                rule  = re.sub('({0})\(([^,]+),([^)]+)\)'.format(x),function,rule)
            if rule == oldrule:
                logMess('ERROR','Malformed pow or root function %s' % rule)
                print 'meep'
        return rule

    #rule = changeToBNGL(['pow','root'],rule,powParse)
    rule = changeToBNGL(['gt','lt','leq','geq','eq'],rule,compParse)
    #rule = changeToBNGL(['and','or'],rule,compParse)
    flag = True
    contentRule = pyparsing.Word(pyparsing.alphanums + '_') | ',' | '.' | '+' | '-' | '*' | '/' | '^' | '&' | '>' | '<' | '=' | '|' 
    parens     = pyparsing.nestedExpr( '(', ')', content=contentRule)
    finalString = ''
    #remove ceil,floor 
    
    if any([re.search(r'(\W|^)({0})(\W|$)'.format(x),rule) != None for x in ['ceil','floor','pow','sqrt','sqr','root','and','or']]):
        argList = parens.parseString('('+ rule + ')').asList()
        rule = constructFromList(argList[0],['floor','ceil','pow','sqrt','sqr','root','and','or'])
    
    
    while 'piecewise' in rule:
        argList = parens.parseString('('+ rule + ')').asList()
        rule = constructFromList(argList[0],['piecewise'])
    #remove references to lambda functions
    if 'lambda(' in rule:
        lambdaList =  parens.parseString('(' + rule + ')')
        functionBody =  constructFromList(lambdaList[0].asList(),['lambda'])
        flag = False
        rule =  '{0}{1}'.format(functionTitle,functionBody)
    
    tmp = rule
    #delete the compartment from the rate function since cBNGL already does it
    for compartment in compartments:
        tmp = re.sub('^{0}\s*[*]'.format(compartment[0]),'',tmp)
        tmp = re.sub('([*]\s*{0})$'.format(compartment[0]),'',tmp)
        if compartment[0] in tmp:
            tmp =re.sub(r'(\W|^)({0})(\W|$)'.format(compartment[0]),r'\1 {0} \3'.format(str(compartment[1])),tmp)
            #tmp = re.sub(r'(\W)({0})(\W)'.format(compartment[0]),r'\1%s\3' % str(compartment[1]),tmp)
            logMess('INFO','Exchanging reference to compartment %s for its dimensions' % compartment[0])
    
    #change references to time for time()    
    #tmp =re.sub(r'(\W|^)(time)(\W|$)',r'\1time()\3',tmp)
    #tmp =re.sub(r'(\W|^)(Time)(\W|$)',r'\1time()\3',tmp)
    while re.search(r'(\W|^)inf(\W|$)',tmp) != None:
        tmp =re.sub(r'(\W|^)(inf)(\W|$)',r'\1 1e20 \3',tmp)
    #BNGL has ^ for power. 
    if flag:
        finalString = '%s = %s' % (functionTitle,tmp)
    else:
        finalString = tmp
    #change references to local parameters
    for parameter in parameterDict:
        finalString = re.sub(r'(\W|^)({0})(\W|$)'.format(parameter),r'\g<1>{0}\g<3>'.format(parameterDict[parameter]),finalString)
    #change references to reaction Id's to their netflux equivalent
    for reaction in reactionDict:
        if reaction in finalString:
            finalString = re.sub(r'(\W|^)({0})(\W|$)'.format(reaction),r'\g<1>{0}\g<3>'.format(reactionDict[reaction]),finalString)
    
    #combinations '+ -' break ibonetgen
    finalString = re.sub(r'(\W|^)([-])(\s)+',r'\1-',finalString)
    #changing reference of 't' to time()
    #finalString = re.sub(r'(\W|^)(t)(\W|$)',r'\1time()\3',finalString)
    #pi
    finalString = re.sub(r'(\W|^)(pi)(\W|$)',r'\g<1>3.1415926535\g<3>',finalString)
    #print reactants,finalString
    #log for log 10
    finalString = re.sub(r'(\W|^)log\(',r'\1 ln(',finalString)
    #reserved keyword: e
    finalString = re.sub(r'(\W|^)(e)(\W|$)',r'\g<1>are\g<3>',finalString)
    #changing ceil
    #avoiding variables whose name starts with a number
    
    #removing mass-action elements
    
    tmp = finalString
    
    #print finalString,reactants
    #for reactant in reactants:
    #    finalString = re.sub(r'(\W|^)({0}\s+\*)'.format(reactant[0]),r'\1',finalString)
    #    finalString = re.sub(r'(\W|^)(\*\s+{0}(\s|$))'.format(reactant[0]),r'\1',finalString)
    #print finalString
    
    #if finalString != tmp:
    #    logMess('WARNING','Removed mass action elements from )
    return finalString

def curateParameters(param):
    '''
    The objective of this function is to remove elements extraneous to bionetgen
    '''
    for element in range(0,len(param)):
        tmp = param[element]
        while re.search(r'(\W|^)inf(\W|$)',tmp) != None:
            tmp =re.sub(r'(\W|^)(inf)(\W|$)',r'\g<1>1e20\g<3>',tmp)
        param[element] = tmp
    return param
    
def finalText(comments,param,molecules,species,observables,rules,functions,compartments,fileName):
    #output = open(fileName,'w')
    output = StringIO.StringIO()
    output.write(comments.decode('ascii','ignore'))
    output.write('begin model\n')
    param = curateParameters(param)
    output.write(sectionTemplate('parameters',param))
    if len(compartments) > 0:
        output.write(sectionTemplate('compartments',compartments))          
    output.write(sectionTemplate('molecule types',molecules))
    output.write(sectionTemplate('seed species',species))
    output.write(sectionTemplate('observables',observables))
    if len(functions) > 0:
        output.write(sectionTemplate('functions',functions))
    output.write(sectionTemplate('reaction rules',rules))
    output.write('end model\n')
    #output.write('generate_network({overwrite=>1})\n')
    #output.write('simulate({method=>"ode",t_end=>100,n_steps=>100})')
    #output.write('writeXML()\n')
    #with open(fileName,'w') as outputFile:
    #    outputFile.write(output.getvalue()) 
    #output.close()
    return output.getvalue()
def sectionTemplate(name,content):
    section = 'begin %s\n' % name
    temp = ['\t%s\n' % line for line in content]
    section += ''.join(temp)
    section += 'end %s\n' % name
    return section

#341,6,12

def extendFunction(function, subfunctionName,subfunction):
    def constructFromList(argList,optionList,subfunctionParam,subfunctionBody):
        parsedString = ''
        idx = 0
        while idx < len(argList):
            if type(argList[idx]) is list:
                parsedString += '(' + constructFromList(argList[idx],optionList,subfunctionParam,subfunctionBody) + ')'
            elif argList[idx] in optionList:
                tmp = subfunctionBody
                commaIndexes = [0]
                commaIndexes.extend([i for i, x in enumerate(argList[idx+1]) if x == ","])
                commaIndexes.append(len(argList[idx+1]))
                instancedParameters = [argList[idx+1][commaIndexes[i]:commaIndexes[i+1]] for i in range(0,len(commaIndexes)-1)]
                for parameter,instance in zip(subfunctionParam,instancedParameters):
                    if ',' in instance:
                        instance.remove(',')
                    parsedParameter = ' ( ' + constructFromList(instance,optionList,subfunctionParam,subfunctionBody) + ' ) '
                    tmp = re.sub(r'(\W|^)({0})(\W|$)'.format(parameter.strip()),r'\1{0} \3'.format(parsedParameter),tmp)
                parsedString += ' ' + tmp + ' '
                idx += 1
            else:
                if argList[idx] == '=':
                    parsedString += ' ' + argList[idx] + ' '
                else:
                    parsedString += argList[idx]
            idx += 1
        return parsedString
    param = subfunction.split(' = ')[0][len(subfunctionName)+1:-1]
    body = subfunction.split(' = ')[1]
    while re.search(r'(\W|^){0}\([^)]*\)(\W|$)'.format(subfunctionName),function) != None:
        contentRule = pyparsing.Word(pyparsing.alphanums + '_.') |  ',' | '+' | '-' | '*' | '/' | '^' | '&' | '>' | '<' | '=' | '|'  
        parens     = pyparsing.nestedExpr( '(', ')', content=contentRule)
        subfunctionList = parens.parseString('(' + function + ')').asList()
        function = constructFromList(subfunctionList[0],[subfunctionName],param.split(','),body)
    return function
        
        