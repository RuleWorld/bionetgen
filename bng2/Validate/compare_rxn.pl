#!/usr/bin/perl

use strict;
use warnings;

print "Testing Reaction Network..\n";
print join ' ', ('command: ', $0, @ARGV, "\n");

my $err = 0; 
my $err_msg;
my $ordered = 0;


# get filenames
my $sim_file = shift @ARGV;
my $ref_file = shift @ARGV;

# read simulation file
($err_msg, my $rxn1) = read_file( $sim_file );
if ($err_msg) { die $err_msg; }

# read reference file
($err_msg, my $rxn2) = read_file( $ref_file );
if ($err_msg) { die $err_msg; }


# check reaction network
if ( @$rxn1 == @$rxn2 )
{
    if ($ordered)
    {
        foreach my $i ( 0 .. $#$rxn1 )
        {
            next if ( $rxn1->[$i] eq $rxn2->[$i] );
            printf "Mismatch at reaction %d:\n %s\n %s\n", $i+1, $rxn1->[$i], $rxn2->[$i];
            $err = 1;
        }
    }
    else
    {   # permit reactions to be in arbitary order
        my %rxns1 = ();
        foreach my $rxn (@$rxn1)
        {
            $rxns1{$rxn} = 1;
        }   
        my %rxns2 = ();
        foreach my $rxn (@$rxn2)
        {
            $rxns2{$rxn} = 1;
        }  
        foreach my $i ( 0..$#$rxn1 )
        {
            unless ( exists $rxns2{ $rxn1->[$i] } )
            {
                printf "No match for rxn %d of $ref_file: %s\n", $i+1, $rxn1->[$i];
                $err=1;
            }
            unless ( exists $rxns1{ $rxn2->[$i] } )
            {
                printf "No match for rxn %d of $sim_file: %s\n", $i+1, $rxn2->[$i];
                $err=1;
            }
        }
    }
}
else
{
    printf "Reaction networks are not the same size: %d (sim) versus %d (ref)";
    $err = 1;
}


my $msg;
if ($err)
{   $msg = "FAILED!! reaction network in $sim_file is different than $ref_file.";   }
else
{   $msg = "$sim_file and $ref_file passed reaction network test.";   }
print "$msg\n\n";
exit($err);




# read reactions from NET file
sub read_file
{
    my $file_name = shift @_;

    my $err;
    my $reactions = [];

    # open file
    open( my $fh, '<', $file_name )  or  return ("Couldn't open $file_name: $!");

    while ( my $line = <$fh> )
    {
        if ( $line =~ /^begin reactions/ )
        {
            while ( $line = <$fh> )
            {
                # exit loop if this is the end of reactions
                last if ( $line =~ /^end reactions/ );

                # trim leading whitespace
                $line =~ s/^\s+//;
                # remove comments
                $line =~ s/#.*$//;
                # trim trailing whitespace
                $line =~ s/\s+$//;
                # skip empty lines
                next unless ( $line =~ /\S/ );    

                # parse reaction
                my ($index, $reac, $prod, $rate) = split ' ', $line;
                $reac = join( ',', sort {$a<=>$b} split(',',$reac) );
                $prod = join( ',', sort {$a<=>$b} split(',',$prod) );
                # remove empty argument list from ratelaW
                $rate =~ s/\(\)//;
                # compute numeric
                push @$reactions, "$reac $prod $rate";
            }
        }
    }
    close $fh;
    return ($err, $reactions);
}
