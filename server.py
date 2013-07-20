# -*- coding: utf-8 -*-
"""
Created on Fri May 31 16:56:13 2013

@author: proto
"""

import datetime
import os
import time
import subprocess
from SimpleXMLRPCServer import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCRequestHandler
import threading
import signal

import libsbml2bngl
# Restrict to a particular path.
class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

# Create server
server = SimpleXMLRPCServer(("10.253.98.102", 9000),
                            requestHandler=RequestHandler)
#server = SimpleXMLRPCServer(("128.237.114.30", 9000),
#                            requestHandler=RequestHandler)
server.register_introspection_functions()



iid = 1
iid_lock = threading.Lock()

def next_id():
    global iid
    with iid_lock:
        result = iid
        iid += 1
    return result


class AtomizerServer:
    
    def __init__(self):
        pass
    def atomize(self, bxmlFile,atomize=False):
        counter = next_id()
        xmlFile = bxmlFile.data
        result = libsbml2bngl.readFromString(xmlFile,
                                             'reactionDefinitions/reactionDefinition7.json',True,None,atomize)

        return result
    def getSpeciesConventions(self):
        
        return ''


        
server.register_instance(AtomizerServer())

# Run the server's main loop
server.serve_forever()
