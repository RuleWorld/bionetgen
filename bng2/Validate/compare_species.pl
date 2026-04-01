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
    my %spec1_fp = ();
    foreach my $spec (@$species1)
    {
        $spec1_fp{species_fingerprint($spec)} = 1;
    }
    my %spec2_fp = ();
    foreach my $spec (@$species2)
    {
        $spec2_fp{species_fingerprint($spec)} = 1;
    }
    foreach my $i (0..$#$species1)
    {
        my $fp1 = species_fingerprint($species1->[$i]);
        unless ( exists $spec2_fp{$fp1} )
        {
            printf "No match for species %d of $sim_file: %s\n", $i+1, $species1->[$i];
            $err = 1;
        }
        my $fp2 = species_fingerprint($species2->[$i]);
        unless ( exists $spec1_fp{$fp2} )
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




# Compute a graph-structure fingerprint for a BNG species string.
# The fingerprint is invariant to molecule ordering, component ordering,
# and bond numbering. Two isomorphic species produce the same fingerprint.
sub species_fingerprint {
    my $spec = shift;

    # Parse into molecules: split on '.' outside parentheses
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

    # Parse each molecule into components with bond info
    # bond_endpoints{bond_num} = [ [mol_idx, comp_name_state], ... ]
    my %bond_endpoints;
    my @mol_infos;  # [{name, comps: [{key, bonds}]}]
    for my $mi (0 .. $#mol_strings) {
        my $ms = $mol_strings[$mi];
        my ($mol_name, $comp_str, $suffix);
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

    # Build fingerprint as a sorted multiset of:
    # 1. Unbound components: "UNCOMP:mol_name:comp_key"
    # 2. Bond edges: sorted pair "BOND:mol1_name:comp1_key--mol2_name:comp2_key"
    # 3. Molecule signatures: "MOL:name:sorted_comp_keys"
    my @fp_parts;

    # Molecule signatures (captures unbound structure)
    for my $mol (@mol_infos) {
        my @comp_keys = sort map {
            my $bcount = scalar @{$_->{bonds}};
            $_->{key} . ($bcount > 0 ? "!$bcount" : "")
        } @{$mol->{comps}};
        push @fp_parts, "M:" . $mol->{name} . "(" . join(",", @comp_keys) . ")";
    }

    # Bond edges (captures connectivity)
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
