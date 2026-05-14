import unittest
from unittest.mock import MagicMock, patch
import sys
import os

# Mock lxml before importing
sys.modules['lxml'] = MagicMock()
sys.modules['lxml.etree'] = MagicMock()

# Add parent directory to path so we can import the module
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import bpgMaps

class TestBpgMaps(unittest.TestCase):
    def setUp(self):
        self.names = MagicMock()

        # Mock patterns
        self.p1 = MagicMock()
        self.p2 = MagicMock()
        self.names.p = {self.p1: 'p1', self.p2: 'p2'}

        # Mock transformations
        self.t1 = MagicMock()
        self.t1.action = "AddBond"
        self.t1.isSynDel.return_value = False
        self.t1.getLHS.return_value = [self.p1]
        self.t1.getRHS.return_value = [self.p2]

        self.t2 = MagicMock()
        self.t2.action = "DeleteBond"
        self.t2.isSynDel.return_value = False
        self.t2.getLHS.return_value = [self.p2]
        self.t2.getRHS.return_value = [self.p1]

        self.names.t = {self.t1: 't1', self.t2: 't2'}

        # Mock rules
        self.r1 = MagicMock()
        self.r1.transformations = [self.t1]
        self.r1.transf_center_lhs = [[self.p1]]
        self.r1.context = []
        self.r1.syndel_context = [[]]
        self.r1.syncontext = [[]]
        self.r1.delcontext = [[]]

        self.names.r = {self.r1: 'r1'}

        # Mock transformation pairs
        self.tp1 = MagicMock()
        self.tp1.forward = self.t1
        self.tp1.reverse = self.t2

        self.names.tp = {self.tp1: 'tp1'}

        # Mock irreversibles
        self.names.irr = {}

    def test_getMaps_verbose_false(self):
        """Test getMaps with verbose=False"""
        maps = bpgMaps.getMaps(self.names, verbose=False)

        self.assertIsInstance(maps, bpgMaps.allMaps)
        self.assertEqual(maps.r.r2t, {('r1', 't1')})
        self.assertEqual(maps.t.t2action, {'t1': 'AddBond', 't2': 'DeleteBond'})
        self.assertEqual(maps.tp.tp2t_forward, {'tp1': 't1'})
        self.assertEqual(maps.tp.tp2t_reverse, {'tp1': 't2'})

    @patch('sys.stdout', new_callable=MagicMock)
    def test_getMaps_verbose_true(self, mock_stdout):
        """Test getMaps with verbose=True to cover print statements"""
        maps = bpgMaps.getMaps(self.names, verbose=True)

        self.assertIsInstance(maps, bpgMaps.allMaps)
        self.assertEqual(maps.r.r2t, {('r1', 't1')})
        self.assertEqual(maps.t.t2action, {'t1': 'AddBond', 't2': 'DeleteBond'})
        self.assertEqual(maps.tp.tp2t_forward, {'tp1': 't1'})
        self.assertEqual(maps.tp.tp2t_reverse, {'tp1': 't2'})

        calls = [call[0][0] for call in mock_stdout.write.call_args_list]
        output = "".join(calls)
        self.assertIn("Building maps between elements...", output)
        self.assertIn("Maps to reaction rules:", output)
        self.assertIn("Maps to transformations:", output)
        self.assertIn("Maps to transformation pairs:", output)

if __name__ == '__main__':
    unittest.main()
