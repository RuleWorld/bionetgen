---
title: home
layout: default
---

# Welcome to BioNetGen! <img src="assets/flagman.png" width="75" height="72" valign="bottom"/>


BioNetGen is software designed for modular, structure-based modeling of biochemical reaction networks. It can be applied to many other types of modeling as well. It provides a simple, graph-based syntax that lets users build reaction models out of structured objects that can bind and undergo modification.

### Download

- [VS Code extension for BioNetGen](https://marketplace.visualstudio.com/items?itemName=als251.bngl). We encourage both new and current BioNetGen users to use this interface for developing BioNetGen models. Installation instructions are [here](https://bng-vscode-extension.readthedocs.io/en/latest/install.html). This extension uses [PyBioNetGen](https://pypi.org/project/bionetgen/), which provides Python and command-line interfaces for BioNetGen models.

### Documentation

- Getting Started
     -   [Modeling Reaction Networks with BioNetGen](https://github.com/mcellteam/Workshop_2021/blob/master/bionetgen/slides/CMW2021-IntroToRxnNetModelingBNG.pptx?raw=true). Introduces basic BioNetGen block syntax, actions, and workflows for simulating models, plotting results, and parameter scanning.
     -   [Rule-Based Modeling with BioNetGen](https://github.com/mcellteam/Workshop_2021/blob/master/bionetgen/slides/CMW2021-IntroRBM.pptx?raw=true). Introduces the concepts and syntax for modeling biochemical networks using structured molecules and rules in BioNetGen.
     -   [BioNetGen tutorial on GitHub](/tutorial) Provides detailed examples that demonstrate a wide range of language and modeling features.
- Reference
    -   [Quick Reference Guide](https://github.com/RuleWorld/bionetgen/blob/master/docs/QRG-30Mar2016.pdf)
    -   [Actions and Arguments](https://docs.google.com/spreadsheets/d/1Co0bPgMmOyAFxbYnGCmwKzoEsY2aUCMtJXQNpQCEUag/edit?usp=sharing)
    -   [FAQ](/faq)
- Papers
    -   [Application Note](https://academic.oup.com/bioinformatics/article-lookup/doi/10.1093/bioinformatics/btw469) (open
    access)
    -   [BioNetGen Primer](https://link.springer.com/protocol/10.1007%2F978-1-61779-833-7_9) ([pdf
    –
    free](https://www.dropbox.com/s/rm0535pgom2zr6i/Sekar-RuleBasedPrimer-2012.pdf?dl=0))
    -   [BioNetGen Bible ](https://link.springer.com/protocol/10.1007%2F978-1-59745-525-1_5)([pdf
    –
    free](https://www.csb.pitt.edu/Faculty/Faeder/Publications/Reprints/Faeder_2009.pdf))
    -   [Published BioNetGen models](/published)

### News

- Command line tool for weighted ensemble sampling of BNGL models is released, [GitHub link](https://github.com/ASinanSaglam/webng), [documentation](https://webng.readthedocs.io/en/latest/).
- **Warning MacOS users!** New versions of OS X (11.5 or newer) might force you to switch your default shell to be zsh, see [here](https://support.apple.com/en-us/HT208050). This will break the extension if you are using anaconda python since anaconda python won't be your default python in zsh. Try renaming your `.bash_profile` to `.zprofile`. 

### Getting Help and Reporting Problems
- The best way to get help, report a bug, or request a feature is to post an issue on the [BioNetGen GitHub issues page](https://github.com/RuleWorld/bionetgen/issues).
- You may also send email to <bionetgen.help@gmail.com>. We will treat all help email requests, including models or model snippets, as *confidential*.

### Citing BioNetGen

If you use BioNetGen for a project please cite 
- Harris, L. A. *et al.* [BioNetGen 2.2: advances in rule-based
modeling](https://academic.oup.com/bioinformatics/article-lookup/doi/10.1093/bioinformatics/btw469).
*Bioinformatics* **32,** 3366–3368 (2016).
In addition, consider sending us an 
[email](mailto:bionetgen.help@gmail.com) or tweet 
[@bionetgen](http://twitter.com/bionetgen). We’d love to hear about it!

### Other tools

- [RuleBender](https://github.com/RuleWorld/rulebender/releases/latest) provides an Eclispe-based interface for BioNetGen that includes interactive model visualization capabilities. Installation instructions are [here](https://github.com/RuleWorld/rulebender/blob/master/docs/RuleBender-installation-guide.pdf).
- [BioNetGen command line](https://github.com/RuleWorld/bionetgen/releases/latest). The core BioNetGen code includes a command-line interface that may be useful to developers and advanced users. The BioNetGen code is bundled with both the PyBioNetGen and RuleBender.

### Acknowledgements

Current development of BioNetGen is supported in part by the
NIGMS-funded (P41GM103712) National Center for Multiscale Modeling of
Biological Systems
([MMBioS](http://mmbios.org/ "http://mmbios.org")).
Past support has been provided by NIH grants (GM076570, GM103712,
GM085273, AI35997, CA109552), NSF grant 0829788, the Arizona Biomedical
Research Commission, and the Department of Computational Biology at the
University of Pittsburgh School of Medicine.

BioNetGen and this web site are maintained by the [Lab of James Faeder](http://www.csb.pitt.edu/Faculty/Faeder/) in the [Department of Computational and Systems Biology](http://www.csb.pitt.edu) and the University of Pittsburgh School of Medicine.