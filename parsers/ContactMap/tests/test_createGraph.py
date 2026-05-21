import unittest
from unittest.mock import patch, MagicMock
import sys
import os

# Mock dependencies before import
sys.modules['pygraphviz'] = MagicMock()
sys.modules['pexpect'] = MagicMock()
# Since readBNGXML imports lxml, we mock it too
sys.modules['lxml'] = MagicMock()
sys.modules['lxml.etree'] = MagicMock()
sys.modules['readBNGXML'] = MagicMock()

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import createGraph

class MockMolecule:
    def __init__(self, name):
        self.name = name

class MockAtomic:
    def __init__(self, molecules):
        self.molecules = molecules

class TestCreateGraph(unittest.TestCase):
    def test_solveWildcards(self):
        # Normal atomic with multiple molecules (gets indexed)
        molA1 = MockMolecule("A")
        molB1 = MockMolecule("B")
        atomic1 = MockAtomic([molA1, molB1])

        # Normal atomic with single molecule (ignored by index)
        molC1 = MockMolecule("C")
        atomic2 = MockAtomic([molC1])

        # Another normal atomic with multiple molecules (gets indexed)
        molA2 = MockMolecule("A")
        molD1 = MockMolecule("D")
        atomic3 = MockAtomic([molA2, molD1])

        # Wildcard atomic resolving to A
        molA_wc = MockMolecule("A")
        wildcard1 = MockAtomic([molA_wc])

        # Wildcard atomic resolving to unknown molecule
        molE_wc = MockMolecule("E")
        wildcard2 = MockAtomic([molE_wc])

        # We need a dictionary that allows len() checks on atomicArray elements
        # like `len(atomicArray[x].molecules) > 1` inside solveWildcards
        class AtomicArrayDict(dict):
            pass

        atomicArray = AtomicArrayDict({
            'atomic1': atomic1,
            'atomic2': atomic2,
            'atomic3': atomic3,
            '+wildcard1': wildcard1,
            '+wildcard2': wildcard2
        })

        createGraph.solveWildcards(atomicArray)

        # Check that wildcard1 resolved to [atomic1, atomic3]
        self.assertEqual(len(atomicArray['+wildcard1']), 2)
        self.assertIn(atomic1, atomicArray['+wildcard1'])
        self.assertIn(atomic3, atomicArray['+wildcard1'])

        # Check that wildcard2 did not resolve and remains unmodified
        self.assertEqual(atomicArray['+wildcard2'], wildcard2)

if __name__ == '__main__':
    unittest.main()
