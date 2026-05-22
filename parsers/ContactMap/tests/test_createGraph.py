import unittest
from unittest.mock import MagicMock
import sys
import os

# Mock dependencies before importing the target module
sys.modules['pygraphviz'] = MagicMock()
sys.modules['pexpect'] = MagicMock()
# Since readBNGXML imports lxml, we mock it too
sys.modules['lxml'] = MagicMock()
sys.modules['lxml.etree'] = MagicMock()
sys.modules['readBNGXML'] = MagicMock()

# Import the module under test
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import createGraph

class MockMolecule:
    def __init__(self, name):
        self.name = name

class MockAtomic:
    def __init__(self, molecules):
        self.molecules = molecules

class TestCreateGraph(unittest.TestCase):
    def test_extractMolecules_empty(self):
        """Test with an empty chemical array."""
        atomicPatterns, reactionCenter, context = createGraph.extractMolecules('action', 'site1', 'site2', [])
        self.assertEqual(atomicPatterns, {})
        self.assertEqual(reactionCenter, set())
        self.assertEqual(context, set())

    def test_extractMolecules_single_reactant(self):
        """Test with a single reactant in the array."""
        mock_reactant = MagicMock()
        mock_reactant.extractAtomicPatterns.return_value = ({'a': 1, 'b': 2}, ['rc1', 'rc2'], ['ctx1'])

        atomicPatterns, reactionCenter, context = createGraph.extractMolecules('Bind', 'S1', 'S2', [mock_reactant])

        self.assertEqual(atomicPatterns, {'a': 1, 'b': 2})
        self.assertEqual(reactionCenter, {'rc1', 'rc2'})
        self.assertEqual(context, {'ctx1'})

        # Verify the arguments were passed correctly
        mock_reactant.extractAtomicPatterns.assert_called_once_with('Bind', 'S1', 'S2')

    def test_extractMolecules_multiple_reactants_overlap(self):
        """Test with multiple reactants having overlapping elements."""
        mock_reactant1 = MagicMock()
        mock_reactant1.extractAtomicPatterns.return_value = ({'a': 1, 'b': 2}, ['rc1', 'rc2'], ['ctx1'])

        mock_reactant2 = MagicMock()
        # Note: dictionary update overwrites 'b': 2 with 'b': 3
        mock_reactant2.extractAtomicPatterns.return_value = ({'b': 3, 'c': 4}, ['rc2', 'rc3'], ['ctx1', 'ctx2'])

        atomicPatterns, reactionCenter, context = createGraph.extractMolecules(
            'Unbind', 'S3', 'S4', [mock_reactant1, mock_reactant2]
        )

        self.assertEqual(atomicPatterns, {'a': 1, 'b': 3, 'c': 4})
        self.assertEqual(reactionCenter, {'rc1', 'rc2', 'rc3'})
        self.assertEqual(context, {'ctx1', 'ctx2'})

        mock_reactant1.extractAtomicPatterns.assert_called_once_with('Unbind', 'S3', 'S4')
        mock_reactant2.extractAtomicPatterns.assert_called_once_with('Unbind', 'S3', 'S4')

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
