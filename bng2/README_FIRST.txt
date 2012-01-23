QUICK INSTALLATION INSTRUCTIONS 

The latest distributions, release notes, installation guides, etc. are avaiable
at http://bionetgen.org.

To install, simply unzip the BioNetGen distribution file BioNetGen_x.y.z.tar.gz
in the directory where you would like BioNetGen installed.

Pre-compiled binaries for the simulation back end are NOT provided, but may be
availble for download at the BioNetGen website or as part of a RuleBender
distribution. Users of GNU/Linux and Mac OS/X can compile the binaries by
opening a shell to the BioNetGen root directory and executing 'make'.

Running Perl2/BNG2.pl requires the perl interpreter to be installed on your
machine.  If you have Linux or Mac OS X, an adequate perl is already
installed.  If you have Windows, Perl can be installed in your Cygwin
environment (if you have it), or it ActivePerl can be installed (see
http://aspn.activestate.com/ASPN/Downloads/ActivePerl/).

RuleBender, the graphical front-end for BioNetGen is available at
http://rulebender.org.  Rulebender requires Java Runtime 1.5.0+.

Some example model files are provided in Models2.  To run them, simply cd to
the Models2 directory, and type:
 > ../Perl2/BNG2.pl <model>.bngl
where <model> is one of the models found in the Models2 directory.

A simple demo model is provided in this directory (egfr_demo.bng).  Consult
RuleBuilder_GettingStarted.pdf for detailed instructions in using the program.
If you wish to run a simulation, click on the settings menu item under the
File menu.  You will be prompted for a path to BNG2.pl and a temporary file.
BNG2.pl is in BioNetGen_x.y.z/Perl2.  The temp file can be placed anywhere.

--
Last modified June 29, 2011 for release 2.1.8 beta
