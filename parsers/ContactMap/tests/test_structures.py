import unittest
from unittest.mock import patch, MagicMock

# The repository memory explicitly states:
# "The development environment lacks internet access; pip installs will fail. Use mocking (e.g., unittest.mock) to handle missing external dependencies such as lxml, networkx, pygraphviz, or pyparsing in Python unit tests."
# Therefore, we MUST mock these at the top level before importing structures.py,
# but we do it gracefully without doing del sys.modules in teardown.
import sys
sys.modules['lxml'] = MagicMock()
sys.modules['lxml.etree'] = MagicMock()
sys.modules['pygraphviz'] = MagicMock()
mock_pyparsing = MagicMock()
sys.modules['pyparsing'] = mock_pyparsing
mock_pyparsing.Word = MagicMock()
mock_pyparsing.Suppress = MagicMock()
mock_pyparsing.Optional = MagicMock()
mock_pyparsing.alphanums = 'alphanums'
mock_pyparsing.Group = MagicMock()
mock_pyparsing.ZeroOrMore = MagicMock()

from parsers.ContactMap.structures import readFromString, Species, Molecule, Component, parseReactions

class TestStructures(unittest.TestCase):
    @patch('parsers.ContactMap.structures.parseReactions')
    def test_readFromString_basic(self, mock_parseReactions):
        mock_parseReactions.return_value = [
            ['A', ['b', ['!', '1']], ['p', ['~', 'P']]],
            ['B', ['a', ['!', '1']]]
        ]

        sp = readFromString('dummy_string')

        self.assertIsInstance(sp, Species)
        self.assertEqual(len(sp.molecules), 2)

        molA = sp.getMolecule('A')
        self.assertIsNotNone(molA)
        self.assertEqual(len(molA.components), 2)
        comp_b = molA.getComponent('b')
        self.assertIsNotNone(comp_b)
        self.assertEqual(comp_b.bonds, ['1'])

        comp_p = molA.getComponent('p')
        self.assertIsNotNone(comp_p)
        self.assertEqual(comp_p.activeState, 'P')

        molB = sp.getMolecule('B')
        self.assertIsNotNone(molB)
        self.assertEqual(len(molB.components), 1)
        comp_a = molB.getComponent('a')
        self.assertIsNotNone(comp_a)
        self.assertEqual(comp_a.bonds, ['1'])

    @patch('parsers.ContactMap.structures.parseReactions')
    def test_readFromString_empty_molecule(self, mock_parseReactions):
        mock_parseReactions.return_value = [['C']]
        sp = readFromString('dummy')

        self.assertEqual(len(sp.molecules), 1)
        molC = sp.getMolecule('C')
        self.assertIsNotNone(molC)
        self.assertEqual(len(molC.components), 0)

    @patch('parsers.ContactMap.structures.parseReactions')
    def test_readFromString_multiple_states_bonds(self, mock_parseReactions):
        mock_parseReactions.return_value = [
            ['M', ['c', ['~', 'S1'], ['~', 'S2'], ['!', '1'], ['!', '2']]]
        ]
        sp = readFromString('dummy')
        molM = sp.getMolecule('M')
        self.assertIsNotNone(molM)
        comp_c = molM.getComponent('c')
        self.assertIsNotNone(comp_c)
        self.assertEqual(comp_c.states, ['S1', 'S2'])
        self.assertEqual(comp_c.activeState, 'S2')
        self.assertEqual(comp_c.bonds, ['1', '2'])

    def test_parseReactions_called(self):
        # We can only verify grammar setup is triggered since the environment lacks pyparsing.
        mock_pyparsing.Word.reset_mock()
        try:
            parseReactions('some_reaction')
        except AttributeError:
            pass # mock objects don't have parseString
        self.assertTrue(mock_pyparsing.Word.called)

if __name__ == '__main__':
    unittest.main()
