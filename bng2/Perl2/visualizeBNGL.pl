#!/usr/bin/perl 
# pragmas
use strict;
use POSIX qw(/ceil/);
#use warnings;
no warnings 'redefine';

# how to use: perl visualizeBNGL.pl FULL_PATH_TO_BNGLFILE [list of starting numbers for different slices of the model]

use Class::Struct;

use BNGModel;
use RxnRule;

use StructureGraph;
use VisualizationIO;

my $model = BNGModel->new();
$model->initialize();

my $filename = shift @ARGV;
#my $type_of_viz = shift @ARGV;
my $err = $model->readModel({file=>$filename}) ;
if ($err) { print "ERROR:".$err."\n";}


my @splits = split(/[.\\]/,$filename);
pop @splits;

my @rrules = @{$model->RxnRules};

my @numbers;


if (@ARGV) { @numbers = sort {$a <=> $b} @ARGV; } else { @numbers = (scalar @rrules + 1); }

my $i = 0;
my $currentmax = shift @numbers;
if ($currentmax == 1) { $currentmax = shift @numbers};

my $currentslice = 0;
my @rrs = ();
my @rrnames = ();
foreach my $rrule(@rrules)
{
	++$i;
	my $n = scalar @$rrule;
	my $reversible = $n==2 ? " reversible " : " ";
	#print "Processing".$reversible."rule ".$i."\n";
	my $j=-1;
	
	my %h = ("0"=>'f',"1"=>'r');
	foreach my $rr(@$rrule)
	{
		++$j;
		my $name = ($n==2) ? $i.$h{$j} : $i; 
		push @rrs, $rr;
		push @rrnames, $name;
	}
	
	if ($i==$currentmax-1) 
	{
		++$currentslice;
		my $suffix = $currentslice;
		# have to pad this suffix to make sure files are named 001, 002.. do it later...
		my $outfile = join("\\",@splits)."_".$suffix.".gml";
		open( my $fh, ">",$outfile);
		print "Writing GML file... $outfile\n";
		print $fh VisualizationIO::vizBPG(\@rrs,\@rrnames);
		close $fh;
		
		@rrs = ();
		@rrnames = ();
		$currentmax = @numbers ? shift @numbers : scalar @rrules+1;
		if ($currentmax > scalar @rrules+1) { $currentmax = scalar @rrules + 1;}
	}
}



