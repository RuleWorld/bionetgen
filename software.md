---
title: Software
layout: default
permalink: /software
parent: Uses of BioNetGen
nav_order: 2
# TODO: Sort software into those that use BNG & those that support BNGL models; add blurbs about usage/compatibility
---

# Software that Uses BioNetGen
This page links to other software that use BioNetGen code or BNGL-formatted models. *Note: This list is a work in progress and may not be complete.*

## Software
<!-- DON'T ADD ANOTHER PIPE (" | ") AT THE END; adds anextra column for some reason -->

| Software | Description 
| -------- | ----------- 
| [AMICI](https://amici.readthedocs.io/en/latest/index.html) | Provides an interface to the CVODES and IDAS solvers for models specified in SBML or [PySB](https://pysb.org/), both of which are capable of representing BNGL models
| [BioSimulators](https://biosimulators.org/)| Provides a registry of biosimulation tools supporting many different model formats, including BNGL
| [BioUML](https://www.biouml.org/) | Java platform for -omics data analysis; a BNG plugin allows users to edit rule-based models as either BioUML diagrams or as BNGL text files
| [Kappa](https://kappalanguage.org/) | Another prominent rule-based modeling language. The syntax used to be very similiar to BNGL, but that is less true now.
| [MCell](https://mcell.org/) | Uses 3D cellular models and Monte Carlo algorithms for simulating the movements and reactions of molecules within and between cells; the latest version, MCell4, uses BNGL syntax for rule-based modeling
| [ML-Rules](https://git.informatik.uni-rostock.de/mosi/mlrules2) | Multi-level rule-based modeling formalism for modeling biochemical reaction networks
| [MolClustPy](https://molclustpy.github.io/) | A Python package to characterize multivalent biomolecular clusters [Chattaraj et al.](https://academic.oup.com/bioinformatics/article/39/6/btad385/7199581)
| [NFsim](http://michaelsneddon.net/nfsim/) | Biochemical reaction simulator designed to handle large numbers of possible molecular interactions or states; uses BNGL for model specification
| Parts & Pools | Employs BioNetGen to help design synthetic gene circuits <!-- Can't find a suitable link or download site -->
| [PEtab](https://petab.readthedocs.io/en/latest/) | Data format for specifying systems biology parameter estimation problems using SBML
| [Pleione](https://pleione.readthedocs.io/en/latest/) | Provides methods for parameterizing models written in BNGL or *kappa*
| [PyBioNetFit](https://bionetfit.nau.edu/) | Provides a suite of algorithms for parameterizing models specified in BNGL or SBML
| [pyPESTO](https://pypesto.readthedocs.io/en/latest/#) | Python toolbox for parameter estimation, including options to interface with [AMICI](https://amici.readthedocs.io/en/latest/index.html) and [PEtab](https://petab.readthedocs.io/en/latest/)
| [PySB](https://pysb.org/) | Allows for building models of biochemical systems as Python programs; internally translates Python code into BNGL or Kappa rules
| [PyViPR](https://pyvipr.readthedocs.io/en/latest/) | IPython widget for interactively visualizing systems biology models using [PySB](https://pysb.org/); supports BNGL and SBML models
| [RuleDSD](https://aaltodoc.aalto.fi/handle/123456789/43847) | Converts a DNA Strand Displacement (DSD) model into BNGL, then uses [PySB](https://pysb.org/) to perform simulation and analysis
| [Smoldyn](https://www.smoldyn.org/) | Performs cell-scale biochemical simulations; can perform rule-based modeling using either its native wildcards or by running BioNetGen on BNGL files
| [SPATKIN](https://pmbm.ippt.pan.pl/web/Spatkin) | Uses an extension of BNGL to simluate biomolecular interactions on or at a 2D surface
| [SRSim](https://www.biosys.uni-jena.de/Members/Gerd+Gruenert/SRSim.html) | Spatially simulates Brownian Dynamics of rule-based reaction systems (defined in BNGL) in 3D
| [VDNA-Lab](https://github.com/Frankie-Spencer/virtual_dna_lab) | Employs rule-based modeling (in BNGL format) to simulate DNA assembly systems
| [VirtualCell](https://vcell.org/bionetgen/) | Cell biological systems modeling platform that uses a central database; BioNetGen@VCell can be used to perform rule-based modeling in the platform
