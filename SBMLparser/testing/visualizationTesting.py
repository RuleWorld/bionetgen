# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 18:32:05 2014

@author: proto
"""

import unittest
import sys
import utils.consoleCommands as consoleCommands
import os
import os.path

def getValidBNGLFiles(directory):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'
    """
    onlyfiles = [ f for f in os.listdir(directory) if os.path.isfile(os.path.join(directory, f)) ]
    
    logFiles = [x[0:-4] for x in onlyfiles if 'log' in x]
    errorFiles = []
    #dont skip the files that only have warnings    
    for log in logFiles:    
        with open(os.path.join(directory, log) + '.log','r') as f:
            k = f.readlines()
            if 'ERROR' in ','.join(k):
                errorFiles.append(log)
    bnglFiles = [x for x in onlyfiles if x.endswith('bngl') and 'log' not in x and 'dict' not in x]
    
    validFiles = [x for x in bnglFiles if x not in errorFiles]
    return validFiles


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

from subprocess import call        
import os

class GMLIntegrityTestCase(ParametrizedTestCase):
    '''
    validates that the output gml is a valid gml file given a bngl file
    '''
    
    def generateGraph(self,fileName,graphType):
        consoleCommands.generateGraph(fileName,graphType,outputDirectory='/tmp')
        fileName2 = fileName.split('/')[-1]
        fileRoot = '.'.join(fileName2.split('.')[:-1])
        
        inputGraph = '/tmp/{1}_{0}.gml'.format(graphType,fileRoot)
        with open(os.devnull,'r') as f:
            result = call(['gml2gv',inputGraph],stdout=f)
        return result
        
    def test_gml_contact(self):
        print self.param
        self.assertEqual(self.generateGraph(self.param,'contactmap'),0)
    def test_gml_regulatory(self):
        self.assertEqual(self.generateGraph(self.param,'regulatory'),0)
    def test_gml_process(self):
        self.assertEqual(self.generateGraph(self.param,'process'),0)
    
    
if __name__ == "__main__":      
    suite = unittest.TestSuite()
    directory = '/home/proto/workspace/bionetgen/parsers/SBMLparser/complex2'
    validFiles = getValidBNGLFiles(directory) 
    validFiles = sorted(validFiles)
    for validf in validFiles:
        suite.addTest(ParametrizedTestCase.parametrize(GMLIntegrityTestCase, 
                                                param='{1}/{0}'.format(validf,directory)))
    unittest.TextTestRunner(verbosity=2).run(suite)
