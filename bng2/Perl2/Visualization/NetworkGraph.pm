package Viz;
# pragmas
use strict;
use warnings;
no warnings 'redefine';
# Perl Modules
use Class::Struct;
# BNG Modules
use Viz;
use StructureGraph;

sub makeAtomicPattern
{
	my @nodelist = @{shift @_};
	my $node = shift @_;
	
	my $type = $node->{'Type'};
	my $ap;
	if ($type eq 'CompState')
		{
		my $comp = findNode(\@nodelist,${$node->{'Parents'}}[0]);
		my $mol = findNode(\@nodelist,${$comp->{'Parents'}}[0]);
		my $string = $mol->{'Name'}."(".$comp->{'Name'}."~".$node->{'Name'}.")";
		$ap = ($node->{'Name'} ne '?') ?  $string : "";
		}
	elsif ($type eq 'BondState')
		{
		my @comps = map (findNode(\@nodelist,$_), @{$node->{'Parents'}}) ;
		my @mols = map (findNode(\@nodelist,${$_->{'Parents'}}[0]), @comps) ;
		if (scalar @comps == 1)
			{
			# it's a wildcard
			my $string = $mols[0]->{'Name'}."(".$comps[0]->{'Name'}."!".$node->{'Name'}.")";
			$ap = ($node->{'Name'} ne '?') ?  $string : "";
			}
		else
			{
			# it's a specified bond
			my $string1 = $mols[0]->{'Name'}."(".$comps[0]->{'Name'}."!1)";
			my $string2 = $mols[1]->{'Name'}."(".$comps[1]->{'Name'}."!1)";
			$ap = join(".", sort {$a cmp $b} ($string1,$string2));
			}
		}
	elsif ($type eq 'Comp')
		{
		# return the unbound state
		# is it really unbound? check it external to this method
		my $mol = findNode(\@nodelist,${$node->{'Parents'}}[0]);
		my $string = $mol->{'Name'}."(".$node->{'Name'}.")";
		$ap = $string;
		}
	elsif ($type eq 'Mol')
		{
		$ap = $node->{'Name'};
		}
	return $ap;
}

sub makeAtomicPatterns 
{ 
	my $nodelist = shift @_;
	my $nodes = shift @_;
	my @aps = map { makeAtomicPattern($nodelist,$_)} @$nodes; 
	return @aps;
}

sub makeTransformation
{
	my @nodelist = @{shift @_};
	my $node = shift @_;
	my $type = $node->{'Type'};
	my $name = $node->{'Name'};
	my $arrow = "->";
	my $comma = ",";
	my $tr;
	if ($type ne 'GraphOp') { return undef; }
	if ($name eq 'ChangeState')
		{
		my @comps = map (findNode(\@nodelist,$_), @{$node->{'Parents'}});
		my @left = grep( $_->{'Side'} eq 'left', @comps) ;
		my @right = grep( $_->{'Side'} eq 'right', @comps) ;
		my $leftstr = makeAtomicPattern(\@nodelist,$left[0]);
		my $rightstr = makeAtomicPattern(\@nodelist,$right[0]);
		$tr = $leftstr.$arrow.$rightstr;
		}
	elsif ($name eq 'AddBond')
		{
		my $bond = findNode(\@nodelist,${$node->{'Parents'}}[0]);
		my @comps = map (findNode(\@nodelist,$_), @{$bond->{'Parents'}});
		my @leftstr = sort map ( makeAtomicPattern(\@nodelist,$_),@comps);
		my $rightstr = makeAtomicPattern(\@nodelist,$bond);
		$tr = join($comma,@leftstr).$arrow.$rightstr;
		}
	elsif ($name eq 'DeleteBond')
		{
		my $bond = findNode(\@nodelist,${$node->{'Parents'}}[0]);
		# bond wildcards are also being deleted when molecules are deleted
		# how do you transform them into processes?
		# need to talk to bngdev
		my @comps = map (findNode(\@nodelist,$_), @{$bond->{'Parents'}});
		if (scalar @comps == 1) { return ""; }
		my @rightstr = sort map ( makeAtomicPattern(\@nodelist,$_),@comps);
		my $leftstr = makeAtomicPattern(\@nodelist,$bond);
		$tr = $leftstr.$arrow.join($comma,@rightstr);
		}
	elsif ($name eq 'AddMol')
		{
		my $mol = findNode(\@nodelist,${$node->{'Parents'}}[0]);
		my $name = makeAtomicPattern(\@nodelist,$mol);
		$tr = $arrow.$name;
		}
	elsif ($name eq 'DeleteMol')
		{
		# species deletion is interpreted as molecule deletion
		# how to check? what to do?
		my $mol = findNode(\@nodelist,${$node->{'Parents'}}[0]);
		my $name = makeAtomicPattern(\@nodelist,$mol);
		$tr = $name.$arrow;
		}
	return $tr;
}
