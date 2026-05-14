import unittest
from unittest.mock import patch, MagicMock
import subprocess

import sys

# We need to mock pexpect but have pexpect.TIMEOUT be an Exception.
class MockPexpectTimeout(Exception):
    pass

mock_pexpect = MagicMock()
mock_pexpect.TIMEOUT = MockPexpectTimeout
sys.modules['pexpect'] = mock_pexpect

from parsers.utils.consoleCommands import bngl2xml

class TestConsoleCommands(unittest.TestCase):

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
