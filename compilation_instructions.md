## Compilation from Source

The distribution mentioned above comes with pre-compiled binaries (executables)
for the ODE, SSA and PLA engines. However, developers can compile their own
binaries assuming they have appropriate compilation tools (autoconf, etc).

First, you'll need to pull in or update the git submodules (currently NFsim and
atomizer). To do this, enter the following at the terminal:

    git submodule init
    git submodule update

### Linux and OSX

At a terminal, type the following to build NFsim and atomizer:

    cd <bngpath>
    make
	
### Windows

#### NFsim and atomizer

Make sure you have Ninja, CMake, and Anaconda installed and that Python is in
your path. For more detailed information, please see the NFsim and atomizer
repos. From a PowerShell prompt, type the following:

    cd <bngpath>/atomizer/SBMLparser
    python.exe -m pip install --user -r ./requirements_win.txt
    python.exe -m pip install --target=. python-libsbml
    python.exe -m PyInstaller ./sbmlTranslator.spec
    mv ./dist/sbmlTranslator.exe ../../bin
    cd ../../nfsim_src
    mkdir build
    cd build
    cmake -G "Ninja" ..
    ninja

#### CVODE and muparser

If you have Windows, try Strawberry Perl, (see http://strawberryperl.com/).
Perl is also available for Windows users within the Cygwin environment.

Make sure Cygwin used is for the appropriate Windows architecture (32/64 bit).
Make sure Cygwin has developer tools installed (Select Devel when installing
Cygwin).  Extract folders `cvode-2.6.0` and `muparser_v2_2_4` to
`<bngpath>\Network3` from archives in `<bngpath>\libsource`. In the Cygwin
terminal:

    cd <bngpath>\Network3
    autoreconf --no-recursive --install
    ./configure --disable-shared --prefix=<bngpath>
    make
    make install
    make clean

Using BioNetGen from the Windows command prompt can throw Missing DLL errors.
These DLLs are specific to the Cygwin version used and found in Cygwin\bin.
Copy them to `<bngpath>\bin` to resolve these errors. The required DLLs for the
64-bit version are:

* cygwin1.dll
* cygstdc++-6.dll
* cygz.dll
* cyggcc_s-seh-1.dll.

## Creating a Distribution

Developers preparing a new distribution can simply run the following command:

    cd <bngpath>
    make dist

This will create a tarball called '<bngpath>/BioNetGen-<version>.tgz'.

## Examples

Example models are provided in the Models2 folder. To run them, go to the
Models2 directory, and type:

    ../BNG2.pl <model>.bngl

where <model> is one of the models found in the Models2 directory.

## Validation

To validate the BioNetGen installation:
1) run the Perl script `<bngpath>/Validate/validate_examples.pl`.
2) if any tests fail, look at the log files for more information.

PLEASE NOTE: Stochastic simulation tests rely on statistical validation.
Occasionally a validation will fail due to outliers. The current validation
script only reports stochastic validation failures if they occur TWICE in
a row. Therefore, if this occurs there is a good chance that something is
wrong. To confirm, we suggest rerunning the validation script at least once.
Single tests may be run by passing the model basename as an argument.
