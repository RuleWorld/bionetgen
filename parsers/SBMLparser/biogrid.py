# -*- coding: utf-8 -*-
"""
Created on Tue Nov 12 13:44:53 2013

@author: proto
"""

import csv
import pickle

import pandas as pd

def loadBioGrid(fileName='BIOGRID-ALL-3.2.106.tab2.txt'):
    if hasattr(loadBioGrid, 'db'):
        return loadBioGrid.db
    loadBioGrid.db = {}
    def check(db,term):
        if term not in db:
            db[term] = set([])
            
    f = open(fileName,'rb')
    content =  csv.DictReader(f,delimiter='\t')
    for idx,line in enumerate(content):
        tmp = {}
        tmp['osiB'] = line['Official Symbol Interactor B']
        tmp['osiA'] = line['Official Symbol Interactor A']
        #tmp['siA'] = tmp['Synonyms Interactor A']
        #tmp['siB'] = tmp['Synonyms Interactor B']
        check(loadBioGrid.db,tmp['osiB'].upper())
        check(loadBioGrid.db,tmp['osiA'].upper())
        loadBioGrid.db[tmp['osiB'].upper()].add(tmp['osiA'].upper())
        loadBioGrid.db[tmp['osiA'].upper()].add(tmp['osiB'].upper())
    p = pd.Series(loadBioGrid.db)
    p.to_pickle('BioGridPandas.dump')
    
    
def loadBioGridDict(fileName='BioGridPandas.dump'):
    if hasattr(loadBioGrid, 'db'):
        return loadBioGrid.db
    loadBioGrid.db = pd.read_pickle('BioGridPandas.dump')
    return loadBioGrid.db
    
if __name__ == "__main__":
    #extractStatistics()
    #db = loadBioGrid()
    #print len(db)
    #f = open('bioGridDict.dump','wb')
    #pickle.dump(db,f)
    #pass
    #db2 = loadBioGridDict()
    #print len(db2)
    #f = open('bioGridDict.dump','wb')
    #print len(db)
    loadBioGrid()
    #print len(db2)  