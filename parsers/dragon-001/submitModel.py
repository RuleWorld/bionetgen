# -*- coding: utf-8 -*-
"""
Created on Mon May 27 20:32:52 2013

@author: proto
"""

import cgi
import urllib
import os
from google.appengine.api import users
from google.appengine.ext import ndb
from google.appengine.ext.webapp import blobstore_handlers
from google.appengine.ext import blobstore

import webapp2


import jinja2

JINJA_ENVIRONMENT = jinja2.Environment(
    loader=jinja2.FileSystemLoader(os.path.dirname(__file__)),
    extensions=['jinja2.ext.autoescape'])
    

DEFAULT_GUESTBOOK_NAME = 'default_guestbook'


# We set a parent key on the 'Greetings' to ensure that they are all in the same
# entity group. Queries across the single entity group will be consistent.
# However, the write rate should be limited to ~1/second.

def dbmodel_key(model_name=DEFAULT_GUESTBOOK_NAME):
    """Constructs a Datastore key for a ModelDB entity with model_name."""
    return ndb.Key('ModelDB', model_name)

class PublicationInfo(ndb.Model):
    name = ndb.StringProperty()
    journal = ndb.StringProperty()

 
class AnnotationInfo(ndb.Model):
    database = ndb.StringProperty()
    databaseID = ndb.StringProperty()


class ModelInfo(ndb.Model):
    """Models an individual Guestbook entry with author, content, and date."""
    author = ndb.StringProperty()
    content = ndb.BlobKeyProperty() #BlobInfo(blobkey)
    simulationResults = ndb.BlobKeyProperty()
    date = ndb.DateTimeProperty(auto_now_add=True)
    publication = ndb.StructuredProperty(PublicationInfo)
    fileFormat = ndb.StringProperty(choices=set(["bngl","kappa"]))
    submitter = ndb.UserProperty()
    annotationInfo = ndb.KeyProperty(kind=AnnotationInfo,repeated=True)
   

class MainPage(webapp2.RequestHandler):
    def get(self):
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
    
        template_values ={
            'url': url,
            'url_linktext': url_linktext,        
        }
        template =JINJA_ENVIRONMENT.get_template('index.html')
        self.response.write(template.render(template_values))

class Submit(webapp2.RequestHandler):

    def get(self):
        upload_url = blobstore.create_upload_url('/sign')
        
        #model_name = self.request.get('model_name', DEFAULT_GUESTBOOK_NAME)
        #models_query = ModelInfo.query(
        #    ancestor=dbmodel_key(model_name)).order(-ModelInfo.date)
        #models = models_query.fetch(10)
        
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'

        template_values = {
            'action':  upload_url,
            #'models': models,
            #'model_name': urllib.urlencode({'model_name':model_name}),
            'url': url,
            'url_linktext': url_linktext,
            'formatOptions':set(["bngl","kappa"])
        }

        template = JINJA_ENVIRONMENT.get_template('submit.html')
        self.response.write(template.render(template_values))



class ModelDB(blobstore_handlers.BlobstoreUploadHandler):

    def post(self):
        # We set the same parent key on the 'Greeting' to ensure each greeting
        # is in the same entity group. Queries across the single entity group
        # will be consistent. However, the write rate to a single entity group
        # should be limited to ~1/second.
        
        
        
        model_name = self.request.get('model_name',
                                          DEFAULT_GUESTBOOK_NAME)
        modelSubmission = ModelInfo(parent=dbmodel_key(model_name))
        publicationInfo = PublicationInfo(parent=dbmodel_key(model_name))
        publicationInfo.name = self.request.get('publication')
        publicationInfo.journal =   self.request.get('journal')
        if users.get_current_user():
            modelSubmission.submitter = users.get_current_user()
            
        upload_files = self.get_uploads('file')
        simulation = self.get_uploads('simulation')
        blob_info = upload_files[0]
        blob_info2 = simulation[0]
        modelSubmission.content = blob_info.key()
        modelSubmission.author = self.request.get('author')
        modelSubmission.publication = publicationInfo
        modelSubmission.fileFormat = self.request.get('fileFormat')
        modelSubmission.simulationResults = blob_info2.key() 
        
        modelSubmission.put()

        query_params = {'model_name': model_name}
        self.redirect('/?' + urllib.urlencode(query_params))
 
    
class Query(webapp2.RequestHandler):
    def get(self):
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
    
        template_values ={
            'url': url,
            'url_linktext': url_linktext,
            'queryOptions':set(['Author','Publication'])
        }
        template =JINJA_ENVIRONMENT.get_template('query.html')
        self.response.write(template.render(template_values))

class addAnnotation(webapp2.RequestHandler):
    def post(self):
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
        
#http://www.youtube.com/watch?feature=player_embedded&v=I3Dh5a9XxX4                


class Search(webapp2.RequestHandler):
    def post(self):
        if self.request.get('searchSelection') == 'Publication':
            query = ModelInfo.publication
        elif self.request.get('searchSelection') == 'Author':
            query = ModelInfo.author
        q = ModelInfo.query(query == self.request.get('queryString'))
        queryArray = []
        counter = 0
        for p in q.iter(limit=5):
            counter += 1            
            dp = p.to_dict()
            newdp = {}
            for element in dp:
                if element in ['content','simulationResults']:
                    printStatement = blobstore.BlobInfo(dp[element]).filename
                else:
                    printStatement = dp[element]
                newdp[element] = printStatement
                
                self.response.write('{0} : {1}<br>'.format(element,printStatement))
            queryArray.append(newdp)
        self.response.write('<br>Found {0} results<br>'.format(counter))
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'            
        template_values ={
            'url': url,
            'url_linktext': url_linktext,
            'queryArray':queryArray
        }
        template =JINJA_ENVIRONMENT.get_template('results.html')   
        if counter > 0:
            self.response.write(dp)
            self.response.write('<br>')
            
  
            
app = webapp2.WSGIApplication([
    ('/', MainPage),
    ('/submit',Submit),
    ('/query',Query),
    ('/sign', ModelDB),
    ('/search',Search),
    
    
], debug=True)