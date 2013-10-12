'''
Bipartite Graph - bpgModel.py

Contains methods and classes to take reaction rules and atomize into atomic patterns and transformations.

Usage:
atomizedrules = bpgModel.getAtomizedRules(bngxml)
patterns,transformations = bpgModel.getElements(atomizedrules)


Author: John Sekar

Dependencies: readBNGXML.py (needs lxml), structures.py

Not supported: ChangeCompartment, DeleteSpecies
'''

import readBNGXML
import structures as st
import sys

class Transformation:
	def __init__(self,lhs,rhs,action):
		# lhs and rhs are lists of atomic patterns
		# action is a string indicating action
		available = ['Add','Delete','AddBond','DeleteBond','StateChange']
		assert (isinstance(lhs,list) and isinstance(rhs,list) and action in available), "Bad transformation!"
		
		if not len(lhs)==0:
			for item in lhs:
				assert isinstance(item,AtomicPattern), "Bad LHS!"
		else:
			lhs=None
			
		if not len(rhs)==0:
			for item in rhs:
				assert isinstance(item,AtomicPattern), "Bad RHS!"
		else:
			rhs=None
			
			
		self.lhs = lhs
		self.rhs = rhs
		self.action = action


	def __str__(self):
		return " ".join([self.strLHS(),"->",self.strRHS()])
	def __eq__(self,other):
		return str(self)==str(other)
	def __hash__(self):
		return hash(str(self))
	def __repr__(self):
		return str(self)
		
	def strLHS(self):
		if self.lhs==None:
			lhs_str = '0()'
		else:
			lhs_str = " + ".join(sorted([str(x) for x in self.lhs]))
		return lhs_str
	
	def strRHS(self):
		if self.rhs==None:
			rhs_str = '0()'
		else:
			rhs_str = " + ".join(sorted([str(x) for x in self.rhs]))
		return rhs_str
		
	def getLHS(self):
		if self.lhs==None:
			return []
		else:
			return self.lhs
			
	def getRHS(self):
		if self.rhs==None:
			return []
		else:
			return self.rhs
			
	def isSynDel(self):
		if self.action in ['Add','Delete']:
			return True
		else:
			return False
	
			

class AtomicPattern:
	def __init__(self,sp):
		self.sp = sp
	def __str__(self):
		return ".".join(sorted([str(x) for x in self.sp.molecules]))
	def __eq__(self, other):
		return str(self.sp)==str(other.sp)
	def __hash__(self):
		return hash(str(self.sp))
	def __repr__(self):
		return str(self.sp)
	def isMolecule(self):
		if len(self.sp.molecules) > 1:
			return False
		elif len(self.sp.molecules[0].components) > 0:
			return False
		else:
			return True

class ChoppedRule:
	def __init__(self,n_actions):
		# transformations are objects, rest are just ids
		self.transformations = [None]*n_actions
		self.syndel_context = [None]*n_actions
		self.transf_center = [None]*n_actions
		self.context = []
		
	def __str__(self):
		tempstr = []
		tempstr.append("\nTransformations: ")
		for x in self.transformations:
			tempstr.append(str(x))
		tempstr.append("Transformation Center: ")
		for xlist in self.transf_center:
			tempstr.append(" ".join([str(x) for x in xlist]))
		tempstr.append("Syndel Context: ")
		for xlist in self.syndel_context:
			tempstr.append(" ".join([str(x) for x in xlist]))
		tempstr.append("Context: ")
		for xlist in self.context:
			tempstr.append(" ".join([str(x) for x in xlist]))
			
		return "\n".join(tempstr)

def chopRule(reactants, products, actions, mappings, nameDict):
	'''chops the parsed rules into relevant sections:
	transformations, syndel_context, context
	stores the relevant component IDs
	'''
	maps = dict(mappings)
	patterns = reactants+products

	tempactions = [x for x in actions if x.action != 'ChangeCompartment']
	rule = ChoppedRule(len(tempactions))

	# Chopped rule has components
	# self.transformations = []*n_actions
	# self.syndel_context = []*n_actions
	#self.transf_center = []*n_actions
	# self.context = []
	
	for act_idx, act in enumerate(tempactions):
		if act.action=='StateChange':
			# Get LHS state
			p_idx, m_idx, c_idx = decompose_cidx(act.site1)
			p1,m1,c1 = getPMC(patterns,p_idx,m_idx,c_idx)
			s1 = c1.activeState
		
			# Get RHS state
			p_idx, m_idx, c_idx = decompose_cidx(maps[act.site1])
			p2,m2,c2 = getPMC(patterns,p_idx,m_idx,c_idx)
			s2 = c2.activeState
			
			#assert not s1==s2, "Bad state change operation. Check parsing!"
			
			# Making atomic patterns and transformations
			patt_lhs = AtomicPattern(makeStatePattern(m1,c1,s1))
			patt_rhs = AtomicPattern(makeStatePattern(m2,c2,s2))
			
			# Making transformation and updating sets
			tr = Transformation([patt_lhs],[patt_rhs],act.action)
			
			# Adding elements to chopped rule
			rule.transformations[act_idx] = tr
			rule.transf_center[act_idx] = getTransfCenterIDs(act,maps)

		
		if act.action=='DeleteBond':
			# Get participating componens
			p_idx1, m_idx1, c_idx1 = decompose_cidx(act.site1)
			p_idx2, m_idx2, c_idx2 = decompose_cidx(act.site2)
			
			p1,m1,c1 = getPMC(patterns,p_idx1,m_idx1,c_idx1)
			p2,m2,c2 = getPMC(patterns,p_idx2,m_idx2,c_idx2)
			
			#assert ('!' in str(c1) and '!' in str(c2)), "Bad bond operation. Check parsing!"
			
			# Make LHS and RHS patterns
			patt_lhs = AtomicPattern(makeBondPattern([m1,c1],[m2,c2]))
			patt_rhs1 = AtomicPattern(makeUnboundPattern(m1,c1))
			patt_rhs2 = AtomicPattern(makeUnboundPattern(m2,c2))
			
			# Making transformation and updating sets
			tr = Transformation([patt_lhs],[patt_rhs1,patt_rhs2],act.action)
			
			# Adding elements to chopped rule
			rule.transformations[act_idx] = tr
			rule.transf_center[act_idx] = getTransfCenterIDs(act,maps)

		
		if act.action=='AddBond':
			# Get participating components
			p_idx1, m_idx1, c_idx1 = decompose_cidx(act.site1)
			p_idx2, m_idx2, c_idx2 = decompose_cidx(act.site2)
			
			
			p1,m1,c1 = getPMC(patterns,p_idx1,m_idx1,c_idx1)
			p2,m2,c2 = getPMC(patterns,p_idx2,m_idx2,c_idx2)
			
			#assert ('!' in str(c1) and '!' in str(c2)), "Bad bond operation. Check parsing!"
			
			# Make LHS and RHS patterns
			patt_lhs1 = AtomicPattern(makeUnboundPattern(m1,c1))
			patt_lhs2 = AtomicPattern(makeUnboundPattern(m2,c2))
			patt_rhs = AtomicPattern(makeBondPattern([m1,c1],[m2,c2]))
			
			# Making transformation and updating sets
			tr = Transformation([patt_lhs1,patt_lhs2],[patt_rhs],act.action)
			
			# Adding elements to chopped rule
			rule.transformations[act_idx] = tr
			rule.transf_center[act_idx] = getTransfCenterIDs(act,maps)

		if act.action=='Add':
			# Get participating molecule
			p_idx, m_idx = decompose_midx(act.site1)
			p1,m1,_ = getPMC(patterns,p_idx,m_idx,None)
			# Making LHS and RHS patterns
			patt_rhs = AtomicPattern(makeMoleculePattern(m1))
			# Making transformation and updating sets
			tr = Transformation([],[patt_rhs],act.action)
			
			# Adding elements to chopped rule
			rule.transformations[act_idx] = tr
			rule.transf_center[act_idx] = getTransfCenterIDs(act,maps)
			rule.syndel_context[act_idx] = getSynDelContextIDs(rule.transf_center[act_idx][0],nameDict.keys())

			
		if act.action=='Delete':
			# Get participating molecule
			p_idx, m_idx = decompose_midx(act.site1)
			p1,m1,_ = getPMC(patterns,p_idx,m_idx,None)
			# Making LHS and RHS patterns
			patt_lhs = AtomicPattern(makeMoleculePattern(m1))
			# Making transformation and updating sets
			tr = Transformation([patt_lhs],[],act.action)
			
			# Adding elements to chopped rule
			rule.transformations[act_idx] = tr
			rule.transf_center[act_idx] = getTransfCenterIDs(act,maps)
			rule.syndel_context[act_idx] = getSynDelContextIDs(rule.transf_center[act_idx][0],nameDict.keys())
	return rule

def printRule(reactants,products):
	return "+".join([str(x) for x in reactants])+"->"+"+".join([str(x) for x in products])

		
class AtomizedRule:
	'''uses chopped rules (i.e. component IDs)
	to construct atomic patterns from each rule
	returns an atomized rule
	'''
	def __str__(self):
		return self.rulestring
	
	def __init__(self,choppedrule,reactants,products):
		# this is to generate the rule as required
		self.rulestring = printRule(reactants,products)
		
		self.transformations = choppedrule.transformations
		
		# this lhs is for the purpose of unambiguously assigning context for simultaneous transformationst
		# it is okay to leave this empty for syndel transforms
		self.transf_center_lhs = [None]*len(choppedrule.transformations)
		
		# this is the context that is identical on both sides of a reaction
		# i.e. not affected by the transformations
		self.context = []
		
		self.syndel_context = [None]*len(choppedrule.transformations)
		
		for idx,tr in enumerate(self.transformations):
			if tr.action in ['AddBond','DeleteBond','StateChange']:
				self.transf_center_lhs[idx] = tr.lhs

		
		# Get a complete list of not-context components and add state, bond and bond-wildcards from components NOT on this list to context
		complete_transf_center = [ item for sublist in choppedrule.transf_center for item in sublist ]
		complete_syndel_context = [ item for sublist in choppedrule.syndel_context if sublist is not None for item in sublist ]
		notcontext =  complete_transf_center+complete_syndel_context

		for patt in reactants:
			bonds = []
			for mol in patt.molecules:
				for comp in mol.components:
					if comp.idx not in notcontext:
						if comp.activeState:
							self.context.append(AtomicPattern(makeStatePattern(mol,comp,comp.activeState)))
						if comp.bonds:
							if '+' in comp.bonds:
								self.context.append(AtomicPattern(makeBondWildcardPattern(mol,comp)))
							else:
								bonds.append([mol,comp,comp.bonds[0]])
			bondnames = set([b for m,c,b in bonds])
			for bond in bondnames:
				mc = [ [m,c] for m,c,b in bonds if b==bond]
				if len(mc)==2:
					# when Delete happens, certain components exist for which the bond is removed, but the component itself is considered outside
					# the reaction center of the Delete operation and is hence not identified by the complete_transf_center and complete_syndel_context identified above
					# this would then pick up the undeleted half of the bond as context
					self.context.append(AtomicPattern(makeBondPattern(mc[0],mc[1])))
				
		
		for idx,tr in enumerate(choppedrule.transformations):
			#get components for "this" transformation
			c_idxlist_tr = [c for c in choppedrule.transf_center[idx] if '_RP' in c]
			#get components for "other" transformations
			c_idxlist_other = [c for idx2 in range(len(choppedrule.transf_center)) for c in choppedrule.transf_center[idx2] if ('_RP' in c and idx2 != idx)]
			#print tr.action,c_idxlist_tr,c_idxlist_other
			
			# for the component in "this" state transformation, pick out its bond patterns unless they were modified in the "other" transformations
			if tr.action == 'StateChange':
				# there should be only on component
				c_idx = c_idxlist_tr[0]
				p_idx,m_idx,_ = decompose_cidx(c_idx)
				patt,mol,comp = getPMC(reactants,p_idx,m_idx,c_idx)
				if comp.bonds:
					if '+' in comp.bonds:
						self.context.append(AtomicPattern(makeBondWildcardPattern(mol,comp)))
					else:
						# this section untested in fceri_ji.xml
						# get bondname
						bond = comp.bonds[0]
						# find partner (should be in same pattern, should not be same component, should have same bondname, should not be in "other" transfomration
						comp2list = [c for m in patt.molecules for c in m.components if (c.bonds and c.idx != c_idx and c.idx not in c_idxlist_other )]
						for comp2 in comp2list:
							if comp2.bonds[0] == bond:
								p_idx2,m_idx2,c_idx2 = decompose_cidx(comp2.idx)
								_,mol2,_ = getPMC(reactants,p_idx2,m_idx2,c_idx2)
								# make the pattern
								self.context.append(AtomicPattern(makeBondPattern([mol,comp],[mol2,comp2])))
			# for each component in "this" bond transformation, pick out its state patterns unless they were modified by "other" transformations
			if tr.action in ['AddBond','DeleteBond']:
				# there must be 2 components, so using forloop
				for c_idx in c_idxlist_tr:
					# check if active state exists and if it doesnt exist in other transformations
					if c_idx not in c_idxlist_other:
						p_idx,m_idx,_ = decompose_cidx(c_idx)
						patt,mol,comp = getPMC(reactants,p_idx,m_idx,c_idx)
						if comp.activeState:
							self.context.append(AtomicPattern(makeStatePattern(mol,comp,comp.activeState)))
							
			# for each molecule in "this" syndel transformation, pick out its state patterns, bond wildcards and bond patterns
			# add these to syndelcontext
			# bond patterns will require you to look outside the created/deleted molecules
			if tr.action in ['Add','Delete']:
				patterns = reactants+products
				mol = [m for patt in patterns for m in patt.molecules if m.idx==choppedrule.transf_center[idx][0]][0]
				p_idx,m_idx = decompose_midx(mol.idx)
				patt,_,_ = getPMC(patterns,p_idx,m_idx,None)
				
				self.syndel_context[idx] = []
				# get each component:
				c_list = [c for c in mol.components]
				for comp in c_list:
					# query activestate
					if comp.activeState:
						self.syndel_context[idx].append(AtomicPattern(makeStatePattern(mol,comp,comp.activeState)))
					if comp.bonds:
						# no need to check for wildcards, since they cannot be created or deleted
						# get the bond name
						bond = comp.bonds[0]
						# find partner (should be in same pattern, should not be same component, should have same bondname, 
						comp2list = [c for m in patt.molecules for c in m.components if (c.bonds and c.idx != comp.idx)]
						#comp2 = [c for m in patt.molecules for c in m.components if (c.idx != comp.idx and c.bonds[0] == bond )][0]
						for comp2 in comp2list:
							if comp2.bonds[0] == bond:
								p_idx2,m_idx2,c_idx2 = decompose_cidx(comp2.idx)
								_,mol2,_ = getPMC(patterns,p_idx2,m_idx2,c_idx2)
								# make the pattern
								self.syndel_context[idx].append(AtomicPattern(makeBondPattern([mol,comp],[mol2,comp2])))	
			
		# remaking lists into sets
		#self.transformations = choppedrule.transformations
		self.context = set(self.context)
		for idx,item in enumerate(self.transf_center_lhs):
			if item is None:
				self.transf_center_lhs[idx] = set()
			else:
				self.transf_center_lhs[idx] = set(item)
		for idx,item in enumerate(self.syndel_context):
			if item is None:
				self.syndel_context[idx] = set()
			else:
				self.syndel_context[idx] = set(item)

def decompose_cidx(idx):
	''' returns ids for pattern, molecule and component respectively from component id
	'''
	c_idx = idx
	m_idx = c_idx.rsplit('_',1)[0]
	p_idx =	m_idx.rsplit('_',1)[0]
	return p_idx, m_idx, c_idx
	
def decompose_midx(idx):
	''' returns ids for pattern and molecule respectively from mol id
	'''
	m_idx = idx
	p_idx =	m_idx.rsplit('_',1)[0]
	return p_idx, m_idx
	
def getPMC(patterns,p_idx,m_idx,c_idx):
	''' Returns Pattern Molecule Component objects from ids.
	If c_idx does not exist, it returns only pattern and molecule objects
	'''

	patt = [x for x in patterns if x.idx == p_idx][0]
	mol = [x for x in patt.molecules if x.idx == m_idx][0]
	if c_idx:
		comp = [x for x in mol.components if x.idx == c_idx][0]
	else:
		comp = None
	#state = comp.activeState
	return patt, mol, comp

def makeStatePattern(mol,comp,state):
	'''Make state pattern from Molecule Component State
	'''
	c = st.Component(comp.name,comp.idx,[],[])
	c.addState(state)
	c.setActiveState(state)

	m = st.Molecule(mol.name,mol.idx)
	m.addComponent(c)	
	sp = st.Species()
	sp.addMolecule(m)
	return sp

def makeBondPattern(mc1,mc2):
	'''Make bond pattern from [Molecule Component], [Molecule Component]
	'''
	[m1,c1] = mc1
	[m2,c2] = mc2
	
	c_1 = st.Component(c1.name,c1.idx,[],[])
	c_1.addBond('1')
	m_1 = st.Molecule(m1.name, m1.idx)
	m_1.addComponent(c_1)
	
	c_2 = st.Component(c2.name,c2.idx,[],[])
	c_2.addBond('1')
	m_2 = st.Molecule(m2.name, m2.idx)
	m_2.addComponent(c_2)
	
	sp = st.Species()
	sp.addMolecule(m_1)
	sp.addMolecule(m_2)
	return sp
	
def makeBondWildcardPattern(mol,comp):
	'''Make unbound pattern from Molecule Component
	'''
	c = st.Component(comp.name,comp.idx,[],[])
	c.addBond('+')
	m = st.Molecule(mol.name,mol.idx)
	m.addComponent(c)
	sp = st.Species()
	sp.addMolecule(m)
	return sp	

def makeUnboundPattern(mol,comp):
	'''Make unbound pattern from Molecule Component
	'''
	c = st.Component(comp.name,comp.idx,[],[])
	m = st.Molecule(mol.name,mol.idx)
	m.addComponent(c)
	sp = st.Species()
	sp.addMolecule(m)
	return sp
	
def makeMoleculePattern(mol):
	'''Make molecule pattern from Molecule
	'''
	m = st.Molecule(mol.name, mol.idx)
	sp = st.Species()
	sp.addMolecule(m)
	return sp

def getTransfCenterIDs(act,maps):
	'''Finds components in both reactant and product that are part of the transformation center
	'''
	inv_maps = dict([(y,x) for x,y in maps.items()])
	temp = []
	
	if act.action in ['AddBond','DeleteBond','StateChange']:
		temp = temp + [act.site1]
		if act.site1 in maps.keys():
			temp = temp + [maps[act.site1]]
		elif act.site1 in inv_maps.keys():
			temp = temp + [inv_maps[act.site1]]
		if act.site2:
			temp = temp + [act.site2]
			if act.site1 in maps.keys():
				temp = temp + [maps[act.site1]]
			elif act.site1 in inv_maps.keys():
				temp = temp + [inv_maps[act.site1]]
				
	if act.action in ['Add','Delete']:
		temp = temp + [act.site1]
	return temp

def getSynDelContextIDs(site1,all_sites):
	''' Gets the syndel context from the maps '''
	return  [x for x in all_sites if site1 in x and len(x)>len(site1)]
	

def getAtomizedRules(bngxml):
	'''
	input an xml file, return atomized rules
	'''
	_,rules = readBNGXML.parseXML(bngxml)

	print "\nChopping and atomizing rules..."
	atomizedrules = []
	for idx, [reactants, products, actions, mappings, nameDict] in enumerate(rules):
		choppedrule = chopRule(reactants, products, actions, mappings, nameDict)
		atomizedrules.append(AtomizedRule(choppedrule,reactants,products))
	print len(atomizedrules), "rules atomized."
	return atomizedrules
	
def getElements(atomizedrules):
	'''
	input atomized rules, return patterns (set) and transformations (set)
	'''
	patterns = set()
	transformations = set()
	print "\nExtracting basic patterns and transformations..."
	for rule in atomizedrules:
		transformations.update(rule.transformations)
		for tr in rule.transformations:
			patterns.update(tr.getLHS())
			patterns.update(tr.getRHS())
		#for item in rule.transf_center_lhs:
		#	patterns.update(item)
		patterns.update(rule.context)
		for item in rule.syndel_context:
			patterns.update(item)
			
	syndels = [x for x in transformations if x.isSynDel()]
	molecpats = [x for x in patterns if x.isMolecule()]

	print len(transformations),"transformations found ("+str(len(syndels)),"are syndels)."
	print len(patterns),"basic patterns constructed, ("+str(len(molecpats)),"in syndels)."
	
	return patterns,transformations
	

if __name__ == "__main__":
	atomizedrules = getAtomizedRules(sys.argv[-1])
	patterns,transformations = getElements(atomizedrules)
	for item in atomizedrules:
		print item
	
