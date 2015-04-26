import networkx as nx
from networkx.algorithms import bipartite
import numpy as np
import pandas
import os
import fnmatch
import sys
import yaml
import scipy
sys.path.insert(0, '.')
sys.path.insert(0, os.path.join('.','SBMLparser'))
#import SBMLparser.utils.consoleCommands as consoleCommands
import concurrent.futures
import multiprocessing as mp
import progressbar
import argparse

import os.path

def loadGraph(graphname):
    """
    load GML file as a digraph object from a filename
    """
    graph = nx.read_gml(graphname)
    return graph


def getFiles(directory,extension,abspath=False):
    """
    Gets a list of <extension> files that could be correctly translated in a given 'directory'

    Keyword arguments:
    directory -- The directory we will recurseviley get files from
    extension -- A file extension filter
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            filepath = os.path.join(root, filename)
            if abspath:
                filepath = os.path.abspath(filepath)
            matches.append([filepath,os.path.getsize(os.path.join(root, filename))])

    #sort by size
    #matches.sort(key=lambda filename: filename[1], reverse=False)
    
    matches = [x[0] for x in matches]

    return matches

from collections import Counter

class ProcessGraph:
    """
    creates a pandas dataframe from a gml file containing several graph metrics
    like centrality and wiener index
    """
    def __init__(self,graph):
        self.graph = graph
        graphics = {x:self.graph.node[x]['graphics']['type'] for x in self.graph.node}
        #print {x:self.graph.node[x]['graphics'] for x in self.graph.node}
        self.nodes = pandas.DataFrame.from_dict(self.graph.node,orient='index')
        self.nodes['graphics'] = pandas.Series(graphics)
        self.nodes['graphics'] = self.nodes['graphics'].map({'roundrectangle': 'species', 'hexagon': 'process'})
        
    def entropy(self,dist):
        """
        Returns the entropy of `dist` in bits (base-2).

        """
        dist = np.asarray(dist)
        ent = np.nansum( dist *  np.log2( 1/dist ) )/np.log2(len(dist))
        return ent

    def centrality_distribution(self,node_type):
        """
        Returns a centrality distribution.

        Each normalized centrality is divided by the sum of the normalized
        centralities. Note, this assumes the graph is simple.

        """
        centrality = self.nodes[self.nodes.graphics == node_type]['degree'].values
        centrality = np.asarray(centrality)
        centrality /= centrality.sum()
        return centrality

    def removeContext(self):
        context2 = []
        context3 = []
        edges = self.graph.edges(data=True)
        for source,destination,data in edges:
                if 'graphics' in data:
                    if data['graphics']['fill'] ==  u'#798e87':
                        context2.append((source,destination))
                else:
                    for idx in data:
                        if 'graphics' in data[idx]:
                            if data[idx]['graphics']['fill'] ==  u'#798e87':
                                context3.append((source,destination,idx))
        self.graph.remove_edges_from(context2)
        self.graph.remove_edges_from(context3)

    def wiener(self):
        g2 = nx.Graph(self.graph)
        speciesnodes =  set(n for n, d in self.graph.nodes(data=True) if d['graphics']['type']=='roundrectangle')
        wienerIndex = []
        connected = 0
        for node1 in speciesnodes:
            wiener = 0
            for node2 in speciesnodes:
                if node1 == node2:
                    continue
                try:
                    wiener += len(nx.shortest_path(g2,node1,node2)) - 1
                    connected += 1
                except nx.exception.NetworkXNoPath:
                    continue
            wienerIndex.append(wiener)
        if connected ==0:
            return 0,1
        return sum(wienerIndex)*1.0/connected,self.entropy(np.asarray(wienerIndex)*1.0/sum(wienerIndex))

    def graphMeasures(self):
        """
        calculates several graph measures
        """

        #average_degree_connectivity = nx.average_degree_connectivity(self.graph)
        #average_neighbor_degree = nx.average_neighbor_degree(self.graph)
        average_node_connectivity = nx.average_node_connectivity(self.graph)
        #average_node_connectivity =  1
        return [average_node_connectivity]

    def centrality(self):
        """
        calculates several measures of node centrality and stores them in the general node table
        """
        speciesnodes =  set(n for n, d in self.graph.nodes(data=True) if d['graphics']['type']=='roundrectangle')

        g2 = nx.Graph(self.graph)
        self.nodes['degree'] = pandas.Series(nx.degree_centrality(self.graph))
        self.nodes['closeness'] = pandas.Series(nx.closeness_centrality(self.graph))
        self.nodes['betweenness'] = pandas.Series(nx.betweenness_centrality(self.graph))
        self.nodes['communicability'] = pandas.Series(nx.communicability_centrality(g2))

        #print self.nodes.sort(column='load',ascending=False).head(20)
        #
 
def generateGraph(bngfile,timeout=180,graphtype='regulatory',options = []):
    """
    Generates a bng-xml file via the bng console
    """
    #consoleCommands.generateGraph(bngfile,graphtype,options)

    graphname = '.'.join(bngfile.split('.')[:-1]) + '_{0}.gml'.format(graphtype)
    graphname = graphname.split('/')[-1]
    return graphname


def getGraphEntropy(graphname,nodeType):
    """
    given a filename pointing to a gml file it will return a series of metrics describing
    the properties of the graph
    """
    #try:
    graph = loadGraph(graphname)
    process = ProcessGraph(graph)
    #process.removeContext()
    try:
        process.centrality()
        dist = process.centrality_distribution(node_type=nodeType)
        centropy = process.entropy(dist)
        #centropy = 1
    except ZeroDivisionError:
        centropy = 1
    #print process.wiener()

    #return graphname,nodeType,process.wiener(),centropy,process.graphMeasures(),
    #[len(process.nodes[process.nodes.graphics =='process']),len(process.nodes[process.nodes.graphics=='species']),len(graph.edges)]

    return {'graphname':graphname,'nodeType':nodeType,
            'wiener':process.wiener(),'centropy':centropy,
            'measures': process.graphMeasures(),
            'graphstats':[len(process.nodes[process.nodes.graphics =='process']),len(process.nodes[process.nodes.graphics=='species']),len(graph.edges())]
    }
#except:
    #    return graphname,nodeType,-1

import shutil

def createGMLFiles(directory,options):
    bngfiles= getFiles(directory,'bngl')
    for bngfile in bngfiles:
        for option in options:
            graphname = generateGraph(bngfile,options = options[option])
            shutil.move(graphname, os.path.join(directory,option))    

def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-s','--settings',type=str,help='settings file')
    parser.add_argument('-o','--output',type=str,help='output directory')
    return parser    

def loadFilesFromYAML(yamlFile):
    with open(yamlFile,'r') as f:
        yamlsettings = yaml.load(f)

    print yamlsettings
    return yamlsettings

def getEntropyMeasures(graphnames):
    """
    batch process returns a distribution of metrics for fileset <graphnames>
    """
    futures = []
    workers = mp.cpu_count()-1
    results = pandas.DataFrame()
    with concurrent.futures.ProcessPoolExecutor(max_workers=workers) as executor:
        for gidx in range(len(graphnames)):
            futures.append(executor.submit(getGraphEntropy,graphnames[gidx],'species'))
        for future in concurrent.futures.as_completed(futures,timeout=3600):
            partialResults = future.result()
            row = partialResults['graphname'].split('/')[-1]
            column = partialResults['graphname'].split('/')[-3]
            results = results.set_value(row,column + '_wiener',partialResults['wiener'][0])
            results = results.set_value(row,column + '_entropy',partialResults['wiener'][1])
            results = results.set_value(row,column + '_ccentropy',partialResults['centropy'])
            results = results.set_value(row,column + '_nconn',partialResults['measures'][0])
            results = results.set_value(row,column + '_nprocess',partialResults['graphstats'][0])
            results = results.set_value(row,column + '_nspecies',partialResults['graphstats'][1])
            results = results.set_value(row,column + '_nedges',partialResults['graphstats'][2])
    return results     

if __name__ == "__main__":

    parser = defineConsole()
    namespace = parser.parse_args()
    if namespace.settings != None:
        settings = loadFilesFromYAML(namespace.settings)
        graphnames = settings['inputfiles']
        outputdirectory = namespace.output
        outputfile = 'entropy_{0}.h5'.format(namespace.settings.split('/')[-1].split('.')[-2])
    else:
        graphnames = getFiles('egfr/collapsed_contact','gml')
        outputdirectory = 'egfr'
        outputfile = 'entropy_test.h5'

    #bngfile = 'egfr_net.bngl'
    #createGMLFiles('egfr',options)
    nodeTypes = ['species','process']
    results = getEntropyMeasures(graphnames)
    results.to_hdf(os.path.join(outputdirectory,outputfile),'entropy')
    #raise Exception
