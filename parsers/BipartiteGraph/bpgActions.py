'''
Methods implementing actions that can be used to create, manipulate, annotate and output bipartite maps

Usage: bpgActions.processActions(strings)

Dependencies: bpgModel.py, bpgMaps.py, bpgAnnotate.py

'''

import bpgModel
import bpgMaps
import bpgAnnotate

import sys
import os
from collections import deque



def cleanstrings(strings):
	''' preprocesses strings from actions file'''
	available = ['read_xml','tprule','make_pairs','make_maps','write_elements','read_annot','make_flow','make_viz']
	allcommands = []
	for item in strings.split("\n"):
		item2  = item.strip()
		item2 = item2.split()
		#item2 is a string now
		if len(item2)==0:
			continue
		elif item2[0][0] == '#':
			continue
		elif item2[0] not in available:
			print "Line: \'",item,"\' is not an action. Exiting!"
			exit()
		allcommands.append(item2)
	return deque(allcommands)

def processActions(strings):
	'''implements actions regarding bipartite maps from strings read from actions file'''

	allcommands = cleanstrings(strings)
	#Commands are processed linearly
	successfullyissued = []
	tprules = []

	while len(allcommands)>0:
		current_line = deque(allcommands.popleft())
		command = current_line.popleft()
		
		if command == 'read_xml':
			# Usage read_xml xyz.xml
			assert len(current_line)==1, "Provide exactly one argument for read_xml command"
			filename = current_line.popleft()
			assert os.path.isfile(filename),"File "+filename+"not found for read_xml command"
			atomizedrules = bpgModel.getAtomizedRules(filename)
			patterns,transformations = bpgModel.getElements(atomizedrules)
			
		elif command == 'tprule':
			# Usage tprule x > y (or) x < y
			assert 'make_pairs' not in successfullyissued, "Use tprule before make_pairs"
			temp = []
			assert len(current_line)==3, "Provide arguments of the form: x > y (or) x < y for tprule"
			temp.append(current_line.popleft())
			op = current_line.popleft()
			assert op in ['>','<'], "Provide arguments of the form: x > y (or) x < y for tprule"
			temp.append(current_line.popleft())
			if op=='>':
				tprules.append(temp)
			else:
				tprules.append([temp[1],temp[0]])
			
		elif command == 'make_pairs':
			# Usage make_pairs
			assert 'read_xml' in successfullyissued, "Use read_xml before make_pairs"
			assert len(current_line)==0, "make_pairs command takes no arguments"
			transformationpairs, irreversibles = bpgMaps.getTransformationPairs(transformations,tprules)

			
		elif command == 'write_elements':
			assert len(current_line)>0, "write_elements requires a filename to output to"
			if 'make_names' not in successfullyissued:
				names = bpgMaps.getNameDictionary(atomizedrules,patterns,transformations,transformationpairs,irreversibles)
				successfullyissued.append('make_names')
			filename = current_line.popleft()
			if len(current_line)==0:
				current_line='p-t-tp-r'
			printopts = current_line[0].split('-')
			for item in printopts:
				assert item in ['p','t','tp','ir','r',''],"Option "+item+" not found. Provided options should be a subset of p-t-tp-ir-r with dashes between them"
			tempstring = ''
			alreadyadded = []
			optdict = {'p':'patterns','t':'transformations','tp':'transformationpairs','ir':'irreversibles','r':'reactionrules'}
			for item in printopts:
				if item not in alreadyadded:
					if item=='':
						continue
					elif item=='p':
						templist = sorted([str(x) for x in patterns])
					elif item=='t':
						templist = sorted([str(x) for x in transformations])
					elif item=='tp':
						templist = sorted([str(x) for x in transformationpairs])
					elif item=='ir':
						templist = sorted([str(x) for x in irreversibles])
					elif item=='r':
						templist = [str(x) for x in atomizedrules]
					tempstring = tempstring + "\n["+optdict[item]+"]\n"
					tempstring = tempstring + "# " + str(len(templist)) + " " + optdict[item] + "\n"
					tempstring = tempstring + "\n".join(templist) + "\n"
					alreadyadded.append(item)
			with open(filename,'w') as f:		
				f.write(tempstring)
		
		elif command == 'read_annot':
			# Usage: read_annot <list of filenames>
			assert len(current_line)>0, "read_annot requires atleast one filename to read from"
			if 'make_names' not in successfullyissued:
				names = bpgMaps.getNameDictionary(atomizedrules,patterns,transformations,transformationpairs,irreversibles)
				successfullyissued.append('make_names')
			tempstring = ''
			for item in current_line:
				assert(os.path.isfile(item)),"File "+item+" not found!"
				with open(item) as f:
					tempstring = tempstring + f.read()
			if 'read_annot' not in successfullyissued:
				annot = bpgAnnotate.Annotation()
				annot.initialize(names)
				annot.processAnnotations(tempstring,names)
		
		elif command == 'make_maps':
			if 'make_names' not in successfullyissued:
				names = bpgMaps.getNameDictionary(atomizedrules,patterns,transformations,transformationpairs,irreversibles)
				successfullyissued.append('make_names')
			all_maps = bpgMaps.getMaps(names,0)
			
		elif command == 'make_flow':
			assert len(current_line)>1, "provide a seed of the form: start <list of atomic patterns> [end <list of atomic patterns>]"
			assert current_line.popleft()=='start',"provide a seed of the form: start <list of atomic patterns> [end <list of atomic patterns>]"
			start = []
			end = []
			startend = 0 # a switch used to create either the start vector or the end vector of patterns
			while(len(current_line)>0):
				patt = current_line.popleft()
				if patt != 'end' and startend == 0:
					start.append(patt)
				if startend ==1:
					end.append(patt)
				if patt == 'end':
					startend =1
				
			flowlevels = bpgMaps.makeFlow(names,all_maps,start,end)

		elif command == 'make_viz':
			assert len(current_line) > 0, "provide a filename for writing vizdata"
			filename = current_line.popleft()
			with open("data.js","w") as f:
				f.write("var data = "+bpgMaps.writeJSON(names,all_maps,annot))
			f.close()
			
		
		else:
			print "Command not found: "+command
			
		successfullyissued.append(command)	
	
if __name__ == "__main__":
	if os.path.isfile(sys.argv[-1]):
		with open(sys.argv[-1],'r') as f:
			processActions(f.read())
