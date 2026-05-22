import sys
import os
import unittest
from unittest.mock import patch, MagicMock

# Add current dir to sys.path so it finds readBNGXML
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# The issue requested parsing a simple XML string to an element.
# While lxml is not installed, we can simulate its parsing behavior
# with the standard xml.etree.ElementTree and a small wrapper to handle getchildren().
import xml.etree.ElementTree as ET  # nosec

class LxmlElementWrapper:
    """A wrapper for ET elements to provide lxml-like getchildren() behavior needed by the parser."""
    def __init__(self, et_element):
        self._et_element = et_element

    def get(self, key, default=None):
        return self._et_element.get(key, default)

    def find(self, path):
        res = self._et_element.find(path)
        if res is not None:
            return LxmlElementWrapper(res)
        return None

    def getchildren(self):
        return [LxmlElementWrapper(child) for child in self._et_element]


class TestParseComponent(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        # We need to mock the external dependencies that might not be available
        cls.patcher_lxml = patch.dict('sys.modules', {
            'lxml': MagicMock(),
            'lxml.etree': MagicMock()
        })
        cls.patcher_lxml.start()

        cls.patcher_pgv = patch.dict('sys.modules', {'pygraphviz': MagicMock()})
        cls.patcher_pgv.start()

        cls.patcher_pyparsing = patch.dict('sys.modules', {'pyparsing': MagicMock()})
        cls.patcher_pyparsing.start()

        # Now we can safely import the module
        global readBNGXML
        global st
        import readBNGXML
        import smallStructures as st

    @classmethod
    def tearDownClass(cls):
        cls.patcher_lxml.stop()
        cls.patcher_pgv.stop()
        cls.patcher_pyparsing.stop()

    def test_parseComponent_no_states(self):
        """Test parseComponent with a component that has no states."""
        # Parsing a simple XML string to an element
        xml_str = '<Component id="comp1" />'
        element = ET.fromstring(xml_str)  # nosec
        wrapped_element = LxmlElementWrapper(element)

        # Call the function
        result = readBNGXML.parseComponent(wrapped_element)

        # Verify the result
        self.assertIsInstance(result, st.Component)
        self.assertEqual(result.name, 'comp1')
        self.assertEqual(result.idx, 'comp1')
        self.assertEqual(len(result.states), 0)

    def test_parseComponent_with_states(self):
        """Test parseComponent with a component that has states."""
        # Parsing a simple XML string to an element, including the specific SBML namespace
        xml_str = '''
        <Component id="comp2">
            <ns:ListOfAllowedStates xmlns:ns="http://www.sbml.org/sbml/level3">
                <ns:State id="P" />
                <ns:State id="U" />
            </ns:ListOfAllowedStates>
        </Component>
        '''
        element = ET.fromstring(xml_str)  # nosec
        wrapped_element = LxmlElementWrapper(element)

        # Call the function
        result = readBNGXML.parseComponent(wrapped_element)

        # Verify the result
        self.assertIsInstance(result, st.Component)
        self.assertEqual(result.name, 'comp2')
        self.assertEqual(result.idx, 'comp2')
        self.assertEqual(len(result.states), 2)
        self.assertIn('P', result.states)
        self.assertIn('U', result.states)

if __name__ == '__main__':
    unittest.main()
