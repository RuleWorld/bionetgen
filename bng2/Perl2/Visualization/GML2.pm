package Viz;

use strict;
use warnings;
no warnings 'redefine';

# Perl Modules
use Class::Struct;

# BNG Modules
use Visualization::StructureGraph;

struct GMLNode2 =>
{
	'ID' => '$',
	'type' => '$', 
	'label' => '$',
	'isGroup'=>'$',
	'gid'=>'$',
	'embed'=>'$',
	'object'=>'$',
};

struct GMLEdge2 =>
{
	'source' =>'$',
	'target' =>'$',
	'sourceArrow'=>'$',
	'targetArrow'=>'$',
	'object'=>'$',
};

1;