# -*- coding: utf-8 -*-
"""
Created on Tue Nov 12 13:44:53 2013

@author: proto
"""

import csv
import pickle
import itertools
try:
    import pandas as pd
except ImportError:
    pd = None

def loadBioGrid(fileName='BIOGRID-ALL-3.2.108.tab2.txt'):
    if hasattr(loadBioGrid, 'db'):
        return loadBioGrid.db
    loadBioGrid.db = {}
    def check(db,term):
        if term not in db:
            db[term] = set([])
            
    f = open(fileName,'rb')
    content =  csv.DictReader(f,delimiter='\t')
    loadBioGrid.db['synonyms'] = {}
    
    for idx,line in enumerate(content):
        tmp = {}
        tmp['osiB'] = line['Official Symbol Interactor B']
        tmp['osiA'] = line['Official Symbol Interactor A']
        tmp['siA'] = line['Synonyms Interactor A'].split('|')
        tmp['siB'] = line['Synonyms Interactor B'].split('|')
        
        tmp['siA'].append(tmp['osiA'])
        tmp['siB'].append(tmp['osiB'])
        comb = ([x,y] for x in tmp['siA'] for y in tmp['siB'])

        '''
        for element in tmp['siA']:
            check(loadBioGrid.db['synonyms'],element.upper())
            loadBioGrid.db['synonyms'][element.upper()] = tmp['osiA'].upper()
        for element in tmp['siB']:
            check(loadBioGrid.db['synonyms'],element.upper())
            loadBioGrid.db['synonyms'][element.upper()] = tmp['osiB'].upper()
        if tmp['osiA'].upper() in loadBioGrid.db['synonyms']:
            tmp['osiA'] = loadBioGrid.db['synonyms'][tmp['osiA'].upper()]
        if tmp['osiB'].upper() in loadBioGrid.db['synonyms']:
            tmp['osiB'] = loadBioGrid.db['synonyms'][tmp['osiB'].upper()]
        '''
        
        for element in comb:
            check(loadBioGrid.db,element[1].upper())
            loadBioGrid.db[element[1].upper()].add(element[0].upper())

        #check(loadBioGrid.db,tmp['osiA'].upper())
        #loadBioGrid.db[tmp['osiA'].upper()].add(tmp['osiB'].upper())
    p = pd.Series(loadBioGrid.db)
    p.to_hdf('BioGridPandas.h5','biogrid')
    
    
def loadBioGridDict(fileName='BioGridPandas.h5'):
    if hasattr(loadBioGrid, 'db'):
        return loadBioGrid.db
    if pd is None:
        print 'pandas library not found.'
        loadBioGrid.db = {}
    else:
        try:
            loadBioGrid.db = pd.read_hdf('BioGridPandas.h5','biogrid')
        except:
            print 'Biogrid database not found'
            loadBioGrid.db = {}
    
    return loadBioGrid.db
    
if __name__ == "__main__":
        #extractStatistics()
    db = loadBioGrid()
    #print len(db)
    #f = open('bioGridDict.dump','wb')
    #pickle.dump(db,f)
    #pass
    #db2 = loadBioGridDict()
    #print len(db2)
    #f = open('bioGridDict.dump','wb')
    #print len(db)
    #loadBioGrid()
    #print len(db2)  
