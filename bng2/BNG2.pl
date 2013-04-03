#!/usr/bin/perl

#    BioNetGen : rule-based modeling language and simulation platform
#
#                  Copyright (C) 2006,2009,2012 by
#
#      James R. Faeder    (faeder at pitt dot edu)
#      Justin S. Hogg     (justinshogg at gmail dot com)
#      Leonard A. Harris  (lh64 at cornell dot com)
#      John A. P. Sekar
#      Jose Juan Tapia
#      Arshi Arora
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

# pragmas
use strict;
use warnings;


# Perl Modules
use File::Spec;
use IO::Handle;
use FindBin;
use Config;

# get Perl2 Module directory: look for environment variables BNGPATH or BioNetGenRoot.
# If neither are defined, use RealBin module
use lib File::Spec->catdir( (exists $ENV{'BNGPATH'} ? $ENV{'BNGPATH'} :
                                (exists $ENV{'BioNetGenRoot'} ? $ENV{'BioNetGenRoot'} : $FindBin::RealBin)), "Perl2" );

# BNG Modules
use BNGUtils;
use BNGModel;
use Console;


# Set up Signal Handlers..
# Define global variable to store PID of child process.
$::CHILD_PID = undef;
# Get signal names
my $i = 0;
my %SIGNO=();
defined($Config{sig_name}) or die "No signals defined";
foreach my $signame ( split " ", $Config{sig_name} )
{
    $SIGNO{$signame} = $i;
    $i++;
}
# TERM signal handler: make sure any child processes are shutdown before termination
$SIG{'TERM'} = sub
{
    if (defined $::CHILD_PID)
    {   # kill off child process
        print "\n>>> relaying TERM signal to child with PID: ", $::CHILD_PID, " <<<\n";
        kill $SIGNO{"TERM"}, $::CHILD_PID;
    }
    exit_error( sprintf "BioNetGen received TERM signal (%d)", $SIGNO{"TERM"} );
};
# INT signal handler: make sure any child processes are shutdown before termination
$SIG{'INT'} = sub
{
    if (defined $::CHILD_PID)
    {   # kill off child process
        print "\n>>> relaying INT signal to child with PID: ", $::CHILD_PID, " <<<\n";
        kill $SIGNO{"INT"}, $::CHILD_PID;
    }
    exit_error( sprintf "BioNetGen received TERM signal (%d)", $SIGNO{"INT"} );
};



# Defaults params for File mode
my $PARAMS_DEFAULT = { write_xml=>0, write_mfile=>0, write_SBML=>0, generate_network=>0,
                       allow_actions=>1, action_skip_warn=>0, logging=>0, no_exec=>0, allow_perl=>0,
                       no_nfsim=>0 };
# Default params for Console mode
my $PARAMS_CONSOLE = { write_xml=>0, write_mfile=>0, write_SBML=>0, generate_network=>0,
                       allow_actions=>0, action_skip_warn=>1, logging=>0, no_exec=>0, allow_perl=>0,
                       no_nfsim=>0 };

# Read command line options
my $params = {%$PARAMS_DEFAULT};
while ( @ARGV  and  $ARGV[0] =~ s/^(-{1,2})// )
{
    my $arg = shift @ARGV;
    if ( $arg eq 'check' ) {
        $params->{no_exec} = 1;
    }
    elsif ( $arg eq 'log' ) {
        $params->{logging} = 1;
    }
    elsif ( $arg =~ /^(v|version)$/ ){
        printf "BioNetGen version %s\n", BNGversion();
        exit(0);    
    }
    elsif ( $arg eq 'xml' ) {
        $params->{write_xml} = 1;
    }
    elsif ( $arg eq 'mfile' ) {
        $params->{write_mfile} = 1;
    }
    elsif ( $arg eq 'sbml' ) {
        $params->{write_sbml} = 1;
    }
    elsif ( $arg eq 'outdir' )
    {   # specify output directory (default is CWD)
        if (@ARGV) 
        {   $params->{output_dir} = shift @ARGV;   }
        else
        {   exit_error("expected directory name following option 'outdir'.");  }
    }
    elsif ( $arg eq 'console' ) {
        $params->{console} = 1;
    }
    elsif ( $arg eq 'findbin' )
    {   # ask if BNG can find binary
        if (@ARGV) 
        {   # next argument is binary name
            my $bin = shift @ARGV;
            if ( BNGModel::findExec($bin) )
            {   exit 0;  }
            else
            {   exit 1;  }
        }
        else
        {   exit_error("expected binary name following option 'findbin'.");  }
    }
    elsif ( $arg eq 'no-nfsim' ) {
        $params->{no_nfsim} = 1;
    }
    elsif ( $arg =~ /^(h|help)$/ )
    {   # show help menu and exit
        display_help();
        exit(0);
    }
    else {
        exit_error("Unrecognized command line option $arg");
    }
}




# check/set output directory
unless ( $params->{outdir} )
{   # set to current directory
    $params->{outdir} = File::Spec->curdir();
}
unless ( -d $params->{outdir} )
{   #  output directory is not a directory
    send_warning( sprintf "Default output directory %s is not a directory.", $params->{outdir} );
}
unless ( -w $params->{outdir} )
{   # can't write to output directory
    send_warning( sprintf "Not able to write to default output directory %s.", $params->{outdir} );
}




if ( $params->{console} )
{
    # Start BioNetGen console!!
    # use default params for the console (ignore most command line arguments!)
    my $local_params = {%$PARAMS_CONSOLE};
    # but do get the output_dir argument
    if ( defined $params->{output_dir} )
    {  $local_params->{output_dir} = $params->{output_dir};  }
    # start console
    my $err = BNGconsole( $local_params );
    if ($err) {  exit_error($err);  }
}
else
{
    # Process any files
    while ( my $file = shift @ARGV )
    {
        # create BNGMOdel object
        my $model = BNGModel->new();
        $BNGModel::GLOBAL_MODEL = $model;
        # make local copy of params
        my $local_params = { %$params };
        # add filename to params
        $local_params->{file} = $file;

        # read and process Model file
        my $err = $model->readFile( $local_params );
        if ($err) {  exit_error($err);  }
        # undefine model
        %$model = ();  undef %$model;
        $BNGModel::GLOBAL_MODEL = undef;
    }
}


# all done!
exit(0);





# Display Help Menu
sub display_help
{
    printf "\nBioNetGen version %s\n", BNGversion();
    print  "--------------------------------------------------/ HELP MENU /-----\n"
          ."  SYNOPSIS                                                          \n"
          ."    process MODEL:      BNG2.pl [OPTION]... MODEL...                \n"
          ."    start BNG console:  BNG2.pl --console                           \n"
          ."    display help:       BNG2.pl -h                                  \n" 
          ."    display version:    BNG2.pl -v                                  \n"           
          ."                                                                    \n"
          ."  OPTIONS                                                           \n"
          ."    --log          write log to file MODEL.log (default is STDOUT)  \n"
          ."    --xml          write XML output after processing MODEL          \n"
          ."    --mfile        write MATLAB M-file output after processing MODEL\n"
          ."    --sbml         write SBML output after processing MODEL         \n"
          ."    --check        read MODEL, but do not execute actions           \n"
          ."    --outdir PATH  change default output path                       \n"
          ."                                                                    \n"
          ."  For more information, visit bionetgen.org                         \n"
          ."--------------------------------------------------------------------\n";
}

