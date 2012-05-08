#!/usr/bin/perl
use strict;
use warnings;
use File::Spec;

# find perl binary
my $perlbin = $^X;

# BNG root directory
my $bngpath;
my $bngexec;
{
    # try to find path in environment variables
    $bngpath = (exists $ENV{'BNGPATH'} ? $ENV{'BNGPATH'} :
                                (exists $ENV{'BioNetGenRoot'} ? $ENV{'BioNetGenRoot'} : undef) );
    unless (defined $bngpath)
    {   # assume we're one folder above BNG path
        $bngpath = File::Spec->updir();
    }
    # define executable
    $bngexec = File::Spec->catfile( $bngpath, "BNG2.pl" );
}


# get all BNGL files
my @files = <*.bngl>;

# run BNG on all files
my @command = ($perlbin, $bngexec, "--log", @files);
print join( " ", @command ), "\n";
system(@command)==0 or die "Some problem running BioNetGen";

