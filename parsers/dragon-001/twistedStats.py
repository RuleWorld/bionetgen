# -*- coding: utf-8 -*-
"""
Created on Mon Oct  8 14:16:25 2012

@author: proto

"""

import networkx as nx
import json

import bioservices
import pyparsing as pyp

from twisted.web import xmlrpc, server
from twisted.internet import threads,reactor


import concurrent.futures
import urllib2

import tempfile
import subprocess
from os import getcwd
# Restrict to a particular path.

# Create server
port = 9200

#server = SimpleXMLRPCServer(("127.0.0.1", port), requestHandler=RequestHandler)


#def updateDictionary(tmpArray,key,newInfo):

def resolveAnnotations(annotations):
    tmpArray = {}
    futures = []
    with concurrent.futures.ProcessPoolExecutor(max_workers=8) as executor:
        for annotation in annotations:
            futures.append(executor.submit(resolveAnnotation, annotation))
        for future in concurrent.futures.as_completed(futures):
            resolvedAnnotation = future.result()
    
            for element in resolvedAnnotation:
                if resolvedAnnotation[element] != annotation:
                    if element not in tmpArray:
                        tmpArray[element] = []
                    if isinstance(resolvedAnnotation[element],list):
                        tmpArray[element].extend(resolvedAnnotation[element])
                    else:
                        tmpArray[element].append(resolvedAnnotation[element])
    finalArray = []
    #transforming to an [key [result],....] structure since the ouput cant be dictionaries
    for element in tmpArray:
        finalArray.append([element,tmpArray[element]])
    print finalArray
    return finalArray

def removeTags(taggedInformation):
        taggedInformation2 = taggedInformation.decode('ascii','ignore')
        goGrammar = pyp.Suppress('<' + pyp.Word(pyp.alphanums) + '>') +  pyp.Word(pyp.alphanums + pyp.alphas8bit + ' ._-') + pyp.Suppress('</' + pyp.Word(pyp.alphanums) + '>')
        tmp = goGrammar.parseString(str(taggedInformation2))
        return tmp[0]
    
def resolveAnnotation(annotation):
    if not hasattr(resolveAnnotation, 'db'):
        resolveAnnotation.db = {}
        resolveAnnotation.ch = bioservices.ChEBI(verbose=False)
        resolveAnnotation.bio = bioservices.BioModels(verbose=False)
        resolveAnnotation.uni = bioservices.UniProt(verbose=False)
        resolveAnnotation.k = bioservices.kegg.KeggParser(verbose=False)
        resolveAnnotation.s = bioservices.Service('name')
        resolveAnnotation.qg = bioservices.QuickGO(verbose=False)
        resolveAnnotation.db['http://identifiers.org/uniprot/P62988'] = 'http://identifiers.org/uniprot/P62988'
        resolveAnnotation.db['http://identifiers.org/uniprot/P06842'] = 'http://identifiers.org/uniprot/P06842'
        resolveAnnotation.db['http://identifiers.org/uniprot/P07006'] = 'http://identifiers.org/uniprot/P06842'
        
    if annotation in resolveAnnotation.db:
        return resolveAnnotation.db[annotation]
    
        
    tAnnotation = annotation.replace('%3A',':')
    tAnnotation = annotation.split('/')[-1]
    #tAnnotation = re.search(':([^:]+:[^:]+$)',tAnnotation).group(1)
    tmpArray = {}
    try:
        if 'obo.go' in annotation:
            res = resolveAnnotation.qg.Term(tAnnotation)
            tmp = res.findAll('name')
            finalArray = []
            goGrammar = pyp.Suppress(pyp.Literal('<name>')) +  pyp.Word(pyp.alphanums + ' -_') + pyp.Suppress(pyp.Literal('</name>')) 
            for x in tmp:
                finalArray.append(str(goGrammar.parseString(str(x))[0]))
            tmp = finalArray
            resolveAnnotation.db[annotation] = tmp
            tmpArray['tags'] =  tmp

        elif 'kegg' in annotation:
            
            data = resolveAnnotation.k.get(tAnnotation)
            dict_data =  resolveAnnotation.k.parse(data)
            resolveAnnotation.db[annotation] = dict_data['name']
            tmpArray['tags'] =  resolveAnnotation.db[annotation]
            
        elif 'uniprot' in annotation:
            identifier = annotation.split('/')[-1]
            result = resolveAnnotation.uni.quick_search(identifier)
            resolveAnnotation.db[annotation] = result[result.keys()[0]]['Protein names']
            tmpArray['tags'] = resolveAnnotation.db[annotation]
            
        elif 'chebi' in annotation:
            tmp = annotation.split('/')[-1]
            
            entry = resolveAnnotation.ch.getLiteEntity(tmp)
            for element in entry:
                resolveAnnotation.db[annotation] = element['chebiAsciiName']
                tmpArray['tags'] =  resolveAnnotation.db[annotation]
        elif 'cco' in annotation or 'pirsf' in annotation or 'pubchem' in annotation or 'omim' in annotation:
            tmpArray['ignore'] =  annotation
        elif 'biomodels.db' in annotation:
            tmp = annotation.split('/')[-1]
            tmpArray = {}
            try:
                request = resolveAnnotation.bio.getSimpleModelsByIds(tmp)
            except ValueError:
                tmpArray['ignore'] = annotation
                return tmpArray
            entry = resolveAnnotation.s.easyXML(request)
            tmpArray['modelName'] = []
            for tag in entry['modelname']:
                tmpArray['modelName'].append(removeTags(str(tag)))
            tmpArray['author'] = []
            for tag in entry['author']:
                tmpArray['author'].append(removeTags(str(tag)))
            
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
            print 'ERRROERROROERRRO',annotation
            #assert(False)
            tmpArray['ignore'] =  annotation
    except (SyntaxError,TypeError,urllib2.HTTPError):
        tmpArray['ignore'] = annotation
        #finally:
    return tmpArray



def generateContactMap(bnglFile,graphType):
    pointer = tempfile.mkstemp(suffix='.bngl',text=True)
    bngDistro  = '../../bng2/Perl2/'
    name = pointer[1].split('.')[0]
    with open(pointer[1],'w' ) as f:
        f.write(bnglFile)
    subprocess.call(['perl',bngDistro +'visualize.pl',pointer[1],
            graphType,graphType],cwd=bngDistro)
    fileName =    '{1}_{0}.gml'.format(graphType,name)
    return fileName



def gml2cyjson(gmlText):
    jsonDict = {}
    jsonDict['style'] =  [
    {
      'selector': 'node',
      'css': {
        'content': 'data(label)',
        'text-valign': 'center',
        'text-halign': 'center'
      }
    },
    {
      'selector': '$node > node',
      'css': {
        'padding-top': '20px',
        'padding-left': '10px',
        'padding-bottom': '10px',
        'padding-right': '10px',
        'text-valign': 'top',
        'text-halign': 'center'
      }
    },
    {
      'selector': 'edge',
      'css': {
        'target-arrow-shape': 'triangle'
      }
    },
    {
      'selector': ':selected',
      'css': {
        'background-color': 'black',
        'line-color': 'black',
        'target-arrow-color': 'black',
        'source-arrow-color': 'black'
      }
    }
  ]
    jsonDict['elements'] = {'nodes':[]}
    jsonDict['elements']['edges'] = []
    #for nd in gmlText.node:
    #    gmlText.node[nd].pop('id')
    #jsgrph = json_graph.node_link_data(gmlText)
    for node in gmlText.node:
        tmp = {'data':{}}
        tmp['data']['id'] = str(node)
        tmp['data']['label'] = str(gmlText.node[node]['label'])
        if 'gid' in gmlText.node[node]:
            tmp['data']['parent'] =  str(gmlText.node[node]['gid'])
        jsonDict['elements']['nodes'].append(tmp)
    for link in gmlText.edge:
        for dlink in gmlText.edge[link]:
            tmp = {'data':{}}
            tmp['data']['source'] = int(link)
            tmp['data']['target'] = int(dlink)
            tmp['data']['id'] = '{0}_{1}'.format(tmp['data']['source'],tmp['data']['target'])
            jsonDict['elements']['edges'].append(tmp)

    jsonDict['layout'] = {
    'name': 'cose',
    'padding': 4,
     'fit'                 : True, 
   'nodeRepulsion'       : 10000,
    'nodeOverlap'         : 10,
    'idealEdgeLength'     : 10,
   'edgeElasticity'      : 100,
    'nestingFactor'       : 5, 
    'gravity'             : 250, 
    'numIter'             : 100,
    'initialTemp '        : 200,
    'coolingFactor'       : 0.95, 
    'minTemp'             : 1  },
  
    jsonDict['ready'] =  'function(){window.cy = this;}'

    return jsonDict

 
def tmpGenerateCont(bnglFile,graphType):
        fileName = generateContactMap(bnglFile, graphType)
        print '++++',fileName
        try:
            gml = nx.read_gml(fileName)
        except IOError:
            return ''
        result = gml2cyjson(gml)
        with open('out.json','w') as f:
            return json.dumps(result,f,indent=1, separators=(',', ': '))
        

class AnnotationServer(xmlrpc.XMLRPC):


    def xmlrpc_resolveAnnotations(self,annotations):

        result = threads.deferToThread(resolveAnnotations,annotations)
        return result
  
    def xmlrpc_getContactMap(self,bnglFile,graphType):
        fileName = generateContactMap(bnglFile, graphType)
        try:
            gml = nx.read_gml(fileName)
        except IOError:
            return {'jsonStr':'','gmlStr':''}
        result = gml2cyjson(gml)
        jsonStr = json.dumps(result,indent=1, separators=(',', ': '))
        result = {'jsonStr':jsonStr,'gmlStr':gml}
        return result

#server.register_function(is_even, "is_even")

#resolveAnnotations(tmpD)        
if __name__ == '__main__':
    print "Listening on port {0}...".format(port)
    r = AnnotationServer()
    reactor.listenTCP(port, server.Site(r))
    reactor.run()
    #print getcwd()
    #with open(getcwd() + '/../SBMLparser/complex/output48.bngl','r') as f:
    #    s = f.read()

    #tmpGenerateCont(s,'contact') 
   #a = [u'http://rest.kegg.jp/get/ath04710']
   #resolveAnnotation(a[0])

        

