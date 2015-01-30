# -*- coding: utf-8 -*-
"""
Created on Mon Sep  2 18:11:35 2013

@author: proto
"""

import pexpect
import subprocess
import os

bngExecutable = 'bngdev'

def setBngExecutable(executable):
    bngExecutable = executable

def getBngExecutable():
    return bngExecutable

def bngl2xml(bnglFile,timeout=60):
    try:
        
        bngconsole = pexpect.spawn('{0} --console'.format(getBngExecutable()),timeout=timeout)
        bngconsole.expect('BNG>')
        bngconsole.sendline('load {0}'.format(bnglFile))
        bngconsole.expect('BNG>')
        bngconsole.sendline('action writeXML()')
        bngconsole.expect('BNG>')
        bngconsole.close() 
    except pexpect.TIMEOUT:
        subprocess.call(['killall','bngdev'])        
    
def correctness(bnglFile):
    bngconsole = pexpect.spawn('{0} --console'.format(getBngExecutable()))
    bngconsole.expect('BNG>')
    bngconsole.sendline('load {0}'.format(bnglFile))
    bngconsole.expect('BNG>')
    output= bngconsole.before
    bngconsole.close()
    if 'ERROR' in output  in output:
        return False
    return True

    
def writeNetwork(bnglFile):
    bngconsole = pexpect.spawn('{0} --console'.format(getBngExecutable()))
    bngconsole.expect('BNG>')
    bngconsole.sendline('load {0}'.format(bnglFile))
    bngconsole.expect('BNG>')
    bngconsole.sendline('action generate_network()')
    bngconsole.expect('BNG>')
    bngconsole.close() 


def generateGraph(bnglFile,graphType):
    directory = os.sep.join(bnglFile.split(os.sep)[:-1])
    os.chdir(directory)
    print directory
    bngconsole = pexpect.spawn('{0} --console'.format(getBngExecutable()))
    bngconsole.expect('BNG>')
    bngconsole.sendline('load {0}'.format(bnglFile))
    bngconsole.expect('BNG>')
    if graphType == "regulatory":
        bngconsole.sendline('action visualize()')
    elif graphType == "contactmap":
        bngconsole.sendline('action visualize({type=>"contactmap"})')        
    else:
        return False
    bngconsole.expect('BNG>')
    bngconsole.close() 
    return True
    
    
if __name__ == "__main__":      
    print bngl2xml('output19.bngl')