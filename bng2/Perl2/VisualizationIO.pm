# VisualizationIO.pm, 
# Input and output for viz tools
# Author: John Sekar (johnarul.sekar@gmail.com)

package VisualizationIO;
# pragmas
use strict;
use warnings;
no warnings 'redefine';

# Perl Modules
use Class::Struct;
use List::Util qw(min max sum);
use Data::Dumper;

# BNG Modules
use StructureGraph;
use BipartiteGraph;

sub GMLNode
{
	my $id = shift @_;
	my $label = shift @_;
	my $nodestyle = shift @_;
	my $labelstyle = shift @_;
	#my $shape = shift @_;
	#my $fill = shift @_;
	my $gid = @_ ? shift @_: undef;
	my $isgroup = @_ ? shift @_: undef;
	
	my $string = sprintf "id %d label \"%s\" ",$id,$label;
	#$string .= sprintf "graphics [ type \"%s\" fill \"%s\" ] ",$shape,$fill;
	$string .= $nodestyle." ";
	#$string .= sprintf "LabelGraphics [ label \"%s\" anchor \"t\" ] ",$label;
	$string .= $labelstyle." ";
	if (defined $isgroup) { $string .= " isGroup 1"; }
	if (defined $gid) { $string .= sprintf " gid %d", $gid;}
	$string = " node [ ".$string." ]";
	return $string;
}

sub GMLEdge
{
	my $source = shift @_;
	my $target = shift @_;
	my $isdirected = @_ ? shift @_: 0;
	my $string = sprintf "source %d target %d ",$source,$target;
	if ($isdirected)
		{ $string .= "graphics [ fill \"#000000\" targetArrow \"standard\" ] ";}
	else
		{ $string .= "graphics [ fill \"#000000\" ] ";}
	$string = " edge [ ".$string." ]";
	return $string;
}

sub toGML_yED
{
	my $sg = shift @_;

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
	my @bondnodes = grep ( { $_->{'Type'} eq 'BondState' } @nodelist);
	my @graphopnodes = grep ( { $_->{'Type'} eq 'GraphOp'} @nodelist);
	my @rulenodes = grep ( {$_->{'Type'} eq 'Rule'} @nodelist);
	
	# hashes for isgroup and gid
	my %isgroup;
	my %gid;

	foreach my $node(@structnodes) 
	{
		if ($node->{'Type'} eq 'Rule') { $isgroup{$node->{'ID'}} = 1; }
		if ($node->{'Type'} eq 'Mol') { $gid{$node->{'ID'}} = $node->{'Rule'}; }
		if ($node->{'Parents'})
		{
			foreach my $parent_id(@{$node->{'Parents'}}) 
			{
				$isgroup{$parent_id} = 1; 
				$gid{$node->{'ID'}} = $parent_id;
			}
		}
	}
	foreach my $node(@graphopnodes)
	{
		if ($node->{'Name'} eq 'ChangeState' )
		{
			my @parents = @{$node->{'Parents'}};
			my $parent = StructureGraph::findNode(\@nodelist,$parents[0]);
			my @grandparents = @{$parent->{'Parents'}};
			my $grandparent = $grandparents[0];
			$gid{$node->{'ID'}} = $grandparents[0];
		}
		else
		{
			$gid{$node->{'ID'}} = $node->{'Rule'};
		}
	}
	foreach my $node(@bondnodes)
	{
		if (scalar @{$node->{'Parents'}} == 1)
			{
			$gid{$node->{'ID'}} = $node->{'Rule'};
			}
	}
	my @nodestrings = ();
	my @edgestrings = ();
	# make node strings
	foreach my $node(@nodelist)
	{
		# ignore if it is a bond with two parents
		if ( $node->{'Type'} eq 'BondState' and scalar @{$node->{'Parents'}} == 2) { next; }
		
		my $id = $node->{'ID'};
		my $nm = $node->{'Name'};
		my $type = $node->{'Type'};
		my $nodestyle =  styling('node',$type,$nm,$isgroup{$id});
		my $labelstyle = styling('nodelabel',$type,$nm,$isgroup{$id});
		my $string = GMLNode($id,$nm,$nodestyle,$labelstyle,$gid{$id},$isgroup{$id});
		push @nodestrings, $string;
	}
	# make edgestrings for bonds
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
		else { next; }
		my $string = GMLEdge($source,$target);
		push @edgestrings,$string;
	}
	# make edgestrings for edges adjacent to graph operation nodes
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
			my $string = GMLEdge($id2,$id,1);
			push @edgestrings,$string;
		}
		
		foreach my $id2(@produced)
		{
			my $string = GMLEdge($id,$id2,1);
			push @edgestrings,$string;
		}
	}

	my $string = "graph\n[\n directed 1\n";
	$string .= join("\n",@nodestrings)."\n";
	$string .= join("\n",@edgestrings)."\n";
	$string .= "]\n";
	
	return $string;
}

sub vizRules
{
	# input is an array of rules
	my @rules = @{shift @_};
	my @rulenames = @{shift @_};
	
	my $n = scalar @rules;
	my @rsgs = () x $n;
	foreach my $i(0..$n-1)
		{
		my $rr = $rules[$i];
		my $name = $rulenames[$i];
		$rsgs[$i] = StructureGraph::makeRuleStructureGraph($rr,$i,$name);
		}
	my $rsg = StructureGraph::combine2(\@rsgs);
	my $string = toGML_yED($rsg);
	return $string;
}
sub styling
{
	my $attrib = shift @_; # what attribute is required graphics/LabelGraphics
	my $type = shift @_; # what node type the attribute is required for
	my $label = shift @_;
	my $isgroup = shift @_;
	
	my %shape = ( 'Mol'=>'roundrectangle', 'Comp'=>'roundrectangle', 
	'CompState'=>'roundrectangle', 'BondState'=>'roundrectangle', 
	'GraphOp'=>'hexagon','Rule'=>'roundrectangle');
	my %fill = ( 'Mol'=>"#FFFFFF", 'Comp'=>"#D2D2D2", 
	'CompState'=>"#FFCC00", 'BondState'=>"#D2D2D2", 
	'GraphOp'=>"#CC99FF",'Rule'=>"#FFFFFF");
	my %outlineStyle = ('Rule'=>"dotted");
	my %anchor = ( '1'=>"t", '0'=>"c");
	my %fontstyle = ( 'Mol'=>"bold", 'GraphOp'=>"italic");
	
	
	my $string = "";
	my $q1 = " \"";
	my $q2 = "\" ";
	my $sp = " ";
	if ($attrib eq 'node')
	{
		$string .= "type".$q1.$shape{$type}.$q2;
		$string .= "fill".$q1.$fill{$type}.$q2;
		if ($type eq 'Rule')
		{ $string .= "outlineStyle".$q1."dotted".$q2; }
		$string = "graphics [ ".$string." ]";
		
	}
	elsif ($attrib eq 'nodelabel')
	{
		$string .= "label".$q1.$label.$q2;
		$isgroup = $isgroup ? 1 : 0;
		$string .= "anchor".$q1.$anchor{$isgroup}.$q2;
		if (exists $fontstyle{$type})
		{ $string .= "fontStyle".$q1.$fontstyle{$type}.$q2;}
		$string = "LabelGraphics [ ".$string." ]";
	}	
	return $string;	
}


sub vizBPG
{
	# input is an array of rules
	my @rules = @{shift @_};
	my @rulenames = @{shift @_};
	
	my $n = scalar @rules;
	my @rsgs = () x $n;
	my @bpgs = () x $n;
	foreach my $i(0..$n-1)
		{
		my $rr = $rules[$i];
		my $name = $rulenames[$i];
		$rsgs[$i] = StructureGraph::makeRuleStructureGraph($rr,$i,$name);
		$bpgs[$i] = BipartiteGraph::makeRuleBipartiteGraph($rsgs[$i]);
		}
		
	my $bpg = BipartiteGraph::combine(@bpgs);
	my $string
	### needs filling
	return $string;
}
1;