import unittest
from unittest.mock import Mock, patch
import sys
import os

# We need to mock lxml, networkx, pygraphviz, pyparsing before importing collapsedContactMap
sys.modules['lxml'] = Mock()
sys.modules['lxml.etree'] = Mock()
sys.modules['networkx'] = Mock()
sys.modules['pygraphviz'] = Mock()

# mock pyparsing for smallStructures
mock_pyparsing = Mock()
mock_pyparsing.Word = Mock(return_value=Mock())
mock_pyparsing.alphas = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
mock_pyparsing.nums = "0123456789"
mock_pyparsing.alphanums = mock_pyparsing.alphas + mock_pyparsing.nums
mock_pyparsing.Suppress = Mock(return_value=Mock())
mock_pyparsing.Group = Mock(return_value=Mock())
mock_pyparsing.Optional = Mock(return_value=Mock())
mock_pyparsing.ZeroOrMore = Mock(return_value=Mock())
mock_pyparsing.OneOrMore = Mock(return_value=Mock())
sys.modules['pyparsing'] = mock_pyparsing

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..')))
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'utils')))
sys.path.insert(0, os.path.abspath(os.path.dirname(__file__)))

from parsers.ContactMap.collapsedContactMap import extractMolecules

class TestCollapsedContactMap(unittest.TestCase):
    def test_extractMolecules_basic(self):
        # Create mock reactants
        mock_reactant_1 = Mock()
        mock_reactant_1.extractAtomicPatterns.return_value = (
            {'p1': 'pattern1', 'p2': 'pattern2'},  # ta (dict)
            ['r1', 'r2'],                          # tr (iterable)
            ['c1', 'c2']                           # tc (iterable)
        )

        mock_reactant_2 = Mock()
        mock_reactant_2.extractAtomicPatterns.return_value = (
            {'p3': 'pattern3'},                    # ta (dict)
            ['r2', 'r3'],                          # tr (iterable)
            ['c2', 'c3']                           # tc (iterable)
        )

        action = "AddBond"
        site1 = "site1"
        site2 = "site2"
        chemicalArray = [mock_reactant_1, mock_reactant_2]

        atomicPatterns, reactionCenter, context = extractMolecules(
            action, site1, site2, chemicalArray
        )

        # Verify return values
        self.assertEqual(atomicPatterns, {'p1': 'pattern1', 'p2': 'pattern2', 'p3': 'pattern3'})
        self.assertEqual(reactionCenter, {'r1', 'r2', 'r3'})
        self.assertEqual(context, {'c1', 'c2', 'c3'})

        # Verify extractAtomicPatterns was called correctly on each mock
        mock_reactant_1.extractAtomicPatterns.assert_called_once_with(action, site1, site2)
        mock_reactant_2.extractAtomicPatterns.assert_called_once_with(action, site1, site2)

    def test_extractMolecules_empty_array(self):
        atomicPatterns, reactionCenter, context = extractMolecules("AddBond", "s1", "s2", [])
        self.assertEqual(atomicPatterns, {})
        self.assertEqual(reactionCenter, set())
        self.assertEqual(context, set())

if __name__ == '__main__':
    unittest.main()
