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
import string
from util import logMess,NumericStringParser
import re
log = {'species':[],'reactions':[]}
class SBML2BNGL:

    
    def __init__(self,model,useID=True):
        self.useID = useID
        self.model = model
        self.tags= {}
        self.speciesDictionary = {}
        self.getSpecies()
        

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

    def __getRawRules(self, reaction):
        if self.useID:
            reactant = [(reactant.getSpecies(),reactant.getStoichiometry()) for reactant in reaction.getListOfReactants()]
            product = [(product.getSpecies(),product.getStoichiometry()) for product in reaction.getListOfProducts()]
        else:
            reactant = [(self.speciesDictionary[reactant.getSpecies()],reactant.getStoichiometry()) for reactant in reaction.getListOfReactants()]
            product = [(self.speciesDictionary[product.getSpecies()],product.getStoichiometry()) for product in reaction.getListOfProducts()]
        kineticLaw = reaction.getKineticLaw()
        parameters = [(parameter.getId(),parameter.getValue()) for parameter in kineticLaw.getListOfParameters()]
        math = kineticLaw.getMath()
        rate = libsbml.formulaToString(math)
        reversible = reaction.getReversible()
        if not self.useID:
            rate = self.convertToName(rate)
        if reversible:
            pass
        #this removes any reactants that appear in the reaction from the formula
        #because of the way BNG handles functions
        #for element in reactant:
        #    rate = rate.replace('* %s' % element[0],'',1)
        return (reactant,product,parameters,rate,reversible)
        
    def convertToName(self,rate):
        for element in sorted(self.speciesDictionary,key=len,reverse=True):
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
        functions = []
        for function in enumerate(self.model.getListOfFunctionDefinitions()):
            functionInfo = self.__getRawFunctions(function)
            functions.append(writer.bnglFunction(functionInfo[1],functionInfo[0],[]))
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
            if 'delay' in rawRules[3]:
                logMess('ERROR','BNG cannot handle delay functions in function %s' % functionName)
            if rawRules[4]:
                tmp = rawRules[3].split('-')
                if len(tmp) == 2:
                    if tmp[1][-1] == ')':
                        tmp[0] = tmp[0] + ') #' + rawRules[3] 
                        tmp[1] = tmp[0][0:string.find(tmp[0],'(')+1]+ tmp[1]
                    if not tester.eval(tmp[0]) or not tester.eval(tmp[1]):
                        tmp[0] = "if({0} >= 0 ,{0},0)".format(rawRules[3])
                        tmp[1] = "if({0} < 0 ,{0},0)".format(rawRules[3])
                        logMess('WARNING','Cannot reliably separate rate function %s into two, falling back to an if statement' % functionName)
                        #idx = logMess('ERROR','Splitting the rate law %s into two return an invalid expression' % (functionName))
                        #print 'mop'
                    functions.append(writer.bnglFunction(tmp[0],functionName,compartmentList,parameterDict))
                    functionName2 = '%s%dm()' % (functionTitle,index)
                    functions.append(writer.bnglFunction(tmp[1],functionName2,compartmentList,parameterDict))
                else:
                    tmp = [0,0]
                    tmp[0] = "if({0} >= 0 ,{0},0)".format(rawRules[3])
                    tmp[1] = "if({0} < 0 ,{0},0)".format(rawRules[3])
                    functions.append(writer.bnglFunction(tmp[0],functionName,compartmentList,parameterDict))
                    functionName2 = '%s%dm()' % (functionTitle,index)
                    functions.append(writer.bnglFunction(tmp[1],functionName2,compartmentList,parameterDict))
                    idx = logMess('WARNING','I do not know how to split the rate law {0} into two, falling back to an if statement'.format(functionName))

                    #functions.append(writer.bnglFunction('ERROR CHECK LOG {0}'.format(idx),functionName2,compartmentList))
                functionName +=', %s' % (functionName2)
            else: 
                functions.append(writer.bnglFunction(rawRules[3],functionName,compartmentList,parameterDict))
            rules.append(writer.bnglReaction(rawRules[0],rawRules[1],functionName,self.tags,translator,isCompartments,rawRules[4]))
        if len(rules) == 0:
            logMess("ERROR","The file contains no reactions")
        return parameters, rules,functions

    def __getRawAssignmentRules(self,arule):
        variable =   arule.getVariable()
        rate = libsbml.formulaToString(arule.getMath())
        if not self.useID:
            rate = self.convertToName(rate)
        return variable,rate
        
    def getAssignmentRules(self,paramRules=[]):
        arules = []
        aParameters = {}
        for arule in self.model.getListOfRules():
            rawArule = self.__getRawAssignmentRules(arule)
            #newRule = rawArule[1].replace('+',',').strip()
            ruleName = 'ar' + rawArule[0] if rawArule[0] not in paramRules else rawArule[0]
            arules.append(writer.bnglFunction(rawArule[1],ruleName,[]))
            aParameters[rawArule[0]] = 'ar' + rawArule[0]
            #arules.append('%s = %s' %(rawArule[0],newRule))
        return aParameters,arules

    def getParameters(self):
        return ['%s %f' %(parameter.getId(),parameter.getValue()) for parameter in self.model.getListOfParameters() if parameter.getValue() != 0], [x.getId() for x in self.model.getListOfParameters() if x.getValue() == 0]


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
            if rawSpecies[1]>0:
                #tmp= translator[rawSpecies[0]].toString()
                #print translator[rawSpecies[0]].toString()
                tmp2 = temp
                if rawSpecies[0] in self.tags:
                    tmp2 = (self.tags[rawSpecies[0]])
                speciesText.append('%s:%s%s %f' % (tmp2,temp,str(tmp),rawSpecies[1])) 
            observablesText.append('Species %s %s #%s' % (rawSpecies[0], tmp,rawSpecies[5]))
        moleculesText.append('NullSpecies()')
        speciesText.append('$NullSpecies() 1')
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
    name2 = name.replace("-","_")
    name2 = name2.replace("^","")
    name2 = name2.replace("'","")
    name2 = name2.replace("*","m")
    name2 = name2.replace(" ","_")
    name2 = name2.replace(",","_")
    name2 = name2.replace("(","_")
    name2 = name2.replace(")","_")
    name2 = name2.replace("/","_")
    name2 = name2.replace(":","_")
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
                    index2+=1
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

def analyzeFile(bioNumber,reactionDefinitions,useID,outputFile,speciesEquivalence=None):
    reader = libsbml.SBMLReader()
    document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD%010i.xml' % bioNumber)
    parser =SBML2BNGL(document.getModel(),useID)
    database = structures.Databases()
    #translator,log = m2c.transformMolecules(parser,database,reactionDefinitions,speciesEquivalence)
    translator = {}
    print evaluation(len(parser.getSpecies()[0]),translator)
    param,zparam = parser.getParameters()
    molecules,species,observables = parser.getSpecies(translator)
    compartments = parser.getCompartments()
    
    aParameters,aRules = parser.getAssignmentRules(zparam) 
    functions = []
    functions.extend(aRules)
    _,rules,tfunc = parser.getReactions(translator,True)
    functions.extend(tfunc) 
    functions.extend(parser.getSBMLFunctions())
    functions.extend(aRules)
    
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
    pass

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
    #208,236
    for bioNumber in range(1,410):
    #bioNumber = 175
        logMess.log = []
        logMess.counter = -1
        reactionDefinitions,useID = selectReactionDefinitions(bioNumber)
        print reactionDefinitions,useID
        spEquivalence = 'reactionDefinitions/speciesEquivalence1.json'
        analyzeFile(bioNumber,reactionDefinitions,useID,'raw/output' + str(bioNumber) + '.bngl',speciesEquivalence=spEquivalence)

if __name__ == "__main__":
    #identifyNamingConvention()
    #processDatabase()
    main()
#todo: some of the assignmentRules defined must be used instead of parameters. remove from the paraemter
#definitions those that are defined as 0'
#2:figure out which assignment rules are being used in reactions. Done before the substitution for id;s