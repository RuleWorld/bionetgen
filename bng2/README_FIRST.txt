QUICK START GUIDE

The latest BioNetGen distributions, release notes, installation guides, 
tutorials, etc. are available at http://bionetgen.org.

RuleBender, a Java-based graphical front end for BioNetGen, is available at
http://rulebender.org. RuleBender is supported on Linux, Mac OS/X, and Windows
platforms. A compatible version of BioNetGen is included in the RuleBender
distribution. If you are not sure if your version of BNG is compatible with
RuleBender, please use the version distributed with RuleBender.

QUESTIONS?
Email the developers at bionetgen.help@gmail.com

To install BioNetGen, unzip the BioNetGen distribution file, 
e.g. BioNetGen-2.2.0-stable.tar.gz, in the directory where you would like
BioNetGen installed:

    cd <installdir>
    tar -zxvf BioNetGen-2.2.0-stable.tar.gz

Running BNG2.pl requires a Perl interpreter. If you have Linux or Mac OS/X,
an adequate perl is already installed. If you have Windows, try ActivePerl
(see http://aspn.activestate.com/ASPN/Downloads/ActivePerl/). Perl is also
available for Windows users within the Cygwin environment.

Pre-compiled binaries (executables) for the ODE, SSA and PLA engines are
provided for Linux, OS/X, and Windows. If your system is equipped with suitable
compilation tools, you may compile your own binaries from a console as follows:

    cd <bngpath>\Network3
    ./configure --disable-shared --prefix=<bngpath>
    make
    make install

PLEASE NOTE: This procedure will not work if the directories 'cvode-2.6.0' and 
'muparser_v2_2_4' do not exist. If that is the case, they can be extracted from
the archive files in BNGPATH/libsource (available in the repository version of 
the code at https://code.google.com/p/bionetgen/).

When compiling binaries for 64-bit Windows (Vista/7), make sure that the 64-bit 
version of Cygwin is installed including the latest version of required developer tools 
(gcc,g++,make,autoconf). Compile from the Cygwin prompt as above. The compiled binary 
can be executed as is from the Cygwin prompt. Executing from the Windows command prompt 
will throw Missing DLL errors. Copying these DLLs from the Cygwin bin folder to the 
BioNetGen bin folder should resolve these errors.

Alternatively, binaries can be compiled using CMake. Download CMake from
http://www.cmake.org/ and add to PATH. Then:

	cd <bngpath>\Network3
	cp Makefile.cmake Makefile
	make
	
Example models are provided in the Models2 folder. To run them, go to
the Models2 directory, and type:

    ../BNG2.pl <model>.bngl

where <model> is one of the models found in the Models2 directory.

To validate the BioNetGen installation:
1) run the Perl script BNGPATH/Validate/validate_examples.pl.
2) if any tests fail, look at the log files for more information.

PLEASE NOTE: Stochastic simulation tests rely on statistical validation.
Occasionally a validation will fail due to outliers. The current validation
script only reports stochastic validation failures if they occur TWICE in
a row. Therefore, if this occurs there is a good chance that something is
wrong. To confirm, we suggest rerunning the validation script at least once.
Single tests may be run by passing the model basename as an argument.

TROUBLESHOOTING:
If you see the message "ABORT: Could not find executable run_network". Try
setting the environment variable "BNGPATH" to the root directory of your 
BioNetGen installation. Under bash-like shells, this is done by typing:
"export BNGPATH=<bngpath>", where <bngpath> is the root directory of BNG.

--
Last modified 13 December 2014 for release 2.2.6-testing
