#!/usr/bin/perl 
# pragmas
use strict;
#use warnings;
no warnings 'redefine';

# how to use: perl visualizeBNGL.pl FULL_PATH_TO_BNGLFILE

use Class::Struct;

use BNGModel;
use RxnRule;

use StructureGraph;
use VisualizationIO;

my $model = BNGModel->new();
$model->initialize();
my $filename = $ARGV[0];
my $err = $model->readModel({file=>$filename}) ;

my @splits = split(/[.\\]/,$filename);
pop @splits;
my $outfile = join("\\",@splits).".gml";

my @rrules = @{$model->RxnRules};
my @rrs = ();
foreach my $rrule(@rrules)
{
	foreach my $rr(@$rrule)
	{
		push @rrs, $rr;
	}
}

open( my $fh, ">",$outfile);
print $fh VisualizationIO::vizRules(\@rrs);
close $fh;


