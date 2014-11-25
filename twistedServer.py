# -*- coding: utf-8 -*-
"""
Created on Fri May 2 16:56:13 2014

@author: proto
"""


import libsbml2bngl
import annotationExtender
# Restrict to a particular path.
from twisted.web import xmlrpc, server
from twisted.internet import reactor
import threading
import sys
sys.path.insert(0, '../utils/')
import consoleCommands
import tempfile
sys.path.insert(0, '../gml2sbgn/')
import libsbgn
import networkx
iid = 1
iid_lock = threading.Lock()
from subprocess import call 

bngDistro  = '/home/ubuntu/bionetgen/bng2/BNG2.pl'
#bngDistro = '/home/proto/workspace/bionetgen/bng2/BNG2.pl'

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
        print ticket
        try:
            result = libsbml2bngl.readFromString(xmlFile,
                                                 reaction,False,None,atomize)
            self.addToDict(ticket,result)
            print 'sucess',result
        except:
            self.addToDict(ticket,-5)
            print 'failure'

    def generateAnnotation(self,ticket,xmlFile):
        try:
            print ticket
            pointer = tempfile.mkstemp(suffix='.xml',text=True)
            with open(pointer[1],'w' ) as f:
                f.write(xmlFile)
            
            call(['python','annotationExtender.py',
            '-i',pointer[1],
            '-o',pointer[1]+'.xml'])
            with open(pointer[1]+'.xml','r') as f:
                result = f.read()
            #result = annotationExtender.expandAnnotation(pointer[1])
            self.addToDict(ticket,result)
            print 'sucess',result
        except:
            self.addToDict(ticket,-5)
            print 'failure'


    def generateGraph(self,ticket,bnglContents,graphtype):
        print ticket
        pointer = tempfile.mkstemp(suffix='.bngl',text=True)
        with open(pointer[1],'w' ) as f:
            f.write(bnglContents)
        #try:
        if graphtype in ['regulatory','contactmap']:
            consoleCommands.setBngExecutable(bngDistro)
            consoleCommands.generateGraph(pointer[1],graphtype)
            name = pointer[1].split('.')[0]
            with open('{0}_{1}.gml'.format(name,graphtype),'r') as f:
                graphContent = f.read()         
                self.addToDict(ticket,graphContent)
                print 'success',ticket
        elif graphtype in ['sbgn_er']:
            consoleCommands.setBngExecutable(bngDistro)
            consoleCommands.generateGraph(pointer[1],'contactmap')
            name = pointer[1].split('.')[0]
            #with open('{0}_{1}.gml'.format(name,'contactmap'),'r') as f:
            #   graphContent = f.read()
            graphContent = networkx.read_gml('{0}_{1}.gml'.format(name,'contactmap'))
            sbgn = libsbgn.createSBNG_ER_gml(graphContent)
            self.addToDict(ticket,sbgn)
            print 'success',ticket

        #except:
        #  self.addToDict(ticket,-5)
        #    print 'failure',ticket


    def xmlrpc_generateGraph(self,bbnglFile,graphtype):
        counter = next_id()
        bnglFile = bbnglFile.data
        reactor.callInThread(self.generateGraph,counter,bnglFile,graphtype)
        processDict[counter] = -2
        return counter
        #self.generateGraph(counter,bnglFile,graphtype)
        #return counter

    def xmlrpc_generateAnnotations(self,bxmlFile):
        counter = next_id()
        xmlFile = bxmlFile.data
        reactor.callInThread(self.generateAnnotation,counter,xmlFile)
        #reactor.callInThread(self.atomize,counter,xmlFile,False)

        processDict[counter] = -2
        return counter

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
    #f = open('XMLExamples/curated/BIOMD0000000019.xml')
    #s = f.read()
    #r.generateAnnotation(1,s)
