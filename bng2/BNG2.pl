#!/usr/bin/perl

# pragmas
use strict;
use warnings;


# Perl Modules
use File::Spec;
use IO::Handle;
use FindBin;

# get Perl2 Module directory: look for environment variables BNGPATH or BioNetGenRoot.
# If neither are defined, use RealBin module
use lib File::Spec->catdir( (exists $ENV{'BNGPATH'} ? $ENV{'BNGPATH'} :
                                (exists $ENV{'BioNetGenRoot'} ? $ENV{'BioNetGenRoot'} : $FindBin::RealBin)), "Perl2" );

# BNG Modules
use BNGUtils;
use BNGModel;
use Console;



# Termination signal handler: make sure any child processes are shutdown before termination
# TODO: Figure out what termination signal is sent by RuleBender
sub TERM_handler
{
    if ( ${^O} eq 'MSWin32' )
    {   # Windows supports a subset of kill. This variant should work.
        kill -9, $$;
    }
    else
    {   # Send termination signal to process group   
        kill SIGTERM, -$$;
    }
    die "termination signal";
}
$SIG{'TERM'} = \&TERM_handler;


# Defaults params for File mode
our $PARAMS_DEFAULT = { write_xml=>0, write_mfile=>0, write_SBML=>0, generate_network=>0,
                        allow_actions=>1, action_skip_warn=>0, logging=>0, no_exec=>0, allow_perl=>0 };
# Default params for Console mode
our $PARAMS_CONSOLE = { write_xml=>0, write_mfile=>0, write_SBML=>0, generate_network=>0,
                        allow_actions=>0, action_skip_warn=>1, logging=>0, no_exec=>0, allow_perl=>0 };

# Read command line options
my $params = {%$PARAMS_DEFAULT};
while ( @ARGV  and  $ARGV[0] =~ /^-/ )
{
    my $arg = shift @ARGV;
    if ( $arg eq '-check' ) {
        $params->{no_exec} = 1;
    }
    elsif ( $arg eq '-log' ) {
        $params->{logging} = 1;
    }
    elsif ( $arg eq '-v' ){
        printf "BioNetGen version %s\n", BNGversion();
        exit(0);    
    }
    elsif ( $arg eq '-xml' ) {
        $params->{write_xml} = 1;
    }
    elsif ( $arg eq '-mfile' ) {
        $params->{write_mfile} = 1;
    }
    elsif ( $arg eq '-sbml' ) {
        $params->{write_sbml} = 1;
    }
    elsif ( $arg eq '-outdir' )
    {   # specify output directory (default is CWD)
        if (@ARGV) 
        {   $params->{output_dir} = shift @ARGV;   }
        else
        {   exit_error("expected directory name following option -outdir.");  }
    }
    elsif ( $arg eq '-console' ) {
        $params->{console} = 1;
    }
    elsif ( $arg eq '-findbin' )
    {   # ask if BNG can find binary
        if (@ARGV) 
        {   # next argument is binary name
            my $bin = shift @ARGV;
            if ( BNGModel::findExec($bin) )
            {   exit(0);  }
            else
            {   exit(1);  }
        }
        else
        {   exit_error("expected binary name following option -findbin.");  }
    }
    elsif ( $arg eq '-h' )
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
        # make local copy of params
        my $local_params = { %$params };
        # add filename to params
        $local_params->{file} = $file;

        # read and process Model file
        my $err = $model->readFile( $local_params );
        if ($err) {  exit_error($err);  }
        # undefine model
        %$model = ();  undef %$model;
    }
}


# all done!
exit(0);





# Display Help Menu
sub display_help
{
    printf "\nBioNetGen version %s\n", BNGversion();
    print  "--------------------------------------------------/ HELP MENU /----\n"
          ."  SYNOPSIS                                                         \n"
          ."    process MODEL:      BNG2.pl [OPTION]... MODEL...               \n"
          ."    start BNG console:  BNG2.pl -console                           \n"
          ."    display help:       BNG2.pl -h                                 \n" 
          ."    display version:    BNG2.pl -v                                 \n"           
          ."                                                                   \n"
          ."  OPTIONS                                                          \n"
          ."    -log          write log to file MODEL.log (default is STDOUT)  \n"
          ."    -xml          write XML output after processing MODEL          \n"
          ."    -mfile        write MATLAB M-file output after processing MODEL\n"
          ."    -sbml         write SBML output after processing MODEL         \n"
          ."    -check        read MODEL, but do not execute actions           \n"
          ."    -outdir PATH  change default output path                       \n"
          ."                                                                   \n"
          ."  For more information, visit bionetgen.org                        \n"
          ."-------------------------------------------------------------------\n";
}
