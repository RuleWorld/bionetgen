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
    errorFiles = []
    #dont skip the files that only have warnings    
    for x in logFiles:    
        with open('./raw/' + x +'.log','r') as f:
            k = f.readlines()
            if 'ERROR' in ','.join(k):
                errorFiles.append(x)
    bnglFiles = [x for x in onlyfiles if 'bngl' in x and 'log' not in x]
    validFiles = [x for x in bnglFiles if x not in errorFiles]
    skip = ['334','225','332']
    with open('executionTestErrors' + '.log', 'w') as f:
        subprocess.call(['rm','*net'])
        for bnglFile in validFiles:
            
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
                if 'cvode' in ','.join(lines):
                    print '///',bnglFile
                    tag = 'cvode'
                elif 'time' in ','.join(lines):
                    print '\\\\\\',bnglFile
                elif 'Incorrect number of arguments' in ','.join(lines):
                    print '[[]]',bnglFile
                else:
                    print '---',bnglFile
                    tag = lines
                    f.write('%s %s\n' % (bnglFile,tag))
                f.flush()
            else:
                print '+++',bnglFile
        

if __name__ == "__main__":
    main()