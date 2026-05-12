import unittest
from unittest.mock import patch, MagicMock

import sys
sys.modules['lxml'] = MagicMock()
sys.modules['lxml.etree'] = MagicMock()
sys.modules['pygraphviz'] = MagicMock()
mock_pyparsing = MagicMock()
sys.modules['pyparsing'] = mock_pyparsing
sys.modules['pexpect'] = MagicMock()

from parsers.utils.consoleCommands import correctness

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

if __name__ == '__main__':
    unittest.main()
