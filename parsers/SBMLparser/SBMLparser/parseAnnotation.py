import sys
import string
from xml.dom import minidom, Node


def walk(parent, outFile, level, database):                               # [1]
    for node in parent.childNodes:
        if node.nodeType == Node.ELEMENT_NODE:
            # Write out the element name.
            printLevel(outFile, level)
            #outFile.write('Element: %s\n' % node.nodeName)
            # Write out the attributes.
            attrs = node.attributes                             # [2]
            for attrName in attrs.keys():
                attrNode = attrs.get(attrName)
                attrValue = attrNode.nodeValue
                printLevel(outFile, level + 2)
                if attrName == 'rdf:resource':
                    database.append(attrValue)

           # outFile.write('Attribute -- Name: %s  Value: %s\n' % \
                #    (attrName, attrValue))
            # Walk over any text nodes in the current node.
            content = []                                        # [3]
            for child in node.childNodes:
                if child.nodeType == Node.TEXT_NODE:
                    content.append(child.nodeValue)
            if content:
                strContent = string.join(content)
                printLevel(outFile, level)
           # outFile.write('Content: "')
           # outFile.write(strContent)
           # outFile.write('"\n')
            # Walk the child nodes.
            walk(node, outFile, level+1, database)


def printLevel(outFile, level):
    for idx in range(level):
        #outFile.write('    ')
        pass


# [5]
def parseAnnotation(XMLString):
    outFile = ''
    if XMLString == '':
        return []
    doc = minidom.parseString(XMLString)
    rootNode = doc.documentElement
    level = 0
    database = []
    walk(rootNode, outFile, level, database)
    return database


def main():
    args = sys.argv[1:]
    if len(args) != 1:
        print 'usage: python test.py infile.xml'
        sys.exit(-1)
    print parseAnnotation(args[0])


if __name__ == '__main__':
    main()
