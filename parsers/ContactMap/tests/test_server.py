import unittest
from unittest.mock import patch, MagicMock, mock_open
import sys
import os

# Mock dependencies before import to avoid errors since this runs in a constrained environment
sys.modules['pexpect'] = MagicMock()
sys.modules['pygraphviz'] = MagicMock()
sys.modules['lxml'] = MagicMock()
sys.modules['lxml.etree'] = MagicMock()
sys.modules['readBNGXML'] = MagicMock()

# Add ContactMap to path so we can import server
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import server

class TestServer(unittest.TestCase):

    @patch('server.os.remove')
    @patch('server.glob.glob')
    @patch('builtins.open', new_callable=mock_open, read_data=b'dummy')
    @patch('server.createGraph')
    @patch.object(server.BipartiteServer, 'bngl2xml')
    def test_bipartite_oserror_path(self, mock_bngl2xml, mock_createGraph, mock_file, mock_glob, mock_remove):
        """
        Tests the error path in bipartite where os.remove throws an OSError,
        verifying that it is caught and ignored gracefully.
        """
        # Mock glob to return some files that will be "deleted"
        mock_glob.return_value = ['temp1.bngl', 'temp1.xml', 'temp1.xml.dot', 'temp1.xml.png']

        # Make os.remove raise an OSError to simulate permission denied or file not found
        mock_remove.side_effect = OSError("Mocked OSError")

        # Set up a mock bbnglFile object
        mock_bbnglFile = MagicMock()
        mock_bbnglFile.data = "dummy bngl data"

        srv = server.BipartiteServer()

        # Test bipartite with 'dot' returnType
        # The OSError should be caught gracefully without crashing
        result = srv.bipartite(mock_bbnglFile, 'dot', 'center', 'context', 'product')

        # Verify the returned object is a Binary object as expected
        self.assertIsInstance(result, server.xmlrpclib.Binary)
        self.assertEqual(result.data, b'dummy')

        # Verify os.remove was called for each file returned by glob
        self.assertEqual(mock_remove.call_count, 4)

    def test_getTransformations(self):
        """
        Tests the getTransformations method which is currently empty.
        Included to cover all untested methods.
        """
        srv = server.BipartiteServer()
        result = srv.getTransformations("dummy_file")
        self.assertIsNone(result)

if __name__ == '__main__':
    unittest.main()
