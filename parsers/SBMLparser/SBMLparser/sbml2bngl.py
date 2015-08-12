# -*- coding: utf-8 -*-
"""
Created on Tue Dec  6 17:42:31 2011

@author: proto
"""
from copy import deepcopy,copy
import writer.bnglWriter as writer
log = {'species': [], 'reactions': []}
import re
from collections import Counter
from collections import defaultdict
import numpy as np

from utils.util import logMess
import libsbml

def factorial(x):
    temp = x
    acc = 1
    while temp > 0:
        acc *= temp
        temp -= 1
    return acc

def comb(x,y,exact=True):
    return factorial(x)/(factorial(y) * factorial(x-y))


bioqual = ['BQB_IS','BQB_HAS_PART','BQB_IS_PART_OF','BQB_IS_VERSION_OF',
           'BQB_HAS_VERSION','BQB_IS_HOMOLOG_TO',
           'BQB_IS_DESCRIBED_BY','BQB_IS_ENCODED_BY','BQB_ENCODES','BQB_OCCURS_IN',
           'BQB_HAS_PROPERTY','BQB_IS_PROPERTY_OF','BQB_HAS_TAXON','BQB_UNKNOWN']

modqual = ['BQM_IS','BQM_IS_DESCRIBED_BY','BQM_IS_DERIVED_FROM','BQM_IS_INSTANCE_OF', 'BQM_HAS_INSTANCE', 'BQM_UNKNOWN']

class SBML2BNGL:
    '''
    contains methods for extracting and formatting those sbml elements
    that are translatable into bngl
    '''
    def __init__(self, model, useID=True):
        self.useID = useID
        self.model = model
        self.tags = {}
        self.boundaryConditionVariables = []
        self.speciesDictionary = {}
        self.speciesMemory = []
        self.getSpecies()
        self.reactionDictionary = {}
        self.speciesAnnotation = None    
        self.speciesCompartments = None
        self.getUnitDefinitions()
        self.convertSubstanceUnits = False

    def reset(self):
        self.tags = {}
        self.boundaryConditionVariables = []
        self.speciesDictionary = {}
        self.speciesMemory = []
        self.getSpecies()
        self.reactionDictionary = {}

    def static_var(varname, value):
        def decorate(func):
            setattr(func, varname, value)
            return func
        return decorate


    def getMetaInformation(self, additionalNotes):

        # get unit information
        unitList = self.getUnitDefinitions()

        metaInformation = {}
        annotation = self.model.getAnnotation()
        lista = libsbml.CVTermList()
        libsbml.RDFAnnotationParser.parseRDFAnnotation(annotation,lista)
        modelHistory = self.model.getModelHistory()
        if modelHistory:
            try:
                tmp = libsbml.ModelHistory.getCreator(self.model.getModelHistory(),0).getFamilyName()
                tmp += ' ' + libsbml.ModelHistory.getCreator(self.model.getModelHistory(),0).getGivenName()
                metaInformation['creatorEmail'] = "'" + libsbml.ModelHistory.getCreator(self.model.getModelHistory(),0).getEmail() + "'"
                metaInformation['creatorName'] = "'" + tmp + "'"
            except:
                metaInformation['creatorEmail'] = "''"
                metaInformation['creatorName'] = "''"
              
        for idx in range(lista.getSize()):
            biol,qual = lista.get(idx).getBiologicalQualifierType(),lista.get(idx).getModelQualifierType()
            if biol >= len(bioqual) or bioqual[biol] == 'BQB_UNKNOWN':
              index = modqual[qual]
            else:
              index = bioqual[biol]
            if index not in metaInformation:
              metaInformation[index] = set([])
            resource = lista.get(idx).getResources().getValue(0)
            #print stats.resolveAnnotation(resource)
            metaInformation[index].add(resource)
          
        metaInformation.update(additionalNotes)

        metaString = '###\n'
        for element in metaInformation:
          if type(metaInformation[element]) == set:
              metaInformation[element] = list(metaInformation[element])
          metaString += '#@{0}:{1}\n'.format(element,(metaInformation[element]))
        metaString += '###\n'
          #if biol 
        return metaString
    def getRawSpecies(self, species,parameters=[], logEntries=True):
        '''
        *species* is the element whose SBML information we will extract
        this method gets information directly
        from an SBML related to a particular species.
        It returns id,initialConcentration,(bool)isconstant and isboundary,
        and the compartment
        It also accounts for the fact that sometimes ppl use the same name for 
        molecules with different identifiers
        '''
        identifier = species.getId()
        name = species.getName()
        
        if name == '':
            name = identifier
        if species.isSetInitialConcentration():
            initialConcentration = species.getInitialConcentration()
        else:
            initialConcentration = species.getInitialAmount()
        isConstant = species.getConstant()
        isBoundary = species.getBoundaryCondition()
        #FIXME: this condition means that a variable/species can be changed
        #by rules and/or events. this means that we effectively need a variable
        #changed by a function that tracks this value, and all references
        #to this observable have to be changed to the referrencing variable.
        #http://sbml.org/Software/libSBML/docs/java-api/org/sbml/libsbml/Species.html
        if isBoundary and not isConstant:
            isConstant = True
            if not species.isSetInitialConcentration() \
                and not species.isSetInitialAmount():
                initialConcentration = 1
        compartment = species.getCompartment()
        boundaryCondition = species.getBoundaryCondition()
        standardizedName = standardizeName(name)
        
        if standardizedName in parameters:
            standardizedName = 'sp_{0}'.format(standardizedName)
            
        
        
        # it cannot start with a number
        if standardizedName[:1].isdigit():
            standardizedName = 's' + standardizedName
        
        # two species cannot have the same name. Ids are unique but less informative
        if logEntries:
            if standardizedName in self.speciesMemory:
                standardizedName += '_' + species.getId()

            self.speciesMemory.append(standardizedName)

        if boundaryCondition:
            self.boundaryConditionVariables.append(standardizedName)
        self.speciesDictionary[identifier] = standardizedName
        returnID = identifier if self.useID else \
        self.speciesDictionary[identifier]
        
        values = {}
        values['returnID'] = returnID
        values['initialConcentration'] = initialConcentration
        values['isConstant'] = isConstant
        values['isBoundary'] = isBoundary
        values['compartment'] = compartment
        values['name'] = name
        values['identifier'] = identifier
        return values


  

    def getPrunnedTree(self,math,remainderPatterns):
        """
        walks through a series of * nodes and removes the remainder reactant factors
        arg:remainderPatterns: argumetns to be removed from the tree
        it also changes references to time variables to the keyword 'Time'
        """

        swapDict = {libsbml.AST_NAME_TIME:'Time'}
        for node in [x for x in math.getLeftChild(),math.getRightChild() if x != None]:
            if node.getType() in swapDict.keys():
                node.setName(swapDict[node.getType()])
        if math.getCharacter() == '+' and math.getNumChildren() == 1:
            math = math.getLeftChild()
        while (math.getCharacter() == '*' or math.getCharacter() == '/') and len(remainderPatterns) > 0:
            if libsbml.formulaToString(math.getLeftChild()) in remainderPatterns:
                remainderPatterns.remove(libsbml.formulaToString(math.getLeftChild()))
                if math.getCharacter() == '*':
                    math = math.getRightChild()
                else:
                    math.getLeftChild().setValue(1)
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
        
    def getIsTreeNegative(self, math):
        '''
        walks through a series of * nodes and detects whether there's a negative factor
        fixme: we should actually test if the number of negative factors is odd
        right now we are relying on  the modelers not being malicious
        when writing their rates laws.
        '''
        if (math.getCharacter() == '*' or math.getCharacter() == '/'):
            if math.getLeftChild().isUMinus():
                math.setCharacter('+')
                #math.getLeftChild().setValue(long(libsbml.formulaToString(math.getLeftChild())[1:]))
                return True
            elif math.getLeftChild().getNumChildren() == 0 and libsbml.formulaToString(math.getLeftChild()).startswith('-'):
                math.getLeftChild().setValue(long(libsbml.formulaToString(math.getLeftChild())[1:]))
                return True
            elif math.getRightChild().isUMinus():
                math.setCharacter('+')
                #math.getRightChild().setValue(long(libsbml.formulaToString(math.getRightChild())[1:]))
                return True
            elif math.getRightChild().getNumChildren() == 0 and libsbml.formulaToString(math.getRightChild()).startswith('-'):
                math.getRightChild().setValue(long(libsbml.formulaToString(math.getRightChild())[1:]))
                return True

            else:
                if(math.getLeftChild().getCharacter()) in ['*', '/', '-']:
                    if self.getIsTreeNegative(math.getLeftChild()):
                        return True
                if(math.getRightChild().getCharacter()) in ['*', '/', '-']:
                    if self.getIsTreeNegative(math.getRightChild()):
                        return True
        elif math.getCharacter() == '-' and math.getNumChildren() == 1:
            math.setCharacter('+')
            return True
        return False

    def getUnitDefinitions(self):
        self.unitDictionary = {}
        for unitDefinition in self.model.getListOfUnitDefinitions():
            unitList = []
            for unit in unitDefinition.getListOfUnits():
                correctedUnit = libsbml.Unit.convertToSI(unit)
                #unitList.append({'kind':unit.getKind(), 'scale':unit.getScale(),'multiplier':unit.getMultiplier(), 'exponent': unit.getExponent(), 'name':name})

                for unit2 in correctedUnit.getListOfUnits():
                    name = unit.getName() if unit.getName() else unitDefinition.getName()
                    unitList.append({'kind':unit2.getKind(), 'scale': unit2.getScale(), 'multiplier':unit2.getMultiplier(), 'exponent': unit2.getExponent(), 'name':name})
            self.unitDictionary[unitDefinition.getId()] = unitList

    def preProcessStoichiometry(self, reactants):
        '''
        checks for reactants with the same name in the reactant list. This
        is mainly to account for reactants that dont have the stoichiometry
        flag properly set and instead appear repeated
        '''
        uniqueReactantDict = defaultdict(int)
        for reactant in reactants:
            uniqueReactantDict[reactant[0]] += reactant[1]
        return [(x, uniqueReactantDict[x]) for x in uniqueReactantDict]

    def removeFactorFromMath(self, math, reactants, products):
        '''
        it also adds symmetry factors. this checks for symmetry in the species names
        s
        '''
        ifStack = Counter()
        remainderPatterns = []
        highStoichoiMetryFactor = 1
        processedReactants = self.preProcessStoichiometry(reactants)

        for x in processedReactants:
            highStoichoiMetryFactor  *= factorial(x[1])
            y = [i[1] for i in products if i[0] == x[0]]
            y = y[0] if len(y) > 0 else 0
            # TODO: check if this actually keeps the correct dynamics
            # this is basically there to address the case where theres more products
            # than reactants (synthesis)
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
            ifStack.update([element])
        for element in ifStack:
            if ifStack[element] > 1:
                rateR = 'if({0}>0,{1}/({0}^{2}),0)'.format(element,rateR,ifStack[element])
            else:
                rateR = 'if({0}>0,{1}/{0},0)'.format(element,rateR)
        if np.isinf(highStoichoiMetryFactor):
            rateR = '{0} * 1e20'.format(rateR)
            logMess('SIMULATION:WARNING','Found usage of "inf" inside function {0}'.format(rateR))
        elif highStoichoiMetryFactor != 1:
            rateR = '{0}*{1}'.format(rateR, int(highStoichoiMetryFactor))
        
        return rateR,math.getNumChildren()


    def analyzeReactionRate(self,math,compartmentList,reversible,rReactant,rProduct):
        """
        This functions attempts to obtain the left and right hand sides of a rate reaction 
        function. It also removes compartments and chemical factors from the function

        Keyword arguments:
            math -- the MathML math object
            imag -- compartmentList: a list of all the compartments in the system
            reversible -- a boolean indicated whether there's should be a backward rate
            rReactant -- a string list of the reactants.
            rProduct -- a string list of the products.
        """
        math = self.getPrunnedTree(math, compartmentList)
        if reversible:
            if math.getCharacter() == '-': 
                if math.getNumChildren() > 1:
                    rateL, nl = (self.removeFactorFromMath(
                    math.getLeftChild().deepCopy(), rReactant, rProduct))
                    rateR, nr = (self.removeFactorFromMath(
                    math.getRightChild().deepCopy(), rProduct, rReactant))
                else:
                    rateR,rateL,nr,nl = self.analyzeReactionRate(math.getChild(0),compartmentList,reversible,rProduct,rReactant)
            elif math.getCharacter() == '+' and math.getNumChildren()> 1:

                if(self.getIsTreeNegative(math.getRightChild())):
                    rateL, nl = (self.removeFactorFromMath(
                    math.getLeftChild().deepCopy(), rReactant, rProduct))
                    rateR, nr = (self.removeFactorFromMath(
                    math.getRightChild().deepCopy(), rProduct, rReactant))
                elif(self.getIsTreeNegative(math.getLeftChild())):
                    rateR, nr = (self.removeFactorFromMath(
                    math.getLeftChild().deepCopy(), rProduct, rReactant))
                    rateL, nl = (self.removeFactorFromMath(
                    math.getRightChild().deepCopy(), rReactant, rProduct))
                else:
                    rateL, nl = self.removeFactorFromMath(math.deepCopy(), rReactant,
                                                          rProduct)
                    rateL = "if({0}>= 0,{0},0)".format(rateL)
                    rateR, nr = self.removeFactorFromMath(math.deepCopy(), rProduct,
                                                          rReactant)
                    rateR = "if({0}< 0,-({0}),0)".format(rateR)
                    nl, nr = 1,1
                    
            else:
                rateL, nl = self.removeFactorFromMath(math.deepCopy(), rReactant,
                                                      rProduct)
                rateL = "if({0}>= 0,{0},0)".format(rateL)
                rateR, nr = self.removeFactorFromMath(math.deepCopy(), rProduct,
                                                      rReactant)
                rateR = "if({0}< 0,-({0}),0)".format(rateR)
                nl, nr = 1,1
        else:
            rateL, nl = (self.removeFactorFromMath(math.deepCopy(),
                                                 rReactant,rProduct))
            rateR, nr = '0', '-1'
        return rateL,rateR,nl,nr

        
    def __getRawRules(self, reaction, symmetryFactors, functionFlag):
        if self.useID:
            reactant = [(reactant.getSpecies(), reactant.getStoichiometry())
            for reactant in reaction.getListOfReactants() if
            reactant.getSpecies() != 'EmptySet']
            product = [(product.getSpecies(), product.getStoichiometry())
            for product in reaction.getListOfProducts() if product.getSpecies()
            != 'EmptySet']
        else:
            reactant = [(self.speciesDictionary[rElement.getSpecies()], rElement.getStoichiometry()) for rElement in reaction.getListOfReactants()]
            product = [(self.speciesDictionary[rProduct.getSpecies()], rProduct.getStoichiometry()) for rProduct in reaction.getListOfProducts()]
        kineticLaw = reaction.getKineticLaw()
        reversible = reaction.getReversible()

        if kineticLaw == None:
            return {'reactants':reactant,'products':product,'parameters':[],'rates':['0','0'],
            'reversible':reversible,'reactionID':reaction.getId(),'numbers':[0,0]}

        rReactant = [(x.getSpecies(), x.getStoichiometry()) for x in reaction.getListOfReactants() if x.getSpecies() != 'EmptySet']
        rProduct = [(x.getSpecies(), x.getStoichiometry()) for x in reaction.getListOfProducts() if x.getSpecies() != 'EmptySet']
        #rReactant = [reactant for reactant in reaction.getListOfReactants()]
        parameters = [(parameter.getId(), parameter.getValue(),parameter.getUnits()) for parameter in kineticLaw.getListOfParameters()]

        rateL = rateR = nl = nr = None
        if True:
            # TODO: For some reason creating a deepcopy of this screws everything up, even
            # though its what we should be doing
            # apparently the solution was to use copy instead of deepcopy. Go figure.
            math = copy(kineticLaw.getMath())
            
            # get a list of compartments so that we can remove them
            compartmentList = []
            for compartment in (self.model.getListOfCompartments()):
                compartmentList.append(compartment.getId())
                
            # remove compartments from expression. also separate left hand and right hand side
            rateL, rateR, nl, nr = self.analyzeReactionRate(math, compartmentList,
                reversible, rReactant, rProduct)
            if symmetryFactors[0] > 1:
                rateL = '({0})/{1}'.format(rateL, symmetryFactors[0])
            if symmetryFactors[1] > 1:
                rateR = '({0})/{1}'.format(rateR, symmetryFactors[1])
            if not self.useID:
                rateL = self.convertToName(rateL)
                rateR = self.convertToName(rateR)
            if reversible:
                pass
            # return compartments if the reaction is unimolecular
            # they were removed in the first palce because its easier to handle
            # around the equation in tree form when it has less terms
            '''
            if len(self.model.getListOfCompartments()) > 0:
                for compartment in (self.model.getListOfCompartments()):
                    if compartment.getId() not in compartmentList:
                        if len(rReactant) != 2:
                            rateL = '{0} * {1}'.format(rateL,compartment.getSize())
                        if len(rProduct) != 2:
                             rateR = '{0} * {1}'.format(rateR,compartment.getSize())
            '''     

        
        return {'reactants':reactant,'products':product,'parameters':parameters,'rates':[rateL,rateR],
        'reversible':reversible,'reactionID':reaction.getId(),'numbers':[nl,nr]}
                
        return (reactant, product, parameters, [rateL, rateR],
                reversible, reaction.getId(), [nl, nr])
    
    def getReactionCenter(self,reactant,product,translator):
        rcomponent = Counter()
        pcomponent = Counter()
        
        for element in reactant:
            if element[0] in translator:
                for molecule in translator[element[0]].molecules:
                    for component in molecule.components:
                        molecule.sort()
                        rcomponent.update(Counter([(molecule.name,component.name,len(component.bonds)>0,component.activeState)]))
        for element in product:
            if element[0] in translator:
                for molecule in translator[element[0]].molecules:
                    molecule.sort()
                    for component in molecule.components:
                        molecule.sort()
                        pcomponent.update(Counter([(molecule.name,component.name,len(component.bonds)>0,component.activeState)]))
        reactionCenter = [(x[0],x[1]) for x in rcomponent for y in pcomponent if (x[0],x[1]) == (y[0],y[1]) and x!= y and rcomponent[x] != pcomponent[y]]
        rreactionCenter = [(x[0],x[1]) for x in pcomponent for y in rcomponent if (x[0],x[1]) == (y[0],y[1]) and x!= y and pcomponent[x] != rcomponent[y]]
        return reactionCenter,rreactionCenter
        
    def updateComponentCount(self,counterArray,reference,updateValue):
        for element in counterArray:
            if reference in counterArray[element]:
                counterArray[element][reference] += updateValue
    def reduceComponentSymmetryFactors(self,reaction,translator,functions):
        '''
        create symmetry factors for reactions with components and species with
        identical names. This checks for symmetry in the components names then.
        '''
        
        if self.useID:
            reactant = [(rElement.getSpecies(), rElement.getStoichiometry())
            for rElement in reaction.getListOfReactants() if
            rElement.getSpecies() != 'EmptySet']
            product = [(product.getSpecies(), product.getStoichiometry())
            for product in reaction.getListOfProducts() if product.getSpecies()
            != 'EmptySet']
        else:
            reactant = [(self.speciesDictionary[rElement.getSpecies()], rElement.getStoichiometry()) for rElement in reaction.getListOfReactants()]
            product = [(self.speciesDictionary[rProduct.getSpecies()], rProduct.getStoichiometry()) for rProduct in reaction.getListOfProducts()]
        kineticLaw = reaction.getKineticLaw()
        reversible = reaction.getReversible()

        if kineticLaw == None:
            return 1,1

        rReactant = [(x.getSpecies(), x.getStoichiometry()) for x in reaction.getListOfReactants() if x.getSpecies() != 'EmptySet']
        rProduct = [(x.getSpecies(), x.getStoichiometry()) for x in reaction.getListOfProducts() if x.getSpecies() != 'EmptySet']
        
        #TODO: For some reason creating a deepcopy of this screws everything up, even
        #though its what we should be doing
        rcomponent = defaultdict(Counter)
        pcomponent = defaultdict(Counter)
        
        #get the total count of components in the reactants and products
        #e.g. components across diffent species
        freactionCenter,breactionCenter = self.getReactionCenter(reactant,product,translator)
        
        for element in reactant:
            if element[0] in translator:
                
                for molecule in translator[element[0]].molecules:
                    for component in molecule.components:
                        molecule.sort()
                        componentList = Counter([(molecule.signature(freactionCenter))])
                        for _ in range(0,int(element[1])):
                            rcomponent[(molecule.name,component.name,len(component.bonds)>0,component.activeState)].update(componentList)
                        

        
        for element in product:
            if element[0] in translator:
                for molecule in translator[element[0]].molecules:
                    molecule.sort()
                    for component in molecule.components:

                        componentList = Counter([(molecule.signature(breactionCenter))])
                        for _ in range(0,int(element[1])):
                            pcomponent[(molecule.name,component.name,len(component.bonds)>0,component.activeState)].update(componentList)

        '''
        only keep information for reaction centers
        '''
        reactionCenters = [(x[0],x[1]) for x in rcomponent for y in pcomponent if (x[0],x[1]) == (y[0],y[1]) and x!= y]      
        rcomponent= {x:rcomponent[x] for x in rcomponent if (x[0],x[1]) in reactionCenters}
        pcomponent= {x:pcomponent[x] for x in pcomponent if (x[0],x[1]) in reactionCenters}

        #is the number of components across products and reactants the same?
        #eg is there any DeleteMolecules action
        pcorrectionFactor = 1
        rcorrectionFactor = 1
        rStack = []
        pStack = []
        '''
        if a reaction can take place in several ways account for it in the reaction 
        rate (this is specially important in dimer and trimer binding)
        pcomponent[element] < rcomponent[element] asks if an specific instance
        of a component decreases in number from a reactant to a product
        for example if there are 3 A(b)'s and one binds, we will have 2 A(b)'s
        in the product  
        '''
        rcomponentTemp = deepcopy(rcomponent)
        pcomponentTemp = deepcopy(pcomponent)
        
        #calculate actual symmetry factors
        for key in rcomponent:
            if key in pcomponent:
                for element in rcomponent[key]:
                    if rcomponent[key] ==1:
                        continue
                    #if theres a component on one side of the equation that
                    #appears a different number of times on the other side of the equation
                    if element in pcomponent[key]:
                        if pcomponent[key][element] < rcomponent[key][element] and set([key[0].lower(),key[1].lower()]) not in rStack:
                            rcorrectionFactor *= comb(rcomponent[key][element],pcomponent[key][element],exact=1)
                            rStack.append(set([key[0].lower(),key[1].lower()]))
                            #once we choose a component for a previous action
                            #this limits the options for subsequent actions
                            #although substracting one from the target sites
                            #may not be the right option. double check.
                            self.updateComponentCount(pcomponent,element,-1)
                    else:
                        for element2 in pcomponent[key]:
                            if pcomponent[key][element2] < rcomponent[key][element] and set([key[0].lower(),key[1].lower()]) not in rStack:
                                rcorrectionFactor *= comb(rcomponent[key][element],pcomponent[key][element2],exact=1)
                                rStack.append(set([key[0].lower(),key[1].lower()]))
                                self.updateComponentCount(pcomponent,element2,-1)

        rcomponent = rcomponentTemp
        pcomponent = pcomponentTemp
        
        if reversible:
            for key in pcomponent:
                if key in rcomponent:
                    for element in pcomponent[key]:
                        if pcomponent[key] ==1:
                            continue
                        if element in rcomponent[key]:
                            if rcomponent[key][element] < pcomponent[key][element] and set([key[0].lower(),key[1].lower()]) not in pStack:
                                pcorrectionFactor *= comb(pcomponent[key][element],rcomponent[key][element],exact=1)
                                pStack.append(set([key[0].lower(),key[1].lower()]))
                                self.updateComponentCount(rcomponent,element,-1)

                        else:
                            for element2 in rcomponent[key]:
                                if rcomponent[key][element2] < pcomponent[key][element] and set([key[0].lower(),key[1].lower()]) not in pStack:
                                    pcorrectionFactor *= comb(pcomponent[key][element],rcomponent[key][element2],exact=1)
                                    pStack.append(set([key[0].lower(),key[1].lower()]))
                                    self.updateComponentCount(rcomponent,element2,-1)


        return rcorrectionFactor,pcorrectionFactor
        
    def convertToName(self, rate):
        for element in sorted(self.speciesDictionary, key=len, reverse=True):
            if element in rate:
                rate = re.sub(r'(\W|^)({0})(\W|$)'.format(element),
                              r'\1{0}\3'.format(
                              self.speciesDictionary[element]), rate)
            #rate = rate.replace(element,self.speciesDictionary[element])
        return rate

    def __getRawCompartments(self, compartment):
        '''
        Private method used by the getCompartments method 
        '''
        name = compartment.getId()
        size = compartment.getSize()
        dimensions = compartment.getSpatialDimensions()
        #if size != 1:
        #    print '!',
        #return name,3,size
        return name, dimensions, size
        
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
            compartments.append("%s  %d  %s" % (name, compartmentInfo[1], compartmentInfo[2]))
        return compartments

    def updateFunctionReference(self,reaction,updatedReferences):
        newRate = reaction[3]
        for reference in updatedReferences:
            newRate = re.sub(r'(\W|^)({0})(\W|$)'.format(reference),r'\1{0}\3'.format(updatedReferences[reference]),newRate)
            
        return newRate
    
    def getReactions(self, translator={}, isCompartments=False, extraParameters={},atomize=False):
        '''
        @returns: a triple containing the parameters,reactions,functions
        '''
        
        ##@FIXME:this part of the code is there so that we only generate the functions list once through different
        #iterations of this call. This is because we cannot create a clone of the 'math' object for this
        #reaction and it is being permanently changed every call. It's ugly but it works. Change for something
        #better when we figure out how to clone the math object
        if not hasattr(self.getReactions,'functionFlag'):
            self.getReactions.__func__.functionFlag = False or (not atomize)

        reactions = []
        parameters = []
        functions = []
        functionTitle = 'functionRate'
        
        if len(self.model.getListOfReactions()) == 0:
            logMess('ERROR:Simulation','Model contains no natural reactions, all reactions are produced by SBML rules')
        for index, reaction in enumerate(self.model.getListOfReactions()):
            parameterDict = {}
            #symmetry factors for components with the same name
            sl, sr = self.reduceComponentSymmetryFactors(reaction, translator, functions)
            
            rawRules =  self.__getRawRules(reaction,[sl,sr],self.getReactions.functionFlag)
            if len(rawRules['parameters']) >0:
                for parameter in rawRules['parameters']:
                    """
                    if self.convertSubstanceUnits:
                        #newParameter = self.convertToStandardUnits(parameter[1],self.unitDictionary)
                        parameters.append('r%d_%s %f' % (index+1, parameter[0], parameter[1]))
                        parameterDict[parameter[0]] = parameter[1]
                    else:
                    """
                    parameters.append('r%d_%s %f' % (index+1, parameter[0], parameter[1]))
                    parameterDict[parameter[0]] = parameter[1]
            compartmentList = [['cell',1]]
            compartmentList.extend([[self.__getRawCompartments(x)[0],self.__getRawCompartments(x)[2]] for x in self.model.getListOfCompartments()])
            threshold = 0
            if rawRules['numbers'][0] > threshold:  
                functionName = '%s%d()' % (functionTitle,index)
            else:
                #append reactionNumbers to parameterNames 
                finalString = str(rawRules['rates'][0])
                for parameter in parameterDict:
                    finalString = re.sub(r'(\W|^)({0})(\W|$)'.format(parameter), r'\1{0}\3'.format('r{0}_{1}'.format(index+1,parameter)), finalString)
                functionName = finalString
            
            if self.getReactions.functionFlag and 'delay' in rawRules['rates'][0]:
                logMess('ERROR:Simulation','BNG cannot handle delay functions in function %s' % functionName)
            if rawRules['reversible']:
                if rawRules['numbers'][0] > threshold:
                    if self.getReactions.functionFlag:
                        functions.append(writer.bnglFunction(rawRules['rates'][0], functionName, rawRules['reactants'], compartmentList, parameterDict, self.reactionDictionary))
                if rawRules['numbers'][1] > threshold:
                    functionName2 = '%s%dm()' % (functionTitle,index)    
                    if self.getReactions.functionFlag:
                        functions.append(writer.bnglFunction(rawRules['rates'][1],functionName2,rawRules['reactants'],compartmentList,parameterDict,self.reactionDictionary))
                    self.reactionDictionary[rawRules['reactionID']] = '({0} - {1})'.format(functionName, functionName2)                
                    functionName = '{0},{1}'.format(functionName, functionName2)
                else:
                    finalString = str(rawRules['rates'][1])
                    for parameter in parameterDict:
                        finalString = re.sub(r'(\W|^)({0})(\W|$)'.format(parameter),r'\1{0}\3'.format('r{0}_{1}'.format(index+1,parameter)),finalString)
                    functionName = '{0},{1}'.format(functionName,finalString)
            else:
                if rawRules['numbers'][0] > threshold:
                    if self.getReactions.functionFlag:
                        functions.append(writer.bnglFunction(rawRules['rates'][0], functionName, rawRules['reactants'], compartmentList, parameterDict,self.reactionDictionary))
                    self.reactionDictionary[rawRules['reactionID']] = '{0}'.format(functionName)
            #reactants = [x for x in rawRules[0] if x[0] not in self.boundaryConditionVariables]
            #products = [x for x in rawRules[1] if x[0] not in self.boundaryConditionVariables]
            reactants = [x for x in rawRules['reactants']]
            products = [x for x in rawRules['products']]
            reactions.append(writer.bnglReaction(reactants,products,functionName,self.tags,translator,(isCompartments or ((len(reactants) == 0 or len(products) == 0) and self.getReactions.__func__.functionFlag)),rawRules['reversible'],reactionName=rawRules['reactionID']))
        if atomize:
            self.getReactions.__func__.functionFlag = True
        return parameters, reactions,functions

    def __getRawAssignmentRules(self,arule):
        variable =   arule.getVariable()
        
        #try to separate into positive and negative sections
        if arule.getMath().getCharacter() == '-' and arule.getMath().getNumChildren() > 1 and not arule.isAssignment():
            rateL = libsbml.formulaToString(arule.getMath().getLeftChild())
            if(arule.getMath().getRightChild().getCharacter()) == '*':
                if libsbml.formulaToString(arule.getMath().getRightChild().getLeftChild()) == variable:
                    rateR = libsbml.formulaToString(arule.getMath().getRightChild().getRightChild())
                elif libsbml.formulaToString(arule.getMath().getRightChild().getRightChild()) == variable:
                    rateR = libsbml.formulaToString(arule.getMath().getRightChild().getLeftChild())
                else:
                    rateR = 'if({0}>0,({1})/{0},0)'.format(variable,libsbml.formulaToString(arule.getMath().getRightChild()))
            else:
                rateR = 'if({0}>0,({1})/{0},0)'.format(variable,libsbml.formulaToString((arule.getMath().getRightChild())))
        else:
            rateL = libsbml.formulaToString(arule.getMath())
            rateR = '0'
        if not self.useID:
            rateL = self.convertToName(rateL)
            rateR = self.convertToName(rateR)
            variable = self.convertToName(variable).strip()
        #print arule.isAssignment(),arule.isRate()
        return variable,[rateL, rateR], arule.isAssignment(), arule.isRate()
        
    def getAssignmentRules(self, zparams, parameters, molecules,observables):
        '''
        this method obtains an SBML rate rules and assignment rules. They
        require special handling since rules are often both defined as rules 
        and parameters initialized as 0, so they need to be removed from the parameters list
        '''

        compartmentList = [['cell',1]]
        compartmentList.extend([[self.__getRawCompartments(x)[0], self.__getRawCompartments(x)[2]] for x in self.model.getListOfCompartments()])

        arules = []
        aParameters = {}
        zRules = zparams
        removeParameters = []
        artificialReactions = []
        artificialObservables = {}
        for arule in self.model.getListOfRules():
            
            rawArule = self.__getRawAssignmentRules(arule)
            #tmp.remove(rawArule[0])
            #newRule = rawArule[1].replace('+',',').strip()
            if rawArule[3] == True:
                #it is a rate rule
                if rawArule[0] in self.boundaryConditionVariables:
                    logMess('SIMULATION:CRITICAL','rate rules ({0}) \
                    are not properly supported in BioNetGen simulator'.format(rawArule[0]))

                    #aParameters[rawArule[0]] = 'arj' + rawArule[0] 
                    #tmp = list(rawArule)
                    #tmp[0] = 'arj' + rawArule[0]
                    #rawArule = tmp


                rateLaw1 = rawArule[1][0]
                rateLaw2 = rawArule[1][1]
                arules.append(writer.bnglFunction(rateLaw1, 'arRate{0}'.format(rawArule[0]),[],compartments=compartmentList, reactionDict=self.reactionDictionary))
                arules.append(writer.bnglFunction(rateLaw2, 'armRate{0}'.format(rawArule[0]),[],compartments=compartmentList, reactionDict=self.reactionDictionary))
                artificialReactions.append(writer.bnglReaction([], [[rawArule[0],1]],'{0},{1}'.format('arRate{0}'.format(rawArule[0]), 'armRate{0}'.format(rawArule[0])), self.tags, {}, isCompartments=True, comment = '#rateLaw'))
                #arules.append(writer.bnglFunction('({0}) - ({1})'.format(rawArule[1][0],rawArule[1][1]), '{0}'.format(rawArule[0]),[],compartments=compartmentList, reactionDict=self.reactionDictionary))
                if rawArule[0] in zparams:
                    removeParameters.append('{0} 0'.format(rawArule[0]))
                    zRules.remove(rawArule[0])
                else:
                    for element in parameters:
                        #TODO: if for whatever reason a rate rule
                        #was defined as a parameter that is not 0
                        #remove it. This might not be exact behavior
                        if re.search('^{0}\s'.format(rawArule[0]), element):
                            logMess("WARNING","Parameter {0} corresponds both as a non zero parameter \
                            and a rate rule, verify behavior".format(element))
                            removeParameters.append(element)
                        
            elif rawArule[2] == True:

                #it is an assigment rule
                if rawArule[0] in zRules:
                    zRules.remove(rawArule[0])
                    #print rawArule[0]

                    #aParameters[rawArule[0]] = 'arj' + rawArule[0] 
                    #tmp = list(rawArule)
                    #tmp[0] = 'arj' + rawArule[0]
                    #rawArule= tmp
                    logMess('Error:Simulation', 'Boundary condition/assignment type variables ({0}) are not properly \
                    supported in BioNetGen simulator'.format(rawArule[0]))

                elif rawArule[0] in molecules:
                        if molecules[rawArule[0]]['isBoundary']:
                            artificialObservables[rawArule[0]+'_ar'] = writer.bnglFunction(rawArule[1][0],rawArule[0]+'_ar()',[],compartments=compartmentList,reactionDict=self.reactionDictionary)
                            continue
                else:
                    #check if it is defined as an observable
                    candidates =  [idx for idx,x in enumerate(observables) if rawArule[0] in x]
                    assigObsFlag = False
                    for idx in candidates:
                        if re.search('\s{0}\s'.format(rawArule[0]),observables[idx]):
                            artificialObservables[rawArule[0]+ '_ar'] = writer.bnglFunction(rawArule[1][0],rawArule[0]+'_ar()',[],compartments=compartmentList,reactionDict=self.reactionDictionary)
                            assigObsFlag = True
                            break
                    if assigObsFlag:
                        continue
                # if its not a param/species/observable
                artificialObservables[rawArule[0]] = writer.bnglFunction(rawArule[1][0],rawArule[0]+'()',[],compartments=compartmentList,reactionDict=self.reactionDictionary)

            else:
                '''
                if for whatever reason you have a rule that is not assigment
                or rate and it is initialized as a non zero parameter, give it
                a new name
                '''
                if rawArule[0] not in zparams:
                    ruleName = 'ar' + rawArule[0]
                else:
                    ruleName = rawArule[0]
                    zRules.remove(rawArule[0])
                arules.append(writer.bnglFunction(rawArule[1][0], ruleName, [], compartments=compartmentList, reactionDict=self.reactionDictionary))
                aParameters[rawArule[0]] = 'ar' + rawArule[0]
            '''
            elif rawArule[2] == True:
                for parameter in parameters:
                    if re.search('^{0}\s'.format(rawArule[0]),parameter):
                        print '////',rawArule[0]
            '''
            #arules.append('%s = %s' %(rawArule[0],newRule))
        return aParameters, arules, zRules, artificialReactions, removeParameters, artificialObservables

    def convertToStandardUnits(self, parameterValue, unitDefinition):

        for factor in unitDefinition:
            parameterValue *= 10 ** int(factor['scale'])
            parameterValue /= int(factor['multiplier'])
            parameterValue **= int(factor['exponent'])
        return parameterValue

    def getParameters(self):
        parameters = []
        zparam = []
        for parameter in self.model.getListOfParameters():
            parameterSpecs = (parameter.getId(), parameter.getValue(), parameter.getConstant(), parameter.getUnits())
            #print parameterSpecs
            #reserved keywords
            if parameterSpecs[0] == 'e':
                parameterSpecs = ('are', parameterSpecs[1])
            if parameterSpecs[1] == 0:
                zparam.append(parameterSpecs[0])
            else:
                """
                if self.convertSubstanceUnits:
                    newParameterSpecs = [parameterSpecs[0], self.convertToStandardUnits(parameterSpecs[1], self.unitDictionary[parameterSpecs[3]]), parameterSpecs[2], parameterSpecs[3]]
                    parameters.append('{0} {1} #original units:{2}={3}'.format(newParameterSpecs[0], newParameterSpecs[1], newParameterSpecs[3],parameterSpecs[1]))

                else:
                """
                if parameterSpecs[3] != '':
                    parameters.append('{0} {1} #units:{2}'.format(parameterSpecs[0], parameterSpecs[1], parameterSpecs[3]))
                else:
                    parameters.append('{0} {1}'.format(parameterSpecs[0], parameterSpecs[1]))

        #return ['%s %f' %(parameter.getId(),parameter.getValue()) for parameter in self.model.getListOfParameters() if parameter.getValue() != 0], [x.getId() for x in self.model.getListOfParameters() if x.getValue() == 0]
        return parameters,zparam

    def getSpecies(self,translator = {},parameters = []):
        '''
        in sbml parameters and species have their own namespace. not so in
        bionetgen, so we need to rename things if they share the same name
        '''

        moleculesText  = []
        speciesText = []
        observablesText = []
        names = []
        rawSpeciesName = translator.keys()
        speciesTranslationDict = {}
        compartmentDict = {}
        compartmentDict[''] = 1
        for compartment in self.model.getListOfCompartments():
            compartmentDict[compartment.getId()] = compartment.getSize()

        for species in self.model.getListOfSpecies():
            rawSpecies = self.getRawSpecies(species,parameters)
            #if rawSpecies['returnID'] in self.boundaryConditionVariables:
            #    continue
            if (rawSpecies['compartment'] != ''):
                self.tags[rawSpecies['returnID']] = '@%s' % (rawSpecies['compartment'])
            if(rawSpecies['returnID'] in translator):
                if rawSpecies['returnID'] in rawSpeciesName:
                    rawSpeciesName.remove(rawSpecies['returnID'])
                if translator[rawSpecies['returnID']].getSize()==1 \
                and translator[rawSpecies['returnID']].molecules[0].name not in names \
                and translator[rawSpecies['returnID']].molecules[0].name not in rawSpeciesName:
                    names.append(translator[rawSpecies['returnID']].molecules[0].name)
                    moleculesText.append(translator[rawSpecies['returnID']].str2())
            else:
                moleculesText.append(rawSpecies['returnID'] + '()')
            temp = '$' if rawSpecies['isConstant'] != 0 else ''
            tmp = translator[str(rawSpecies['returnID'])] if rawSpecies['returnID'] in translator \
                else rawSpecies['returnID'] + '()'
            if rawSpecies['initialConcentration']>=0:
                tmp2 = temp
                if rawSpecies['returnID'] in self.tags:
                    tmp2 = (self.tags[rawSpecies['returnID']])
                if rawSpecies['initialConcentration'] > 0.0 or rawSpecies['isConstant']:
                    speciesText.append('{0}:{1}{2} {3} #{4} #{5}'.format(tmp2, temp, str(tmp), rawSpecies['initialConcentration'],rawSpecies['returnID'],rawSpecies['identifier']))
            if rawSpecies['returnID'] == 'e':
                modifiedName = 'are'
            else:
                modifiedName = rawSpecies['returnID']
            observablesText.append('Species {0} {1} #{2}'.format(modifiedName, tmp,rawSpecies['name']))
            speciesTranslationDict[rawSpecies['identifier']] = tmp
        sorted(rawSpeciesName,key=len)
        for species in rawSpeciesName:
            if translator[species].getSize()==1 and translator[species].molecules[0].name not in names:
                names.append(translator[species].molecules[0].name)
                moleculesText.append(translator[species].str2())
        #moleculesText.append('NullSpecies()')
        #speciesText.append('$NullSpecies() 1')
        self.speciesMemory = []
        return moleculesText,speciesText,observablesText,speciesTranslationDict

    def getInitialAssignments(self,translator,param,zparam,molecules,initialConditions):
        '''
        process the get initial assigmnetns section. This can be used to initialize
        parameters or species, so we have to account for both checking both arrays
        '''
        param2 = param
        zparam2 = zparam
        initialConditions2 =initialConditions
        pparam = {}
        for element in param:
            pparam[element.split(' ')[0]] =(element.split(' ')[1],None)
        for element in zparam:
            pparam[element] = (0,None)
        for species in self.model.getListOfSpecies():
            tmp = self.getRawSpecies(species)
            name = species.getName() if species.isSetName() else species.getId()
            constant = '$' if species.getConstant() or species.getBoundaryCondition() else ''
            if name in  translator:
                extendedStr = '@{0}:{2}{1}'.format(species.getCompartment(),translator[name],constant)
            else:
                extendedStr = '@{0}:{2}{1}()'.format(tmp['compartment'],standardizeName(tmp['name']),constant)
            initConc = species.getInitialConcentration() if \
            species.isSetInitialConcentration() else species.getInitialAmount()
            pparam[species.getId()] = (initConc,extendedStr)
        from copy import copy
        for initialAssignment in self.model.getListOfInitialAssignments():
            symbol = initialAssignment.getSymbol()
            math = libsbml.formulaToString(initialAssignment.getMath())
            for element in pparam:
                if element in math:
                    math = re.sub(r'(\W|^)({0})(\W|$)'.format(element),
                    r'{0}'.format(pparam[element][0]),math)
            
            param2 = [x for x in param if '{0} '.format(symbol) not in x]
            zparam2 = [x for x in zparam if '{0}'.format(symbol) not in x]
            '''
            if (len(param2) != len(param)) or (len(zparam) != len(zparam2)):
                param2.append('{0} {1}'.format(symbol,math))
                param = param2
                zparam = zparam2
            '''
            try:
                if pparam[symbol][1] == None:
                    param2.append('{0} {1}'.format(symbol,math))
                    param = param2
                    zparam = zparam2
                else:
                    
                    initialConditions2 = [x for x in initialConditions if '#{0}'.format(symbol) not in x]
                    initialConditions2.append('{0} {1} #{2}'.format(pparam[symbol][1],math,symbol))
                    initialConditions = initialConditions2
            except:
                continue
        return param,zparam,initialConditions
            
            
    def getSpeciesAnnotation(self):
        if self.speciesAnnotation:
            return self.speciesAnnotation
        
        self.speciesAnnotation = defaultdict(list)

        for species in self.model.getListOfSpecies():
            rawSpecies = self.getRawSpecies(species,logEntries=False)
            annotationXML = species.getAnnotation()
            lista = libsbml.CVTermList()
            libsbml.RDFAnnotationParser.parseRDFAnnotation(annotationXML,lista)
            if lista.getSize() == 0:
                self.speciesAnnotation[rawSpecies['returnID']] =  None
            else:
                for idx in range(lista.getSize()):
                    self.speciesAnnotation[rawSpecies['returnID']].append(lista.get(idx).getResources())
       
        return self.speciesAnnotation

    def getModelAnnotation(self):
        modelAnnotation = []
        annotationXML = self.model.getAnnotation()
        lista = libsbml.CVTermList()
        libsbml.RDFAnnotationParser.parseRDFAnnotation(annotationXML,lista)
        if lista.getSize() == 0:
            modelAnnotations = []
        else:
            tempDict = {}
            for element in [2,3,4,5,6]:
                if lista.get(element) == None:
                    continue
                tempDict[lista.get(element).getBiologicalQualifierType()] = lista.get(element)
            if 3 in tempDict:
                modelAnnotation = tempDict[3].getResources()
            elif 0 in tempDict and ('GO' in tempDict[0].getResources().getValue(1) or 'kegg' in tempDict[0].getResources().getValue(1)):
                modelAnnotation = tempDict[0].getResources()
            elif 5 in tempDict:
                modelAnnotation = tempDict[5].getResources()
            else:
                if lista.get(3) != None and ('GO' in lista.get(3).getResources().getValue(0) or 'kegg' in lista.get(3).getResources().getValue(0)):
                    modelAnnotation = lista.get(3).getResources()
                    
                elif lista.get(4) != None and ('GO' in lista.get(4).getResources().getValue(0) or 'kegg' in lista.get(4).getResources().getValue(0)):
                    modelAnnotation = lista.get(4).getResources()
                elif lista.get(5) != None and ('GO' in lista.get(5).getResources().getValue(0) or 'kegg' in lista.get(5).getResources().getValue(0)):
                    modelAnnotation = lista.get(5).getResources()
                else:
                    if lista.get(3) != None and ('reactome' in lista.get(3).getResources().getValue(0)):
                        modelAnnotation = lista.get(3).getResources()
                        
                    elif lista.get(4) != None and ('reactome' in lista.get(4).getResources().getValue(0)):
                        modelAnnotation = lista.get(4).getResources()
                    elif lista.get(5) != None and ('reactome' in lista.get(5).getResources().getValue(0)):
                        modelAnnotation = lista.get(5).getResources()
                    elif lista.get(2) != None:
                        modelAnnotation = lista.get(2).getResources()
        return modelAnnotation
        
    def getSpeciesInfo(self, name):
        return self.getRawSpecies(self.model.getSpecies(name))

    def writeLog(self, translator):
        rawSpecies = [self.getRawSpecies(x) for x in self.model.getListOfSpecies()]
        log['species'].extend([x[0] for x in rawSpecies if x[0] not in translator])
        logString = ''
        #species stuff
        if(len(log['species']) > 0):
            logString += "Species we couldn't recognize:\n"
            for element in log['species']:
                logString += '\t%s\n' % element
        if(len(log['reactions']) > 0):
            logString += "Reactions we couldn't infer more about due to \
            insufficient information:"
            for element in log['reactions']:
                logString += '\t%s + %s -> %s\n' % (element[0][0],
                                                    element[0][1],
                                                    element[1])
        return logString

    def getStandardName(self,name):
        if name in self.speciesDictionary:
            return self.speciesDictionary[name]
        return name
    
    
def standardizeName(name):
    '''
    Remove stuff not used by bngl
    '''
    name2 = name
    
    sbml2BnglTranslationDict = {"^":"",
                                "'":"",
                                "*":"m"," ":"_",
                                "#":"sh",
                                ":":"_",'α':'a',
                                'β':'b',
                                'γ':'g',"(":"__",
                                ")":"__",
                                " ":"","+":"pl",
                                "/":"_",":":"_",
                                "-":"_",
                                ".":"_",
                                '?':"unkn",
                                ',':'_',
                                '[':'__',
                                  ']':'__',
                                  '>':'_',
                                  '<':'_'}
                                
    for element in sbml2BnglTranslationDict:
        name = name.replace(element,sbml2BnglTranslationDict[element])
    name = re.sub('[\W]', '', name)
    return name
