#!/usr/bin/perl 
# pragmas
use strict;
use POSIX qw(/ceil/);

#use warnings;
no warnings 'redefine';
use Class::Struct;
use List::Util qw(min max sum);
use List::MoreUtils qw( uniq);
use Data::Dumper;

use BNGModel;
use StructureGraph;

use VisualizationIO;
use Visualization;

# use: perl visualize.pl FULL_PATH_TO_BNGL FULL_PATH_TO_CONFIG
# use: perl visualize.pl FULL_PATH_TO_BNGL <suffix> <vistype> [ <range>,... ]

my $line = "1   rules 1-10	 12-25 38 39";
my $model = "";
my $bnglname = shift @ARGV;
my @rrules = getRules($bnglname);
my $commandfile = $ARGV[0];
my $basename = $1 if ($bnglname =~ /^(.*)\.bngl$/);
print "\n\n\n";

if (-e $commandfile) 
{ 
	print "Visualize config file found.\n";
	open (my $fh, '<',$commandfile);
	while (my $line = <$fh>)
	{
		chomp $line;
		processLine($line,$basename,\@rrules);
	}
	close $fh;
} 
else 
{
	print "No visualize config file found or specified.\n";
	if (scalar @ARGV)
	{
	print "Command line options found.\n\n";
	my $command = join(" ",@ARGV);
	processLine($command,$basename,\@rrules);
	}
}




sub getRules
{
	my $filename = shift @_;
	my %args = ();
	$args{'file'} = $filename;
	$args{'skip_actions'} = 1;

	#print $filename;
	my $model = BNGModel->new();
	$model->initialize();
	
	#print "Opening file: ".$filename."\n";
	my $err = $model->readModel(\%args) ;
	if ($err) { print "ERROR:".$err."\n";}
	
	my @rrules = @{$model->RxnRules};
	if ( ! scalar @rrules) { print "ERROR: No rules found in model.\n"; exit;}
	return @rrules;
}


sub processLine
{
	# suffix type list of ranges/numbers
	# suffix type slice list of numbers
	# type = rules rules2 contact influence process
	
	
	my $line = shift @_;
	my $basename = shift @_;
	my @rrules = @{shift @_};
	my $string;
	
	print "Processing line \'".$line."\'.\n";
	my @splits = split (/\s+/,$line);
	my @types = qw(rules_text rules_ rules contact regulatory process);
	my %typehash = map {$_ => 1} @types;
	#@typehash{@types} = (1) x scalar @types;
	
	my $suffix = shift @splits;
	my $type = shift @splits;
	
	if (! $typehash{$type} ) 
	{ print "Error! Bad vis type \'".$type."\'.\n"; return 1;}
	my @rules;
	if (@splits)
	{
		my @numbers = ();
		foreach my $item(@splits) { push @numbers, getRange($item); }
		@numbers = sort {$a <=> $b} uniq(@numbers);
		if (max @numbers > scalar @rrules) { print "Number/Range provided exceeds number of rules."; return;}
		foreach my $num(@numbers) { push @rules,@{$rrules[$num]}; }
	}
	else
	{
		foreach my $rrule(@rrules) { push @rules, @$rrule; }
	}
	
	#print join("\n",map {$_->toString()} @rules);
	if ($type eq 'rules_text') 
	{ 
		my $string = join("\n",map {$_->toString()} @rules);  
		my $outfile = $basename.'_'.$suffix.'.txt';
		print "Writing to file: ".$outfile."\n\n";
		open (my $fh, ">", $outfile);
		print $fh $string;
		close $fh;
		return; 
	}
	my @rsgs = ();
	if ($type eq'rules')
	{
		foreach my $i (0..@rules-1)
		{
		$rsgs[$i] = StructureGraph::makeRuleStructureGraph($rules[$i],$i,$rules[$i]->Name);
		}	
		
		my $rsg = StructureGraph::combine2(\@rsgs);
		$string = Visualization::toGML_rules($rsg);
		

		my $outfile = $basename.'_'.$suffix.'.gml';
		print "Writing to file: ".$outfile."\n\n";
		open (my $fh, ">", $outfile);
		print $fh $string;
		close $fh;
		return; 
	}
		
	#print $string;
	
}

sub getRange
{
	my $string = shift @_;
	
	my @splits = sort {$a <=> $b} split('-',$string);
	if (scalar @splits == 2) 
	{ return (($splits[0]-1)..($splits[1]-1)); }
	return ($string-1);
}

