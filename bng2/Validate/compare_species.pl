#!/usr/bin/perl

use strict;
use warnings;

print "Testing Species..\n";
print join ' ', ('command: ', $0, @ARGV, "\n");

my $err = 0;
my $err_msg;
my $ordered = 0;

while ($ARGV[0] =~ /^-/)
{
    my $arg = shift @ARGV;
    if ( $arg =~ /^-n$/)
    {
        $ordered=0;
    }
    else
    {
        die "Unrecognized command line option: $_";
    }
}


# get filenames
my $sim_file = shift @ARGV;
my $ref_file = shift @ARGV;

# read simulation file
($err_msg, my $species1) = read_file( $sim_file );
if ($err_msg) { die $err_msg; }

# read simulation file
($err_msg, my $species2) = read_file( $ref_file );
if ($err_msg) { die $err_msg; }


# check for equal number of species
if (@$species1 != @$species2)
{
    printf "Incompatible number of species in two files: %d %d\n", scalar @$species1, scalar @$species2;
    $err = 1;
    exit($err);
}


# compare species
if ($ordered)
{
    foreach my $i (0..$#$species1)
    {
        next if ($species1->[$i] eq $species2->[$i]);
        printf "Mismatch at species %d: %s %s\n", $i+1, $species1->[$i], $species2->[$i];
        $err = 1;
    }
}
else
{
    my %spec1_names = ();
    foreach my $spec (@$species1)
    {
        $spec1_names{$spec} = 1;
    }  
    my %spec2_names=();
    foreach my $spec (@$species2)
    {
        $spec2_names{$spec} = 1;
    }  
    foreach my $i (0..$#$species1)
    {
        unless ( exists $spec2_names{$species1->[$i]} )
        {
            printf "No match for species %d of $sim_file: %s\n", $i+1, $species1->[$i];
            $err = 1;
        }
        unless ( exists $spec1_names{$species2->[$i]} )
        {
            printf "No match for species %d of $ref_file: %s\n", $i+1, $species2->[$i];
            $err = 1;
        }
    }
}

my $msg;
if ($err)
{   
    $msg = "FAILED!! species in $sim_file are different than $ref_file.";
}
else
{
    $msg = "$sim_file and $ref_file passed species test.";
}
print "$msg\n\n";
exit($err);




# read species from NET file
sub read_file
{
    my $file_name = shift @_;

    my $err;
    my $species = [];

    # open file
    open( my $fh, '<', $file_name )  or  return ("Couldn't open $file_name: $!");

    while ( my $line = <$fh> )
    {
        if ( $line =~ /^begin species/ )
        {
            while ( $line = <$fh> )
            {
                # exit loop if this is the end of species
                last if ( $line =~ /^end species/ );

                # trim leading whitespace
                $line =~ s/^\s+//;
                # remove comments
                $line =~ s/#.*$//;
                # trim trailing whitespace
                $line =~ s/\s+$//;
                # skip empty lines
                next unless ( $line =~ /\S/ );   

                # split species tokens
                my ($index, $spec, $conc) = split ' ', $line;
                push @$species, $spec;
            }
        }
    }
    close $fh;
    return ($err, $species);
}
