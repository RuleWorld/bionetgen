
package BipartiteGraph;
# pragmas
use strict;
use warnings;
no warnings 'redefine';

# Perl Modules
use Class::Struct;
use List::Util qw(min max sum);
use List::MoreUtils qw( uniq);
use Data::Dumper;

# BNG modules
use StructureGraph;


struct BipartiteGraph => 
{ 
	'NodeList' => '@', # array of strings
	'EdgeList' => '@', # array of strings
	'Name' => '$', # a name which might come in handy to compare/combine rules
	# of the form <transformationstring>:<atomicpatternstring>:<edgetype>
	# or <wildcardpattern>:<atomicpatternstring>:Wildcard
};

sub isAtomicPattern { return ($_[0] =~ '->') ? 0 : 1; }
sub isTransformation { return ($_[0] =~ '->') ? 1 : 0; }
sub makeAtomicPattern { return StructureGraph::makeAtomicPattern(@_); }
sub makeTransformation { return StructureGraph::makeTransformation(@_); }
sub printGraph 
{
	my $bpg = shift @_;
	my @nodelist = @{$bpg->{'NodeList'}};
	my @transf = grep ( $_ =~ /->/, @nodelist);
	my @ap = ();
	foreach my $item(@nodelist)
		{
		if ( $item =~ m/->/) { next;}
		else { push @ap, $item; }
		}
	
	my $string = "";
	$string .= "\n\nAtomicPatterns:\n";
	$string .= join("", map prettify($_)."\n", @ap);
	$string .= "\n\nTransformations:\n";
	$string .= join("", map prettify($_)."\n", @transf);
	$string .= "\n\nEdges:\n";
	$string .= join("", map $_."\n",@{$bpg->{'EdgeList'}});
	return $string;
	
}

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
	my @nodes_struct = getStructures(\@nodelist);
	my @nodes = hasSide(\@nodes_struct,'both');
	my @context = ();
	
	# comp states
	my @compstates = hasType(\@nodes,'CompState');
	if (@compstates)
	{
		foreach my $node(@compstates)
		{
			my $string = makeAtomicPattern(\@nodes_struct,$node);
			if ($string) { push @context,$string;}
		}
	}
	
	# bond states
	my @bondstates = hasType(\@nodes,'BondState');
	foreach my $node(@bondstates)
	{
		my $string = makeAtomicPattern(\@nodes_struct,$node);
		if ($string) { push @context,$string;}
	}
	
	# unbound states
	my @comps = hasType(\@nodes,'Comp');
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
	foreach my $node(@comps) { push @allcompparents, @{$node->{'Parents'}}; }
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

sub makeEdge
{
	my %shortname = ( 'r'=>"Reactant", 'p'=>"Product", 'c'=>"Context", 's'=>"Syndel", 'w'=>"Wildcard" );
	
	my $node1 = shift @_;
	my $node2 = shift @_;
	my $rel = $shortname{shift @_};
	
	my $string = $node1.":".$node2.":".$rel;
	return $string;
	
}

sub findNode { return StructureGraph::findNode(@_); }
sub hasType { return StructureGraph::hasType(@_); }
sub hasSide { return StructureGraph::hasSide(@_); }
sub printHash {return StructureGraph::printHash(@_); }
sub findChildren
{
	my $nodelist = shift @_;
	my $node = shift @_;
	my $id = $node->{'ID'};
	return grep listHas($_->{'Parents'},$id)==1, @$nodelist;
}
sub findParents
{
	my $nodelist = shift @_;
	my $node = shift @_;
	return map findNode($nodelist,$_),@{$node->{'Parents'}};
}

sub combine
{
	my @bpgs = @{shift @_};
	my $bpgout = BipartiteGraph->new();
	
	foreach my $bpg(@bpgs)
		{
		push @{$bpgout->{'NodeList'}}, @{$bpg->{'NodeList'}};
		push @{$bpgout->{'EdgeList'}}, @{$bpg->{'EdgeList'}};
		}
	
	@{$bpgout->{'NodeList'}} = uniq @{$bpgout->{'NodeList'}};
	@{$bpgout->{'EdgeList'}} = uniq @{$bpgout->{'EdgeList'}};
	
	return $bpgout;

}

sub makeRuleBipartiteGraph
{
	# from a rule structure graph
	my $rsg = shift @_;
	my $name = $rsg->{'Name'};
	my @nodelist = @{$rsg->{'NodeList'}};
	
	my $bpg = BipartiteGraph->new();
	$bpg->{'Name'} = $name;
	
	my @graphop = hasType(\@nodelist,'GraphOp');
	my @transf;
	# isolate the common context
	my @contexts = getContext(\@nodelist);
	
	#add edges for reactant, product, common context
	foreach my $op(@graphop)
	{
		my $tr = makeTransformation(\@nodelist,$op);
		push @{$bpg->{'NodeList'}}, $tr;
		push @transf, $tr;
		
		my ($reac,$prod) = getReactantsProducts($tr);
		push @{$bpg->{'NodeList'}}, @$reac, @$prod;
		foreach my $reactant (@$reac)
		{
			my $edge = makeEdge($tr,$reactant,'r');
			push @{$bpg->{'NodeList'}}, $reactant;
			push @{$bpg->{'EdgeList'}}, $edge;
		}
		foreach my $product (@$prod)
		{
			my $edge = makeEdge($tr,$product,'p');
			push @{$bpg->{'NodeList'}}, $product;
			push @{$bpg->{'EdgeList'}}, $edge;
		}
		foreach my $context(@contexts)
		{
			my $edge = makeEdge($tr,$context,'c');
			push @{$bpg->{'NodeList'}}, $context;
			push @{$bpg->{'EdgeList'}}, $edge;
		}
		
		# add syndel edges
		if ($op->{'Name'} =~ /Mol/)
		{
			my @syndels = getSyndelContext(\@nodelist,$op);
			foreach my $syndel(@syndels)
			{
				my $edge = makeEdge($tr,$syndel,'s');
				push @{$bpg->{'NodeList'}}, $syndel;
				push @{$bpg->{'EdgeList'}}, $edge;
			}
		}
	}
	
	@{$bpg->{'NodeList'}} = uniq @{$bpg->{'NodeList'}};
	@{$bpg->{'EdgeList'}} = uniq @{$bpg->{'EdgeList'}};
	
	#print join("\n",map prettify($_), @{$bpg->{'NodeList'}}); print "\n\n";
	#print join("\n",@{$bpg->{'EdgeList'}}); print "\n\n";
	print printGraph($bpg);
	return $bpg;
}

1;