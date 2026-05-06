import sys
import os
import unittest
from unittest.mock import MagicMock, patch

# Add current dir to sys.path so it finds readBNGXML
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

# Mock lxml before importing readBNGXML
sys.modules['lxml'] = MagicMock()
sys.modules['lxml.etree'] = MagicMock()

import readBNGXML

class TestReadBNGXML(unittest.TestCase):

    @patch('readBNGXML.etree')
    def test_parseXML(self, mock_etree):
        """Test parseXML function in readBNGXML.py"""
        # Create mock elements
        mock_doc = MagicMock()
        mock_molecule1 = MagicMock()
        mock_molecule2 = MagicMock()
        mock_rule1 = MagicMock()
        mock_rule2 = MagicMock()

        # Configure doc mock
        mock_etree.parse.return_value = mock_doc

        def findall_side_effect(path):
            if 'MoleculeType' in path:
                return [mock_molecule1, mock_molecule2]
            elif 'ReactionRule' in path:
                return [mock_rule1, mock_rule2]
            return []

        mock_doc.findall.side_effect = findall_side_effect

        # We need to mock parseMolecules and parseRule
        with patch('readBNGXML.parseMolecules') as mock_parseMolecules, \
             patch('readBNGXML.parseRule') as mock_parseRule:

             # Setup return values
             mock_parseMolecules.side_effect = ['parsed_mol1', 'parsed_mol2']
             mock_parseRule.side_effect = ['parsed_rule1', 'parsed_rule2']

             # Call the function
             moleculeList, ruleDescription = readBNGXML.parseXML("dummy.xml")

             # Assertions
             mock_etree.parse.assert_called_once()
             self.assertEqual(mock_etree.parse.call_args[0][0], "dummy.xml")
             mock_doc.findall.assert_any_call('.//{http://www.sbml.org/sbml/level3}MoleculeType')
             mock_doc.findall.assert_any_call('.//{http://www.sbml.org/sbml/level3}ReactionRule')

             self.assertEqual(mock_doc.findall.call_count, 2)

             self.assertEqual(mock_parseMolecules.call_count, 2)
             mock_parseMolecules.assert_any_call(mock_molecule1)
             mock_parseMolecules.assert_any_call(mock_molecule2)

             self.assertEqual(mock_parseRule.call_count, 2)
             mock_parseRule.assert_any_call(mock_rule1)
             mock_parseRule.assert_any_call(mock_rule2)

             self.assertEqual(moleculeList, ['parsed_mol1', 'parsed_mol2'])
             self.assertEqual(ruleDescription, ['parsed_rule1', 'parsed_rule2'])

    @patch('readBNGXML.etree')
    def test_parseXML_empty(self, mock_etree):
        """Test parseXML function with no molecules or rules"""
        # Create mock elements
        mock_doc = MagicMock()

        # Configure doc mock
        mock_etree.parse.return_value = mock_doc
        mock_doc.findall.return_value = []

        # Call the function
        moleculeList, ruleDescription = readBNGXML.parseXML("empty.xml")

        # Assertions
        mock_etree.parse.assert_called_once()
        self.assertEqual(mock_etree.parse.call_args[0][0], "empty.xml")
        self.assertEqual(moleculeList, [])
        self.assertEqual(ruleDescription, [])

if __name__ == '__main__':
    unittest.main()
