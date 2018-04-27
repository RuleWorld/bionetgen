import sys
import time
import os
import os.path
import copy
from libsbml import *
from sbml_multi_aux import *

def SpeciesTypes(mplugin,model):
	speciesTypes = mplugin.getListOfMultiSpeciesTypes()
	s =  speciesTypes[0]
	molecules = []
	bindingsites = []
	complexes = []	
	for i in speciesTypes:
		if i.getElementName() == 'speciesType':
			if len(i.getListOfInSpeciesTypeBonds())>0:
				complexes.append(i)
			else:
				molecules.append(i)
		elif i.getElementName() == 'bindingSiteSpeciesType':
			bindingsites.append(i)
	full_bindingSite={}
	for site in bindingsites:
		#siteDescriptor = site.id#
		siteDescriptor =(site.id,site.name)
		print siteDescriptor
		full_bindingSite[siteDescriptor] = {}
		if len(site.getListOfSpeciesFeatureTypes())>0:
			for feature in site.getListOfSpeciesFeatureTypes():
				full_bindingSite[siteDescriptor][feature.id] = []		
				for featureVal in feature.getListOfPossibleSpeciesFeatureValues():
					full_bindingSite[siteDescriptor][feature.id].append([featureVal.id,featureVal.name]) 			
	Molecules = {} #A dictionary of the form key: (molecule_id, molecule_name) value: {dictionary of SpeciesTypes (binding sites) dictionary and FeatureTypes dictionary}	
	mol_order_index = 0
	for i in molecules:
			full_molecule = (i.id,i.name,mol_order_index)
			mol_order_index+1 #Adding an ordering term so we can print in the right order later. Dictionary does not preserve order. 
			Molecules[full_molecule] = {'SpeciesTypes':[],'FeatureTypes':{}} #Molecule types have species types - which are binding sites, and feature types
			list_speciestypeinstances = i.getListOfSpeciesTypeInstances()
			list_speciesfeaturetypes = i.getListOfSpeciesFeatureTypes()
			species_type_order = 0
			feature_type_order = 0
			if len(list_speciestypeinstances)>0:
				for k in list_speciestypeinstances:
					id_ = k.getSpeciesType()
					name_ = k.getName()
					Molecules[full_molecule]['SpeciesTypes'].append([id_,name_])
					species_type_order+=1
			if len(list_speciesfeaturetypes)>0:
				for k in list_speciesfeaturetypes:
					id_ = k.id
					name_ = k.name
					feature = (id_,name_)
					Molecules[full_molecule]['FeatureTypes'][feature] = []
					for q in k.getListOfPossibleSpeciesFeatureValues():
						id_ = q.id
						name_ = q.name
						Molecules[full_molecule]['FeatureTypes'][feature].append([id_,name_])
						feature_type_order+=1						
			mol_order_index+=1
	print full_bindingSite
	nmol = len(Molecules)
	keys = Molecules.keys()
	'''
	#Simmune adds an extra layer of hierarchy. Molecule-component-bindingsite instead of molecule-bindingsite. Need to flatten for BNG
	bindingsiteID = full_bindingSite.keys()
	for i in range(0,nmol):
		molecule = keys[i]
		if molecule in Molecules.keys():
			speciestype_list = copy.copy(Molecules[molecule]['SpeciesTypes'])
			num_st = len(speciestype_list)
			for speciestype in speciestype_list:
				speciestype_index = speciestype_list.index(speciestype)
				#Check if the species type is a binding site
				speciestypeID = speciestype[0]
				if speciestypeID not in bindingsiteID: #If the species type is not a binding site, flatten things. 
					#speciestypeID must appear as a key
					moleculeID = [x[0] for x in keys]
					if speciestypeID not in moleculeID:
						print 'Something is wrong'
						return
					#Get index of the component definition
					index = moleculeID.index(speciestypeID)
					#Replace species types
					speciestypes_new = Molecules[keys[index]]['SpeciesTypes']
					Molecules[molecule]['SpeciesTypes'].remove(speciestype)
					for x in speciestypes_new:
						x[1] = keys[index][1] + '_'+x[1]+'_'+'bs'
						Molecules[molecule]['SpeciesTypes'].append(x)
					ftnew = Molecules[keys[index]]['FeatureTypes']
					ftnew_modified = {}
					for ft in ftnew:
						ftnew_modified[tuple([ft[0],keys[index][1]+'_'+ft[1]+'_ft'])] = ftnew[ft]
					Molecules[molecule]['FeatureTypes'].update(ftnew_modified)	
					#Delete component from dictionary. Don't want it to show up in Molecule types
					Molecules.pop(keys[index])
	print full_bindingSite
	#Update the binding site species types in the Molecules definition with the full binding site dictionary definition
	'''
	keys = Molecules.keys()
	nmol = len(keys)
	print full_bindingSite
	for i in range(0,nmol):
		key = keys[i]
		#Molecules[key]['SpeciesTypes'] = [{tuple(x):full_bindingSite[x[0]]} for x in Molecules[key]['SpeciesTypes'] if len(x)>0]
		Molecules[key]['SpeciesTypes'] = [{tuple(x):full_bindingSite[tuple(x)]} for x in Molecules[key]['SpeciesTypes'] if len(x)>0]
	Complexes = {} #Considering Complexes, i.e. species with InsSpecies bonds
	for i in complexes:
		full_complex = (i.id,i.name)
		Complexes[full_complex] = {'molecules':[],'bonds':[],'components':[]}
		list_speciestypeinstances = i.getListOfSpeciesTypeInstances()
		list_inspeciesbonds = i.getListOfInSpeciesTypeBonds()
		list_componentindexes = i.getListOfSpeciesTypeComponentIndexes()
		for k in list_speciestypeinstances:
			st_ = k.getSpeciesType()
			id_ = k.getId()
			name_ = k.getName()	
			Complexes[full_complex]['molecules'].append([st_,name_,id_])
		for k in list_inspeciesbonds:
			bs1=k.getBindingSite1()
			bs2=k.getBindingSite2()
			Complexes[full_complex]['bonds'].append([bs1,bs2])
		for k in list_componentindexes:
			id_ = k.getId()
			comp_=k.getComponent()
			ip_ = k.getIdentifyingParent()
			Complexes[full_complex]['components'].append([id_,comp_,ip_])
		#Check if the bond definition is in the components list. If so, replace the bond id, with the [component name,identifying parent].
		for component in Complexes[full_complex]['components']:
			component_name = component[0]
			bondcounter = 0
			for bond in Complexes[full_complex]['bonds']:
				if component_name == bond[0]:
					Complexes[full_complex]['bonds'][bondcounter][0] = [component[1],component[2]]
				elif component_name == bond[1]:
					Complexes[full_complex]['bonds'][bondcounter][1] = [component[1],component[2]]
				bondcounter = bondcounter + 1
	speciesList = model.getListOfSpecies()
	ModelSpecies = {}
	for sp in speciesList:
		sp_plugin = sp.getPlugin('multi')
		id_ = sp.getId()
		ModelSpecies[id_] = {'name':sp.name,'speciesType':sp_plugin.getSpeciesType(),'bindingsites':[],'features':[],'wildcards':[],'ic':sp.getInitialConcentration()}
		outwardbindingsites = sp_plugin.getListOfOutwardBindingSites()
		speciesfeatures = sp_plugin.getListOfSpeciesFeatures()
		#outward binding sites are binding sites not involved in in species bonds.
		for q in  outwardbindingsites:
			if q.getBindingStatus() == 1: # 1 signifies an "unbound" bindingStatus, 0 signifies bound and 2 signifies either/don't care state
				ModelSpecies[id_]['bindingsites'].append(q.getComponent())
			elif q.getBindingStatus() == 0:
				ModelSpecies[id_]['wildcards'].append(q.getComponent())
		for q in speciesfeatures:
			spfl = q.getListOfSpeciesFeatureValues()
			ModelSpecies[id_]['features'].append([q.getSpeciesFeatureType(),spfl[0].getValue(),q.getComponent()])
	Result = {'Molecules':Molecules,'Complexes':Complexes,'Species':ModelSpecies,'fullBindingSites':full_bindingSite}
	return Result
	
def parameterBlockString(model):
	pblock = 'begin parameters\n'
	plist = model.getListOfParameters()
	for i in plist:
		pblock = pblock + plist[0].id +'\t'+str(plist[0].value)+'\n'
	pblock = pblock + 'end parameters\n'
	return {'pblock':pblock,'plist':plist}

def MolTypesString(m):
	s = toString(m)
	moltypes = 'begin molecule types \n'
	for j in s:
		moltypes = moltypes + s[j]+'\n'
	moltypes = moltypes + 'end molecule types \n'
	return moltypes
	
if __name__ == "__main__":
	filename = sys.argv[1];
	base = filename.split('.')
	reader = SBMLReader()
	document = reader.readSBML(filename);
	errors = document.getNumErrors();
	model = document.getModel()
	mplugin = model.getPlugin("multi")
	mst = mplugin.getListOfMultiSpeciesTypes()
	mst1 = model.getListOfSpeciesTypes()
	result = SpeciesTypes(mplugin,model)
	#getRules(model,result)
	#print model.getListOfSpecies()
	#
	#RL = Reactions(model)
	#rxnstring(RL,result)
	bng = GetBNGFile(model,result)
	#f = open(base[0]+'.bngl','w')
	#f.write(bng)
	#f.close()
	#print  MolTypesString(result['Molecules'])
	#print SeedSpeciesString(result['Species'],result['Molecules'],result['Complexes'],result['fullBindingSites'])#PartialMolecule(result['Species'],result['Molecules'],result['Complexes'],result['fullBindingSites'])
	#print mplugin.BindingSiteSpeciesType
 	#tmp = parameterBlockString(model)
 	#print tmp['pblock']