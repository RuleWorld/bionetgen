#!/usr/bin/perl
# Simple parameter scanning script.  Creates and runs a single BNGL file that
# scans a single parameter using the setParameter command".  User provides
# a BNGL file containing the model - actions in this file are ignored.
#
# Written by Jim Faeder, Los Alamos National Laboratory, 3/6/2007 

# Updated on 2 April 2012 by J.Hogg
# + now looks at environment variables BNGPATH and BioNetGenRoot for the BNG root folder
# + now using File::Spec for platform independent path handling
# + output now written in current directory (not the model directory!)
# + This version requires BioNetGen version 2.2.0 testing


# Perl Modules
use FindBin;
use lib $FindBin::RealBin;
use File::Spec;

# get BNG path
my $BNGPATH;
my $BNGEXEC;
{
    # try to find path in environment variables
    $BNGPATH = (exists $ENV{'BNGPATH'} ? $ENV{'BNGPATH'} :
                                (exists $ENV{'BioNetGenRoot'} ? $ENV{'BioNetGenRoot'} : undef) );
    unless (defined $BNGPATH)
    {   # use FindBin to locate BNG
        my ($volume,$directories,$file) = File::Spec->splitpath( $FindBin::RealBin );
        my @dirs = File::Spec->splitdir( $directories );
        pop @dirs;   # BNG executable script should be down one directory from here
        $BNGPATH = File::Spec->catdir( $volume, File::Spec->catdir(@dirs) );
    }
    # define executable
    $BNGEXEC = File::Spec->catfile( $BNGPATH, "BNG2.pl" );
}

# some default parameters
my $log     = 0;
my $t_end   = 20;
my $n_steps = 20;
my $steady_state = 0;

my $prefix;
while ($ARGV[0] =~ /^-/)
{
    $_ = shift;
    if (/^-log$/){
        $log=1;
    }
    elsif(/^-n_steps/){
        $n_steps = shift @ARGV;
    }
    elsif(/^-prefix/){
        $prefix = shift @ARGV;
    }
    elsif(/^-steady_state/){
        $steady_state = 1;
    }
    elsif(/^-t_end/){
        $t_end = shift @ARGV;
    }
    else{
        exit_error("Unrecognized command line option $_");
    }
}

if ($#ARGV != 4) {
    die "Usage:[ $0 file.bngl varname var_min var_max n_pts ]";
}

# get mandatory arguments
my $file    = shift @ARGV;
my $var     = shift @ARGV;
my $var_min = shift @ARGV;
my $var_max = shift @ARGV;
my $n_pts   = shift @ARGV;

# Automatic assignment of prefix if unset
unless ($prefix)
{
	# separate directories from filename (if any)
    my ($volume,$directories,$file) = File::Spec->splitpath( $file );
	unless ( $file =~ /\.bngl$/ )
	{   die "Cannot extract prefix from filename $file";   }
	# trim bngl extension to define prefix
	($prefix = $file) =~ s/\.bngl$//;
	# add variable name to prefix
	$prefix.="_${var}";
}

if ($log){
    $var_min = log($var_min);
    $var_max = log($var_max);
}

my $delta = ($var_max-$var_min)/($n_pts-1);

# Read file 
open(IN,$file) || die "Couldn't open file $file: $?\n";
my $script="";
while ( my $line = <IN> )
{
    $script .= $line;
    # Skip actions
    last if (/^\s*end\s*model\s*$/);
}

if (-d $prefix){
    # delete output directory
    system("rm -r $prefix");
    # die "Directory $prefix exists.  Remove before running this script.";
}

# make directory for output files
mkdir $prefix;

# Create input file scanning variable
my $scanmodel = File::Spec->catfile( ${prefix}, "${prefix}.bngl" );
my $logfile   = File::Spec->catfile( ${prefix}, "${prefix}.log" );

open(BNGL,">", $scanmodel) or die "Couldn't write to $scanmodel";
print BNGL $script;
print BNGL "generate_network({overwrite=>1});\n";
print BNGL "saveConcentrations()\n";

my $val = $var_min;
foreach my $run (1..$n_pts)
{
    my $srun = sprintf "%05d", $run;
    if ($run>1){
        print BNGL "resetConcentrations()\n";
    }
    my $x= $val;
    if ($log){ $x = exp($val);}
    printf BNGL "setParameter(\"$var\",$x);\n";

    my $opt= "suffix=>\"$srun\",t_end=>$t_end,n_steps=>$n_steps";
    if ($steady_state){
        $opt.=",steady_state=>1";
    }
    printf BNGL "simulate_ode({$opt});\n";
    $val+=$delta;
}  
close(BNGL);

# Run BioNetGen on file
print "Running BioNetGen on $scanmodel\n";
my $BNGARGS = "-outdir \"${prefix}\"";
system("$BNGEXEC $BNGARGS $scanmodel > $logfile");

# Process output
my $outfile = "${prefix}.scan";
open(OUT,">", $outfile) or die "Couldn't open $outfile";
#my $val= $var_min;
$val = $var_min;
foreach my $run (1..$n_pts)
{
    # Get data from gdat file
    my $gdat_file = File::Spec->catfile( $prefix, sprintf("${prefix}_%05d.gdat", $run) );
    print "Extracting data from $gdat_file\n";
    open(IN, "<", $gdat_file) or die "Couldn't open $gdat_file";
    if ($run==1)
    {
        my $head= <IN>;
        $head=~ s/^\s*\#//;
        my @heads= split(' ',$head);
        shift(@heads);
        printf OUT "# %+14s", $var;
        foreach my $head (@heads){
            printf OUT " %+14s", $head;
        }
        print OUT "\n";
    }
    while (my $line = <IN>) { $last = $line };
    my @dat = split(' ',$last);
    my $time = shift @dat;
    my $x = ($log)? exp($val) : $val;
    printf OUT "%16.8e %s\n", $x, join(' ',@dat);
    close(IN);
    $val += $delta;
}
close(OUT);
print "Final state data printed to $outfile\n";


