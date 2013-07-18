# -*- coding: utf-8 -*-
"""
Created on Wed Jul 17 15:42:26 2013

@author: proto
"""

import webapp2
import jinja2
import os
from google.appengine.ext.webapp import blobstore_handlers
from google.appengine.ext import blobstore
from google.appengine.ext import ndb
import libsbml2bngl


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
        bnglContent = reader.read()
        libsbml2bngl.readFromString(bnglContent,None,None)
        #modelSubmission.put()

app = webapp2.WSGIApplication([
    ('/', MainPage),
    ('/process',ProcessFile)
], debug=True)