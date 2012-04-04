QUICK START GUIDE

The latest BioNetGen distributions, release notes, installation guides, 
tutorials, etc. are avaiable at http://bionetgen.org.

RuleBender, a Java-based graphical front end for BioNetGen, is available at
http://rulebender.org. RuleBender is supported on Linux,
Mac OS/X and Windows platforms. A compatible version of BioNetGen is included
in the RuleBender distribution. If you are not sure if your version of BNG is
compatible with RuleBender, please use the version distributed with RuleBender.

QUESTIONS? Email the developers at bionetgen.help@gmail.com

To install BioNetgGen, unzip the BioNetGen distribution file, 
e.g. BioNetGen-2.2.0-stable.tar.gz, in the directory where you would like
BioNetGen installed.

Pre-compiled binaries (executables) for the ODE, SSA and PLA engines are
NOT provided with the BNG distribution. Binaries are distributed with
RuleBender (recommended for new users). Pre-compiled binaries may also be
available at the BioNetGen website. If your system is equipped with suitable
compilation tools, the binaries can be compiled at the console as follows:

    cd <bngpath>\Network3
    ./configure --disable-shared --prefix=<bngpath>
    make
    make install

Running BNG2.pl requires a Perl interpreter. If you have Linux or Mac OS/X,
an adequate perl is already installed. If you have Windows, Perl can be
installed in your Cygwin environment (if you have it), or try ActivePerl
(see http://aspn.activestate.com/ASPN/Downloads/ActivePerl/).


Some example model files are provided in Models2. To run them, simply cd to
the Models2 directory, and type:

    ../BNG2.pl <model>.bngl

where <model> is one of the models found in the Models2 directory.

--
Last modified 3 April 2012 for release 2.2.0 testing
