import sys
import re
import copy

def getRules(model,res):
	s = res['Species']
	m = res['Molecules']
	c = res['Complexes']
	b = res['fullBindingSites']
	result = PartialMolecule(s,m,c,b)
	pm = result['pm']
	pc = result['pc']

	pmstr = toString(pm)
	pcstr = Complex2String(pc)

	rules = model.getListOfRules()
	rulevar = []
	for rule in rules:
		rulevar.append(rule.variable)
	functionstring = 'begin functions\n'
	observablesstring = 'begin observables\n'
	for rule in rules: #iterate through assignment rules
		rf = rule.getMath()		
		l = rf.getListOfNodes()
		possible_observable = []
		counter=0
		for i in range(0,l.getSize()): #all the nodes in the assignment rule
			a= l.get(i)
			if a.isName(): #If the node type has a string name, i.e. a variable name
				#If all the node names appear as previously defined function names, then according to BNG syntax this is a function
				counter = counter+1 #count the number of variables
				if a.getName() in rulevar:
					flag = 1
				else:
					flag = 0
					possible_observable.append(a.getName())
		if flag==1:
			functionstring = functionstring+rule.variable+'() '+rule.getFormula()+'\n'
		elif flag==0:
			#Some additional checks to make sure this is a bionetgen observable
			if counter==1: #An observable formula should only have one variable (CHECK!)
				observable_species = possible_observable[0]
				if observable_species in pmstr.keys():
					observablesstring = observablesstring+'Molecules '+rule.variable+' '+pmstr[observable_species]+'\n'
	observablesstring = observablesstring + 'end observables\n'
	functionstring = functionstring+'end functions\n'
	return {'functions':functionstring,'observables':observablesstring}

def Reactions(model):
	plist = parameterBlockString(model)['plist']
	par = [x.id for x in plist]
	rxns = model.getListOfReactions()
	reactants = {}
	products = {}
	parameters = {}
	nrxns = len(rxns)	
	ReactionList = []
	localparlist = []
	for i in range(0,nrxns):
		tmp = {}
		rxn = rxns[i]
		reactant = []
		tmp['annotation'] = rxn.getName()
		tmp['reversible'] = rxn.getReversible()
		tmp['reactants'] = []
		tmp['products'] = []
		tmp['parameters'] = []
		lreact = rxn.getListOfReactants()
		lprod = rxn.getListOfProducts()
		klaw = rxn.getKineticLaw()
		for reactant in lreact:
			tmp['reactants'].append(reactant.getSpecies())
		for product in lprod:
			tmp['products'].append(product.getSpecies())
		localpar = klaw.getListOfLocalParameters()
		for p in localpar:
			localparlist.append([p.getId(),p.getValue()])
		'''
		flag = 0
		for j in rxn:
			elif j.tag == prefix+'kineticLaw':
				for k in j:
					if k.tag == prefix+'listOfLocalParameters':
						flag = 1
						for q in k:
							tmp['full_parameters'].append({dict(q.attrib)['id']:dict(q.attrib)['value']})'''
		
		'''
		if flag == 0:
			for j in rxn:
				for k in j:
					if 'math' in k.tag:
						for ci in k[0]:
							if ci.text != None and ci.text.strip() not in tmp['reactants']: 
								tmp['parameters'].append({ci.text:[pmap[ci.text.strip()]]})'''

		klaw_formula = klaw.getFormula()
		klcomp = klaw_formula.split('*')
		for i in klcomp:
			if i.strip() not in tmp['reactants']:
				tmp['parameters'].append(i.strip())
		ReactionList.append(tmp)				
	return ReactionList


def rxnstring(RL,res):
	s = res['Species']
	m = res['Molecules']
	c = res['Complexes']
	b = res['fullBindingSites']
	result = PartialMolecule(s,m,c,b)
	pm = result['pm']
	pc = result['pc']
	string_pm = toString(pm)
	string_pc = Complex2String(pc)
	reactantpatterns = []
	productpatterns = []
	reversible = []
	for i in RL:
		rpattern = []
		ppattern = []	
		if i['reversible'] != False:
			reversible.append(' <-> ')
		else:
			reversible.append(' -> ')
		for j in i['reactants']:
			if j in string_pm.keys():
				rpattern.append(string_pm[j])
			elif j in string_pc.keys():
				rpattern.append(string_pc[j])
		for j in i['products']:
			if j in string_pm.keys():
				ppattern.append(string_pm[j])
			elif j in string_pc.keys():
				ppattern.append(string_pc[j])
		if len(i['products']) == 0:
			ppattern = ['0']
		reactantpatterns.append(rpattern)
		productpatterns.append(ppattern)
	rxnstring = 'begin reaction rules \n'
	for i in range(0,len(RL)):
		if RL[i]['annotation'] == '':
			RL[i]['annotation'] = 'R'+str(i)
		for pattern in reactantpatterns[i]:
			rxnstring = rxnstring+pattern+' + '
		rxnstring = rxnstring[:-2]+reversible[i]
		for pattern in productpatterns[i]:
			rxnstring = rxnstring+pattern+' + '
		rxnstring = rxnstring[:-2]+'\t'
		for parameter in RL[i]['parameters']:
			rxnstring = rxnstring+parameter+','
		rxnstring = rxnstring[:-1]+'\n'
	rxnstring = rxnstring+'end reaction rules \n'
	return rxnstring

def toString(Molecules):
	mol_string = {}
	for i in  Molecules:
		mol_name = i[1]
		m = mol_name+'('
		st = Molecules[i]['SpeciesTypes']
		ft = Molecules[i]['FeatureTypes']
		if 'wildcards' in Molecules[i].keys():
			wc = Molecules[i]['wildcards']
		else:
			wc = []
		list_of_st = []
		flag = 0
		for j in st:
			tmp = j[j.keys()[0]]
			if len(tmp) ==0:
				if j.keys()[0][1] != '':
					m = m+j.keys()[0][1] +','
				else:
					m = m + j.keys()[0][0] +','
				flag = 1
			else:
				feature_values = tmp[tmp.keys()[0]]
				m = m +j.keys()[0][1]
				for fv in feature_values:
					m = m + '~' +fv[1]
				m = m+','
				flag = 1

		if len(wc)>0:
			for j in wc:
				tmp = j[j.keys()[0]]
				if len(tmp)==0:
					if j.keys()[0][1] != '':
						m = m+j.keys()[0][1] +'!+,'
					else:
						m = m + j.keys()[0][0] +'!+,'
					flag = 1
				else:
					feature_values = tmp[tmp.keys()[0]]
					m = m +j.keys()[0][1]
					for fv in feature_values:
						m = m + '~' +fv[1]
					m = m+'!+ '
					flag = 1

		for j in ft.keys():
			m = m+j[1]
			if any(isinstance(el, list) for el in ft[j]):
				for k in ft[j]:
					m = m+'~'+k[1]
			else:
				m = m +'~'+ft[j][1]
			m = m+','
			flag = 1
		if flag == 0:
			m = m+')'
		else:
			m = m[:-1]+')'
		mol_string[i[0]] = m
	return mol_string

def Mol2String(Molecules):
	mol_string = {}
	for mol in  Molecules:
		mol_order = mol[2] #order in which molecule should be printed
		mol_name = mol[1] #Get molecule name
		m = mol_name+'('
		st = Molecules[mol]['SpeciesTypes'] #List of binding site dictionaries. 
		ft = Molecules[mol]['FeatureTypes'] #Dictionary of feature sites. Key: (site ID, site name) Value: list of value ID,value pairs
		if 'wildcards' in Molecules[mol].keys():
			wc = Molecules[mol]['wildcards']
		else:
			wc = []
		list_of_st = []
		flag = 0
		for site in st:
			#site is a dictionary of with one key and one value. 
			#Key = (site id, site name) Value: Dictionary of features {feature ID: [[feature value ID 1, feature value 1],...[feature value ID N, feature value N]]}
			site_def = site.keys()[0]
			site_features = site.values()[0] #This is a dictionary of length 1. key is the feature ID, value is list of feature value ID,value pairs
			j = site
			tmp = j[j.keys()[0]]
			if len(site_features) ==0: #binding site has no features
				if site_def[1] != '': #Binding site has a name
					m = m+site_def[1] +','
				else: #Use the binding site ID instead of the name
					m = m + site_def[0] +','
				flag = 1
			else: #binding site does have features
				feature_values = site_features.values()[0]
				m = m +site_def[1] #binding site name
				for fv in feature_values:
					m = m + '~' +fv[1] #feature values separated by BNG connector syntax
				m = m+',' #Comma separats sites
				flag = 1
		if len(wc)>0:
			for j in wc:
				tmp = j[j.keys()[0]]
				if len(tmp)==0:
					if j.keys()[0][1] != '':
						m = m+j.keys()[0][1] +'!+,'
					else:
						m = m + j.keys()[0][0] +'!+,'
					flag = 1
				else:
					feature_values = tmp[tmp.keys()[0]]
					m = m +j.keys()[0][1]
					for fv in feature_values:
						m = m + '~' +fv[1]
					m = m+'!+ '
					flag = 1
		for site in ft.keys():
			m = m+site[1] #site name
			if any(isinstance(el, list) for el in ft[site]): #Making sure that elements are lists 
				for featureval in ft[site]:
					m = m+'~'+featureval[1]
			else:
				m = m +'~'+ft[j][1]
			m = m+','
			flag = 1
		if flag == 0: #No sites in the molecule
			m = m+')'
		else:
			m = m[:-1]+')'
		mol_string[(mol[0],mol_order)] = m
		mol_order += 1
	return mol_string

def Complex2String(complexes):
	complex_string = {}
	for cmplex in complexes:
		c = ''
		for participating_mol in complexes[cmplex]['molecules']:
			molecule_name = toString(participating_mol).values()[0] #Participating molecule e.g.X(y,z)
			mol = participating_mol.keys()[0][1]#Participating molecule name, e.g. X
			#mol  = mol.replace('(','\(')
			#mol = mol.replace(')','\)') 
			counter = 0
			bondCounter = {}
			for b in complexes[cmplex]['bonds']: #iterate through bonds in complex 
				#b is a list of the two binding sites ([site id, site name, parent ID]) in this bond. 
				counter = counter + 1
				#get list of binding sites on participating molecule
				st = [list(x.keys()[0]) for x in participating_mol[participating_mol.keys()[0]]['SpeciesTypes']]
				bondCounter[(b[0][0],b[1][0])] = counter #key of the bondCounter dict is a tuple of binding site IDs			
				for bindingsite in b:
					#if the binding site is in the participating molecule binding site list
					#and the parent ID matches the participating molecule ID
					if [bindingsite[0],bindingsite[1]] in st and bindingsite[2] == participating_mol.keys()[0][2]:
						bst = bindingsite[1] #get binding site name
						#find the binding site name in the molecule name string
						m = re.search(mol+'\(\s*'+bst+'\s*[\,\)]',molecule_name) #e.g a in A(a, ...) Instace of binding site such as molecule name ( white space binding site name white space either comma or )
						if m==None: # e.g. b in A(a,b, or b in A(a,b)
							m = re.search(mol+'\(.*\,\s*'+bst+'\s*[\,\)]',molecule_name)
						if m!=None:
							index = molecule_name.find(m.group(0))+len(m.group(0)) #find the index at which to insert the bond character					
							tmp = bondCounter.keys() #binding sites in bond
							ind = 1e29
							for tmp1 in tmp:
								if bindingsite[0] == tmp1[0] or bindingsite[0] == tmp1[1]: #if the binding site is in the list of binding sites
									ind = bondCounter[tmp1] #get the bond index
									break
							molecule_name_new = molecule_name[0:index-1]+'!'+str(ind)+molecule_name[index-1:]
							molecule_name = molecule_name_new
						else: #e.g. b in A(a,b~0~p)
							m = re.search(mol+'\(\s*'+bst+'\s*\~\w+\s*[\,\)]',molecule_name) #A(a~0)
							if m == None:
								m = re.search(mol+'\(.*\,\s*'+bst+'\s*\~\w+\s*[\~,\)]',molecule_name) #A(a,b~0)								
							if m != None:
								index = molecule_name.find(m.group(0))+len(m.group(0))					
								tmp = bondCounter.keys()
								ind = 1e29
								for tmp1 in tmp:
									if bindingsite[0] == tmp1[0] or bindingsite[0] == tmp1[1]:
										ind = bondCounter[tmp1]
										break
								molecule_name_new = molecule_name[0:index-1]+'!'+str(ind)+molecule_name[index-1:]
								molecule_name = molecule_name_new	
			c = c+molecule_name_new+'.'
		c = c[:-1]
		complex_string[cmplex[0]] = c
	return complex_string


def PartialMolecule(Species,Molecules,Complexes,full_bindingSite):
	complex_id = [x[0] for x in Complexes.keys()]
	mol_id = [x[0] for x in Molecules.keys()]
	partialMolecule = {}
	partialComplex = {}
	for i in Species.keys(): #This is the list of model species
		st = Species[i]['speciesType'] #This 'speciesType' does not refer to binding sites!
		f = Species[i]['features']
		bs = Species[i]['bindingsites'] #These are free binding sites
		wc = Species[i]['wildcards']
		pf_id = [x[0] for x in f] #speciesFeatureType
		pf_val_id = [x[1] for x in f] #speciesFeatureValue
		if st in mol_id: #If the species type is a molecule
			mind = mol_id.index(st) #Locate the molecule index in the list of species
			key = Molecules.keys()[mind] #The molecule keys are (id,name) pairs
			pname = (i,key[1]) #Species ID, Molecule Name
			partialMolecule[pname] = {'SpeciesTypes' : [], 'FeatureTypes':{},'wildcards': []} #Define the partial molecule with free binding sites, features and wild cards
			#Features
			full_molecule = Molecules[key]
			full_features = full_molecule['FeatureTypes']
			for ff in full_features.keys(): #Complete feature list
				fid = ff[0] #Check if feaure id is present
				if fid in pf_id:
					fval = full_features[ff] #Complete feature values list
					for j in fval:
						if j[0] in pf_val_id:
							partialMolecule[pname]['FeatureTypes'][ff] = j
			#Free binding sites
			full_binding = full_molecule['SpeciesTypes']
			for binding in full_binding: #Go through free binding sites in the full molecule
				site = {}
				if binding.keys()[0][0] in bs: #if the binding site ID (binding.keys()[0][0]) is in the list of binding site IDs for the species
					site[binding.keys()[0]] = {} #define the binding site dictionary with the ID,Name tuple as key
					if len(binding[binding.keys()[0]])>0: #If the binding site in the full molecule has features
						f_bs = binding[binding.keys()[0]].keys()[0] #f_bs is the feature ID of the full molecule feature #CHECK can there be >1
						site[binding.keys()[0]][f_bs] = [] #Add this feature to the partial molecule
						if f_bs in pf_id: #if the feature ID is in the list of feature IDs for the partial molecule
							index = pf_id.index(f_bs) #find out where it is
							#f is a list of [feature ID, feature value ID,component] lists
							fvals_bs = [x for x in binding[binding.keys()[0]][f[index][0]] if x[0] ==f[index][1]] #Get the feature value in the full molecule corresponding to the right feature value ID
							site[binding.keys()[0]][f_bs].append(fvals_bs[0])
					partialMolecule[pname]['SpeciesTypes'].append(site)	

				elif binding.keys()[0][0] in wc:
					site[binding.keys()[0]] = {} #define the wild card dictionary with the ID,Name tuple as key
					if len(binding[binding.keys()[0]])>0: #If the binding site in the full molecule has features
						f_bs = binding[binding.keys()[0]].keys()[0] #f_bs is the feature ID of the full molecule feature #CHECK can there be >1
						site[binding.keys()[0]][f_bs] = [] #Add this feature to the partial molecule
						if f_bs in pf_id: #if the feature ID is in the list of feature IDs for the partial molecule
							index = pf_id.index(f_bs) #find out where it is
							#f is a list of [feature ID, feature value ID,component] lists
							fvals_bs = [x for x in binding[binding.keys()[0]][f[index][0]] if x[0] ==f[index][1]] #Get the feature value in the full molecule corresponding to the right feature value ID
							site[binding.keys()[0]][f_bs].append(fvals_bs[0])
					partialMolecule[pname]['wildcards'].append(site)	
			'''
			#Wild cards
			for wc_index in wc:
				wc_instance = [{x:full_bindingSite[x]} for x in full_bindingSite.keys() if x == wc_index]
				partialMolecule[pname]['wildcards'].append(wc_instance[0])'''


		if st in complex_id: #Find the full complex definition
			cid = complex_id.index(st)
			key = Complexes.keys()[cid]
			cname = (i,key[1]) 
			pmolecules = Complexes[key]['molecules'] #Participating molecules in the full complex
			pcomponents = Complexes[key]['components'] #Components are defined as a list of [component id, component name, parent iD] lists
			component_ids = [x[0] for x in pcomponents]
			#Go through participating molecules
			wildcards = Species[i]['wildcards']
			features = Species[i]['features']
			bst = Species[i]['bindingsites']
			ft = [x[0] for x in features]
			partialComplex[cname] = {'molecules':[],'bonds':[]} #Construct partial complex definition based on partial molecule definitions		
			bt = [x[0] for x in bst]			
			bonds = Complexes[key]['bonds']
			for mol in pmolecules:
				#Get full definition of participating molecule
				pid = mol_id.index(mol[0])
				full_molecule =  Molecules[Molecules.keys()[pid]]
				#Construct the partial molecule
				moleculeCopy = {}
				moleculeCopy[tuple(mol)] = {'SpeciesTypes':[],'FeatureTypes':{},'wildcards':[]}
				'''
				for wc_index in wc:
					if wc_index in component_names:
						ti = component_names.index(wc_index)
						if pcomponents[ti][2] == j[2]:
							wc_instance = [{x:full_bindingSite[x]} for x in full_bindingSite.keys() if x[0] == pcomponents[ti][1]]					
							moleculeCopy[tuple(j)]['wildcards'].append(wc_instance)'''
				#Edit the molecule based on features provided
				mft =  [x[0] for x in full_molecule['FeatureTypes'].keys()] #Get features present in full molecule 
				mst = [x.keys()[0][0] for x in full_molecule['SpeciesTypes']] # Get binding sites present in full molecule
				for f in features: #iterate through complex instance features: [feature id, feature value ID, component] lists
					#Go through the list of complex components and find the component that matches the feature component
					#Basically finding the component on the complex that contains the feature
					parent_id = [x[1] for x in pcomponents if x[0] ==f[2]]
					#If the feature ID exists in the full molecule feature list, and the parent ID matches the molecule component 
					if f[0] in mft and parent_id[0]==mol[2]:
						fid = mft.index(f[0]) #get index of feature ID
						#Get full feature definition (feature ID, feature name)
						sft = full_molecule['FeatureTypes'].keys()[fid]
						#Get possible feature values
						fvalues = full_molecule['FeatureTypes'][sft]
						#get feature value ID of this species
						fval = f[1]
						newfval = [x for x in fvalues if x[0] == fval] #newfval is the feature val ID, feature val name pair
						moleculeCopy[tuple(mol)]['FeatureTypes'][sft] = newfval[0] #This [0] is only to get rid of an additional nesting layer
				
				for bond in bonds: #iterate through bonds in the full complex
					b1 = bond[0] #component name, identifying parent
					b2 = bond[1] #component name, identifying parent
					#If the component matches something in the list of speciestypes in the participating molecule
					#and the identifying parent is the ID of the participating molecule
					if b1[0] in mst and b1[1]==mol[2]:
						#find this binding site in the list of species type indices for the participating molecule
						b_index = mst.index(b1[0])
						#get full site info
						full_site = full_molecule['SpeciesTypes'][b_index]
						site_name = full_site.keys()[0]
						#Check to see if the site has feature values
						fs_fvals = full_site[full_site.keys()[0]] #fullsite_featurevals
						if len(fs_fvals)>0:
							#Check for the parent molecule ID of the bond
							bond_parent = b1[1]
							#Check components corresponding to that parent
							component_parent = [x[0] for x in pcomponents if x[2] == bond_parent]
							#Check feature corresponding to this component
							feature_parent = [[x[0],x[1]] for x in features if x[2] in component_parent]
							#These are the feature values we need to keep
							fp_id = [x[0] for x in feature_parent]
							if fs_fvals.keys()[0] in fp_id:
								site = {site_name:{fp_id[0]:{}}}
								vals = fs_fvals.values()[0]
								site[site_name][fp_id[0]] = [x for x in vals if x[0] == feature_parent[0][1]]
						else:
							site = {site_name:{}}
					
					if b2[0] in mst and b2[1] ==mol[2]:
						b_index = mst.index(b2[0])
						full_site = full_molecule['SpeciesTypes'][b_index]
						site_name = full_site.keys()[0]
						#Check to see if the site has feature values
						fs_fvals = full_site[full_site.keys()[0]]
						if len(fs_fvals)>0:
							#Check for the parent molecule of the binding site
							bond_parent = b2[1]
							#Check components corresponding to that parent
							#x[2] is the identifying parent of the component
							#x[0] is the component ID
							component_parent = [x[0] for x in pcomponents if x[2] == bond_parent]
							#Check features in the model species instance corresponding to this component
							feature_parent = [[x[0],x[1]] for x in features if x[2] in component_parent]
							#These are the feature values we need to keep
							fp_id = [x[0] for x in feature_parent][0] #0 here is the remove extraneous nesting
							if fs_fvals.keys()[0] in fp_id:
								site = {site_name:{fp_id:{}}}
								vals = fs_fvals.values()[0] #0 here is the remove extraneous nesting
								site[site_name][fp_id] = [x for x in vals if x[0] == feature_parent[0][1]] #feature parent [0][1] is the feature value ID
						else:
							site = {site_name:{}}
					if b1[0] in mst or b2[0] in mst:
						#add the created site to the list of molecule species types
						if site not in moleculeCopy[tuple(mol)]['SpeciesTypes']:
							moleculeCopy[tuple(mol)]['SpeciesTypes'].append(site)
				#outward binding sites. Binding sites not participating in bonds
				for bt_instance in bst:
					if bt_instance in component_ids: #if the binding site is in the list of component ids for the full complex
						bt_index = component_ids.index(bt_instance) #find its index
						comp = pcomponents[bt_index][1] #get the component name
						if comp in mst:#if the component name is a binding site of the full molecule 
							mst_bt_index = mst.index(comp)
							#need to identify features of the site
							full_site = full_molecule['SpeciesTypes'][mst_bt_index] #get the full site definition
							site_name = full_site.keys()[0]
							#Check to see if the site has feature values
							fs_fvals = full_site[full_site.keys()[0]]
							if len(fs_fvals)>0:
								component_parent = bt_instance
								#Check features in the model species instance corresponding to this component
								feature_parent = [[x[0],x[1]] for x in features if x[2] in component_parent]
								fp_id = [x[0] for x in feature_parent][0] #0 here is the remove extraneous nesting
								if fs_fvals.keys()[0] in fp_id:
									site = {site_name:{fp_id:{}}}
									vals = fs_fvals.values()[0] #0 here is the remove extraneous nesting
									site[site_name][fp_id] = [x for x in vals if x[0] == feature_parent[0][1]] #feature parent [0][1] is the feature value ID
							else:
								site = {site_name:{}}
							if site not in moleculeCopy[tuple(mol)]['SpeciesTypes']: #append the site to the list of species types
								moleculeCopy[tuple(mol)]['SpeciesTypes'].append(site)
				partialComplex[cname]['molecules'].append(moleculeCopy)
			for bond in bonds:
				b1 = bond[0]
				b2 = bond[1]
				tmp = [] #list of binding sites (id,name) pairs involved in the bond
				for mol in pmolecules: #go through participating molecules
					pid = mol_id.index(mol[0]) #index of participating molecule
					full_molecule =  Molecules[Molecules.keys()[pid]]
					mst = [x.keys()[0][0] for x in full_molecule['SpeciesTypes']] #get list of species type IDs for participating molecule
					#if the binding site is in the participating molecule (b1[0] in mst) and the identifying parent matches the participating molecule ID
					if b1[0] in mst and b1[1]==mol[2]:
						b_index = mst.index(b1[0])
						#get the site, i.e. the (binding site ID,name) pair as a list
						site = list(copy.copy(full_molecule['SpeciesTypes'][b_index]).keys()[0])
						site.append(b1[1]) #append the identifying parent ID
					if b2[0] in mst and b2[1] == mol[2]:
						b_index = mst.index(b2[0])
						site = list(copy.copy(full_molecule['SpeciesTypes'][b_index]).keys()[0])
						site.append(b2[1])
					if (b1[0] in mst or b2[0] in mst) and site not in tmp:
						tmp.append(site)
				partialComplex[cname]['bonds'].append(tmp)
	return {'pm':partialMolecule,'pc':partialComplex}
	
def MolTypesString(m):
	s = Mol2String(m)
	skeys = s.keys()
	skeys.sort(key=lambda x:x[1]) #obtain sorted list of keys so we can print in order
	moltypes = 'begin molecule types \n'
	for j in skeys:
		moltypes = moltypes + s[j]+'\n'
	moltypes = moltypes + 'end molecule types \n'
	return moltypes

def SeedSpeciesString(s,m,c,b):
	result = PartialMolecule(s,m,c,b)
	pm = result['pm']
	pc = result['pc']
	string_pm = toString(pm)
	string_pc = Complex2String(pc)
	seedspecies = 'begin seed species\n'
	for i in s:
		if float(s[i]['ic']) != 0:
			if i in string_pm.keys():
				seedspecies = seedspecies + string_pm[i] +'\t'
			elif i in string_pc.keys():
				seedspecies = seedspecies + string_pc[i] +'\t'
			else:
				print 'error, can\'t find species'
				return
			seedspecies = seedspecies + str(s[i]['ic'])+'\n'
	return seedspecies+'end seed species \n'

def parameterBlockString(model):
	pblock = 'begin parameters\n'

	rxns = model.getListOfReactions()
	nrxns = len(rxns)	
	localparlist = []
	for i in range(0,nrxns):
		rxn = rxns[i]
		klaw = rxn.getKineticLaw()
		localpar = klaw.getListOfLocalParameters()
		for p in localpar:
			localparlist.append([p.getId(),p.getValue()])		
	plist = model.getListOfParameters()
	for i in plist:
		pblock = pblock + i.id +'\t'+str(i.value)+'\n'
	for i in localparlist:
		pblock = pblock + i[0]+'\t'+str(i[1])+'\n'
	pblock = pblock + 'end parameters\n'
	return {'pblock':pblock,'plist':plist}


def GetBNGFile(model,res):
	s = res['Species']
	m = res['Molecules']
	c = res['Complexes']
	b = res['fullBindingSites']
	RL = Reactions(model)
	Results = PartialMolecule(s,m,c,b)
	modelString = 'begin model\n'
	parameter_string = parameterBlockString(model)['pblock']
	molecules_string = MolTypesString(m)
	seedspecies_string = SeedSpeciesString(s,m,c,b)
 	reactions_string = rxnstring(RL,res)
 	rulesstring = getRules(model,res)
 	functions_string = rulesstring['functions']
 	observables_string = rulesstring['observables']
 	modelString = modelString+parameter_string+molecules_string+observables_string+functions_string+seedspecies_string+reactions_string
 	modelString = modelString +'end model\n'
 	print modelString
 	return modelString

if __name__ == '__main__':
	#TESTING
	#Parse sbml file
	#tree = ET.parse('ecad.xml')
	#tree = ET.parse('ex15.xml')
	#path ='../Downloads/simple_system_sbml_sbmlmulti.xml'
	#path = '/home/sag134/multi/bionetgen/bng2/toy-jim_sbml_sbmlmulti.xml'
	path =  sys.argv[1]
	mfile = GetBNGFile(path)
	print mfile
	
