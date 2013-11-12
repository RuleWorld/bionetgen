'''
Methods for annotating models, model elements and bipartite maps

Usage: annot = Annotation()
annot.processAnnotations(strings)
'''
from collections import deque

class Annotation:
	def __init__(self):
		self.r = dict()
		self.p = dict()
		self.t = dict()
		self.tp = dict()
			
	def initialize(self,nameDict):
		for rule,idx in nameDict.r.items():
			self.r[idx]=''
		for patt,idx in nameDict.p.items():
			self.p[idx]=''
		for tr,idx in nameDict.t.items():
			self.t[idx]=''
		for trp,idx in nameDict.tp.items():
			self.tp[idx]=''
	
	def copy(self):
		annot = Annotation()
		annot.r = self.r
		annot.p = self.p
		annot.t = self.t
		annot.tp = self.tp
		return annot
		
	def processAnnotations(self,tempstring,names):
		optdict = {'p':'patterns','t':'transformations','tp':'transformationpairs','ir':'irreversibles','r':'reactionrules'}
		opts = optdict.values()
		
		strings = deque(cleanstrings(tempstring))
		
		current_annot_type = ''
		
		while len(strings)>0:
			current_line = strings.popleft()

			# check if it begins a '['
			if current_line[0] == '[':
				assert current_line[-1] == ']', "Need to use ] to close section head"
				#update the current_annot_type
				current_annot_type = current_line[1:-1].strip()
				assert current_annot_type in opts, "Bad section head for annotations: "+current_annot_type+ ". Only use "+"/".join(opts)
			else:
				#check if you have a still current_annot_type
				assert current_annot_type != '',"Need a section head"
				assert current_annot_type in opts, "Bad section head for annotations: "+current_annot_type+ ". Only use "+"/".join(opts)
				self.updateDict(current_annot_type,current_line,names)
		#print self.p,self.tp



	def updateDict(self,annot_type,line,names):
		linesplit = line.split(':')
		assert len(linesplit)==2,"Bad annotation. Use element: annotation format."
		if annot_type in ['transformations','irreversibles']:
			assert linesplit[0] in  [str(x) for x in names.t.keys()],"Cannot find transformation:"+linesplit[0]
			relevant_t = names.getIdx('t',linesplit[0])
			self.t[relevant_t]=linesplit[1].strip()
		
		if annot_type in ['transformationpairs']:
			assert linesplit[0] in  [str(x) for x in names.tp.keys()],"Cannot find transformation pair:"+linesplit[0]
			relevant_tp = names.getIdx('tp',linesplit[0])
			self.tp[relevant_tp]=linesplit[1].strip()
		
		if annot_type in ['patterns']:
			assert linesplit[0] in  [str(x) for x in names.p.keys()],"Cannot find pattern:"+linesplit[0]
			relevant_p = names.getIdx('p',linesplit[0])
			self.p[relevant_p]=linesplit[1].strip()

		if annot_type in ['reactionrules']:
			assert linesplit[0] in  [str(x) for x in names.r.keys()],"Cannot find reaction rule:"+linesplit[0]
			relevant_r = names.getIdx('r',linesplit[0])
			self.r[relevant_r]=linesplit[1].strip()
			
	def __str__(self):
		print self.r
		print self.p
		print self.t
		print self.tp
		return "\n"


		
def cleanstrings(somestring):
	return [x for x in somestring.split("\n") if x is not '' and x[0] is not '#']

