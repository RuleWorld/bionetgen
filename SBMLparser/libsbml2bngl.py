#!/usr/bin/env python

from libsbml import *
import bnglWriter as writer
from optparse import OptionParser


class SBML2BNGL:

    
    def __init__(self,model):
        self.model = model        
    def __getRawSpecies(self,species):
        id = species.getId()
        initialConcentration = species.getInitialConcentration()
        isConstant = species.getConstant()
        isBoundary = species.getBoundaryCondition()
        compartment = species.getCompartment()
        
        return (id,initialConcentration,isConstant,isBoundary,compartment)
        
    def __getRawRules(self,reaction):
        reactant = [reactant.getSpecies() for reactant in reaction.getListOfReactants()]
        product = [product.getSpecies() for product in reaction.getListOfProducts()]
        
        kineticLaw = reaction.getKineticLaw()
        parameters = [(parameter.getId(),parameter.getValue()) for parameter in kineticLaw.getListOfParameters()]
        math = kineticLaw.getMath()
        rate = formulaToString(math)
        for element in reactant:
        	rate = rate.replace('* %s' % element,'',1)
        return (reactant,product,parameters,rate)
            
    def getReactions(self):
        rules = []
        parameters = []
        functions = []
        functionTitle = 'functionRate'
        for index,reaction in enumerate(self.model.getListOfReactions()):
        	rawRules =  self.__getRawRules(reaction)
        	functionName = '%s%d()' % (functionTitle,index)
        	rules.append(writer.bnglReaction(rawRules[0],rawRules[1],functionName))
        	if len(rawRules[2]) >0:
        		parameters.append('%s %f' % (rawRules[2][0][0],rawRules[2][0][1]))
        	functions.append(writer.bnglFunction(rawRules[3],functionName))
            
        return parameters, rules,functions
            
    def getParameters(self):
        return ['%s %f' %(parameter.getId(),parameter.getValue()) for parameter in self.model.getListOfParameters()]
	
        
    def getSpecies(self):
    
        moleculesText  = []
        speciesText = [] 
        observablesText = []
        
        for species in self.model.getListOfSpecies():
        	rawSpecies = self.__getRawSpecies(species)
        	moleculesText.append(rawSpecies[0] + '()')
        	temp = '$' if rawSpecies[2] != 0 else ''
        	speciesText.append(temp + '%s %f' % (rawSpecies[0],rawSpecies[1]))
        	observablesText.append('Species %s %s()' % (rawSpecies[0], rawSpecies[0]))
            
        return moleculesText,speciesText,observablesText
	
       

def main():
	
    parser = OptionParser()
    parser.add_option("-i","--input",dest="input",
    	default='input.xml',type="string",
    	help="The input SBML file in xml format. Default = 'input.xml'",metavar="FILE")
    parser.add_option("-o","--output",dest="output",
    	default='output.bngl',type="string",
    	help="the output file where we will store our matrix. Default = output.bngl",metavar="FILE")
          
    (options, args) = parser.parse_args()
    reader = SBMLReader()
    document = reader.readSBMLFromFile(options.input)
    print options.input
    parser =SBML2BNGL(document.getModel())
    param2 = parser.getParameters()
    param,rules,functions = parser.getReactions()
    
    
    molecules,species,observables = parser.getSpecies()
    
    param += param2
    print rules
         
    writer.finalText(param,molecules,species,observables,rules,functions,options.output)
		
if __name__ == "__main__":
	main()
