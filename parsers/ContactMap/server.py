# -*- coding: utf-8 -*-
"""
Created on Fri May 31 16:56:13 2013

@author: proto
"""

import os
from SimpleXMLRPCServer import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCRequestHandler
import threading
import time
import datetime
import subprocess
import createGraph
import pexpect
import xmlrpclib
# Restrict to a particular path.
class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

# Create server
server = SimpleXMLRPCServer(("10.253.98.102", 9100),              requestHandler=RequestHandler)
#server = SimpleXMLRPCServer(("127.0.0.1", 9100), requestHandler=RequestHandler)
server.register_introspection_functions()



iid = 1
iid_lock = threading.Lock()

def next_id():
    global iid
    with iid_lock:
        result = iid
        iid += 1
    return result   


class BipartiteServer:
    
    def __init__(self):
        pass
    def bipartite(self, bbnglFile,returnType):
        counter = next_id()
        bnglFile = bbnglFile.data
        with open('temp{0}.bngl'.format(counter),'w') as f:
            f.write(bnglFile)
        xmlFile = self.bngl2xml('temp{0}.bngl'.format(counter))
        createGraph.processBNGL('temp{0}.xml'.format(counter))
        with open('temp{0}.xml.dot'.format(counter),'rb') as f:
            dot = f.read()
        with open('temp{0}.xml.png'.format(counter),'rb') as f:
            png = f.read()
        subprocess.call(['rm','temp{0}*'.format(counter)])
        if returnType == 'dot':
            data = xmlrpclib.Binary(dot)
        else:
            data = xmlrpclib.Binary(png)
        return data

    def getTransformations(self,bbnglFile):
        
    def bngl2xml(self,bnglFile):

        bngconsole = pexpect.spawn('bngdev --console')
        bngconsole.expect('BNG>')
        bngconsole.sendline('load {0}'.format(bnglFile))
        bngconsole.expect('BNG>')
        bngconsole.sendline('action writeXML()')
        bngconsole.expect('BNG>')
        bngconsole.close()
        
        
server.register_instance(BipartiteServer())

# Run the server's main loop
server.serve_forever()
