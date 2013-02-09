# -*- coding: utf-8 -*-
"""
Created on Sun Mar 25 21:26:49 2012

@author: proto
"""

import json

def defaultReactionDefinition():
    listOfReactions= {'1': [['S0', 'S1'], ['S2']], '2': [['S2'], ['S0', 'S1']]}
    listOfDefinitions = {'Binding': [1,2]}
    final = {'reactions':listOfReactions,'definitions':listOfDefinitions}
    with open('reactionDefinition.json','w') as fp:
        json.dump(final,fp)
 
def logMess(logType,logMessage):
    if not hasattr(logMess, "log"):
        logMess.log = []
        logMess.counter = -1
    logMess.counter += 1
    logMess.log.append("%s.%d: %s"%(logType,logMess.counter,logMessage))
    return logMess.counter
    
if __name__ == "__main__":
    defaultReactionDefinition()