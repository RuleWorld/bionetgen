import sys
import time
import os
import os.path
from libsbml import *

def parameterBlockString(model):
	pblock = 'begin parameters\n'
	plist = model.getListOfParameters()
	for i in plist:
		pblock = pblock + plist[0].id +'\t'+str(plist[0].value)+'\n'
	pblock = pblock + 'end parameters\n'
	return {'pblock':pblock,'plist':plist}

	
if __name__ == "__main__":
	filename = sys.argv[1];
	reader = SBMLReader()
	document = reader.readSBML(filename);
	errors = document.getNumErrors();
	model = document.getModel()
 	tmp = parameterBlockString(model)
 	print tmp['pblock']