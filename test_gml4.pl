#!/usr/bin/perl
use strict;
use warnings;
use lib 'bng2/Perl2';
use Visualization::GML;
use Visualization::ParseGraph;

my $psg = stringToPatternStructureGraph("A(b!1).B(a!1)", 0);
my $gml_string = toGML_pattern($psg, "gml");
print "GML output:\n$gml_string\n";
