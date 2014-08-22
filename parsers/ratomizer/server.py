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
import signal
# Restrict to a particular path.
class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

# Create server
server = SimpleXMLRPCServer(("128.237.203.160", 9000),
                            requestHandler=RequestHandler)
server.register_introspection_functions()


import threading

iid = 1
iid_lock = threading.Lock()

def next_id():
    global iid
    with iid_lock:
        result = iid
        iid += 1
    return result


class AtomizerServer:
    def atomize(self, xmlFile):
        timeout = 30
        counter = next_id()
        with open('./dummy{0}.bngl'.format(counter),'w') as f:
            f.write(xmlFile)
        with open('temp.tmp', "w") as outfile:
            d = open('dummy.tmp','w')
            start = datetime.datetime.now()
            result = subprocess.Popen(['bngdev', './dummy{0}.bngl'.format(counter)],stderr=outfile,stdout=d)
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
        print counter
        
        with open('dummy{0}.gdat'.format(counter),'r') as f:
            result=  f.read()
        #subprocess.Popen(['rm', './dummy{0}.*'.format(counter)],stderr=outfile,stdout=d)
        return result


        
server.register_instance(AtomizerServer())

# Run the server's main loop
server.serve_forever()
