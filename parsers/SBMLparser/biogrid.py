# -*- coding: utf-8 -*-
"""
Created on Tue Nov 12 13:44:53 2013

@author: proto
"""

import csv
import pickle
def loadBioGrid(fileName='BIOGRID-ALL-3.2.106.tab2.txt'):
    if hasattr(loadBioGrid, 'db'):
        return loadBioGrid.db
    loadBioGrid.db = {}
    def check(db,term):
        if term not in db:
            db[term] = []
            
    f = open(fileName,'rb')
    content =  csv.DictReader(f,delimiter='\t')
    for idx,line in enumerate(content):
        tmp = {}
        tmp['osiB'] = line['Official Symbol Interactor B']
        tmp['osiA'] = line['Official Symbol Interactor A']
        #tmp['siA'] = tmp['Synonyms Interactor A']
        #tmp['siB'] = tmp['Synonyms Interactor B']
        check(loadBioGrid.db,tmp['osiB'])
        check(loadBioGrid.db,tmp['osiA'])
        loadBioGrid.db[tmp['osiB']].append(tmp['osiA'])
        loadBioGrid.db[tmp['osiA']].append(tmp['osiB'])
        
    return loadBioGrid.db
    
def loadBioGridDict(fileName='bioGridDict.dump'):
    if hasattr(loadBioGrid, 'db'):
        return loadBioGrid.db
    loadBioGrid.db = {}

    f = open(fileName,'rb')
    loadBioGrid.db = pickle.load(f)
    return loadBioGrid.db
    
if __name__ == "__main__":
    #extractStatistics()
    #db = loadBioGrid()
    #print len(db)
    #f = open('bioGridDict.dump','wb')
    #pickle.dump(db,f)

    db2 = loadBioGridDict()
    print len(db2)
    
    #f = open('bioGridDict.dump','wb')
    #print len(db)
    #db2 = loadBioGrid()
    #print len(db2)  