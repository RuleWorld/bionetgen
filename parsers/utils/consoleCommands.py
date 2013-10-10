# -*- coding: utf-8 -*-
"""
Created on Mon Sep  2 18:11:35 2013

@author: proto
"""

import pexpect

def bngl2xml(bnglFile):

    bngconsole = pexpect.spawn('bngdev --console')
    bngconsole.expect('BNG>')
    bngconsole.sendline('load {0}'.format(bnglFile))
    bngconsole.expect('BNG>')
    bngconsole.sendline('action writeXML()')
    bngconsole.expect('BNG>')
    bngconsole.close() 
    
def correctness(bnglFile):
    bngconsole = pexpect.spawn('bngdev --console')
    bngconsole.expect('BNG>')
    bngconsole.sendline('load {0}'.format(bnglFile))
    bngconsole.expect('BNG>')
    output= bngconsole.before
    bngconsole.close()
    if 'ERROR' in output or 'WARNING' in output:
        return False
    return True

    
def writeNetwork(bnglFile):
    bngconsole = pexpect.spawn('bngdev --console')
    bngconsole.expect('BNG>')
    bngconsole.sendline('load {0}'.format(bnglFile))
    bngconsole.expect('BNG>')
    bngconsole.sendline('action generate_network()')
    bngconsole.expect('BNG>')
    bngconsole.close() 
    
if __name__ == "__main__":      
    print correctness('complex/output21.bngl')