# -*- coding: utf-8 -*-
"""
Created on Tue Aug 19 00:46:05 2014

@author: proto
"""

import unittest
import os
import sys

sys.path.insert(0, '..')
sys.path.insert(0, os.path.join('..','SBMLparser'))

import SBMLparser.libsbml2bngl as libsbml2bngl
import numpy as np
import pexpect
import types

from IPython.lib import deepreload

        
def bnglExecution(bnglFile,settings):
    os.chdir('tmp')
    try:
        bngconsole = pexpect.spawn('bngdev --console')
        bngconsole.expect('BNG>')
        bngconsole.sendline('load {0}.bngl'.format(bnglFile))
        
        bngconsole.expect('BNG>')
        bngconsole.sendline('action generate_network()')
        bngconsole.expect('BNG>')
        bngconsole.sendline('action simulate({{method=>"ode",t_start=>{0},t_end=>{1},n_steps=>{4},atol=>{2},rtol=>{3}}})'.format(
        settings['start'][0],settings['duration'][0],settings['absolute'][0],settings['relative'][0],settings['steps'][0]))
        
        bngconsole.expect('BNG>')
        bngconsole.close() 
        with open('{0}.gdat'.format(bnglFile)) as f:
            output = f.readlines()
        output = [x.strip().split(' ') for x in output]
        header = [x for x in output[0] if x not in ['#','']]
        values = np.array([[float(y) for y in x if y!= ''] for x in output[1:] ])
        headerIndex = [header.index(x) for x in settings['variables']]
        values = values[:,headerIndex]
    finally:
        os.chdir('..')

    return values,settings['absolute'][0]

def parseCSV(fileName,headers):
    with open(fileName,'r') as f:
        inputLines = f.readlines()
    inputLines = [x.strip().split(',') for x in inputLines]
    content =  np.array([[float(y) for y in x] for x in inputLines[1:]])
    header = [x.strip() for x in inputLines[0]]
    headerIndex = [header.index(x) for x in headers]
    values = content[:,headerIndex]
    return values   
    
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
    '''
    Test for ability to ruleify
    '''
    def setUp(self):
        #reactionDefinitions, useID = libsbml2bngl.selectReactionDefinitions('BIOMD%010i.xml' % self.param)
        #spEquivalence = detectCustomDefinitions(bioNumber)
        pass
    
    def extractSimulationSettings(self,fileName):
        with open(fileName,'r') as f:
            settings = f.readlines()
            
        settings = [x.strip().split(':') for x in settings]
        settingsDict = {x[0]:x[1].split(',') for x in settings if len(x)==2}
        for element in settingsDict:
            settingsDict[element] = [x.strip() for x in settingsDict[element]]
        return settingsDict
            
    def test_valid(self):
        print self.param,
        #deepreload.reload(libsbml2bngl)
        libsbml2bngl.analyzeFile('{0}/{1}/{1}-sbml-l2v4.xml'.format(self.param[0],self.param[1]), '../reactionDefinitions/reactionDefinition7.json',
                    False, '../config/namingConventions.json',
                    outputFile='tmp/output' + str(self.param[1]) + '.bngl', speciesEquivalence=None,atomize=True,bioGrid=False)
        settings = self.extractSimulationSettings('{0}/{1}/{1}-settings.txt'.format(self.param[0],self.param[1]))
        bnglValues,atol = bnglExecution('output{0}'.format(self.param[1]),settings)
        referenceValues = parseCSV('{0}/{1}/{1}-results.csv'.format(self.param[0],self.param[1]),settings['variables'])
        print float(atol),((bnglValues - referenceValues)**2).mean()
        self.assertAlmostEqual(((bnglValues - referenceValues)**2).mean(),0,delta=float(atol))
        dirs = [ f for f in os.listdir('./tmp') if self.param[1] in f]
        for element in dirs:
            os.remove('./tmp/{0}'.format(element))

    def tearDown(self):
        dirs = [ f for f in os.listdir('./tmp') if not f.endswith('bngl')]
        for element in dirs:
            os.remove('./tmp/{0}'.format(element))
        pass

class TestValid(ParametrizedTestCase):
    '''
    Test for whether a file is recognized as correct by bng --check
    '''
    #skip stoichiometry math
    xdirs = ['00068','00069','00070','00518']
    xdirs.append('00588')
    #fast
    xdirs.extend(['00870','00871','00872','00873','00874','00875'])
    dirs = [ f for f in os.listdir('./semantic')]
    dirs.sort()
    dirs = ['01061']
    #dirs = ['00994','00998','01027']
    suite = unittest.TestSuite()
    for t in [x for x in dirs if x not in xdirs]:
        suite.addTest(ParametrizedTestCase.parametrize(TestOne, param=['semantic',t]))

    unittest.TextTestRunner(verbosity=2).run(suite)

