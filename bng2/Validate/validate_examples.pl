#!/usr/bin/perl
# Validation script for BioNetGen suite.
# 
# SYNOPSIS:
#   validate_examples.pl [OPTIONS]              : validate all models
#   validate_examples.pl [OPTIONS] MODEL...     : validate MODEL
#   validate_Examples.pl --help                 : display help menu
#
# Exits with value 0 if all validation tests are passed, otherwise exits
#  with a positive value equal to the number of failed tests, or -1 (=255) if
#  there was some problem executing the validation.
#
# A log file named MODEL.log is created for each validation model. If a test
#  fails, check the log file for more details.
#
# What's New?
#   8sep2011: validation of equilibrium distribution for SSA models
#  19apr2012: better support under Windows OS
#  19apr2012: improved logfiles and new console "look"
#
#
# More Details:
#   This script will automatically recognize and validate the following output
#    files for each MODEL in the models directory:
#
#   output file             reference file         : description                                 
#  ----------------------------------------------------------------------------
#   MODEL.net             : MODEL.net              : reaction network
#   MODEL.xml             : MODEL.xml              : XML specification
#   MODEL.gdat            : MODEL.gdat             : ODE observables trajectory
#   MODEL.cdat            : MODEL.cdat             : ODE species trajectory
#   MODEL_ssa_equil.gdat  : MODEL_ssa_equil.stats  : SSA equilibrium samples   
#   MODEL_nf_equil.gdat   : MODEL_nf_equil.stats   : NFsim equilibrium samples
#
#
# To add new validation MODEL:
#  1) Create MODEL.bngl, including actions that generate some or all of
#       the recognized output files.
#  2) Put the validation model in $modeldir directory.
#  3) Generate reference output files and place in the $datdir subdirectory.**
#
#  ** for most validations, the reference file shares the same format and name
#    as the output file. However, special reference files are required for
#    validating stochastic equilibrium samples. These reference files have a
#    ".stat" extension and describe a binned equilibrium distribution of a
#    model observable. See $datdir/gene_expr_ssa_equil.stats for an example.
#    Equilibrium samples are compared to the reference distribution by a
#    Chi-square goodness of fit test. The stats file defines the observable of
#    interest, bin widths and probabilities, and the Chi-square values
#    corresponding to various significance levels.
#
#  ** It is the Modeler's responsibility to validate the reference trajectory.
#    It's advisable to compare the reference to analytic results, simulations
#    reported in the literature, or simulations generated from independent
#    platforms (e.g. MATLAB).
#
# TODO: Add validations for the following
#   non-equilibrium stochastic validation (multirun validation)
#   Syntax error checking
#   Canonical labeling
#   On-the-fly simulation

use strict;
use warnings;
# Perl Modules
use FindBin;
use File::Spec;
use Getopt::Long;
use Scalar::Util qw( looks_like_number );
use Config;
use IO::Handle;
use IPC::Open3;


### PARAMETERS ###

# perl binary
my $perlbin = $^X;

# BNG root directory
my $bngpath;
my $bngexec;
{
    # try to find path in environment variables
    $bngpath = (exists $ENV{'BNGPATH'} ? $ENV{'BNGPATH'} :
                                (exists $ENV{'BioNetGenRoot'} ? $ENV{'BioNetGenRoot'} : undef) );
    unless (defined $bngpath)
    {   # use FindBin to locate BNG
        my ($volume,$directories,$file) = File::Spec->splitpath( $FindBin::RealBin );
        my @dirs = File::Spec->splitdir( $directories );
        pop @dirs;   # BNG executable script should be down one directory from here
        $bngpath = File::Spec->catpath( $volume, File::Spec->catdir(@dirs), "" );
    }
    # define executable
    $bngexec = File::Spec->catfile( $bngpath, "BNG2.pl" );
}

# directory containing validation reference files
my $datdir   = File::Spec->catdir( $bngpath, 'Validate', 'DAT_validate' );
# directory containing models
my $modeldir = File::Spec->catdir( $bngpath, 'Validate' );
# output directory
my $outdir = $modeldir; #File::Spec->curdir();
# compare species and rxns script
my $compare_species = File::Spec->catfile( $modeldir, 'compare_species.pl' );
my $compare_rxn = File::Spec->catfile( $modeldir, 'compare_rxn.pl' );
 # if true, delete output files after validation
my $delete_working_files = 1;   
# size of indent to STDOUT       
my $INDENT = '  ';
# seperator 
our $SEPARATOR = "-" x 79 . "\n";
# p-value for statistical validation (recommend 0.01)
#   Options are 0.05, 0.02, 0.01, 0.005, 0.002, 0.001.
#   Keep in mind that validation will fail (100*p) percent of the time, even
#   if the simulator is generating the correct distribution!!                
my $pvalue = 0.01;
# try to validate NFsim?                  
my $check_nfsim = 1;
# arguments for BNG
my @bngargs = ();
# models to exclude
my @exclude = ();

###                                                          ###
###  BEGIN MAIN SCRIPT, no user options beyond this point!!  ###
###                                                          ###

# Greet the User
print "\n---[ BioNetGen Validation Utility ]---\n\n";

# parse command line arguments
GetOptions( 'help|h'        => sub { display_help(); exit(0); },
            'bngpath=s'     => \$bngpath,
            'modelpath=s'   => \$modeldir,
            'datpath=s'     => \$datdir,
#            'outpath=s'     => \$outdir,
            'pvalue=f'      => \$pvalue,
            'nfsim!'        => \$check_nfsim,
            'delete-files!' => \$delete_working_files,
            'exclude:s'     => \@exclude
          )
or die "Error in command line arguments (try: validate_examples.pl --help)";

# get models to validate
my @models;
if (@ARGV)
{   # remaining command line arguments are models
    @models = (@ARGV);
    foreach my $model (@models)
    {   # strip trailing .bngl, if any
        $model =~ s/\.bngl$//;
    }
}
else
{   # run validations on all models in $modeldir
    my $dh;
    unless( opendir $dh, $modeldir )
    {
        print "Error: validate_examples can't open directory $modeldir: $!\n";
        exit(-1);
    }
    # strip ".bngl" extension
    @models = map { /^(.+)\.bngl$/ } ( grep {/^.+\.bngl$/} readdir($dh) );
    # sort models alphabetically
    @models = sort {"\L$a" cmp "\L$b"} @models; # case-insensitive sort
    # done with directory
    closedir $dh;
}

# remove excluded models from model list
foreach my $x (@exclude){
	$x =~ s/\.bngl$//; # strip trailing .bngl, if any
	@models = grep { ! /$x/ } @models;
}

# check that we can find the BNG2.pl executable script!
unless ( -e $bngexec )
{   exit_error( "Cannot find BNG2.pl script" );   }
# check that we can find ODE trajectory verification script
my $verifyexec = File::Spec->catfile( $bngpath, "Perl2", "verify.pl" );
{
    unless ( -e $verifyexec )
    {   exit_error("cannot find verify.pl script\n");   }
}
# check if BioNetGen can find NFsim binary
if ($check_nfsim)
{
    print " -> checking for NFsim executable\n";
    my @args = ( $perlbin, $bngexec, '--findbin', 'NFsim' ); 
    system(@args);
    if ($? == -1) {  exit_error("failed to execute ($!)");  }
    # check return value: 0 means NFsim was found successfully
    if ( ($?>>8)==0 )
    {
        print "    ..found NFsim\n";
    }
    else
    {  
        print "WARNING: validate_examples cannot find NFsim binary!\n";
        print "Continuing, but will not validate NFsim.\n";
        push @bngargs, '--no-nfsim';
    }
}
else
{   # tell BNG to not run NFsim
    push @bngargs, '--no-nfsim';
}
    


# count number of tests and failures
my $fail_count = 0;
my $test_count = 0;


## Validate Models
MODEL:
foreach my $model (@models)
{
    my $err;
    ++$test_count;

    # define model path
    my $model_file = File::Spec->catfile( $modeldir, "${model}.bngl" );
    my $log_file   = File::Spec->catfile( $outdir, "${model}.log" );
    # define output file prefix
    my $outprefix = File::Spec->catfile( $outdir, $model );
    # define datfile prefix
    my $datprefix = File::Spec->catfile( $datdir, $model );

    # first check that model exists
    unless ( -e $model_file )
    {   exit_error("Can't find validation model $model_file");   }

    # open logfile
    open( my $log, ">", $log_file ) or exit_error("Can't open logfile $log_file ($!)");
    $log->autoflush(1);

    # list all output filehandles
    my @allFH = ($log, \*STDOUT);

    # write header
    print "\n";   # add blank line to console output
    multi_print( "[validate ${model}]\n", @allFH );
    multi_print( " -> processing model file with BioNetGen\n", @allFH );

    # execute BNGL model;
    run_BNG( $model_file, $model, $log_file, $log, $outdir );

	# check SBML import
    if ( -e "${datprefix}_SBML.bngl"  and  -e "${outprefix}_SBML.bngl" )
    {
        multi_print( " -> checking BNGL generated by SBML importer\n", @allFH );
        my $skipline = '^#';
        my $exit_status = diff_files( "${outprefix}_SBML.bngl", "${datprefix}_SBML.bngl", $skipline );
        if ($exit_status ne "")
        {   
            multi_print( "..FAILED!! $exit_status\n", @allFH ); 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
        print $log $SEPARATOR;
    }

    # check species in reaction network
    if ( -e "${datprefix}.net"  and  -e "${outprefix}.net" )
    {
        multi_print( " -> checking species in .NET file\n", @allFH );
        my @command = ( $perlbin, $compare_species, "${outprefix}.net", "${datprefix}.net" );
        my $exit_status = run_command( $log, \*STDOUT, @command );
        unless ( $exit_status==0)
        {   # compare_species encountered some problem
            print "FAILED!! exit status = $exit_status.\n";
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
    }

    if ( -e "${datprefix}.net"  and  -e "${outprefix}.net" )
    {   # check reactions in reaction network
        multi_print( " -> checking reactions in .NET file\n", @allFH );
        my @command = ( $perlbin, $compare_rxn, "${outprefix}.net", "${datprefix}.net" );
        my $exit_status = run_command( $log, \*STDOUT, @command );
        unless ( $exit_status==0)
        {   # compare_rxn encountered some problem
            print "FAILED!! exit status = $exit_status.\n"; 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
    }

    # check XML specification
    if ( -e "${datprefix}.xml"  and  -e "${outprefix}.xml" )
    {
        multi_print( " -> checking XML specification\n", @allFH );
        my $skipline = '<!-- Created by BioNetGen .* -->';
        my $exit_status = diff_files( "${outprefix}.xml", "${datprefix}.xml", $skipline );
        if ($exit_status ne "")
        {   
            multi_print( "..FAILED!! $exit_status\n", @allFH ); 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
        print $log $SEPARATOR;
    }

    # check SBML specification
    if ( -e "${datprefix}_sbml.xml"  and  -e "${outprefix}_sbml.xml" )
    {
        multi_print( " -> checking SBML specification\n", @allFH );
        my $skipline = '<!-- .* -->';
        my $exit_status = diff_files( "${outprefix}_sbml.xml", "${datprefix}_sbml.xml", $skipline );
        if ($exit_status ne "")
        {   
            multi_print( "..FAILED!! $exit_status\n", @allFH ); 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
        print $log $SEPARATOR;
    }

    # check hybrid model generation
    if ( -e "${datprefix}_hpp.bngl"  and  -e "${outprefix}_hpp.bngl" )
    {
        multi_print( " -> checking hybrid model generator\n", @allFH );
        my $skipline = '^#';
        my $exit_status = diff_files( "${outprefix}_hpp.bngl", "${datprefix}_hpp.bngl", $skipline );
        if ($exit_status ne "")
        {   
            multi_print( "..FAILED!! $exit_status\n", @allFH ); 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
        print $log $SEPARATOR;
    }

    # check hybrid model XML generation
    if ( -e "${datprefix}_hpp.xml"  and  -e "${outprefix}_hpp.xml" )
    {
        multi_print( " -> checking hybrid model XML specification\n", @allFH );
        my $skipline = '<!-- Created by BioNetGen .* -->';
        my $exit_status = diff_files( "${outprefix}_hpp.xml", "${datprefix}_hpp.xml", $skipline );
        if ($exit_status ne "")
        {   
            multi_print( "..FAILED!! $exit_status\n", @allFH ); 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
        print $log $SEPARATOR;
    }

    # check M-file specification
    if ( -e "${datprefix}.m"  and  -e "${outprefix}.m" )
    {
        multi_print( " -> checking M-file generator\n", @allFH );
        my $skipline = '^%';
        my $exit_status = diff_files( "${outprefix}.m", "${datprefix}.m", $skipline );
        if ($exit_status ne "")
        {   
            multi_print( "..FAILED!! $exit_status\n", @allFH ); 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
        print $log $SEPARATOR;
    }

    # check CVode specification
    if ( -e "${datprefix}_mex_cvode.c"  and  -e "${outprefix}_mex_cvode.c" )
    {
        multi_print( " -> checking MEX/CVode generator\n", @allFH );
        my $skipline = '^\/\*.*\*\/';
        my $exit_status = diff_files( "${outprefix}_mex_cvode.c", "${datprefix}_mex_cvode.c", $skipline );
        if ($exit_status ne "")
        {   
            multi_print( "..FAILED!! $exit_status\n", @allFH ); 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
        print $log $SEPARATOR;
    }

    # check CVode companion M-file specification
    if ( -e "${datprefix}_mex.m"  and  -e "${outprefix}_mex.m" )
    {
        multi_print( " -> checking Mex/CVode companion M-file specification\n", @allFH );
        my $skipline = '^%';
        my $exit_status = diff_files( "${outprefix}_mex.m", "${datprefix}_mex.m", $skipline );
        if ($exit_status ne "")
        {   
            multi_print( "..FAILED!! $exit_status\n", @allFH ); 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
        print $log $SEPARATOR;
    }

    # check ODE species trajectories
    if ( -e "${datprefix}.cdat"  and  -e "${outprefix}.cdat" )
    {
        multi_print( " -> checking species trajectory\n", @allFH );
        my @command = ( $perlbin, $verifyexec, "${outprefix}.cdat", "${datprefix}.cdat" );
        my $exit_status = run_command( $log, \*STDOUT, @command );
        unless ( $exit_status==0)
        {   # compare_species encountered some problem
            print "..FAILED!! exit_status = $exit_status\n"; 
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
    }

    # check ODE observable trajectories
    if ( -e "${datprefix}.gdat"  and  -e "${outprefix}.gdat" )
    {
        multi_print( " -> checking observable trajectory\n", @allFH );
        my @command = ( $perlbin, $verifyexec, "${outprefix}.gdat", "${datprefix}.gdat" );
        my $exit_status = run_command( $log, \*STDOUT, @command );
        unless ( $exit_status==0)
        {   # compare_species encountered some problem
            print "..FAILED!! exit_status = $exit_status\n";
            print "see $log_file for more details.\n";
            close $log;
            ++$fail_count;
            next MODEL;
        }
    }

    # check SSA equilibrium distribution (observables)
    {
        my $datfile  = "${outprefix}_ssa_equil.gdat";
        my $statfile = "${datprefix}_ssa_equil.stats";
        if ( -e $datfile  and  -e $statfile )
        {
            multi_print( " -> checking SSA equilibrium distribution\n", @allFH );
            my $exit_status = validate_equilibrium_data( $datfile, $statfile, $pvalue );
            if ( defined $exit_status )
            {
                multi_print( "..FAILED!! $exit_status\n", @allFH ); 
                # Try again
                print "Trying again..\n";
                run_BNG( $model_file, $model, $log_file, $log, $outdir );
                $exit_status = validate_equilibrium_data( $datfile, $statfile, $pvalue );
                if ( defined $exit_status )
            		{
            			multi_print( "..FAILED!! $exit_status\n", @allFH ); 
                		print "See $log_file for more details.\n";
                		close $log;
                		++$fail_count;
                		next MODEL;
            		}
            		else{
            			print "..PASSED!!\n";
            		}
            }
            print $log $SEPARATOR;
        }
    }

    # check NFsim equilibrium distribution (observables)
    if ($check_nfsim)
    {
        my $datfile  = "${outprefix}_nf_equil.gdat";
        my $statfile = "${datprefix}_nf_equil.stats";
        if ( -e $datfile  and  -e $statfile )
        {
            multi_print( " -> checking NFsim equilibrium distribution\n", @allFH );     
            my $exit_status = validate_equilibrium_data( $datfile, $statfile, $pvalue );
            if ( defined $exit_status )
            {
                multi_print( "..FAILED!! $exit_status\n", @allFH ); 
                # Try again
                print "Trying again..\n";
                run_BNG( $model_file, $model, $log_file, $log, $outdir );
                $exit_status = validate_equilibrium_data( $datfile, $statfile, $pvalue );
                if ( defined $exit_status )
            		{
            			multi_print( "..FAILED!! $exit_status\n", @allFH ); 
                		print "See $log_file for more details.\n";
                		close $log;
                		++$fail_count;
                		next MODEL;
            		}
            		else{
            			print "..PASSED!!\n";
            		}
            }
            print $log $SEPARATOR;
        }
    }

    # check PLA equilibrium distribution (observables)
    {
        my $datfile  = "${outprefix}_pla_equil.gdat";
        my $statfile = "${datprefix}_pla_equil.stats";
        if ( -e $datfile  and  -e $statfile )
        {
            multi_print( " -> checking PLA equilibrium distribution\n", @allFH );
            my $exit_status = validate_equilibrium_data( $datfile, $statfile, $pvalue );
            if ( defined $exit_status )
            {
                multi_print( "..FAILED!! $exit_status\n", @allFH ); 
                # Try again
                print "Trying again..\n";
                run_BNG( $model_file, $model, $log_file, $log, $outdir );
                $exit_status = validate_equilibrium_data( $datfile, $statfile, $pvalue );
                if ( defined $exit_status )
            		{
            			multi_print( "..FAILED!! $exit_status\n", @allFH ); 
                		print "See $log_file for more details.\n";
                		close $log;
                		++$fail_count;
                		next MODEL;
            		}
            		else{
            			print "..PASSED!!\n";
            		}
            }
            print $log $SEPARATOR;
        }
    }

	# check visualization outputs
	if($model eq 'visualize')
	{
		multi_print( " -> checking visualization outputs\n", @allFH );
		my @suffixes = ('_contactmap.gml', '_ruleviz_pattern.gml','_ruleviz_operation.gml',
						'_regulatory_1.gml', '_regulatory_2.gml',
						'_regulatory_3.gml', '_regulatory_4.gml', );
		my %messages = ('_contactmap.gml' => 'contact map',
						'_ruleviz_pattern.gml' => 'rule visualization using patterns',
						'_ruleviz_operation.gml' => 'rule visualization using graph operations',
						'_regulatory_1.gml' => 'regulatory graph',
						'_regulatory_2.gml' => 'background assignment',
						'_regulatory_3.gml' => 'group assignment',
						'_regulatory_4.gml' => 'collapsing groups',  
						);
		foreach my $suffix(@suffixes)
		{
			if( -e $datprefix.$suffix  and  -e $outprefix.$suffix)
			{
				multi_print( " ->-> checking ".$messages{$suffix}."\n", @allFH );
				my $exit_status = diff_files( $datprefix.$suffix, $outprefix.$suffix );
				if ($exit_status ne "")
				{   
					multi_print( "..FAILED!! $exit_status\n", @allFH ); 
					print "see $log_file for more details.\n";
					close $log;
					++$fail_count;
					next MODEL;
				}
				print $log $SEPARATOR;
			}
		}
	}
	
    if ($delete_working_files)
    {   delete_files($outprefix);   }

    # write log trailer
    multi_print( "$model passed all validation tests.\n", @allFH );  
    close $log;
}


## Print summary results and exit
if ($fail_count)
{   print "\n!! validate_examples failed to validate $fail_count of $test_count test models !!\n\n";   }
else
{   print "\nvalidate_examples passed all $test_count test model(s).\n\n";   }
exit($fail_count);


###                                                      ###
### END OF MAIN SCRIPT. Accessory subroutines are below. ###
###                                                      ###

# execute BNGL model;
sub run_BNG
{
	my $model_file = shift @_;
	my $model      = shift @_;
	my $log_file   = shift @_;
	my $log        = shift @_;
	my $outdir     = shift @_;
	
	if(@_) { push @bngargs, @_; }
	
    my @command = ( $perlbin, $bngexec, @bngargs, "--outdir", $outdir, $model_file );
    my $exit_status = run_command( $log, \*STDOUT, @command );
    unless ( $exit_status==0 )
    {   # BNG encountered some problem..
        print "!! BioNetGen failed to process $model !!\n"; 
        print "exit status = $exit_status.\n"; 
        print "see $log_file for more details.\n";
        ++$fail_count;
        close $log;
        next MODEL;
    }
}

# script for deleting working files
sub delete_files
{
    my $outprefix = shift @_;
    my @suffixes = qw( .net .xml _sbml.xml _cvode.c .cdat .gdat
                       _burnin.net     _burnin.cdat     _burnin.gdat
                       _equil.net      _equil.cdat      _equil.gdat
                       _ssa_equil.net  _ssa_equil.cdat  _ssa_equil.gdat 
                       _pla_equil.net  _ssa_pla.cdat    _pla_equil.gdat
                       _nf_equil.xml   _nf_equil.gdat   _nf_equil.species                 
                       .m              _mex.m           _mex_cvode.c 
                       _hpp.bngl       _hpp.xml     
					   _contactmap.gml   _ruleviz_pattern.gml  _ruleviz_operation.gml
					   _regulatory_1.gml _regulatory_2.gml	
					   _regulatory_3.gml _regulatory_4.gml 
					   _SBML.bngl);
    my @files = ();
    foreach my $suffix (@suffixes)
    {   push @files, ${outprefix}.${suffix};   }
    unlink @files;
}



# script for reading DAT files
sub read_datfile
{
    my $file = shift;

    # read first data file
    open (my $fh, '<', $file)  or  return undef;

    my $data = [];

    # read headers
    my $header = <$fh>;
    $header =~ s/^#\s*//;    # remove leading hash and whitespace
    $header =~ s/\s*$//;     # remove trailing whitespace
    
    # process fields
    my $fields = [];
    @$fields = split (/\s+/, $header);
    foreach my $field (@$fields)
    {
        my $field_hash = { 'field' => $field, 'elements' => [] };
        push @$data, $field_hash;
    }

    while ( my $line = <$fh> )
    {
        $line =~ s/#.*$//;   # remove comments
        $line =~ s/^\s*//;   # remove leading whitespace
        $line =~ s/\s*$//;   # remove trailing whitespace
        
        # is there any line left?
        next unless ( $line =~ /\S+/ );

        # split tokens
        my $tokens = [];
        @$tokens = split (/\s+/, $line);
        
        my $idx = 0;
        foreach my $token (@$tokens)
        {
            push @{ $data->[$idx]->{elements} }, sprintf( '%f', $token );
            ++$idx;
        }
    }
    close $fh;

    return $data;
}



# script for reading stats files
sub read_stats
{
    # open stats file
    my $file = shift;
    open( my $fh, '<', $file )  or  return undef;

    # read stats file, parse, and put results in hash
    my $stats = {};
    while ( my $line = <$fh> )
    {
        # trim comments and leading and trailing white space
        $line =~ s/#.*$//;
        $line =~ s/^\s+//;
        $line =~ s/\s+$//;
        # find key
        $line =~ s/(\w+)\s*=\s*//;  my $key = $1;
        next unless (defined $key);
        # get values
        my $values = [];
        @$values = split (/,\s*/, $line);

        # add to stats hash
        foreach my $val (@$values)
        {
            if ( Scalar::Util::looks_like_number($val) )
            {
                push @{$stats->{$key}}, sprintf( '%f', $val );
            }
            else
            {
                push @{$stats->{$key}}, sprintf( '%s', $val );
            }
        }
        $stats->{$key} = $values;
    }

    # close stats file
    close $fh;
    # return stats
    return $stats;
}



# validate stochastic equilibrium distribution
sub validate_equilibrium_data
{
    my $datfile  = shift;   # file containing sample data
    my $statfile = shift;   # file containing true distribution stats
    my $pvalue   = shift;

    # tolerance for equality of floating point numbers
    my $tolerance = 1e-12;

    # read sample data
    my $data = read_datfile( $datfile );
    unless ( defined $data )
    {   return "ERROR: some problem reading $datfile!";   }

    # read stats file
    my $stats = read_stats( $statfile );
    unless ( defined $stats )
    {   return "ERROR: some problem reading $statfile!";   }


    # get observable
    unless ( exists $stats->{observable} )
    {   return "ERROR: 'observable' not defined in $statfile!";   }
    my $observable = $stats->{observable}->[0];

    # get statistic
    unless ( exists $stats->{statistic} )
    {   return "ERROR: 'statistic' not defined in $statfile!";   }
    my $statistic = $stats->{statistic}->[0];

    # get degrees of freedom
    unless ( exists $stats->{degrees_of_freedom} )
    {   return "ERROR: 'degrees_of_freedom' not defined in $statfile!";   }
    my $degfree = $stats->{degrees_of_freedom}->[0];

    # get bin min
    unless ( exists $stats->{bin_minimum_value} )
    {   return "ERROR: 'bin_minimum_value' not defined in $statfile!";  }
    my $bin_mins = $stats->{bin_minimum_value};

    # get bin max
    unless ( exists $stats->{bin_maximum_value} )
    {   return "ERROR: 'bin_maximum_value' not defined in $statfile!";   }
    my $bin_maxs = $stats->{bin_maximum_value};

    # get bin probability
    unless ( exists $stats->{bin_probability} )
    {   return "ERROR: 'bin_probability' not defined in $statfile!";   }
    my $bin_probs = $stats->{bin_probability};

    # get significance levels
    unless ( exists $stats->{significance_levels} )
    {   return "ERROR: 'significance_levels' not defined in $statfile!";   }
    my $sig_levels = $stats->{significance_levels};


    # get statistic values
    unless ( exists $stats->{statistic_values} )
    {   return "ERROR: 'statistic_values' not defined in $statfile!";   }
    my $stat_vals = $stats->{statistic_values};

    # get observable data
    (my $samples) = (grep {$_->{field} eq $observable} @$data);
    unless ( defined $samples )
    {   return "ERROR: could not find observable $observable in $datfile!";   }


    # bin sample data
    my ($bins, $n_samples) = bin_sample_data( $samples->{elements}, $bin_mins, $bin_maxs );
    unless ( defined $bins )
    {   return "ERROR: some problem binning sample data!";   }

    # check statistic statistic
    if ( $statistic eq 'chi_square' )
    {
        my $chisq = chi_square( $bins, $n_samples, $bin_probs );
        unless ( defined $chisq )
        {   return "ERROR: some problem calculating chi_square!";   }

        my $idx = 0;
        my $n_levels = @$sig_levels;
        while ( ($idx < $n_levels) and (abs($sig_levels->[$idx] - $pvalue) > $tolerance) )
        {   ++$idx;   }
        my $sig_stat = $stat_vals->[$idx];

        if ( $sig_stat > $chisq )
        {   return undef;   }
        else
        {
            return sprintf "$statistic rejected the null hypothesis (dof=%d, p<%.3f, chi2=%.3f).",
                           $degfree, $pvalue, $chisq;
        }
    }
    else
    {   return "ERROR: statistic $statistic is not defined!";   }

    return undef;        
}



# put sample data into bins
sub bin_sample_data
{
    # get arguments
    my $samples = shift;
    my $bin_mins = shift;
    my $bin_maxs = shift;

    # initialize return structures
    my $n_samples = @$samples;
    my $n_bins = @$bin_mins;
    my $bins = [ (0) x $n_bins ];

    # loop over samples
    foreach my $sample (@$samples)
    {
        # find bin
        my $bin = 0;
        while ( ($bin < $n_bins) and !(($bin_mins->[$bin] < $sample) and ($sample <= $bin_maxs->[$bin])) )
        {   ++$bin;   }
        # increment bin
        if ( $bin < @$bins )
        {   ++($bins->[$bin]);   }
    }

    return ($bins, $n_samples);
}



# calculate a chi_square value given binned samples, bin probabilities, and the number of samples
sub chi_square
{
    my $bins = shift;
    my $n_samples = shift;
    my $bin_prob = shift;

    # number of bins should be same as number of bin probabilities
    unless ( @$bins == @$bin_prob )
    {   return undef;   }
    # number of samples should be positive
    unless ( $n_samples > 0 )
    {   return undef;   }

    # loop over bins
    my $chisq = 0.0;
    for ( my $idx = 0;  $idx < @$bins;  ++$idx )
    {
        # calculate this component of chi_square
        $chisq += ( $bins->[$idx] - $n_samples*$bin_prob->[$idx] )**2 / ( $n_samples*$bin_prob->[$idx] );
    }

    # return chi_square value
    return $chisq;
}



# A simple method for file difference detection.
# Returns empty string if files are the same,
# otherwise returns string describing the first difference.
# Ignores blank lines, differences in whitespace, and
# trailing comments.
#
sub diff_files
{
    my $file1 = shift @_;
    my $file2 = shift @_;
    my $skipline = @_ ? shift @_ : '';

    my $lno1=0;
    my $lno2=0;

    # open file1
    my $fh1;
    unless ( open $fh1, '<', $file1 )
    {   return "diff_files($file1,$file2): ERROR!! some problem opening file1.";   }

    # read file2
    my $fh2;
    unless ( open $fh2, '<', $file2 )
    {   return "diff_files($file1,$file2): ERROR!! some problem opening file2.";   }

    my $diff = "";
    # loop through file1
    while ( my $line1 = <$fh1> )
    {
        ++$lno1;
        # skip blank lines
        next unless ( $line1 =~ /\S/ );
        # check skipline regex
        unless ( $skipline eq '' )
        {   next if ( $line1 =~ /$skipline/ );   }
        
        # get next valid line of file2
        my $line2;
        while ($line2 = <$fh2> )
        {
            ++$lno2;
            # skip blank line
            next unless ( $line2 =~ /\S/ );
            # check skipline regex
            unless ( $skipline eq '' )
            {   next if ( $line2 =~ /$skipline/ );   }
            # ok, this line2 will be compared to line1
            last;
        }
    
        # check if line2 is defined
        unless (defined $line2)
        {   # found difference
            $diff = "diff_files($file1,$file2): found difference at lines $lno1 and $lno2.";
            last;
        }

        # eliminate leading whitespace
        $line1 =~ s/^\s+//;
        $line2 =~ s/^\s+//;
        # eliminate repeated whitespace
        $line1 =~ s/\s+/ /g;
        $line2 =~ s/\s+/ /g;
        # remove trailing whitespace
        $line1 =~ s/\s+$//;
        $line2 =~ s/\s+$//;
        # remove trailing comments
        $line1 =~ s/#.*//;
        $line2 =~ s/#.*//;

        # Special fix for MSWin32
        # ..exponents are written with 3 digits in Windows rather than the standard 2.
        if ( $Config{osname} eq "MSWin32" )
        {   # convert 3 digit exponents to 2 digits
            $line1 =~ s/(\d[Ee][+\-])0(\d\d)/$1$2/g;
            $line2 =~ s/(\d[Ee][+\-])0(\d\d)/$1$2/g;
        }        

        unless ( $line1 eq $line2 )
        {   # found difference
            $diff = "diff_files($file1,$file2): found difference at lines $lno1 and $lno2.";
            last;
        }
    }

    if ($diff eq "")
    {
        # check if valid lines remain in file2
        while ( my $line2 = <$fh2> )
        {
            ++$lno2;
            # skip blank line
            next unless ( $line2 =~ /\S/ );
            unless ( $skipline eq '' )
            {   next if ( $line2 =~ /$skipline/ );   }

            # ok, this is a valid line. report difference.
            $diff = "diff_files($file1,$file2): found difference at lines $lno1 and $lno2.";
            last;
        }
    }

    close $fh1;
    close $fh2;
    return $diff;
}


# run external command
sub run_command
{
    my ($parent_out, $parent_err, @command) = @_;

    # start simulator as child process with communication pipes
    my ($child_in, $child_out); 
    my $pid = eval{ open3( $child_in, $child_out, $child_out, @command ) };
    if ($@) {  print $parent_err "Problem running command: $@";  return -1;  }

    # remember child PID
    print $parent_out "running command:\n", join(" ", @command), "\n";
    print $parent_out "[child process ID is: $pid]\n";

    # monitor output of child process
    while (my $line = <$child_out> )
    {   # relay to parent
        print $parent_out $line;
    }

    # make sure child process has completed
    waitpid($pid,0);
    my $exit_status = ($? >> 8);
    # write line to delinate end of command
    print $parent_out $::SEPARATOR;
    # return exit status
    return $exit_status;
}


# print to multiple filehandles
sub multi_print
{
    my ($line, @fhs) = @_;
    foreach my $fh (@fhs)
    {   print $fh $line;   }
}


# print error and exit
sub exit_error
{
    my $err = shift @_;
    print "ABORT: $err\n";
    exit -1;
}


# display help
sub display_help
{

#print <<END_HELP
print qq{
Validation script for BioNetGen suite.

SYNOPSIS:
  validate_examples.pl [OPTS]       : run validation script on standard models
  validate_examples.pl [OPTS] MODEL : run validation script on MODEL
                                        (omit ".bngl" extension from MODEL)

OPTIONS:
  --exclude MODEL     : Models to exclude from validation; can be used multiple
                        times (e.g., --exclude model1.bngl --exclude model2.bngl)
  --bngpath PATH      : BioNetGen path (..)
  --modelpath PATH    : model path ([BNGPATH\\Validate)
  --datpath PATH      : database path (BNGPATH]\\Validate\\DAT_validate)
  --pvalue VAL        : pvalue for distribution tests (0.01)
  --no-nfsim          : skip NFsim validation
  --no-delete-files   : keep output files after validation
  --help              : display help

Exits with value 0 if all validation tests are passed, otherwise exits
with a positive value equal to the number of failed tests, or -1 if there
was a problem running the script.

A log file named MODEL.log is created for each validation model. If a test
fails, check the log file for more details.  NOTE: stochastic fluctuations
will occasionally cause a distribution test to fail.
}
#END_HELP

#  --outpath PATH      : output path (cwd)

}
