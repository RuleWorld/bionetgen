# -*- coding: utf-8 -*-
"""
Created on Thu Nov  7 19:22:45 2013

@author: proto
"""

import os
from SimpleXMLRPCServer import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCRequestHandler
import threading


def importBNGL(fileName):
    fields = {}
    with open(fileName,'r') as f:
        content = f.read()
    content = content.split('\n')
    fields = {}
    try:
        start = content.index('###')
        end = content[start+1:].index('###')
    except ValueError:
        return []
    comments = content[start+1:end+1]
    for comment in comments:
        try:
            tmp = comment.split(':')
            header = tmp[0].split('@')[1]
            tmp[1] = ':'.join(tmp[1:])
            message = tmp[1].encode('ascii', 'ignore')
            fields[header] = message
        except:
            continue
    return fields

        
if __name__ == "__main__":      
    print importBNGL('output19.bngl')