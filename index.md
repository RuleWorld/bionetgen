---
title: Home
layout: default
nav_order: 1
---

{% include googlebody.html %}

# Welcome to BioNetGen! <img src="assets/flagman.png" width="75" height="72" valign="bottom" alt="Flagman"/>

BioNetGen is software designed for modular, structure-based modeling of biochemical reaction networks. It can be applied to many other types of modeling as well. It provides a simple, graph-based syntax that lets users build reaction models out of structured objects that can bind and undergo modification.

## Using BioNetGen with the VS Code Extension

{% include home_slideshow.html %}

*Note*: yEd graph editor must be [installed separately](https://www.yworks.com/products/yed).

## Download
To begin using BioNetGen, see the [installation instructions](https://bng-vscode-extension.readthedocs.io/en/latest/install.html). This will guide new users through installing VS Code and the BNG extension.

The best way to get help, report a bug, or request a feature is to post an issue on the appropriate project's GitHub issues page. Otherwise, you may send an email to <bionetgen.help@gmail.com>. All help requests, including models or model snippets, will be treated *confidentially*.

## News
 - If you've spent some time browsing this site, please consider filling out this [feedback form](https://forms.gle/T4vuoMT23u6F8Vj7A) to help improve it!
 - A command line tool for weighted ensemble sampling of BNGL models, **WEBNG**, has been released. See the [GitHub repository](https://github.com/ASinanSaglam/webng) and the [documentation](https://webng.readthedocs.io/en/latest/).
 - **WARNING for MacOS users**: New versions of OS X (11.5 or newer) might force you to switch your default shell to zsh (see [here](https://support.apple.com/en-us/HT208050)). This will break the extension if you are using Anaconda Python, since it will no longer be your default Python in zsh. Try renaming your `.bash-profile` to `.zprofile`.

## Citing BioNetGen
If you use BioNetGen for a project please cite 
- Harris, L. A. *et al.* [BioNetGen 2.2: advances in rule-based
modeling](https://academic.oup.com/bioinformatics/article-lookup/doi/10.1093/bioinformatics/btw469).
*Bioinformatics* **32,** 3366–3368 (2016).

In addition, consider sending us an 
[email](mailto:bionetgen.help@gmail.com) or tweet 
[@bionetgen](http://twitter.com/bionetgen). We’d love to hear about it!

## Acknowledgements
Current development of BioNetGen is supported in part by the
NIGMS-funded (P41GM103712) National Center for Multiscale Modeling of
Biological Systems
([MMBioS](http://mmbios.org/ "http://mmbios.org")).
Past support has been provided by NIH grants (GM076570, GM103712,
GM085273, AI35997, CA109552), NSF grant 0829788, the Arizona Biomedical
Research Commission, and the Department of Computational Biology at the
University of Pittsburgh School of Medicine.

BioNetGen and this website are maintained by the [Lab of James Faeder](http://www.csb.pitt.edu/Faculty/Faeder/) in the [Department of Computational and Systems Biology](http://www.csb.pitt.edu) and the University of Pittsburgh School of Medicine.
