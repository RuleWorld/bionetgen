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
	'NodeClass' => '%', # a hash indicating equivalence class
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
	my @nodelist = @{$bpg->{'NodeList'}};
	my %nodetype = %{$bpg->{'NodeType'}};
	
	#get atomic patterns
	my @ap = grep { $nodetype{$_} eq 'AtomicPattern' } @nodelist;
	# get binding sites
	my @bs = sort {$a cmp $b} grep { $_ !~ /~/ and $_ !~ /\!/ } @ap;
	# get internal states
	my @is = sort {$a cmp $b} grep {$_ =~ /~/ } @ap;
	# get bonds
	my @bonds = sort {$a cmp $b} grep { $_ =~ /\!/ and $_ !~ /\!\+/ } @ap;
	# wildcards
	my @wc = sort {$a cmp $b} grep { $_ =~ /\!\+/ } @ap;
	# rules
	my @rules = sort {$a cmp $b} grep { $nodetype{$_} eq 'Rule' } @nodelist;
	# groups
	my @groups;
	my @names;
	if(defined $bpg->{'NodeClass'})
	{
	my %classes = %{$bpg->{'NodeClass'}};
	@names = uniq(sort values %classes);
	foreach my $grp(@names)
		{
		my @items = sort {$a cmp $b} grep {$classes{$_} eq $grp} keys %classes;
		push @groups, \@items;
		}
	}
	
	my @str;
	if(@bs) { push @str,"Binding Sites:\n".join("\n",@bs)."\n"; }
	if(@is) { push @str,"Internal States:\n".join("\n",@is)."\n"; }
	if(@bonds) { push @str,"Bonds:\n".join("\n",@bonds)."\n"; }
	if(@wc) { push @str,"Wildcards:\n".join("\n",@wc)."\n"; }
	if(@rules) { push @str,"Rules:\n".join("\n",@rules)."\n"; }
	if(@groups)
		{
		my @grpstrs = map {$names[$_].":".join(" ",@{$groups[$_]}) } 0..@groups-1;
		push @str,"Groups:\n".join("\n",@grpstrs)."\n";
		}
		
	my @edgelist = @{$bpg->{'EdgeList'}};
	my @reac = sort {$a cmp $b} map {$_ =~ /(.*:.*):.*/} grep {$_ =~ /Reactant$/} @edgelist;
	my @prod = sort {$a cmp $b} map {$_ =~ /(.*:.*):.*/} grep {$_ =~ /Product$/} @edgelist;
	my @context = sort {$a cmp $b} map {$_ =~ /(.*:.*):.*/} grep {$_ =~ /Context$/} @edgelist;
	my @wildcards = sort {$a cmp $b} map {$_ =~ /(.*:.*):.*/} grep {$_ =~ /Wildcard$/} @edgelist;
	
	if(@reac) { push @str,"Reactant Relationships:\n".join("\n",@reac)."\n"; }
	if(@prod) { push @str,"Product Relationships:\n".join("\n",@prod)."\n"; }
	if(@context) { push @str,"Context Relationships:\n".join("\n",@context)."\n"; }
	if(@wildcards) { push @str,"Wildcard Relationships:\n".join("\n",@wildcards)."\n"; }
	
	return join("\n",@str);
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

sub getTransformations
{
	my $rsg = shift @_;
	my @nodelist = @{$rsg->{'NodeList'}};
	my @graphop = hasType(\@nodelist,'GraphOp');
	my @tr = map {makeTransformation(\@nodelist,$_);} @graphop;
	return @tr;
}
sub reverseTransformation
{
	my $tr = shift @_; #unprettified
	my @splits = reverse split('->',prettify($tr));
	#my @splits2 = map ( join(',',sort split(',',$_ =~ s/\s//g)), @splits);
	sub clean { $_ =~ s/\s//g; return $_; }
	my @splits2 = map ( join(',',sort split(',',clean($_))), @splits);
	my $tr2 = unprettify(join '->',@splits2 );
	return $tr2;
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


# do things to network graphs
sub uniqNetworkGraph
{
	my $bpg = shift(@_);
	$bpg->{'NodeList'} = [uniq(@{$bpg->{'NodeList'}})];
	$bpg->{'EdgeList'} = [uniq(@{$bpg->{'EdgeList'}})];
	return;
}

sub makeRuleGroups
{
	my $bpg = shift @_;
	my %nodetype = %{$bpg->{'NodeType'}};
	my @rules = grep { $nodetype{$_} =~ /Rule/ } @{$bpg->{'NodeList'}};
	my @edges = grep { $_ =~ /:(Reactant|Product)/} @{$bpg->{'EdgeList'}} ;

	
	# get a list of reactants and products for each rule, "vals"
	my %reacprodhash;
	@reacprodhash { @rules } = 
		map  
			{
			my $rule = quotemeta $_;
			my $x = join " ", sort {$a cmp $b}
					map { $_ =~ /^$rule:(.*):.*/; $1; }
					grep { $_ =~ /^$rule:.*:.*/ } 
					@edges; 
			
			print $rule.":".$x."\n";
			$x;
			} @rules;
	
	
	my @vals = uniq(values(%reacprodhash));
	my @rulegroups = ();
	my @reacprods = ();
	# make groups from rules, based on similar reacprods
	# all these hoops are because I want to preserve order
	# in which rules are added to groups
	for(my $i=0;$i<@rules;$i++)
	{
		my $rule = $rules[$i];
		my $reacprod = $reacprodhash{$rule};

		push @reacprods, $reacprod if(has(\@reacprods,$reacprod)==0);
		for(my $j=0;$j<scalar @reacprods;$j++)
			{
			if ($reacprods[$j] eq $reacprod) 
				{
				if (not defined $rulegroups[$j]) { $rulegroups[$j] = []; }
				push @{$rulegroups[$j]}, $rule;
				}
			}
	}
	#print map {join(' ',@$_)."\n";} @rulegroups;
	return @rulegroups;
}

sub filterNetworkGraph
{
	my $bpg = shift @_;
	my $filter = shift @_;

	my @nodelist = @{$bpg->{'NodeList'}};
	my @edgelist = @{$bpg->{'EdgeList'}};
	my %nodetype = %{$bpg->{'NodeType'}};
	
	my @new_nodelist = grep { has($filter,$_)==0; } @nodelist;
	my %new_nodetype = map { $_=>$nodetype{$_} } @new_nodelist;
	
	my @removed_edges;
	my @remove1 = 	grep { 
					my $x = $_; 
					$x =~ /.*:(.*):.*/; 
					has($filter,$1)==1;
					} @edgelist;
	my @remove2 = 	grep { 
					my $x = $_; 
					$x =~ /(.*):.*:.*/; 
					has($filter,$1)==1;
					} @edgelist;
	my @new_edgelist = grep { has([(@remove1,@remove2)],$_)==0;} @edgelist;
	
	$bpg->{'NodeList'} = \@new_nodelist;
	$bpg->{'EdgeList'} = \@new_edgelist;
	$bpg->{'NodeType'} = \%new_nodetype;
	return;
}

sub filterNetworkGraphByList
{
	my $bpg = shift @_;
	my @items = @{shift @_};
	my $level = @_ ? shift @_ : 1;
	
	my @nodes = @{$bpg->{'NodeList'}};
	my @edges = @{$bpg->{'EdgeList'}};
	
	for (my $i=1; $i<=$level; $i++)
	{
		my @items2=();
		foreach my $edge(@edges)
		{
			$edge =~ /(.*):(.*):.*/;
			my $x = $1; my $y = $2;
			next if(has(\@items,$x)==has(\@items,$y));
			if(has(\@items,$x)==0) { push @items2,$x; }
			if(has(\@items,$y)==0) { push @items2,$y; }
			#print scalar @items2;print "\n";
		}
		push @items,uniq(@items2);
	}
	#print @items;
	@items = uniq(@items);
	my @remove = grep { has(\@items,$_)==0; } @{$bpg->{'NodeList'}};
	filterNetworkGraph($bpg,\@remove);
	uniqNetworkGraph($bpg);
	return;
}

sub makeRuleClasses
{
	my $model = shift @_;
	my @rgs = @{$model->VizGraphs->{'RuleGroups'}};
	my $gr = $model->VizGraphs;
	my $bpg = shift @_;
	#my @rgs = @{shift @_}; # rule groups
	my $pre = "G";
	my %classes;
	# Rule1 => RG1
	#map { @classes{@{$rgs[$_]}} = ($pre.$_) x @{$rgs[$_]};} 0..@rgs-1;
	# note: $bpg may have a filtered node set. 
	# so we need to filter the rule groups as well 
	# to only show the relevant ones
	foreach my $i(0..@rgs-1)
	{
		++$gr->{'NewName'};
		my @rg = @{$rgs[$i]};
		my @rules = grep { has(\@rg,$_)==1 } @{$bpg->{'NodeList'}};
		if(@rules) { @classes{@rules} = ($pre.$gr->{'NewName'}) x @rules };
	}
	$bpg->{'NodeClass'} = {} if( not defined($bpg->{'NodeClass'}) );
	
	my $cl = $bpg->{'NodeClass'};
	map { $$cl{$_} = $classes{$_}; } keys %classes;

	my $ty = $bpg->{'NodeType'};
	@$ty { values %classes } = ('RuleGroup') x scalar values %classes;
 	return;
}

sub collapseNetworkGraph
{
	my $bpg = shift @_;
	my %classes = %{$bpg->{'NodeClass'}};
	
	my @classed = keys %classes;
	my @edges = @{$bpg->{'EdgeList'}};
	
	my @edges2;
	map { push @edges2, $_; } @edges;
	
	my $edit = 1;
	while ($edit>0)
	{
		my @edges3;
		$edit = 0;
		foreach my $edge(@edges2)
		{
			foreach my $node(@classed)
			{
				if($edge =~ /$node:.*:.*/)  { $edge =~ s/($node):/$classes{$node}:/;  $edit++;	}
				if($edge =~ /.*:$node:.*/)  { $edge =~ s/:($node):/:$classes{$node}:/; $edit++; }			
			}
		push @edges3,$edge;
		}
		@edges2 = uniq(@edges3);
	}
	
	# recompiling $bpg
	my @nodes2;
	foreach my $edge(@edges2)
	{
		$edge =~ /(.*):(.*):.*/;
		push @nodes2, $1;
		push @nodes2, $2;
	}
	@nodes2 = uniq(@nodes2);
	
	my %nodetype2;
	my %nodetype = %{$bpg->{'NodeType'}};
	@nodetype2 { @nodes2 } = @nodetype { @nodes2 };
	
	my $bpg2 = NetworkGraph->new();
	$bpg2->{'NodeList'} = \@nodes2;
	$bpg2->{'EdgeList'} = \@edges2;
	$bpg2->{'NodeType'} = \%nodetype2;
	return $bpg2;	
}

1;

