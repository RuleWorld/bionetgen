#!/usr/bin/perl

use strict;
use warnings;


use POSIX qw/floor/;
use Math::Trig qw(tan asin acos atan sinh cosh tanh asinh acosh atanh pi); 
use List::Util qw(min max sum);
use Scalar::Util ("looks_like_number");
use Safe;

print "Testing Reaction Network..\n";
print join ' ', ('command: ', $0, @ARGV, "\n");

my $err = 0; 
my $err_msg;
my $ordered = 0;


# get filenames
my $sim_file = shift @ARGV;
my $ref_file = shift @ARGV;

# read simulation file
($err_msg, my $rxns1, my $params1, my $species1) = read_file( $sim_file );
if ($err_msg) { die $err_msg; }

# read reference file
($err_msg, my $rxns2, my $params2, my $species2) = read_file( $ref_file );
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

        # hash the reactions using species-string-aware comparison
        # This makes the comparison invariant to species ordering differences
        my $rxn_hash = {};
        foreach my $rxn (@$rxns1)
        {
            my $rtext = rxn2text($rxn,$params1,$species1);
            if ( exists $rxn_hash->{$rtext} )
            {   $rxn_hash->{$rtext}->[0] = $rxn;   }
            else
            {   $rxn_hash->{$rtext} = [ $rxn, undef ];   }
        }
        foreach my $rxn (@$rxns2)
        {
            my $rtext = rxn2text($rxn,$params2,$species2);
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
    my $species = {};

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
        elsif ( $line =~ /^begin species/ )
        {
            while ( $line = <$fh> )
            {
                last if ( $line =~ /^end species/ );
                $line =~ s/^\s+//;
                $line =~ s/#.*$//;
                $line =~ s/\s+$//;
                next unless ( $line =~ /\S/ );
                my ($index, $spec, $conc) = split ' ', $line;
                $species->{$index} = $spec;
            }
        }
    }
    close $fh;
    return $err, $reactions, $params, $species;
}



sub rxn2text
{
    my ($rxn, $params, $species) = @_;

    # Use species fingerprints for comparison (invariant to canonical form differences)
    my $reactants;
    my $products;
    if (defined $species and %$species) {
        $reactants = join(" + ", sort map { species_fingerprint($species->{$_} || "S$_") } split(",", $rxn->[1]) );
        $products  = join(" + ", sort map { species_fingerprint($species->{$_} || "S$_") } split(",", $rxn->[2]) );
    } else {
        $reactants = join(" + ", map {"S$_"} split(",", $rxn->[1]) );
        $products  = join(" + ", map {"S$_"} split(",", $rxn->[2]) );
    }

    # evaluate the reaction rate in crude fashion..
    # TODO: implement a more reliable way to do this (but don't make it
    # depend on the BNG expression parser, as it might produce the same bugs.)
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

        # try to substitute log for ln
        $rate =~ s/(^|[\+\-\/\*\^\(\ ])ln\(/$1log(/g;

        # try to substitute ** for ^
        $rate =~ s/([\w\)\.\ ])\^([\w\+\-\.\(\ ])/$1**$2/g;

        ++$iter;
        last if ($rate eq $last_rate);
        $last_rate = $rate;
    }

    if ( looks_like_number($rate) )
    {   # limit resolution to avoid round-off errors
        $rate = sprintf "%.8g", $rate;
    }
    else
    {   # try to evaluate safely
        my $cpt = Safe->new;
        $cpt->permit(qw(atan2 sin cos exp log sqrt pow entereval));
        $cpt->share('tan', 'asin', 'acos', 'atan', 'sinh', 'cosh', 'tanh', 'asinh', 'acosh', 'atanh', 'pi');
        my $eval_rate = $cpt->reval($rate);

        if (defined $eval_rate)
        {   $rate = $eval_rate;   }
   
        if ( looks_like_number($rate) )
        {   $rate = sprintf( "%.8g", $rate);   }
    }

    # generate rxn string
    return "$reactants -> $products at $rate";
}


# Compute a graph-structure fingerprint for a BNG species string.
# Invariant to molecule/component ordering and bond numbering.
sub species_fingerprint {
    my $spec = shift;

    my @mol_strings;
    my $depth = 0;
    my $cur = '';
    for my $ch (split //, $spec) {
        if ($ch eq '(') { $depth++; $cur .= $ch; }
        elsif ($ch eq ')') { $depth--; $cur .= $ch; }
        elsif ($ch eq '.' && $depth == 0) {
            push @mol_strings, $cur; $cur = '';
        }
        else { $cur .= $ch; }
    }
    push @mol_strings, $cur if $cur ne '';

    my %bond_endpoints;
    my @mol_infos;
    for my $mi (0 .. $#mol_strings) {
        my $ms = $mol_strings[$mi];
        my ($mol_name, $comp_str);
        if ($ms =~ /^([^(]+)\(([^)]*)\)(.*)$/) {
            $mol_name = $1 . ($3 // '');
            $comp_str = $2;
        } elsif ($ms =~ /^([^(]+)\(\)(.*)$/) {
            $mol_name = $1 . ($2 // '');
            $comp_str = '';
        } else {
            push @mol_infos, { name => $ms, comps => [] };
            next;
        }
        my @comps;
        for my $cs (split /,/, $comp_str) {
            my $key = $cs;
            my @bonds;
            while ($key =~ s/!(\d+)//) { push @bonds, $1; }
            push @comps, { key => $key, bonds => \@bonds };
            for my $b (@bonds) {
                push @{$bond_endpoints{$b}}, [$mi, $key];
            }
        }
        push @mol_infos, { name => $mol_name, comps => \@comps };
    }

    my @fp_parts;
    for my $mol (@mol_infos) {
        my @comp_keys = sort map {
            my $bc = scalar @{$_->{bonds}};
            $_->{key} . ($bc > 0 ? "!$bc" : "")
        } @{$mol->{comps}};
        push @fp_parts, "M:" . $mol->{name} . "(" . join(",", @comp_keys) . ")";
    }
    for my $bnum (sort { $a <=> $b } keys %bond_endpoints) {
        my @eps = @{$bond_endpoints{$bnum}};
        next unless @eps == 2;
        my @edge = sort map {
            $mol_infos[$_->[0]]{name} . ":" . $_->[1]
        } @eps;
        push @fp_parts, "B:" . join("--", @edge);
    }

    return join("|", sort @fp_parts);
}

