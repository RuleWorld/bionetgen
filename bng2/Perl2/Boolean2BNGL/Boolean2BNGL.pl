#!/usr/bin/perl

use strict;
use warnings;

my @NODES = ();

my @molecule_types = ();
my @seed_species = ();
my @observables = ();
my @functions = ();
my @reaction_rules = ();

my $file = shift @ARGV;

#my $mode = shift @ARGV; # synch, roa, ga, gsp
#if ( $mode eq 'synch' || $mode eq 'roa' || $mode eq 'ga' || $mode eq 'gsp' ){
#	printf "Update mode '$mode'.\n";
#}
#else{
#	printf "Update mode '$mode' not recognized. Allowed modes are 'syn', 'roa', 'ga', 'gsp'.\n";
#}
#exit 1;

my $include_reset = (@ARGV) ? shift @ARGV : 0;

# read entire file as a string
local $/ = undef;
open FILE, $file or die "Couldn't open file: $!";
binmode FILE;
my $text = <FILE>;
close FILE;
$text =~ s/\r\n*/\n/g; # remove Windows-style carriage returns

my @lines = split('\n', $text);
foreach my $line (@lines) { # read line-by-line
	$line =~ tr/A-Z/a-z/; # Make everything lower case
    if ( $line =~ /^\s*(\w+)\s*=\s*([t|f])/ )
    {
		# MOLECULE TYPES
	    	my $mt = $1;
	    	$mt .= "(x~0~1";
	    	if ($include_reset){ $mt .= ",u~N~Y" }
	    	$mt .= ")";
	    	push @molecule_types, $mt;
	    	
	    	# SEED SPECIES
	    	my $ss = $1;
	    	my $state = ($2 eq 't') ? '1' : '0';
	    	$ss .= "(x~$state";
	    	if ($include_reset){ $ss .= ",u~N"; }
	    	$ss .= ") 1";
	    	push @seed_species, $ss;
	    	
	    	# OBSERVABLES
	    	my $obs = $1;
	    	$obs =~ tr/a-z/A-Z/; # Make observable names all upper case
	    	$obs = "Molecules " . $obs . " " . $1 . "(x~1)";
	    	push @observables, $obs;
    }
    elsif( $line =~ /^\s*(\d+):\s*(\w+)\*\s*=\s*(.+)/ )
    {
	    	my $rank = $1;
	    	my $node = $2;
	    	my $func = $3;
	    	
	    	push @NODES, $node;
	    	
	    	# FUNCTIONS
	    	my $fname = "${node}_func()";
	    	
	    	$func =~ tr/a-z/A-Z/;          # make everything upper case
	    	$func =~ s/\(\s+/\(/g;         # remove whitespace after '(' character
	    	$func =~ s/\s+\)/\)/g;         # remove whitespace before ')' character
	    	$func =~ s/\s+or\s+/ \|\| /ig;   # replace 'or' with '||'
	    	$func =~ s/\s+and\s+/ \&\& /ig;  # replace 'and' with '&&'
	    	$func =~ s/(\s*)not\s+/$1!/ig; # replace 'not' with '!' (also remove trailing whitespace)
	    	$func =~ s/(\w+)/$1>0.5/g;       # replace all 'ABC' with 'ABC>0.5'
	    	$func =~ s/!(\w+)>0.5/$1<0.5/g;    # replace all '!ABC>0.5' with 'ABC<0.5'
	    	
	    	$func = "$fname if($func, 1, 0)";
		push @functions, $func;

		# RULES
		my $rate = "1";
		if ($rank > 1){ $rate = "1/$rank"; }
		my $rule_f = "R" . (scalar(@reaction_rules)+1) . ": $node(x";
		if ($include_reset){ $rule_f .= ",u~N"; }
		$rule_f .= ") -> $node(x~1";
		if ($include_reset){ $rule_f .= ",u~Y"; }
		$rule_f .= ") if($fname>0.5,$rate,0)";
		push @reaction_rules, $rule_f;
		my $rule_r = "R" . (scalar(@reaction_rules)+1) . ": $node(x"; 
		if ($include_reset){ $rule_r .= ",u~N"; }
		$rule_r .= ") -> $node(x~0";
		if ($include_reset){ $rule_r .= ",u~Y"; }
		$rule_r .= ") if($fname<0.5,$rate,0)";			
		push @reaction_rules, $rule_r;
    }
    else{
    	# Error message
    }
}

if ($include_reset){
	# Update rule
	my $first = shift @NODES;
	my $rule = "R0: " . $first . "(u~Y)";
	foreach my $i (@NODES){ $rule .= " + " . $i . "(u~Y)" }
	$rule .= " -> " . $first . "(u~N)";
	foreach my $i (@NODES){ $rule .= " + " . $i . "(u~N)" }
	$rule .= " 1e10";
	unshift @reaction_rules, $rule;
}

printf "begin molecule types\n";
foreach my $i (@molecule_types){
	printf "   %s\n", $i;
}
printf "end molecule types\n";

printf "begin seed species\n";
foreach my $i (@seed_species){
	printf "   %s\n", $i;
}
printf "end seed species\n";

printf "begin observables\n";
foreach my $i (@observables){
	printf "   %s\n", $i;
}
printf "end observables\n";

printf "begin functions\n";
foreach my $i (@functions){
	printf "   %s\n", $i;
}
printf "end functions\n";

printf "begin reaction rules\n";
foreach my $i (@reaction_rules){
	printf "   %s\n", $i;
}
printf "end reaction rules\n"
