import unittest
from unittest.mock import MagicMock, patch
import sys
import os

# Mock dependencies
mock_networkx = MagicMock()
mock_readBNGXML = MagicMock()

sys.modules['networkx'] = mock_networkx
sys.modules['readBNGXML'] = mock_readBNGXML

# Import the module under test
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import collapsedContactMap

class TestCollapsedContactMap(unittest.TestCase):
    def test_main(self):
        # We need to test line 170 which is in the main() function
        # The line is `createCollapsedContact(rules,molecules,[1],outputfilename)`
        # Let's verify it calls parseXML and createCollapsedContact

        # Setup mocks
        mock_molecules = ['mol1', 'mol2']
        mock_rules = ['rule1']
        mock_other = 'other'
        mock_readBNGXML.parseXML.return_value = (mock_molecules, mock_rules, mock_other)

        with patch('collapsedContactMap.createCollapsedContact') as mock_create:
            collapsedContactMap.main('input.xml', 'output')

            # Verify parseXML was called
            mock_readBNGXML.parseXML.assert_called_once_with('input.xml')

            # Verify createCollapsedContact was called with correct arguments
            mock_create.assert_called_once_with(mock_rules, mock_molecules, [1], 'output')

if __name__ == '__main__':
    unittest.main()
