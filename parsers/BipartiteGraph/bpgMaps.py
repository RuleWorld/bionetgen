'''
Creates transformation pairs, creates various dictionaries and infers various maps between rules, patterns, transformations, transformation maps

Usage: 
transformationpairs, irreversibles = bpgMaps.getTransformationPairs(transformations,tprules)
names = bpgMaps.getNameDictionary(atomizedrules,patterns,transformations,transformationpairs)
rule_map, tr_map, trpair_map = bpgMaps.getMaps(names)

Dependencies: bpgModel.py

Author: John Sekar
'''

import bpgModel
import sys
import itertools


# Classes and methods for collecting transformation pairs
class TransformationPair:
	'''
	Pair of transformations whose reactant and product are mirrors of each other
	'''
	def __init__(self,trlist):
		self.forward = trlist[0]
		self.reverse = trlist[1]

	
	def __str__(self):
		return " ".join([self.strLHS(),"<->",self.strRHS()])
		
	def strLHS(self):
		return self.forward.strLHS()
	def strRHS(self):
		return self.forward.strRHS()

	def flip(self):
		x =  TransformationPair((self.reverse,self.forward))
		return x
		
	def isPresent(self,tr):
		return (tr==self.forward or tr==self.reverse)
		
	def getLHS(self):
		return self.forward.getLHS()
			
	def getRHS(self):
		return self.forward.getRHS()
	def isSynDel(self):
		return isSynDel(self.forward)

def processTransformationPairRules(tprules):
	'''
	This method processes the transformation pair sorting rules to make language compatible with string matching
	(It replaces * with '')
	'''
	tprules2 = []
	for tprule in tprules:
		tempitem = []
		for item in tprule:
			if item == '*':
				tempitem.append('')
			else:
				tempitem.append(item)
		tprules2.append(tempitem)
	return tprules2
	
def getTransformationPairs(transformations,tprules):
	'''
	Constructs transformation pairs from transformations, sorted using provided rules
	'''
	print "\nIdentifying transformation pairs..."
	tprules2 = processTransformationPairRules(tprules)
	tp = [x for x in itertools.combinations(transformations,2) if (x[0].lhs==x[1].rhs and x[0].rhs==x[1].lhs)]
	tp2 = []
	for item in tp:
		x = TransformationPair(item)
		#sorting pair by mere alphabetical order
		if x.strLHS() > x.strRHS():
			x = x.flip()
		#sorting pair by rules provided by user
		for tprule in tprules2:
			if(tprule[0] in x.strRHS() and tprule[1] in x.strLHS()):
				x = x.flip()
		tp2.append(x)
	irrev = []
	paired_trlist = [y.forward for y in tp2]+[y.reverse for y in tp2]
	for tr in transformations:
		if tr not in paired_trlist:
			irrev.append(tr)
	tp2 = set(tp2)
	irrev = set(irrev)
	
	print len(tp2),"transformation pairs found."
	print len(irrev),"irreversible transformations found."
	return tp2,irrev
	
# Classes and methods for handling names and ids of rules, patterns, transformations and transformation pairs
class NameDictionary:
	'''
	Class that holds the unique names mapped to rules, patterns, transformations and transformation pairs
	Sorts everything except rules, which are in the order read from the xml file
	'''
	def __init__(self,atomizedrules,patterns,transformations,transformationpairs,irreversibles):
		self.r = dict()
		for idx,rule in enumerate(atomizedrules):
			self.r[rule] = idx
		
		self.p = dict()
		pats = sorted(patterns,key=lambda x:str(x))
		for idx,patt in enumerate(pats):
			self.p[patt] = idx

		self.t = dict()
		trs = sorted(transformations,key=lambda x:str(x))
		for idx,tr in enumerate(trs):
			self.t[tr] = idx
		
		self.tp = dict()
		trpairs = sorted(transformationpairs,key= lambda x:str(x))
		for idx,trp in enumerate(trpairs):
			self.tp[trp] = idx
			
		self.irr = dict()
		irrs = sorted(irreversibles,key= lambda x:str(x))
		for idx,ir in enumerate(irrs):
			self.irr[ir] = self.tp[ir]		

	
	def getIdx(self,elemtype,string):
		if elemtype == 'p':
			return [idx for x,idx in self.p.items() if str(x)==string][0]
		if elemtype == 't':
			return [idx for x,idx in self.t.items() if str(x)==string][0]
		if elemtype == 'tp':
			return [idx for x,idx in self.tp.items() if str(x)==string][0]
		if elemtype == 'r':
			return [idx for x,idx in self.r.items() if str(x)==string][0]
		if elemtype == 'irr':
			return [idx for x,idx in self.irr.items() if str(x)==string][0]
			
	def getElement(self,elemtype,idx1):
		if elemtype == 'p':
			return [x for x,idx in self.p.items() if idx==idx1][0]
		if elemtype == 't':
			return [x for x,idx in self.t.items() if idx==idx1][0]
		if elemtype == 'tp':
			return [x for x,idx in self.tp.items() if idx==idx1][0]
		if elemtype == 'r':
			return [x for x,idx in self.r.items() if idx==idx1][0]
		if elemtype == 'irr':
			return [x for x,idx in self.irr.items() if idx==idx1][0]
			
			
def getNameDictionary(atomizedrules,patterns,transformations,transformationpairs,irreversibles):
	'''
	Takes atomized rules, patterns, transformations and transformation pairs
	returns dictionaries of rules, patterns, transformations, transformation pairs
	'''
	print "\nBuilding dictionaries of unique elements..."
	#dictRules,dictPatterns,dictTransformations,dictTransformationPairs = getNameDictionaries(atomizedrules,patterns,transformations,transformationpairs)
	names = NameDictionary(atomizedrules,patterns,transformations,transformationpairs,irreversibles)
	return names
	
def resolveBondWildcards(pattstringlist,patt_list_from_model):
	'''
	Takes a list of patterns (strings) and returns the list with wildcards resolved (and includes the wildcard also)
	'''
	pattlist = [str(x) for x in patt_list_from_model]
	resolvedlist = []
	for item in pattstringlist:
		if '!+' in item:
			tempstr = item.split('+')[0]
			templist = [x for x in pattlist if tempstr in x]
			assert templist, item+" cannot be resolved/found in the model"
		else:
			assert item in pattlist, item+" cannot be resolved/found in the model"
			templist = [item]
		resolvedlist.extend(templist)
	return resolvedlist

	
# Classes and methods for building basic bipartite relations between rules, patterns,transformations and transformation pairs
class RuleMap:
	'''
	Class to contain maps from rules
	to transformations (r2t)
	to patterns (r2p_transfcenter, r2p_context, r2p_syndelcontext)
	'''
	def __init__(self,dictNames):
	
		dictRules = dictNames.r
		dictPatterns = dictNames.p
		dictTransformations = dictNames.t
		
		self.r2t = set( [ (dictRules[r],dictTransformations[t]) for r in dictRules.keys() for t in r.transformations ] )
		self.r2p_transfcenter = set( [ (dictRules[r],dictPatterns[p]) for r in dictRules.keys() for item in r.transf_center_lhs for p in item ] )
		self.r2p_context = set( [ (dictRules[r],dictPatterns[p]) for r in dictRules.keys() for p in r.context ] )
		self.r2p_syndelcontext = set( [ (dictRules[r],dictPatterns[p]) for r in dictRules.keys() for item in r.syndel_context for p in item ] )

 
class TransformationMap:
	'''
	Class to contain maps from transformations
	to patterns (t2p_reactant, t2p_product, t2p_context, t2p_syndelcontext
	'''
	def __init__(self,dictNames,rule_map):
		dictRules = dictNames.r
		dictPatterns = dictNames.p
		dictTransformations = dictNames.t
		
		self.t2action = dict([(dictTransformations[t],t.action) for t in dictTransformations.keys()])
	
		self.t2p_reactant = list(set( [ (dictTransformations[t],dictPatterns[p]) for t in dictTransformations.keys() for p in t.getLHS()] ))
		self.t2p_product = list(set( [ (dictTransformations[t],dictPatterns[p]) for t in dictTransformations.keys() for p in t.getRHS()] ))
		
		self.t2p_context =  set()
		self.t2p_context.update([  (dictTransformations[t],dictPatterns[p]) for r in dictRules.keys() for t in r.transformations for p in r.context])
		self.t2p_context.update([  (dictTransformations[t],dictPatterns[p]) for r in dictRules.keys() for t in r.transformations for tr in r.transformations for p in tr.getLHS() if tr!=t ])
		self.t2p_context = list(set(self.t2p_context))
		
		self.t2p_syndelcontext = list(set( [ (dictTransformations[t],dictPatterns[p]) for r in dictRules.keys() for idx,t in enumerate(r.transformations) for p in r.syndel_context[idx] if t.isSynDel() ] ))
		
class TransformationPairMap:
	'''
	Class to contain maps from transformation pairs
	to transformations (tp2t_forward, tp2t_reverse) -dicts
	to patterns (tp2p_forwardreactant, tp2p_reversereactant, tp2p_forwardcontext, tp2p_reversecontext, tp2p_syncontext, tp2p_delcontext, 
	'''
	def __init__(self,dictNames,tr_map):

		dictTransformations = dictNames.t
		dictTransformationPairs = dictNames.tp	
	
		self.tp2actions = dict([(dictTransformationPairs[tp],[tp.forward.action,tp.reverse.action]) for tp in dictTransformationPairs.keys()])
	
		self.tp2t_forward = dict([ [dictTransformationPairs[tp], dictTransformations[tp.forward]] for tp in dictTransformationPairs.keys() ] )
		self.tp2t_reverse = dict([ [dictTransformationPairs[tp], dictTransformations[tp.reverse]] for tp in dictTransformationPairs.keys() ] )

		self.tp2p_forwardreactant = list(set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_reactant if t_id==self.tp2t_forward[tp_id] ] ))
		self.tp2p_reversereactant = list(set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_reactant if t_id==self.tp2t_reverse[tp_id] ] ))

		self.tp2p_forwardcontext = set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_context if t_id==self.tp2t_forward[tp_id] ] )
		self.tp2p_reversecontext = set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_context if t_id==self.tp2t_reverse[tp_id] ] )
		
		syndel_list = [(tp_id,t_id,dictNames.getElement('t',t_id).action) for tp_id,t_id in list(self.tp2t_forward.items())+list(self.tp2t_reverse.items()) ]
		self.tp2p_syncontext = [ (tp_id,p_id) for tp_id,t_id,action in syndel_list for (t_id2,p_id) in tr_map.t2p_syndelcontext if t_id==t_id2 and action=='Add']
		self.tp2p_delcontext = [ (tp_id,p_id) for tp_id,t_id,action in syndel_list for (t_id2,p_id) in tr_map.t2p_syndelcontext if t_id==t_id2 and action=='Delete']
					

	
		
class allMaps:
	'''
	Class to contain all map objects
	'''
	def __init__(self,rule_map,tr_map,trpair_map):
		self.r = rule_map
		self.t = tr_map
		self.tp = trpair_map
		
	def getFlow(self, type_vector, idx_list):
		if type_vector == ['p','t']:
			# get transformations that contain the pattern as reactant, context or delcontext
			list1 = unq([t_id for t_id,p_id1 in self.t.t2p_reactant for p_id2 in idx_list if p_id1==p_id2])
			list2 = unq([t_id for t_id,p_id1 in self.t.t2p_context for p_id2 in idx_list if p_id1==p_id2])
			list3 = unq([t_id for t_id,p_id1 in self.t.t2p_syndelcontext for p_id2 in idx_list if self.t.t2action[t_id]=='Delete' and p_id1==p_id2])
			return unq(combineLists([list1,list2,list3]))
		if type_vector == ['p','tp']:
			# get transformation pairs that contain the pattern as forwardreactant, forward or reverse context, or delcontext
			list1 = unq([tp_id for tp_id,p_id1 in self.tp.tp2p_forwardreactant for p_id2 in idx_list if p_id1==p_id2])
			list2 = unq([tp_id for tp_id,p_id1 in self.tp.tp2p_forwardcontext for p_id2 in idx_list if p_id1==p_id2])
			list3 = unq([tp_id for tp_id,p_id1 in self.tp.tp2p_reversecontext for p_id2 in idx_list if p_id1==p_id2])
			list4 = unq([tp_id for tp_id,p_id1 in self.tp.tp2p_delcontext for p_id2 in idx_list if p_id1==p_id2])
			return unq(combineLists([list1,list2,list3,list4]))
		if type_vector == ['t','p']:
			# get transformations that contain the pattern as product or syncontext
			list1 = unq([p_id for t_id1,p_id in self.t.t2p_product for t_id2 in idx_list if t_id1==t_id2])
			list2 = unq([p_id for t_id1,p_id in self.t.t2p_syndelcontext for t_id2 in idx_list if self.t.t2action[t_id]=='Add' and t_id1==t_id2])
			return unq(combineLists([list1,list2]))
		if type_vector == ['tp','p']:
			# get transformation pairs that contain the pattern as product or syncontext
			list1 = unq([p_id for tp_id1,p_id in self.tp.tp2p_reversereactant for tp_id2 in idx_list if tp_id1==tp_id2])
			list2 = unq([p_id for tp_id1,p_id in self.tp.tp2p_syncontext for tp_id2 in idx_list if tp_id1==tp_id2])
			return unq(combineLists([list1,list2]))

def getMaps(names):
	'''
	Takes dictionaries of rules, patterns, transformations, transformation pairs
	returns map objects from RuleMap, TransformationMap and TransformationPairMap classes
	'''
	
	
	print "Building maps between elements..."

	rule_map = RuleMap(names)
	print "\nMaps to reaction rules:"
	print "There are",len(rule_map.r2t),"maps to transformations,"
	print len(rule_map.r2p_transfcenter),"maps from patterns to reaction centers,"
	print len(rule_map.r2p_context),"maps from patterns to reaction contexts, and"
	print len(rule_map.r2p_syndelcontext),"maps from patterns that are syndel contexts."
	
	tr_map = TransformationMap(names,rule_map)
	print "\nMaps to transformations:"
	print "There are",len(tr_map.t2p_reactant),"maps from reactant patterns,"
	print len(tr_map.t2p_product),"maps from product patterns,"
	print len(tr_map.t2p_context),"maps from context patterns, and"
	print len(tr_map.t2p_syndelcontext),"maps from patterns that are syndel contexts."
	
	trpair_map = TransformationPairMap(names,tr_map)
	print "\nMaps to transformation pairs:"
	print "There are",len(trpair_map.tp2p_forwardreactant),"forward reactant patterns and",len(trpair_map.tp2p_reversereactant),"reverse reactant patterns,"
	print len(trpair_map.tp2p_forwardcontext),"forward context patterns and",len(trpair_map.tp2p_reversecontext),"reverse context patterns, and"
	print len(trpair_map.tp2p_syncontext),"synthesis context patterns and",len(trpair_map.tp2p_delcontext),"deletion context patterns."
	
	
	#return rule_map,tr_map,trpair_map
	return allMaps(rule_map,tr_map,trpair_map)
	
#methods for more complex stuff based on bipartite relations
	
def makeFlow(names,all_maps,start,end):
	'''
	Creates a flow from starting pattern list to ending pattern list
	Returns a dictionaries of patterns and transformation groups and irreversibles with levels attached to them
	Level meaning: 0 - not important/default, 1 - start, -1 or max - end
	'''
	start  = resolveBondWildcards(start,names.p.keys())
	end  = resolveBondWildcards(end,names.p.keys())
	
	start = [names.p[x] for x in names.p.keys() if str(x) in start]
	end = [names.p[x] for x in names.p.keys() if str(x) in end]
	
	p_levels = defaultDict(names.p.values(),0)
	tp_levels = defaultDict(names.tp.values(),0)
	irr_levels = defaultDict(names.irr.values(),0)
	
	curr_level = 1
	p_levels = assignVal(p_levels,start,1)
	stopCriterion = False
	

	while not stopCriterion:
		tp_set = all_maps.getFlow(['p','tp'],[p_id for p_id,level in p_levels.items() if level==curr_level])
		tp_set_pruned = [tp_id for tp_id in tp_set if tp_levels[tp_id]==0]
		tp_levels = assignVal(tp_levels,tp_set_pruned,curr_level)

		curr_level = curr_level+1
		p_set = all_maps.getFlow(['tp','p'],[tp_id in tp_set_pruned])
		p_set_pruned = [p_id for p_id in p_set if p_levels[p_id]==0]
		p_levels = assignVal(p_levels,p_set_pruned,curr_level)
		if len(tp_set_pruned)==0 or len(p_set_pruned)==0:
			stopCriterion=True
	
	print p_levels
	print tp_levels
	
	
	

	return "Hi"	
	
#methods for handling dictionaries and lists and generic bulk actions
def unq(list1):
	return list(set(list1))
 
def combineLists(listoflists):
	return reduce(lambda x,y: x+y,listoflists)
	
def printDict(somedict):
	return "\n".join(sorted([str(x)+":"+str(y) for x,y in sorted(somedict.items())]))
	
def defaultDict(somelist,defaultval):
	return dict([x,defaultval] for x in somelist)

def assignVal(somedict,somelistofkeys,val):
	tempdict = somedict
	for item in somelistofkeys:
		if somedict[item]==0 or somedict[item]>val:
			tempdict[item] = val
	return tempdict

if __name__ == "__main__":
	atomizedrules = bpgModel.getAtomizedRules(sys.argv[-1])
	patterns,transformations = bpgModel.getElements(atomizedrules)
	
	tprules = [['+','!'],['~Y','~pY'],['0()','*']]
	
	transformationpairs, irreversibles = getTransformationPairs(transformations,tprules)
	
	names = getNameDictionary(atomizedrules,patterns,transformations,transformationpairs)
	
	all_maps = getMaps(names)
	

	
	# use d3js to make graphs
	
	

