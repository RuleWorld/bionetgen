import unittest
from unittest.mock import MagicMock
import sys
import os

# Mock dependencies before importing the target module
sys.modules['pygraphviz'] = MagicMock()
sys.modules['pexpect'] = MagicMock()
sys.modules['readBNGXML'] = MagicMock()

# Import the module under test
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from createGraph import extractMolecules

class TestCreateGraph(unittest.TestCase):
    def test_extractMolecules_empty(self):
        """Test with an empty chemical array."""
        atomicPatterns, reactionCenter, context = extractMolecules('action', 'site1', 'site2', [])
        self.assertEqual(atomicPatterns, {})
        self.assertEqual(reactionCenter, set())
        self.assertEqual(context, set())

    def test_extractMolecules_single_reactant(self):
        """Test with a single reactant in the array."""
        mock_reactant = MagicMock()
        mock_reactant.extractAtomicPatterns.return_value = ({'a': 1, 'b': 2}, ['rc1', 'rc2'], ['ctx1'])

        atomicPatterns, reactionCenter, context = extractMolecules('Bind', 'S1', 'S2', [mock_reactant])

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

        atomicPatterns, reactionCenter, context = extractMolecules(
            'Unbind', 'S3', 'S4', [mock_reactant1, mock_reactant2]
        )

        self.assertEqual(atomicPatterns, {'a': 1, 'b': 3, 'c': 4})
        self.assertEqual(reactionCenter, {'rc1', 'rc2', 'rc3'})
        self.assertEqual(context, {'ctx1', 'ctx2'})

        mock_reactant1.extractAtomicPatterns.assert_called_once_with('Unbind', 'S3', 'S4')
        mock_reactant2.extractAtomicPatterns.assert_called_once_with('Unbind', 'S3', 'S4')

if __name__ == '__main__':
    unittest.main()
