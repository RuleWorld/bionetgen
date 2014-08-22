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
 
def logMess(logType,logMessage):
    if not hasattr(logMess, "log"):
        logMess.log = []
        logMess.counter = -1
    logMess.counter += 1
    logMess.log.append("%s.%d: %s"%(logType,logMess.counter,logMessage))
    return logMess.counter
    
if __name__ == "__main__":
    a = NumericStringParser()
    print a.eval('pa(a,b)')