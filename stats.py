# -*- coding: utf-8 -*-
"""
Created on Mon Oct  8 14:16:25 2012

@author: proto

"""
import numpy as np
import libsbml2bngl
import libsbml
import analyzeRDF
import MySQLdb
from SOAPpy import WSDL,Types
import pickle
import re
#from restful_lib import Connection
import urllib,urllib2
#from wordcloud import cloudText
import operator
from collections import Counter

def main():
    history = np.load('stats3b.npy')
    history2 = np.load('stats3.npy')
    dictionary = {}
    
    for index in range (0,len(history)):
        flag = False
        if history[index] == []:
            continue
        else:
            for index2 in range(0,len(history2)):
                element = history2[index2]
                
                if element[0] == index+1:
                    if element[2] == 0.0:
                        flag = True
                        break
                    else:
                        break
                elif element[0] > index+1:
                    continue
            if flag:
                continue
            for molecule in history[index]:
                if molecule == []:
                    continue
                
                for uniprot in molecule[1][1]:
                    
                    if uniprot not in dictionary:
                        dictionary[uniprot] = []
                    if index+1 not in [x[0] for x in dictionary[uniprot]]:
                        dictionary[uniprot].append([index+1,molecule[0]])
    
    np.save('statsFinal.npy',np.array(dictionary))
    for element in dictionary:
        if len(dictionary[element]) > 1:
            print element,dictionary[element]
            
def bagOfWords():
    with open('sortedC.dump','rb') as f:
        sortedFiles = pickle.load(f)
        
    with open('parsedAnnotations.dump','rb') as f:
        annotations = pickle.load(f)
    
    with open('classificationDict.dump','rb') as f:
        classifications = pickle.load(f)
        
    bins = np.digitize(sortedFiles,np.arange(0,1.1,0.1))
    annotationDict = {x:{} for x in range(1,12)}
    classDict = {x:{} for x in range(1,12)}
    
    problem = {}
    problem['empty'] = 0
    problem['none'] = 0
    problem['pheno'] = 0
    for idx,element in enumerate(classifications):
        if bins[idx] == 1:
            if element == {}:
                problem['empty'] += 1
            else:
                if 'None' in element:
                    problem['none'] += element['None']
                if 'Generation' in element:
                    problem['pheno'] += element['Generation']  
                if 'Decay' in element:
                    problem['pheno'] += element['Decay']
    
    print problem
    for idx,element in enumerate(annotations):
        if idx < len(bins):
            if len(element) > 0:
                for ann in element:
                    for word in ann[0].split(' '):
                        if word not in annotationDict[bins[idx]]:
                            annotationDict[bins[idx]][word] = 0
                        annotationDict[bins[idx]][word] += 1.0
    for element in annotationDict:
        annotationDict[element] = {x:annotationDict[element][x] for x in annotationDict[element] if annotationDict[element][x] > 3 and len(x) > 2}
    print annotationDict
    
    for element in annotationDict:
        stringA = ''
        for word in annotationDict[element]:
            tword = word.replace('/','')
            tword = word.replace('-','')
            stringA += ' ' + ' '.join([tword for x in range(0,int(annotationDict[element][word]))])
        cloudText(stringA,'cloud{0}.png'.format(element))
    
    stringA = ''
    for element in [9,10,11]:
        for word in annotationDict[element]:
            tword = word.replace('/','')
            tword = word.replace('-','')
            stringA += ' ' + ' '.join([tword for x in range(0,int(annotationDict[element][word]))])
    cloudText(stringA,'cloudMax.png')
        
          
def main2():
    #go database
    dbs = MySQLdb.connect(host='mysql.ebi.ac.uk',user='go_select',passwd='amigo',db='go_latest',port=4085)
    cur = dbs.cursor()
    #reactome
    rwsdl = "http://www.reactome.org:8080/caBIOWebApp/services/caBIOService?wsdl"
    rserv = WSDL.Proxy(rwsdl)
    
    #kegg
    kegg_url = "http://rest.kegg.jp"
    #conn = Connection(kegg_url)
    
    #uniprot taxonomy
    url = 'http://www.uniprot.org/taxonomy/'
    
    
    print '---'    
    annotationArray = []
    with open('annotations.dump','rb') as f:
        ar = pickle.load(f)
    for idx,element in enumerate(ar):
        print idx
        modelAnnotations = Counter()
        for index  in element:
            for annotation in element[index]:
                try:
                    bioArray = []
                    tAnnotation = annotation.replace('%3A',':')
                    tAnnotation = re.search(':([^:]+:[^:]+$)',tAnnotation).group(1)
                    
                    if 'GO' in annotation:            
                        cur.execute("SELECT * FROM term WHERE acc='{0}'".format(tAnnotation))
                        for row in cur.fetchall():
                            bioArray.append([row[1],row[3]])
                            modelAnnotations.update([row[1]])
                    elif 'reactome' in annotation:
                        tAnnotation2 = re.search('_([^_]+$)',tAnnotation).group(1)
                        try:
                            query = rserv.queryById(Types.longType(long(tAnnotation)))
                        except:
                            continue
                        bioArray.append([query['name'],tAnnotation])
                        modelAnnotations.update([query['name']])
                        '''
                    elif 'kegg' in annotation:
                        if 'pathway' in tAnnotation:
                            tAnnotation2 = 'map' + re.search('[^0-9]+([0-9]+$)',tAnnotation).group(1)
                            reply = conn.request_get('find/pathway/{0}'.format(tAnnotation2), headers={'Accept':'text/json'})
                            if reply['body'] != '\n':
                                bioArray.append([reply['body'].split('\t')[1].strip(),tAnnotation])
                                modelAnnotations.update([reply['body'].split('\t')[1].strip()])
                        else:
                            print annotation
                        '''
                    elif 'uniprot' in annotation:
                        identifier = annotation.split(':')[-1]
                        url = 'http://www.uniprot.org/uniprot/{0}.tab'.format(identifier)
                        params = {}
                        data = urllib.urlencode(params)
                        request = urllib2.Request(url, data)
                        request.add_header('User-Agent', 'Python contact')
                        response = urllib2.urlopen(request)
                        page = response.read(200000)
                        proteinName = page.split('\n')[1].split('\t')[3]
                        modelAnnotations.update([proteinName])
                    elif 'interpro' in annotation:
                        identifier = annotation.split(':')[-1]
                        url = 'http://www.ebi.ac.uk/interpro/entry/{0}'.format(identifier)
                        params = {}
                        data = urllib.urlencode(params)
                        request = urllib2.Request(url, data)
                        request.add_header('User-Agent', 'Python contact')
                        response = urllib2.urlopen(request)
                        page = response.read(200000)
                        pointer = page.index('h2 class="strapline"')
                        extract = page[pointer:pointer+100]
                        extract = extract[extract.index('>')+1:extract.index('<')]
                        modelAnnotations.update([extract])
    
                        
                    #elif 'taxonomy' in annotation:
                        #uniprot stuff for taxonomy
                    #    pass
                        '''
                        url = 'http://www.uniprot.org/taxonomy/'
                        params = {
                        'from':'ACC',
                        'to':'P_REFSEQ_AC',
                        'format':'tab',
                        'query':'P13368 P20806 Q9UM73 P97793 Q17192'
                        }
                        
                        data = urllib.urlencode(params)
                        request = urllib2.Request(url, data)
                        contact = "" # Please set your email address here to help us debug in case of problems.
                        request.add_header('User-Agent', 'Python contact')
                        response = urllib2.urlopen(request)
                        page = response.read(200000)
                        '''
                    else:
                        print '--',annotation,'GO' in tAnnotation
                except:
                    continue
        print modelAnnotations
        annotationArray.append(modelAnnotations)
    with open('parsedAnnotations.dump','wb') as f:
        pickle.dump(annotationArray,f)


def extractProteinCompendiums():
    '''
    Extract a dictionary where keys: proteins that map
    to an array of models that contain them
    '''
    annotationFile = open('annotations.dump','rb')
    annotations = pickle.load(annotationFile)
    
    annotationDictionary = {}
    for idx,fileAnnotation in enumerate(annotations):
        for annotation in fileAnnotation:
            for miriamNumber in fileAnnotation[annotation]:
                if miriamNumber not in annotationDictionary:
                    annotationDictionary[miriamNumber] = []
                annotationDictionary[miriamNumber].append(idx)
    print annotationDictionary
                    

    
def rankingAnalysis():
    annotationFile = open('parsedAnnotations.dump','rb')
    evaluationFile = open('sortedC.dump','rb')
    
    annotations = pickle.load(annotationFile)
    pickle.load(evaluationFile)
    ev1 =     pickle.load(evaluationFile)
    totalAnnotations = {}
    totalAppereances = {}
    weightedTotalAppereances = {}
    for ann,evaluation in zip(annotations,ev1):
        total = sum(ann[x] for x in ann)
        for x in ann:
            if x not in totalAnnotations:
                totalAnnotations[x] = 0
                totalAppereances[x] = 0
                weightedTotalAppereances[x] = 0
            totalAnnotations[x] += ann[x]*evaluation/total
            totalAppereances[x] += ann[x]
            weightedTotalAppereances[x] += ann[x]*1.0/total
            
    sortedAnnotations = sorted(totalAnnotations.iteritems(), key=operator.itemgetter(1),reverse=True)
    sortedAppereances = sorted(totalAppereances.iteritems(), key=operator.itemgetter(1),reverse=True)
    sortedWeightedAppereances = sorted(weightedTotalAppereances.iteritems(), key=operator.itemgetter(1),reverse=True)
    print '--- top proteins weighted by the number of times they appear and how atomizable their model is'
    print sortedAnnotations[0:10]
    print '--- top protein by number of appereances in a model'
    print sortedAppereances[0:10]
    print '--- top proteins by number of weighted appeareances in a model'
    print sortedWeightedAppereances[0:10]
    #print sortedAnnotations
    
def inverseAnnotationClassification():
    annotationFile = open('annotations.dump','rb')
    annotations = pickle.load(annotationFile)    
    
if __name__ == "__main__":
    #bagOfWords()
    #main2()
    extractFamilies()
    #rankingAnalysis()