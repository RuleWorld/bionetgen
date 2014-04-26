# StructureGraph.pm, used for creating visualizations of patterns
# Author: John Sekar (johnarul.sekar@gmail.com)

package StructureGraph;
# pragmas
use strict;
use warnings;
no warnings 'redefine';

# Perl Modules
use Class::Struct;
use List::Util qw(min max sum);

# BNG Modules
use SpeciesGraph;

# Notes for self
# Hash keys and values must be scalars!
# Always pass the array reference when passing an array as an argument.. e.g. mysub($aa, \@ba, $ca);
# Inside the subroutine, "get" that reference as an element of @_ and assign it inside structs 
# e.g. if struct A => {'Ba' => '@'} and if $a is a struct of type A, then assign $a->{'Ba'} = shift @_;
# where @_ contains a reference to some array @ba
# When needed to access the elements, dereference the array using @{$a->{'Ba'}}


struct Node => 
{ 
	'Type' => '$', #must be one of 'Mol', 'Comp', 'BondState', 'CompState'
	'Name' => '$', 
	'ID' => '@', 
	'Parents' => '@', 
	'Side' => '$', #must be one of 'left', 'right', 'both'
}; 


struct StructureGraph => { 'Type' => '$', 'NodeList' => '@' };

sub makeNode
{
	# input in this order Type, Name, ID, Parents, Side
	# type and name are compulsory, rest are not
	my $node = Node->new();
	$node->{'Type'} = shift @_;
	$node->{'Name'} = shift @_;
	$node->{'ID'} = @_ ? shift @_ : ();
	$node->{'Parents'} = @_ ? shift @_ : ();
	$node->{'Side'} = @_ ? shift @_ : ();
	return $node;
}

sub makeStructureGraph
{
	# input is Type, NodeList
	my $psg = StructureGraph->new();
	$psg->{'Type'} = $_[0];
	$psg->{'NodeList'} = $_[1];
	#print $_->{'Name'} for @{$psg->{'NodeList'}};
	return $psg;
}

sub printNode
{
	my $string = join(' ',@{$_->{'ID'}});
	$string .= ' ';
	$string .= $_->{'Name'};
	return $string;
}


sub printGraph 
{
	my $psg = shift @_;
	my @string = map {printNode()} @{$psg->{'NodeList'}};
	return join("\n",@string);
}


sub makePatternStructureGraph
{
	my $sg = shift @_;
	my $index = @_ ? shift @_ : 0; # this is the index of the pattern in some list of patterns
	
	my @nodelist = ();
	
	if ( $sg->isNull() != 0) { return undef;}
	
	# Nodes for molecules, assigned the ID p.m, where p is the index of the pattern
	# index must be passed to this method.
	my $imol = 0;
	foreach my $mol ( @{$sg->Molecules} )
	{
		my $mol_id = $index.".".$imol;
		my @idlist = ($mol_id);
		push @nodelist, makeNode('Mol',$mol->Name,\@idlist);
		
		# Nodes for components, assigned the ID p.m.c
		my $icomp = 0;
		foreach my $comp( @{$mol->Components} )
		{
			my $comp_id = $mol_id.".".$icomp;
			my @idlist = ($comp_id);
			my @parents = ($mol_id);
			push @nodelist, makeNode('Comp',$comp->Name,\@idlist,\@parents);
			
			# Nodes for internal states, assigned the ID p.m.c.0
			if (defined $comp->State) 
				{ 
					my $state_id = $comp_id.".0";
					my @idlist = ($state_id);
					my @parents = ($comp_id);
					push @nodelist, makeNode('CompState',$comp->State,\@idlist,\@parents);
				}
			
			# Nodes for each wildcard bond (either !? or !+), assigned the ID p.m.c.1
			if (scalar @{$comp->Edges} > 0) 
			{ 
				my $bond_state = ${$comp->Edges}[0];
				if ($bond_state =~ /^[\+\?]$/)
				{
					my $bond_id = $comp_id.".1";
					my @idlist = ($bond_id);
					my @parents = ($comp_id);
					push @nodelist, makeNode('BondState',$bond_state,\@idlist,\@parents);	
				}
			}
			$icomp++;
		}
		$imol++;
	}
	# Nodes for each specified bond, assigned the ID p.m1.c1.1, p.m2.c2.1 
	# where m1.c1 and m2.c2 are the bonded components
	# specified bonds are all assigned the same name "+"
	if (scalar @{$sg->Edges} > 0) 
		{
			foreach my $edge (@{$sg->Edges}) 
				{ 
				my @comps = split(/ /,$edge);
				my @idlist = map {$index.".".$_.".1"} @comps;
				my @parents = map {$index.".".$_} @comps;
				push @nodelist, makeNode('BondState',"+",\@idlist,\@parents);	
				}
		}

	my $psg = makeStructureGraph('Pattern',\@nodelist);
	#print $psg->printGraph();
	return $psg;	
}


1;
