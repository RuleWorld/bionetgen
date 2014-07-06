#!/usr/bin/perl

use strict;
use warnings;

my @NODES = ();

my @molecule_types = ();
my @seed_species = ();
my @observables = ();
my @functions = ();
my @reaction_rules = ();
my $n_rules = 0;

my $file = shift @ARGV;

my $update_mode = (@ARGV) ? shift @ARGV : 'GSP'; # ROA, GA, GSP
$update_mode =~ tr/a-z/A-Z/; # make upper case
if ( $update_mode eq 'ROA' || $update_mode eq 'GA' || $update_mode eq 'GSP' ){
	printf "Update mode: '$update_mode'\n";
}
else{
	printf "Update mode '$update_mode' not recognized. Allowed modes are 'ROA', 'GA', and 'GSP'.\n";
}

my $include_reset = ($update_mode eq 'GSP') ? 0 : 1;

# read entire file as a string
local $/ = undef;
open FILE, $file or die "Couldn't open file: $!";
binmode FILE;
my $text = <FILE>;
close FILE;
$text =~ s/\r\n*/\n/g; # remove Windows-style carriage returns
my @lines = split('\n', $text);

# read line-by-line
foreach my $line (@lines) 
{ 
	$line =~ tr/A-Z/a-z/; # Make everything lower case
    if ( $line =~ /^\s*(\w+)\s*=\s*([t|f])/ )
    {
		# MOLECULE TYPES
		my $mt = $1;
		$mt .= "(state~0~1";
		if ($include_reset){ $mt .= ",reset~N~Y" }
		$mt .= ")";
		push @molecule_types, $mt;
		
		# SEED SPECIES
		my $ss = $1;
		my $state = ($2 eq 't') ? '1' : '0';
		$ss .= "(state~$state";
		if ($include_reset){ $ss .= ",reset~N"; }
		$ss .= ") 1";
		push @seed_species, $ss;
		
		# OBSERVABLES
		my $obs = $1;
		$obs =~ tr/a-z/A-Z/; # Make observable names all upper case
		$obs = "Molecules " . $obs . " " . $1 . "(state~1)";
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
    	
    	$func =~ tr/a-z/A-Z/;            # make everything upper case
    	$func =~ s/\(\s+/\(/g;           # remove whitespace after '(' character
    	$func =~ s/\s+\)/\)/g;           # remove whitespace before ')' character
    	$func =~ s/\s+or\s+/ \|\| /ig;   # replace 'or' with '||'
    	$func =~ s/\s+and\s+/ \&\& /ig;  # replace 'and' with '&&'
    	$func =~ s/(\s*)not\s+/$1!/ig;   # replace 'not' with '!' (also remove trailing whitespace)
    	$func =~ s/(\w+)/$1>0.5/g;       # replace all 'ABC' with 'ABC>0.5'
    	$func =~ s/!(\w+)>0.5/$1<0.5/g;  # replace all '!ABC>0.5' with 'ABC<0.5'
    	
    	$func = "$fname if($func, 1, 0)";
    	push @functions, $func;

		# RULES
		my $rate = "1";
		if ($rank > 1){ $rate .= "/$rank"; }
		
		my $rule_f = "R" . ($n_rules+1) . ": $node(state";
		if ($include_reset){ $rule_f .= ",reset~N"; }
		$rule_f .= ") -> $node(state~1";
		if ($include_reset){ $rule_f .= ",reset~Y"; }
		$rule_f .= ") if($fname>0.5,$rate,0)";
		push @reaction_rules, $rule_f;
		$n_rules++;
		
		my $rule_r = "R" . ($n_rules+1) . ": $node(state"; 
		if ($include_reset){ $rule_r .= ",reset~N"; }
		$rule_r .= ") -> $node(state~0";
		if ($include_reset){ $rule_r .= ",reset~Y"; }
		$rule_r .= ") if($fname<0.5,$rate,0)";			
		push @reaction_rules, $rule_r;
		$n_rules++;
    }
}

if ($update_mode eq 'GA'){
	push @reaction_rules, "\n   # reset rules";
	foreach my $node (@NODES){
		my $rule = "R" . ($n_rules+1) . ": $node(reset~Y) -> $node(reset~N) 1e10";
		push @reaction_rules, $rule;
		$n_rules++;
	}
}
elsif ($update_mode eq 'ROA'){
	
}

#if ($include_reset){
#	# Update rule
#	my $first = shift @NODES;
#	my $rule = "R0: " . $first . "(reset~Y)";
#	foreach my $i (@NODES){ $rule .= " + " . $i . "(reset~Y)" }
#	$rule .= " -> " . $first . "(reset~N)";
#	foreach my $i (@NODES){ $rule .= " + " . $i . "(reset~N)" }
#	$rule .= " 1e10";
#	unshift @reaction_rules, $rule;
#}

printf "begin molecule types\n";
foreach my $i (@molecule_types){
	print "   $i\n";
}
printf "end molecule types\n";

printf "begin seed species\n";
foreach my $i (@seed_species){
	print "   $i\n";
}
printf "end seed species\n";

printf "begin observables\n";
foreach my $i (@observables){
	printf "   $i\n";
}
printf "end observables\n";

printf "begin functions\n";
foreach my $i (@functions){
	printf "   $i\n";
}
printf "end functions\n";

printf "begin reaction rules\n";
foreach my $i (@reaction_rules){
	printf "   $i\n";
}
printf "end reaction rules\n"
