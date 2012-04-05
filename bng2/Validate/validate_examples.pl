#!/usr/bin/perl
# Validation script for BioNetGen suite.
# 
# SYNOPSIS:
#   validate_examples.pl [OPTIONS]              : validate all models
#   validate_examples.pl [OPTIONS] MODEL...     : validate MODEL
#
# Exits with value 0 if all validation tests are passed, otherwise exits
#  with a positive value equal to the number of failed tests, or -1 (=255) if
#  there was some problem executing the validation.
#
# A log file named MODEL.log is created for each validation model. If a test
#  fails, check the log file for more details.
#
# Validation on WINDOWS:
#    User should point the $perlbin variable to the Perl executable
#
# What's New?
#   8sep2011: validation of equilibrium distribution for SSA models. --Justin
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
#  1) Create MODEL.bngl, including actions that generates some or all of
#       the recognized output files.
#  2) Put the validation model in $modeldir directory.
#  3) Generate reference output files and place in the $datdir subdirectory.**
#
#  ** for most validations, the reference file shares the same format and name
#    as the output file. However, special reference files are required for
#    validating stochastic equilibrium samples. These reference files have a
#    .stat extension and describe a binned equilibrium distribution of a model
#    observable. See $datdir/gene_expr_ssa_equil.stats for an example.
#    Equilibrium samples are compared to the reference distribution by a
#    Chi-square goodness of fit test. The stats file defines the obsevable of
#    interest, bin widths and probabilities, and the Chi-square values
#    corresponding to various significance levels.
#
#  ** It is the Modeler's responsibility to validate the reference trajectory.
#    It is advisable to compare the reference to analytic results, simulations
#    reported in the literature, or simulations generated from independent
#    platforms (e.g. MATLAB).
#
# NOTE: make sure BNG knows where to find NFsim, otherwise NFsim validation
#  will fail!

# TODO: Add validations for the following
#   non-equilibrium stochastic validation (multirun validation)
#   Syntax errors
#   Canonical labeling
#   On-the-fly simulation

use strict;
use warnings;

# Perl Modules
use FindBin;
use File::Spec;
use Scalar::Util qw( looks_like_number );
use Config;



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
        $bngpath = File::Spec->catpath( $volume, File::Spec->catdir(@dirs) );
    }
    # define executable
    $bngexec = File::Spec->catfile( $bngpath, "BNG2.pl" );
}

# directory containing validation reference files
my $datdir   = File::Spec->catdir( $bngpath, 'Validate', 'DAT_validate' );
# directory containing models
my $modeldir = File::Spec->catdir( $bngpath, 'Validate' );
# output directory
my $outdir = File::Spec->curdir();
# compare species and rxns script
my $compare_species = File::Spec->catfile( $modeldir, 'compare_species.pl' );
my $compare_rxn = File::Spec->catfile( $modeldir, 'compare_rxn.pl' );
 # if true, delete output files after validation
my $delete_working_files = 1;   
# size of indent to STDOUT       
my $INDENT = '  ';
# p-value for statistical validation (recommend 0.01)
#   Options are 0.05, 0.02, 0.01, 0.005, 0.002, 0.001.
#   Keep in mind that validation will fail (100*p) percent of the time, even
#   if the simulator is generating the correct distribution!!                
my $pvalue = 0.01;
# try to validate NFsim?                  
my $check_nfsim = 1;
# arguments for BNG
my @bngargs = ();



###                                                          ###
###  BEGIN MAIN SCRIPT, no user options beyond this point!!  ###
###                                                          ###

# Greet the User
print ">>> BioNetGen Validation Utility\n";

# Process command line arguments
while ( @ARGV and $ARGV[0] =~ /^--/ )
{
    my $arg = shift @ARGV;
    if ( $arg eq '--bngpath' )
    {   $bngpath = shift @ARGV;   }
    elsif ( $arg eq '--modelpath' )
    {   $modeldir = shift @ARGV;  }
    elsif ( $arg eq '--datpath' )
    {   $datdir = shift @ARGV;    }
    elsif ( $arg eq '--outpath' )
    {   $outdir = shift @ARGV;    }
    elsif ( $arg eq '--pvalue' )
    {   $pvalue = shift @ARGV;    }
    elsif ( $arg eq '--no-nfsim' )
    {   $check_nfsim = 0;   }
    elsif ( $arg eq '--no-delete-files' )
    {   $delete_working_files = 0;   }
    elsif ( $arg eq '--help' )
    {   display_help();  exit 0;   }
    else
    {   # unrecognized option!
        print "$0 syntax error: unrecognized command line option '$arg'.\n";
        exit -1;
    }
}

# get models to validate
my @models;
if (@ARGV)
{   # remaining command line arguments are models
    @models = (@ARGV);
}
else
{   # run validations on all models in $modeldir
    my $dh;
    unless( opendir $dh, $modeldir )
    {
        print "Error: scan_var can't open directory $modeldir: $!\n";
        exit(-1);
    }
    @models = map { /^(.+)\.bngl$/ } ( grep {/^.+\.bngl$/} readdir($dh) );
    closedir $dh;
}




# check that we can find the BNG2.pl executable script!
unless ( -e $bngexec )
{
    print "ERROR: scan_var cannot find BNG2.pl script!!\n";
    print "Aborting validation.\n";
    exit(-1);
}
# check that we can find ODE trajectory verification script
my $verifyexec = File::Spec->catfile( $bngpath, "Perl2", "verify.pl" );
{
    unless ( -e $verifyexec )
    {
        print "ERROR: scan_var cannot find Verify script!!\n";
        print "Aborting validation.\n";
        exit(-1);
    }
}
# check if BioNetGen can find NFsim binary
if ($check_nfsim)
{
    print "Checking for NFsim executable..\n";
    my @args = ( $perlbin, $bngexec, '--findbin', 'NFsim' ); 
    system(@args);
    if ($? == -1) { die "failed to execute: $!\n"; }
    # check return value: 0 means NFsim was found successfully
    if ( ($?>>8)==0 )
    {
        print "..found NFsim\n";
    }
    else
    {  
        print "WARNING: scan_var cannot find NFsim binary!\n";
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
foreach my $model (@models)
{
    print "\nValidating model '$model'..\n";

    my $err;
    ++$test_count;

    # define model path
    my $model_file = File::Spec->catfile( $modeldir, "${model}.bngl" );
    my $log_file   = File::Spec->catfile( $outdir, "${model}.log" );

    # define output file prefix
    my $outprefix = File::Spec->catfile( $outdir, $model );

    # define datfile prefix
    my $datprefix = File::Spec->catfile( $datdir, $model );

    # delete any old files (to avoid validating against old files)
    delete_files($outprefix,$model);

    # first check that model exists
    unless ( -e $model_file )
    {
        print "!! Can't find validation model $model_file !!\n";
        exit(-1);
    }

    # execute BNGL model
    my $bngargs = join( ' ', @bngargs );
    my $command = "\"$perlbin\" \"$bngexec\" $bngargs --outdir \"$outdir\" \"$model_file\" > \"$log_file\"";
    system( $command );
    if ($? == -1) { die "failed to execute: $!"; }
    unless ( ($?>>8)==0 )
    {   # BNG encountered some problem..
        print "!! Execution of BNGL model $model' FAILED!!\n"
             ."   see $log_file form more details.\n";
        ++$fail_count;
        if ($delete_working_files)
        {   delete_files($outprefix);   }
        next;
    }


    # check reaction network
    if ( -e "${datprefix}.net"  and  -e "${outprefix}.net" )
    {
        print $INDENT . "Checking species in .NET file.. ";
        my $command = "\"$perlbin\" \"$compare_species\" \"${outprefix}.net\" \"${datprefix}.net\" >> \"$log_file\"";
        system( $command );
        if ($? == -1) { die "failed to execute: $!"; }
        unless ( ($?>>8)==0 )
        {   # compare_species encountered some problem
            print "FAILED!!\n", $?>>8, "\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";

        print $INDENT . "Checking reactions in .NET file.. ";
        $command = "\"$perlbin\" \"$compare_rxn\" \"${outprefix}.net\" \"${datprefix}.net\" >> \"$log_file\"";
        system( $command );
        if ($? == -1) { die "failed to execute: $!"; }
        unless ( ($?>>8)==0 )
        {   # compare_rxn encountered some problem
            print "FAILED!!\n", $?>>8, "\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check XML specification
    if ( -e "${datprefix}.xml"  and  -e "${outprefix}.xml" )
    {
        print $INDENT . "Checking XML specification.. ";
        my $skipline = '<!-- Created by BioNetGen .* -->';
        my $diff = diff_files( "${outprefix}.xml", "${datprefix}.xml", $skipline );
        if ($diff ne "")
        {   
            my $command = "echo \"$diff\" >> \"$log_file\"";
            system( $command )==0 or die "system $command failed: $?\n";
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check SBML specification
    if ( -e "${datprefix}_sbml.xml"  and  -e "${outprefix}_sbml.xml" )
    {
        print $INDENT . "Checking SBML specification.. ";
        my $skipline = '<!-- .* -->';
        my $diff = diff_files( "${outprefix}_sbml.xml", "${datprefix}_sbml.xml", $skipline );
        if ($diff ne "")
        {   
            my $command = "echo \"$diff\" >> \"$log_file\"";
            system( $command )==0 or die "System $command failed: $?\n";
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check hybrid model generation
    if ( -e "${datprefix}_hybrid.bngl"  and  -e "${outprefix}_hybrid.bngl" )
    {
        print $INDENT . "Checking hybrid model generation.. ";
        my $skipline = '^#';
        my $diff = diff_files( "${outprefix}_hybrid.bngl", "${datprefix}_hybrid.bngl", $skipline );
        if ($diff ne "")
        {   
            my $command = "echo \"$diff\" >> \"$log_file\"";
            system( $command )==0 or die "System $command failed: $?\n";
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check hybrid model XML generation
    if ( -e "${datprefix}_hybrid.xml"  and  -e "${outprefix}_hybrid.xml" )
    {
        print $INDENT . "Checking hybrid model XML generation.. ";
        my $skipline = '<!-- Created by BioNetGen .* -->';
        my $diff = diff_files( "${outprefix}_hybrid.xml", "${datprefix}_hybrid.xml", $skipline );
        if ($diff ne "")
        {   
            my $command = "echo \"$diff\" >> \"$log_file\"";
            system( $command )==0 or die "System $command failed: $?\n";
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check M-file specification
    if ( -e "${datprefix}.m"  and  -e "${outprefix}.m" )
    {
        print $INDENT . "Checking M-file specification.. ";
        my $skipline = '^%';
        my $diff = diff_files( "${outprefix}.m", "${datprefix}.m", $skipline );
        if ($diff ne "")
        {   
            my $command = "echo \"$diff\" >> \"$log_file\"";
            system( $command )==0 or die "System $command failed: $?\n";
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check CVode specification
    if ( -e "${datprefix}_mex_cvode.c"  and  -e "${outprefix}_mex_cvode.c" )
    {
        print $INDENT . "Checking CVode output specification.. ";
        my $skipline = '^\/\*.*\*\/';
        my $diff = diff_files( "${outprefix}_mex_cvode.c", "${datprefix}_mex_cvode.c", $skipline );
        if ($diff ne "")
        {   
            my $command = "echo \"$diff\" >> \"$log_file\"";
            system( $command )==0 or die "System $command failed: $?\n";
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check CVode companion M-file specification
    if ( -e "${datprefix}_mex.m"  and  -e "${outprefix}_mex.m" )
    {
        print $INDENT . "Checking CVode companion M-file specification.. ";
        my $skipline = '^%';
        my $diff = diff_files( "${outprefix}_mex.m", "${datprefix}_mex.m", $skipline );
        if ($diff ne "")
        {   
            my $command = "echo \"$diff\" >> \"$log_file\"";
            system( $command )==0 or die "System $command failed: $?\n";
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check ODE species trajectories
    if ( -e "${datprefix}.cdat"  and  -e "${outprefix}.cdat" )
    {
        print $INDENT . "Checking species trajectories.. ";
        system "\"$perlbin\" \"$verifyexec\" \"${outprefix}.cdat\" \"${datprefix}.cdat\" >> \"$log_file\"";
        if ($? == -1) { die "failed to execute: $!"; }
        unless ( ($?>>8)==0 )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check ODE observable trajectories
    if ( -e "${datprefix}.gdat"  and  -e "${outprefix}.gdat" )
    {
        print $INDENT . "Checking observable trajectories.. ";
        system "\"$perlbin\" \"$verifyexec\" \"${outprefix}.gdat\" \"${datprefix}.gdat\" >> \"$log_file\"";
        if ($? == -1) { die "failed to execute: $!"; }
        unless ( ($?>>8)==0 )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check SSA equilibrium distribution (observables)
    {
        my $datfile  = "${outprefix}_ssa_equil.gdat";
        my $statfile = "${datprefix}_ssa_equil.stats";
        if ( -e $datfile  and  -e $statfile )
        {
            print $INDENT . "Comparing SSA equilibrium distributions.. ";
            
            # validate data
            my $err = validate_equilibrium_data( $datfile, $statfile, $pvalue );
            if ( defined $err )
            {
                print "FAILED!!\n";
                print $INDENT . "$err\n";
                print $INDENT . "See $log_file form more details.\n";
                ++$fail_count;
                next;
            }
            print sprintf "passed (p>=%.3f)\n", $pvalue;
        }
    }

    # check NFsim equilibrium distribution (observables)
    if ($check_nfsim)
    {
        my $datfile  = "${outprefix}_nf_equil.gdat";
        my $statfile = "${datprefix}_nf_equil.stats";
        if ( -e $datfile  and  -e $statfile )
        {
            print $INDENT . "Comparing NFsim equilibrium distributions.. ";
            
            # validate data
            my $err = validate_equilibrium_data( $datfile, $statfile, $pvalue );
            if ( defined $err )
            {
                print "FAILED!!\n";
                print $INDENT . "$err\n";
                print $INDENT . "See $log_file form more details.\n";
                ++$fail_count;
                next;
            }
            print sprintf "passed (p>=%.3f)\n", $pvalue;
        }
    }

    if ($delete_working_files)
    {   delete_files($outprefix);   }
}





## Print summary results and exit
if ($fail_count)
{   print "\n!! scan_var failed $fail_count of $test_count validation tests !!\n";   }
else
{   print "\nscan_var passed all $test_count test(s).\n";   }
exit($fail_count);






###                                                      ###
### END OF MAIN SCRIPT. Accessory subroutines are below. ###
###                                                      ###

# script for deleting working files
sub delete_files
{
    my $outprefix = shift @_;
    my @suffixes = qw( .net .xml _sbml.xml _cvode.c .cdat .gdat
                       _burnin.net     _burnin.cdat     _burnin.gdat
                       _equil.net      _equil.cdat      _equil.gdat
                       _ssa_burnin.net _ssa_burnin.cdat _ssa_burnin.gdat
                       _ssa_equil.net  _ssa_equil.cdat  _ssa_equil.gdat 
                       _nf_equil.xml   _nf_equil.gdat                    
                       .m              _mex.m           _mex_cvode.c 
                       _hybrid.bngl    _hybrid.xml                        );
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
    @$fields = split /\s+/, $header;
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
        @$tokens = split /\s+/, $line;
        
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
        @$values = split /,\s*/, $line;

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



# A simple method for file difference detection
# returns emtpy string if files are the same,
# otherwise returns string describing the first differnce.
# Ignores blank lines and differences in whitespace
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

        # Special fix for MSWin32
        # ..exponents are written with 3 digits in Windows rather than the standard 2.
        if ( $Config{archname} eq "MSWin32" )
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



# display help
sub display_help
{

print <<END_HELP

Validation script for BioNetGen suite.

SYNOPSIS:
  validate_examples.pl [OPTS]       : run validation script on standard models
  validate_examples.pl [OPTS] MODEL : run validation script on MODEL

OPTIONS:
  --bngpath PATH      : BioNetGen path (..)
  --modelpath PATH    : model path ([BNGPATH]\\Validate)
  --datpath PATH      : database path ([BNGPATH]\\Validate\\DAT_validate)
  --outpath PATH      : output path (cwd)
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

END_HELP

}
