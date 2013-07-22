# -*- coding: utf-8 -*-
"""
Created on Fri May 31 16:56:13 2013

@author: proto
"""

import os
from SimpleXMLRPCServer import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCRequestHandler
import threading

from os import listdir
from os.path import isfile, join

import libsbml2bngl
# Restrict to a particular path.
class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

# Create server
server = SimpleXMLRPCServer(("10.253.98.102", 9000),              requestHandler=RequestHandler)
#server = SimpleXMLRPCServer(("127.0.0.1", 9000), requestHandler=RequestHandler)
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
    def atomize(self, bxmlFile,atomize=False,reaction='reactionDefinitions/reactionDefinition7.json',species=None):
        counter = next_id()
        xmlFile = bxmlFile.data
        reaction = 'reactionDefinitions/' + reaction
        species = 'reactionDefinitions/' + species
        result = libsbml2bngl.readFromString(xmlFile,
                                             reaction,True,None,atomize)

        return result
    def getSpeciesConventions(self):
        onlyfiles = [ f for f in listdir('./reactionDefinitions') if isfile(join('./reactionDefinitions',f)) ]
        reactionFiles = [x for x in onlyfiles if 'reaction' in x]
        speciesFiles = [x for x in onlyfiles if 'species' in x ]
        
        return reactionFiles,speciesFiles


        
server.register_instance(AtomizerServer())

# Run the server's main loop
server.serve_forever()
