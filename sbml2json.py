6# -*- coding: utf-8 -*-
"""
Created on Mon Jun 17 11:19:37 2013

@author: proto
"""

import libsbml
from scipy.misc import factorial, comb
import json
from optparse import OptionParser


class SBML2JSON:
    
    def __init__(self, model):
        self.model = model
        self.getUnits()
        self.moleculeData = {}
    def getUnits(self):
        self.unitDictionary = {}
        #unitDictionary['substance'] = [libsbml.UNIT_KIND_MOLE,1]
        #unitDictionary['volume'] = [libsbml.UNIT_KIND_LITER,1]
        #unitDictionary['area'] = [libsbml.UNIT_KIND_METER,1]
        #unitDictionary['length'] = [libsbml.UNIT_KIND_METER,1]
        #unitDictionary['time'] = [libsbml.UNIT_KIND_SECOND,1]
        
        for unitDefinition in self.model.getListOfUnitDefinitions():
            unitList = []
            for unit in unitDefinition.getListOfUnits():
                
                unitList.append([unit.getKind(),unit.getScale(),unit.getExponent()])
                
            self.unitDictionary[unitDefinition.getId()] = unitList
        '''
        
Table 3: SBML's built-in units.
Name	 Possible Scalable Units	 Default Units	
substance	 mole, item	 mole	
volume	 litre, cubic metre	 litre	
area	 square metre	 square metre	
length	 metre	 metre	
time	 second	 second	
'''
        
    def getParameters(self):
        parameters = []
        for parameter in self.model.getListOfParameters():
            parameterSpecs = {'name':parameter.getId(),'value':parameter.getValue(),
                              'unit':parameter.getUnits(),'type' : ""}
            '''                             
            if parameterSpecs[0] == 'e':
                parameterSpecs = ('are',parameterSpecs[1])
            if parameterSpecs[1] == 0:
                zparam.append(parameterSpecs[0])
            '''
            if parameter.getUnits() in self.unitDictionary:
                for factor in self.unitDictionary[parameter.getUnits()]:
                    parameterSpecs['value'] *= 10 ** (factor[1] * factor[2])
                    parameterSpecs['unit'] = '{0}*1e{1}'.format(parameterSpecs['unit'],factor[1]*factor[2])
                if 'mole' in parameter.getUnits() and 'per_mole' not in parameter.getUnits():
                    parameterSpecs['value' ] *= float('6.022e23')
                    parameterSpecs['unit'] = '{0}*{1}'.format(parameterSpecs['unit'],'avo.num')
            
            parameters.append(parameterSpecs)
        prx = {'name':"rxn_layer_t",'value':"0.01",'unit':"um",'type':""}
        ph = {'name':"h",'value':"rxn_layer_t",'unit':"um",'type':""}
        pRs = {'name':"Rs",'value':"0.002564",'unit':"um",'type':""}
        pRc = {'name':"Rc",'value':"0.0015",'unit':"um",'type':""}
        parameters.append(prx)        
        parameters.append(ph)
        parameters.append(pRs)
        parameters.append(pRc)
        return parameters
        
    
    def __getRawCompartments(self):
        '''
        extracts information about the compartments in a model
        *returns* name,dimensions,size
        '''
        compartmentList = {}
        for compartment in self.model.getListOfCompartments():
            name = compartment.getId()
            size = compartment.getSize()
            outside = compartment.getOutside()
            dimensions = compartment.getSpatialDimensions()
            compartmentList[name] = [dimensions,size,outside]
        return compartmentList
    
    def getOutsideInsideCompartment(self,compartmentList,compartment):
        outside = compartmentList[compartment][2]
        for comp in compartmentList:
            if compartmentList[comp][2] == compartment:
                return outside, comp
        return outside,-1
    
    def getMolecules(self):
        '''
        *species* is the element whose SBML information we will extract
        this method gets information directly
        from an SBML related to a particular species.
        It returns id,initialConcentration,(bool)isconstant and isboundary,
        and the compartment
        '''
        
        compartmentList = self.__getRawCompartments()
        
        molecules = []
        release = []
        for idx,species in enumerate(self.model.getListOfSpecies()):
            compartment = species.getCompartment()
            if compartmentList[compartment][0] == 3:
                typeD = '3D'
                outside,inside = self.getOutsideInsideCompartment(compartmentList, compartment)
                diffusion = 'KB*T/(6*PI*mu_{0}*Rs)'.format(compartment)
            else:
                typeD = '2D'
                
                diffusion = 'KB*T*LOG((mu_{0}*h/(SQRT(4)*Rc*(mu_{1}+mu_{2})/2))-gamma)/(4*PI*mu_{0}*h)'.format(compartment,outside,inside)
            self.moleculeData[species.getId()] = [compartmentList[compartment][0]]
            moleculeSpecs={'name':species.getId(),'type':typeD,'extendedName':species.getName(),'dif':diffusion}
            initialConcentration = species.getInitialConcentration()

            if initialConcentration == 0:
                initialConcentration = species.getInitialAmount()
            if species.getSubstanceUnits() in self.unitDictionary:
                for factor in self.unitDictionary[species.getSubstanceUnits()]:
                    initialConcentration *= 10 ** (factor[1] * factor[2])
                if 'mole' in species.getSubstanceUnits():
                    initialConcentration *= float('6.022e23')

            isConstant = species.getConstant()
            #isBoundary = species.getBoundaryCondition()
            if initialConcentration != 0:
                if compartmentList[compartment][0] == 2:
                    objectExpr = '{0}[{1}]'.format(inside.upper(),compartment.upper())
                else:
                    objectExpr = '{0}'.format(compartment)                    
                releaseSpecs = {'name': 'Release_Site_s{0}'.format(idx),'molecule':species.getId(),'shape':'OBJECT'
            ,'quantity_type':"NUMBER_TO_RELEASE",'quantity_expr':initialConcentration,'object_expr':objectExpr}
                release.append(releaseSpecs)
            #self.speciesDictionary[identifier] = standardizeName(name)
            #returnID = identifier if self.useID else \
            molecules.append(moleculeSpecs)
            
            #self.sp eciesDictionary[identifier]
        return molecules,release
    
    def getPrunnedTree(self,math,remainderPatterns):
        while (math.getCharacter() == '*' or math.getCharacter() == '/') and len(remainderPatterns) > 0:
            if libsbml.formulaToString(math.getLeftChild()) in remainderPatterns:
                remainderPatterns.remove(libsbml.formulaToString(math.getLeftChild()))
                math = math.getRightChild()
            elif libsbml.formulaToString(math.getRightChild()) in remainderPatterns:
                remainderPatterns.remove(libsbml.formulaToString(math.getRightChild()))
                math = math.getLeftChild()            
            else:
                if(math.getLeftChild().getCharacter()) == '*':
                    math.replaceChild(0, self.getPrunnedTree(math.getLeftChild(), remainderPatterns))
                if(math.getRightChild().getCharacter()) == '*':
                    math.replaceChild(math.getNumChildren() - 1,self.getPrunnedTree(math.getRightChild(), remainderPatterns))
                break
        return math
        
    def getInstanceRate(self,math,compartmentList, reversible,rReactant,rProduct):

        
        #remove compartments from expression
        math = self.getPrunnedTree(math, compartmentList)
            
        if reversible:
            if math.getCharacter() == '-' and math.getNumChildren() > 1:
                rateL, nl = (self.removeFactorFromMath(
                math.getLeftChild().deepCopy(), rReactant, rProduct))
                rateR, nr = (self.removeFactorFromMath(
                math.getRightChild().deepCopy(), rProduct, rReactant))
            else:
                rateL, nl = self.removeFactorFromMath(math, rReactant,
                                                      rProduct)
                rateL = "if({0} >= 0 ,{0},0)".format(rateL)
                rateR, nr = self.removeFactorFromMath(math, rReactant,
                                                      rProduct)
                rateR = "if({0} < 0 ,-({0}),0)".format(rateR)
                nl, nr = 1,1
        else:
            rateL, nl = (self.removeFactorFromMath(math.deepCopy(),
                                                 rReactant,rProduct))
            rateR, nr = '0', '-1'
        if reversible:
            pass
        return rateL,rateR

    def removeFactorFromMath(self, math, reactants, products):
        
            
        remainderPatterns = []
        highStoichoiMetryFactor = 1
        for x in reactants:
            highStoichoiMetryFactor  *= factorial(x[1])
            y = [i[1] for i in products if i[0] == x[0]]
            y = y[0] if len(y) > 0 else 0
            #TODO: check if this actually keeps the correct dynamics
            # this is basically there to address the case where theres more products
            #than reactants (synthesis)
            if x[1] > y:
                highStoichoiMetryFactor /= comb(int(x[1]), int(y), exact=True)
            for counter in range(0, int(x[1])):
                remainderPatterns.append(x[0])
        #for x in products:
        #    highStoichoiMetryFactor /= math.factorial(x[1])
        #remainderPatterns = [x[0] for x in reactants]
        math = self.getPrunnedTree(math,remainderPatterns)
        rateR = libsbml.formulaToString(math) 
        for element in remainderPatterns:
            rateR = 'if({0} >0,({1})/{0} ,0)'.format(element,rateR)
        if highStoichoiMetryFactor != 1:
            rateR = '{0}*{1}'.format(rateR, int(highStoichoiMetryFactor))

        return rateR,math.getNumChildren()

    def getReactions(self):
        reactionSpecs = []
        for index, reaction in enumerate(self.model.getListOfReactions()):
            reactant = [(reactant.getSpecies(), reactant.getStoichiometry())
            for reactant in reaction.getListOfReactants() if
            reactant.getSpecies() != 'EmptySet']
            product = [(product.getSpecies(), product.getStoichiometry())
            for product in reaction.getListOfProducts() if product.getSpecies()
            != 'EmptySet']

            kineticLaw = reaction.getKineticLaw()
            rReactant = [(x.getSpecies(), x.getStoichiometry()) for x in reaction.getListOfReactants() if x.getSpecies() != 'EmptySet']
            rProduct = [(x.getSpecies(), x.getStoichiometry()) for x in reaction.getListOfProducts() if x.getSpecies() != 'EmptySet']

            parameters = [(parameter.getId(), parameter.getValue()) for parameter in kineticLaw.getListOfParameters()]
            
            math = kineticLaw.getMath()
            reversible = reaction.getReversible()
            compartmentList  = []
            for compartment in (self.model.getListOfCompartments()):
                compartmentList.append(compartment.getId())
                
            rateL, rateR = self.getInstanceRate(math,compartmentList,reversible,rReactant,rProduct)
            #finalReactant = [x[0]]    
            #testing whether we have volume-surface interactions
            rcList = []
            prdList = []
            for element in reactant:
                orientation = "," if len(set(self.moleculeData[x[0]][0] for x in reactant)) \
                > 1 and self.moleculeData[element[0]] == '3' else "'"
                rcList.append("{0}{1}".format(element[0],orientation))
            for element in product:
                orientation = "," if len(set(self.moleculeData[x[0]][0] for x in reactant)) \
                > 1 and self.moleculeData[element[0]] == '3' else "'"
                
                prdList.append("{0}{1}".format(element[0],orientation))
            if rateL != 0:
                tmp = {}
                tmp['reactants'] = ' + '.join(rcList)
                tmp['products'] = ' + '.join(prdList)
                tmp['fwd_rate'] = rateL
                reactionSpecs.append(tmp)
            if rateR != 0:
                tmp ={}
                tmp['reactants'] = ' + '.join(prdList)
                tmp['products'] = ' + '.join(rcList)
                tmp['fwd_rate'] = rateR
                reactionSpecs.append(tmp)

        return reactionSpecs
            #SBML USE INSTANCE RATE 
            #HOW TO GET THE DIFFUSION CONSTANT

def main():
	
    parser = OptionParser()
    parser.add_option("-i","--input",dest="input",
		default='input.xml',type="string",
		help="The input SBML file in xml format. Default = 'input.xml'",metavar="FILE")
    parser.add_option("-o","--output",dest="output",
		default='output.json',type="string",
		help="the output JSON file. Default = output.json",metavar="FILE")
    (options, args) = parser.parse_args()
    reader = libsbml.SBMLReader()
    nameStr = options.input
    document = reader.readSBMLFromFile(nameStr)
    if document.getModel() == None:
        print 'No such input file'
        return
    parser = SBML2JSON(document.getModel())
    parameters =  parser.getParameters()
    molecules,release = parser.getMolecules()        
    reactions =  parser.getReactions()
    definition = {}
    definition['par_list'] = parameters
    definition['mol_list'] = molecules
    definition['rxn_list'] = reactions
    definition['rel_list'] = release
    print 'Writing output to {0}'.format(options.output)
    with open(options.output,'w') as f:
        json.dump(definition,f,sort_keys=True,indent=4, separators=(',', ': '))
        

if __name__ == "__main__":
	main()