import urllib
import urllib2
import functools
import marshal
from util import logMess

def memoize(obj):
    cache = obj.cache = {}

    @functools.wraps(obj)
    def memoizer(*args, **kwargs):
        #key = str(args) + str(kwargs)
        key = marshal.dumps([str(obj), args, kwargs])
        if key not in cache:
            cache[key] = obj(*args, **kwargs)
        return cache[key]
    return memoizer

'''
from bioservices import UniProt
u = UniProt(verbose=False)
@memoize
def name2uniprot(nameStr):
    """
    get the uniprot id for a given biological name. gives preference to human data
    """
    data = u.search('{0}+AND+organism:9606'.format(nameStr), limit=5, columns="entry name,id")

    if len(data) == 0:
        data = u.search('{0}'.format(nameStr), limit=10,columns="entry name,id")
    parsedData = [x.split('\t') for x in data.split('\n')][1:]
    if len([x for x in parsedData if nameStr in x[0]]) > 0:
        return [x[1] for x in parsedData if nameStr in x[0]]
    return [x[1] for x in parsedData if len(x) == 2]
'''


@memoize
def name2uniprot(nameStr, organism):
    url = 'http://www.uniprot.org/uniprot/?'

    response = None
    if organism:
        organismExtract = list(organism)[0].split('/')[-1]
        xparams = 'query={0}+AND+organism:{1}&columns=entry name,id&format=tab&limit=5&sort=score'.format(nameStr, organismExtract)
        try:
            response = urllib2.urlopen(url, xparams).read()
        except urllib2.HTTPError:
            logMess('ERROR:pathwaycommons', 'A connection could not be established to uniprot')
            return None

    if response in ['', None]:
        url = 'http://www.uniprot.org/uniprot/?'
        xparams = 'query={0}&columns=entry name,id&format=tab&limit=5&sort=score'.format(nameStr)
        try:
            response = urllib2.urlopen(url, xparams).read()
        except urllib2.HTTPError:
            return None
    parsedData = [x.split('\t') for x in response.split('\n')][1:]
    return [x[1] for x in parsedData if len(x) == 2 and any(nameStr.lower() in z for z in [y.lower() for y in x[0].split('_')])]

@memoize
def getReactomeBondByUniprot(uniprot1, uniprot2):
    """
    Queries reactome to see if two proteins references by their uniprot id
    are bound in the same complex
    """
    url = 'http://www.pathwaycommons.org/pc2/graph'
    source = '&'.join(['source={0}'.format(x) for x in uniprot1])
    target = '&'.join(['target={0}'.format(x) for x in uniprot2])
    xparams = '{0}&{1}&kind=PATHSFROMTO&format=EXTENDED_BINARY_SIF'.format(source, target)
    # query reactome
    try:
        response = urllib2.urlopen(url, xparams).read()
    except urllib2.HTTPError:
        #logMess('ERROR:pathwaycommons','A connection could not be established to pathwaycommons')
        return None
    # divide by line
    parsedResponse = [x.split('\t') for x in response.split('\n')]

    # response is divided in two  sections. actual protein-protein relationships and protein descriptors
    separation = [i for i, x in enumerate(parsedResponse) if len(x) < 2]
    
    # separate the first half and focus on actual ppi entries
    ppi = [x for x in parsedResponse[:separation[0]] if x[1] == 'in-complex-with']
    #ppi = [x for x in parsedResponse[:separation[0]]]
    # get protein descriptors and filter by the initial uniprot id given in the method parameters
    includedElements = [[x[0], x[-1]] for x in parsedResponse[separation[0]:]]
    includedElements1 = [x for x in includedElements if any(y in x[1] for y in uniprot1)]
    includedElements2 = [x for x in includedElements if any(y in x[1] for y in uniprot2)]
    includedElements1 = [x[0] for x in includedElements1]
    includedElements2 = [x[0] for x in includedElements2]
    # filter protein interaction by those uniprot ids and names we truly care about
    ppi = [x[0:3] for x in ppi if (len([y for y in includedElements1 if y == x[0]]) == 1 and len([y for y in includedElements2 if y == x[2]]) == 1) \
                                or (len([y for y in includedElements1 if y == x[2]]) == 1 and len([y for y in includedElements2 if y == x[0]]) == 1)]
    #ppi = [x[0:3] for x in ppi if len([y for y in includedElements1 if y in x]) == 1 and len([y for y in includedElements2 if y in x]) == 1]    
    return ppi

@memoize
def getReactomeBondByName(name1, name2, sbmlURI, sbmlURI2, organism=None):
    """
    resolves the uniprot id of parameters *name1* and *name2* and obtains whether they
    can be bound in the same complex or not based on reactome information
    """
    if len(sbmlURI) > 0:
        uniprot1 = [x.split('/')[-1] for x in sbmlURI]
    else:
        uniprot1 = name2uniprot(name1, organism)
    if len(sbmlURI2) > 0:
        uniprot2 = [x.split('/')[-1] for x in sbmlURI2]
    else:
        uniprot2 = name2uniprot(name2, organism)
    uniprot1 = uniprot1 if uniprot1 else [name1]
    uniprot2 = uniprot2 if uniprot2 else [name2]
    return getReactomeBondByUniprot(uniprot1, uniprot2)


def isInComplexWith(name1, name2, sbmlURI=[], sbmlURI2=[], organism=None):
    nameset = sorted([name1, name2], key=lambda x: x[0])
    result = None
    retry = 0
    while retry < 3:
        result = getReactomeBondByName(nameset[0][0], nameset[1][0], nameset[0][1], nameset[1][1], organism)
        retry += 1
        if result:
            return any([x[1] == 'in-complex-with' for x in result])
    return False

if __name__ == "__main__":
    #results =  isInComplexWith('GAP','Ras')
    print getReactomeBondByName('EGFR', 'EGFR', ['Q9QX70'], ['Q9QX70'])
    #print getReactomeBondByName('EGF', 'EGF', ['P07522'], ['P07522'])
    #print name2uniprot('MEKK1')
    #print results
    #print getReactomeBondByUniprot('Q9QX70','Q9QX70')
    #print getReactomeBondByUniprot('P07522','P07522')
