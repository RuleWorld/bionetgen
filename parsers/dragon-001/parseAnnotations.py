# -*- coding: utf-8 -*-
"""
Created on Fri Jan 17 15:16:28 2014

@author: proto
"""
import pyparsing as pyp

def syntaxDefinition():
    commentDemarker = pyp.Suppress(pyp.Literal('###'))
    literal = pyp.Word(pyp.alphanums + "_")
    quotedLiteral = pyp.Suppress(pyp.Literal("'")) + pyp.Word(pyp.alphanums + "._@ :/") + pyp.Suppress(pyp.Literal("'"))
    tags =  pyp.Suppress(pyp.Literal('#@')) + literal.setResultsName('tags')
    
    listDefinition = (pyp.Suppress(pyp.Literal('[')) 
        + pyp.Group(pyp.delimitedList(quotedLiteral.setResultsName('definitionElement'))).setResultsName('definitionList') 
        + pyp.Suppress(pyp.Literal(']')))
    documentation = pyp.Group(((tags + pyp.Suppress(pyp.Literal(':'))) + (quotedLiteral.setResultsName('definitionElement') | listDefinition))).setResultsName('documentationElement')
    
    bnglDocHeader = (commentDemarker + pyp.ZeroOrMore(documentation) + commentDemarker).setResultsName('bnglDocumentation')
    return bnglDocHeader
    
def bitSyntaxDefinition():
    commentDemarker = pyp.Suppress(pyp.Literal('###'))
    literal = pyp.Word(pyp.alphanums + "_")
    quotedLiteral = pyp.Suppress(pyp.Literal("'")) + pyp.Word(pyp.alphanums + "._@ :/") + pyp.Suppress(pyp.Literal("'"))
    tags =  pyp.Suppress(pyp.Literal('@')) + literal.setResultsName('tags')
    listDefinition = (pyp.Suppress(pyp.Literal('[')) 
        + pyp.Group(pyp.delimitedList(quotedLiteral.setResultsName('definitionElement'))).setResultsName('definitionList') 
        + pyp.Suppress(pyp.Literal(']')))
    documentation = pyp.Suppress(commentDemarker) + (((tags + pyp.Suppress(pyp.Literal(':'))) + (quotedLiteral.setResultsName('definitionElement') | listDefinition))).setResultsName('documentationElement')
    bitDocumentation = pyp.OneOrMore(documentation)  
    return bitDocumentation
    
def dict2DatabaseFormat(tagsDict):
    finalDict = {}
    finalDict['structuredTags'] = []
    finalDict['notes'] = []
    for element in tagsDict:
        if element == 'creatorName':
            finalDict['author'] = tagsDict[element]
        elif element != 'creatorEmail':
            if not isinstance(tagsDict[element],str) and not isinstance(tagsDict[element],unicode):
                finalDict['structuredTags'].extend(tagsDict[element])
            else:
                finalDict['notes'] = tagsDict[element]
    return finalDict

def parseAnnotations(bnglString):
    docSyntax = syntaxDefinition()
    speciesDocSyntax = bitSyntaxDefinition()
    docSyntax = syntaxDefinition()
    scans = speciesDocSyntax.searchString(bnglString)
    tokens = docSyntax.parseString(bnglString)
    tagsDict = {}
    for element in tokens:
        tagsDict[element[0]] = element[1]
    return tagsDict
    
if __name__ == "__main__":
    with open('output48.bngl') as f:
        lines = f.read()
    tagsDict = parseAnnotations(lines)
    print tagsDict
    print dict2DatabaseFormat(tagsDict)