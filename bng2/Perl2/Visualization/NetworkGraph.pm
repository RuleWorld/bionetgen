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

struct NetworkGraph => 
{ 
	'NodeList' => '@', # array of strings
	'EdgeList' => '@', # array of strings
	'NodeType' => '%', # a hash indicating each node type
	'Name' => '$', # a name which might come in handy to compare/combine rules
	# of the form <transformationstring>:<atomicpatternstring>:<edgetype>
	# or <wildcardpattern>:<atomicpatternstring>:Wildcard
	'NodeGroups' => '@' # array of arrayrefs, each being a ref to a group of nodes	
};
# is methods for checking
sub isWildcard{ return ($_[0] =~ /\!\+/) ? 1 : 0; }

# basic make methods
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

sub makeEdge
{
	my %shortname = ( 'r'=>"Reactant", 'p'=>"Product", 'c'=>"Context", 's'=>"Syndel", 'w'=>"Wildcard", 'pp'=>"ProcessPair", 'co'=>"Cotransform", 'os'=>"Onsite" );
	
	my $node1 = shift @_;
	my $node2 = shift @_;
	my $rel = $shortname{shift @_};
	
	my $string = $node1.":".$node2.":".$rel;
	return $string;
	
}

# print for sanity check
sub printNetworkGraph
{
	my $bpg = shift @_;
	print "Nodes:\n";
	print map $_."\n", @{$bpg->{'NodeList'}};
	print "Edges:\n";
	print map $_."\n", @{$bpg->{'EdgeList'}};
	print "\n";
	return;
}
# text cleaning for atomic patterns and transformations
sub prettify
{
	my $string = shift @_;
	my $arrow = '->';
	#print ($string, $string =~ /$arrow/, "\n");
	# check if it is a transformation
	if ($string =~ /$arrow/)
	{
		# see if arrow has spaces already
		if ($string =~ /\b$arrow\b/) { return $string;}
		else  
		{
			my @splits = split $arrow,$string;
			if (scalar @splits == 1) { push @splits,"0"; }
			elsif (length $splits[0] == 0) { $splits[0]="0";}
			return join(" -> ",map prettify($_), @splits);
		}
	}
	my $comma = ',';
	if ($string =~ /$comma/) 
	{
		if ($string =~ /\b$comma\b/) { return $string; }
		else 
		{ 
			my @splits = split $comma,$string;
			return join(" , ", @splits); 
		}
	} 
	if ($string =~ /$0^/) { return $string; } 
	#if ($string =~ /\(/)  { return $string; }
	#else { return $string."\(\)"; }
	return $string;
}

sub unprettify
{
	my $string = shift @_;
	$string =~ s/\s//g;
	$string =~ s/\(\)//g;
	$string =~ s/^0//g;
	$string =~ s/0$//g;
	return $string;
}

sub combine3
{
	my @bpgs = @{shift @_};
	my @nodelist = ();
	my @edgelist = ();
	my %nodetype;
	foreach my $bpg(@bpgs)
	{
		push @nodelist, @{$bpg->{'NodeList'}};
		push @edgelist, @{$bpg->{'EdgeList'}};
		foreach my $node( keys %{$bpg->{'NodeType'}} )
		{
			$nodetype{$node} = $bpg->{'NodeType'}->{$node};
		}
		
	}
	my $bpg = NetworkGraph->new();
	$bpg->{'NodeList'} = [uniq(@nodelist)];
	$bpg->{'EdgeList'} = [uniq(@edgelist)];
	$bpg->{'NodeType'} = \%nodetype;
	return $bpg;
}
sub addWildcards
{
	my $bpg = shift @_;
	my @nodelist = @{$bpg->{'NodeList'}};
	my %nodetype = %{$bpg->{'NodeType'}};
	
	my @ap = grep {$nodetype{$_} eq 'AtomicPattern'} @nodelist;
	my @wildcards = grep (isWildcard($_), @ap);
	my @notwildcards = grep (!isWildcard($_), @ap);

	foreach my $wc(@wildcards)
		{
		my @splits = split '\+', $wc;
		my $string = $splits[0];
		
		my @matches = grep(index($_, $string) != -1, @notwildcards);
		foreach my $match(@matches)
			{
			my $edge = makeEdge($wc,$match,'w');
			push @{$bpg->{'EdgeList'}},$edge;
			}
		}
	return;
}

# get methods
sub getReactantsProducts
{
	my $string = unprettify(shift @_);
	my @splits = split '->',$string;
	my @reac = ();
	my @prod = ();
	if (scalar @splits == 1) { @reac = ($splits[0]); }
	elsif (length $splits[0] == 0) { @prod = ($splits[1]); }
	else { @reac = split ',',$splits[0]; @prod = split ',',$splits[1]; }
	return (\@reac,\@prod);
}

sub getStructures
{
	my @nodelist = @{shift @_};
	my %structures = ('Mol'=>1,'Comp'=>1,'CompState'=>1,'BondState'=>1,'GraphOp'=>0,'Rule'=>0);
	my @nodes = grep( $structures{$_->{'Type'}}==1, @nodelist);
	return @nodes;
}
sub getContext
{
	my @nodelist = @{shift @_};
	my @exclude = ();
	if (@_) { @exclude = @{shift @_} };
	my @exclude_ids = ();
	foreach my $exc (@exclude)
	{
		my @x = @$exc;
		my $y = shift @x;
		push @exclude_ids, map $_->{'ID'}, @x;
	}
	#print scalar @exclude_ids;
	my @nodes_struct = getStructures(\@nodelist);
	my @nodes = hasSide(\@nodes_struct,'both');
	my @context = ();
	
	# comp states
	my @compstates = 	grep has(\@exclude_ids,$_->{'ID'})==0,
						hasType(\@nodes,'CompState');
	if (@compstates)
	{
		foreach my $node(@compstates)
		{
			my $string = makeAtomicPattern(\@nodes_struct,$node);
			if ($string) { push @context,$string;}
		}
	}
	
	# bond states
	my @bondstates = 	grep has(\@exclude_ids,$_->{'ID'})==0,
						hasType(\@nodes,'BondState');
	foreach my $node(@bondstates)
	{
		my $string = makeAtomicPattern(\@nodes_struct,$node);
		if ($string) { push @context,$string;}
	}
	
	# unbound states
	my @comps =	grep has(\@exclude_ids,$_->{'ID'})==0,
				hasType(\@nodes,'Comp');
	my %unbound;
	foreach my $x(@comps) { $unbound{$x->{'ID'}}=1; }
	my @allbonds = hasType(\@nodelist,'BondState');
	my @allbondparents;
	foreach my $node(@allbonds) { push @allbondparents, @{$node->{'Parents'}}; }
	foreach my $x(@allbondparents) { $unbound{$x}=0; }
	foreach my $x(keys %unbound) 
	{ 
		if ($unbound{$x}) 
		{
			my $node = findNode(\@comps,$x);
			push @context,makeAtomicPattern(\@nodes_struct,$node);
		}
	}
	
	# mol nodes that do not have any components (hence identified by only label)
	my @mols = hasType(\@nodes,'Mol');
	my %havenocomps;
	foreach my $x(@mols) { $havenocomps{$x->{'ID'}}=1; }
	my @allcompparents;
	foreach my $node(hasType(\@nodes,'Comp')) 
		{ push @allcompparents, @{$node->{'Parents'}}; }
	foreach my $x(@allcompparents) { $havenocomps{$x}=0; }
	foreach my $x(keys %havenocomps) 
	{ 
		if ($havenocomps{$x}) 
		{
			my $node = findNode(\@mols,$x);
			push @context,makeAtomicPattern(\@nodes_struct,$node);
		}
	}
	
	return @context;
}

sub getSyndelContext
{
	my @nodelist = @{shift @_};
	my $op = shift @_;
	
	my $mol = findNode(\@nodelist,${$op->{'Parents'}}[0]);
	
	# get child components
	my @allcomps = hasType(\@nodelist,'Comp');
	my @comps = grep (${$_->{'Parents'}}[0] eq $mol->{'ID'}, @allcomps);
	my @comps_ids = map $_->{'ID'}, @comps;
	
	# get child component states
	my @allcompstates = hasType(\@nodelist,'CompState');
	my @compstates = ();
	foreach my $x(@allcompstates)
	{
		foreach my $y (@comps_ids)
			{
				if (${$x->{'Parents'}}[0] eq $y) { push @compstates,$x; }
			}
	}
	
	# get child bond states
	my %unbound;
	foreach my $y (@comps_ids) { $unbound{$y} = 1; }
			
	my @allbondstates = hasType(\@nodelist,'BondState');
	my @bondstates = ();
	foreach my $x(@allbondstates)
	{	
		my @parents = @{$x->{'Parents'}};
		foreach my $y (@comps_ids)
		{
			foreach my $z(@parents)
			{
			if ($y eq $z) 
				{
				push @bondstates,$x;
				$unbound{$z} = 0;
				}
			}
		}
	}
	
	my @unboundcomps = ();
	foreach my $x(keys %unbound)
	{
	if ($unbound{$x}) 
		{
		my $node = findNode(\@nodelist,$x);
		push @unboundcomps, $node;
		}
	}
	
	my @syndelnodes = (@compstates,@bondstates,@unboundcomps);
	my @syndel = ();
	foreach my $node(@syndelnodes) { push @syndel, makeAtomicPattern(\@nodelist,$node); }
	
	return @syndel;
}


# make graph methods
sub makeRuleNetworkGraph
{
	# from a rule structure graph
	my $rsg = shift @_;
	my $name = shift @_;
	
	my @nodelist = @{$rsg->{'NodeList'}};
	
	my $bpg = NetworkGraph->new();
	$bpg->{'Name'} = $name;
	
	my @graphop = hasType(\@nodelist,'GraphOp');
	my @contexts = getContext(\@nodelist);
	
	# add node for rule
	push @{$bpg->{'NodeList'}}, $name;
	$bpg->{'NodeType'}->{$name} = 'Rule';
	
	# add reactant and product edges
	foreach my $op(@graphop)
	{
		my $tr = makeTransformation(\@nodelist,$op);
		# this is because we are currently not treating wildcard bond deletions
		if(length $tr == 0) { next; }
		my ($reac,$prod) = getReactantsProducts($tr);
		push @{$bpg->{'NodeList'}}, @$reac, @$prod;
		foreach my $reactant (@$reac)
		{
			if (length $reactant == 0) {next;}
			my $edge = makeEdge($name,$reactant,'r');
			push @{$bpg->{'NodeList'}}, $reactant;
			push @{$bpg->{'EdgeList'}}, $edge;
			$bpg->{'NodeType'}->{$reactant} = 'AtomicPattern';
			
		}
		foreach my $product (@$prod)
		{
			if (length $product == 0) {next;}
			my $edge = makeEdge($name,$product,'p');
			push @{$bpg->{'NodeList'}}, $product;
			push @{$bpg->{'EdgeList'}}, $edge;
			$bpg->{'NodeType'}->{$product} = 'AtomicPattern';
		}
	}
	# add context edges
	foreach my $context(@contexts)
	{
		if (length $context == 0) {next;}
		my $edge = makeEdge($name,$context,'c');
		push @{$bpg->{'NodeList'}}, $context;
		push @{$bpg->{'EdgeList'}}, $edge;
		$bpg->{'NodeType'}->{$context} = 'AtomicPattern';
	}
	# add syndel edges
	foreach my $op(@graphop)
	{
		if ($op->{'Name'} =~ /Mol/)
		{
			my $rel = ($op->{'Name'} =~ /Add/) ? 'p' : 'r';
			my @syndels = getSyndelContext(\@nodelist,$op);
			foreach my $syndel(@syndels)
			{
				if (length $syndel == 0) {next;}
				my $edge = makeEdge($name,$syndel,$rel);
				push @{$bpg->{'NodeList'}}, $syndel;
				push @{$bpg->{'EdgeList'}}, $edge;
				$bpg->{'NodeType'}->{$syndel} = 'AtomicPattern';
			}
		}
	}
	
	uniqNetworkGraph($bpg);
	addWildcards($bpg);
	uniqNetworkGraph($bpg);
	return $bpg;
	
}

sub uniqNetworkGraph
{
	my $bpg = shift(@_);
	$bpg->{'NodeList'} = [uniq(@{$bpg->{'NodeList'}})];
	$bpg->{'EdgeList'} = [uniq(@{$bpg->{'EdgeList'}})];
	return;
}

sub ruleGroups
{


}



1;
