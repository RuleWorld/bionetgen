import unittest
from unittest.mock import MagicMock
import sys

# Mock readBNGXML and pgv, lxml, pexpect, networkx
sys.modules['readBNGXML'] = MagicMock()
sys.modules['pygraphviz'] = MagicMock()
sys.modules['pexpect'] = MagicMock()

from parsers.ContactMap.createGraph import getMapping

class TestCreateGraph(unittest.TestCase):
    def test_getMapping_happy_path(self):
        # Happy path: site is in mapping and returns the other element
        mapp = [['A', 'B'], ['C', 'D']]
        result = getMapping(mapp, 'A')
        self.assertEqual(result, 'B')

        result2 = getMapping(mapp, 'D')
        self.assertEqual(result2, 'C')

    def test_getMapping_not_present(self):
        # Site is not present in any mapping, returns None
        mapp = [['A', 'B'], ['C', 'D']]
        result = getMapping(mapp, 'Z')
        self.assertIsNone(result)

    def test_getMapping_multiple_same_site(self):
        # Site is present multiple times in a mapping list/tuple
        mapp = [['A', 'A', 'B']]
        result = getMapping(mapp, 'A')
        self.assertEqual(result, 'B')

    def test_getMapping_only_site(self):
        # Mapping contains only the site itself and no other elements, returns None
        mapp = [['A', 'A']]
        result = getMapping(mapp, 'A')
        self.assertIsNone(result)

    def test_getMapping_empty_mapp(self):
        # empty mapp
        mapp = []
        result = getMapping(mapp, 'A')
        self.assertIsNone(result)

if __name__ == '__main__':
    unittest.main()
