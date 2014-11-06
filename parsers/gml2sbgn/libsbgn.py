# -*- coding: utf-8 -*-
"""
Created on Tue Nov  4 15:38:55 2014

@author: proto
"""

from lxml import etree

def createSBNG_ER(molecules):
    page = etree.Element('sbgn')    
    doc = etree.ElementTree(page)
    sbgnmap = etree.SubElement(page, 'map')
    for idx,molecule in enumerate(molecules):
        glyphtag = etree.SubElement(sbgnmap, 'glyph',id='id{0}'.format(idx+1),class2='entity')
        label = etree.SubElement(glyphtag, 'label',text=molecule.name)
        for idx2,component in enumerate(molecule.components):
            component = etree.SubElement(glyphtag, 'glyph',
                                         class2="state variable",id="id{0}.{1}".format(idx,idx2))
            value = '~'.join([x for x in component.states])
            if value != '':
                clabel = etree.SubElement(component,'state',value=value,variable=component.name)
            else:
                clabel = etree.SubElement(component,'state',variable=component.name)
            
from collections import defaultdict
def createSBNG_ER_gml(graph):
    page = etree.Element('sbgn',xmlns="http://sbgn.org/libsbgn/0.2")    
    doc = etree.ElementTree(page)
    tagDictionary = {}
    stateDictionary = {}
    sbgnmap = etree.SubElement(page, 'map',language="entity relationship")
    gidx=0
    for nodeId in (graph.nodes()):
        if 'gid' not in graph.node[nodeId]:
            glyphtag = etree.SubElement(sbgnmap, 'glyph',
                                id='id{0}'.format(nodeId))    
            glyphtag.set('class','entity')
            etree.SubElement(glyphtag, 'label',text=graph.node[nodeId]['LabelGraphics']['text'])
            etree.SubElement(glyphtag, 'bbox',x='0', y="0", w="120", h="60")
            tagDictionary[nodeId] = glyphtag
    for nodeId in (graph.nodes()):
        if 'gid' in graph.node[nodeId]:
            if 'isGroup' in graph.node[nodeId]:
                glyphtag = tagDictionary[graph.node[nodeId]['gid']]
                component = etree.SubElement(glyphtag, 'glyph',
                                             id="id{0}".format(nodeId))
                component.set('class',"state variable")

                clabel = etree.SubElement(component,'state',variable=graph.node[nodeId]['LabelGraphics']['text'])
                etree.SubElement(component, 'bbox',x='0', y="0", w="54", h="15")

                tagDictionary[nodeId] = component
                
    for nodeId in (graph.nodes()):
        if 'gid' in graph.node[nodeId] and 'isGroup' not in graph.node[nodeId]:
            glyphtag = tagDictionary[graph.node[nodeId]['gid']]
            if glyphtag.get('class') == 'entity':
                component = etree.SubElement(glyphtag, 'glyph',
                                             id="id{0}".format(nodeId))
                component.set('class',"state variable")

                clabel = etree.SubElement(component,'state',variable=graph.node[nodeId]['LabelGraphics']['text'])
                etree.SubElement(component, 'bbox',x='0', y="0", w="54", h="15")

            elif glyphtag.get('class') == 'state variable':
                component = etree.SubElement(sbgnmap, 'glyph',
                                             id="id{0}".format(nodeId))
                component.set('class',"variable value")
                
                clabel = etree.SubElement(component,'label',text=graph.node[nodeId]['LabelGraphics']['text'])
                arctag = etree.SubElement(sbgnmap, 'arc')
                arctag.set('class','assignment')        
                arctag.set('source','id{0}'.format(nodeId))
                arctag.set('target','id{0}'.format(graph.node[nodeId]['gid']))
                arctag.set('id','as{0}'.format(gidx))
                etree.SubElement(arctag,'start',x='0',y='0')
                etree.SubElement(arctag,'end',x='0',y='0')

                etree.SubElement(component, 'bbox',x='0', y="0", w="20", h="20")

                gidx+=1

    idx = 0
    for edgeId in (graph.edges()):
        arctag = etree.SubElement(sbgnmap, 'arc')
        arctag.set('class','assignment')        
        arctag.set('source','id{0}'.format(edgeId[0]))
        arctag.set('target','id{0}'.format(edgeId[1]))
        arctag.set('id','a{0}'.format(idx))
        etree.SubElement(arctag,'start',x='0',y='0')
        etree.SubElement(arctag,'end',x='0',y='0')

        idx+=1
    return etree.tostring(page,pretty_print=True)
    #with open('test.sbgn','w') as  f:
    #    f.write(etree.tostring(page,pretty_print=True))
        
            