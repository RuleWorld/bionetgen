# -*- coding: utf-8 -*-
"""
Created on Fri May 2 16:56:13 2014

@author: proto
"""


import libsbml2bngl
# Restrict to a particular path.

from twisted.web import xmlrpc, server
from twisted.internet import threads,defer,reactor
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
    
    
    def addToDict(self,ticket,result):
        if len(processDict) > 40:
            s = min(processDict.keys())
            processDict.pop(s)
        processDict[ticket] = result
        
    def atomize(self,ticket,xmlFile,atomize):
        reaction = 'config/reactionDefinitions.json'
        try:
            result = libsbml2bngl.readFromString(xmlFile,
                                                 reaction,False,None,atomize)
            self.addToDict(ticket,result)
        except:
            self.addToDict(ticket,-5)
    def xmlrpc_atomize(self, bxmlFile,atomize=False,reaction='config/reactionDefinitions.json',species=None):
        counter = next_id()
        xmlFile = bxmlFile.data
        reactor.callInThread(self.atomize,counter,xmlFile,atomize)
        processDict[counter] = -2
        #result = threads.deferToThread(libsbml2bngl.readFromString,xmlFile,
        #                                     reaction,True,None,atomize)
        #result = libsbml2bngl.readFromString(xmlFile,
        #                                     reaction,True,None,atomize)
        
        return counter
    def xmlrpc_getDict(self,ticketNumber):
        if ticketNumber in processDict:
            return processDict.pop(ticketNumber)
        else:
            return -1
    
    def xmlrpc_isready(self,ticketNumber):
        if ticketNumber in processDict:
            if processDict[ticketNumber] == -2:
                return -2
            return 1
        else:
            return -1
        

if __name__ == '__main__':
    r = AtomizerServer()
    reactor.listenTCP(9000, server.Site(r))
    reactor.run() 
