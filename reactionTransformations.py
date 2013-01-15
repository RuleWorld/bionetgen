from copy import copy,deepcopy
import random
import sys
import libsbml2bngl
import structures as st
import itertools


class InsufficientInformationError(Exception):
     def __init__(self):
         pass

def printSpecies(label,definition):
    molecules = []
    for tag, species in zip(label,definition):
        components = []
        for member in species:
            if len(member) == 2:
                components.append('{0}!{1}'.format(member[0], member[1]))
            else:
                components.append('{0}'.format(member[0]))
        molecules.append('{0}({1})'.format(tag,",".join(components)))
    return ".".join(molecules)

def issubset(possible_sub, superset):
    return all(element in superset for element in possible_sub)



def getFreeRadical(element,rawDatabase,translator,product,dictionary):
    """
    this method is used when the user does not provide a full binding specification
    it searches a molecule for a component that has not been used before in another reaction
    in case it cannot find one it returns an empty list
    """
    
    
    if element not in rawDatabase:
        return []
    
    #components = copy(rawDatabase[element][0])
    components2 = [x[0][0] for x in rawDatabase[element][0]]
    #components2 = copy(rawDatabase[element][0]) 
    '''
    for member in synthesisDatabase:
        ##the last condition issubset is in there because knowing that S1 + s2 -> s1.s2 
        #does not preclude s2 from using the same bnding site in s1.s3 + s2 -> s1.s2.s3
        #TODO: removed condition as a temporal fix. Verify that it doesn't break things
        #elsewherez
        if element[0] in member: #and not issubset(member,product):
            index = member.index(element[0])
            #FIXME fix how things are stored in synthesisDatabase with bigger complexes
            #when we are saving information from the previous reaction
            #what i did yesterday: changed things from extended[12] to binding[12] in the
            #findintersection method
            for temp in [x for x in synthesisDatabase[member][index] if len(x) > 1]:
                if (temp[0],) in components:
                    components.remove((temp[0],))
    '''
    for member in translator:
        molecules = [x for x in translator[member].molecules if x.name == element[0]]
        if len(translator[member].molecules) > 1 and len(molecules) > 0:
            for molecule in molecules:
                for component in molecule.components:
                    if len(component.bonds) > 0 and component.name in components2:
                        components2.remove(component.name)
    if not components2:
        return []
    return components2[0]
'''
def findIntersection(set1,set2,translator):
    intersections2 = []
    intersectionElements = []
    for tag,element in [(x,translator[x]) for x in translator if translator[x].getSize() > 1]:
        intersection1 = [x for x in element.getMoleculeNames() if x in set1]
        intersection2 = [x for x in element.getMoleculeNames() if x in set2]
        notInEither = [x for x in element.getMoleculeNames() if x not in set1 and x not in set2]
        if len(intersection1)>0 and len(intersection2)>0 and len(notInEither) == 0:
            intersections2.append(tuple(element.getMoleculeNames()))
            intersectionElements.append(tag)

    return intersectionElements,intersections2
'''

def findIntersection(set1,set2,database):
    """
    this method receives a list of components and returns an element from the database
    that is a subset of the union of both sets
    """
    intersections = []
    #print database
    if set1 == set2:
        for element in [x for x in database.keys() if len(x) > 1]:
            if [element.count(x) for x in set1].count(2) == len(set1):
                intersections.append(element)
            else:
                return []
        #for element in [x for x in database.keys() if ]      
    else:
        for element in [x for x in database.keys() if len(x) > 1]:
            intersection1 = [x for x in element if x in set1]
            intersection2 = [x for x in element if x in set2]
            notInEither = [x for x in element if x not in set1 and x not in set2]
            if len(intersection1)>0 and len(intersection2)>0 and len(notInEither) == 0:
                intersections.append(element)
            
    return intersections

    
def synthesis(original,dictionary,rawDatabase,synthesisDatabase,translator,outputFlag=False):
    #reaction = []
    for elements in original:
        #temp = []  
        for sbml_name in elements:
            ## If we have translated it before and its in mem   ory
 #           if molecule in translator:
 #               species.append(translator[molecule])
 #           else:
            if outputFlag:
                print '-',sbml_name
            tags,molecules = findCorrespondence(original[0],original[1],dictionary,sbml_name,rawDatabase,synthesisDatabase,translator,outputFlag)
            if (tags,molecules) == (None,None):
                tmp = st.Species()
                tmp.addMolecule(st.Molecule(sbml_name))
                if sbml_name not in translator:
                    translator[sbml_name] = tmp
                    #raise InsufficientInformationError
                libsbml2bngl.log['reactions'].append(original)
            #TODO: probably we will need to add a check if there are several ways of defining a reaction
            elif isinstance(molecules,st.Species):
                #FIXME: there shouls be a better way to check whether i actually want to check or not
                if tags not in translator:
                    translator[tags] = molecules
            else:
                #tags = list(tags)
                #tags.sort()
                #tags = tuple(tags)
                precursors = []
                if sbml_name not in translator:
                    species = st.Species()
                    #here we check if the new species is made of already existing molecules
                                            

                else:
                    species = translator[sbml_name]
                species.addChunk(tags,molecules,precursors)
                if sbml_name not in translator:
                    other = original[0] if original[0] != elements else original[1]
                    for tag in [x for x in other if x in translator]:
                        species.extend(translator[tag])
                translator[sbml_name] = species
                if tags not in synthesisDatabase and tags not in rawDatabase:
                    synthesisDatabase[tags] = tuple(molecules)
    return 0
        #reaction.append(temp)
    #return reaction

def getBindingPoints(extended1,extended2,reactants,product):
    #ideally we'd want to use user information
        
    #naming convention binding
    
    if len(extended1) == 1:
        bindingComponent1 = extended1[0]
    else:
        scores = []
        for element in extended1:
            score = 0
            score += len([x for x in element if x in product])
            score -= len([x for x in element if x not in product])
            score -= len([x for x in product if x not in element])
            scores.append(score)
        bindingComponent1 = extended1[scores.index(max(scores))]
            
    if len(extended2) == 1:
        bindingComponent2 = extended2[0]
    else:
        scores = []
        for element in extended2:
            score = 0
            score += len([x for x in element if x in product])
            score -= len([x for x in element if x not in product])
            score -= len([x for x in product if x not in element])
            scores.append(score)
        bindingComponent2 = extended2[scores.index(max(scores))]
    
    return bindingComponent1,bindingComponent2

def reverseFind(name,dictionary):
    for element in dictionary:
        if dictionary[element] == name:
            return element
        
def getIntersection(reactants,product,dictionary,rawDatabase,translator,synthesisDatabase,originalProductName,outputFlag=False):
    '''
    this method goes through two complexes and tries to check how they
    get together to create a product (e.g. how their components link)
    either by using previous knowledge or by creating a new complex
    '''
    #global log

    extended1 = (copy(dictionary[reactants[0]]))
    extended2 = (copy(dictionary[reactants[1]]))
    if isinstance(extended1,str):
        extended1 = [extended1]
    if isinstance(extended2,str):
        extended2 = [extended2]
    #if we can find an element in the database that is a subset of 
    #union(extended1,extended2) we take it
    intersection = findIntersection(extended1,extended2,synthesisDatabase)
    #otherwise we create it from scratch
    if not intersection:
        r1 = getFreeRadical(extended1,rawDatabase,translator,product,dictionary)
        r2 = getFreeRadical(extended2,rawDatabase,translator,product,dictionary)
        binding1,binding2 = getBindingPoints(extended1,extended2,reactants,originalProductName[0])
        if not r1 or not r2:
            #prin   t 'Cannot infer how',extended1,'binds to',extended2
            #log['reactions'].append((reactants,product))
            #return None,None,None
            #TODO this section should be activated by a flag instead
            #of being accessed by default
            #print extended1,extended2
            
            createIntersection((binding1,binding2),rawDatabase,translator,dictionary)
            r1 = getFreeRadical((binding1,),rawDatabase,translator,product,dictionary)
            r2 = getFreeRadical((binding2,),rawDatabase,translator,product,dictionary)
            #print 'rrrrrrrrrrr',r1,r2
            if not r1 or not r2:
                return (None,None,None,None)
        
        ##todo: modify code to allow for carry over free radicals
        #FIXME: we can remove synthesisDatabase easily
        species = st.Species()
        if reactants[0] in translator:
            species.append(translator[reactants[0]])
        if reactants[1] in translator:
            species.append(translator[reactants[1]])
        if outputFlag:
            print '-----------',species,reactants[0],reactants[1],reactants[1] in translator
            print '+++',binding1,binding2,r1,r2
            
        bondName = max(species.getBondNumbers()) + 1
        molecule1 = st.Molecule(binding1)
        molecule2 = st.Molecule(binding2)
        component1 = st.Component(r1)
        component2 = st.Component(r2)
        component1.addBond(str(bondName))
        component2.addBond(str(bondName))
        molecule1.addComponent(component1)
        molecule2.addComponent(component2)
        
        if outputFlag:
            print '////////',molecule1,molecule2
        species.addMolecule(molecule1,True,1)
        counter = 2 if binding1 == binding2 else 1 
        species.addMolecule(molecule2,True,counter)
        if outputFlag:
            print '\\\\\\',species
        ####TODO: update the rawDAtabase with the m1m2 information
        sp = st.Species()
        sp.addMolecule(deepcopy(molecule1))
        if molecule1.name in translator:
            translator[molecule1.name].extend(sp)
        else:
            translator[molecule1.name] = sp
        translator[molecule1.name].reset()
        sp = st.Species()
        sp.addMolecule(deepcopy(molecule2))
        if molecule2.name in translator:
            translator[molecule2.name].extend(sp)
        else:
            translator[molecule2.name] = sp
        translator[molecule2.name].reset()
        if outputFlag:
            print '||||||||||||||||||||||',translator[molecule2.name]
        #print reactants,product,str(species)
        #print name1,name2,extended1,extended2
        #print {x:str(translator[x]) for x in translator}, translator
        if len(species.molecules) == 0:
            return (None,None,None,None)
        return species,[],[],[]
    return extended1,extended2,intersection,[]

def merge(reactant1,reactant2,r1,r2,translator,outputFlag=False):
    '''
    Receives two species reactant1 and reactant1, and their intersection points 
    r1 and r2 and creates a new complex that is the union
    '''
    species = st.Species()
    if reactant1 in translator:
        species.append(translator[reactant1])
    if reactant2 in translator:
        species.append(translator[reactant2])
    if outputFlag:
        print '-----------',species,reactant1,reactant2,reactant2 in translator
    rnd = max(species.getBondNumbers()) + 1
    molecule1 = st.Molecule(binding1)
    molecule2 = st.Molecule(binding2)
    component1 = st.Component(r1)
    component2 = st.Component(r2)
    component1.addBond(str(rnd))
    component2.addBond(str(rnd))
    molecule1.addComponent(component1)
    molecule2.addComponent(component2)
    species.addMolecule(molecule1,True,1)
    counter = 2 if binding1 == binding2 else 1 
    species.addMolecule(molecule2,True,counter)
    
    ####TODO: update the rawDAtabase with the m1m2 information
    if molecule1.name in translator:
        sp = st.Species()
        sp.addMolecule(deepcopy(molecule1))
        translator[molecule1.name].extend(sp)
        translator[molecule1.name].reset()
    
    if molecule2.name in translator:
        sp = st.Species()
        sp.addMolecule(deepcopy(molecule2))
        translator[molecule2.name].extend(sp)
        translator[molecule2.name].reset()

    
def createIntersection(reactants,rawDatabase,translator,dictionary):
    '''
    if there are no components in record that allow for two species to merge
    we will create those components instead
    '''
   
    #label = dictionary[reactants[0]] if isinstance(dictionary[reactants[0],str) else reactants[0]
    if (reactants[0],) not in rawDatabase:
        rawDatabase[(reactants[0],)] = ([([reactants[1].lower()],)],)
    else:
        temp = rawDatabase[(reactants[0],)][0]
        temp.append(([reactants[1].lower()],))
        rawDatabase[(reactants[0],)] = (temp,)
    if reactants[0] == reactants[1]:
        return
    #label = dictionary[reactants[1]] if isinstance(dictionary[reactants[1],str) else reactants[1]
    if (reactants[1],) not in rawDatabase:
        rawDatabase[(reactants[1],)] = ([([reactants[0].lower()],)],)
    else:
        temp = rawDatabase[(reactants[1],)][0]
        temp.append(([reactants[0].lower()],))
        rawDatabase[(reactants[1],)] = (temp,)
        
def findCorrespondence(reactants,products,dictionary,sbml_name,rawDatabase,synthesisDatabase,translator,outputFlag=False):
    """
    this method reconstructs the component structure for a set of sbml
    molecules that undergo synthesis using context and history information    
    """   
    #print reactants,products,dictionary,sbml_name
    species = dictionary[sbml_name]
    if isinstance(species,str):
        species = dictionary[species]
    product = dictionary[products[0]]
    #if the element is already in the synthesisDatabase
    if species in synthesisDatabase:
        return species,synthesisDatabase[species]
    if products[0] in translator:
        return None,None
    #elif species in rawDatabase:
        #for product in productArray:
        #if product in synthesisDatabase:
        #    temp = [(x[0],) for x in synthesisDatabase[product][product.index(dictionary[species[0]][0])]]
        #    return species,(temp,)
        #print '::::::::::',species,rawDatabase[species]
        #return species,rawDatabase[species]
    #this means we are dealing with the basic components rather than the
    #synthetized product
    extended1,extended2,intersection,extendedComponent = getIntersection(reactants,product,
                                                       dictionary,rawDatabase,translator,
                                                      synthesisDatabase,products,outputFlag)
                                                     
    #print 'aaaaa',dictionary
    if outputFlag:
        print '---',extended1,extended2,intersection,extendedComponent
    if len(species) <2 and species in rawDatabase:
        return species,rawDatabase[species]
    if (extended1,extended2,intersection) == (None,None,None):
        return None,None
    if isinstance(extended1,st.Species):
        return products[0],extended1
    constructed = [[] for element in species]
    #add elements preexistent in the first molecule
    print '2........'
    species2 = st.Species()
    species2.append(translator[reactants[0]])
    species2.append(translator[reactants[1]])
    species2.addChunk(intersection[0],synthesisDatabase[intersection[0]],[])
    print intersection[0],synthesisDatabase[intersection[0]]
    #add elements preexistent in the second molecule
    #add the intersection
    
    '''
    for element in [intersection[0],extended1,extended2]:
        if len(element) > 1:
            for tag,molecule in zip(element,synthesisDatabase[element]):
                
                #in case we know that one element name is actually another in a special form
                realTag = dictionary[tag][0]
                for member in [x for x in molecule if x not in constructed[species.index(realTag)]]:
                    flag = True
                    for temp in deepcopy(constructed[species.index(realTag)]):
                        if member[0] in temp:
                            if len(member) == 1:
                                flag = False
                            else:
                                constructed[species.index(realTag)].remove(temp)
                    if flag:
                        constructed[species.index(realTag)].append(tuple(member))
    '''
    return species,species2
    #return tuple(species),constructed

def creation(original,dictionary,rawDatabase,translator):
    """
    This method deals with reactions of the type 0 -> A
    """ 
    reaction = [[0],[]]
    reaction[1] = [(tuple(original[1]),rawDatabase[tuple(original[1])],)]
    return reaction

def decay(original,dictionary,rawDatabase,translator):
    """
    This method deals with reactions of the type A -> 0
    """
    reaction = [[],[0]]
    reaction[0] = [(tuple(original[0]),rawDatabase[tuple(original[0])],)]
    return reaction   
    
    
def addRawDatabaseState(reactant,state,rawDatabase):
    if (reactant,) not in rawDatabase:
        rawDatabase[(reactant,)] = ([([state],)],)
    else:
        temp = rawDatabase[(reactant,)][0]
        temp.append(([state],))
        rawDatabase[(reactant,)] = (temp,)
        
        
def catalyze(original,modified,namingConvention,rawDatabase,translator,reactionProperties):
    result = [0,0]
    siteName = reactionProperties[namingConvention][0]
    stateName = reactionProperties[namingConvention][1]
    if (original,) not in translator:
        result[0] = [siteName,'U']
        #rawDatabase[(original,)] = ([([getCatalysisSiteName(namingConvention),'U'],)],)
    else:
        siteName = reactionProperties[namingConvention][0]
        if siteName in rawDatabase[(original,)][0]:
            if not 'U' in rawDatabase[(original,)][1]:
                temp = list(rawDatabase[(original,)])
                #todoL we ave to actually get an index, not blindly append
                temp[1].append('U')
                result[0] = temp
                #addRawDatabaseState(original,tuple(temp),rawDatabase)
                #rawDatabase[(original,)] = tuple(temp)
        else:
            temp = list(rawDatabase[(original,)])
            temp[0].append(siteName)
            temp[1].append('U')
            #addRawDatabaseState(original,tuple(temp),rawDatabase)
            #rawDatabase[(original,)] = tuple(temp)
            result[0] = temp
    if (modified,) not in translator:
        
        #rawDatabase[(modified,)] = ([([getCatalysisSiteName(namingConvention),getStateName(namingConvention)],)],)
        result[1] = [siteName,stateName]
        #addRawDatabaseState(modified,([getCatalysisSiteName(namingConvention),getStateName(namingConvention)],),rawDatabase)
    else:
        if siteName in rawDatabase[(modified,)][0]:
            if not stateName in rawDatabase[(modified,)][1]:
                temp = list(rawDatabase[(modified,)])
                #TODO: in the special case for generic-catalysis, we do it so that any additional modification to an already
                #modified state is done via an additional M. ['M','MM' etc]. Check if it makes sense for other modifications
                if namingConvention == 'Generic-Catalysis':
                    longestStateName = max(temp[1],key=len)
                    temp[1].append(stateName+longestStateName)
                else:
                    temp[1].append([stateName])
                #addRawDatabaseState(modified,tuple(temp),rawDatabase)
                #rawDatabase[(modified,)] = tuple(temp)
                result[1] = tuple(temp)
        else:
            temp = list(rawDatabase[(modified,)]) 
            temp[0].append(siteName)
            temp[1].append(stateName)
            #addRawDatabaseState(modified,tuple(temp),rawDatabase)
            #rawDatabase[(modified,)] = tuple(temp)
            result[1] = temp
    
    return result

def rebalance(original,sortedResult,translator):
    
    print original
    reactantsCount = {}
    for element in translator[original[0][0]].molecules:
        if element.name not in reactantsCount:
            reactantsCount[element.name] = 0
        reactantsCount[element.name] += 1
    for product in original[1]:
        for element in translator[product].molecules:
            if element.name not in reactantsCount:
                reactantsCount[element.name] = 0
            reactantsCount[element.name] -= 1         
    for element in reactantsCount:
        if reactantsCount[element] < 0:
            translator[sortedResult[1]].deleteMolecule(element)
            
    
            
def catalysis(original,dictionary,rawDatabase,catalysisDatabase,translator,
              namingConvention,classification,reactionProperties):
    """
    This method is for reactions of the form A+ B -> A' + B
    """
    result = catalyze(namingConvention[0],namingConvention[1],classification,rawDatabase
    ,translator,reactionProperties)
    k = [x  == min(namingConvention,key=len) for x in original[0]]
    k2 = [x == max(namingConvention,key=len)  for x in original[1]]
    k =  k and k2
    sortedResult = [result[0],result[1]] if any(k) else [result[1],result[0]]
    sortedConvention = [namingConvention[0],namingConvention[1]] if any(k) else [namingConvention[1],namingConvention[0]]
    flag = False
    
    if 'EGF_EGFRm2' in original[0] or 'EGF_EGFRm2' in original[1]:
        print '---',sortedResult,sortedConvention,original
    for reactantGroup,res,conv in zip(original,sortedResult,sortedConvention):
        for reactant in reactantGroup:
            flag = False
            species = st.Species()
            if original[0][0] in translator:
                species = deepcopy(translator[original[0][0]])
            tmp = dictionary[reactant]
            for element in tmp:
                molecule = st.Molecule(element)
                if element in conv:
                    #chunk = result[1] if reactant == max(namingConvention,key=len) else result[0]
                    component = st.Component(res[0])
                    component.addState(res[1])
                    molecule.addComponent(component,1)
                    flag = True
                    finalMolecule = molecule
                '''
                else:
                    if conv in reactant:
                        
                        component = st.Component(res[0])
                        component.addState(res[1])
                        molecule.addComponent(component,1)
                        print conv,molecule,element
                        flag = True
                        #continue
                '''                    
                    #FIXME: the comparison should be done a lil more carefully
                    #to avoid overlap
            
 
                    
                species.addMolecule(molecule,True)
                if str(species) == '':
                    species.addMolecule(molecule)
            if flag:
                
                if reactant not in translator:
                    translator[reactant] = species

                else:
                    translator[reactant].extend(species,False)
                if finalMolecule.name in translator:
                    if len(translator[finalMolecule.name].molecules) == 1:
                        sp = st.Species()
                        sp.addMolecule(deepcopy(finalMolecule))
                        translator[finalMolecule.name].extend(sp,False)
                        translator[finalMolecule.name].reset()
                else:
                        sp = st.Species()
                        sp.addMolecule(molecule)
                        translator[molecule.name] = sp
                
    if len(original[0]) < len(original[1]):
        rebalance(original,sortedConvention,translator)
               #if len(translator[reactant].molecules) > 1 and molecule.name in translator:
                #    sp = st.Species()
                #    sp.addMolecule(molecule,)
                #    translator[molecule.name].extend(sp)
    
    #if namingConvention[0] not in translator:
    #    species = st.Species()
    #    species.addChunk([dictionary[namingConvention[0]][0]],result[0])
    #    translator[namingConvention[0]] = species
    #species = st.Species()
    #species.addChunk([dictionary[namingConvention[1]][0]],result[1])
    #translator[namingConvention[1]] = species