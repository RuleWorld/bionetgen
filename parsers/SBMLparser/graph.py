# -*- coding: utf-8 -*-
"""
Created on Mon Oct  7 16:52:49 2013

@author: proto
"""

import pygraphviz as pgv
import pickle
import subprocess


def orBox(bioNumber):
    with open('orBox{0}.dump'.format(bioNumber),'rb') as f:
        boxes = pickle.load(f)        
    fileName = 'image'
    graph = pgv.AGraph(directed=False,concentrate=True)
    for element in boxes[0]:
        graph.add_node(element,shape='diamond',color='pink',style='filled')
        s = set()
        edges = set()
        rootEdges = set()
        for component in boxes[0][element]:
            s.add('{0}'.format(component[0]))
            s.add('{0}'.format(component[1]))
            #if boxes[0][element][component]:
            #    edges.add(('{0}-{1}'.format(element,component[0]),'{0}-{1}'.format(element,component[1])))

            rootEdges.add((element,'{0}-{1}'.format(element,component[0])))
            rootEdges.add((element,'{0}-{1}'.format(element,component[1])))
        for item in s:
            graph.add_node('{0}-{1}'.format(element,item),label=item)
        graph.add_edges_from(edges,color='red',weight=0.05,penwidth=2)
        graph.add_edges_from(rootEdges,weight=1)
    color = ['red','green']
    for idx,box in enumerate(boxes):
        for element in box:
            edges = set()
            for component in box[element]:
                if box[element][component]:
                    edges.add(('{0}-{1}'.format(element,component[0]),'{0}-{1}'.format(element,component[1])))
            graph.add_edges_from(edges,color=color[idx],weight=0.05,penwidth=2)

    graph.write('%s.dot' % fileName)
    subprocess.call(['circo', '-Tpng', '{0}.dot'.format(fileName),'-o{0}.png'.format(fileName)])

        
if __name__ == "__main__":
    orBox(19)

