import string
import logging
import datetime
import models
import errors
from google.appengine.api import search
from google.appengine.ext import ndb




class BaseDocumentManager(object):
  """Abstract class. Provides helper methods to manage search.Documents."""

  _INDEX_NAME = None
  _VISIBLE_PRINTABLE_ASCII = frozenset(
    set(string.printable) - set(string.whitespace))

  def __init__(self, doc):
    """Builds a dict of the fields mapped against the field names, for
    efficient access.
    """
    self.doc = doc
    fields = doc.fields

  def getFieldVal(self, fname):
    """Get the value of the document field with the given name.  If there is
    more than one such field, the method returns None."""
    try:
      return self.doc.field(fname).value
    except ValueError:
      return None

  def setFirstField(self, new_field):
    """Set the value of the (first) document field with the given name."""
    for i, field in enumerate(self.doc.fields):
      if field.name == new_field.name:
        self.doc.fields[i] = new_field
        return True
    return False

  @classmethod
  def isValidDocId(cls, doc_id):
    """Checks if the given id is a visible printable ASCII string not starting
    with '!'.  Whitespace characters are excluded.
    """
    for char in doc_id:
      if char not in cls._VISIBLE_PRINTABLE_ASCII:
        return False
    return not doc_id.startswith('!')

  @classmethod
  def getIndex(cls):
    return search.Index(name=cls._INDEX_NAME)

  @classmethod
  def deleteAllInIndex(cls):
    """Delete all the docs in the given index."""
    docindex = cls.getIndex()

    try:
      while True:
        # until no more documents, get a list of documents,
        # constraining the returned objects to contain only the doc ids,
        # extract the doc ids, and delete the docs.
        document_ids = [document.doc_id
                        for document in docindex.get_range(ids_only=True)]
        if not document_ids:
          break
        docindex.delete(document_ids)
    except search.Error:
      logging.exception("Error removing documents:")

  @classmethod
  def getDoc(cls, doc_id):
    """Return the document with the given doc id. One way to do this is via
    the get_range method, as shown here.  If the doc id is not in the
    index, the first doc in the index will be returned instead, so we need
    to check for that case."""
    if not doc_id:
      return None
    try:
      index = cls.getIndex()
      response = index.get_range(
          start_id=doc_id, limit=1, include_start_object=True)
      if response.results and response.results[0].doc_id == doc_id:
        return response.results[0]
      return None
    except search.InvalidRequest: # catches ill-formed doc ids
      return None

  @classmethod
  def removeDocById(cls, doc_id):
    """Remove the doc with the given doc id."""
    try:
      cls.getIndex().delete(doc_id)
    except search.Error:
      logging.exception("Error removing doc id %s.", doc_id)

  @classmethod
  def add(cls, documents):
    """wrapper for search index add method; specifies the index name."""
    try:
      return cls.getIndex().put(documents)
    except search.Error:
      logging.exception("Error adding documents.")


class ModelDoc(BaseDocumentManager):
  MID = 'mid'
  _INDEX_NAME = 'model_doc'
  MODEL_NAME = 'model_name'
  MODEL_AUTHOR = 'model_author'
  MODEL_KEYWORDS = 'model_keywords'
  MODEL_SKEYWORDS = 'model_skeywords'
  MODEL_FILE = 'model_file'
  UPDATED = 'updated'

  _SORT_OPTIONS = [
        [UPDATED, 'modified', search.SortExpression(
            expression=UPDATED,
            direction=search.SortExpression.DESCENDING, default_value=1)],
        [MODEL_AUTHOR, 'model author', search.SortExpression(
            expression=MODEL_AUTHOR,
            direction=search.SortExpression.ASCENDING, default_value='')],
        [MODEL_NAME, 'model name', search.SortExpression(
            expression=MODEL_NAME,
            direction=search.SortExpression.ASCENDING, default_value='zzz')]
      ]

  _SORT_MENU = None
  _SORT_DICT = None

  def getName(self):
    return self.getFieldVal(self.MODEL_NAME)
  def getAuthor(self):
    return self.getFieldVal(self.MODEL_AUTHOR)
  def getKeywords(self):
    return self.getFieldVal(self.MODEL_KEYWORDS)
  def getMID(self):
    return self.getFieldVal(self.MID)
  def getSKeywords(self):
    return self.getFieldVal(self.MODEL_SKEYWORDS)
  def getFile(self):
    return self.getFieldVal(self.MODEL_FILE)

  @classmethod
  def getSortMenu(cls):
    if not cls._SORT_MENU:
      cls._buildSortMenu()
    return cls._SORT_MENU

  @classmethod
  def getSortDict(cls):
    if not cls._SORT_DICT:
      cls._buildSortDict()
    return cls._SORT_DICT

  @classmethod
  def _buildSortMenu(cls):
    """Build the default set of sort options used for Product search.
    Of these options, all but 'relevance' reference core fields that
    all Products will have."""
    res = [(elt[0], elt[1]) for elt in cls._SORT_OPTIONS]
    cls._SORT_MENU = [('relevance', 'relevance')] + res

  @classmethod
  def _buildSortDict(cls):
    """Build a dict that maps sort option keywords to their corresponding
    SortExpressions."""
    cls._SORT_DICT = {}
    for elt in cls._SORT_OPTIONS:
      cls._SORT_DICT[elt[0]] = elt[2]

  @classmethod
  def getDocFromMid(cls, mid):
    """Given a pid, get its doc. We're using the pid as the doc id, so we can
    do this via a direct fetch."""
    return cls.getDoc(mid)


  @classmethod
  def _buildCoreModelFields(
      cls, mid, name, author, keywords,skeywords,fileInfo):
    """Construct a 'core' document field list for the fields common to all
    Models. """
    fields = [search.TextField(name=cls.MID, value=mid),
              # The 'updated' field is always set to the current date.
              search.DateField(name=cls.UPDATED,
                  value=datetime.datetime.now().date()),
              search.TextField(name=cls.MODEL_AUTHOR, value=author),
              search.TextField(name=cls.MODEL_KEYWORDS,value=keywords),
              search.TextField(name=cls.MODEL_SKEYWORDS, value=skeywords),
               search.TextField(name=cls.MODEL_FILE, value=fileInfo)
                ]
    return fields

  @classmethod
  def _createDocument(
      cls, mid, name, author, keywords,skeywords,fileInfo):
      """Create a Document object from given params."""
      # construct the document fields from the params
      resfields = cls._buildCoreModelFields(
          mid=mid, name=name, author=author,keywords=keywords,
          skeywords=skeywords,fileInfo=fileInfo)
      # build and index the document.  Use the pid (product id) as the doc id.
      # (If we did not do this, and left the doc_id unspecified, an id would be
      # auto-generated.)
      d = search.Document(fields=resfields)
      return d


  @classmethod
  def buildProduct(cls, params):
    """Create/update a product document and its related datastore entity.  The
    product id and the field values are taken from the params dict.
    """
    # check to see if doc already exists.  We do this because we need to retain
    # some information from the existing doc.  We could skip the fetch if this
    # were not the case.
    #curr_doc = cls.getDocFromMid(params['mid'])
    d = cls._createDocument(params['mid'],params['name'],' '.join(params['author'])
      ,' '.join(params['tags']),' '.join(params['structuredTags']),params['fileInfo'])

    # This will reindex if a doc with that doc id already exists
    doc_ids = cls.add(d)
    try:
      doc_id = doc_ids[0].id
    except IndexError:
      doc_id = None
      raise errors.OperationFailedError('could not index document')
    logging.debug('got new doc id %s for product: %s', doc_id, params['mid'])
    
    # now update the entity
    def _tx():
      # Check whether the product entity exists. If so, we want to update
      # from the params, but preserve its ratings-related info.
      model = models.ModelInfo.get_by_id(doc_id)
      if model:  #update
        model.update_core(params,doc_id)
      else:   # create new entity
        model = models.ModelInfo.create(params, doc_id)
      #model.put()
      return model
    model = ndb.transaction(_tx)
    logging.debug('prod: %s', model)
    return model