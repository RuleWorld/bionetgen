#!/usr/bin/perl 
# pragmas
package Viz;
use strict;
use warnings;
use File::Spec;
use warnings;						  
use FindBin;
use Getopt::Long;
use lib 	File::Spec->catdir(
				(	exists $ENV{'BNGPATH'} ? $ENV{'BNGPATH'} :
					( 	exists $ENV{'BioNetGenRoot'} ? $ENV{'BioNetGenRoot'} : 
						do 	{
							my ($vol,$dir,$file) = File::Spec->splitpath( $FindBin::RealBin );
							my @dirs = File::Spec->splitdir($dir);
							pop @dirs;
							pop @dirs;
							File::Spec->catpath($vol,File::Spec->catdir(@dirs))
						} 
					)
				),'Perl2'
			);

use BNGModel;
use Visualization::Viz;
use Visualization::VisOptParse;
use Cwd;
use Class::Struct;

# Get list of bngls
my $dir = getcwd;
opendir (DIR, $dir) or die $!;
my @bngls;
while(my $file = readdir(DIR))
	{ if ($file=~ /.bngl$/) {push @bngls, $file;} }
closedir DIR;
@bngls = ('Faeder2003.bngl');

foreach my $bngl(@bngls)
{
	$bngl =~ /^(.*).bngl$/;
	my $modelname = $1;
	my $model = getModel($bngl);
	initializeGraphsObject($model);
	my $gr = $model->VizGraphs;
	
	# 1
	getRuleNames($model);
	my @rules = flat @{$gr->{'RuleNames'}};
	my @reverses = grep { $_ =~ /^_reverse.*$/} @rules;
	
	# 2
	getRulePatternGraphs($model);
	my @rpgs = flat @{$gr->{'RulePatternGraphs'}};
	my %lstathash; my %rstathash; # hash in which values are stathashes
	# left and right merged pattern structure graphs
	foreach my $rpg(@rpgs)
	{
		my ($lstats,$rstats) =  getPSGstats($rpg);
		my $name = $lstats->{'RuleName'};
		$lstathash{$name} = $lstats;
		$rstathash{$name} = $rstats;
	}
	
	# example of how to access data
	# print $rstathash{'R19'}->{'UnboundComponents'};
	
}

# Individual rules
# Model-Name 
# Rule-Name
# LMPSG-Nodes - left merged pattern structure graph
# RMPSG-Nodes - right merged pattern structure graph
# RSG-Nodes - rule structure graph
# RRG-Nodes - rule regulatory graph

sub newStatsHash
{
	return (
	'RuleName' => $_[0],
	'Side' => $_[1],
	'Patterns' => $_[2],
	'Molecules' => $_[3],
	'Components' => $_[4],
	'BoundComponents' => $_[5],
	'UnboundComponents' => $_[6],
	'Bonds' => $_[7],
	'WildcardBonds' => $_[8],
	'InternalStates' => $_[9],
	);
};

sub initializeGraphsObject
{
	my $model = shift @_;
	my $gr = Graphs->new();
	$model->VizGraphs($gr);
	$gr->{'NewName'} = -1;
	return;
}


sub getPSGstats
{
	my $rpsg = shift @_;
	my @nodelist = @{$rpsg->{'NodeList'}};
	my @ids = map { $_->{'ID'}; } @nodelist;
	# node ids are like this
	# RuleNum.[Re=0/Pr=1].PattNum.PattNum.MolNum.CompNum.[IntState=0,BondState=1]
	# No idea why PattNum is repeated. Clean this up later.
	my %lstat = newStatsHash(); $lstat{'Side'} = 'Reactant';
	my %rstat = newStatsHash(); $rstat{'Side'} = 'Product';
	
	# partition left nodes versus right nodes versus rule node
	my @rule_arr = grep { /^\d+$/ } @ids; # should be array of size 1
	
	my @left = grep { /^\d+[.]0/} @ids;
	my @right = grep { /^\d+[.]1/} @ids;	
	
	my $rule_id = $rule_arr[0];
	my $rulename = join "", map {$_->{'Name'}} grep {$_->{'ID'} eq $rule_id; } @nodelist;	
	my $leftpatts = scalar grep { /^\d+[.]0[.]\d+$/} @left;
	my $rightpatts = scalar grep { /^\d+[.]1[.]\d+$/} @right;
	my $leftmols = scalar grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+$/} @left;
	my $rightmols = scalar grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+$/} @right;
	my $leftcomps = scalar grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]\d+$/} @left;
	my $rightcomps = scalar grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]\d+$/} @right;
	my $leftints = scalar grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]0$/} @left;
	my $rightints = scalar grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]0$/} @right;
	
	my @lbonds =  grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]1$/} @left;
	my @rbonds =  grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]1$/} @right;
	my @lbondnodes = grep { has(\@lbonds,$_->{'ID'})==1 } @nodelist;
	my @rbondnodes = grep { has(\@rbonds,$_->{'ID'})==1 } @nodelist;
	my $leftwcs = scalar grep { scalar(@{$_->{'Parents'}}) == 1 } @lbondnodes;
	my $rightwcs = scalar grep { scalar(@{$_->{'Parents'}}) == 1 } @rbondnodes;
	my $leftbonds = scalar grep { scalar(@{$_->{'Parents'}}) == 2 } @lbondnodes;
	my $rightbonds = scalar grep { scalar(@{$_->{'Parents'}}) == 2 } @rbondnodes;
	my @lcomps = grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]\d+$/} @left;
	my @rcomps = grep { /^\d+[.]\d+[.]\d+[.]\d+[.]\d+[.]\d+$/} @right;
	my $lboundcomps = scalar uniq map { @{$_->{'Parents'}} } @lbondnodes;
	my $rboundcomps = scalar uniq map { @{$_->{'Parents'}} } @rbondnodes;
	my $l_unboundcomps = $leftcomps - $lboundcomps;
	my $r_unboundcomps = $leftcomps - $rboundcomps;
	
	my @arr = ($rulename,scalar(@rule_arr),scalar(@left),scalar(@right),$leftpatts,$rightpatts,$leftmols,$rightmols,$leftcomps,$rightcomps,$leftints,$rightints,$leftbonds,$rightbonds,$leftwcs,$rightwcs,$leftbonds,$rightbonds,$lboundcomps,$rboundcomps,$l_unboundcomps,$r_unboundcomps);
	
	my %lstats = newStatsHash($rulename,'Reactant',$leftpatts,$leftmols,$leftcomps,$lboundcomps,$l_unboundcomps,$leftbonds,$leftwcs,$leftints);
	my %rstats = newStatsHash($rulename,'Product',$rightpatts,$rightmols,$rightcomps,$rboundcomps,$r_unboundcomps,$rightbonds,$rightwcs,$rightints);
	
	return (\%lstats,\%rstats);
}

sub getRSGNodes
{

}

sub getRRGNodes
{

}


# Models
# Model-Name
# Rules - number of rules
# CRV-Nodes - conventional rule visualization
# CRV-Edges
# CMV-Nodes - compact rule visualization
# CMV-Edges
# SNV-Nodes - Simmune Network Viewer
# SNV-Edges
# MRG-Nodes - model regulatory graph 
# MRG-Edges
# MRGC-Nodes - model regulatory graph with pruning, grouping and collapsing
# MRGC-Edges
# RINF-Nodes - rule influence diagram
# RINF-Edges
# RXN-Nodes - reaction network
# RXN-Edges





#filecheck($args{'bngl'},'BNGL');
#map { filecheck($_,'Opts') } @{$args{'opts'}} if defined $args{'opts'};
#my $model = getModel($args{'bngl'});


#my $exec_params = getExecParams(\%args);
#execute_params($model,$exec_params);

sub filecheck
{
	my $file = shift @_;
	my $type = shift @_;
	if(not defined $file) { print $type." file not defined.\n"; exit;}
	if(not -e $file) { print $type." file ".$file." not found.\n"; exit;}
	print "Found ".$type." file ".$file.".\n";
	return;
}

sub getModel
{
	my $filename = shift @_;
	print "Importing model from ".$filename."\n";
	my %args = ();
	$args{'file'} = $filename;
	$args{'skip_actions'} = 1;
	$args{'action_skip_warn'} = 1;

	#print $filename;
	my $model = BNGModel->new();
	$model->initialize();
	$BNGModel::GLOBAL_MODEL = $model;
	
	#print "Opening file: ".$filename."\n";
	my $err = $model->readModel(\%args) ;
	
	if ($err) { print "ERROR:".$err."\n";}
	
	return $model;
}

1;
