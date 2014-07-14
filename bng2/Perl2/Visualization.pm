# Visualization.pm, 
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
#use List::MoreUtils qw( uniq);
use Data::Dumper;

# BNG Modules
use StructureGraph;
use BipartiteGraph;
use ProcessGraph;

sub listHas { return BipartiteGraph::listHas(@_); }
sub uniq { return BipartiteGraph::uniq(@_); }
	
	

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
	'gid'=>'$',
	'hasOutline' => '$',
	'outline'=>'$'
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
	my $object = @_ ? shift @_ : 0;
	
	my $gmledge = GMLEdge->new();
	$gmledge->{'source'} = $source;
	$gmledge->{'target'} = $target;
	if ($targetArrow) { $gmledge->{'targetArrow'} = 1;}
	if ($sourceArrow) { $gmledge->{'sourceArrow'} = 1;}
	if ($object) { $gmledge->{'object'} = $object; }
	
	return $gmledge;
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
		if ($node->{'hasOutline'}==0) { $string1 .= "hasOutline 0 ";}
		if (defined $node->{'outline'}) 
			{ $string1 .= "outline".$q1.$node->{'outline'}.$q2;}
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
		if (defined $node->{'gid'} and length($node->{'gid'}) > 0)
			{$string .= "gid".$q1.$node->{'gid'}.$q2;}
		
		$string = "node [".$string." ".$string1." ".$string2." ]";
		push @nodestrings, $string;
	}
	foreach my $edge(@edges)
	{
		my $string = "";
		my $string2 = "";
		$string2 .= "fill".$q1.$edge->{'fill'}.$q2;
		my @temp = (0,1);
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
		if ($edge->{'sourceArrow'})
			{
				if(!listHas(\@temp,$edge->{'sourceArrow'}))
					{
					$string2 .= "sourceArrow".$q1.$edge->{'sourceArrow'}.$q2;
					}
			}
		if ($edge->{'targetArrow'})
			{
				if(!listHas(\@temp,$edge->{'targetArrow'}))
					{
					$string2 .= "targetArrow".$q1.$edge->{'targetArrow'}.$q2;
					}
			}
		if (defined $edge->{'width'}) 
		{
			$string2 .= "width ".$edge->{'width'}." ";
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

sub toGML_contact
{
	
	my $sg = shift @_;
	my @nodelist = @{$sg->{'NodeList'}};
	my @idlist = map{$_->{'ID'}} @nodelist;
	my %indhash = StructureGraph::indexHash(\@idlist);
	foreach my $node(@nodelist) { StructureGraph::remapNode($node,\%indhash); }
	my @structnodes = grep $_->{'Type'} ne 'BondState', @nodelist;
	
	my @gmlnodes = ();
	foreach my $node(@structnodes)
	{
		my $id = $node->{'ID'};
		my $name = $node->{'Name'};
		my $type = $node->{'Type'};
		my $gmlnode = initializeGMLNode($id,$name,$node);
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
	my %stylemap = ('Mol'=>2, 'Comp'=>3,
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


sub toGML_regulatory
{
	my $bpg = shift @_;
	my @nodelist = @{$bpg->{'NodeList'}};
	my @edgelist = @{$bpg->{'EdgeList'}};
	my @groups = @{$bpg->{'NodeGroups'}};
	
	my @allnodes = ();
	push @allnodes, @nodelist;
	push @allnodes, @groups;
	my %indhash = StructureGraph::indexHash( \@allnodes );

	my @gmlnodes = ();
	foreach my $node(@allnodes)
	{
		my $id = $indhash{$node};
		my $name;
		my $type;
		my $gmlnode;
		
		if (ref($node))
		{
			my @x = @$node;
			$name = BipartiteGraph::groupName($node);
			$type = 'Group';
			$gmlnode = initializeGMLNode($id,$name,$node);
			$gmlnode->{'isGroup'} = 1;
		}
		else 
		{
			$name = BipartiteGraph::prettify($node);
			$type = BipartiteGraph::isTransformation( $node ) ? 'Transformation' : 'AtomicPattern';
			$gmlnode = initializeGMLNode($id,$name,$node);
			my $gid;
			foreach my $grp (@groups)
			{
				if (listHas($grp,$node)) { $gid = $indhash{$grp}; last;}
			}
			$gmlnode->{'gid'} = $gid;
		}
		push @gmlnodes, $gmlnode;
	}
	
	my @gmledges = ();
	foreach my $edge( @edgelist )
	{
		my @splits = split(":",$edge);
		next if ($splits[2] eq 'ProcessPair');
		my $source = $indhash{$splits[0]};
		my $target = $indhash{$splits[1]};
		my $gmledge = initializeGMLEdge($source,$target,"","",$edge);
		push @gmledges,$gmledge;
	}
	
	my %nodestyle = ('Group'=>6,'AtomicPattern'=>7,'Transformation'=>8);
	my %edgestyle = ('Reactant'=>1,'Product'=>2,'Context'=>3,'Wildcard'=>1,
	'Syn'=>4,'Del'=>5,'ProcessPair'=>6,'Cotransform'=>7,'Onsite'=>13);
		
	foreach my $node(@gmlnodes)
	{
		if ($node->{'isGroup'}) 
			{ styleNode($node,$nodestyle{'Group'});}
		elsif ( BipartiteGraph::isTransformation( $node->{'object'}) ) 
			{ styleNode($node,$nodestyle{'Transformation'}); }
		else
			{ styleNode($node,$nodestyle{'AtomicPattern'});}
			
	}
	
	foreach my $edge(@gmledges)
	{
		my $object = $edge->{'object'};
		my @splits = split ":", $object;
		my $type = $splits[2];
		if ($type eq 'Syndel')
			{
			if (BipartiteGraph::isSyn($splits[0])) { $type = 'Syn' };
			if (BipartiteGraph::isDel($splits[0])) { $type = 'Del' };
			}
		styleEdge($edge,$edgestyle{$type});
	}
	my $gmlgraph = GMLGraph->new();
	$gmlgraph->{'Nodes'} = \@gmlnodes;
	$gmlgraph->{'Edges'} =\@gmledges;
	return printGraph($gmlgraph);
}

sub toGML_regulatory3
{
	my $bpg = shift @_;
	my @nodelist = @{$bpg->{'NodeList'}};
	my @edgelist = @{$bpg->{'EdgeList'}};
	
	my %indhash = StructureGraph::indexHash( \@nodelist );
	my @gmlnodes = ();
	foreach my $node(@nodelist)
	{
		my $id = $indhash{$node};
		my $name = BipartiteGraph::prettify($node);
		my $gmlnode = initializeGMLNode($id,$name,$node);
		push @gmlnodes, $gmlnode;
	}
	
	my @gmledges = ();
	foreach my $edge( @edgelist )
	{
		my @splits = split(":",$edge);
		my $source = $indhash{$splits[0]};
		my $target = $indhash{$splits[1]};
		my $gmledge = initializeGMLEdge($source,$target,"","",$edge);
		push @gmledges,$gmledge;
	}
	
	my %nodestyle = ('Group'=>6,'AtomicPattern'=>7,'Transformation'=>8);
	my %edgestyle = ('Reactant'=>1,'Product'=>2,'Context'=>3,'Wildcard'=>1,
	'Syn'=>4,'Del'=>5,'ProcessPair'=>6,'Cotransform'=>7,'Onsite'=>13);
		
	foreach my $node(@gmlnodes)
	{
		if($node->{'label'} =~ /^Rule/) 
			{ styleNode($node,$nodestyle{'Transformation'}); }
		else
			{ styleNode($node,$nodestyle{'AtomicPattern'});}		
	}
	
	foreach my $edge(@gmledges)
	{
		my $object = $edge->{'object'};
		my @splits = split ":", $object;
		my $type = $splits[2];
		if ($type eq 'Syndel')
			{
			if (BipartiteGraph::isSyn($splits[0])) { $type = 'Syn' };
			if (BipartiteGraph::isDel($splits[0])) { $type = 'Del' };
			}
		styleEdge($edge,$edgestyle{$type});
	}
	my $gmlgraph = GMLGraph->new();
	$gmlgraph->{'Nodes'} = \@gmlnodes;
	$gmlgraph->{'Edges'} =\@gmledges;
	return printGraph($gmlgraph);
}



sub toGML_process
{
	my @bi = @{shift @_};
	my @uni = @{shift @_};
	my @nodelist = @{shift @_};;
	my @edgelist = ();
	my %indhash = StructureGraph::indexHash(\@nodelist);
	my @gmlnodes = ();
	foreach my $node(@nodelist)
	{
		my $gmlnode = initializeGMLNode($indhash{$node},$node,$node);
		styleNode($gmlnode);
		push @gmlnodes,$gmlnode;
	}		
	
	my @gmledges = ();
	foreach my $node(@bi)
	{
		my @splits = split " ",$node;
		my $source = $indhash{$splits[0]};
		my $target = $indhash{$splits[1]};
		my $gmledge = initializeGMLEdge($source,$target,1,1);
		styleEdge($gmledge);
		push @gmledges,$gmledge;
	}
	foreach my $node(@uni)
	{
		my @splits = split " ",$node;
		my $source = $indhash{$splits[0]};
		my $target = $indhash{$splits[1]};
		my $gmledge = initializeGMLEdge($source,$target,1,0);
		styleEdge($gmledge);
		push @gmledges,$gmledge;
	}
	
	my $gmlgraph = GMLGraph->new();
	$gmlgraph->{'Nodes'} = \@gmlnodes;
	$gmlgraph->{'Edges'} =\@gmledges;
	return printGraph($gmlgraph);
}


sub transfNames
{
	my $tr = shift @_;
	my ($re,$pr) = BipartiteGraph::getReactantsProducts($tr);
	my @reac = @$re;
	my @prod = @$pr;
	
	if ($tr =~ /~/)
		{
			$reac[0] =~ /^(.*)\((.*)~(.*)\)$/;
			my $mol = $1;
			my $comp = $2;
			my $state1 = $3;
			$prod[0] =~ /^(.*)\((.*)~(.*)\)$/;
			my $state2 = $3;
			return $mol."(".$comp."~".$state1."->".$state2.")";
		}
	if ($tr =~ /\!/)
		{
			return (scalar @reac > scalar @prod)? "->".join(":",@reac) : join(":",@prod)."->";
		}
	if (BipartiteGraph::isSyn($tr)) { return "->".$prod[0]; }
	if (BipartiteGraph::isDel($tr)) { return $reac[0]."->"; }
	return $tr;
}

sub groupNames
{
	my $tr = shift @_;
	my ($re,$pr) = BipartiteGraph::getReactantsProducts($tr);
	my @reac = @$re;
	my @prod = @$pr;
	
	if ($tr =~ /~/)
		{
			$reac[0] =~ /^(.*)\((.*)~(.*)\)$/;
			my $mol = $1;
			my $comp = $2;
			my $state1 = $3;
			$prod[0] =~ /^(.*)\((.*)~(.*)\)$/;
			my $state2 = $3;
			#return $mol."(".$comp."~".$state1."->".$state2.")";
			return $mol."(".$comp."~)";
		}
	if ($tr =~ /\!/)
		{
			#return (scalar @reac > scalar @prod)? "->".join(":",@reac) : join(":",@prod)."->";
			return (scalar @reac > scalar @prod)? join(":",@reac) : join(":",@prod);
		}
	if (BipartiteGraph::isSyn($tr)) { return $prod[0]; }
	if (BipartiteGraph::isDel($tr)) { return $reac[0]; }
	return $tr;
}
sub transfNames2
{
	my $tr = shift @_;
	my ($re,$pr) = BipartiteGraph::getReactantsProducts($tr);
	my @reac = @$re;
	my @prod = @$pr;
	
	if ($tr =~ /~/)
		{
			$reac[0] =~ /^(.*)\((.*)~(.*)\)$/;
			my $mol = $1;
			my $comp = $2;
			my $state1 = $3;
			$prod[0] =~ /^(.*)\((.*)~(.*)\)$/;
			my $state2 = $3;
			return $state1."->".$state2;
		}
	if ($tr =~ /\!/)
		{
			return (scalar @reac > scalar @prod)? "->".join(":",@reac) : join(":",@prod)."->";
		}
	if (BipartiteGraph::isSyn($tr)) { return "syn"; }
	if (BipartiteGraph::isDel($tr)) { return "deg"; }
	return $tr;
}


sub nodelabel
{
my @nodes = @{shift @_};
my @hashes = @{shift @_};
my %singlehash = %{$hashes[0]};
my %pairhash = %{$hashes[1]};
my @pairedlist = keys %pairhash;
my @unpairedlist = grep !listHas(\@pairedlist,$_), keys %singlehash;
my %names;

foreach my $node(@nodes)
{
	my $isgroup = (scalar (@$node) > 1) ? 1 : 0;
	my $isalone = listHas(\@unpairedlist,$$node[0])  ? 1: 0;
	if ($isgroup)
	{
		$names { $pairhash{$$node[0]} } = groupNames($$node[0]);
		foreach my $i(0..@$node-1)
		{
			#$names { $singlehash{$$node[$i]} } = ($i==0) ? "f" :"r";
			#if(BipartiteGraph::isSyn($$node[$i])) { $names { $singlehash{$$node[$i]} } = "syn"; }
			#if(BipartiteGraph::isDel($$node[$i])) { $names { $singlehash{$$node[$i]} } = "deg"; }
			$names { $singlehash{$$node[$i]} } = transfNames2($$node[$i]);
		}
	}
	if ($isalone)
	{
		$names { $singlehash{$$node[0]} } = transfNames($$node[0]);
	}
}
return %names;
}

sub toGML_process2
{
	my @nodes = @{shift @_};
	my @influences = @{shift @_};

	# construct pairhash and singlehash
	my %pairhash;
	my %singlehash;
	foreach my $i(0..@nodes-1)
	{
		my @trs = @{$nodes[$i]};
		if (scalar @trs == 2) { @pairhash { @trs } = ($i) x @trs; }
		if (scalar @trs == 1) { @singlehash { @trs } = ($i) x @trs; }
	}
	my @pairedlist = keys %pairhash;
	
	my @gmlnodes = ();
	my %labels = nodelabel(\@nodes,[\%singlehash,\%pairhash]);
	my @i = 0..@nodes-1;
	#%labels = map { $_ => ( (scalar @{$nodes[$_]} > 1) ? "" : $nodes[$_]->[0]) } 0..@nodes-1;
	
	foreach my $i(0..@nodes-1)
	{
		my $node = $nodes[$i];
		my $isgroup = (scalar @$node > 1) ? 1:0;
		my $id = $isgroup ? $pairhash{$$node[0]} : $singlehash{$$node[0]};
		#my $label = $isgroup ? "" : $$node[0];
		#my $label = nodelabel($node,[\%singlehash,\%pairhash]);
		my $label = $labels{$i};
		my $object = $isgroup ? join(":",@$node) : $$node[0];
		my $gmlnode = initializeGMLNode($id,$label,$object);
		if ($isgroup) 
			{
			$gmlnode->{'isGroup'} = 1; 
			styleNode($gmlnode,6);
			}
		else
			{
			styleNode($gmlnode,8);
			if (listHas(\@pairedlist,$$node[0])) { $gmlnode->{'gid'} = $pairhash{$$node[0]}; }
			}
		push @gmlnodes,$gmlnode;
	}
	
	my @gmledges = ();
	my @act = grep /Activation/, @influences;
	my @inh = grep /Inhibition/, @influences;
	foreach my $str(@influences)
	{
		my @ids = split(":",$str);
		my $type = pop @ids;
		my $gmledge;
		if ($type eq 'Activation')
			{
			$gmledge = initializeGMLEdge($ids[0],$ids[1],1);
			styleEdge($gmledge,8);
			}
		elsif ($type eq 'Inhibition')
			{
			$gmledge = initializeGMLEdge($ids[0],$ids[1],1);
			styleEdge($gmledge,9);
			}
		elsif ($type eq 'Competition')
			{
			$gmledge = initializeGMLEdge($ids[0],$ids[1],1,1);
			styleEdge($gmledge,12);
			}
		elsif ($type eq 'Synthesis')
			{
			$gmledge = initializeGMLEdge($ids[0],$ids[1],1);
			styleEdge($gmledge,10);
			}
		elsif ($type eq 'Degradation')
			{
			$gmledge = initializeGMLEdge($ids[0],$ids[1],1);
			styleEdge($gmledge,11);
			}
		else {next;}
		push @gmledges,$gmledge;
	}

	@gmledges = uniq @gmledges;
	
	my $gmlgraph = GMLGraph->new();
	$gmlgraph->{'Nodes'} = \@gmlnodes;
	$gmlgraph->{'Edges'} =\@gmledges;
	return printGraph($gmlgraph);
}


# styling
my %nodepalette2 = 
	('Group'=>"#e5e5e5",
	'Pattern'=>"#ffcc00", 
	'Transformation'=>"#c57eed");
my %outline1 = 
	('Group'=>"#cecece",
	'Pattern'=>"#e5bece", 
	'Transformation'=>"#93c3ae");
	
	
my %nodepalette1 = 
	('Group'=>"#efdbc4",
	'Pattern'=>"#fda7a9", 
	'Transformation'=>"#bbb8f4");
my %outline = 
	('Group'=>"#000000",
	'Pattern'=>"#000000", 
	'Transformation'=>"#000000");
	
# red #ff8d84"
# blue #4cafaf
my %edgepalette2 = 
	('Reactant'=>"#7d7566",
	'Product'=>"#7d7566",
	'Context'=>"#9068d4",
	'ProcessPair'=>"#c57eed",
	'Cotransform'=>"#9068d4",
	'Syndel'=>"#ff8d84");

my $reaccolor = "#5e3c58";
my $contcolor = "#798e87";

my %edgepalette1 = 
	('Reactant'=>$reaccolor,
	'Product'=>$reaccolor,
	'Context'=>$contcolor,
	'ProcessPair'=>"#000000",
	'Cotransform'=>$contcolor,
	'Syndel'=>$contcolor);

sub styleNode
{
	my $gmlnode = shift @_;
	my $arg = @_ ? shift @_ : 0;
	$gmlnode->{'hasOutline'} = 1;
	# defaults
	$gmlnode->{'type'} = "roundrectangle";
	$gmlnode->{'fill'} = "#FFFFFF";
	$gmlnode->{'outline'} = "#000000";
	if ($gmlnode->{'isGroup'}) {$gmlnode->{'anchor'} = "t";}
	else {$gmlnode->{'anchor'} = "c";}
	
	# node, Rule, yED
	if ($arg==1) { $gmlnode->{'outlineStyle'} = "dotted"; }
	# node, Mol, yED
	if ($arg==2) { $gmlnode->{'fontStyle'} = "bold"; $gmlnode->{'fill'} = "#D2D2D2";}
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
	# groups on bipartite graph, yED
	if ($arg==6) 
		{ 
		$gmlnode->{'fill'} = $nodepalette1{'Group'};  
		$gmlnode->{'fontStyle'} = "bold";
		}
	# pattern on bipartite graph
	if ($arg==7) 
		{
		$gmlnode->{'fill'} = $nodepalette1{'Pattern'}; 
		}
	# transformation on bipartite graph, yED
	if ($arg==8)
	{
		$gmlnode->{'fill'} = $nodepalette1{'Transformation'}; 
		$gmlnode->{'type'} = 'hexagon';
	}
	return;
}

sub styleEdge
{
	my $gmledge = shift @_;
	my $arg = @_ ? shift @_ : 0 ;
	# defaults
	$gmledge->{'fill'} = "#000000";
	
	# reactant/wildcard
	if ($arg==1) 
		{
		$gmledge->{'fill'} = $edgepalette1{'Reactant'};
		$gmledge->{'sourceArrow'} = 1;
		$gmledge->{'targetArrow'} = 0;
		$gmledge->{'width'} = 3;
		}
	# product
	if ($arg==2) 
		{
		$gmledge->{'fill'} = $edgepalette1{'Product'};
		$gmledge->{'sourceArrow'} = 0;
		$gmledge->{'targetArrow'} = 1;
		$gmledge->{'width'} = 3;
		}
	# context
	if ($arg==3) 
		{
		$gmledge->{'fill'} = $edgepalette1{'Context'};
		$gmledge->{'sourceArrow'} = 1;
		$gmledge->{'targetArrow'} = 0;
		}
	# syn context
	if ($arg==4) 
		{
		$gmledge->{'fill'} = $edgepalette1{'Syndel'};
		$gmledge->{'sourceArrow'} = 0;
		$gmledge->{'targetArrow'} = 1;
		$gmledge->{'width'} = 2;
		}
	# del context
	if ($arg==5) 
		{
		$gmledge->{'fill'} = $edgepalette1{'Syndel'};
		$gmledge->{'sourceArrow'} = 1;
		$gmledge->{'targetArrow'} = 0;
		$gmledge->{'width'} = 3;
		}
	# process pair
	if ($arg==6) 
		{
		$gmledge->{'fill'} = $edgepalette1{'ProcessPair'};
		$gmledge->{'sourceArrow'} = 0;
		$gmledge->{'targetArrow'} = 0;
		}
	# cooccurring transformations
	if ($arg==7) 
		{
		$gmledge->{'fill'} = $edgepalette1{'Context'};
		$gmledge->{'sourceArrow'} = 1;
		$gmledge->{'targetArrow'} = 1;
		}
	# onsite context
	if ($arg==13) 
		{
		#$gmledge->{'fill'} = $edgepalette1{'Context'};
		$gmledge->{'sourceArrow'} = 1;
		$gmledge->{'targetArrow'} = 0;
		$gmledge->{'width'} = 2;
		}
	# process graph -activation
	if ($arg==8 )
		{
		$gmledge->{'targetArrow'} = "bezier";
		$gmledge->{'fill'} = $edgepalette1{'Context'};
		}
	# process graph - inhibition
	if ($arg==9)
		{
		#$gmledge->{'sourceArrow'} = "t_shape";
		$gmledge->{'targetArrow'} = "t_shape";
		$gmledge->{'fill'} = $edgepalette1{'Context'};
		}
	# process graph -synthesis
	if ($arg==10 )
		{
		$gmledge->{'targetArrow'} = "bezier";
		$gmledge->{'fill'} = $edgepalette1{'Syndel'};
		}
	# process graph - degradation
	if ($arg==11)
		{
		$gmledge->{'sourceArrow'} = "bezier";
		$gmledge->{'targetArrow'} = "t_shape";
		$gmledge->{'fill'} = $edgepalette1{'Syndel'};
		}
	# process graph - competition
	if ($arg==12)
		{
		$gmledge->{'sourceArrow'} = "t_shape";
		$gmledge->{'targetArrow'} = "t_shape";
		$gmledge->{'fill'} = $edgepalette1{'Context'};
		}
	return;
}

1;
