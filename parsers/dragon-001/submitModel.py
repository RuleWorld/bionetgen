# -*- coding: utf-8 -*-
"""
Created on Mon May 27 20:32:52 2013

@author: proto
"""

from __future__ import with_statement

import urllib
import os
from google.appengine.ext.db import Key
from google.appengine.api import users
from google.appengine.ext import ndb
from google.appengine.ext.webapp import blobstore_handlers
from google.appengine.ext import blobstore
from google.appengine.api.images import get_serving_url
from google.appengine.api import taskqueue

import webapp2
import xmlrpclib
import jinja2
import zipfile
import tempfile

from google.appengine.api import files
import parseAnnotations

from google.appengine.api import urlfetch
urlfetch.set_default_fetch_deadline(45)
import logging

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
    author = ndb.StringProperty(repeated=True)
    content = ndb.BlobKeyProperty() #BlobInfo(blobkey)
    contactMap = ndb.BlobKeyProperty()
    name = ndb.StringProperty()
    description = ndb.StringProperty()
    date = ndb.DateTimeProperty(auto_now_add=True)
    publication = ndb.StructuredProperty(PublicationInfo)
    fileFormat = ndb.StringProperty(choices=set(["bngl","kappa"]))
    submitter = ndb.UserProperty()
    annotationInfo = ndb.KeyProperty(kind=AnnotationInfo,repeated=True)
    tags = ndb.StringProperty(repeated=True)
    structuredTags = ndb.StringProperty(repeated=True)
    privacy = ndb.StringProperty()
    notes = ndb.StringProperty()

class MainPage(webapp2.RequestHandler):
    def get(self):
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
            current_user = True
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
            current_user = False
    
        template_values ={
            'url': url,
            'url_linktext': url_linktext,
            'current_user':current_user,
            'homepageh':'current_page_item'
        }
        template =JINJA_ENVIRONMENT.get_template('index2.html')
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
            current_user=True
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
            current_user=False

        template_values = {
            'action':  upload_url,
            #'models': models,
            #'model_name': urllib.urlencode({'model_name':model_name}),
            'url': url,
            'url_linktext': url_linktext,
            'formatOptions':set(["bngl","kappa"]),
            'current_user':current_user,
            'submith':'current_page_item'
        }

        template = JINJA_ENVIRONMENT.get_template('submit2.html')
        self.response.write(template.render(template_values))

class SubmitFile(webapp2.RequestHandler):
    def get(self):
        upload_url = blobstore.create_upload_url('/signFile')
        
        #model_name = self.request.get('model_name', DEFAULT_GUESTBOOK_NAME)
        #models_query = ModelInfo.query(
        #    ancestor=dbmodel_key(model_name)).order(-ModelInfo.date)
        #models = models_query.fetch(10)
        
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
            current_user=True
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
            current_user=False

        template_values = {
            'action':  upload_url,
            #'models': models,
            #'model_name': urllib.urlencode({'model_name':model_name}),
            'url': url,
            'url_linktext': url_linktext,
            'formatOptions':set(["bngl","kappa"]),
            'current_user':current_user,
            'submith':'current_page_item'
        }

        template = JINJA_ENVIRONMENT.get_template('submitFile2.html')
        self.response.write(template.render(template_values))

class SubmitBatch(webapp2.RequestHandler):
    def get(self):
        upload_url = blobstore.create_upload_url('/signBatch')
        
        #model_name = self.request.get('model_name', DEFAULT_GUESTBOOK_NAME)
        #models_query = ModelInfo.query(
        #    ancestor=dbmodel_key(model_name)).order(-ModelInfo.date)
        #models = models_query.fetch(10)
        
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
            current_user=True
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
            current_user=False

        template_values = {
            'action':  upload_url,
            #'models': models,
            #'model_name': urllib.urlencode({'model_name':model_name}),
            'url': url,
            'url_linktext': url_linktext,
            'formatOptions':set(["bngl","kappa"]),
            'current_user':current_user,
            'submith':'current_page_item'
        }

        template = JINJA_ENVIRONMENT.get_template('submitBatch.html')
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
        contact = self.get_uploads('contact ')
        blob_info = upload_files[0]
        blob_info2 = contact[0]
        modelSubmission.content = blob_info.key()
        modelSubmission.author = [self.request.get('author')]
        modelSubmission.publication = publicationInfo
        modelSubmission.fileFormat = self.request.get('fileFormat')
        modelSubmission.contactMap = blob_info2.key() 
        modelSubmission.name = self.request.get('name')
        modelSubmission.description = self.request.get('description')
        modelSubmission.privacy = self.request.get('privacy')
        modelSubmission.submitter =  users.get_current_user()        
        modelSubmission.put()
        

        query_params = {'model_name': model_name}
        self.redirect('/?' + urllib.urlencode(query_params))
 

def processAnnotations(bnglContent):
    annotationDict = parseAnnotations.parseAnnotations(bnglContent)
    parsedAnnotationDict = parseAnnotations.dict2DatabaseFormat(annotationDict)
    #s = xmlrpclib.ServerProxy('http://127.0.0.1:9200')
    s = xmlrpclib.ServerProxy('http://54.214.249.43:9200',GAEXMLRPCTransport())
    tagArray = s.resolveAnnotations(parsedAnnotationDict['structuredTags'])
    tagDict = {}
    for element in tagArray:
        tagDict[element[0]] = element[1]
    print tagDict
    return parsedAnnotationDict,tagDict
    
class ModelDBFile(blobstore_handlers.BlobstoreUploadHandler):

    def post(self):
        # We set the same parent key on the 'Greeting' to ensure each greeting
        # is in the same entity group. Queries across the single entity group
        # will be consistent. However, the write rate to a single entity group
        # should be limited to ~1/second.
        
        
        
        model_name = self.request.get('model_name',
                                          DEFAULT_GUESTBOOK_NAME)
        modelSubmission = ModelInfo(parent=dbmodel_key(model_name))
        publicationInfo = PublicationInfo(parent=dbmodel_key(model_name))

        '''
        publicationInfo.name = self.request.get('publication')
        publicationInfo.journal =   self.request.get('journal')
        '''        
        if users.get_current_user():
            modelSubmission.submitter = users.get_current_user()
            
        upload_files = self.get_uploads('file')
        contact = self.get_uploads('contact')
        blob_info = upload_files[0]
        blob_info2 = contact[0]
        modelSubmission.content = blob_info.key()
        modelSubmission.privacy = self.request.get('privacy')
        modelSubmission.submitter =  users.get_current_user()
        modelSubmission.name = blob_info.filename 
        modelSubmission.contactMap = blob_info2.key() 
        modelSubmission.contactMap = blob_info2.key()
        
        #annotationDict = parseAnnotations.parseAnnotations(bnglContent)
        #parsedAnnotationDict = parseAnnotations.dict2DatabaseFormat(annotationDict)
        #s = xmlrpclib.ServerProxy('http://127.0.0.1:9200')
        #tagArray = s.resolveAnnotations(parsedAnnotationDict['structuredTags'])

        bnglContent = blob_info.open().read()
        parsedAnnotationDict,tagArray = processAnnotations(bnglContent)
        modelSubmission.author = [parsedAnnotationDict['author']]
        modelSubmission.structuredTags = parsedAnnotationDict['structuredTags']
        if 'tags' in tagArray:
            modelSubmission.tags = tagArray['tags']
        if 'modelName' in tagArray:
            modelSubmission.name = tagArray['modelName'][0]
        if 'author' in tagArray:
            modelSubmission.author = tagArray['author']
        modelSubmission.put()

        #modelSubmission.author = self.request.get('author')
        #modelSubmission.publication = publicationInfo
        #modelSubmission.fileFormat = self.request.get('fileFormat')
        
        #modelSubmission.name = self.request.get('name')
        #modelSubmission.description = self.request.get('description')
        

        query_params = {'model_name': model_name}
        self.redirect('/?' + urllib.urlencode(query_params))
 
 
 
class ModelDBBatch(blobstore_handlers.BlobstoreUploadHandler):

    def post(self):
        # We set the same parent key on the 'Greeting' to ensure each greeting
        # is in the same entity group. Queries across the single entity group
        # will be consistent. However, the write rate to a single entity group
        # should be limited to ~1/second.
        
        
        

        '''
        publicationInfo.name = self.request.get('publication')
        publicationInfo.journal =   self.request.get('journal')
        '''        
            
        upload_files = self.get_uploads('file')
        #contact = self.get_uploads('contact')
        #blob_info = upload_files[0]
        reader = upload_files[0].open()
        
        tmp = tempfile.TemporaryFile()
        tmp.write(reader.read()) # myZipFile is your decoded string containing the zip-data
        objZip = zipfile.ZipFile(tmp)
        
        
        nameList = objZip.namelist()

        bnglnameList = [x for x in nameList if '.bngl' in x]        
        print nameList,bnglnameList
        for element in bnglnameList:

            zipModel = objZip.open(element)
            bnglContent = zipModel.read()
            taskqueue.add(url='/processfileq', params={'element':element,'bnglContent':bnglContent,'nameList':nameList})

        self.redirect('/')
 

class ProcessAnnotation(webapp2.RequestHandler):
    def post(self):
            element = self.request.get('element')
            bnglContent = self.request.get('bnglContent')
            nameList = self.request.get('nameList')

            model_name = self.request.get('model_name',
                                              DEFAULT_GUESTBOOK_NAME)
            modelSubmission = ModelInfo(parent=dbmodel_key(model_name))
            if users.get_current_user():
                modelSubmission.submitter = users.get_current_user()
       
            #fixme: this will be deprecated in the near future, use gcs client library instead
            file_name = files.blobstore.create(mime_type='application/octet-stream',_blobinfo_uploaded_filename=element)
            with files.open(file_name, 'a') as f:
              f.write(bnglContent)
              #files.finalize(file_name)
              f.close(finalize=True)
            blob_key = files.blobstore.get_blob_key(file_name)   
            
            modelSubmission.content = blob_key
            modelSubmission.name = element
            '''
            if '{0}.png'.format(element) in nameList:
                zipModel = objZip.open(element)
                file_name = files.blobstore.create(mime_type='application/octet-stream',
                                                   _blobinfo_uploaded_filename='{0}.png'.format(element))
                with files.open(file_name, 'a') as f:
                  f.write(zipModel.read())
                files.finalize(file_name)
                modelSubmission.contactMap(file_name)
             '''

            parsedAnnotationDict,tagArray = processAnnotations(bnglContent)
            modelSubmission.author = [parsedAnnotationDict['author']]
            modelSubmission.structuredTags = parsedAnnotationDict['structuredTags']
            if 'tags' in tagArray:
                modelSubmission.tags = tagArray['tags']
            if 'modelName' in tagArray:
                modelSubmission.name = tagArray['modelName'][0]
            if 'author' in tagArray:
                modelSubmission.author = tagArray['author']
            modelSubmission.put()

class Query(webapp2.RequestHandler):
    def get(self):
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
            current_user=True
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
            current_user=False
    
        template_values ={
            'url': url,
            'url_linktext': url_linktext,
            'queryOptions':set(['Author','Key Terms','Biomodels.org URI']),
            'current_user':current_user,
            'queryh':'current_page_item'
        }
        template =JINJA_ENVIRONMENT.get_template('query2.html')
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

class List(webapp2.RequestHandler):
    def get(self):
        q = ModelInfo.query()
        queryArray = []
        counter = 0
        for p in q.iter():
            
            counter += 1            
            dp = p.to_dict()
            newdp = {}
            #for element in dp:
            #response.write('{1}: Name: <a href="description?file={2}">{0}</a><br>'.format(dp['name'],counter,dp['name']))
            #response.write('Description: {0}<br><br>'.format(dp['description']))
            newdp['link'] = 'description?file={0}'.format(dp['name'])
            newdp['name'] = dp['name']
            newdp['counter'] = counter
            newdp['description'] = dp['description']
            
            #response.write('{0} : {1}<br>'.format(element,printStatement))
            queryArray.append(newdp)

        #self.response.write('<br><br>Found {0} results<br>'.format(counter))
     
        template_values = boilerplateParams(self.request.uri)
        template_values['counter'] = counter
        template_values['queryArray'] = queryArray
        template_values['listh'] = 'current_page_item'
        
        template =JINJA_ENVIRONMENT.get_template('resultsList2.html')
        self.response.write(template.render(template_values))

def boilerplateParams(uri):
    if users.get_current_user():
        url = users.create_logout_url(uri)
        url_linktext = 'Logout'
        current_user=True
    else:
        url = users.create_login_url(uri)
        url_linktext = 'Login'
        current_user=False
    
    
    template_values ={
    'url': url,
    'url_linktext': url_linktext,
    'current_user': current_user
    }
    
    return template_values

    

class Description(webapp2.RequestHandler):
    def get(self):
        query = ModelInfo.name
        q = ModelInfo.query(query == self.request.get('file'))
        ndp = {}
        queryArray = []
        
        for p in q.iter():
            dp = p.to_dict()
            
            for element in dp:
                #self.response.write('{0} : {1}<br>'.format(element,dp[element]))

                if element in ['content']:
                    ndp[element] = ["serve/{1}?key={0}".format(dp[element],dp['name']),blobstore.BlobInfo(dp[element]).filename]
                elif element in ['contactMap']:
                    try:                    
                        blobkey = urllib.unquote(str(dp[element]))
                        url = get_serving_url(blobkey,size=400)
                        url ='serve/{1}?key={0}'.format(dp[element],blobstore.BlobInfo(dp[element]).filename)
                        ndp[element] = url
                    except TypeError:
                        pass
                    #printStatement = '<img src=image?img_id={0}/><br>'.format(dp[element])
                else:
                    ndp[element] = dp[element]
                 
            queryArray.append(ndp)

        template_values = boilerplateParams(self.request.uri)
        template_values['queryArray'] = queryArray
        template_values['listh'] = 'current_page_item'

        template =JINJA_ENVIRONMENT.get_template('singleResult2.html')
        self.response.write(template.render(template_values))


class Image (webapp2.RequestHandler):
    def get(self):
        #get the key of the image "img_id" from datastore
        #what is the value of "img_id"? Where does it come from?
        #how does the app engine know to get what key for which image?
        key = Key(self.request.get("img_id"))
        the_document = DocumentsModel.all().filter("__key__ =", key).get()
        #what is greeting.avatar?
        #is it img_id.avatar ?
        #I assume "avatar" refers to the "avatar" property in the model
        if key:
            file_data = blobstore.BlobInfo.get(str(the_document.blobstore_key))
            self.response.headers['Content-Type'] = "image/png"
            #does this display the avatar?
            #I thought the img tag displayed the avatar
            payload = {}
            payload['user_id'] = '1234123412341234'
            payload['test_file'] = MultipartParam('the_file', filename="something",
                                      filetype=file_data.content_type,
                                      fileobj=file_data.open())
        else:
            self.error(404)
          
class SendDocuments(webapp2.RequestHandler):
    def post(self):
        document_key = self.request.get("document_key")
        document_key = Key(str(document_key))
        the_document = DocumentsModel.all().filter("__key__ =", document_key).get()
        file_data = blobstore.BlobInfo.get(str(the_document.blobstore_key))
        payload = {}
        payload['user_id'] = '1234123412341234'
        payload['test_file'] = MultipartParam('the_file', filename="something",
                                      filetype=file_data.content_type,
                                      fileobj=file_data.open())
                                      

#queries and prints
def search(parameter,queryString,response):
    q = ModelInfo.query(parameter.IN([queryString]))
    queryArray = []
    counter = 0
    for p in q.iter(limit=5):
        counter += 1            
        dp = p.to_dict()
        newdp = {}
        #for element in dp:
        #response.write('Description: {0}<br><br>'.format(dp['description']))
        #response.write('{1}: Name: <a href="description?file={2}">{0}</a><br>'.format(dp['name'],counter,dp['name']))
        newdp['link'] = 'description?file={0}'.format(dp['name'])
        newdp['name'] = dp['name']
        newdp['counter'] = counter
        newdp['description'] = dp['description']
        
        #response.write('{0} : {1}<br>'.format(element,printStatement))
        queryArray.append(newdp)
    #response.write('<br>Found {0} results<br>'.format(counter))
    
        
    return queryArray,counter
    
class MyModels(webapp2.RequestHandler):
    def get(self):
        query = ModelInfo.submitter
        queryString = users.get_current_user()
        queryArray,counter = search(query,queryString,self.response)
        
        template_values = boilerplateParams(self.request.uri)
        template_values['counter'] = counter
        template_values['queryArray'] = queryArray
        template_values['myModelsh']  = 'current_page_item'
        template =JINJA_ENVIRONMENT.get_template('resultsList2.html')
        self.response.write(template.render(template_values))

            
class Search(webapp2.RequestHandler):
    def post(self):
        if self.request.get('searchSelection') == 'Key Terms':
            query = ModelInfo.tags
        elif self.request.get('searchSelection') == 'Author':
            query = ModelInfo.author
        elif self.request.get('searchSelection') == 'Biomodels.org URI':
            query = ModelInfo.structuredTags
            
        queryArray,counter = search(query,self.request.get('queryString'),self.response)
        template_values = boilerplateParams(self.request.uri)
        template_values['counter'] = counter
        template_values = boilerplateParams(self.request.uri)
        template_values['queryArray'] = queryArray
        
        template =JINJA_ENVIRONMENT.get_template('resultsList2.html')
        self.response.write(template.render(template_values)) 
        #if counter > 0:
        #    self.response.write(dp)
        #    self.response.write('<br>')

class ServeHandler(blobstore_handlers.BlobstoreDownloadHandler):
  def get(self, resource):
    resource = str(urllib.unquote(self.request.get('key')))
    blob_info = blobstore.BlobInfo.get(resource)
    self.send_blob(blob_info)
            
  
            
app = webapp2.WSGIApplication([
    ('/', MainPage),
    ('/submit',Submit),
    ('/submitFile',SubmitFile),
    ('/submitBatch',SubmitBatch),
    ('/query',Query),
    ('/sign', ModelDB),
    ('/signFile',ModelDBFile),
    ('/signBatch',ModelDBBatch),
    ('/search',Search),
    ('/list',List),
    ('/description',Description),
    ('/myModels',MyModels),
    ('/serve/([^/]+)?', ServeHandler),
    ('/processfileq',ProcessAnnotation),
    ('/image',Image)
], debug=True)