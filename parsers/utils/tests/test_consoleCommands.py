import unittest
from unittest.mock import patch, MagicMock
import subprocess
import sys

# We need to mock dependencies for testing environment
sys.modules['lxml'] = MagicMock()
sys.modules['lxml.etree'] = MagicMock()
sys.modules['pygraphviz'] = MagicMock()
mock_pyparsing = MagicMock()
sys.modules['pyparsing'] = mock_pyparsing

# We need to mock pexpect but have pexpect.TIMEOUT be an Exception for origin/master tests
class MockPexpectTimeout(Exception):
    pass

mock_pexpect = MagicMock()
mock_pexpect.TIMEOUT = MockPexpectTimeout
sys.modules['pexpect'] = mock_pexpect

from parsers.utils.consoleCommands import correctness, bngl2xml

class TestConsoleCommands(unittest.TestCase):
    @patch('parsers.utils.consoleCommands.pexpect.spawn')
    @patch('parsers.utils.consoleCommands.getBngExecutable', return_value='dummy_bng')
    def test_correctness_success(self, mock_getBngExecutable, mock_spawn):
        mock_bngconsole = MagicMock()
        mock_bngconsole.before = 'Some output without error'
        mock_spawn.return_value = mock_bngconsole

        result = correctness('dummy.bngl')
        self.assertTrue(result)

    @patch('parsers.utils.consoleCommands.pexpect.spawn')
    @patch('parsers.utils.consoleCommands.getBngExecutable', return_value='dummy_bng')
    def test_correctness_error(self, mock_getBngExecutable, mock_spawn):
        mock_bngconsole = MagicMock()
        mock_bngconsole.before = 'Some output with ERROR inside'
        mock_spawn.return_value = mock_bngconsole

        result = correctness('dummy.bngl')
        self.assertFalse(result)

    @patch('parsers.utils.consoleCommands.subprocess.call')
    @patch('parsers.utils.consoleCommands.pexpect.spawn')
    def test_bngl2xml_timeout(self, mock_spawn, mock_subprocess_call):
        # Configure the mock to raise pexpect.TIMEOUT when spawn is called
        mock_spawn.side_effect = mock_pexpect.TIMEOUT('Timeout occurred')

        # Call the function
        bngl2xml('dummy.bngl')

        # Assert that subprocess.call was called with the correct arguments
        mock_subprocess_call.assert_called_once_with(['/usr/bin/killall', 'bngdev'], shell=False)

if __name__ == '__main__':
    unittest.main()
