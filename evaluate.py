# -*- coding: utf-8 -*-
"""
Created on Thu Feb  7 18:20:33 2013

@author: proto
"""
from os import listdir
from os.path import isfile, join
import subprocess

def main():
    onlyfiles = [ f for f in listdir('./raw') if isfile(join('./raw',f)) ]
    
    logFiles = [x[0:-4] for x in onlyfiles if 'log' in x]
    bnglFiles = [x for x in onlyfiles if 'log' not in x]
    validFiles = [x for x in bnglFiles if x not in logFiles]
    skip = ['334','225','332']
    with open('executionTestErrors' + '.log', 'w') as f:
        for bnglFile in validFiles:
            print bnglFile
            if len([x for x in skip if x in bnglFile]) > 0:
                continue
            with open('temp.tmp', "w") as outfile:
                d = open('dummy.tmp','w')
                result = subprocess.call(['/opt/RuleBender-2.0.271-lin64/BioNetGen-2.2.2/BNG2.pl', './raw/{0}'.format(bnglFile)],stderr=outfile,stdout=d)
                d.close()
                
            if  result > 0:
                with open('temp.tmp','r') as outfile:
                    lines = outfile.readlines()
                tag = ''
                if 'cvode' in lines[0]:
                    tag = 'cvode'
                else:
                    tag = lines
                f.write('%s %s\n' % (bnglFile,tag))
                f.flush()
        

if __name__ == "__main__":
    main()