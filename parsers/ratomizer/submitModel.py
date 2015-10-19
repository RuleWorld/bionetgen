# -*- coding: utf-8 -*-
"""
Created on Wed Jul 17 15:42:26 2013

@author: proto
"""
from __future__ import with_statement

import webapp2
import jinja2
import os
from google.appengine.ext.webapp import blobstore_handlers
from google.appengine.ext import blobstore
from google.appengine.ext import ndb

import xmlrpclib
import urllib
import json
from google.appengine.api import files
from google.appengine.api import app_identity


from google.appengine.api import urlfetch
urlfetch.set_default_fetch_deadline(60)
import logging
import cloudstorage as gcs


class GAEXMLRPCTransport(object):
    """taken directly from http://brizzled.clapper.org/blog/2008/08/25/making-xmlrpc-calls-from-a-google-app-engine-application/"""
    """Handles an HTTP transaction to an XML-RPC server."""

    def __init__(self):
        pass

    def request(self, host, handler, request_body, verbose=0):
        result = None
        url = 'http://%s%s' % (host, handler)
        try:
            response = urlfetch.fetch(url,
                                      payload=request_body,
                                      method=urlfetch.POST,
                                      headers={'Content-Type': 'text/xml'},
                                      deadline=60)
        except:
            msg = 'Failed to fetch %s' % url
            logging.error(msg)
            raise xmlrpclib.ProtocolError(host + handler, 500, msg, {})

        if response.status_code != 200:
            logging.error('%s returned status code %s' %
                          (url, response.status_code))
            raise xmlrpclib.ProtocolError(host + handler,
                                          response.status_code,
                                          "",
                                          response.headers)
        else:
            result = self.__parse_response(response.content)

        return result

    def __parse_response(self, response_body):
        p, u = xmlrpclib.getparser(use_datetime=False)
        p.feed(response_body)
        return u.close()


JINJA_ENVIRONMENT = jinja2.Environment(
    loader=jinja2.FileSystemLoader(os.path.dirname(__file__)),
    extensions=['jinja2.ext.autoescape'])

DATABASE_NAME = 'models'

class ModelInfo(ndb.Model):
    """Models an individual Guestbook entry with author, content, and date."""
    content = ndb.BlobKeyProperty() #BlobInfo(blobkey)


class MainPage(webapp2.RequestHandler):
    def get(self):
        template_values = {
        
        }
        template =JINJA_ENVIRONMENT.get_template('index.html')
        self.response.write(template.render(template_values))

    
def dbmodel_key(model_name=DATABASE_NAME):
    """Constructs a Datastore key for a ModelDB entity with model_name."""
    return ndb.Key('ModelDB', model_name)
    
    
remoteServer = "http://54.214.249.43:9000"
#remoteServer = "http://127.0.0.1:9000"
class Translate(webapp2.RequestHandler):
    """
    Class frontend manager for the translate page. Calls the template that the user sees.
    """
    def get(self):
        upload_url = blobstore.create_upload_url('/process')
        
        #reactionFiles,speciesFiles = s.getSpeciesConventions()
        #print '-----',reactionFiles,speciesFiles
        template_values={
            'action' : upload_url,
            #'reactionDefinition' : ['1','2','3','4','5','6','7','8','9','10','a','b','c']
            #'reactionDefinition' : reactionFiles,
            #'speciesDefinition': speciesFiles
        }
        template =JINJA_ENVIRONMENT.get_template('translate.html')
        self.response.write(template.render(template_values))

class ProcessFile(blobstore_handlers.BlobstoreUploadHandler):
    def post(self):
        """
        Process the file translation form contained in translate.html. Calls a remove service defined
        in <remoteServer>, and sends it a file and an atomization flag.
        """
        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]
        reader = blob_info.open()
        sbmlContent = xmlrpclib.Binary(reader.read())
        atomizeString = self.request.get('atomize')
        reaction = self.request.get('reaction')
        species = self.request.get('species')
        #print 'fsdgsdgsd',atomize
        #https://developers.google.com/appengine/docs/python/urlfetch/fetchfunction
        #https://groups.google.com/forum/#!topic/google-appengine/XbrJvt9LfuI
        s = xmlrpclib.ServerProxy(remoteServer,GAEXMLRPCTransport())
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9000',GAEXMLRPCTransport())
        ticket = s.atomize(sbmlContent,atomizeString,reaction,species)
        #self.response.write(result)
        
        self.redirect('/waitFile?ticket={0}&fileName={1}'.format(ticket,blob_info.filename))


class WaitFileJson(webapp2.RequestHandler):
    def get(self):
        ticket = self.request.get("ticket")
        s = xmlrpclib.ServerProxy(remoteServer,GAEXMLRPCTransport())
        result = s.isready(int(ticket))
        resultJson = {'result':result}
        resultJson = json.dumps(resultJson)
        self.response.headers.add_header('content-type', 'application/json', charset='utf-8')
        self.response.out.write(resultJson)
    
import re
class generateConfigFile(webapp2.RequestHandler):
    def processPattern(self, pattern):
        #speciesDefinition = re.
        pass

    def get(self):
        names = self.request.get_all("speciesNames")
        patterns = self.request.get_all("patterns")
        results = {'complexDefinition': [], 'reactionDefinition': []}

        for name,pattern in zip(names,patterns):
            processedPattern = self.processPattern(pattern)
            #result.


def CreateFile(filename,content):
    """Create a GCS file with GCS client lib.

    Args:
    filename: GCS filename.

    Returns:
    The corresponding string blobkey for this GCS file.
    """
    # Create a GCS file with GCS client.
    with gcs.open(filename, 'w') as f:
        f.write(content.encode('utf-8', 'replace'))

    # Blobstore API requires extra /gs to distinguish against blobstore files.
    blobstore_filename = '/gs' + filename
    # This blob_key works with blobstore APIs that do not expect a
    # corresponding BlobInfo in datastore.
    bk =  blobstore.create_gs_key(blobstore_filename)
    if not isinstance(bk, blobstore.BlobKey):
        bk = blobstore.BlobKey(bk)
    return bk

class WaitFile(webapp2.RequestHandler):
    """
    manages the waiting between the time a file is sent to the remote server and the time the json is received. prints the status to
    the user (sucess, still watiing, error)
    """
    def get(self):
        ticket = self.request.get("ticket")
        fileName = self.request.get("fileName")
        s = xmlrpclib.ServerProxy(remoteServer,GAEXMLRPCTransport())
        result = s.isready(int(ticket))
        #the first time it loads the page, if the result isnt immediate it will enter this branch
        if result in [-2,'-2']:
            redirectionURL='waitFile?ticket={0}&fileName={1}'.format(ticket,fileName)
            template_values={
                'redirection' : redirectionURL,
                'ticket':int(ticket),
                'fileName':fileName
            }
            template =JINJA_ENVIRONMENT.get_template('wait.html')
            self.response.write(template.render(template_values))
        elif result in [-1,'-1']:
            #bad request
            self.response.write("Your request doesn't exist. Please submit your file again")
            return

        else:
            #when the result is here it will enter this branch
            s = xmlrpclib.ServerProxy(remoteServer,GAEXMLRPCTransport())
            result = s.getDict(int(ticket))
            if result in ['-5',-5]:
                self.response.write("There was an error processing your request")
                return

            #old create a file using the files api
            '''    
            file_name = files.blobstore.create(mime_type='application/octet-stream')
            
            # Open the file and write to it
            result = result.encode('ascii', 'ignore')
            with files.open(file_name, 'a') as f:
              f.write(result)
            
            # Finalize the file. Do this before attempting to read it.
            files.finalize(file_name)
            
            # Get the file's blob key
            blob_key = files.blobstore.get_blob_key(file_name)
            '''

            bucket_name = os.environ.get('BUCKET_NAME',
                                         app_identity.get_default_gcs_bucket_name())

            gcs_filename = '/{1}/{0}.bngl'.format(fileName, bucket_name)
            blob_key = CreateFile(gcs_filename, result.decode('utf-8', 'replace'))

            ###        
            #blob_info = blobstore.BlobInfo.get(blob_key)
            #output = blob_info.open()
            ###
            printStatement = '<a href="/serve/{1}?key={0}">{1}</a>'.format(blob_key,fileName)
            #p2 = output.read()        
            self.response.write(printStatement)
                    #modelSubmission.put()
    def post(self):
        return self.get()
        
class Refine(webapp2.RequestHandler):
    def get(self):
        template_values={
        
        }
        template =JINJA_ENVIRONMENT.get_template('refine.html')
        self.response.write(template.render(template_values))


class Graph(webapp2.RequestHandler):
    def get(self):
        upload_url = blobstore.create_upload_url('/graphp')
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9100')
        
        template_values={
            'action' : upload_url,
            #'reactionDefinition' : ['1','2','3','4','5','6','7','8','9','10','a','b','c']
        }
        template =JINJA_ENVIRONMENT.get_template('graph.html')
        self.response.write(template.render(template_values))


class ExpandAnnotation(webapp2.RequestHandler):
    def get(self):
        upload_url = blobstore.create_upload_url('/eannotation')
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9100')
        
        template_values={
            'action' : upload_url,
            #'reactionDefinition' : ['1','2','3','4','5','6','7','8','9','10','a','b','c']
        }
        template =JINJA_ENVIRONMENT.get_template('annotation.html')
        self.response.write(template.render(template_values))

class ExpandAnnotationMethod(blobstore_handlers.BlobstoreUploadHandler):
     def post(self):
        
        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]
        reader = blob_info.open()
        sbmlContent = xmlrpclib.Binary(reader.read())

        #https://developers.google.com/appengine/docs/python/urlfetch/fetchfunction
        #https://groups.google.com/forum/#!topic/google-appengine/XbrJvt9LfuI
        s = xmlrpclib.ServerProxy(remoteServer,GAEXMLRPCTransport())
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9000',GAEXMLRPCTransport())
        ticket = s.generateAnnotations(sbmlContent)
        #self.response.write(result)
        
        self.redirect('/waitFile?ticket={0}&fileName={1}'.format(ticket,blob_info.filename))


class GraphFile(blobstore_handlers.BlobstoreUploadHandler):
     def post(self):
        
        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]
        reader = blob_info.open()
        bnglContent = xmlrpclib.Binary(reader.read())

        returnType = self.request.get('return')
        if returnType == 'Regulatory Graph':
            graphType = 'regulatory'
        elif returnType == 'Contact map':
            graphType = 'contactmap'
        elif returnType == 'SBGN-ER':
            graphType = 'sbgn_er'
        #https://developers.google.com/appengine/docs/python/urlfetch/fetchfunction
        #https://groups.google.com/forum/#!topic/google-appengine/XbrJvt9LfuI
        s = xmlrpclib.ServerProxy(remoteServer,GAEXMLRPCTransport())
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9000',GAEXMLRPCTransport())
        ticket = s.generateGraph(bnglContent,graphType)
        #self.response.write(result)
        
        self.redirect('/waitFile?ticket={0}&fileName={1}_{2}.gml'.format(ticket,blob_info.filename,graphType))

                
class ServeHandler(blobstore_handlers.BlobstoreDownloadHandler):
  def get(self, resource):
    resource = str(urllib.unquote(self.request.get('key')))
    #blob_info = blobstore.BlobInfo.get(resource)
    self.send_blob(resource)
    
app = webapp2.WSGIApplication([
    ('/', MainPage),
    ('/translate',Translate),
    ('/serve/([^/]+)?', ServeHandler),
    ('/process',ProcessFile),
    ('/refine',Refine),
    ('/annotation',ExpandAnnotation),
    ('/eannotation',ExpandAnnotationMethod),
    ('/graphp',GraphFile),
    ('/graph',Graph),
    ('/waitFile',WaitFile),
    ('/testFile',WaitFileJson)
], debug=True)