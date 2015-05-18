import urllib
import urllib2
from bioservices import UniProt
import functools
import marshal

u = UniProt(verbose=False)

def memoize(obj):
    cache = obj.cache = {}

    @functools.wraps(obj)
    def memoizer(*args, **kwargs):
        #key = str(args) + str(kwargs)
        key = marshal.dumps([args,kwargs])
        if key not in cache:
            cache[key] = obj(*args, **kwargs)
        return cache[key]
    return memoizer

def name2uniprot(nameStr):
    """
    get the uniprot id for a given biologican name. gives preference to human data
    """
    data = u.search('{0}+AND+organism:9606'.format(nameStr), limit=5,columns="entry name,id")
    
    if len(data) == 0:
        data = u.search('{0}'.format(nameStr), limit=10,columns="entry name,id")
    parsedData = [x.split('\t') for x in data.split('\n')][1:]
    if len([x for x in parsedData if nameStr in x[0]]) > 0:
        return [x[1] for x in parsedData if nameStr in x[0]]
    return [x[1] for x in parsedData if len(x) == 2]



def getReactomeBondByUniprot(uniprot1,uniprot2):
    """
    Queries reactome to see if two proteins references by their uniprot id
    are bound in the same complex
    """
    url = 'http://www.pathwaycommons.org/pc2/graph'
    source = '&'.join(['source={0}'.format(x) for x in uniprot1])
    target = '&'.join(['target={0}'.format(x) for x in uniprot2])
    xparams = '{0}&{1}&kind=PATHSFROMTO&format=EXTENDED_BINARY_SIF'.format(source,target)

    #query reactome
    try:
        response = urllib2.urlopen(url,xparams).read()
    except urllib2.HTTPError:
        return None

    #divide by line
    parsedResponse = [x.split('\t') for x in response.split('\n')]

    #response is divided in two  sections. actual protein-protein relationships and protein descriptors
    separation = [i for i,x in enumerate(parsedResponse) if len(x) < 2]
    
    #separate the first half and focus on actual ppi entries
    ppi = [x for x in parsedResponse[:separation[0]] if x[1] == 'in-complex-with']
    #ppi = [x for x in parsedResponse[:separation[0]]]
    #get protein descriptors and filter by the initial uniprot id given in the method parameters
    includedElements =  [[x[0],x[-1]] for x in parsedResponse[separation[0]:]]
    includedElements1 = [x for x in includedElements if any(y in x[1] for y in uniprot1)]
    includedElements2 = [x for x in includedElements if any(y in x[1] for y in uniprot2)]
    includedElements1 =  [x[0] for x in includedElements1]
    includedElements2 =  [x[0] for x in includedElements2]

    #filer protein interaction by those uniprot ids and names we truly care about
    ppi = [x[0:3] for x in ppi if len([y for y in includedElements1 if y in x]) == 1 and len([y for y in includedElements2 if y in x]) == 1]
    return ppi


@memoize
def getReactomeBondByName(name1,name2):
    """
    resolves the uniprot id of parameters *name1* and *name2* and obtains whether they
    can be bound in the same complex or not based on reactome information
    """
    uniprot1 = name2uniprot(name1)
    uniprot2 = name2uniprot(name2)
    return getReactomeBondByUniprot(uniprot1,uniprot2)

def isInComplexWith(name1,name2):
    nameset = sorted([name1,name2])
    result = getReactomeBondByName(nameset[0],nameset[1])
    if result:
        return any([x[1]== 'in-complex-with' for x in  result])
    return False

if __name__ == "__main__":
    #results =  isInComplexWith('GAP','Ras')
    print getReactomeBondByName('EGFR', 'Grb2')
    #print results
    #print getReactomeBondByUniprot('P20936','P01112')
