# VisualizationIO.pm, 
# Input and output for viz tools
# Author: John Sekar (johnarul.sekar@gmail.com)

package Visualization;
# pragmas
use strict;
use warnings;
no warnings 'redefine';

# Perl Modules
use Class::Struct;
use List::Util qw(min max sum);
use List::MoreUtils qw( uniq);
use Data::Dumper;

# BNG Modules
use StructureGraph;
use BipartiteGraph;

struct GMLNode => 
{
	'ID' => '$',
	'type' => '$', 
	'label' => '$', 
	'fill' => '$', 
	'outlineStyle' => '$', 
	'fontStyle' => '$',
	'anchor'=>'$',
	'object'=>'$',
	'isGroup'=>'$',
	'gid'=>'$'
};

struct GMLEdge =>
{
	'source' => '$',
	'target' => '$',
	'fill' => '$',
	'sourceArrow' => '$',
	'targetArrow' => '$',
	'object' => '$'
};

struct GMLGraph =>
{
	'Nodes'=>'@',
	'Edges'=>'@',
};


sub initializeGMLNode
{
	my $id = shift @_;
	my $label = shift @_;
	my $object = shift @_;
	
	my $gmlnode = GMLNode->new();
	$gmlnode->{'ID'} = $id;
	$gmlnode->{'label'} = $label;
	$gmlnode->{'object'} = $object;
	
	$gmlnode->{'fill'} = "#FFFFFF";
	$gmlnode->{'outlineStyle'} = "";
	$gmlnode->{'fontStyle'} = "";
	$gmlnode->{'anchor'} = "c";
	$gmlnode->{'isGroup'} = 0;
	$gmlnode->{'gid'} = "";
	
	return $gmlnode;
}

sub initializeGMLEdge
{
	my $source = shift @_;
	my $target = shift @_;

	#my $object = @_ ? shift @_ : "";
	my $targetArrow = @_ ? shift @_ : 0;
	my $sourceArrow = @_ ? shift @_ : 0;
	
	my $gmledge = GMLEdge->new();
	$gmledge->{'source'} = $source;
	$gmledge->{'target'} = $target;
	if ($targetArrow) { $gmledge->{'targetArrow'} = 1;}
	if ($sourceArrow) { $gmledge->{'sourceArrow'} = 1;}
	
	return $gmledge;
}

sub styleNode
{
	my $gmlnode = shift @_;
	my $arg = shift @_;
	# defaults
	$gmlnode->{'type'} = "roundrectangle";
	$gmlnode->{'fill'} = "#FFFFFF";
	if ($gmlnode->{'isGroup'}) {$gmlnode->{'anchor'} = "t";}
	else {$gmlnode->{'anchor'} = "c";}
	
	# node, Rule, yED
	if ($arg==1) { $gmlnode->{'outlineStyle'} = "dotted"; }
	# node, Mol, yED
	if ($arg==2) { $gmlnode->{'fontStyle'} = "bold"; }
	# node, Comp/BondState, yED
	if ($arg==3) { $gmlnode->{'fill'} = "#D2D2D2"; }
	# node, CompState, yED
	if ($arg==4) { $gmlnode->{'fill'} = "#FFCC00"; }
	# node, GraphOp, yED
	if ($arg==5) 
	{ 
		$gmlnode->{'fill'} = "#CC99FF"; 
		$gmlnode->{'type'} = 'hexagon';
		$gmlnode->{'fontStyle'} = 'italic';
	}
	return;
}

sub styleEdge
{
	my $gmledge = shift @_;
	my $arg = @_ ? shift @_ : 1 ;
	# defaults
	$gmledge->{'fill'} = "#000000";
	return;
}

sub printGraph
{
	my $gmlgraph = shift @_;
	my @nodes = @{$gmlgraph->{'Nodes'}};
	my @edges = @{$gmlgraph->{'Edges'}};
	
	my @nodestrings = ();
	my @edgestrings = ();
	
	my $q1 = " \"";
	my $q2 = "\" ";
	foreach my $node(@nodes)
	{
		my $string1= "";
		# graphics
		$string1 .= "type".$q1.$node->{'type'}.$q2;
		$string1 .= "fill".$q1.$node->{'fill'}.$q2;
		if ($node->{'outlineStyle'})
			{$string1 .= "outlineStyle".$q1.$node->{'outlineStyle'}.$q2;}
		$string1 = "graphics [ ".$string1." ]";
		
		# labelgraphics
		my $string2 = "";
		$string2 .= "text".$q1.$node->{'label'}.$q2;
		#$string2 .= "fill".$q1."#FFFFFF".$q2;
		$string2 .= "anchor".$q1.$node->{'anchor'}.$q2;
		if ($node->{'fontStyle'})
			{$string2 .= "fontStyle".$q1.$node->{'fontStyle'}.$q2;}
		$string2 = "LabelGraphics [ ".$string2." ]";
		
		my $string = "";
		$string .= "id".$q1.$node->{'ID'}.$q2;
		$string .= "label".$q1.$node->{'label'}.$q2;
		if ($node->{'isGroup'})
			{$string .= "isGroup ".$node->{'isGroup'}." ";}
		if (defined $node->{'gid'})
			{$string .= "gid".$q1.$node->{'gid'}.$q2;}
		$string = "node [".$string." ".$string1." ".$string2." ]";
		push @nodestrings, $string;
	}
	foreach my $edge(@edges)
	{
		my $string = "";
		my $string2 = "";
		$string2 .= "fill".$q1.$edge->{'fill'}.$q2;
		if ($edge->{'sourceArrow'} and $edge->{'targetArrow'})
			{
			$string2 .= "arrow \"both\" ";
			}
		elsif($edge->{'sourceArrow'})
			{
			$string2 .= "arrow \"first\" ";
			}
		elsif($edge->{'targetArrow'})
			{
			$string2 .= "arrow \"last\" ";
			}
		$string2 = "graphics [ ".$string2." ]";
		
		$string .= "source".$q1.$edge->{'source'}.$q2;
		$string .= "target".$q1.$edge->{'target'}.$q2;
		$string = "edge [ ".$string." ".$string2." ]";
		push @edgestrings,$string;
	}

	my $string = "graph\n[\n directed 1\n";
	$string .= join("\n",@nodestrings)."\n";
	$string .= join("\n",@edgestrings)."\n";
	$string .= "]\n";	
	return $string;
}

sub toGML_rules
{
	my $sg = shift @_; #imports a combined rule structure graph

	#this is a structure graph.
	# could be pattern or a rule or combination of rules
	my $type = $sg->{'Type'};
	my @nodelist = @{$sg->{'NodeList'}};
	
	# remap all the ids to integers
	my @idlist = map {$_->{'ID'}} @nodelist;
	my %indhash = StructureGraph::indexHash(\@idlist);
	foreach my $node(@nodelist) { StructureGraph::remapNode($node,\%indhash); }
	#StructureGraph::remapNodeList(\@nodelist,\%indhash);
	

	my @structnodes = grep ( { $_->{'Type'} ne 'BondState' and $_->{'Type'} ne 'GraphOp'} @nodelist);
	my @gmlnodes = ();
	foreach my $node(@nodelist)
	{
		my $id = $node->{'ID'};
		my $name = $node->{'Name'};
		my $type = $node->{'Type'};
		
		# ignore if it is a bond with two parents
		if ( $type eq 'BondState' and scalar @{$node->{'Parents'}} == 2) 
		{ next; }
		
		my $gmlnode = initializeGMLNode($id,$name,$node);
		my $isstruct = 1;
		# treat rules
		if ($type eq 'Rule') { $gmlnode->{'isGroup'} = 1; $isstruct = 0;}
		# treat graph ops
		if ($type eq 'GraphOp')
		{
			if ($name eq 'ChangeState')
			{
				my @parents = @{$node->{'Parents'}};
				my $parent = StructureGraph::findNode(\@nodelist,$parents[0]);
				my @grandparents = @{$parent->{'Parents'}};
				my $grandparent = $grandparents[0];
				$gmlnode->{'gid'} = $grandparents[0];
			}
			else
			{ $gmlnode->{'gid'} = $node->{'Rule'}; }
			$isstruct = 0;
		}
		# treat wildcard bonds
		if ($type eq 'BondState') 
		{
			$gmlnode->{'gid'} = $node->{'Rule'}; 
			$isstruct = 0; 
		}
		
		# remaining nodes are structural and nonbonds
		if ($isstruct)
		{
			if ($node->{'Parents'})
			{
				foreach my $parent_id(@{$node->{'Parents'}}) 
				{ 
					$gmlnode->{'gid'} = $parent_id;
				}
			}
			else 
			{ 
				$gmlnode->{'gid'} = $node->{'Rule'}; 
			}
			if (StructureGraph::hasChildren(\@structnodes,$node))
			{
				$gmlnode->{'isGroup'} = 1;
			}
		}
		push @gmlnodes, $gmlnode;
	}
	
	my @bondnodes = grep ( { $_->{'Type'} eq 'BondState' } @nodelist);
	my @gmledges = ();
	
	# make edges for bonds
	foreach my $node (@bondnodes)
	{
		my @parents = @{$node->{'Parents'}};
		my $source;
		my $target;
		# address wildcards
		if (scalar @parents == 1) 
		{
			$source = $node->{'ID'};
			$target = $parents[0];
		}
		# ignore bonds that are made or removed
		elsif ($node->{'Side'} eq 'both')
		{ 			
			$source = $parents[0];
			$target = $parents[1]; 
		}
		else {next;}
		my $gmledge = initializeGMLEdge($source,$target);
		push @gmledges,$gmledge;
	}
	
	# make edges for edges adjacent to graph operation nodes
	my @graphopnodes = grep ( { $_->{'Type'} eq 'GraphOp'} @nodelist);
	foreach my $node (@graphopnodes)
	{
		my $name = $node->{'Name'};
		my $id = $node->{'ID'};
		my @parents = @{$node->{'Parents'}};
		my @c;
		my @p;
		my @consumed;
		my @produced;
		
		if ($name eq 'ChangeState')
			{
			my @compstates = grep ( { $_->{'Type'} eq 'CompState' } @nodelist);
			my @nodes = StructureGraph::findNodes(\@compstates,\@parents);
			@c = grep ( { $_->{'Side'} eq 'left' } @nodes);
			@p = grep ( { $_->{'Side'} eq 'right' } @nodes);
			}
			
		if ($name eq 'AddMol' or $name eq 'DeleteMol')
			{
			my @mols = grep ( { $_->{'Type'} eq 'Mol' } @nodelist);
			my @nodes = StructureGraph::findNodes(\@mols,\@parents);
			@c = grep ( { $_->{'Side'} eq 'left' } @nodes);
			@p = grep ( { $_->{'Side'} eq 'right' } @nodes);
			}
			
		if ($name eq 'AddBond' or $name eq 'DeleteBond')
			{
				my @allbonds = grep ( { $_->{'Type'} eq 'BondState' } @nodelist);
				my $bond = StructureGraph::findNode(\@allbonds,$parents[0]);
				my @comps = grep ( { $_->{'Type'} eq 'Comp' } @nodelist);
				my @nodes = StructureGraph::findNodes(\@comps, $bond->{'Parents'});
				if ($name eq 'DeleteBond') { @c = @nodes; }
				if ($name eq 'AddBond') { @p = @nodes; }
			}
		
		if (@c) { @consumed = map ($_->{'ID'},@c); }
		if (@p) { @produced = map ($_->{'ID'},@p); }	
		
		foreach my $id2(@consumed)
		{
			my $gmledge = initializeGMLEdge($id2,$id,1);
			push @gmledges,$gmledge;
		}
		
		foreach my $id2(@produced)
		{
			my $gmledge = initializeGMLEdge($id,$id2,1);
			push @gmledges,$gmledge;
		}
	}
	
	# styling
	my %stylemap = ('Rule'=>1,'Pattern'=>1, 'Mol'=>2, 'Comp'=>3,
	'BondState'=>3, 'CompState'=>4, 'GraphOp'=>5 );
	
	foreach my $node(@gmlnodes)
	{
	my $object = $node->{'object'};
	my $type = $object->{'Type'};
	styleNode($node,$stylemap{$type});
	}
	
	foreach my $edge(@gmledges)
	{
	styleEdge($edge);
	}
	
	my $gmlgraph = GMLGraph->new();
	$gmlgraph->{'Nodes'} = \@gmlnodes;
	$gmlgraph->{'Edges'} =\@gmledges;
	return printGraph($gmlgraph);
}

sub toGML_rules_eqn
{
	my $sg = shift @_; #imports a rule bipartite graph
	my @nodelist = @{$sg->{'NodeList'}};
	#remap all ids to integers
	my @idlist = map{$_->{'ID'}} @nodelist;
	my %indhash = StructureGraph::indexHash(\@idlist);
	foreach my $node(@nodelist) { StructureGraph::remapNode($node,\%indhash); }
	my @structnodes = grep ( 
	{ $_->{'Type'} ne 'BondState' 
	and $_->{'Type'} ne 'Rule'
	and $_->{'Type'} ne 'Pattern'} @nodelist);
	#print scalar @nodelist; print "\n";
	#print scalar @structnodes; print "\n";
	
	my @gmlnodes = ();
	foreach my $node(@nodelist)
	{
		my $id = $node->{'ID'};
		my $name = $node->{'Name'};
		my $type = $node->{'Type'};
				
		# ignore if it is a bond with two parents
		if ( $type eq 'BondState' and scalar @{$node->{'Parents'}} == 2) 
		{ next; }
		
		my $gmlnode = initializeGMLNode($id,$name,$node);
		my $isstruct = 1;
		
		# treat patterns
		if ($type eq 'Pattern') 
		{ 
			$gmlnode->{'isGroup'} = 1; 
			#$gmlnode->{'gid'} = $node->{'Rule'};
			$isstruct = 0;
		}
		
		# treat wildcard bonds
		if ($type eq 'BondState') 
		{
			my @comps = @{$node->{'Parents'}};
			my $comp = StructureGraph::findNode(\@nodelist,$comps[0]);
			my @mols = @{$comp->{'Parents'}};
			my $mol = StructureGraph::findNode(\@nodelist,$mols[0]);
			my @sp = @{$mol->{'Parents'}};
			$gmlnode->{'gid'} = $sp[0]; 
			$isstruct = 0; 
		}
		if ($isstruct)
		{
			if ($node->{'Parents'})
			{
			foreach my $parent_id(@{$node->{'Parents'}}) 
				{ 
					$gmlnode->{'gid'} = $parent_id;
				}
			}
			if (StructureGraph::hasChildren(\@structnodes,$node))
			{
				$gmlnode->{'isGroup'} = 1;
			}
		}
		push @gmlnodes, $gmlnode;
	}
	
	my @gmledges = ();
	# draw the bonds
	my @bondnodes = grep ( { $_->{'Type'} eq 'BondState' } @nodelist);
	foreach my $node(@bondnodes)
	{
		my @parents = @{$node->{'Parents'}};
		my $source;
		my $target;
		if (scalar @parents == 1)
		{
			$source = $node->{'ID'};
			$target = $parents[0];
		}
		else
		{
			$source = $parents[0];
			$target = $parents[1];		
		}
		my $gmledge = initializeGMLEdge($source,$target);
		push @gmledges,$gmledge;
		
	}
	# draw edges to rules
	
	my @rulenodes = grep ( { $_->{'Type'} eq 'Rule' } @nodelist);
	my @patterns = grep ( { $_->{'Type'} eq 'Pattern' } @nodelist);
	foreach my $rule(@rulenodes)
	{
	#find the participating species
	my $id = $rule->{'ID'};
	my @participating = grep ( { $_->{'Rule'} eq $id } @patterns);
	my @reac = grep ( { index($_->{'Name'},'R')==0 } @participating);
	my @prod = grep ( { index($_->{'Name'},'P')==0} @participating);
	my $targetarrow = 1;
	my $sourcearrow = $rule->{'Reversible'} ? 1 : 0;
	foreach my $patt(@reac)
		{
		my $source = $patt->{'ID'};
		my $target = $id;
		my $gmledge = initializeGMLEdge($source,$target,$targetarrow,$sourcearrow);
		push @gmledges,$gmledge;
		}
	foreach my $patt(@prod)
		{
		my $source = $id;
		my $target = $patt->{'ID'};
		my $gmledge = initializeGMLEdge($source,$target,$targetarrow,$sourcearrow);
		push @gmledges,$gmledge;
		}
	}
	
	my %stylemap = ('Rule'=>5,'Pattern'=>1, 'Mol'=>2, 'Comp'=>3,
	'BondState'=>3, 'CompState'=>4 );
	foreach my $node(@gmlnodes)
	{
	my $object = $node->{'object'};
	my $type = $object->{'Type'};
	styleNode($node,$stylemap{$type});
	}
	
	foreach my $edge(@gmledges)
	{
	styleEdge($edge);
	}
	
	my $gmlgraph = GMLGraph->new();
	$gmlgraph->{'Nodes'} = \@gmlnodes;
	$gmlgraph->{'Edges'} =\@gmledges;
	return printGraph($gmlgraph);
	
}
1;
