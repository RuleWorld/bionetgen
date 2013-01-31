#!/usr/bin/perl

use strict;
use warnings;


use POSIX qw/floor/;
use Math::Trig qw(tan asin acos atan sinh cosh tanh asinh acosh atanh pi); 
use List::Util qw(min max sum);
use Scalar::Util ("looks_like_number");

print "Testing Reaction Network..\n";
print join ' ', ('command: ', $0, @ARGV, "\n");

my $err = 0; 
my $err_msg;
my $ordered = 0;


# get filenames
my $sim_file = shift @ARGV;
my $ref_file = shift @ARGV;

# read simulation file
($err_msg, my $rxns1, my $params1) = read_file( $sim_file );
if ($err_msg) { die $err_msg; }

# read reference file
($err_msg, my $rxns2, my $params2) = read_file( $ref_file );
if ($err_msg) { die $err_msg; }


# check reaction network
if ( @$rxns1 == @$rxns2 )
{
    if ($ordered)
    {
        foreach my $i ( 0 .. $#$rxns1 )
        {
            next if ( $rxns1->[$i] eq $rxns2->[$i] );
            printf "Mismatch at reaction %d:\n %s\n %s\n", $i+1, $rxns1->[$i], $rxns2->[$i];
            $err = 1;
        }
    }
    else
    {   # permit reactions to be in arbitary order

        # hash the reactions
        my $rxn_hash = {};
        foreach my $rxn (@$rxns1)
        {
            my $rtext = rxn2text($rxn,$params1);
            if ( exists $rxn_hash->{$rtext} )
            {   $rxn_hash->{$rtext}->[0] = $rxn;   }
            else
            {   $rxn_hash->{$rtext} = [ $rxn, undef ];   }
        }   
        foreach my $rxn (@$rxns2)
        {
            my $rtext = rxn2text($rxn,$params2);
            if ( exists $rxn_hash->{$rtext} )
            {   $rxn_hash->{$rtext}->[1] = $rxn;   }
            else
            {   $rxn_hash->{$rtext} = [ undef, $rxn ];   }
        }  

        foreach my $rtext (sort keys %$rxn_hash)
        {
            my ($rxn1,$rxn2) = @{$rxn_hash->{$rtext}};
            if ( defined $rxn1 and not defined $rxn2 )
            {
                printf "No match for rxn %d of test network in %s: (%s)\n", $rxn1->[0], $ref_file, $rtext;
                $err=1;
            }
            elsif ( not defined $rxn1 and defined $rxn2 )
            {
                printf "No match for rxn %d of reference network in %s: (%s)\n", $rxn2->[0], $sim_file, $rtext;
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
    my $params = {};

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
                ## TODO: is this a good idea??
                #$reac = join( ',', sort {$a<=>$b} split(',',$reac) );
                #$prod = join( ',', sort {$a<=>$b} split(',',$prod) );
                # remove empty argument list from ratelaW
                $rate =~ s/\(\)//g;
                # compute numeric
                push @$reactions, [$index, $reac, $prod, $rate];
            }
        }
        elsif ( $line =~ /^begin parameters/ )
        {
            while ( $line = <$fh> )
            {
                # exit loop if this is the end of reactions
                last if ( $line =~ /^end parameters/ );

                # trim leading whitespace
                $line =~ s/^\s+//;
                # remove comments
                $line =~ s/#.*$//;
                # trim trailing whitespace
                $line =~ s/\s+$//;
                # skip empty lines
                next unless ( $line =~ /\S/ );    

                # parse parameter definition (this isn't very robust)
                my ($index, $name, $value) = split ' ', $line;
                # add to param hash
                $params->{$name} = $value;
            }
        }
    }
    close $fh;
    return $err, $reactions, $params;
}



sub rxn2text
{
    my ($rxn, $params) = @_;

    my $reactants = join(" + ", map {"S$_"} split(",", $rxn->[1]) );
    my $products  = join(" + ", map {"S$_"} split(",", $rxn->[2]) );

    # evaluate the reaction rate in crude fashion..
    my $rate = $rxn->[3];    
    my $last_rate = $rate;
    my $iter = 0;
    my $max_iter = 100;
    while ($iter < $max_iter)
    {
        while ( my ($par,$val) = each %$params )
        {   # substitute this parameter
            $rate =~ s/(^|[\+\-\/\*\^\(\)\ ])$par([\+\-\/\*\^\(\)\ ]|$)/$1($val)$2/g;
        }

        ++$iter;
        $last_rate = $rate;

        last if ($rate eq $last_rate);
    }

    if ( looks_like_number($rate) )
    {   # limit resolution to avoid round-off errors
        $rate = sprintf "%.8g", $rate;
    }
    else
    {   # try to evaluate
        my $eval_rate = eval "$rate";
        if (defined $eval_rate)
        {   $rate = $eval_rate;   }
   
        if ( looks_like_number($rate) )
        {   $rate = sprintf( "%.8g", $rate);   }
    }

    # generate rxn string
    return "$reactants -> $products at $rate";   
}

