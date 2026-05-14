import unittest
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../..')))

from parsers.utils.extractAtomic import solveWildcards

class DummyMolecule:
    def __init__(self, name):
        self.name = name

class DummyElement:
    def __init__(self, molecules):
        self.molecules = molecules

class TestSolveWildcards(unittest.TestCase):

    def test_empty_array(self):
        atomic_array = {}
        solveWildcards(atomic_array)
        self.assertEqual(atomic_array, {})

    def test_no_wildcards(self):
        element = DummyElement([DummyMolecule("A"), DummyMolecule("B")])
        atomic_array = {"A_B": element}
        solveWildcards(atomic_array)
        self.assertEqual(atomic_array, {"A_B": element})

    def test_wildcards_no_resolution(self):
        w_element = DummyElement([DummyMolecule("A")])
        atomic_array = {"A_+": w_element}
        solveWildcards(atomic_array)
        self.assertEqual(atomic_array, {"A_+": w_element})

    def test_wildcards_resolved(self):
        element = DummyElement([DummyMolecule("A"), DummyMolecule("B")])
        w_element = DummyElement([DummyMolecule("A")])

        atomic_array = {
            "A_B": element,
            "A_+": w_element
        }

        solveWildcards(atomic_array)

        # A_+ should now point to a list containing `element`
        self.assertEqual(atomic_array["A_B"], element)
        self.assertEqual(atomic_array["A_+"], [element])

    def test_wildcards_with_single_molecule_non_wildcard(self):
        # Non-wildcard has only 1 molecule, so it won't be used to resolve
        element = DummyElement([DummyMolecule("A")])
        w_element = DummyElement([DummyMolecule("A")])

        atomic_array = {
            "A": element,
            "A_+": w_element
        }

        solveWildcards(atomic_array)

        self.assertEqual(atomic_array["A"], element)
        self.assertEqual(atomic_array["A_+"], w_element)

    def test_multiple_resolutions(self):
        element1 = DummyElement([DummyMolecule("A"), DummyMolecule("B")])
        element2 = DummyElement([DummyMolecule("A"), DummyMolecule("C")])
        w_element = DummyElement([DummyMolecule("A")])

        atomic_array = {
            "A_B": element1,
            "A_C": element2,
            "A_+": w_element
        }

        solveWildcards(atomic_array)

        self.assertEqual(atomic_array["A_B"], element1)
        self.assertEqual(atomic_array["A_C"], element2)
        # Should contain both element1 and element2
        self.assertIn(element1, atomic_array["A_+"])
        self.assertIn(element2, atomic_array["A_+"])
        self.assertEqual(len(atomic_array["A_+"]), 2)

    def test_empty_molecules(self):
        element1 = DummyElement([])
        w_element = DummyElement([])

        atomic_array = {
            "A_B": element1,
            "A_+": w_element
        }

        with self.assertRaises(IndexError):
            solveWildcards(atomic_array)

    def test_multiple_wildcards(self):
        element1 = DummyElement([DummyMolecule("A"), DummyMolecule("B")])
        element2 = DummyElement([DummyMolecule("C"), DummyMolecule("D")])
        w_element1 = DummyElement([DummyMolecule("A")])
        w_element2 = DummyElement([DummyMolecule("C")])

        atomic_array = {
            "A_B": element1,
            "C_D": element2,
            "A_+": w_element1,
            "C_+": w_element2
        }

        solveWildcards(atomic_array)

        self.assertEqual(atomic_array["A_+"], [element1])
        self.assertEqual(atomic_array["C_+"], [element2])

if __name__ == "__main__":
    unittest.main()
