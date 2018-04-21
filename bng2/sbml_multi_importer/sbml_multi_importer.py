import sys
import time
import os
import os.path
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
		siteDescriptor = (site.id,site.name)
		full_bindingSite[siteDescriptor] = {}
		if len(site.getListOfSpeciesFeatureTypes())>0:
			for feature in site.getListOfSpeciesFeatureTypes():
				full_bindingSite[siteDescriptor][feature.id] = []		
				for featureVal in feature.getListOfPossibleSpeciesFeatureValues():
					full_bindingSite[siteDescriptor][feature.id].append([featureVal.id,featureVal.name]) 			
	Molecules = {} #A dictionary of the form key: (moelcule_id, molecule_name) value: {dictionary of SpeciesTypes (binding sites) dictionary and FeatureTypes dictionary}	
	for i in molecules:
			full_molecule = (i.id,i.name) 
			Molecules[full_molecule] = {'SpeciesTypes':[],'FeatureTypes':{}} #Molecule types have species types - which are binding sites, and feature types
			list_speciestypeinstances = i.getListOfSpeciesTypeInstances()
			list_speciesfeaturetypes = i.getListOfSpeciesFeatureTypes()
			if len(list_speciestypeinstances)>0:
				for k in list_speciestypeinstances:
					id_ = k.getSpeciesType()
					name_ = k.getName()
					Molecules[full_molecule]['SpeciesTypes'].append([id_,name_])
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
	nmol = len(Molecules)
	keys = Molecules.keys()
	'''
	SIMUNE RELATED. NEEDS FIXING
	for i in range(0,nmol):
		key = keys[i]
		st = Molecules[key]['SpeciesTypes']
		#print st,'st'
		nst = len(st)
		for j in range(0,nst):
			spt = st[j]
			#Check if the species type is a binding site
			spt_id = spt[0]
		#	print spt_id
			if spt_id not in bindingsiteID: #If the species type is not a binding site, flatten things. #FIX!!
				#spt_id must appear as a key 
				tkey = [x[0] for x in keys]
				if spt_id not in tkey:
					print 'Something is wrong'
					return
				index = tkey.index(spt_id)
				#Replace j in st with species types
				stnew = Molecules[keys[index]]['SpeciesTypes']
				Molecules[key]['SpeciesTypes'][j] = []
				for x in stnew:
					Molecules[key]['SpeciesTypes'].append(x)
				ftnew = Molecules[keys[index]]['FeatureTypes']
				Molecules[key]['FeatureTypes'].update(ftnew)	
	'''
	#Update the binding site species types in the Molecules definition with the full binding site dictionary definition
	for i in range(0,nmol):
		key = keys[i]
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
	reader = SBMLReader()
	document = reader.readSBML(filename);
	errors = document.getNumErrors();
	model = document.getModel()

	mplugin = model.getPlugin("multi")
	mst = mplugin.getListOfMultiSpeciesTypes()
	mst1 = model.getListOfSpeciesTypes()
	#print model.getListOfSpecies()
	result = SpeciesTypes(mplugin,model)
	RL = Reactions(model)
	rxnstring(RL,result)
	#print  MolTypesString(result['Molecules'])
	#print SeedSpeciesString(result['Species'],result['Molecules'],result['Complexes'],result['fullBindingSites'])#PartialMolecule(result['Species'],result['Molecules'],result['Complexes'],result['fullBindingSites'])
	#print mplugin.BindingSiteSpeciesType
 	#tmp = parameterBlockString(model)
 	#print tmp['pblock']