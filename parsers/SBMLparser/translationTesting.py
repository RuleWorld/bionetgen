# -*- coding: utf-8 -*-
"""
Created on Fri May 24 15:41:00 2013

@author: proto
"""

import unittest
import libsbml2bngl
from util import logMess
from evaluate import evaluate
from os import listdir
from os.path import isfile, join
import copasi

##Taken from: http://eli.thegreenplace.net/2011/08/02/python-unit-testing-parametrized-test-cases/

class ParametrizedTestCase(unittest.TestCase):
    """ TestCase classes that want to be parametrized should
        inherit from this class.
    """
    def __init__(self, methodName='runTest', param=None):
        super(ParametrizedTestCase, self).__init__(methodName)
        self.param = param

    @staticmethod
    def parametrize(testcase_klass, param=None):
        """ Create a suite containing all tests taken from the given
            subclass, passing them the parameter 'param'.
        """
        testloader = unittest.TestLoader()
        testnames = testloader.getTestCaseNames(testcase_klass)
        suite = unittest.TestSuite()
        for name in testnames:
            suite.addTest(testcase_klass(name, param=param))
        return suite
        
class TestOne(ParametrizedTestCase):
    def test_parsing(self):
        reactionDefinitions,useID = libsbml2bngl.selectReactionDefinitions('BIOMD%010i.xml' %self.param)
        libsbml2bngl.analyzeFile('XMLExamples/curated/BIOMD%010i.xml' % self.param,reactionDefinitions,
                    False,'complex/output' + str(self.param) + '.bngl',speciesEquivalence=None)


class TestEval(ParametrizedTestCase):
    def test_eval(self):
        fileName = self.param
        print fileName
        self.assertEqual(evaluate(fileName),0)

class TestCopasi(ParametrizedTestCase):
    def test_copasi(self):
        fileNumber = self.param
        score = copasi.evaluate(fileNumber)
        self.assertAlmostEqual(score,0,delta=1e-2)
    
def getValidBNGLFiles(directory):
    onlyfiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) ]
    
    logFiles = [x[0:-4] for x in onlyfiles if 'log' in x]
    errorFiles = []
    #dont skip the files that only have warnings    
    for x in logFiles:    
        with open('./' + directory + '/' + x +'.log','r') as f:
            k = f.readlines()
            if 'ERROR' in ','.join(k):
                errorFiles.append(x)
    bnglFiles = [x for x in onlyfiles if 'bngl' in x and 'log' not in x]
    validFiles = [x for x in bnglFiles if x not in errorFiles]
    return validFiles

if __name__ == "__main__":      
    suite = unittest.TestSuite()
    for index in range(1,410):
        suite.addTest(ParametrizedTestCase.parametrize(TestOne, param=index))
    validFiles = getValidBNGLFiles('raw')
    for fileName in validFiles:
        suite.addTest(ParametrizedTestCase.parametrize(TestEval,param='./raw/' + fileName))
    #validFiles = getValidBNGLFiles('complex')
    #for fileName in validFiles:
    #    suite.addTest(ParametrizedTestCase.parametrize(TestEval,param='./complex/' + fileName))
    for index in range(1,410):
        suite.addTest(ParametrizedTestCase.parametrize(TestCopasi, param=index))
    unittest.TextTestRunner(verbosity=2).run(suite)
