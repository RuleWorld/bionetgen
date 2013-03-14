# -*- coding: utf-8 -*-
"""
Created on Fri Mar  1 16:14:42 2013

@author: proto
"""

#!/usr/bin/env python

import libsbml
import bnglWriter as writer
from optparse import OptionParser
import molecules2complexes as m2c
import sys
import structures
from os import listdir
import numpy as np
import analyzeRDF
from util import logMess,NumericStringParser
import re
import time
log = {'species':[],'reactions':[]}
class SBML2BNGL:

    
    def __init__(self,model,useID=True):
        self.useID = useID
        self.model = model
        self.tags= {}
        self.speciesDictionary = {}
        self.getSpecies()
        self.reactionDictionary = {}
        

    def getRawSpecies(self, species):
        '''
        *species* is the element whose SBML information we will extract
        this method gets information directly
        from an SBML related to a particular species.
        It returns id,initialConcentration,(bool)isconstant and isboundary,
        and the compartment
        '''
        identifier = species.getId()
        name = species.getName()
        if name == '':
            name = identifier
        initialConcentration = species.getInitialConcentration()
        if initialConcentration ==0:
            initialConcentration = species.getInitialAmount()
        isConstant = species.getConstant()
        isBoundary = species.getBoundaryCondition()
        compartment = species.getCompartment()
        self.speciesDictionary[identifier] = standardizeName(name)
        
        returnID = identifier if self.useID else self.speciesDictionary[identifier]
        return (returnID,initialConcentration,isConstant,isBoundary,compartment,name)

    
    def removeFactorFromMath(self,math,reactants):
        '''
        walks through a series of * nodes and removes the remainder reactant factors
        '''
        def getPrunnedTree(math,remainderPatterns):
            while math.getCharacter() == '*' and len(remainderPatterns) > 0:
                if libsbml.formulaToString(math.getLeftChild()) in remainderPatterns:
                    remainderPatterns.remove(libsbml.formulaToString(math.getLeftChild()))
                    math = math.getRightChild()
                elif libsbml.formulaToString(math.getRightChild()) in remainderPatterns:
                    remainderPatterns.remove(libsbml.formulaToString(math.getRightChild()))
                    math = math.getLeftChild()            
                else:
                    if(math.getLeftChild().getCharacter()) == '*':
                        math.replaceChild(0,getPrunnedTree(math.getLeftChild(),remainderPatterns))
                    if(math.getRightChild().getCharacter()) == '*':
                        math.replaceChild(math.getNumChildren() - 1,getPrunnedTree(math.getRightChild(),remainderPatterns))
                    break
            return math
            
        
        remainderPatterns = [x[0] for x in reactants]
        math = getPrunnedTree(math,remainderPatterns)
        rateR = libsbml.formulaToString(math) 
        for element in remainderPatterns:
            rateR = 'if({0} >0,{1}/{0} ,0)'.format(element,rateR)
        return rateR
        
    def __getRawRules(self, reaction):
        timeArray = []
        if self.useID:
            reactant = [(reactant.getSpecies(),reactant.getStoichiometry()) for reactant in reaction.getListOfReactants() if reactant.getSpecies() != 'EmptySet']
            product = [(product.getSpecies(),product.getStoichiometry()) for product in reaction.getListOfProducts() if product.getSpecies() != 'EmptySet']
        else:
            reactant = [(self.speciesDictionary[reactant.getSpecies()],reactant.getStoichiometry()) for reactant in reaction.getListOfReactants()]
            product = [(self.speciesDictionary[product.getSpecies()],product.getStoichiometry()) for product in reaction.getListOfProducts()]
        kineticLaw = reaction.getKineticLaw()
        
        rReactant = [(x.getSpecies(),1) for x in reaction.getListOfReactants() if x.getSpecies() != 'EmptySet']
        rProduct = [(x.getSpecies(),1) for x in reaction.getListOfProducts() if x.getSpecies() != 'EmptySet']
        #rReactant = [reactant for reactant in reaction.getListOfReactants()]
        parameters = [(parameter.getId(),parameter.getValue()) for parameter in kineticLaw.getListOfParameters()]
        math = kineticLaw.getMath()
        reversible = reaction.getReversible()
        if reversible:
            if math.getCharacter() == '-' and math.getNumChildren() > 1:
                rateL = (self.removeFactorFromMath(math.getLeftChild(),rReactant))
                rateR = (self.removeFactorFromMath(math.getRightChild(),rProduct))
            else:
                rateL = "if({0} >= 0 ,{0},0)".format(self.removeFactorFromMath(math,rReactant))
                rateR = "if({0} < 0 ,-{0},0)".format(self.removeFactorFromMath(math,rProduct))
        else:
            rateL =(self.removeFactorFromMath(math,rReactant))
            rateR = '0'
                
                
        
        if not self.useID:
            rateL = self.convertToName(rateL)
            rateR = self.convertToName(rateR)
            
        if reversible:
            pass
        #this removes any reactants that appear in the reaction from the formula
        #because of the way BNG handles functions
        #for element in reactant:
        #    rate = rate.replace('* %s' % element[0],'',1)
        return (reactant,product,parameters,[rateL,rateR],reversible,reaction.getId())
        
    def convertToName(self,rate):
        for element in sorted(self.speciesDictionary,key=len,reverse=True):
            if element in rate:
                rate = re.sub(r'(\W|^)({0})(\W|$)'.format(element),r'\1 {0} \3'.format(self.speciesDictionary[element]),rate)
            #rate = rate.replace(element,self.speciesDictionary[element])
        return rate

    def __getRawCompartments(self, compartment):
        '''
        Private method used by the getCompartments method 
        '''
        name = compartment.getId()
        size = compartment.getSize()
        return name,3,size
        
    def __getRawFunctions(self,function):
        math= function[1].getMath()
        name = function[1].getId()
        
        return name,libsbml.formulaToString(math)

    def getSBMLFunctions(self):
        functions = {}
        for function in enumerate(self.model.getListOfFunctionDefinitions()):
            functionInfo = self.__getRawFunctions(function)
            functions[functionInfo[0]] = (writer.bnglFunction(functionInfo[1],functionInfo[0],[],reactionDict=self.reactionDictionary))
        return functions
            
    def getCompartments(self):
        '''
        Returns an array of triples, where each triple is defined as
        (compartmentName,dimensions,size)
        '''
        compartments = []
        for _,compartment in enumerate(self.model.getListOfCompartments()):
            compartmentInfo = self.__getRawCompartments(compartment)
            name = 'cell' if compartmentInfo[0] == '' else compartmentInfo[0]
            compartments.append("%s  %d  %s" % (name,compartmentInfo[1],compartmentInfo[2]))
        return compartments

    def updateFunctionReference(self,reaction,updatedReferences):
        newRate = reaction[3]
        for reference in updatedReferences:
            newRate = re.sub(r'(\W|^)({0})(\W|$)'.format(reference),r'\1 {0} \3'.format(updatedReferences[reference]),newRate)
        return newRate
    
    def getReactions(self, translator=[],isCompartments=False,extraParameters={}):
        '''
        returns a triple containing the parameters,rules,functions
        '''
        rules = []
        parameters = []
        
        functions = []
        tester = NumericStringParser()
        
        functionTitle = 'functionRate'
        for index,reaction in enumerate(self.model.getListOfReactions()):
            parameterDict = {}
            
            rawRules =  self.__getRawRules(reaction)
            #newRate = self.updateFunctionReference(rawRules,extraParameters)
            if len(rawRules[2]) >0:
                for parameter in rawRules[2]:
                    parameters.append('%s %f' % (parameter[0],parameter[1]))
                    parameterDict[parameter[0]] = parameter[1]
            compartmentList = [['cell',1]]
            
            compartmentList.extend([[self.__getRawCompartments(x)[0],self.__getRawCompartments(x)[2]] for x in self.model.getListOfCompartments()])
            functionName = '%s%d()' % (functionTitle,index)
            if 'delay' in rawRules[3][0]:
                logMess('ERROR','BNG cannot handle delay functions in function %s' % functionName)
            if rawRules[4]:
                
                functions.append(writer.bnglFunction(rawRules[3][0],functionName,rawRules[0],compartmentList,parameterDict,self.reactionDictionary))
                functionName2 = '%s%dm()' % (functionTitle,index)                
                functions.append(writer.bnglFunction(rawRules[3][1],functionName2,rawRules[0],compartmentList,parameterDict,self.reactionDictionary))
                self.reactionDictionary[rawRules[5]] = '({0} - {1})'.format(functionName, functionName2)                
                functionName = '{0},{1}'.format(functionName,functionName2)
                
            else:
                functions.append(writer.bnglFunction(rawRules[3][0],functionName,rawRules[0],compartmentList,parameterDict,self.reactionDictionary))
                self.reactionDictionary[rawRules[5]] = '{0}'.format(functionName)
            rules.append(writer.bnglReaction(rawRules[0],rawRules[1],functionName,self.tags,translator,isCompartments,rawRules[4]))
        return parameters, rules,functions

    def __getRawAssignmentRules(self,arule):
        variable =   arule.getVariable()
        rate = libsbml.formulaToString(arule.getMath())
        
        #try to separate into positive and negative sections
        if arule.getMath().getCharacter() == '-' and arule.getMath().getNumChildren() > 1 and not arule.isAssignment():
            rateL = libsbml.formulaToString(arule.getMath().getLeftChild())
            if(arule.getMath().getRightChild().getCharacter()) == '*':
                if libsbml.formulaToString(arule.getMath().getRightChild().getLeftChild()) == variable:
                    rateR = libsbml.formulaToString(arule.getMath().getRightChild().getRightChild())
                elif libsbml.formulaToString(arule.getMath().getRightChild().getRightChild()) == variable:
                    rateR = libsbml.formulaToString(arule.getMath().getRightChild().getLeftChild())
                else:
                    rateR = 'if({0} >0,{1}/{0} ,0)'.format(variable,libsbml.formulaToString(arule.getMath().getRightChild()))
            else:
                rateR = 'if({0} >0,{1}/{0} ,0)'.format(variable,libsbml.formulaToString((arule.getMath().getRightChild())))
        else:
            rateL = libsbml.formulaToString(arule.getMath())
            rateR = '0'
        if not self.useID:
            rateL = self.convertToName(rateL)
            rateR = self.convertToName(rateR)
            variable = self.convertToName(variable).strip()
        #print arule.isAssignment(),arule.isRate()
        return variable,[rateL,rateR],arule.isAssignment(),arule.isRate()
        
    def getAssignmentRules(self,paramRules,parameters,molecules):
        compartmentList = [['cell',1]]
        compartmentList.extend([[self.__getRawCompartments(x)[0],self.__getRawCompartments(x)[2]] for x in self.model.getListOfCompartments()])

        arules = []
        aParameters = {}
        zRules = paramRules
        removeParameters = []
        newSeedSpecies = []
        artificialReactions = []
        artificialObservables = {}
        for arule in self.model.getListOfRules():
            rawArule = self.__getRawAssignmentRules(arule)
            #newRule = rawArule[1].replace('+',',').strip()
            if rawArule[3] == True:
                rateLaw1 = rawArule[1][0]
                rateLaw2 = rawArule[1][1]
                arules.append(writer.bnglFunction(rateLaw1,'arRate{0}'.format(rawArule[0]),[],compartments=compartmentList,reactionDict=self.reactionDictionary))
                arules.append(writer.bnglFunction(rateLaw2,'armRate{0}'.format(rawArule[0]),[],compartments=compartmentList,reactionDict=self.reactionDictionary))
                artificialReactions.append(writer.bnglReaction([],[[rawArule[0],1]],'{0},{1}'.format('arRate{0}'.format(rawArule[0]),'armRate{0}'.format(rawArule[0])),self.tags,{},isCompartments=True,comment = '#rateLaw'))
                if rawArule[0] in paramRules:
                    removeParameters.append('{0} 0'.format(rawArule[0]))
                    zRules.remove(rawArule[0])
                else:
                    for element in parameters:
                        if re.search('^{0}\s'.format(rawArule[0]),element):
                            removeParameters.append(element)
                        
            elif rawArule[2] == True:
                artificialObservables[rawArule[0]] = writer.bnglFunction(rawArule[1][0],rawArule[0]+'()',[],compartments=compartmentList,reactionDict=self.reactionDictionary)
                if rawArule[0] in zRules:
                    zRules.remove(rawArule[0])
                #if rawArule[0] in paramRules:
                #    removeParameters.append('{0} 0'.format(rawArule[0]))
            
            else:
                        
                if rawArule[0] not in paramRules:
                    ruleName = 'ar' + rawArule[0]
                else:
                    ruleName = rawArule[0]
                    zRules.remove(rawArule[0])
                arules.append(writer.bnglFunction(rawArule[1][0],ruleName,[],compartments=compartmentList,reactionDict=self.reactionDictionary))
                aParameters[rawArule[0]] = 'ar' + rawArule[0]
            '''
            elif rawArule[2] == True:
                for parameter in parameters:
                    if re.search('^{0}\s'.format(rawArule[0]),parameter):
                        print '////',rawArule[0]
            '''

            #arules.append('%s = %s' %(rawArule[0],newRule))
        return aParameters,arules,zRules,artificialReactions,removeParameters,artificialObservables

    def getParameters(self):
        parameters = []
        zparam = []
        for parameter in self.model.getListOfParameters():
            parameterSpecs = (parameter.getId(),parameter.getValue(),parameter.getConstant())
            #reserved keywords
            if parameterSpecs[0] == 'e':
                parameterSpecs = ('are',parameterSpecs[1])
            if parameterSpecs[1] == 0:
                zparam.append(parameterSpecs[0])
            else:
                parameters.append('{0} {1}'.format(parameterSpecs[0], parameterSpecs[1]))

        #return ['%s %f' %(parameter.getId(),parameter.getValue()) for parameter in self.model.getListOfParameters() if parameter.getValue() != 0], [x.getId() for x in self.model.getListOfParameters() if x.getValue() == 0]
        return parameters,zparam

    def getSpecies(self,translator = {}):

        moleculesText  = []
        speciesText = []
        observablesText = []
        nullFlag = False
        names = []
        for species in self.model.getListOfSpecies():
            rawSpecies = self.getRawSpecies(species)
            if (rawSpecies[4] != ''):
                self.tags[rawSpecies[0]] = '@%s' % (rawSpecies[4])
            if(rawSpecies[0] in translator):
                if translator[rawSpecies[0]].getSize()==1 and translator[rawSpecies[0]].molecules[0].name not in names:
                    names.append(translator[rawSpecies[0]].molecules[0].name)
                    moleculesText.append(translator[rawSpecies[0]].str2())
            else:
                moleculesText.append(rawSpecies[0] + '()')
            temp = '$' if rawSpecies[2] != 0 else ''
            tmp = translator[str(rawSpecies[0])] if rawSpecies[0] in translator \
                else rawSpecies[0] + '()'
            if rawSpecies[1]>=00:
                #tmp= translator[rawSpecies[0]].toString()
                #print translator[rawSpecies[0]].toString()
                tmp2 = temp
                if rawSpecies[0] in self.tags:
                    tmp2 = (self.tags[rawSpecies[0]])
                if rawSpecies[1] > 0:
                    speciesText.append('%s:%s%s %f' % (tmp2,temp,str(tmp),rawSpecies[1])) 
            observablesText.append('Species %s %s #%s' % (rawSpecies[0], tmp,rawSpecies[5]))
        #moleculesText.append('NullSpecies()')
        #speciesText.append('$NullSpecies() 1')
        return moleculesText,speciesText,observablesText
        

    def getSpeciesAnnotation(self):
        speciesAnnotation = {}

        for species in self.model.getListOfSpecies():
            rawSpecies = self.getRawSpecies(species)
            annotationXML = species.getAnnotation()
            lista = libsbml.CVTermList()
            libsbml.RDFAnnotationParser.parseRDFAnnotation(annotationXML,lista)
            if lista.getSize() == 0:
                speciesAnnotation[rawSpecies[0]] =  None
            else:
                speciesAnnotation[rawSpecies[0]] = lista.get(0).getResources()
        return speciesAnnotation

    def getSpeciesInfo(self,name):
        return self.getRawSpecies(self.model.getSpecies(name))

    def writeLog(self,translator):
        rawSpecies = [self.getRawSpecies(x) for x in self.model.getListOfSpecies()]
        log['species'].extend([x[0] for x in rawSpecies if x[0] not in translator])
        logString = ''
        #species stuff
        if(len(log['species']) > 0):
            logString += "Species we couldn't recognize:\n"
            for element in log['species']:
                logString += '\t%s\n' % element
        if(len(log['reactions'])>0):
            logString += "Reactions we couldn't infer more about due to insufficient information:"
            for element in log['reactions']:
                logString += '\t%s + %s -> %s\n' % (element[0][0],element[0][1],element[1])
        return logString

    def getStandardName(self,name):
        if name in self.speciesDictionary:
            return self.speciesDictionary[name]
        return name
        
def standardizeName(name):
    name2 = name
    
    
    name2 = name.replace("-","_")
    name2 = name2.replace("^","")
    name2 = name2.replace("'","")
    name2 = name2.replace("*","m")
    #name2 = name2.replace("#","m")
    name2 = name2.replace(" ","_")
    name2 = name2.replace(",","_")
    
    name2 = name2.replace("(","_")
    name2 = name2.replace(")","_")
    name2 = name2.replace(" ","")
    name2 = name2.replace("+","")
    name2 = name2.replace("/","_")
    name2 = name2.replace(":","_")
    name2 = name2.replace(".","_")
    
    return name2
        


def identifyNamingConvention():
    '''
    extracts statistics from the code
    '''
    
    reader = libsbml.SBMLReader()
    jsonFiles = [ f for f in listdir('./reactionDefinitions') if f[-4:-1] == 'jso']
    translationLevel = []
    arrayMolecules = []
    rules = 0
    #go through all curated models in the biomodels database
    for index in range(1,410):
        bestTranslator = {}
        
        try:
            nameStr = 'BIOMD0000000%03d' % (index)
            document = reader.readSBMLFromFile('XMLExamples/curated/' + nameStr + '.xml')
            parser = SBML2BNGL(document.getModel())
            database = structures.Databases()

            print nameStr + '.xml',
            naming = 'reactionDefinition0.json'
            bestUseID = True
            numberofMolecules = numberOfSpecies = 0
            #iterate through our naming conventions and selects that which
            #creates the most rulified elements in the translator
            for jsonFile in jsonFiles:
                for useID in [True,False]:
                    try:
                        oldmaxi = numberOfSpecies
                        parser = SBML2BNGL(document.getModel(),useID)
                        database = structures.Databases()
                        translator = m2c.transformMolecules(parser,database,'reactionDefinitions/' + jsonFile)
                        numberOfSpecies = max(numberOfSpecies,evaluation(len(parser.getSpecies()),translator))
                        if oldmaxi != numberOfSpecies:
                            naming = jsonFile
                            bestTranslator = translator
                            bestUseID = useID
                            _,rules,_ = parser.getReactions(translator)
                            numberofMolecules = len(translator)
                    except:
                        print 'ERROR',sys.exc_info()[0]
                        continue
        except:
            print 'ERROR',sys.exc_info()[0]
            continue

        _,_,obs = parser.getSpecies()
        rdfAnnotations = analyzeRDF.getAnnotations(parser,'miriam')
        #go through the annotation list and assign which species
        #correspond to which uniprot number (if it exists)
        #similarly list the number of times each individual element appears
        analyzeRDF.getAnnotations(parser,'miriam')
        molecules = {}
        if naming[-6] != 0:
            for element in bestTranslator:
                if len(bestTranslator[element].molecules) == 1:
                    name = bestTranslator[element].molecules[0].name
                    for annotation in rdfAnnotations:
                        if name in rdfAnnotations[annotation]:
                            if name not in molecules:
                                molecules[name] = [0,[]]
                            if annotation not in molecules[name][1]:
                                molecules[name][1].extend(annotation)
                    
                    if  name not in molecules:
                        molecules[name] = [1,[]]
                    for rule in rules:
                        if name in rule:
                            molecules[name][0] += 1
        
       # _,rules,_ = parser.getReactions(bestTranslator)
       #for rule in rules:
        
        if len(obs) != 0:
            print index*1.0,int(naming[-6])*1.0,numberOfSpecies*1.0/len(obs),numberofMolecules*1.0/len(obs),len(obs)*1.0,bestUseID
            
            arrayMolecule = [[x,molecules[x]] for x in molecules]
            arrayMolecules.append(arrayMolecule)
            translationLevel.append([index*1.0,int(naming[-6])*1.0,numberOfSpecies*1.0/len(obs),numberofMolecules*1.0/len(obs),len(obs)*1.0,bestUseID])
            np.save('stats3.npy',np.array(translationLevel))
        else:
            arrayMolecules.append([])
        #print arrayMolecules
        np.save('stats3b.npy',np.array(arrayMolecules))

def processDatabase():
    reader = libsbml.SBMLReader()
    jsonFiles = [ f for f in listdir('./reactionDefinitions') if f[-4:-1] == 'jso']
    history = np.load('stats3.npy')
    index2 = 0
    for index in range(1,410):
        try:
            nameStr = 'BIOMD0000000%03d' % (index)
            document = reader.readSBMLFromFile('XMLExamples/curated/' + nameStr + '.xml')
            parser = SBML2BNGL(document.getModel())
            database = structures.Databases()

            print nameStr + '.xml'
            '''
            for jsonFile in jsonFiles:
                try:
                    #print jsonFile,
                    translator = m2c.transformMolecules(parser,database,'reactionDefinitions/' + jsonFile)
                    break
                except:
                    print 'ERROR',sys.exc_info()[0]
                    continue
                #translator = m2c.transformMolecules(parser,database,'reactionDefinition2.json')
            '''
            #translator = []
            while(history[index2][0] < index):
                    index2=1
            print history[index2][0],index
            if (history[index2][0]==index) and history[index2][1] != 0:
                print str( int(history[index2][1]))
                translator = m2c.transformMolecules(parser,database,'reactionDefinitions/reactionDefinition' + str( int(history[index2][1])) + '.json')            
            else:
                translator = {}
            #print len(parser.getSpecies()),len(translator),
            evaluation(len(parser.getSpecies()),translator)

            #translator = {}
            param2 = parser.getParameters()
            molecules,species,observables = parser.getSpecies(translator)
            #print molecules,species,observables
            print 'translated: {0}/{1}'.format(len(translator),len(observables)),
            print evaluation(len(observables),translator)
            param,rules,functions = parser.getReactions(translator)
            compartments = parser.getCompartments()
            param += param2
            writer.finalText(param,molecules,species,observables,rules,functions,compartments,'output/' + nameStr + '.bngl')
            with open('output/' + nameStr + '.log', 'w') as f:
                f.write(parser.writeLog(translator))
        except:
            print 'ERROR',sys.exc_info()[0]
            continue

def evaluation(numMolecules,translator):
    originalElements = (numMolecules)
    ruleElements = len([str(translator[x]) for x in translator if ('.' in str(translator[x]) or '~' in str(translator[x]))])*1.0/originalElements
    return ruleElements


    #print rules
#14,18,56,19,49.87.88.107,109,111,120,139,140,145,151,153,171,175,182,202,205
#230,253,255,256,268,269,288,313,332,333,334,335,336,362,396,397,399,406


def selectReactionDefinitions(bioNumber):
    '''
    This method goes through the stats-biomodels database looking for the 
    best reactionDefinitions definition available
    '''
    
    db = np.load('stats3.npy')
    fileName = 'reactionDefinitions/reactionDefinition7.json'
    useID = True
    for element in db:
        if element[0] == bioNumber and element[1] != 0:
            fileName = 'reactionDefinitions/reactionDefinition' + str(int(element[1])) + '.json'
            useID = element[5]
        elif element[0] > bioNumber:
            break
    return fileName,useID


def resolveDependencies(dictionary,key,idx):
    counter = 0
    for element in dictionary[key]:
        if idx < 20:
            counter += resolveDependencies(dictionary,element,idx+1)
        else:
            counter += 1
    return len(dictionary[key]) + counter    
    
def analyzeFile(bioNumber,reactionDefinitions,useID,outputFile,speciesEquivalence=None):
    
    useArtificialRules = False
    
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD%010i.xml' % bioNumber)
    parser =SBML2BNGL(document.getModel(),useID)
    database = structures.Databases()
    #try:
    #translator,log = m2c.transformMolecules(parser,database,reactionDefinitions,speciesEquivalence)
    translator={}    
    #except:
    #    print 'failure'
    #    return
    #translator = {}
    #print evaluation(len(parser.getSpecies()[0]),translator)
    param,zparam = parser.getParameters()
    molecules,species,observables = parser.getSpecies(translator)
    compartments = parser.getCompartments()

    functions = []
    idxArray = []
    _,rules,tfunc = parser.getReactions(translator,True)
    functions.extend(tfunc)     
    aParameters,aRules,nonzparam,artificialRules,removeParams,artificialObservables = parser.getAssignmentRules(zparam,param,molecules)
    for element in nonzparam:
        param.append('{0} 0'.format(element))

    param = [x for x in param if x not in removeParams]
    tags = '@{0}'.format(compartments[0].split(' ')[0]) if len(compartments) == 1 else '@cell'
    molecules.extend([x.split(' ')[0] for x in removeParams])

    if len(molecules) == 0:
        compartments = []

    observables.extend('Species {0} {0}'.format(x.split(' ')[0]) for x in removeParams)
    for x in removeParams:
        species.append(x.split(' ')[0] + tags + ' ' + x.split(' ')[1])
    ##Comment out those parameters that are defined with assignment rules
    ##TODO: I think this is correct, but it may need to be checked
    for idx,parameter in enumerate(param):
        for key in artificialObservables:
            if re.search('^{0}\s'.format(key),parameter)!= None:
                idxArray.append(idx)
    
    for element in idxArray:
        param[element] = '#' + param[element]
    for key in artificialObservables:
        flag = -1
        for idx,observable in enumerate(observables):
            if 'Species {0} {0}()'.format(key) in observable:
                flag = idx
        if flag != -1:
            observables.pop(flag)
        functions.append(artificialObservables[key])
        flag = -1
        if '{0}()'.format(key) in molecules:
            flag = molecules.index('{0}()'.format(key))

        if flag != -1:
            molecules.pop(flag)
        
        flag = -1
        for idx,specie in enumerate(species):
            if ':{0}('.format(key) in specie:
                flag = idx
        if flag != -1:
            species[flag] = '#' + species[flag]
    functions.extend(aRules)

    sbmlfunctions = parser.getSBMLFunctions()
    for interation in range(0,3):
        for sbml2 in sbmlfunctions:
            for sbml in sbmlfunctions:
                if sbml == sbml2:
                    continue
                if sbml in sbmlfunctions[sbml2]:
                    sbmlfunctions[sbml2] = writer.extendFunction(sbmlfunctions[sbml2],sbml,sbmlfunctions[sbml])
    
    for idx in range(0,len(functions)):
        for sbml in sbmlfunctions:
            if sbml in functions[idx]:
                functions[idx] = writer.extendFunction(functions[idx],sbml,sbmlfunctions[sbml])
    #functions.extend(sbmlfunctions)
    dependencies2 = {}
    for idx in range(0,len(functions)):
        dependencies2[functions[idx].split(' = ')[0].split('(')[0].strip()] = []
        for key in artificialObservables:
            oldfunc = functions[idx]
            functions[idx] = (re.sub(r'(\W|^)({0})([^\w(]|$)'.format(key),r'\1\2()\3',functions[idx]))
            if oldfunc != functions[idx]:
                dependencies2[functions[idx].split(' = ')[0].split('(')[0]].append(key)
        for element in sbmlfunctions:
            oldfunc = functions[idx]
            key = element.split(' = ')[0].split('(')[0]
            if re.search('(\W|^){0}(\W|$)'.format(key),functions[idx].split(' = ')[1]) != None:
                dependencies2[functions[idx].split(' = ')[0].split('(')[0]].append(key)
        for element in tfunc:
            key = element.split(' = ')[0].split('(')[0]
            if key in functions[idx].split(' = ')[1]:
                dependencies2[functions[idx].split( ' = ')[0].split('(')[0]].append(key)
    '''           
    for counter in range(0,3):
        for element in dependencies2:
            if len(dependencies2[element]) > counter:
                dependencies2[element].extend(dependencies2[dependencies2[element][counter]])
    '''
    fd = []
    for function in functions:
        fd.append([function,resolveDependencies(dependencies2,function.split(' = ' )[0].split('(')[0],0)])
    fd = sorted(fd,key= lambda rule:rule[1])
    functions = [x[0] for x in fd]
    if len(param) == 0:
        param.append('dummy 0')
    #functions.extend(aRules)
    if len(compartments) > 1 and 'cell 3 1.0' not in compartments:
        compartments.append('cell 3 1.0')
    
    
    if len(artificialRules) + len(rules) == 0:
        logMess('ERROR','The file contains no reactions')
    if useArtificialRules or len(artificialRules) > 0:
        rules =['#{0}'.format(x) for x in rules]
        artificialRules.extend(rules)
        writer.finalText(param,molecules,species,observables,artificialRules,functions,compartments,outputFile)
    else:
        artificialRules =['#{0}'.format(x) for x in artificialRules]
        rules.extend(artificialRules)
        
        writer.finalText(param,molecules,species,observables,rules,functions,compartments,outputFile)
    print outputFile
    
    if len(logMess.log) > 0:
        with open(outputFile + '.log', 'w') as f:
            for element in logMess.log:
                f.write(element + '\n')
    
            


def processFile(translator,parser,outputFile):
    param2 = parser.getParameters()
    molecules,species,observables = parser.getSpecies(translator)
    compartments = parser.getCompartments()
    param,rules,functions = parser.getReactions(translator,True)
    param += param2
    writer.finalText(param,molecules,species,observables,rules,functions,compartments,outputFile)

   
def BNGL2XML():
    passcomplex

def main():
    jsonFiles = [ f for f in listdir('./reactionDefinitions') if f[-4:-1] == 'jso']
    jsonFiles.sort()
    parser = OptionParser()
    parser.add_option("-i","--input",dest="input",
        default='XMLExamples/curated/BIOMD0000000272.xml',type="string",
        help="The input SBML file in xml format. Default = 'input.xml'",metavar="FILE")
    parser.add_option("-o","--output",dest="output",
        default='output.bngl',type="string",    
        help="the output file where we will store our matrix. Default = output.bngl",metavar="FILE")

    (options, _) = parser.parse_args()
    #144
    for bioNumber in range(293,410):
    #bioNumber = 175
        logMess.log = []
        logMess.counter = -1
        reactionDefinitions,useID = selectReactionDefinitions(bioNumber)
        print reactionDefinitions,useID
        spEquivalence = 'reactionDefinitions/speciesEquivalence1.json'
        #reactionDefinitions = 'reactionDefinitions/reactionDefinition8.json'
        analyzeFile(bioNumber,reactionDefinitions,False,'raw/output' + str(bioNumber) + '.bngl',speciesEquivalence=spEquivalence)

if __name__ == "__main__":
    #identifyNamingConvention()
    #processDatabase()
    main()
#todo: some of the assignmentRules defined must be used instead of parameters. remove from the paraemter
#definitions those that are defined as 0'
#2:figure out which assignment rules are being used in reactions. Done before the substitution for id;s