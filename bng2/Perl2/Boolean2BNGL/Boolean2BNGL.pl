#!/usr/bin/perl

use strict;
use warnings;

my @NODES = ();

my @parameters = ();
my @molecule_types = ();
my @seed_species = ();
my @observables = ();
my @functions = ();
my @reaction_rules = ();

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
my $n_rules = 0;
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
		
		$n_rules++;
		my $rule_f = "R" . $n_rules . ": $node(state";
		if ($include_reset){ $rule_f .= ",reset~N"; }
		$rule_f .= ") -> $node(state~1";
		if ($include_reset){ $rule_f .= ",reset~Y"; }
		$rule_f .= ") if($fname>0.5,$rate,0)";
		push @reaction_rules, $rule_f;
		
		$n_rules++;
		my $rule_r = "R" . $n_rules . ": $node(state"; 
		if ($include_reset){ $rule_r .= ",reset~N"; }
		$rule_r .= ") -> $node(state~0";
		if ($include_reset){ $rule_r .= ",reset~Y"; }
		$rule_r .= ") if($fname<0.5,$rate,0)";			
		push @reaction_rules, $rule_r;
    }
}

if ($update_mode eq 'GA'){
	push @reaction_rules, "\n   # reset rules";
	foreach my $node (@NODES){
		$n_rules++;
		my $rule = "R" . $n_rules . ": $node(reset~Y) -> $node(reset~N) 1e10";
		push @reaction_rules, $rule;
	}
}
elsif ($update_mode eq 'ROA'){
	push @parameters, "N_NODES " . scalar(@NODES);
	unshift @molecule_types, "RESET(reset~N~Y)\n   #";
	unshift @seed_species, "RESET(reset~N) 1\n   #";
	my $obs = "Molecules N_FIRED";
	foreach my $node (@NODES){
		$obs .= " $node(reset~Y)"
	}
	unshift @observables, $obs . "\n   #";
	push @reaction_rules, "\n   # reset rules";
	$n_rules++;
	push @reaction_rules, "R" . $n_rules . ": RESET(reset~N) <-> RESET(reset~Y)  1e10*if(N_FIRED>(N_NODES-0.5),1,0), 1e10*if(N_FIRED<0.5,1,0)";
	foreach my $node (@NODES){
		$n_rules++;
		my $rule = "R" . $n_rules . ": $node(reset~Y) + RESET(reset~Y) -> $node(reset~N) + RESET(reset~Y) 1e10";
		push @reaction_rules, $rule;
	}
}

print "begin model\n";

print "begin parameters\n";
foreach my $i (@parameters){
	print "   $i\n";
}
print "end parameters\n";

print "begin molecule types\n";
foreach my $i (@molecule_types){
	print "   $i\n";
}
print "end molecule types\n";

print "begin seed species\n";
foreach my $i (@seed_species){
	print "   $i\n";
}
print "end seed species\n";

print "begin observables\n";
foreach my $i (@observables){
	print "   $i\n";
}
print "end observables\n";

print "begin functions\n";
foreach my $i (@functions){
	print "   $i\n";
}
print "end functions\n";

print "begin reaction rules\n";
foreach my $i (@reaction_rules){
	print "   $i\n";
}
print "end reaction rules\n";

print "end model\n";

# actions
print "\n# generate_network({overwrite=>1})\n\n";

my $n_nodes = scalar(@NODES);
my $n_rounds = 15;

if ($update_mode eq 'ROA'){
	print "# Simulate for N_ROUNDS = 15 update rounds, outputting every round. Note that with N_NODES = $n_nodes, the\n";
	print "# average time for each update round is 1 + 1/2 + 1/3 + ... + 1/$n_nodes < $n_nodes. Thus, 't_end' is set to\n";
	print "# $n_nodes*$n_rounds*100 (N_NODES*N_ROUNDS*100), a number large enough to ensure that 'max_sim_steps' is reached first.\n";
	print "#\n";
	print "# simulate({method=>\"ssa\",t_end=>($n_nodes*$n_rounds*100),output_step_interval=>($n_nodes*2+2),max_sim_steps=>(($n_nodes*2+2)*$n_rounds),print_CDAT=>0,verbose=>1})\n";
}
elsif ($update_mode eq 'GA'){
	print "# Simulate for 30 time units, outputting every time unit.\n";
	print "#\n";
	print "# simulate({method=>\"ssa\",t_end=>30,n_steps=>30,print_CDAT=>0,verbose=>1})\n\n";
	
	print "# Simulate for 30 \"update rounds\", outputting every round. This allows for a comparison with the ROA update scheme.\n";
	print "# With N_NODES = $n_nodes, each round corresponds to $n_nodes*2 reaction firings (one state-change and one reset rule per node).\n";
	print "# On average, this takes one time unit. Thus, 't_end' is set to 30*100 to ensure that 'max_sim_steps' is reached first.\n";
	print "#\n";
	print "# simulate({method=>\"ssa\",t_end=>(30*100),output_step_interval=>($n_nodes*2),max_sim_steps=>(($n_nodes*2)*30),print_CDAT=>0,verbose=>1})\n";
}
elsif ($update_mode eq 'GSP'){
	print "# Simulate for 30 time units, outputting every time unit.\n";
	print "# simulate({method=>\"ssa\",t_end=>30,n_steps=>30,print_CDAT=>0,verbose=>1})\n";
}
