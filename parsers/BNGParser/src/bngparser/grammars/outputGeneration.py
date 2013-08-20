# -*- coding: utf-8 -*-
"""
Created on Thu Aug 15 21:01:51 2013

@author: proto
"""

from os import listdir
from os.path import isfile, join
import re
import subprocess

if __name__ == "__main__":
    '''
    onlyfiles = [ f for f in listdir('./') if isfile(join('./',f)) ]
    gFiles = [x for x in onlyfiles if '.g' in x and '~' not in x and 'out' not in x]
    outFiles = []
    for gFile in gFiles:
        with open('{0}.out'.format(gFile),'w') as d:
            subprocess.Popen(['java', '-jar', '/opt/antlr-3.3-complete.jar', '-print', '{0}'.format(gFile)],stdout=d)
            d.flush()
    '''
    onlyfiles = [ f for f in listdir('./') if isfile(join('./',f)) ]
    outFiles = [x for x in onlyfiles if '.out' in x and 'final' not in x]

    for outFile in outFiles:
        print outFile
        with open(outFile,'r') as d:
            text = d.readlines()
            text = ['{0}\n'.format(x) for x in text]
            text = ''.join(text)
            text = re.sub(r'->[^;]+;',r';',text)
            text = re.sub(r'returns \[[^\]]+\]',r'',text)
            text = re.sub(r'\[[^\]]+\]',r'',text)
            print text
        f = open('{0}.final'.format(outFile),'w')
        f.write(text)
    
