# -*- coding: utf-8 -*-
"""
Created on Tue Sep  3 13:31:27 2013

@author: proto
"""
import sys
sys.path.insert(0, '../utils/')
import consoleCommands as console
import re
import unittest

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
    Test for file correctness
    '''
    def test_correctness(self):
        fileName = self.param[0]
        score = console.correctness(fileName + '.bngl')
        self.assertTrue(score)

    
    def extractReactions(self,netFile):
        start = netFile.index('begin reactions\n')
        end = netFile.index('end reactions\n')
        
        reactions = netFile[start+1:end]
        reactions = [x.split('#')[0]+'\n' for x in reactions]
        reactions = [x.strip() for x in reactions]
        reactions = [re.sub(' +',' ',x) for x in reactions]
        reactions = [x.split(' ') for x in reactions]
        reactions = [x[1:] for x in reactions]
        start = netFile.index('begin groups\n')
        end = netFile.index('end groups\n')
        groups = netFile[start+1:end]
        groups = [x.strip() for x in groups]
        groups = [re.sub(' +',' ',x) for x in groups]
        groups = [x.split(' ') for x in groups]
        groups = {x[2]:x[1] for x in groups if len(x) == 3}
        groups['0'] = '0'
        transformedReactions = []
        for reaction in reactions:
            tmpArray = []
            flag = True
            for idx in range(0,2):
                tmpChemical = []
                tmp = reaction[idx].split(',')
                for element in tmp:
                    if element in groups:
                        tmpChemical.append(groups[element])
                    else:
                        flag=False
                        break
                if flag:
                    tmpChemical.sort()
                    tmpArray.append(tmpChemical)
                else:
                    break
            #for now ignore those groups that don't have a string representation
            if flag:
                tmpArray.append(reaction[2])
                transformedReactions.append(tmpArray)
                    
            
        return transformedReactions
    
    '''
    test for the generated network to be the same
    '''
    def test_networkSameness(self):
        fileNameReference = self.param[0]
        fileNameComparison = self.param[1]
        console.writeNetwork(fileNameReference + '.bngl')
        console.writeNetwork(fileNameComparison + '.bngl')
        
        netReference = open(fileNameReference + '.net','r')
        netComparison = open(fileNameComparison + '.net','r')
        
        netRContent = netReference.readlines()
        netCContent = netComparison.readlines()
        
        rr = self.extractReactions(netRContent)
        rc = self.extractReactions(netCContent)
        flag = True        
        for element in rr:
            if element not in rc:
                flag =  False
        
        for element in rc:
            if element not in rr:
                flag = False
                print element
                break
        #netReference = open()
        self.assertTrue(flag)

if __name__ == "__main__":      
    suite = unittest.TestSuite()
    #ran = range(1,410)
    suite.addTest(ParametrizedTestCase.parametrize(TestOne, param=['complex/output19.bnglreduced','complex/output19']))
    unittest.TextTestRunner(verbosity=2).run(suite)

  

