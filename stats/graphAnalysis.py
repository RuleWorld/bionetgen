import networkx as nx
import numpy as np
import pandas
import os
import fnmatch
import sys

sys.path.insert(0, '.')
sys.path.insert(0, os.path.join('.','SBMLparser'))
import SBMLparser.utils.consoleCommands as consoleCommands


def generateGraph(bngfile,timeout=180,graphtype='regulatory'):
    """
    Generates a bng-xml file via the bng console
    """
    consoleCommands.generateGraph(bngfile,graphtype)

    graphname = '.'.join(bngfile.split('.')[:-1]) + '_{0}.gml'.format(graphtype)
    return graphname

def loadGraph(graphname):
    """
    load GML file as a digraph object from a filename
    """
    graph = nx.read_gml(graphname)
    return graph


def getFiles(directory,extension,abspath=False):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'

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


class ProcessGraph:
    def __init__(self,graph):
        self.graph = graph
        graphics = {x:self.graph.node[x]['graphics']['type'] for x in self.graph.node}
        self.nodes = pandas.DataFrame.from_dict(self.graph.node,orient='index')
        self.nodes['graphics'] = pandas.Series(graphics)
        self.nodes['graphics'] = self.nodes['graphics'].map({'roundrectangle': 'species', 'hexagon': 'process'})

    def entropy(self,dist):
        """
        Returns the entropy of `dist` in bits (base-2).

        """
        dist = np.asarray(dist)
        ent = np.nansum( dist *  np.log2( 1/dist ) )
        return ent

    def centrality_distribution(self,node_type):
        """
        Returns a centrality distribution.

        Each normalized centrality is divided by the sum of the normalized
        centralities. Note, this assumes the graph is simple.

        """
        centrality = self.nodes[self.nodes.graphics == node_type]['centrality'].values
        centrality = np.asarray(centrality)
        centrality /= centrality.sum()
        return centrality

    def centrality(self):
        """
        calculates several measures of node centrality and stores them in the general node table
        """

        centrality = pandas.Series(nx.degree_centrality(self.graph))
        closeness = pandas.Series(nx.closeness_centrality(self.graph))
        indegree = pandas.Series(nx.in_degree_centrality(self.graph))
        outdegree = pandas.Series(nx.out_degree_centrality(self.graph))



        self.nodes['centrality'] =  centrality
        self.nodes['closeness'] = closeness
        self.nodes['indegree'] = indegree
        self.nodes['outdegree'] = outdegree

        #print self.nodes.sort(column='load',ascending=False).head(20)
        #
 

if __name__ == "__main__":
    #bngfile = 'output19raw.bngl'
    #graphname = generateGraph(bngfile)
    graphnames = getFiles('egfr','gml')
    for nodeType in ['process','species']:
        print '-----',nodeType
        for graphname in graphnames:
            graph = loadGraph(graphname)
            process = ProcessGraph(graph)
            process.centrality()
            dist = process.centrality_distribution(node_type=nodeType)
            print graphname, process.entropy(dist)
