# -*- coding: utf-8 -*-
"""
Created on Sun Mar 25 21:26:49 2012

@author: proto
"""
from __future__ import division
import json


from pyparsing import (Literal,CaselessLiteral,Word,Combine,Group,Optional,
                       ZeroOrMore,Forward,nums,alphas,oneOf,alphanums)
import math
import operator
import logging
import pickle
import os
from subprocess import call
import sys
import fnmatch
# import progressbar

#sys.path.insert(0, '../utils/')
#import consoleCommands as console


class NumericStringParser(object):
    '''
    Most of this code comes from the fourFn.py pyparsing example

    '''
    def pushFirst(self, strg, loc, toks ):
        self.exprStack.append( toks[0] )
    def pushUMinus(self, strg, loc, toks ):
        if toks and toks[0]=='-': 
            self.exprStack.append( 'unary -' )
    def __init__(self):
        """
        expop   :: '^'
        multop  :: '*' | '/'
        addop   :: '+' | '-'
        integer :: ['+' | '-'] '0'..'9'+
        atom    :: PI | E | real | fn '(' expr ')' | '(' expr ')'
        factor  :: atom [ expop factor ]*
        term    :: factor [ multop factor ]*
        expr    :: term [ addop term ]*
        """
        point = Literal( "." )
        e     = CaselessLiteral( "E" )
        fnumber = Combine( Word( "+-"+alphanums+"_", alphanums+"_" ) + 
                           Optional( point + Optional( Word( alphanums+"_" ) ) ) +
                           Optional( e + Word( "+-"+alphanums+"_", alphanums+"_" ) ) )
        
        ident = Word(alphas,alphanums + "_")       
        plus  = Literal( "+" )
        minus = Literal( "-" )
        mult  = Literal( "*" )
        div   = Literal( "/" )
        lpar  = Literal( "(" ).suppress()
        rpar  = Literal( ")" ).suppress()
        
        addop  = plus | minus
        multop = mult | div
        expop = Literal( "^" )
        pi    = CaselessLiteral( "PI" )
        expr = Forward()
        function = ident + lpar + expr + ZeroOrMore("," + expr) + rpar
        atom = ((Optional(oneOf("- +")) +
                 (pi|e|function|fnumber).setParseAction(self.pushFirst))
                | Optional(oneOf("- +")) + Group(lpar+expr+rpar)
                ).setParseAction(self.pushUMinus)       
        # by defining exponentiation as "atom [ ^ factor ]..." instead of 
        # "atom [ ^ atom ]...", we get right-to-left exponents, instead of left-to-right
        # that is, 2^3^2 = 2^(3^2), not (2^3)^2.
        factor = Forward()
        factor << atom + ZeroOrMore( ( expop + factor ).setParseAction( self.pushFirst ) )
        term = factor + ZeroOrMore( ( multop + factor ).setParseAction( self.pushFirst ) )
        expr << term + ZeroOrMore( ( addop + term ).setParseAction( self.pushFirst ) )
        # addop_term = ( addop + term ).setParseAction( self.pushFirst )
        # general_term = term + ZeroOrMore( addop_term ) | OneOrMore( addop_term)
        # expr <<  general_term       
        self.bnf = expr
        # map operator symbols to corresponding arithmetic operations
        epsilon = 1e-12
        self.opn = { "+" : operator.add,
                "-" : operator.sub,
                "*" : operator.mul,
                "/" : operator.truediv,
                "^" : operator.pow }
        self.fn  = { "sin" : math.sin,
                "cos" : math.cos,
                "tan" : math.tan,
                "abs" : abs,
                "trunc" : lambda a: int(a),
                "round" : round,
                "sgn" : lambda a: abs(a)>epsilon and cmp(a,0) or 0}
    def evaluateStack(self, s ):
        op = s.pop()
        print op
        if op == 'unary -':
            return -self.evaluateStack( s )
        if op in "+-*/^":
            op2 = self.evaluateStack( s )
            op1 = self.evaluateStack( s )
            return self.opn[op]( op1, op2 )
        elif op == "PI":
            return math.pi # 3.1415926535
        elif op == "E":
            return math.e  # 2.718281828
        elif op in self.fn:
            return self.fn[op]( self.evaluateStack( s ) )
        elif op[0].isalpha():
            return 0
        else:
            return float( op )
            
    def eval(self,num_string,parseAll=True):
        self.exprStack=[]
        #remove comments
        if num_string.find('#') != -1:
            num_string =num_string[0:num_string.find('#')]
        try:
            results=self.bnf.parseString(num_string,parseAll)
            return True
        except:
            return False

        #val=self.evaluateStack( self.exprStack[:] )
        #return val
        
    
        
def defaultReactionDefinition():
    listOfReactions= {'1': [['S0', 'S1'], ['S2']], '2': [['S2'], ['S0', 'S1']]}
    listOfDefinitions = {'Binding': [1,2]}
    final = {'reactions':listOfReactions,'definitions':listOfDefinitions}
    with open('reactionDefinition.json','w') as fp:
        json.dump(final,fp)
 


def setupLog(fileName, level):
    logging.basicConfig(filename=fileName,level=level,filemode='w')

def logMess(logType, logMessage):
    

    level = logType.split(':')[0]
    module = logType.split(':')[1]
    logger = logging.getLogger(module)
    if level == 'INFO':
        logger.info(logMessage)
    elif level == 'DEBUG':
        logger.debug(logMessage)
    elif level == 'WARNING':
        logger.warning(logMessage)
    elif level == 'CRITICAL':
        logger.critical(logMessage)
    elif level == 'ERROR':
        logger.error(logMessage)




def testBNGFailure(fileName):
    with open(os.devnull,"w") as f:
        result = call(['bngdev',fileName],stdout=f)
    return result

import os.path

'''
def getValidFiles(directory, extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append(os.path.join(root, filename))
    for i in xrange(len(matches)):
        matches[i] = (matches[i], os.path.getsize(matches[i]))
    matches.sort(key=lambda filename: filename[1], reverse=False)
    matches = [x[0] for x in matches]
    return matches


def generateBNGXML(directory):

    bnglFiles = getValidFiles(directory, 'bngl')
    print 'converting {0} bnglfiles'.format(len(bnglFiles))
    progress = progressbar.ProgressBar()
    for i in progress(range(len(bnglFiles))):
        xmlName = '.'.join(bnglFiles[i].split('.')[:-1]) + '.xml'
        

        if os.path.exists(xmlName):
            continue
        console.bngl2xml(bnglFiles[i], timeout=10)

    print 'moving xml files'
    files = glob.iglob(os.path.join('.', "*.xml"))
    for xmlfile in files:
        if os.path.isfile(xmlfile):
            shutil.move(xmlfile, directory)
'''


if __name__ == "__main__":
    '''
    with open('failure.dump','rb') as f:
        failedFiles = pickle.load(f)
    failedFiles.sort()
    for bng in failedFiles:
        sys.stderr.write(bng)
        testBNGFailure(bng)
    '''
    generateBNGXML('new_non_curated')