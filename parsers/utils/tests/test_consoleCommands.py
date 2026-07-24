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
    @patch('parsers.utils.consoleCommands.subprocess.run')
    @patch('parsers.utils.consoleCommands.getBngExecutable', return_value='dummy_bng')
    def test_correctness_success(self, mock_getBngExecutable, mock_run):
        mock_run.return_value = MagicMock(stdout='Some output without error', stderr='')
        result = correctness('dummy.bngl')
        self.assertTrue(result)

    @patch('parsers.utils.consoleCommands.subprocess.run')
    @patch('parsers.utils.consoleCommands.getBngExecutable', return_value='dummy_bng')
    def test_correctness_error(self, mock_getBngExecutable, mock_run):
        mock_run.return_value = MagicMock(stdout='Some output with ERROR inside', stderr='')
        result = correctness('dummy.bngl')
        self.assertFalse(result)

    @patch('parsers.utils.consoleCommands.subprocess.call')
    @patch('parsers.utils.consoleCommands.shutil.which', return_value='/mock/path/killall')
    @patch('parsers.utils.consoleCommands.getBngExecutable', return_value='dummy_bng')
    def test_bngl2xml_timeout(self, mock_getBngExecutable, mock_which, mock_subprocess_call):
        # Configure the mock to raise subprocess.TimeoutExpired on the first call (bng2 execution),
        # and do nothing (return None) on the second call (killall)
        mock_subprocess_call.side_effect = [subprocess.TimeoutExpired(cmd='dummy', timeout=60), None]

        bngl2xml('dummy.bngl')

        # Assert that subprocess.call was called to kill bngdev
        mock_subprocess_call.assert_any_call(['/mock/path/killall', 'bngdev'], shell=False)

if __name__ == '__main__':
    unittest.main()
