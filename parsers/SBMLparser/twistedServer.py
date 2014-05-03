# -*- coding: utf-8 -*-
"""
Created on Fri May 2 16:56:13 2014

@author: proto
"""


import libsbml2bngl
# Restrict to a particular path.

from twisted.web import xmlrpc, server
from twisted.internet import threads,reactor
import threading
iid = 1
iid_lock = threading.Lock()

def next_id():
    global iid
    with iid_lock:
        result = iid
        iid += 1
    return result   

processDict = {}

class AtomizerServer(xmlrpc.XMLRPC):
    
    
    
    def xmlrpc_atomize(self, bxmlFile,atomize=False,reaction='config/reactionDefinitions.json',species=None):
        counter = next_id()
        xmlFile = bxmlFile.data
        species = 'config/' + species
        reaction = 'config/reactionDefinitions.json'
        
        result = threads.deferToThread(libsbml2bngl.readFromString,xmlFile,
                                             reaction,True,None,atomize)
        #result = libsbml2bngl.readFromString(xmlFile,
        #                                     reaction,True,None,atomize)
        
        return result
    def cleanDict(self,ticketNumber):
        pass
        

if __name__ == '__main__':
    r = AtomizerServer()
    reactor.listenTCP(9000, server.Site(r))
    reactor.run()        
