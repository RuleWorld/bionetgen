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

| Software | Description |
| -------- | ----------- |
| [AMICI](https://amici.readthedocs.io/en/latest/index.html) | Provides an interface to the CVODES and IDAS solvers for models specified in SBML or [PySB](https://pysb.org/), both of which are capable of representing BNGL models |
| [BioSimulators](https://biosimulators.org/)| Provides a registry of biosimulation tools supporting many different model formats, including BNGL |
| [BioUML](https://www.biouml.org/) | Java platform for -omics data analysis; a BNG plugin allows users to edit rule-based models as either BioUML diagrams or as BNGL text files |
| [Kappa](https://kappalanguage.org/) | Another prominent rule-based modeling language |
| [MCell](https://mcell.org/) | Uses 3D cellular models and Monte Carlo algorithms for simulating the movements and reactions of molecules within and between cells; the latest version, MCell4, uses BNGL syntax for rule-based modeling |
| [ML-Rules](https://git.informatik.uni-rostock.de/mosi/mlrules2) |  |
<!-- | [ModelingToolkit.jl](https://mtk.sciml.ai/stable/) |  | -->
| [NFsim](http://michaelsneddon.net/nfsim/) | Biochemical reaction simulator designed to handle large numbers of possible molecular interactions or states; uses BNGL for model specification |
| Parts & Pools | Employs BioNetGen to help design synthetic gene circuits | <!-- Can't find a suitable link or download site -->
| [PyBioNetFit](https://bionetfit.nau.edu/) | Provides a suite of algorithms for parameterizing models specified in BNGL or SBML |
| [PySB](https://pysb.org/) | Allows for building models of biochemical systems as Python programs; internally translates Python code into BNGL or Kappa rules |
| [RuleDSD](https://aaltodoc.aalto.fi/handle/123456789/43847) | Converts a DNA Strand Displacement (DSD) model into BNGL, then uses [PySB](https://pysb.org/) to perform simulation and analysis |
<!-- | [SBML_Multi]() |  | [Is this a software or just a set of standards?] -->
| [Smoldyn](https://www.smoldyn.org/) | Performs cell-scale biochemical simulations; can perform rule-based modeling using either its native wildcards or by running BioNetGen on BNGL files |
| [SPATKIN](https://pmbm.ippt.pan.pl/web/Spatkin) | Uses an extension of BNGL to simluate biomolecular interactions on or at a 2D surface |
| [SRSim](https://www.biosys.uni-jena.de/Members/Gerd+Gruenert/SRSim.html) | Spatially simulates Brownian Dynamics of rule-based reaction systems (defined in BNGL) in 3D |
| [VirtualCell](https://vcell.org/bionetgen/) | Cell biological systems modeling platform that uses a central database; BioNetGen@VCell can be used to perform rule-based modeling in the platform |
