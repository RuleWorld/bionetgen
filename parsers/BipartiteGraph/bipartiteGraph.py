
from readBNGXML import parseXML
import sys
import re
from collections import deque
import exceptions
from math import ceil
import string
import argparse
import os
import structures as st

def extractMolecules(action,site1,site2,chemicalArray):
    '''
    this method goes through the chemicals in a given array 'chemicalArray'
    and extracts its atomic patterns into two arrays:
        those elements that are contained in [site1,site2] will be put in the
        reactionCenter set. The rest will    be placed in the context set.
        The entire system will be put into the atomicPatterns dictionary
    Keyword arguments:
    site1,site2 -- where the action takes place
    chemicalArray -- the list of species we will be extracting atomic patters 
    from
    '''
    atomicPatterns = {}
    reactionCenter = set()
    context = set()
    
    #if action=='AddBond':
    #	print action,chemicalArray
    for reactant in chemicalArray:
        ta,tr,tc = reactant.extractAtomicPatterns(action,site1,site2)
        #for element in ta:
        #    atomicPatterns.add(element)
        atomicPatterns.update(ta)
        for element in tr:
            reactionCenter.add(element)
        for element in tc:
            context.add(element)   	
    return atomicPatterns,reactionCenter,context

 
def getMapping(mapp,site):
    for mapping in mapp:
        if site in mapping:
            return [x for x in mapping if x != site][0]                

         
def extractTransformations(rules):
    '''
    goes through the list of rules and extracts its reactioncenter,context and product
    atomic patterns per transformation action
    also resolves wildcard patterns to create additional context links and deleting the wildcard pattern - js
    '''
    atomicArray = {}    
    transformationCenter = []
    transformationContext = []
    productElements = []
    actionName = []
    index=0
    label = []

    #for idx,(react,product,act,mapp,nameDict) in enumerate(rules):
    #	print idx
	#print "react\t"," ".join([str(x) for x in react])
	#print "product\t"," ".join([str(x) for x in product])
	#print "act\t"," ".join([str(x) for x in act])
	#print "mapp\t"," ".join([str(x) for x in mapp])
	#print "nameDict\t"," ".join([x+":"+y for x,y in nameDict.items()])
    
		
    for react,product,act,mapp,nameDict in rules:
        index += 1
        for action in act:
	    atomic,reactionCenter,context = extractMolecules(action.action,action.site1, 
                                                             action.site2, react)
     	    atomicArray.update(atomic)
            # this method does not extract reaction centers for creation and deletion transformations
            # however it extracts context correctly
            # so generate the reactioncenter here
            


            if action.action == 'Delete':
            	temp = st.Species()
            	temp.addMolecule(st.Molecule(nameDict[action.site1],1))
            	atomic = dict()
            	atomic[str(temp)] = temp
            	atomicArray.update(atomic)
            	transformationCenter.append(set([str(temp)]))
            	productElements.append(set())
            	transformationContext.append(context)

            	
            elif action.action == 'Add':
            	temp = st.Species()
            	temp.addMolecule(st.Molecule(nameDict[action.site1],1))
            	atomic = dict()
            	atomic[str(temp)] = temp
            	atomicArray.update(atomic)
            	transformationCenter.append(set())
            	productElements.append(set([str(temp)]))
            	transformationContext.append(context)
            	
            else:
		
		transformationCenter.append(reactionCenter)
		transformationContext.append(context)
		atomicArray.update(atomic) 
     		productSites = [getMapping(mapp,action.site1),getMapping(mapp, action.site2)]
		atomic,rc,_ = extractMolecules(action.action,productSites[0], productSites[1],product)           
		productElements.append(rc)
		atomicArray.update(atomic)
 

            
            	
            actionName.append('%i-%s' % (index,action.action))
            r= '+'.join([str(x) for x in react])
            p = '+'.join([str(x) for x in product])
            label.append('->'.join([r,p,'%i-%s' % (index,action.action)]))



    # resolving bond wildcards
    wildcards = [x for x in atomicArray if '!+' in x]
    bondedpatterns = [x for x in atomicArray if '!' in x and x not in wildcards]
    for item in wildcards:
    	loc = string.find(item,'+')
    	selected_bondedpatterns = [x for x in bondedpatterns if item[0:loc] in x]

    	for idx,set1 in enumerate(transformationContext):
    		if item in set1:
    			set1.update(selected_bondedpatterns)
    			set1.remove(item)
    			transformationContext[idx] = set1
    	del atomicArray[item]

    return atomicArray, transformationCenter, transformationContext, productElements,actionName,label
    
 
def createDictionaries(atomicArray,transformationCenter,productElements):
	'''
	creates dictionaries mapping patterns and transformations to numbers
	'''
	n = len(atomicArray)
	patternDict = dict(zip(atomicArray,range(len(atomicArray))))
	
	
	transformationDict = dict()
	for idx,item in enumerate(transformationCenter):
    		key =tuple([ tuple(transformationCenter[idx]),tuple(productElements[idx]) ])
		transformationDict[key] = idx
	return patternDict,transformationDict

# All T2P maps are Transformation2Patterns
def createT2Pmaps(patternDict,transformationDict,transformationCenter,transformationContext,productElements):
	'''
	creates maps from transformation to pattern with reactant, product and context relationships
	uses numbers in pattern and number dictionaries
	'''
	t2pReactant = list()
	t2pProduct = list()
	t2pContext = list()
	for tr,tr_id in transformationDict.items():
		re_list = tr[0]
		pd_list = tr[1]
		
		for item in re_list:
			t2pReactant.append([tr_id,patternDict[item]])
		for item in pd_list:
			t2pProduct.append([tr_id,patternDict[item]])
			
		#for item in pd_list:
		#	print item
	#Context is a little harder. Need to pick out the transformations from the list
	#that match each unique transformation and then add the context
	
	for tr,tr_id in transformationDict.items():
		for idx,item in enumerate(transformationCenter):
			check =tuple([ tuple(transformationCenter[idx]),tuple(productElements[idx]) ])
			if tr==check:
				ct_list = transformationContext[idx]
				for item2 in ct_list:
					temp = [tr_id,patternDict[item2]]
					if temp not in t2pContext:
						t2pContext.append(temp)
	return t2pReactant, t2pProduct, t2pContext

def createTransformationPairs(t2pReactant,t2pProduct,t2pContext):
	tr_id_list = set()
	tr_id_list.update([x for x,y in t2pReactant])
	tr_id_list.update([x for x,y in t2pProduct])
	reactDict = dict()
	prodDict = dict()
	for tr in tr_id_list:
		r = [y for x,y in t2pReactant if x==tr]
		p = [y for x,y in t2pProduct if x==tr]
		#print r,p
		
		reactDict[tr] = r
		prodDict[tr] = p
		#print tr,reactDict[tr],prodDict[tr]
	trPairs = []
	tr_id_list = deque(tr_id_list)
	
	#while len(tr_id_list)>0:
	#	tr1 = tr_id_list.popleft()
	#	for tr2 in tr_id_list:
	#		if prodDict[tr1] == reactDict[tr2]:
	#			print tr1,tr2
	#		if reactDict[tr1] == prodDict[tr2]:
	#			print tr1,tr2
	

		#tr_id2 = [x for x in tr_id_list if (prodDict[tr_id1]==reactDict[x] and reactDict[tr_id1] == prodDict[x])][0]
		#tr_id_list.remove(tr_id2)
		#trPairs.append([tr_id1,tr_id2])
	return trPairs

def sortTransformationPairs(trPairs,transformationDict):

	trPairs2 = []
	tDict = dict([[v,k] for k,v in transformationDict.items()])
	for tr1,tr2 in trPairs:
		lhs = tDict[tr1][0]
		rhs = tDict[tr1][1]
		print len(lhs),len(rhs)	

	return trPairs

def remapToTransformationPairs(trPairs,t2pReactant,t2pProduct,t2pContext):
	# fc - forward consumptive - pattern is reactant for first item of trPair, i.e. forward transformation
	# rc - reverse consumptive - pattern is reactant for second item of trPair, i.e. reverse transformation
	# fi - forward influential - pattern is context for first item of trPair, i.e. forward transformation
	# ri - reverse influential - pattern is context for second item of trPair, i.e. reverse transformation
	fc = []
	rc = []
	fi = []
	ri = []
	for trPair in trPairs:
		fc_items = [y for x,y in t2pReactant if x==trPair[0]]
		rc_items = [y for x,y in t2pReactant if x==trPair[1]]
		fi_items = [y for x,y in t2pContext if x==trPair[0]]
		ri_items = [y for x,y in t2pContext if x==trPair[1]]	
		for item in fc_items:
			fc.append([tuple(trPair),item])
		for item in rc_items:
			rc.append([tuple(trPair),item])
		for item in fi_items:
			fi.append([tuple(trPair),item])
		for item in ri_items:
			ri.append([tuple(trPair),item])
	return fc,rc,fi,ri

	
def summarizeModel(patternDict,transformationDict,atomicPatternAnnotations,transformationAnnotations,t2pContext):
	with open('model_summary.txt','w') as f:
		# Summarizing Atomic Pattern Annotations
		f.write('Model entities:\n')
		maxsize = max([len(str(item)) for item in atomicPatternAnnotations.keys()])
    		for key,val in sorted(atomicPatternAnnotations.iteritems()):
    			f.write('%-4s: %-*s:\t%s\n' %(patternDict[key],maxsize,str(key),str(val)))
		f.write('\n\n')
			
		# Summarizing  Transformation Annotations
		newdict = dict()
		for key,annot in transformationAnnotations.items():
    			lhs = '+'.join(key[0])
		    	rhs = '+'.join(key[1])
    			rulestring = lhs + " -> " + rhs
    			newdict[transformationDict[key]] = rulestring
		
		maxsize = max([len(str(item)) for item in newdict.keys()])
    		f.write('Model processes:\n')
    		for key,val in sorted(transformationAnnotations.iteritems()):
			#f.write('%-*s\t%s\n' %(maxsize,str(key),str(val)))
			f.write('%-4s: %-*s:\t%s\n' %(transformationDict[key],maxsize,newdict[transformationDict[key]],str(val)))
		f.write('\n\n')
			
		# Summarizing Context Annotations	
		invPatternDict = dict((v,k) for k,v in patternDict.items())
		f.write('Model interactions:\n')
		for tr,tr_id in transformationDict.items():
			f.write("Process: "+transformationAnnotations[tr]+"(" + str(tr_id) + ")\n")
			annot_list = [(str(atomicPatternAnnotations[invPatternDict[item[1]]])+ "(" + str(item[1]) + ")" ) for item in t2pContext if item[0]==tr_id]
			f.write("Kinetic Modifiers: " + ', '.join(annot_list)+"\n\n")
		
def writeAnnotationFiles(atomicPatternAnnotations,transformationAnnotations,patternDict,transformationDict):
	maxsize = max([len(str(item)) for item in atomicPatternAnnotations.keys()])
	with open('atomicPatternAnnotations.txt','w') as f:
    		f.write('#Atomic Pattern Annotations\n')
	    	for key,val in sorted(atomicPatternAnnotations.iteritems()):
			f.write('%-4s: %-*s:\t%s\n' %(patternDict[key],maxsize,str(key),str(val)))
	
	newdict = dict()
	for key,annot in transformationAnnotations.items():
    		lhs = '+'.join(key[0])
	    	rhs = '+'.join(key[1])
    		rulestring = lhs + " -> " + rhs
    		newdict[transformationDict[key]] = rulestring
    	maxsize = max([len(str(item)) for item in newdict.keys()])
    	with open('transformationAnnotations.txt','w') as f:
    		f.write('#Transformation Annotations\n')
    		for key,val in sorted(transformationAnnotations.iteritems()):
			f.write('%-4s: %-*s:\t%s\n' %(transformationDict[key],maxsize,newdict[transformationDict[key]],str(val)))
			
def readAnnotationFiles(patternDict,transformationDict):
	atomicPatternAnnotations = dict()
	with open('atomicPatternAnnotations.txt','r') as f:
		data = deque(f.readlines())
		assert data.popleft() == '#Atomic Pattern Annotations\n', 'Not a valid annotation file for atomic patterns'
		for line in data:
			line_contents =  deque(line.split(':'))
			for idx,item in enumerate(line_contents):
				line_contents[idx] = item.strip()
			patt_id = line_contents.popleft()
			patt = line_contents.popleft()
			assert(patt in patternDict.keys()), 'Pattern not found'
			atomicPatternAnnotations[patt] = ' '.join(line_contents)
	
	transformationAnnotations = dict()
	with open('transformationAnnotations.txt','r') as f:
		transformationAnnotations = dict()
		newdict = dict()
		for key in transformationDict.keys():
    			lhs = '+'.join(key[0])
		    	rhs = '+'.join(key[1])
    			rulestring = lhs + " -> " + rhs
    			rulestring = rulestring.strip()
    			newdict[rulestring] = key
		
		data = deque(f.readlines())
		assert data.popleft() == '#Transformation Annotations\n', 'Not a valid annotation file for transformations'
		for line in data:
			line_contents =  deque(line.split(':'))
			for idx,item in enumerate(line_contents):
				line_contents[idx] = item.strip()
			tr_id = line_contents.popleft()
			rulestring = line_contents.popleft()
			assert(rulestring in newdict.keys()), 'Transformation not found'
			#Note we are only comparing rule-strings to see if the transformation is present. May need refining!!
			transformationAnnotations[newdict[rulestring]] = ' '.join(line_contents)	
	return atomicPatternAnnotations, transformationAnnotations


def writeNewAnnotationFiles(atomicArray,transformationCenter,patternDict,transformationDict):
	atomicPatternAnnotations = dict()
    	for item in atomicArray:
    		atomicPatternAnnotations[item]=None
    	
	transformationAnnotations = dict()
	for idx,item in enumerate(transformationCenter):
    		key =tuple([ tuple(transformationCenter[idx]),tuple(productElements[idx]) ])
		transformationAnnotations[key] = None
	print "Overwriting annotation files..."
	writeAnnotationFiles(atomicPatternAnnotations,transformationAnnotations,patternDict,transformationDict)
	return atomicPatternAnnotations, transformationAnnotations
			
def writeDot(patternDict,transformationDict,atomicPatternAnnotations,transformationAnnotations,t2pReactant, t2pProduct, t2pContext,options):
	'''
	writes a dot file showing bipartite representation
	
	options is a string with characters r,p,c,a
	# r -> show reactant-transformation links
	# p -> show product-transformation links
	# c -> show context-transformation links
	# a -> show annotations instead of code
	'''

	with open('bipartite_map_'+options+'.dot','w') as f:
		f.write('digraph bipartite_map_' + options + '{\n')
		f.write('ranksep=5;\n')
		f.write('rankdir=LR;\n')
		#f.write('ratio=compress')
		#  Lefthand pattern Nodes
		f.write('subgraph cluster_reactants {\n')
		f.write('rankdir=TB;\n')
		f.write('node [shape=box];\n')
		for patt,patt_id in sorted(patternDict.items()):
			name = "L"+str(patt_id)
			if 'a' not in options:
				label= patt
			else:
				label = atomicPatternAnnotations[patt]
			f.write("\t"+name+" [label=\""+label+"\"];\n")
		f.write('}\n')
		#  Transformation Nodes
		f.write('subgraph cluster_transformations {\n')
		f.write('rankdir=TB;\n')
		f.write('ranksep=5;\n')
		f.write('node [shape=oval];\n')
		for tr,tr_id in sorted(transformationDict.items()):
			name = "T"+str(tr_id)
			if 'a' not in options:
				label= getRuleString(tr)
			else:
				label = transformationAnnotations[tr]
			f.write("\t"+name+" [label=\""+label+"\"];\n")
		f.write('}\n')
		#  Righthand pattern Nodes
		f.write('subgraph cluster_products {\n')
		f.write('rankdir=TB;\n')
		f.write('node [shape=box];\n')
		for patt,patt_id in sorted(patternDict.items()):
			name = "R"+str(patt_id)
			if 'a' not in options:
				label= patt
			else:
				label = atomicPatternAnnotations[patt]
			f.write("\t"+name+" [label=\""+label+"\"];\n")
		f.write('}\n')
		
		# Ranking the same reactant and product nodes equally
		#for patt,patt_id in sorted(patternDict.items()):
		#	f.write('{ rank = same; \"r' + str(patt_id) + '\"; \"p' + str(patt_id) + '\"; }\n' )
		
		# Ranking the transformation nodes in the middle of the reactant and product nodes
		#n_reactants = len(patternDict.keys())
		#n_transforms = len(transformationDict.keys())
		#n_nodes_to_leave = int(ceil((n_reactants - n_transforms)/2))
		#print n_reactants, n_transforms, n_nodes_to_leave
		
		patt_id_list = [x for patt,x in sorted(patternDict.items())]
		tr_id_list = [x for tr,x in sorted(transformationDict.items())]
		#for idx, item in enumerate(tr_id_list):
		#	f.write('{ rank = same; \"r'+ str(patt_id_list[n_nodes_to_leave+idx]) + '\"; \"t' + str(tr_id_list[idx]) + '\"; }\n')
			#print idx
				
			
		
		if 'c' not in options:
			#left hand side is reactant, right hand side is product
			for tr_id,patt_id in t2pReactant:
				f.write("L" + str(patt_id) + " -> T" + str(tr_id) + " [color = black];\n")
			for tr_id,patt_id in t2pProduct:
				f.write("T" + str(tr_id) + " -> R" + str(patt_id) + " [color = black];\n")	
		if 'r' not in options:
			#left hand side is context, right hand side is product
			for tr_id,patt_id in t2pContext:
				f.write("L" + str(patt_id) + " -> T" + str(tr_id) + " [color = red];\n")
			for tr_id,patt_id in t2pProduct:
				f.write("T" + str(tr_id) + " -> R" + str(patt_id) + " [color = black];\n")	
		if 'p' not in options:
			# left hand side is context, right hand is reactant with reverse arrow
			for tr_id,patt_id in t2pContext:
				f.write("L" + str(patt_id) + " -> T" + str(tr_id) + " [color = red];\n")
			for tr_id,patt_id in t2pReactant:
				f.write("T" + str(tr_id) + " -> R" + str(patt_id) + " [color = black, dir=back];\n")
				
		if 'r' in options:
			if 'p' in options:
				if 'c' in options:
					# left hand side is reactants and context, right hand side is products
					for tr_id,patt_id in t2pReactant:
						f.write("L" + str(patt_id) + " -> T" + str(tr_id) + " [color = black];\n")
					for tr_id,patt_id in t2pContext:
						f.write("L" + str(patt_id) + " -> T" + str(tr_id) + " [color = red];\n")
					for tr_id,patt_id in t2pProduct:
						f.write("T" + str(tr_id) + " -> R" + str(patt_id) + " [color = black];\n")
					
		f.write("}\n")
		
			
	
def getRuleString(trans):
	lhs = '+'.join(trans[0])
	rhs = '+'.join(trans[1])
    	rulestring = lhs + " -> " + rhs
    	return rulestring
	
				

def printList(list1):
	for item in list1:
		print item
		
def printDict(dict1):
	#getting max size for column1
	maxsize = max([len(str(item)) for item in dict1.keys()])
	for key,val in dict1.iteritems():
		print '%-*s\t%s' %(maxsize,str(key),str(val))

def createXML(self):
    pass

def getStats(rules,patternDict,transformationDict):
	print "\n\n"
	print "Number of rules: ",len(rules)
	print "Number of transformations: ", len(transformationDict)
	print "Number of atomic patterns: ", len(patternDict)


#biograph-matlab
#graphviz, dotty
#d3js.org




if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument('-v','--verbose',dest='verbose',action="store_true",help='Always asks when overwriting files')
	subparsers = parser.add_subparsers(help='Choose one of the modes', dest='mode')
	
	annot_parser = subparsers.add_parser('annotate',help='Write new empty annotation files')
	bpg_parser = subparsers.add_parser('graph',help='Generate a bipartite graph')
	summ_parser = subparsers.add_parser('summarize',help='Summarizes model interactions using annotations')
	devel_parser = subparsers.add_parser('devel',help='Developer')
	
	
	bpg1 =bpg_parser.add_mutually_exclusive_group()
	bpg1.add_argument('--omit-reactants',help='Omit reactant relationships on the bipartite graph',action='store_true',dest='no_r')
	bpg1.add_argument('--omit-products',help='Omit product relationships on the bipartite graph',action='store_true',dest='no_p')
	bpg1.add_argument('--omit-contexts',help='Omit context relationships on the bipartite graph',action='store_true',dest='no_c')
	bpg_parser.add_argument('--use-annot',help='Use annotations instead of code',action='store_true',dest='use_annot')
	bpg_parser.add_argument('--filter',help='Textfile with list of patterns',required=False,dest='filter')
	bpg_parser.add_argument('--level',help='Depth to filter pattern list',required=False,dest='level',type=int)
	

	g1 = parser.add_argument_group('required arguments')
	g1.add_argument('-xml',dest='xml',required=True)
	
	args = parser.parse_args()
	
	
	if(os.path.isfile(args.xml)):
		# This generates all the pattern and transformation arrays and dictionaries
		_,rules = parseXML(args.xml)
		
		
		#rewrite required here:
		atomicArray = extractAtomicArray(rules)

			
		atomicArray, transformationCenter, transformationContext, productElements,actionName,label = extractTransformations(rules)
		
		
			
		
		patternDict,transformationDict = createDictionaries(atomicArray,transformationCenter,productElements)
		

		# This lets you create new empty annotation files
		if args.mode=='annotate':
			getStats(rules,patternDict,transformationDict)
			print "\n\nAnnotate mode."
			if args.verbose:
				tempstr = raw_input('This will overwrite any existing annotation files. Proceed? Y/N\t')
				if(tempstr == 'y' or tempstr== 'Y'):
					atomicPatternAnnotations, transformationAnnotations = writeNewAnnotationFiles(atomicArray,transformationCenter,patternDict,transformationDict)

					sys.exit()
			else:
				atomicPatternAnnotations, transformationAnnotations = writeNewAnnotationFiles(atomicArray,transformationCenter,patternDict,transformationDict)
				sys.exit()
		
		# This figures out the t2p maps (transformation to pattern)
		t2pReactant, t2pProduct, t2pContext = createT2Pmaps(patternDict,transformationDict,transformationCenter,transformationContext,productElements)

		#if args.mode == 'devel':
		#	printDict(patternDict)
		#	printDict(transformationDict)
			
			#trPairs = createTransformationPairs(t2pReactant,t2pProduct,t2pContext)
			#trPairs = sortTransformationPairs(trPairs,transformationDict)
			#trPairs = sortTransformationPairs(trPairs)
			#fc,rc,fi,ri = remapToTransformationPairs(trPairs,t2pReactant,t2pProduct,t2pContext)

		if args.mode=='graph':
			getStats(rules,patternDict,transformationDict)
			
			print "\n\nGraphing mode."
			if os.path.isfile('atomicPatternAnnotations.txt') and os.path.isfile('transformationAnnotations.txt'):
				atomicPatternAnnotations, transformationAnnotations = readAnnotationFiles(patternDict,transformationDict)
			else:
				print "Annotations not found. Generating empty templates. Edit them with your favorite text editor."
				if args.use_annot:
					print "Use-annot will be disabled. Use annotate mode to create empty editable templates."
					args.use_annot = False;
			if args.filter:
				if not args.level:
					args.level = 1
				elif args.level < 0:
					args.level = abs(args.level)
				if args.level <= 1 :
					itermax = 1
				else:
					itermax = args.level

				
				with open(args.filter,'r') as f:
					data = f.read()
					templist = re.findall(r"[\w()~!+]+",data)
					patlist = set()
					for item in templist:
						if '!+' in item:
							loc = string.find(item,'+')
							newlist = [x for x in patternDict.keys() if item[0:loc] in x]
							assert len(newlist) > 0, 'Bond pattern ' + item + ' not found'
							patlist.update(newlist)
						else:
							assert item in patternDict.keys(), 'Bond pattern ' + item + ' not found'
							patlist.update([item])
    							
						
				print patlist
				pat_ids = [patternDict[x] for x in patlist]
				
				for lev in xrange(0,itermax+1):
					print lev
						
			
			options_string = ''
			if args.use_annot:
				options_string = options_string + 'a'	

			if not args.no_r:
				options_string = options_string + 'r'
			if not args.no_c:
				options_string = options_string + 'c'
			if not args.no_p:
				options_string = options_string + 'p'
			print "Generating dot file."
			writeDot(patternDict,transformationDict,atomicPatternAnnotations,transformationAnnotations,t2pReactant, t2pProduct, t2pContext,options_string)
			
		
		
		if args.mode=='summarize':
			getStats(rules,patternDict,transformationDict)
			print "\n\nSummarize mode."
			print "Generating model summary"
			if os.path.isfile('atomicPatternAnnotations.txt') and os.path.isfile('transformationAnnotations.txt'):
				atomicPatternAnnotations, transformationAnnotations = readAnnotationFiles(patternDict,transformationDict)
				
			else:
				print "Annotations not found. Cannot summarize. Use annotate mode to create empty templates and edit them with your favorite text editor."
				sys.exit()
			atomicPatternAnnotations, transformationAnnotations = readAnnotationFiles(patternDict,transformationDict)
			summarizeModel(patternDict,transformationDict,atomicPatternAnnotations,transformationAnnotations,t2pContext)
		
					
	else:
		print "Something wrong with reading bngxml file"
		sys.exit()
