# -*- coding: utf-8 -*-
"""
Created on Mon Oct  8 14:16:25 2012

@author: proto

"""


import bioservices
import pyparsing as pyp


from SimpleXMLRPCServer import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCRequestHandler


# Restrict to a particular path.
class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

# Create server
server = SimpleXMLRPCServer(("10.253.98.102", 9200),              requestHandler=RequestHandler)
#server = SimpleXMLRPCServer(("127.0.0.1", 9200), requestHandler=RequestHandler)
server.register_introspection_functions()



def resolveAnnotation(annotation):
    if not hasattr(resolveAnnotation, 'db'):
        resolveAnnotation.db = {}
        resolveAnnotation.ch = bioservices.ChEBI(verbose=False)
        resolveAnnotation.uni = bioservices.UniProt(verbose=False)
        resolveAnnotation.k = bioservices.kegg.KeggParser(verbose=False)
        resolveAnnotation.qg = bioservices.QuickGO(verbose=False)
        resolveAnnotation.db['http://identifiers.org/uniprot/P62988'] = 'http://identifiers.org/uniprot/P62988'
        resolveAnnotation.db['http://identifiers.org/uniprot/P06842'] = 'http://identifiers.org/uniprot/P06842'
        resolveAnnotation.db['http://identifiers.org/uniprot/P07006'] = 'http://identifiers.org/uniprot/P06842'
        
    if annotation in resolveAnnotation.db:
        return resolveAnnotation.db[annotation]
    
        
    tAnnotation = annotation.replace('%3A',':')
    tAnnotation = annotation.split('/')[-1]
    #tAnnotation = re.search(':([^:]+:[^:]+$)',tAnnotation).group(1)
    '''
    if 'GO' in annotation:            
        cur.execute("SELECT * FROM term WHERE acc='{0}'".format(tAnnotation))
        for row in cur.fetchall():
            bioArray.append([row[1],row[3]])
    elif 'reactome' in annotation:
        tAnnotation2 = re.search('_([^_]+$)',tAnnotation).group(1)
        #query = rserv.queryById(Types.longType(long('9417')))
        #query = rserv.queryPathwaysForReferenceIdentifiers([tAnnotation])
        print str(query)
        bioArray.append([query['name'],tAnnotation])
    '''
    try:
        if 'obo.go' in annotation:
            res = resolveAnnotation.qg.Term(tAnnotation)
            tmp = res.findAll('name')
            finalArray = []
            goGrammar = pyp.Suppress(pyp.Literal('<name>')) +  pyp.Word(pyp.alphanums + ' ') + pyp.Suppress(pyp.Literal('</name>')) 
            for x in tmp:
                finalArray.append(goGrammar.parseString(str(x))[0])
            tmp = finalArray
            resolveAnnotation.db[annotation] = tmp
            return tmp
    
        elif 'kegg' in annotation:
            
            data = resolveAnnotation.k.get(tAnnotation)
            dict_data =  resolveAnnotation.k.parse(data)
            resolveAnnotation.db[annotation] = dict_data['name']
            return resolveAnnotation.db[annotation]
            
        elif 'uniprot' in annotation:
            identifier = annotation.split('/')[-1]
            result = resolveAnnotation.uni.quick_search(identifier)
            resolveAnnotation.db[annotation] = result[result.keys()[0]]['Protein names']
            return resolveAnnotation.db[annotation]
            
        elif 'chebi' in annotation:
            tmp = annotation.split('/')[-1]
            
            entry = resolveAnnotation.ch.getLiteEntity(tmp)
            for element in entry:
                resolveAnnotation.db[annotation] = element['chebiAsciiName']
                return resolveAnnotation.db[annotation]
        elif 'cco' in annotation or 'pirsf' in annotation or 'pubchem' in annotation or 'omim' in annotation:
            return annotation
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
            print 'ERRROERROROERRRO'
            #assert(False)
            return annotation
    except:
        return annotation
    #finally:



class AnnotationServer:
    
    def __init__(self):
        pass

    def getSpeciesConventions(self,annotation):
        return resolveAnnotation(annotation)


        
server.register_instance(AnnotationServer())

# Run the server's main loop
server.serve_forever()

        

