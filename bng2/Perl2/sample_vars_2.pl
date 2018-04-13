#!/usr/bin/perl
# Simple parameter scanning script.  Creates and runs a single BNGL file that
# scans a single parameter using the setParameter command".  User provides
# a BNGL file containing the model - actions in this file are ignored.
#
# Written by Jim Faeder, Los Alamos National Laboratory, 3/6/2007 

# Updated on 2 April 2012 by J.Hogg
# + checks environment variables BNGPATH and BioNetGenRoot for the BNG folder
# + now using File::Spec for platform independent path handling
# + output now written in current directory (not the model directory!)
# + This version requires BioNetGen version 2.2.0-testing

# Additional updates by L. Harris (2012,2013)
# -method argument
# require 'begin/end model'
# -verbose argument
# various bug fixes

use strict;
use warnings;

# Perl Modules
use FindBin;
use lib $FindBin::RealBin;
use File::Spec;
use File::Path qw(remove_tree);
use Config;
use IO::Handle;
use IPC::Open3;
use POSIX;

# Set up Signal Handlers..
# define global variable to store PID of child process.
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
        print "\n>>>relaying TERM signal to child with PID: ", $::CHILD_PID, " <<<\n";
        kill $SIGNO{"TERM"}, $::CHILD_PID;
    }
    die "BioNetGen received TERM signal";
};
# INT signal handler: make sure any child processes are shutdown before termination
$SIG{'INT'} = sub
{
    if (defined $::CHILD_PID)
    {   # kill off child process
        print "\n>>> relaying INT signal to child with PID: ", $::CHILD_PID, " <<<\n";
        kill $SIGNO{"INT"}, $::CHILD_PID;
    }
    die "scan_var received INT signal";
};



# get perl binary
my $perlbin = $^X;

# get BNG path
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
        $bngpath = File::Spec->catpath( $volume, File::Spec->catdir(@dirs), '' );
    }
    # define executable
    $bngexec = File::Spec->catfile( $bngpath, "BNG2.pl" );
}

# some default parameters
my $log     = 0;
my $t_end   = 20;
my $n_steps = 1;
my $steady_state = 0;
my $method = "\"ode\"";
my $pla_output = 0;
my $verbose = 0;
my $samplingMethod = "\"random\"";
my $varMeasured = "\"end\"";
my $n_dims = 2;

my $prefix;
my @mandatory_args = ();
while ( @ARGV )
{
    my $arg = shift @ARGV;
    if ( $arg =~ s/^(-{1,2})// )
    {  
        if ( $arg eq 'log' ){
            $log = 1;
        }
        elsif($arg eq 'n_steps'){
#            unless (@ARGV) { die "Syntax error: $arg requires value"; }
            $n_steps = shift @ARGV;
            if ($n_steps =~ /^-{1,2}/ || $n_steps =~ /\.bngl$/)
            { die "Syntax error: '$arg' requires value"; }
        }
        elsif($arg eq 'prefix'){
#            unless (@ARGV) { die "Syntax error: $arg requires value"; }
            $prefix = shift @ARGV;
            if ($prefix =~ /^-{1,2}/ || $prefix =~ /\.bngl$/)
            { die "Syntax error: '$arg' requires value"; }
        }
        elsif($arg eq 'steady_state'){
            $steady_state = 1;
        }
        elsif($arg eq 't_end'){
#            unless (@ARGV) { exit_error("Syntax error: $arg requires value"); }
            $t_end = shift @ARGV;
            if ($t_end =~ /^-{1,2}/ || $t_end =~ /\.bngl$/)
            { die "Syntax error: '$arg' requires value"; }
        }
        elsif($arg eq 'method'){
            $method = shift @ARGV;
            if ($method =~ /^-{1,2}/ || $method =~ /\.bngl$/)
            { die "Syntax error: '$arg' requires value"; }
            if ($method eq 'pla'){
            	my $pla_config = shift @ARGV;
            	if ($pla_config =~ /^-{1,2}/ || $pla_config =~ /\.bngl$/)
            	{ die "Syntax error: PLA simulator requires a simulation configuration: --method pla CONFIG. " . 
            	      "Please try again."; }
            	$method = "\"$method\",pla_config=>\"$pla_config\"";
            }
            else{
            	$method = "\"$method\"";
            }
        }
        elsif($arg eq 'pla_output'){
        	$pla_output = 1;
        }
        elsif($arg eq 'verbose'){
            $verbose = 1;
        }
        elsif($arg eq 'help'){
            display_help();
            exit 0;
        }
        elsif($arg eq 'samplingMethod'){
            $samplingMethod = shift @ARGV;
            if ($samplingMethod =~ /^-{1,2}/ || $samplingMethod =~ /\.bngl$/)
            { die "Syntax error: '$arg' requires value"; }
            $samplingMethod = "\"$samplingMethod\"";
        }
        elsif($arg eq 'varMeasured'){
            $varMeasured = shift @ARGV;
            if ($varMeasured =~ /^-{1,2}/ || $varMeasured =~ /\.bngl$/)
            { die "Syntax error: '$arg' requires value"; }
            $varMeasured = "\"$varMeasured\"";
        }
        elsif($arg eq 'n_dims'){
            #            unless (@ARGV) { die "Syntax error: $arg requires value"; }
            $n_dims = shift @ARGV;
            if ($n_dims =~ /^-{1,2}/ || $n_dims =~ /\.bngl$/)
            { die "Syntax error: '$arg' requires value"; }
        }
        else{
            die "Syntax error: unrecognized command line option $arg";
        }
    }
    else
    {   # assume this is a mandatory argument
        push @mandatory_args, $arg;
    }
}
#print "args: ", join(',', @mandatory_args), "\n";
unless (@mandatory_args== 2 + 3*$n_dims)
{   # complain about too few arguments
    die "Syntax error: 8 required arguments (Try scan_var.pl --help)";
}

# get mandatory arguments
#my ($file, $var1, $var_min1, $var_max1, $var2, $var_min2, $var_max2, $n_pts) = @mandatory_args;
my $file = $mandatory_args[0];
my $n_pts = $mandatory_args[1 + 3*$n_dims];
my $offset;
my $division;
if ($samplingMethod eq "\"grid\"") {
    $division = $n_pts - 1;
    $n_pts = $n_pts**$n_dims
}
if ($samplingMethod eq "\"latin\"") {
    $offset = 1;
} elsif ($samplingMethod eq "\"random\"" | $samplingMethod eq "\"grid\"") {
    $offset = 3;
}
$n_pts += $offset;


# Automatic assignment of prefix if unset
unless ($prefix)
{
	# separate directories from filename (if any)
    my ($volume,$directories,$file) = File::Spec->splitpath( $file );
	unless ( $file =~ /\.bngl$/ )
	{   die "Cannot extract prefix from filename $file";   }
	# trim bngl extension to define prefix
	($prefix = $file) =~ s/\.bngl$//;
}
# add variable name to prefix
for (my $i = 0; $i < $n_dims; $i++) {
    my $var = $mandatory_args[3*$i + 1];
    $prefix.="_${var}";
}


if ($log)
{   # convert min and max into log values
    for (my $i = 0; $i < $n_dims; $i++) {
        $mandatory_args[2 + 3*$i] = log($mandatory_args[2 + 3*$i]);
        $mandatory_args[3 + 3*$i] = log($mandatory_args[3 + 3*$i]);
    }
}

# calculate parameter step
#my $delta;
#if ($var_max==$var_min){
#	$delta = 0;
#}
#else{
#	$delta = ($var_max-$var_min)/($n_pts-1);
#}

# Read file 
open(IN, $file) or die "Couldn't open file $file: $?\n";
my $script = "";
while ( my $line = <IN> )
{
    $script .= $line;
    # Skip actions
    last if ($line =~ /^\s*end\s+model/);
}
close(IN);

# Require 'end model' directive
if (!($script =~ /\n\s*end\s+model/)){
	die "ERROR: scan_var requires model to be enclosed in 'begin model' / 'end model' block.";
}

# prepare working directory
if (-d $prefix)
{   # delete output directory
    print "WARNING: overwriting existing work directory $prefix.\n";
    my $err;
    remove_tree( $prefix, {'safe'=>1, 'keep_root'=>1, 'error'=> \$err} );
    if (@$err)
    {   die "Unable to delete files in work directory $prefix";   }
}
else
{   # make directory for output files
    mkdir $prefix;
}

# Create model file with scan actions
my $scanmodel = File::Spec->catfile( ${prefix}, "${prefix}.bngl" );
my $logfile   = File::Spec->catfile( ${prefix}, "${prefix}.log" );

open(BNGL,">", $scanmodel) or die "Couldn't open $scanmodel for output";
print BNGL $script;
print BNGL "\n";
print BNGL "generate_network({overwrite=>1})\n";
print $script;
print "\n";
print "generate_network({overwrite=>1})\n";
my @vals;
my @val;

{
    srand(5.014);
    my @count = (0)x$n_dims;
    foreach my $run (1..$n_pts)
    {
        if ($samplingMethod eq "\"random\"") {
            for ($i = 0; $i < $n_dims; $i++) {
                $val[$i] = $mandatory_args[3*$i + 2] + rand()*($mandatory_args[3*$i + 3]-$mandatory_args[3*$i + 2]);
            }
        } elsif ($samplingMethod eq "\"grid\"") {
            if ($run == 1) {
                for ($i = 0; $i < $n_dims; $i++) {
                    $val[$i] = $mandatory_args[3*$i + 2];
                }
            } else {
                my $carry = 1;
                #my $division = ($n_pts - 3)**(1/$n_dims) - 1;
                for ($i = 0; $i < $n_dims; $i++) {
                    if ($carry) {
                        #my $range = $mandatory_args[3*$i + 3]-$mandatory_args[3*$i + 2];
                        #my $division = $n_pts**(1/$n_dims);
                        #my $increment = $range/$division;
                        #print("$range $division $increment\n");
                        $val[$i] = $val[$i] + ($mandatory_args[3*$i + 3]-$mandatory_args[3*$i + 2])/$division;
                        $count[$i]++;
                        my $difference = $division - $count[$i];
                        if ($count[$i] > $division) {
                            #$val[$i] = $val[$i] - ($mandatory_args[3*$i + 3]-$mandatory_args[3*$i + 2]);
                            $val[$i] = $mandatory_args[3*$i + 2];
                            $count[$i] = 0;
                        } else {
                            $carry = 0;
                        }
                    }
                }
            }
        } elsif ($samplingMethod eq "\"latin\"") {
            for ($i = 0; $i < $n_dims; $i++) {
                do {
                    $val[$i] = $mandatory_args[3*$i + 2] + ($mandatory_args[3*$i + 3] - $mandatory_args[3*$i + 2])*int(rand($n_pts))/$n_pts;
                    
                } while (grep( /^$val[$i]$/, @{$vals[$i]}));
            }
        }
        #print("$run ");
        #foreach my $x (@val) {
        #    my $y = $log ? exp($x) : $x;
        #    printf "%16.8e", $y;
        #}
        #print("\n");
        #print("@val\n");
        push @vals, [@val];
        #print("$run @val\n");
        my $srun = sprintf "%05d", $run;
        if ($run > 1){
            #print BNGL "resetConcentrations()\n";
            #print "resetConcentrations()\n";
        }
        for my $i (0 .. $#val) {
            my $x = $val[$i];
            my $var = $mandatory_args[3*$i + 1];
            if ($log){ $x = exp($x);}
            #print("$x ");
            printf BNGL "setParameter(\"$var\",%.12g)\n", $x;
            printf "setParameter(\"$var\",%.12g)\n", $x;
        }
        my $opt = "method=>$method";
        if ($pla_output){
        	$opt .= ",pla_output=>1";
        }
        $opt .= ",suffix=>\"$srun\",t_end=>$t_end,n_steps=>$n_steps";
        if ($steady_state){
            $opt .= ",steady_state=>1";
        }
        if ($verbose){
        	$opt .= ",verbose=>1";
        }
        printf BNGL "simulate({$opt})\n"; #"simulate_ode({$opt})\n";
        printf BNGL "resetConcentrations()\n";
        printf "simulate({$opt})\n"; #"simulate_ode({$opt})\n";
        printf "resetConcentrations()\n";
    }  
}
close(BNGL);




# Run BioNetGen on ScanModel file
#print "Running BioNetGen on $scanmodel\n";
my @command = ($perlbin, $bngexec, "--outdir", $prefix, $scanmodel);
print("@command\n");

# open logfile
open( my $logFH, ">", $logfile ) or die $!;
$logFH->autoflush(1);

# start simulator as child process with communication pipes
my ($child_in, $child_out); 
my $pid = eval{ open3( $child_in, $child_out, $child_out, @command ) };
if ($@) { die $@; }

# remember child PID
$::CHILD_PID = $pid;
#print "[child process ID is: $pid]\n";

# monitor output of child process
while( my $line = <$child_out> )
{
    if ( $line =~ /^ABORT/ )
    {   # some problem
        print $logFH $line;
        print $logFH "Problem running BioNetGen on ${scanmodel}.";
        # also write message to STDOUT
        die "Problem running BioNetGen on $scanmodel";
    }
    else
    {   # write message to log
        print $logFH $line;
		if ($verbose){
            #print $line; # to STDOUT
		}
    }
}

# make sure child process has completed
waitpid($pid,0);

# clear child pid
$::CHILD_PID = undef;


$n_pts -= $offset;
# Process output
my $outfile = "${prefix}.scan";
open(OUT,">", $outfile) or die "Couldn't open $outfile for output ($!)";
{
    foreach my $run (1..$n_pts)
    {
        my @val1;
        for (my $k= 0; $k < $n_dims; $k++)
        {
            if ($samplingMethod eq "\"latin\"") {
                push @val1, $vals[$run + $n_dims - 1][$k];
            } else {
                push @val1, $vals[$run - 1][$k];
            }
        }
        #{ local $\ = "\n"; print "\t [ @val1 ]," }
        # Get data from gdat file
        my $gdat_file = File::Spec->catfile( $prefix, sprintf("${prefix}_%05d.gdat", $run) );
        print "Extracting data from $gdat_file\n";
        #print "$gdat_file\n";
        open(IN, "<", $gdat_file) or die "Couldn't open $gdat_file for input ($!)";
        if ($run==1)
        {
            my $head = <IN>;
            $head =~ s/^\s*\#//;
            my @heads = split(' ',$head);
            shift @heads;
            for (my $i = 0; $i < $n_dims; $i++) {
                printf OUT "# %+14s", $mandatory_args[3*$i + 1];
            }
            foreach my $head (@heads)
            {
                printf OUT " %+16s", $head;
            }
            print OUT "\n";
        }
        my @last;
        while (my $line = <IN>) {
            my @dat = split(' ',$line);
            @last = @dat;
            #print("@dat\n");
        }
        open(IN, "<", $gdat_file) or die "Couldn't open $gdat_file for input ($!)";
        my @max;
        my $count = 0;
        my @dat;
        my @peakInt;
        #foreach my $x (@val1) {
        #    $x = $log ? exp($x) : $x;
        #    print("$x");
        #}
        #print("\n");
        while (my $line = <IN>) {
            my @dat = split(' ',$line);
            #print("@dat \n");
            if ($count == 1) {
                @max = @dat;
                for $i (0 .. $#peakInt) {
                    $peakInt[$i] = 0;
                }
            } elsif ($count > 1) {
                my $i = 0;
                foreach my $point (@dat) {
                    if ($max[$i] < $point) {
                        $max[$i] = $point;
                    }
                    if ($dat[$i] > $last[$i]) {
                        $peakInt[$i] += $dat[$i] - $last[$i];
                    }
                    $i++;
                }
            }
            $count++;
        }
        @dat = @last;
        if ($varMeasured eq "\"peakHeight\"") {
            for my $i (0 .. $#dat) {
                $dat[$i] = $max[$i] - $last[$i];
            }
        } elsif ($varMeasured eq "\"peakRatio\"") {
            for $i (0 .. $#dat) {
                $dat[$i] = ($max[$i] + DBL_EPSILON*100)/($last[$i] + DBL_EPSILON*100);
            }
        } elsif ($varMeasured eq "\"peakIntegral\"") {
            @dat = @peakInt;
        } elsif ($varMeasured eq "\"peakIntRatio\"") {
            for $i (0 .. $#dat) {
                $dat[$i] = ($peakInt[$i] + DBL_EPSILON*100)/($last[$i] + DBL_EPSILON*100);
            }
        } elsif ($varMeasured eq "\"peakWidth\"") {
            for my $i (0 .. $#dat) {
                $dat[$i] = $peakInt[$i]/$max[$i];
            }
        }
        #print("@dat\n");
        #my @dat = split(' ',$last);
        my $time = shift @dat;
        foreach my $x (@val1) {
            $x = $log ? exp($x) : $x;
            printf OUT "%16.8e", $x;
        }
        foreach my $data ( @dat )
        {
            printf OUT " %16.8e", $data;
        }
        print OUT "\n";
        close(IN);
    }
}
close(OUT);
print "Final state data printed to $outfile\n";
exit 0;




# display help
sub display_help
{
#    print <<END_HELP
print qq{
scan_var.pl: a simple parameter scan utility for BioNetGen
----------------------------------------------------------
SYNOPSIS:
  scan_var.pl [OPTS] MODEL (VAR MIN MAX)Xn_dims NPTS     : perform parameter scan
  scan_var.pl --help                            : display help

OPTIONS:
  --verbose           : verbose output
  --log               : select parameter values on a log scale
  --n_steps N         : number of output time steps per simulation
  --t_end VAL         : end time for each simulation
  --prefix PREFIX     : prefix for output file (default: MODEL basename)
  --steady-state      : check for steady state at end of each simulation
                    (ignored if method not 'ode')
  --method METHOD     : simulation method (default: ode)
  --pla_output        : print PLA-specific output (ignored if method not 'pla')
  --samplingMethod    : specify method of parameter sampling (default random ('random'), grid ('grid'), or latin hypercube ('latin'))
  --varMeasured       : specify how output is derived from observables (default final value ('end'), maximum ('peakHeight'),
                                                                        ratio of maximum and endpoint ('peakRatio'),
                                                                        area under timecourse but above endpoint ('peakIntegral'),
                                                                        ratio between integral and endpoint ('peakIntRatio'),
                                                                        or ratio between integral and maximum ('peakHeight'))
  --n_dims            : number of variables to sample over (specify variable name, minimum, and maximum for each variable being scanned)

Runs simulations of MODEL with a range of values of parameter VAR using the method 
METHOD. Simulation data is placed in a directory folder named PREFIX_VAR. A data 
file called PREFIX_VAR.scan contains the final simulation state for each parameter 
value. The scan file may be visualized with a plotting tool, such as PhiBPlot.
};
#END_HELP
}

