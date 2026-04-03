import unittest
from unittest.mock import MagicMock, patch
import sys

# Mocking missing dependencies before importing the module under test
mock_networkx = MagicMock()
mock_lxml = MagicMock()
mock_etree = MagicMock()
mock_libsbgn = MagicMock()
mock_consoleCommands = MagicMock()
mock_readBNGXML = MagicMock()

sys.modules['networkx'] = mock_networkx
sys.modules['lxml'] = mock_lxml
sys.modules['lxml.etree'] = mock_etree
sys.modules['libsbgn'] = mock_libsbgn
sys.modules['consoleCommands'] = mock_consoleCommands
sys.modules['readBNGXML'] = mock_readBNGXML

# Now we can import readGML
import readGML

class TestReadGML(unittest.TestCase):
    def test_readXML(self):
        # Setup
        filename = 'test_file.bngl'
        expected_xml_filename = 'test_file.xml'
        mock_readBNGXML.parseXML.return_value = (['molecule'], ['rule'], {'param': 1})

        # Execute
        result = readGML.readXML(filename)

        # Verify
        mock_readBNGXML.parseXML.assert_called_once_with(expected_xml_filename)
        self.assertEqual(result, (['molecule'], ['rule'], {'param': 1}))

    def test_readXML_with_different_extension(self):
        # Setup
        filename = 'model.xml.bngl'
        expected_xml_filename = 'model.xml.xml'
        mock_readBNGXML.parseXML.return_value = 'some_result'

        # Execute
        result = readGML.readXML(filename)

        # Verify
        mock_readBNGXML.parseXML.assert_called_with(expected_xml_filename)
        self.assertEqual(result, 'some_result')

if __name__ == '__main__':
    unittest.main()
