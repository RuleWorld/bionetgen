#!/usr/bin/perl
# Validation script for BioNetGen suite.
# 
# Synopsis:
#   ./validate_examples.pl              : execute validation script on standard model set
#   ./validate_examples.pl MODEL...     : execute validation script on MODEL
#
# Exits with value 0 if all validation tests are passed, otherwise exits
#  with a positive value equal to the number of failed tests.
#
# A log file named MODEL.log is created for each validation model. If a test
#  fails, check the log file for more details.

#
# What's New?
#   8sep2011: validation of equillibrium distribution for SSA models. --Justin
#
#
# More Details:
#   This script will automatically recognize and validate the following output files for
#    each MODEL in the @validation_models list (see below):
#
#   output file             reference file         : description                                 
#  -----------------------------------------------------------------------------
#   MODEL.net             : MODEL.net              : reaction network
#   MODEL.xml             : MODEL.xml              : XML specification
#   MODEL.gdat            : MODEL.gdat             : ODE observables trajectory
#   MODEL.cdat            : MODEL.cdat             : ODE species trajectory
#   MODEL_ssa_equil.gdat  : MODEL_ssa_equil.stats  : SSA equillibrium samples   
#   MODEL_nf_equil.gdat   : MODEL_nf_equil.stats   : NFsim equillibrium samples
#
#
# To add new validation MODEL:
#  1) Create MODEL.bngl, including actions that generates some or all of
#       the recognized output files.
#  2) Put the validation model in $modeldir directory.
#  3) Add MODEL to @validation_models list in THIS script (do not include .bngl extension!).
#  4) Generate reference output files and place them in the $datdir subdirectory. **
#
#  ** for most validations, the reference file shares the same format and name as the
#    output file.  However, special reference files are required for validating
#    stochastic equillibrium samples. These reference files have a .stat extension and
#    describe a binned equillibrium distribution of a model observable.
#    See $datdir/gene_expr_ssa_equil.stats for an example. Equillibrium samples
#    are compared to the reference distribution by a Chi-square goodness of fit test.
#    The stats file defines the obsevable of interest, bin widths and probabilities,
#    and the Chi-square values corresponding to various significance levels.
#
#  ** It is the Modeler's responsibility to validate the reference trajectory. It is
#    advisable to compare the reference to analytic results, simulations reported
#    in the literature, or simulations generated from independent platforms (e.g. MATLAB).
#
# NOTE: make sure BNG knows where to find NFsim, otherwise NFsim validation will fail!

# TODO: Add validations for the following
#   Mfile output
#   Mexfile output
#   SBML output
#   non-equillibrium stochastic validation (multirun validation)
#   Syntax errors
#   Canonical labeling
#   On-the-fly simulation

use strict;
use warnings;

# Perl Modules
use FindBin;
use lib $FindBin::Bin;
use File::Spec;
use Scalar::Util qw( looks_like_number );


### PARAMETERS ###
# BNG root directory
my $bngpath;
{
    my ($volume,$directories,$file) = File::Spec->splitpath( $FindBin::Bin );
    my @dirs = File::Spec->splitdir( $directories );
    pop @dirs;
    $bngpath  = File::Spec->catdir( $volume, File::Spec->catdir( @dirs ) ); 
}
# directory containing validation reference files
my $datdir   = File::Spec->catdir( $bngpath, 'Validate', 'DAT_validate' );
# directory containing models
my $modeldir = File::Spec->catdir( $bngpath, 'Validate' );
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


### VALIDATION MODELS ###
my @validation_models = qw(
    ANx
    tlmr
    test_paramname
    test_MM
    test_Sat
    test_read_sat
    deleteMolecules
    toy-jim
    Haugh2b
    egfr_net
    fceri_ji
    SHP2_base_model
    tlbr
    simple_system
    Motivating_example
    Motivating_example_cBNGL
    CaOscillate_Sat
    CaOscillate_Func
    Repressilator
    continue
    motor
    test_fixed
    heise
    isomerization
    gene_expr
    gene_expr_func
    localfunc
    hybrid_test
);




###                                                          ###
###  BEGIN MAIN SCRIPT, no user options beyond this point!!  ###
###                                                          ###

# Greet the User
print ">>> BioNetGen Validation Utility\n";

# Process command line arguments
while ( @ARGV and $ARGV[0] =~ /^-/ )
{
    my $arg = shift @ARGV;
    if ( $arg eq '-bngpath' )
    {   $bngpath = shift @ARGV;   }
    elsif ( $arg eq '-modelpath' )
    {   $modeldir = shift @ARGV;  }
    elsif ( $arg eq '-datpath' )
    {   $datdir = shift @ARGV;    }
    elsif ( $arg eq '-pvalue' )
    {   $pvalue = shift @ARGV;    }
    elsif ( $arg eq '-no-nfsim' )
    {   $check_nfsim = 0;         }
    elsif ( $arg eq '-no-delete-files' )
    {   $delete_working_files = 0;   }
    else
    {   # unrecognized option!
        print "$0 syntax error: unrecognized command line option '$arg'.\n";
        exit(1);
    }
}

# get models to validate
my @models;
if (@ARGV)
{   # remaining command line arguments are models
    @models = (@ARGV);
}
else
{   # use default validations
    @models = (@validation_models);
}



# check that we can find the BNG2.pl executable script!
my $bngexec = File::Spec->catfile( $bngpath, 'BNG2.pl' );
unless ( -x $bngexec )
{
    print "ERROR: $0 cannot find BNG2.pl script!!\n";
    print "Aborting validation.\n";
    exit(1);
}
# check that we can find ODE trajectory verification script
my $verifyexec = File::Spec->catfile( $bngpath, 'Perl2', 'verify.pl' );
{
    unless ( -x $verifyexec )
    {
        print "ERROR: $0 cannot find Verify script!!\n";
        print "Aborting validation.\n";
        exit(1);
    }
}
# check if BioNetGen can find NFsim binary
if ($check_nfsim)
{
    my $err = system "$bngexec -findbin NFsim";
    if ( $err )
    {
        print "WARNING: $0 cannot find NFsim binary!!\n";
        print "Continuing, but will not validate NFsim.\n";
    }
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

    # delete any old files
    delete_files($model);

    # define model path
    my $model_file = File::Spec->catfile( $modeldir, "${model}.bngl" );
    my $log_file   = File::Spec->catfile( File::Spec->curdir(), "${model}.log" );

    # first check that model exists
    unless ( -e $model_file )
    {
        print "!! Could not find validation model $model_file !!\n";
        ++$fail_count;
        if ($delete_working_files)
        {   delete_files($model);   }
        next;

    }

    # execute BNGL model
    $err = system "$bngexec $model_file > $log_file";
    if ( $err )
    {
        print "!! Execution of BNGL model '$model' FAILED !!\n"
             ."   see $log_file form more details.\n";
        ++$fail_count;
        if ($delete_working_files)
        {   delete_files($model);   }
        next;
    }

    # check reaction network
    if ( -e "$datdir/$model.net"  and  -e "$model.net" )
    {
        print $INDENT . "Checking species in .NET file.. ";
        $err = system "$compare_species $model.net $datdir/$model.net >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";

        print $INDENT . "Checking reactions in .NET file.. ";
        $err = system "$compare_rxn $model.net $datdir/$model.net >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check XML specification
    if ( -e "${datdir}/${model}.xml"  and  -e "${model}.xml" )
    {
        print $INDENT . "Checking XML specification.. ";
        $err = system "diff -bB -I '<!-- Created by BioNetGen .* -->' ${model}.xml ${datdir}/${model}.xml >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check SBML specification
    if ( -e "${datdir}/${model}_sbml.xml"  and  -e "${model}_sbml.xml" )
    {
        print $INDENT . "Checking SBML specification.. ";
        $err = system "diff -bB -I '<!-- .* -->' ${model}_sbml.xml ${datdir}/${model}_sbml.xml >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check hybrid model generation
    if ( -e "${datdir}/${model}_hybrid.bngl"  and  -e "${model}_hybrid.bngl" )
    {
        print $INDENT . "Checking hybrid model generation.. ";
        $err = system "diff -bB -I '^#' ${model}_hybrid.bngl ${datdir}/${model}_hybrid.bngl >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check hybrid model XML generation
    if ( -e "${datdir}/${model}_hybrid.xml"  and  -e "${model}_hybrid.xml" )
    {
        print $INDENT . "Checking hybrid model XML generation.. ";
        $err = system "diff -bB -I '<!-- Created by BioNetGen .* -->' ${model}_hybrid.xml ${datdir}/${model}_hybrid.xml >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check M-file specification
    if ( -e "${datdir}/${model}.m"  and  -e "${model}.m" )
    {
        print $INDENT . "Checking M-file specification.. ";
        $err = system "diff -bB -I '^%' ${model}.m ${datdir}/${model}.m >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check CVode specification
    if ( -e "${datdir}/${model}_mex_cvode.c"  and  -e "${model}_mex_cvode.c" )
    {
        print $INDENT . "Checking CVode output specification.. ";
        $err = system "diff -bB -I '^%' ${model}_mex_cvode.c ${datdir}/${model}_mex_cvode.c >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check CVode companion M-file specification
    if ( -e "${datdir}/${model}_mex.m"  and  -e "${model}_mex.m" )
    {
        print $INDENT . "Checking CVode companion M-file specification.. ";
        $err = system "diff -bB -I '^%' ${model}_mex.m ${datdir}/${model}_mex.m >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check ODE species trajectories
    if ( -e "$datdir/$model.cdat"  and  -e "$model.cdat" )
    {
        print $INDENT . "Checking species trajectories.. ";
        $err = system "$verifyexec $model.cdat $datdir/$model.cdat >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check ODE observable trajectories
    if ( -e "$datdir/$model.gdat"  and  -e "$model.gdat" )
    {
        print $INDENT . "Checking observable trajectories.. ";
        $err = system "$verifyexec $model.gdat $datdir/$model.gdat >> $log_file";
        if ( $err )
        {
            print "FAILED!!\n";
            print $INDENT . "see $log_file form more details.\n";
            ++$fail_count;
            next;
        }
        print "passed.\n";
    }

    # check SSA equillibrium distribution (observables)
    {
        my $datfile  = "${model}_ssa_equil.gdat";
        my $statfile = "$datdir/${model}_ssa_equil.stats";
        if ( -e $datfile  and  -e $statfile )
        {
            print $INDENT . "Comparing SSA equillibrium distributions.. ";
            
            # validate data
            my $err = validate_equillibrium_data( $datfile, $statfile, $pvalue );
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

    # check NFsim equillibrium distribution (observables)
    if ($check_nfsim)
    {
        my $datfile  = "${model}_nf_equil.gdat";
        my $statfile = "$datdir/${model}_nf_equil.stats";
        if ( -e $datfile  and  -e $statfile )
        {
            print $INDENT . "Comparing NFsim equillibrium distributions.. ";
            
            # validate data
            my $err = validate_equillibrium_data( $datfile, $statfile, $pvalue );
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
    {   delete_files($model);   }
}





## Print summary results and exit
if ($fail_count)
{   print "\n!! $0 failed $fail_count of $test_count validation tests !!\n";   }
else
{   print "\n$0 passed all tests.\n";   }
exit($fail_count);






###                                                      ###
### END OF MAIN SCRIPT. Accessory subroutines are below. ###
###                                                      ###

# script for deleting working files
sub delete_files
{
    my $model = shift;
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
    {   push @files, $model . $suffix;   }
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



# validate stochastic equillibrium distribution
sub validate_equillibrium_data
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

