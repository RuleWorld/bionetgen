
## 2024-05-14 - Python micro-optimizations inside BioNetGen Parsers
**Learning:** In the `ContactMap` and `utils` parsers, components often perform O(N) lookups inside tight loops during `Molecule.addComponent` and `Molecule.getComponent`. Method calls (`component.getName()`) and redundant list comprehensions (`[x.name for x in self.components]`) represent significant call-stack overhead and hidden loop overhead.
**Action:** When working in these modules or porting Python 2 structures, always use direct attribute access (`x.name` vs `x.getName()`) and prefer native explicit variable initialization and short-circuit search loops over implicit O(N) generator expressions to maximize AST operations speed.
