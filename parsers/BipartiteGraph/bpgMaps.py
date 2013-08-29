'''
Creates transformation pairs, creates various dictionaries and infers various maps between rules, patterns, transformations, transformation maps

Usage: 
transformationpairs, irreversibles = bpgMaps.getTransformationPairs(transformations,tprules)
dictRules,dictPatterns,dictTransformations,dictTransformationPairs = bpgMaps.getNameDictionaries(atomizedrules,patterns,transformations,transformationpairs) 
rule_map,tr_map,trpair_map = bpgMaps.getMaps(dictRules,dictPatterns,dictTransformations,dictTransformationPairs)

Dependencies: bpgModel.py

Author: John Sekar
'''

import bpgModel
import sys
import itertools

class TransformationPair:
	'''
	Pair of transformations whose reactant and product are mirrors of each other
	'''
	def __init__(self,trlist):
		self.forward = trlist[0]
		self.reverse = trlist[1]
		self.annotate = ''
	
	def __str__(self):
		return " ".join([self.strLHS(),"<->",self.strRHS()])
		
	def strLHS(self):
		return self.forward.strLHS()
	def strRHS(self):
		return self.forward.strRHS()

	def flip(self):
		x =  TransformationPair((self.reverse,self.forward))
		x.annotate = self.annotate
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
	
def getNameDictionaries(atomizedrules,patterns,transformations,transformationpairs):
	'''
	Creates a dictionaries by mapping unique rules, patterns, transformations and transformationpairs to unique ids
	'''

	r_dict = dict()
	for idx,rule in enumerate(atomizedrules):
		r_dict[rule] = str(idx)
		#r_dict["R"+str(idx)] = rule
		
	p_dict = dict()
	for idx,patt in enumerate(patterns):
		p_dict[patt] = str(idx)
		#p_dict["P"+str(idx)] = patt

	t_dict = dict()
	for idx,tr in enumerate(transformations):
		t_dict[tr] = str(idx)
		#t_dict["T"+str(idx)] = tr
		
	tp_dict = dict()
	for idx,trp in enumerate(transformationpairs):
		tp_dict[trp] = str(idx)
		#tp_dict["TP"+str(idx)] = trp
		
	return r_dict,p_dict,t_dict,tp_dict


class RuleMap:
	'''
	Class to contain maps from rules
	to transformations (r2t)
	to patterns (r2p_transfcenter, r2p_context, r2p_syndelcontext)
	'''
	def __init__(self,dictRules,dictPatterns,dictTransformations):
		self.r2t = set( [ (dictRules[r],dictTransformations[t]) for r in dictRules.keys() for t in r.transformations ] )
		self.r2p_transfcenter = set( [ (dictRules[r],dictPatterns[p]) for r in dictRules.keys() for item in r.transf_center_lhs for p in item ] )
		self.r2p_context = set( [ (dictRules[r],dictPatterns[p]) for r in dictRules.keys() for p in r.context ] )
		self.r2p_syndelcontext = set( [ (dictRules[r],dictPatterns[p]) for r in dictRules.keys() for item in r.syndel_context for p in item ] )

 
class TransformationMap:
	'''
	Class to contain maps from transformations
	to patterns (t2p_reactant, t2p_product, t2p_context, t2p_syndelcontext
	'''
	def __init__(self,dictRules,dictPatterns,dictTransformations,rule_map):
		self.t2p_reactant = set( [ (dictTransformations[t],dictPatterns[p]) for t in dictTransformations.keys() for p in t.getLHS()] )
		self.t2p_product = set( [ (dictTransformations[t],dictPatterns[p]) for t in dictTransformations.keys() for p in t.getRHS()] )
		
		self.t2p_context =  set()
		self.t2p_context.update([  (dictTransformations[t],dictPatterns[p]) for r in dictRules.keys() for t in r.transformations for p in r.context])
		self.t2p_context.update([  (dictTransformations[t],dictPatterns[p]) for r in dictRules.keys() for t in r.transformations for tr in r.transformations for p in tr.getLHS() if tr!=t ])
		
		self.t2p_syndelcontext = set( [ (dictTransformations[t],dictPatterns[p]) for r in dictRules.keys() for idx,t in enumerate(r.transformations) for p in r.syndel_context[idx] if t.isSynDel() ] )
		
class TransformationPairMap:
	'''
	Class to contain maps from transformation pairs
	to transformations (tp2t_forward, tp2t_reverse) -dicts
	to patterns (tp2t_forwardreactant, tp2t_reversereactant, tp2t_forwardcontext, tp2t_reversecontext, tp2t_forwardsyndelcontext, tp2t_reversesyndelcontext, 
	'''
	def __init__(self,dictRules,dictPatterns,dictTransformations,dictTransformationPairs,tr_map):
		self.tp2t_forward = dict([ [dictTransformationPairs[tp], dictTransformations[tp.forward]] for tp in dictTransformationPairs.keys() ] )
		self.tp2t_reverse = dict([ [dictTransformationPairs[tp], dictTransformations[tp.reverse]] for tp in dictTransformationPairs.keys() ] )

		self.tp2p_forwardreactant = set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_reactant if t_id==self.tp2t_forward[tp_id] ] )
		self.tp2p_reversereactant = set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_reactant if t_id==self.tp2t_reverse[tp_id] ] )

		self.tp2p_forwardcontext = set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_context if t_id==self.tp2t_forward[tp_id] ] )
		self.tp2p_reversecontext = set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_context if t_id==self.tp2t_reverse[tp_id] ] )
		
		self.tp2p_forwardsyndelcontext = set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_syndelcontext if t_id==self.tp2t_forward[tp_id]  ] )
		self.tp2p_reversesyndelcontext = set( [ (tp_id,p_id) for tp_id in dictTransformationPairs.values() for t_id,p_id in tr_map.t2p_syndelcontext if t_id==self.tp2t_reverse[tp_id]  ] )

 		
def getMaps(atomizedrules,patterns,transformations,transformationpairs):
	'''
	Takes atomized rules, patterns, transformations and transformation pairs
	returns dictionaries of rules, patterns, transformations, transformation pairs
	returns map objects from RuleMap, TransformationMap and TransformationPairMap classes
	'''
	print "\nBuilding dictionaries of unique elements..."
	dictRules,dictPatterns,dictTransformations,dictTransformationPairs = getNameDictionaries(atomizedrules,patterns,transformations,transformationpairs)
	
	print "Building maps between elements..."

	rule_map = RuleMap(dictRules,dictPatterns,dictTransformations)
	print "\nMaps to reaction rules:"
	print "There are",len(rule_map.r2t),"maps to transformations,"
	print len(rule_map.r2p_transfcenter),"maps from patterns to reaction centers,"
	print len(rule_map.r2p_context),"maps from patterns to reaction contexts, and"
	print len(rule_map.r2p_syndelcontext),"maps from patterns that are syndel contexts."
	
	tr_map = TransformationMap(dictRules,dictPatterns,dictTransformations,rule_map)
	print "\nMaps to transformations:"
	print "There are",len(tr_map.t2p_reactant),"maps from reactant patterns,"
	print len(tr_map.t2p_product),"maps from product patterns,"
	print len(tr_map.t2p_context),"maps from context patterns, and"
	print len(tr_map.t2p_syndelcontext),"maps from patterns that are syndel contexts."
	
	trpair_map = TransformationPairMap(dictRules,dictPatterns,dictTransformations,dictTransformationPairs,tr_map)
	print "\nMaps to transformation pairs:"
	print "There are",len(trpair_map.tp2p_forwardreactant),"forward reactant patterns and",len(trpair_map.tp2p_reversereactant),"reverse reactant patterns,"
	print len(trpair_map.tp2p_forwardcontext),"forward context patterns and",len(trpair_map.tp2p_reversecontext),"reverse context patterns, and"
	print len(trpair_map.tp2p_forwardsyndelcontext),"forward context patterns (syndel) and",len(trpair_map.tp2p_reversesyndelcontext),"reverse context patterns (syndel)."
	
	
	return dictRules,dictPatterns,dictTransformations,dictTransformationPairs,rule_map,tr_map,trpair_map
		
	
def printDict(somedict):
	return "\n".join(sorted([str(x)+":"+str(y) for x,y in sorted(somedict.items())]))
	

if __name__ == "__main__":
	atomizedrules = bpgModel.getAtomizedRules(sys.argv[-1])
	patterns,transformations = bpgModel.getElements(atomizedrules)
	
	tprules = [['+','!'],['~Y','~pY'],['0()','*']]
	
	transformationpairs, irreversibles = getTransformationPairs(transformations,tprules)
	
	dictRules,dictPatterns,dictTransformations,dictTransformationPairs, rule_map,tr_map,trpair_map = getMaps(atomizedrules,patterns,transformations,transformationpairs)
	

	
	# use d3js to make graphs
	
	

