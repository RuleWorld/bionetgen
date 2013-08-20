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

from google.appengine.api import files

from google.appengine.api import urlfetch


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
    
class Translate(webapp2.RequestHandler):
    def get(self):
        upload_url = blobstore.create_upload_url('/process')
        s = xmlrpclib.ServerProxy('http://54.214.249.43:9000')
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9000')
        
        reactionFiles,speciesFiles = s.getSpeciesConventions()
        print '-----',reactionFiles,speciesFiles
        template_values={
            'action' : upload_url,
            #'reactionDefinition' : ['1','2','3','4','5','6','7','8','9','10','a','b','c']
            'reactionDefinition' : reactionFiles,
            'speciesDefinition': speciesFiles
        }
        template =JINJA_ENVIRONMENT.get_template('translate.html')
        self.response.write(template.render(template_values))

class ProcessFile(blobstore_handlers.BlobstoreUploadHandler):
    def post(self):
        
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
        s = xmlrpclib.ServerProxy('http://54.214.249.43:9000')
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9000') 
        result = s.atomize(sbmlContent,atomizeString,reaction,species)
        #self.response.write(result)

        file_name = files.blobstore.create(mime_type='application/octet-stream')
        
        # Open the file and write to it
        with files.open(file_name, 'a') as f:
          f.write(result)
        
        # Finalize the file. Do this before attempting to read it.
        files.finalize(file_name)
        
        # Get the file's blob key
        blob_key = files.blobstore.get_blob_key(file_name)
        
        ###        
        #blob_info = blobstore.BlobInfo.get(blob_key)
        #output = blob_info.open()
        ###
        printStatement = '<a href="/serve/{1}.bngl?key={0}">{1}.bngl</a>'.format(blob_key,blob_info.filename)
        #p2 = output.read()        
        self.response.write(printStatement)
                #modelSubmission.put()

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


class GraphFile(blobstore_handlers.BlobstoreUploadHandler):
    def post(self):
        

        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]
        reader = blob_info.open()
        bnglContent = xmlrpclib.Binary(reader.read())
        
        returnType = self.request.get('return')
        reactionCenter = True if self.request.get('center') else False
        context = True if self.request.get('context') else False
        product = True if self.request.get('products') else False
        #atomizeString = self.request.get('atomize')
        #print 'fsdgsdgsd',atomize
        urlfetch.set_default_fetch_deadline(blob_info.size/300)
        s = xmlrpclib.ServerProxy('http://54.214.249.43:9100')
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9100') 
        dotResult = s.bipartite(bnglContent,returnType,reactionCenter,context,product).data
                
        '''
        rpc = urlfetch.create_rpc(deadline= 10)
        urlfetch.make_fetch_call(rpc, "http://54.214.249.43:9100",method='bipartite',headers=[bnglContent,returnType])
        try:
            result = rpc.get_result()
            if result.status_code == 200:
                dotResult = result.content
        # ...
        except urlfetch.DownloadError:
            dotResult = None
        '''
    # Request timed out or failed.
    # ...
        #self.response.write(result)

        file_name = files.blobstore.create(mime_type='application/octet-stream')
        
        # Open the file and write to it
        with files.open(file_name, 'a') as f:
          f.write(dotResult)
        
        # Finalize the file. Do this before attempting to read it.
        files.finalize(file_name)
        
        # Get the file's blob key
        blob_key = files.blobstore.get_blob_key(file_name)
        
        ###        
        #blob_info = blobstore.BlobInfo.get(blob_key)
        #output = blob_info.open()
        ###
        printStatement = '<a href="/serve/{1}.{2}?key={0}">{1}.{2}</a>'.format(blob_key,blob_info.filename,returnType)
        #p2 = output.read()        
        self.response.write(printStatement)


                
class ServeHandler(blobstore_handlers.BlobstoreDownloadHandler):
  def get(self, resource):
    resource = str(urllib.unquote(self.request.get('key')))
    blob_info = blobstore.BlobInfo.get(resource)
    self.send_blob(blob_info)
    
app = webapp2.WSGIApplication([
    ('/', MainPage),
    ('/translate',Translate),
    ('/serve/([^/]+)?', ServeHandler),
    ('/process',ProcessFile),
    ('/graphp',GraphFile),
    ('/graph',Graph)
], debug=True)