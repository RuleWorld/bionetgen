# -*- coding: utf-8 -*-
"""
Created on Thu Feb  7 18:20:33 2013

@author: proto
"""
from os import listdir
from os.path import isfile, join
import os
import subprocess
import time
import datetime
import signal

def main():
    directory = 'raw'
    onlyfiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) ]
    
    logFiles = [x[0:-4] for x in onlyfiles if 'log' in x]
    errorFiles = []
    #dont skip the files that only have warnings    
    for x in logFiles:    
        with open('./' + directory + '/' + x +'.log','r') as f:
            k = f.readlines()
            if 'ERROR' in ','.join(k):
                errorFiles.append(x)
    bnglFiles = [x for x in onlyfiles if 'bngl' in x and 'log' not in x]
    validFiles = [x for x in bnglFiles if x not in errorFiles]
    print 'Thrown out: {0}'.format(len(bnglFiles)-len(validFiles))
    skip = [] #['334','225','332','105','293','333','337','18','409']
    counter = 0
    with open('executionTestErrors' + '.log', 'w') as f:
        subprocess.call(['rm','./*net'])
        for idx,bnglFile in enumerate(sorted(validFiles)):
            #if '100.' not in bnglFile:
            #    continue
            print bnglFile,
            timeout = 30
            if len([x for x in skip if x in bnglFile]) > 0: 
                continue
            with open('temp.tmp', "w") as outfile:
                d = open('dummy.tmp','w')
                start = datetime.datetime.now()
                result = subprocess.Popen(['bngdev', './' + directory+ '/{0}'.format(bnglFile)],stderr=outfile,stdout=d)
                while result.poll() is None:
                    time.sleep(0.1)
                    now = datetime.datetime.now()
                    if (now - start).seconds > timeout:
                        os.kill(result.pid, signal.SIGKILL)
                        os.waitpid(-1, os.WNOHANG)
                        subprocess.call(['killall','run_network'])
                        print 'breaker',
                        counter -=1
                        break
                
                d.close()
            if  result.poll() > 0:
                
                with open('temp.tmp','r') as outfile:
                    lines = outfile.readlines()
                tag = ''
                if 'cvode' in ','.join(lines):
                    print '///',bnglFile
                    tag = 'cvode'
                elif 'ABORT: Reaction rule list could not be read because of errors' in ','.join(lines):
                    print '\\\\\\',bnglFile
                #elif 'Incorrect number of arguments' in ','.join(lines):
                #    print '[[]]',bnglFile
                else:
                    print '---',bnglFile
                    tag = lines
                    f.write('%s %s\n' % (bnglFile,tag))
                f.flush()
            else:
                counter += 1
                print '+++',bnglFile
    print counter
        

if __name__ == "__main__":
    main()