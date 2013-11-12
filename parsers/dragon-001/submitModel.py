# -*- coding: utf-8 -*-
"""
Created on Mon May 27 20:32:52 2013

@author: proto
"""
import unicodedata

import ast
import urllib
import os
from google.appengine.ext.db import Key
from google.appengine.api import users
from google.appengine.ext import ndb
from google.appengine.ext.webapp import blobstore_handlers
from google.appengine.ext import blobstore
from google.appengine.api.images import get_serving_url
import webapp2
from zipfile import ZipFile
import tempfile
from google.appengine.api import files

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
    authorEmail = ndb.StringProperty()
    content = ndb.BlobKeyProperty() #BlobInfo(blobkey)
    contactMap = ndb.BlobKeyProperty()
    name = ndb.StringProperty()
    description = ndb.StringProperty()
    date = ndb.DateTimeProperty(auto_now_add=True)
    tags = ndb.StringProperty(repeated=True)
    publication = ndb.StructuredProperty(PublicationInfo)
    fileFormat = ndb.StringProperty(choices=set(["bngl","kappa"]))
    submitter = ndb.UserProperty()
    annotationInfo = ndb.KeyProperty(kind=AnnotationInfo,repeated=True)
    privacy = ndb.StringProperty()
    

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

        menuHighlight = {'HomePage':'current_page_item','List':'','Query':'',
                         'Submit':'','Login':''}
    
        template_values ={
            'url': url,
            'url_linktext': url_linktext,
            'current_user':current_user,
            'highlight':menuHighlight
        }
        template =JINJA_ENVIRONMENT.get_template('index2.html')
        self.response.write(template.render(template_values))

class Submit(webapp2.RequestHandler):

    def get(self):
        submissionType =  self.request.get('type')
        if submissionType=='form':
            upload_url = blobstore.create_upload_url('/sign')
            include_url='submit.html'
        elif submissionType=='file':
            upload_url = blobstore.create_upload_url('/signFile')
            include_url = 'submitFile.html'
        elif submissionType == 'archive':
            upload_url = blobstore.create_upload_url('/signBatch')
            include_url = 'submitBatch.html'
            
        
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

        menuHighlight = {'Submit':'current_page_item'}
        
        template_values = {
            'action':  upload_url,
            #'models': models,
            #'model_name': urllib.urlencode({'model_name':model_name}),
            'url': url,
            'url_linktext': url_linktext,
            'formatOptions':set(["bngl","kappa"]),
            'current_user':current_user,
            'highlight':menuHighlight,
            'type':include_url
        }

        template = JINJA_ENVIRONMENT.get_template('submit2.html')
        self.response.write(template.render(template_values))

class ModelDBBatch(blobstore_handlers.BlobstoreUploadHandler):
    def post(self):
        
        

        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]

        reader = blob_info.open()
        content =  reader.read()
        tmp = tempfile.TemporaryFile()
        tmp.write(content)
        
        objZip = ZipFile(tmp,'r')
        filelist = objZip.namelist()
        for element in [x for x in filelist if x.endswith('bngl')]:
            f = objZip.open(element)
            content = f.read()

            file_name = files.blobstore.create(mime_type='application/octet-stream')
        
            with files.open(file_name, 'a') as f:
              f.write(content)
            files.finalize(file_name)
            blob_key = files.blobstore.get_blob_key(file_name)
            blob_key2 = None
            
            if element + '.png' in filelist:
                f = objZip.open(element + '.png')
                content2 = f.read()

                file_name2 = files.blobstore.create(mime_type='application/octet-stream')
                with files.open(file_name2,'ab') as f:
                    f.write(content2)
                files.finalize(file_name2)
                blob_key2 = files.blobstore.get_blob_key(file_name2)
            

            fields = self.importBNGLString(content)
            model_name = self.request.get('model_name',
                                              DEFAULT_GUESTBOOK_NAME)
            modelSubmission = ModelInfo(parent=dbmodel_key(model_name))
            print fields
            if 'creatorName' in fields:
                modelSubmission.author = fields['creatorName']
                modelSubmission.authorEmail= fields['creatorEmail']
            if 'notes' in fields:
                modelSubmission.description = fields['notes']
                                        

            modelSubmission.name = element 
            modelSubmission.fileFormat = 'bngl'
            modelSubmission.privacy = self.request.get('privacy')
            modelSubmission.submitter =  users.get_current_user()
            modelSubmission.content = blob_key
            if blob_key2 != None:
                modelSubmission.contactMap = blob_key2
            modelSubmission.submitter =  users.get_current_user()        
     
            tmp  = []
            for field in fields:
                try:
                    tmpField = ast.literal_eval(fields[field])
                except SyntaxError:
                    continue
                if type(tmpField) == list:
                    tmp.extend(tmpField)
            modelSubmission.tags = tmp
            print modelSubmission
            modelSubmission.put()

            
    def importBNGLString(self,content):
        content = content.split('\n')
        fields = {}
        try:
            start = content.index('###')
            end = content[start+1:].index('###')
        except ValueError:
            return []
        comments = content[start+1:end+1]
        for comment in comments:
            try:
                tmp = comment.split(':')
                header = tmp[0].split('@')[1]
                tmp[1] = ':'.join(tmp[1:])
                message = tmp[1].encode('ascii', 'ignore')
                fields[header] = message
            except:
                continue
        return fields

        
class ModelDBFile(blobstore_handlers.BlobstoreUploadHandler):
    def post(self):
        model_name = self.request.get('model_name',
                                          DEFAULT_GUESTBOOK_NAME)
        modelSubmission = ModelInfo(parent=dbmodel_key(model_name))

        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]
        contact = self.get_uploads('contact')
        blob_info2 = contact[0]

        reader = blob_info.open()
        content =  reader.read()
        fields = self.importBNGLString(content)
        print fields
        if 'creatorName' in fields:
            modelSubmission.author = fields['creatorName']
        if 'notes' in fields:
            modelSubmission.description = fields['notes']
        
        modelSubmission.name = blob_info.filename
        modelSubmission.fileFormat = 'bngl'
        modelSubmission.privacy = self.request.get('privacy')
        modelSubmission.submitter =  users.get_current_user()
        modelSubmission.content = blob_info.key()    
        modelSubmission.contactMap = blob_info2.key() 
        modelSubmission.submitter =  users.get_current_user()        
 
        tmp  = []
        for field in fields:
            try:
                tmpField = ast.literal_eval(fields[field])
            except SyntaxError:
                continue
            if type(tmpField) == list:
                tmp.extend(tmpField)
        modelSubmission.tags = tmp
        print modelSubmission
        modelSubmission.put()
        

        query_params = {'model_name': model_name}
        self.redirect('/?' + urllib.urlencode(query_params))


    def importBNGLString(self,content):
        content = content.split('\n')
        fields = {}
        try:
            start = content.index('###')
            end = content[start+1:].index('###')
        except ValueError:
            return []
        comments = content[start+1:end+1]
        for comment in comments:
            try:
                tmp = comment.split(':')
                header = tmp[0].split('@')[1]
                tmp[1] = ':'.join(tmp[1:])
                message = tmp[1].encode('ascii', 'ignore')
                fields[header] = message
            except:
                continue
        return fields

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
        contact = self.get_uploads('contact')
        blob_info = upload_files[0]
        blob_info2 = contact[0]
        modelSubmission.content = blob_info.key()
        modelSubmission.author = self.request.get('author')
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
    
        menuHighlight = {'Query':'current_page_item'}

        template_values ={
            'url': url,
            'url_linktext': url_linktext,
            'queryOptions':set(['Author','Publication']),
            'current_user':current_user,
            'highlight':menuHighlight

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
        output = ''
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
            current_user=True
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
            current_user=False

        for p in q.iter():
            
            dp = p.to_dict()
            #only display public models
            if dp['privacy'] == 'privacy' and dp['submitter'] != users.get_current_user():
                continue
            counter += 1            
            output += '{1}: Name: <a href="description?file={2}">{0}</a><br>'.format(dp['name'],counter,dp['name'])
            output += 'Description: {0}<br><br>'.format(dp['description'])
            newdp = {}
            #for element in dp:
            #    if element in ['content','simulationResults']:
            #        printStatement = blobstore.BlobInfo(dp[element]).filename
            #    else:
            #        printStatement = dp[element]
            #    newdp[element] = printStatement
                
            #    self.response.write('{0} : {1}<br>'.format(element,printStatement))
            queryArray.append(newdp)
        output += '<br><br>Found {0} results<br>'.format(counter)
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'  
        menuHighlight = {'List':'current_page_item'}
        template_values ={
            'url': url,
            'url_linktext': url_linktext,
            'output':output,
            'current_user':current_user,
            'highlight':menuHighlight
        }
        template =JINJA_ENVIRONMENT.get_template('results2.html')   
        self.response.write(template.render(template_values))

class Description(webapp2.RequestHandler):
    def get(self):
        query = ModelInfo.name
        q = ModelInfo.query(query == self.request.get('file'))
        for p in q.iter():
            dp = p.to_dict()
            for element in dp:
                #self.response.write('{0} : {1}<br>'.format(element,dp[element]))
                try:
                    if element in ['content']:
                        printStatement = '<a href="serve/{1}?key={0}">{1}</a>'.format(dp[element],dp['name'])
                    elif element in ['contactMap']:
                        
                            blobkey = urllib.unquote(str(dp[element]))
                            url = get_serving_url(blobkey,size=400)
                            url =' "serve/{1}?key={0}"'.format(dp[element],blobstore.BlobInfo(dp[element]).filename)                  
                            printStatement = '<img src={0} /><br>'.format(url)
                         #printStatement = '<img src=image?img_id={0}/><br>'.format(dp[element])
                    else:
                        printStatement = dp[element]
                except TypeError:
                    continue
                if printStatement != None:
                    try:
                        self.response.write('{0}:{1}<br>'.format(element.decode('ascii', 'ignore'),printStatement))
                    except UnicodeEncodeError:
                        self.response.write('{0}:{1}<br>'.format(element.decode('ascii', 'ignore'),unicodedata.normalize('NFKD', printStatement).encode('ascii','ignore')))

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
    q = ModelInfo.query(parameter == queryString)
    queryArray = []
    counter = 0
    for p in q.iter(limit=5):
        counter += 1            
        dp = p.to_dict()
        newdp = {}
        #for element in dp:
        response.write('{1}: Name: <a href="description?file={2}">{0}</a><br>'.format(dp['name'],counter,dp['name']))
        response.write('Description: {0}<br><br>'.format(dp['description']))
            
            #response.write('{0} : {1}<br>'.format(element,printStatement))
        queryArray.append(newdp)
    response.write('<br>Found {0} results<br>'.format(counter))
    return queryArray,counter
    
class MyModels(webapp2.RequestHandler):
    def get(self):
        query = ModelInfo.submitter
        queryString = users.get_current_user()
        queryArray,counter = search(query,queryString,self.response)

            
class Search(webapp2.RequestHandler):
    def post(self):
        if self.request.get('searchSelection') == 'Publication':
            query = ModelInfo.publication
        elif self.request.get('searchSelection') == 'Author':
            query = ModelInfo.author
            
        queryArray,counter = search(query,self.request.get('queryString'),self.response)
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
    ('/query',Query),
    ('/sign', ModelDB),
    ('/signFile',ModelDBFile),
    ('/signBatch',ModelDBBatch),
    ('/search',Search),
    ('/list',List),
    ('/description',Description),
    ('/myModels',MyModels),
    ('/serve/([^/]+)?', ServeHandler),
    ('/image',Image)
    
], debug=True)