import sys
import xml.etree.ElementTree as ET
#from converter import toString
from SpeciesTypes import *
import re
import copy

def Reactions(model):
	plist = parameterBlockString(model)['plist']
	par = [x.id for x in plist]
	rxns = model.getListOfReactions()
	reactants = {}
	products = {}
	parameters = {}
	nrxns = len(rxns)	
	ReactionList = []
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
		''' FIX
		flag = 0
		for j in rxn:
			elif j.tag == prefix+'kineticLaw':
				for k in j:
					if k.tag == prefix+'listOfLocalParameters':
						flag = 1
						for q in k:
							tmp['full_parameters'].append({dict(q.attrib)['id']:dict(q.attrib)['value']})
		'''
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
		if i['reversible'] != 'false':
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
		#print Molecules[i]
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
				m = m+' '
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
	#print mol_string
	return mol_string

def Complex2String(complexes):
	complex_string = {}
	for i in complexes:
		#print complexes[i]
		c = ''
		#print complexes[i]
		for j in complexes[i]['molecules']:
			mname = toString(j).values()[0] #Participating molecule e.g.X(y,z)
			#print mname
			mol = j.keys()[0][1]
			mol  = mol.replace('(','\(')
			mol = mol.replace(')','\)') #Participating molecule name, e.g. X
			counter = 0
			bondCounter = {}
			for b in complexes[i]['bonds']:
			#	print b
				counter = counter + 1
				st = [list(x.keys()[0]) for x in j[j.keys()[0]]['SpeciesTypes']]
				bondCounter[(b[0][0],b[1][0])] = counter
			#	print bondCounter				
				for bond in b:
				#	print [bond[0],bond[1]],'bond'
					#print st,'st'
					#print j.keys()[0][2],bond[2]
					if [bond[0],bond[1]] in st and bond[2] == j.keys()[0][2]:
						bst = bond[1]
						#print 'here'
						m = re.search(mol+'\(\s*'+bst+'\s*[\,\)]',mname) #e.g a in A(a, ...)

						#print bst
						#print mname
						#print mol
						if m==None: # e.g. b in A(a,b,...) or b in A(a,b)
							m = re.search(mol+'\(.*\,\s*'+bst+'\s*[\,\)]',mname)
						if m!=None:
							index = mname.find(m.group(0))+len(m.group(0))	
							#print m.group(0)
							#print index					
							tmp = bondCounter.keys()
							#print 'tmp',tmp
							ind = 1e29
							for tmp1 in tmp:
								if bond[0] == tmp1[0] or bond[0] == tmp1[1]:
									ind = bondCounter[tmp1]
									break
							mname_new = mname[0:index-1]+'!'+str(ind)+mname[index-1:]
							mname = mname_new
						else: #e.g. b in A(a,b~0~p)
							m = re.search(mol+'\(\s*'+bst+'\s*\~\w+\s*[\,\)]',mname) #A(a~0)
							if m == None:
								m = re.search(mol+'\(.*\,\s*'+bst+'\s*\~\w+\s*[\~,\)]',mname) #A(a,b~0)								
							if m != None:
								#print m.group(0)
								index = mname.find(m.group(0))+len(m.group(0))	
								#print m.group(0)
								#print index					
								tmp = bondCounter.keys()
								#print 'tmp',tmp
								ind = 1e29
								for tmp1 in tmp:
									if bond[0] == tmp1[0] or bond[0] == tmp1[1]:
										ind = bondCounter[tmp1]
										break
								mname_new = mname[0:index-1]+'!'+str(ind)+mname[index-1:]
								mname = mname_new							
						#print mname

			c = c+mname_new+'.'
		c = c[:-1]
		complex_string[i[0]] = c
	return complex_string


def PartialMolecule(Species,Molecules,Complexes,full_bindingSite):
	complex_id = [x[0] for x in Complexes.keys()]
	mol_id = [x[0] for x in Molecules.keys()]
	partialMolecule = {}
	partialComplex = {}
	for i in Species.keys(): #This is the list of model species
		st = Species[i]['speciesType']
		f = Species[i]['features']
		bs = Species[i]['bindingsites']
		wc = Species[i]['wildcards']
		pf_id = [x[0] for x in f] #speciesFeatureType
		pf_val_id = [x[1] for x in f] #speciesFeatureValue
		if st in mol_id: #If the species type is a molecule
			mind = mol_id.index(st) #Locate the molecule index in the list of species
			key = Molecules.keys()[mind] #The molecule keys are (id,name) pairs
			pname = (i,key[1]) #Species ID, Molecule Name
			print pname
			partialMolecule[pname] = {'SpeciesTypes' : [], 'FeatureTypes':{},'wildcards': []} #Define the partial molecule with free binding sites, features and wild cards
			for wc_index in wc:
				wc_instance = [{x:full_bindingSite[x]} for x in full_bindingSite.keys() if x[0] == wc_index]
				partialMolecule[pname]['wildcards'].append(wc_instance[0])

			full_molecule = Molecules[key]
			full_features = full_molecule['FeatureTypes']
			for ff in full_features.keys(): #Complete feature list
				fid = ff[0] #Check if feaure id is present
				if fid in pf_id:
					fval = full_features[ff] #Complete feature values list
					for j in fval:
						if j[0] in pf_val_id:
							partialMolecule[pname]['FeatureTypes'][ff] = j
			full_binding = full_molecule['SpeciesTypes']
			for binding in full_binding:
				site = {}
				if binding.keys()[0][0] in bs:
					site[binding.keys()[0]] = {}
					if len(binding[binding.keys()[0]])>0:
						f_bs = binding[binding.keys()[0]].keys()[0]
						site[binding.keys()[0]][f_bs] = []
						if f_bs in pf_id:
							index = pf_id.index(f_bs)
							fvals_bs = [x for x in binding[binding.keys()[0]][f[index][0]] if x[0] ==f[index][1]]
							site[binding.keys()[0]][f_bs].append(fvals_bs[0])
					partialMolecule[pname]['SpeciesTypes'].append(site)	
		if st in complex_id:
			#print "sp",Species[i]
			cid = complex_id.index(st)
			key = Complexes.keys()[cid]
			cname = (i,key[1])
			pmolecules = Complexes[key]['molecules']

			pcomponents = Complexes[key]['components']
			#print pcomponents,'PPPPPP'
			component_names = [x[0] for x in pcomponents]
			#Go through participating molecules
			wildcards = Species[i]['wildcards']
			features = Species[i]['features']
			bindingsites = Species[i]['bindingsites']
			ft = [x[0] for x in features]
			partialComplex[cname] = {'molecules':[],'bonds':[]}
			bst = Species[i]['bindingsites']
			bt = [x[0] for x in bst]			
			bonds = Complexes[key]['bonds']
			#print pmolecules
			for j in pmolecules:
				pid = mol_id.index(j[0])
				#print j[0],'PPIIIIII'
				molecule =  Molecules[Molecules.keys()[pid]]
				#print "MOOL",molecule
				moleculeCopy = {}
				moleculeCopy[tuple(j)] = {'SpeciesTypes':[],'FeatureTypes':{},'wildcards':[]}

				for wc_index in wc:
					if wc_index in component_names:
						ti = component_names.index(wc_index)
						if pcomponents[ti][2] == j[2]:
							wc_instance = [{x:full_bindingSite[x]} for x in full_bindingSite.keys() if x[0] == pcomponents[ti][1]]					
							moleculeCopy[tuple(j)]['wildcards'].append(wc_instance[0])
				#Edit the molecule based on features provided
				mft =  [x[0] for x in molecule['FeatureTypes'].keys()]
				mst = [x.keys()[0][0] for x in molecule['SpeciesTypes']]
				for f in features:
					parent_id = [x[1] for x in pcomponents if x[0] ==f[2]]
					if f[0] in mft and parent_id[0]==j[2]:
						fid = mft.index(f[0])
						sft = molecule['FeatureTypes'].keys()[fid]
						fvalues = molecule['FeatureTypes'][sft]
						fval = features[ft.index(f[0])][1]
						newfval = [x for x in fvalues if x[0] == fval]
						newfval = [x for x in fvalues if x[0] == f[1]]
						moleculeCopy[tuple(j)]['FeatureTypes'][sft] = newfval[0]

				for bond in bonds:
					b1 = bond[0]
					b2 = bond[1]
				#	print b1,b2
					if b1[0] in mst and b1[1]==j[2]:
						b_index = mst.index(b1[0])
						full_site = molecule['SpeciesTypes'][b_index]
						site_name = full_site.keys()[0]
				#		print site_name
						#Check to see if the site has feature values
						fs_fvals = full_site[full_site.keys()[0]]
				#		print fs_fvals
						if len(fs_fvals)>0:
							#Check for the parent molecule of the bond

				#			print 'HERE1'
							bond_parent = b1[1]
							#Check components corresponding to that parent
							component_parent = [x[0] for x in pcomponents if x[2] == bond_parent]
				#			print component_parent
							#Check feature corresponding to this component
				#			print features
							feature_parent = [[x[0],x[1]] for x in features if x[2] in component_parent]
							#These are the feature values we need to keep
				#			print feature_parent,'fp'
							fp_id = [x[0] for x in feature_parent]
							#print fp_id
							if fs_fvals.keys()[0] in fp_id:
				#				print 'here',molecule['SpeciesTypes'][b_index]
								site = {site_name:{fp_id[0]:{}}}
								vals = fs_fvals.values()[0]
								site[site_name][fp_id[0]] = [x for x in vals if x[0] == feature_parent[0][1]]
						else:
							site = {site_name:{}}

					if b2[0] in mst and b2[1] ==j[2]:
				#		print 'HERE'
						b_index = mst.index(b2[0])
						full_site = molecule['SpeciesTypes'][b_index]
						site_name = full_site.keys()[0]
						#Check to see if the site has feature values
						fs_fvals = full_site[full_site.keys()[0]]
				#		print fs_fvals
						if len(fs_fvals)>0:
							#Check for the parent molecule of the bond
							bond_parent = b2[1]
							#Check components corresponding to that parent
							component_parent = [x[0] for x in pcomponents if x[2] == bond_parent]
				#			print component_parent
							#Check feature corresponding to this component
				#			print features
							feature_parent = [[x[0],x[1]] for x in features if x[2] in component_parent]
							#These are the feature values we need to keep
							#print feature_parent,'fp'
							fp_id = [x[0] for x in feature_parent]
							#print fp_id
							if fs_fvals.keys()[0] in fp_id:
				#				print molecule['SpeciesTypes'][b_index]
								site = {site_name:{fp_id[0]:{}}}
								vals = fs_fvals.values()[0]
								site[site_name][fp_id[0]] = [x for x in vals if x[0] == feature_parent[0][1]]
						else:
							site = {site_name:{}}
					if b1[0] in mst or b2[0] in mst:

				#		print 'site',site
						if site not in moleculeCopy[tuple(j)]['SpeciesTypes']:
							moleculeCopy[tuple(j)]['SpeciesTypes'].append(site)
				#print moleculeCopy[tuple(j)]
				for bt_instance in bst:
					if bt_instance in component_names:
						bt_index = component_names.index(bt_instance)
						comp = pcomponents[bt_index][1]
						if comp in mst:
							mst_bt_index = mst.index(comp)
							site = molecule['SpeciesTypes'][mst_bt_index]
							if site not in moleculeCopy[tuple(j)]['SpeciesTypes']:
								moleculeCopy[tuple(j)]['SpeciesTypes'].append(site)
				#				print site
				partialComplex[cname]['molecules'].append(moleculeCopy)

			for bond in bonds:
				b1 = bond[0]
				b2 = bond[1]
				tmp = []
				for j in pmolecules:
					pid = mol_id.index(j[0])
					molecule =  Molecules[Molecules.keys()[pid]]
					mst = [x.keys()[0][0] for x in molecule['SpeciesTypes']]
					if b1[0] in mst and b1[1]==j[2]:
						b_index = mst.index(b1[0])
						site = list(copy.copy(molecule['SpeciesTypes'][b_index]).keys()[0])
						site.append(b1[1])
					if b2[0] in mst and b2[1] == j[2]:
						b_index = mst.index(b2[0])
						site = list(copy.copy(molecule['SpeciesTypes'][b_index]).keys()[0])
						site.append(b2[1])
					if (b1[0] in mst or b2[0] in mst) and site not in tmp:
						tmp.append(site)

				partialComplex[cname]['bonds'].append(tmp)
	return {'pm':partialMolecule,'pc':partialComplex}
	
def MolTypesString(m):
	s = toString(m)
	moltypes = 'begin molecule types \n'
	for j in s:
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
	plist = model.getListOfParameters()
	for i in plist:
		pblock = pblock + plist[0].id +'\t'+str(plist[0].value)+'\n'
	pblock = pblock + 'end parameters\n'
	return {'pblock':pblock,'plist':plist}


def GetBNGFile(path):
	print path
	tree = ET.parse(path)
	RL = Reactions(tree)
	s = SpeciesTypes(tree)['Species']
	m = SpeciesTypes(tree)['Molecules']
	c = SpeciesTypes(tree)['Complexes']
	b = SpeciesTypes(tree)['fullBindingSites']
	#print m
	#print c
	#print 'str',toString(m)
	Results = PartialMolecule(s,m,c,b)
	#print toString(Results['pm'])
	#print Complex2String(Results['pc'])
#	print MolTypesString(m)

	#print '\n',s
	modelString = 'begin model\n'
	parameter_string = parameterBlockString(tree)['pblock']
	molecules_string = MolTypesString(m)
	seedspecies_string = SeedSpeciesString(s,m,c,b)
 	reactions_string = rxnstring(RL,tree)

 	modelString = modelString+parameter_string+molecules_string+seedspecies_string+reactions_string
 	modelString = modelString +'end model\n'
 	#print modelString
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
	
