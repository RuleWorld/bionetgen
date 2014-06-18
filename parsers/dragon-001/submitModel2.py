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
from google.appengine.api import search as search2

from google.appengine.ext import ndb
from google.appengine.ext.webapp import blobstore_handlers
from google.appengine.ext import blobstore
from google.appengine.api.images import get_serving_url
from google.appengine.api import taskqueue
import os
from google.appengine.api import app_identity

from google.appengine.ext.webapp import blobstore_handlers
import cloudstorage as gcs

import webapp2
import xmlrpclib
import jinja2
import zipfile
import tempfile
import cPickle as pickle

import json
from google.appengine.api import files
import parseAnnotations

from google.appengine.api import urlfetch
import logging
logging.basicConfig(filename='/home/proto/rulehub.log',level=logging.DEBUG,format='%(asctime)s - %(levelname)s:%(message)s')


from models import ModelInfo
import docs


def CreateFile(filename,content):
  """Create a GCS file with GCS client lib.

  Args:
    filename: GCS filename.

  Returns:
    The corresponding string blobkey for this GCS file.
  """
  # Create a GCS file with GCS client.
  with gcs.open(filename, 'w') as f:
    f.write(content)

  # Blobstore API requires extra /gs to distinguish against blobstore files.
  blobstore_filename = '/gs' + filename
  # This blob_key works with blobstore APIs that do not expect a
  # corresponding BlobInfo in datastore.
  bk =  blobstore.create_gs_key(blobstore_filename)
  if not isinstance(bk,blobstore.BlobKey):
    bk = blobstore.BlobKey(bk)
  return bk

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
                                      deadline=600)
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
            'formatOptions':["bngl","kappa"],
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
        
        
        
        modelSubmission = {}
        if users.get_current_user():
            modelSubmission['submitter'] = users.get_current_user().user_id()
        modelSubmission['author'] = [self.request.get('author')]
        modelSubmission['fileFormat'] = self.request.get('fileFormat')
        modelSubmission['name'] = self.request.get('name')
        modelSubmission['description'] = self.request.get('description')
        modelSubmission['privacy'] = self.request.get('privacy')
        
        modelSubmissionString = pickle.dumps(modelSubmission)
            
        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]
        bnglContent = blob_info.open().read()
        element = blob_info.filename
        bucket_name = os.environ.get('BUCKET_NAME',
                             app_identity.get_default_gcs_bucket_name())

        gcs_filename = '/{1}/{0}'.format(element,bucket_name)
        blob_key = CreateFile(gcs_filename,bnglContent.encode('utf-8'))
        
        taskqueue.add(url='/processfileq', params={'element':element,'bnglKey':blob_key,
                                                        'modelSubmission':modelSubmissionString})
        self.redirect('/')

        #modelSubmission.submitter =  users.get_current_user()        
        

 

#address = 'http://127.0.0.1:9200'
address = 'http://54.214.249.43:9200'
def processAnnotations(bnglContent):
    logging.info('starting annotation processing')
    annotationDict = parseAnnotations.parseAnnotations(bnglContent)
    parsedAnnotationDict = parseAnnotations.dict2DatabaseFormat(annotationDict)
    logging.info(parsedAnnotationDict['structuredTags'])
    print '----',parsedAnnotationDict['structuredTags']
    tagDict = {}
    if parsedAnnotationDict != {}:
        s = xmlrpclib.ServerProxy(address,GAEXMLRPCTransport())
        tagArray = s.resolveAnnotations(parsedAnnotationDict['structuredTags'])
        
        for element in tagArray:
            tagDict[element[0]] = element[1]
        logging.info(tagDict)
    print '+++++',tagDict
    return parsedAnnotationDict,tagDict

def getMap(bnglContent,mapType):
    s = xmlrpclib.ServerProxy(address,GAEXMLRPCTransport())
    mapContent = s.getContactMap(bnglContent,mapType)
    return mapContent

class ModelDBFile(blobstore_handlers.BlobstoreUploadHandler):

    def post(self):
        # We set the same parent key on the 'Greeting' to ensure each greeting
        # is in the same entity group. Queries across the single entity group
        # will be consistent. However, the write rate to a single entity group
        # should be limited to ~1/second.

        
        upload_files = self.get_uploads('file')
        blob_info = upload_files[0]
        bnglContent = blob_info.open().read()
        element = blob_info.filename
        bucket_name = os.environ.get('BUCKET_NAME',
                             app_identity.get_default_gcs_bucket_name())

        gcs_filename = '/{1}/{0}'.format(element,bucket_name)
        blob_key = CreateFile(gcs_filename,bnglContent.encode('utf-8'))
        
        modelSubmission = {}
        if users.get_current_user():
            modelSubmission['submitter'] = users.get_current_user().user_id()
        modelSubmission['author'] = []
        modelSubmission['fileFormat'] = ''
        modelSubmission['name'] = ''
        modelSubmission['description'] = ''
        modelSubmission['privacy'] = self.request.get('privacy')
    
        modelSubmissionString = pickle.dumps(modelSubmission)

        taskqueue.add(url='/processfileq', params={'element':element,'bnglKey':blob_key,
                                                        'modelSubmission':modelSubmissionString})
        self.redirect('/')
  
 
 
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

            modelSubmission = {}
            if users.get_current_user():
                modelSubmission['submitter'] = users.get_current_user().user_id()
            modelSubmission['author'] = []
            modelSubmission['fileFormat'] = ''
            modelSubmission['name'] = ''
            modelSubmission['description'] = ''
            modelSubmission['privacy'] = self.request.get('privacy')
        
            modelSubmissionString = pickle.dumps(modelSubmission)
            
            #store the file in the datastore before passing it to the queue
            bucket_name = os.environ.get('BUCKET_NAME',
                                 app_identity.get_default_gcs_bucket_name())
    
            gcs_filename = '/{1}/{0}'.format(element,bucket_name)
            blob_key = CreateFile(gcs_filename,bnglContent.encode('utf-8'))

            taskqueue.add(url='/processfileq', params={'element':element,'bnglKey':blob_key,
                                                            'modelSubmission':modelSubmissionString})

        self.redirect('/')
 

class ProcessAnnotation(webapp2.RequestHandler):
    def post(self):
            element = self.request.get('element')
            bnglKey = self.request.get('bnglKey')
            bnglContent = blobstore.fetch_data(bnglKey,0,900000)
            modelSubmission = pickle.loads(self.request.get('modelSubmission').encode('utf-8'))
            bucket_name = os.environ.get('BUCKET_NAME',
                                 app_identity.get_default_gcs_bucket_name())
       

            modelSubmission['content'] = blobstore.BlobKey(bnglKey)
            if modelSubmission['name'] == '':
                modelSubmission['name'] = element
            mapInfo = getMap(bnglContent,'contact')

            gcs_filename = '/{1}/{0}.gml'.format(element,bucket_name)
            blob_key = CreateFile(gcs_filename,str(convert(mapInfo['gmlStr'])))

            modelSubmission['contactMap'] = blob_key
            modelSubmission['contactMapJson'] = json.loads(mapInfo['jsonStr'])

            parsedAnnotationDict,tagArray = processAnnotations(bnglContent)
            if 'author' in parsedAnnotationDict:
                modelSubmission['author'] = [parsedAnnotationDict['author']]
                
            modelSubmission['tags'] = []

            if 'structuredTags' in parsedAnnotationDict:
                modelSubmission['structuredTags'] = convert(parsedAnnotationDict['structuredTags'])
            if 'tags' in tagArray:
                modelSubmission['tags'] = convert(tagArray['tags'])

            if 'modelName' in tagArray:
                modelSubmission['name'] = tagArray['modelName'][0].replace(" ","")
                modelSubmission['mid'] = tagArray['modelName'][0].replace(" ","")
            else:
                modelSubmission['mid'] = element
                
            if 'author' in tagArray:
                modelSubmission['author'] = convert(tagArray['author'])

            modelSubmission['fileInfo'] = bnglContent
            #modelObject = ModelInfo.create(modelSubmission,'2')
            modelObject = docs.ModelDoc.buildProduct(modelSubmission)
            #print modelObject
            modelObject.put()

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

class Query2(webapp2.RequestHandler):
  """Displays the 'home' page."""

  def get(self):
        if users.get_current_user():
            url = users.create_logout_url(self.request.uri)
            url_linktext = 'Logout'
            current_user=True
        else:
            url = users.create_login_url(self.request.uri)
            url_linktext = 'Login'
            current_user=False

        sort_info = docs.ModelDoc.getSortMenu()
        template_values = {
             'sort_info': sort_info,
            'url': url,
            'url_linktext': url_linktext,
            'current_user':current_user,
            'queryh':'current_page_item'
        }
        template =JINJA_ENVIRONMENT.get_template('modelResult.html')
        self.response.write(template.render(template_values))
        


class ModelSearchHandler(webapp2.RequestHandler):
  """The handler for doing a product search."""

  _DEFAULT_DOC_LIMIT = 3  #default number of search results to display per page.
  _OFFSET_LIMIT = 1000

  def parseParams(self):
    """Filter the param set to the expected params."""
    params = {
        'qtype': '',
        'query': '',
        'category': '',
        'sort': '',
        'rating': '',
        'offset': '0'
    }
    for k, v in params.iteritems():
      # Possibly replace default values.
      params[k] = self.request.get(k, v)
    return params

  def post(self):
    params = self.parseParams()
    self.redirect('/msearch?' + urllib.urlencode(
        dict([k, v.encode('utf-8')] for k, v in params.items())))

  def _getDocLimit(self):
    """if the doc limit is not set in the config file, use the default."""
    doc_limit = self._DEFAULT_DOC_LIMIT
    try:
      doc_limit = int(100)
    except ValueError:
      logging.error('DOC_LIMIT not properly set in config file; using default.')
    return doc_limit

  def get(self):
    """Handle a product search request."""

    params = self.parseParams()
    self.doProductSearch(params)

  def doProductSearch(self, params):
    """Perform a product search and display the results."""

    # the defined product categories
    #cat_info = models.Category.getCategoryInfo()
    # the product fields that we can sort on from the UI, and their mappings to
    # search.SortExpression parameters
    sort_info = docs.ModelDoc.getSortMenu()
    sort_dict = docs.ModelDoc.getSortDict()
    query = params.get('query', '')
    user_query = query
    doc_limit = self._getDocLimit()

    #categoryq = params.get('category')
    #if categoryq:
      # add specification of the category to the query
      # Because the category field is atomic, put the category string
      # in quotes for the search.
     # query += ' %s:"%s"' % (docs.Product.CATEGORY, categoryq)

    sortq = params.get('sort')
    try:
      offsetval = int(params.get('offset', 0))
    except ValueError:
      offsetval = 0

    # Check to see if the query parameters include a ratings filter, and
    # add that to the final query string if so.  At the same time, generate
    # 'ratings bucket' counts and links-- based on the query prior to addition
    # of the ratings filter-- for sidebar display.
    #query, rlinks = self._generateRatingsInfo(
    #    params, query, user_query, sortq, categoryq)
    logging.debug('query: %s', query.strip())

    #try:
      # build the query and perform the search
    search_query = self._buildQuery(
      query, sortq, sort_dict, doc_limit, offsetval)
    search_results = docs.ModelDoc.getIndex().search(search_query)
    returned_count = len(search_results.results)
    '''
    except search.Error:
      logging.exception("Search error:")  # log the exception stack trace
      msg = 'There was a search error (see logs).'
      url = '/'
      linktext = 'Go to product search page.'
      template =JINJA_ENVIRONMENT.get_template('notification.html')
      self.response.write(template.render({'title': 'Error', 'msg': msg,
           'goto_url': url, 'linktext': linktext}))

     
      return
    ''' 
    # cat_name = models.Category.getCategoryName(categoryq)
    psearch_response = []
    # For each document returned from the search
    for doc in search_results:
      # logging.info("doc: %s ", doc)
      mdoc = docs.ModelDoc(doc)
      # use the description field as the default description snippet, since
      # snippeting is not supported on the dev app server.
      description_snippet = mdoc.getName()
      #price = pdoc.getPrice()
      # on the dev app server, the doc.expressions property won't be populated.
      for expr in doc.expressions:
        if expr.name == docs.ModelDoc.MODEL_NAME:
          description_snippet = expr.value
        # uncomment to use 'adjusted price', which should be
        # defined in returned_expressions in _buildQuery() below, as the
        # displayed price.
        # elif expr.name == 'adjusted_price':
          # price = expr.value

      # get field information from the returned doc
      mid = mdoc.getMID()
      #cat = catname = pdoc.getCategory()
      pname = mdoc.getName()
      author= mdoc.getAuthor()
      tags = mdoc.getKeywords()
      # for this result, generate a result array of selected doc fields, to
      # pass to the template renderer
      psearch_response.append(
          [doc, urllib.quote_plus(mid), 
           description_snippet,  pname, author,tags])
    if not query:
      print_query = 'All'
    else:
      print_query = query

    # Build the next/previous pagination links for the result set.
    (prev_link, next_link) = self._generatePaginationLinks(
        offsetval, returned_count,
        search_results.number_found, params)

    logging.debug('returned_count: %s', returned_count)
    # construct the template values
    template_values = {
        'base_pquery': user_query, 'next_link': next_link,
        'prev_link': prev_link, 'qtype': 'product',
        'query': query, 'print_query': print_query,
        'sort_order': sortq,
        'first_res': offsetval + 1, 'last_res': offsetval + returned_count,
        'returned_count': returned_count,
        'number_found': search_results.number_found,
        'search_response': psearch_response,
        'sort_info': sort_info,
        }
    # render the result page.
    template =JINJA_ENVIRONMENT.get_template('modelResult.html')
    self.response.write(template.render(template_values))

    

  def _buildQuery(self, query, sortq, sort_dict, doc_limit, offsetval):
    """Build and return a search query object."""

    # computed and returned fields examples.  Their use is not required
    # for the application to function correctly.
    #computed_expr = search.FieldExpression(name='adjusted_price',
     #   expression='price * 1.08')
    returned_fields = [docs.ModelDoc.MID, docs.ModelDoc.MODEL_NAME,
                docs.ModelDoc.MODEL_AUTHOR, docs.ModelDoc.MODEL_KEYWORDS,
                docs.ModelDoc.MODEL_SKEYWORDS]

    if sortq == 'relevance':
      # If sorting on 'relevance', use the Match scorer.
      sortopts = search2.SortOptions(match_scorer=search2.MatchScorer())
      search_query = search2.Query(
          query_string=query.strip(),
          options=search2.QueryOptions(
              limit=doc_limit,
              offset=offsetval,
              sort_options=sortopts,
              #snippeted_fields=[docs.Product.DESCRIPTION],
              #returned_expressions=[computed_expr],
              returned_fields=returned_fields
              ))
    else:
      expr_list = [sort_dict.get(sortq)]
      sortopts = search2.SortOptions(expressions=expr_list)
      # logging.info("sortopts: %s", sortopts)
      search_query = search2.Query(
          query_string=query.strip(),
          options=search2.QueryOptions(
              limit=doc_limit,
              offset=offsetval,
              sort_options=sortopts,
              #snippeted_fields=[docs.Product.DESCRIPTION],
              #returned_expressions=[computed_expr],
              returned_fields=returned_fields
              ))
    return search_query

  def _generatePaginationLinks(
        self, offsetval, returned_count, number_found, params):
    """Generate the next/prev pagination links for the query.  Detect when we're
    out of results in a given direction and don't generate the link in that
    case."""

    doc_limit = self._getDocLimit()
    pcopy = params.copy()
    if offsetval - doc_limit >= 0:
      pcopy['offset'] = offsetval - doc_limit
      prev_link = '/psearch?' + urllib.urlencode(pcopy)
    else:
      prev_link = None
    if ((offsetval + doc_limit <= self._OFFSET_LIMIT)
        and (returned_count == doc_limit)
        and (offsetval + returned_count < number_found)):
      pcopy['offset'] = offsetval + doc_limit
      next_link = '/psearch?' + urllib.urlencode(pcopy)
    else:
      next_link = None
    return (prev_link, next_link)


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
    '''
    Lists all the models in the db
    should deprecate for using the new query system
    '''
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
            if dp['privacy'] == 'privacy' and (not users.get_current_user() or dp['submitter'] != users.get_current_user().user_id()):
                continue
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
    '''
    details model description
    '''
    def get(self):
        query = ModelInfo.name
        q = ModelInfo.query(query == self.request.get('file'))
        ndp = {}
        queryArray = []
        
        for p in q.iter():
            dp = p.to_dict()
            print dp.keys()
            print p.key
            for element in dp:
                #self.response.write('{0} : {1}<br>'.format(element,dp[element]))

                if element in ['content']:
                    ndp[element] = ["serve/{1}?key={0}".format(dp[element],dp['name']),'BioNetGen file']
                elif element in ['contactMap']:
                    ndp[element] = ["serve/{1}?key={0}".format(dp[element],dp['name']),'Contact Map in GML format',dp['name']]
                elif element in ['contactMapJson','submitter','doc_id']:
                    continue
                else:
                    if dp[element] in [None,[]]:
                        continue
                    ndp[element] = dp[element]
                '''    
                elif element in ['contactMap']:
                    try:                    
                        blobkey = urllib.unquote(str(dp[element]))
                        url = get_serving_url(blobkey,size=400)
                        url ='serve/{1}?key={0}'.format(dp[element],blobstore.BlobInfo(dp[element]).filename)
                        ndp[element] = url
                    except TypeError:
                        pass
                    #printStatement = '<img src=image?img_id={0}/><br>'.format(dp[element])
                '''
                 
            queryArray.append(ndp)

        template_values = boilerplateParams(self.request.uri)
        template_values['queryArray'] = queryArray
        template_values['listh'] = 'current_page_item'

        template =JINJA_ENVIRONMENT.get_template('singleResult2.html')
        self.response.write(template.render(template_values))

def convert(input):
    '''
    change array/dict of unicode strings to ascii strings
    '''
    if isinstance(input, dict):
        return dict((convert(key), convert(value)) for key, value in input.iteritems())
    elif isinstance(input, list):
        return [convert(element) for element in input]
    elif isinstance(input, unicode):
        return input.encode('utf-8')
    else:
        return input

class Visualize(webapp2.RequestHandler):   
    '''
    calls cytoscape.js to visualize a contact map
    '''
    def get(self):
        query = ModelInfo.name
        q = ModelInfo.query(query == self.request.get('file'))
        model = q.fetch(1)
        model = model[0].to_dict()
        
        template_values = {}
        template_values['graph'] = convert(model['contactMapJson']['elements'])
        template_values['layout'] = convert(model['contactMapJson']['layout'][0])
        template =JINJA_ENVIRONMENT.get_template('visualize.html')
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
        if users.get_current_user():
            queryString = users.get_current_user().user_id()
        else:
            self.redirect('/')
            return
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
    #blob_info = blobstore.BlobInfo.get(resource)
    self.send_blob(resource)
            
  
            
app = webapp2.WSGIApplication([
    ('/', MainPage),
    ('/submit',Submit),
    ('/submitFile',SubmitFile),
    ('/submitBatch',SubmitBatch),
    ('/query',Query2),
    ('/msearch',ModelSearchHandler),
    ('/sign', ModelDB),
    ('/signFile',ModelDBFile),
    ('/signBatch',ModelDBBatch),
    ('/search',Search),
    ('/list',List),
    ('/visualize',Visualize),
    ('/description',Description),
    ('/myModels',MyModels),
    ('/serve/([^/]+)?', ServeHandler),
    ('/processfileq',ProcessAnnotation),
    ('/image',Image)
], debug=True)