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


JINJA_ENVIRONMENT = jinja2.Environment(
    loader=jinja2.FileSystemLoader(os.path.dirname(__file__)),
    extensions=['jinja2.ext.autoescape'])

DATABASE_NAME = 'models'

class ModelInfo(ndb.Model):
    """Models an individual Guestbook entry with author, content, and date."""
    content = ndb.BlobKeyProperty() #BlobInfo(blobkey)

    
def dbmodel_key(model_name=DATABASE_NAME):
    """Constructs a Datastore key for a ModelDB entity with model_name."""
    return ndb.Key('ModelDB', model_name)
    
class MainPage(webapp2.RequestHandler):
    def get(self):
        upload_url = blobstore.create_upload_url('/process')
        
        template_values={
            'action' : upload_url
        }
        template =JINJA_ENVIRONMENT.get_template('index.html')
        self.response.write(template.render(template_values))

class ProcessFile(blobstore_handlers.BlobstoreUploadHandler):
    def post(self):
        
        model_name = self.request.get('model_name',
                                          DATABASE_NAME)
        modelSubmission = ModelInfo(parent=dbmodel_key(model_name))

        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]
        reader = blob_info.open()
        sbmlContent = xmlrpclib.Binary(reader.read())
        s = xmlrpclib.ServerProxy('http://54.214.249.43:9000')
        result = s.atomize(sbmlContent)
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
                
class ServeHandler(blobstore_handlers.BlobstoreDownloadHandler):
  def get(self, resource):
    resource = str(urllib.unquote(self.request.get('key')))
    blob_info = blobstore.BlobInfo.get(resource)
    self.send_blob(blob_info)
    
app = webapp2.WSGIApplication([
    ('/', MainPage),
    ('/serve/([^/]+)?', ServeHandler),
    ('/process',ProcessFile)
], debug=True)