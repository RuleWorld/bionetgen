#!/usr/bin/perl 
# pragmas
package Viz;
use File::Spec;
use warnings;						  
use FindBin;
use Getopt::Long;
use lib 	File::Spec->catdir(
				(	exists $ENV{'BNGPATH'} ? $ENV{'BNGPATH'} :
					( 	exists $ENV{'BioNetGenRoot'} ? $ENV{'BioNetGenRoot'} : 
						do 	{
						my ($vol,$dir,$file) = File::Spec->splitpath( $FindBin::RealBin );
						my @dirs = File::Spec->splitdir($dir);
						pop @dirs;
						pop @dirs;
						File::Spec->catpath($vol,File::Spec->catdir(@dirs))
							} 
					)
				),'Perl2'
			);
use BNGModel;

use lib 	File::Spec->catdir(
				(	exists $ENV{'BNGPATH'} ? $ENV{'BNGPATH'} :
					( 	exists $ENV{'BioNetGenRoot'} ? $ENV{'BioNetGenRoot'} : 
						do 	{
						my ($vol,$dir,$file) = File::Spec->splitpath( $FindBin::RealBin );
						my @dirs = File::Spec->splitdir($dir);
						pop @dirs;
						pop @dirs;
						File::Spec->catpath($vol,File::Spec->catdir(@dirs))
							} 
					)
				),'Perl2\Visualization'
			);
use Viz;
use VisOptParse;

if (scalar (@ARGV) == 0) { display_help(); exit; }

my %args = ('background'=>0, 'collapse'=>0,'groups'=>0,'each'=>0,'textonly'=>0,'suffix'=>'','help'=>0) ;

GetOptions(	\%args,
			'help!',
			'bngl=s',
			'type=s',
			'opts=s@',
			'background!',
			'collapse!',
			'each!',
			'groups!',
			'textonly!',
			'suffix=s',
		) or die "Error in command line arguments.";

if($args{'help'}) 
{
if(defined($args{'type'}) ) { display_help(\%args); }
else { display_help(); }
exit;
}

if(not defined $args{'type'}) {$args{'type'} = 'regulatory'; }

filecheck($args{'bngl'},'BNGL');
map { filecheck($_,'Opts') } @{$args{'opts'}} if defined $args{'opts'};

my $model = getModel($args{'bngl'});
my $exec_params = getExecParams(\%args);
execute_params($model,$exec_params);


sub filecheck
{
	my $file = shift @_;
	my $type = shift @_;
	if(not defined $file) { print $type." file not defined."; exit;}
	if(not -e $file) { print $type." file ".$file." not found."; exit;}
	print "Found ".$type." file ".$file.".\n";
	return;
}

sub getModel
{
	my $filename = shift @_;
	print "Importing model from ".$filename."\n";
	my %args = ();
	$args{'file'} = $filename;
	$args{'skip_actions'} = 1;
	$args{'action_skip_warn'} = 1;

	#print $filename;
	my $model = BNGModel->new();
	$model->initialize();
	
	#print "Opening file: ".$filename."\n";
	my $err = $model->readModel(\%args) ;
	
	if ($err) { print "ERROR:".$err."\n";}
	
	return $model;
}

sub initializeExecParams
{
	my $toggle = 1;
	my @include = ();
	my @exclude = ();
	my $background = {'toggle'=>$toggle,'include'=>\@include,'exclude'=>\@exclude};
	my $each = 0;
	my $groups = 0;
	my $classes = {};
	my %x = ('background'=>$background,'each'=>$each,'groups'=>$groups,'classes'=>$classes);
	return %x;
}

sub getExecParams
{
	my %args = %{shift @_};
	my %exec_params = initializeExecParams();
	my %toggle;
	my %background;
	my %classes;
	
	foreach my $file(@{$args{'opts'}})
	{
		my ($x,$y,$z) = parseOpts($file);
		my %toggle = %$x;
		my %background = %$y;
		my %classes = %$z;
		
		# do toggles;
		foreach my $key(keys %toggle)
		{
			if($key eq 'background') 
				{ $exec_params{'background'}->{'toggle'} = $toggle{$key};}
			if(has(['each','groups'],$key)) 
				{ $exec_params{$key} = $toggle{$key};}
		}
		# do background
		foreach my $key(keys %background)
		{
			my $ref = $exec_params{'background'}->{$key};
			push2ref($ref,$background{$key});
		}
		foreach my $key(keys %classes)
		{
			my $classes2 = $exec_params{'classes'};
			my @arr = ();
			if(not defined $classes2->{$key}) { $classes2->{$key} = \@arr; }
			push2ref($classes2->{$key},$classes{$key});
		}
		
	}
	
	$exec_params{'type'} = $args{'type'};
	$exec_params{'background'}->{'toggle'} = $args{'background'};
	$exec_params{'each'} = $args{'each'};
	$exec_params{'groups'} = $args{'groups'};
	$exec_params{'collapse'} = $args{'collapse'};
	$exec_params{'textonly'} = $args{'textonly'};
	$exec_params{'suffix'} = $args{'suffix'};
	return \%exec_params;
}

sub push2ref
{
	my $arr = shift @_;
	my $item = shift @_;
	if(ref $item) { push @$arr,@$item; }
	else { push @$arr,$item; }
	return;
}


sub display_help
{
	my $args = @_ ? shift @_ : 0;
	my $str;
	if($args==0) {
	$str .= "\n visualize.pl \n";
	$str .= " ---------------------------------------------/ HELP MENU /----------\n";
	$str .= " SYNOPSIS\n                                                            \n";
	$str .= "   visualize.pl                           show this help menu                   \n";
	$str .= "   visualize.pl -h                                \"                   \n";
	$str .= "   visualize.pl -help                             \"                   \n";
	$str .= "   visualize.pl --type TYPE [OPTIONS]     make visualization of type TYPE       \n";
	$str .= "   visualize.pl --type TYPE --help        show help on a specific visualization \n";
	$str .= "   TYPE = rule_network, rule_pattern, rule_operation or process\n";
	$str .= "\n OPTIONS                                                             \n";
	$str .= "\n File I/O                                                          \n\n";
	$str .= "   --bngl BNGL     use BioNetGen model file                          \n";
	$str .= "   --opts FILE     use Options text file                             \n";
	$str .= "   --textonly      Ouputs Text-only version of visualization (default is GML)\n";
	$str .= "   --suffix STR    Adds suffix to output filename\n";
	$str .= "\n Visualization config                                                  \n\n";
	$str .= "   --type TYPE     create visualization of a certain type \n";
	$str .= "   --background    turn background ON (default OFF)                  \n";
	$str .= "   --groups        turn grouping ON (default OFF)                    \n";
	$str .= "   --each          output each rule/group separately (default OFF)	  \n";
	$str .= "\n try: visualize.pl --type regulatory --help\n";
	
	print $str;
	return;
	}
	
	if($args->{'type'} eq 'regulatory')
	{
	$str .= "\n visualize.pl --type regulatory\n";
	$str .= " ---------------------------------------------/ HELP MENU /----------\n";
	$str .= " PURPOSE\n\n";
	$str .= " Used to generate a regulatory network of rules and atomic patterns.\n";
	$str .= "\n USAGE\n\n";
	$str .= " visualize.pl [--type regulatory] [--background] [--groups [--collapse]] [--each] [OPTIONS] \n\n";
	$str .= " --type is assumed as regulatory if not provided.\n\n";

	$str .= " --background turns background ON (OFF by default). When OFF, some patterns are\n";
	$str .= " determined to be background and removed from network graph. The assignment can\n";
	$str .= " be modified using the options file.\n\n";

	$str .= " --groups turns grouping ON (OFF by default). Patterns are grouped using classes\n";
	$str .= " that are provided in the options file. Rules are grouped automatically based on\n";
	$str .= " pattern relationships.\n\n";
	
	$str .= " --collapse computes a smaller network graph where groups of nodes are replaced\n";
	$str .= " by a single node representing the group. Requires --groups.\n\n"; 

	$str .= " For help on file input and output options try: visualize.pl --help \n";
	print $str;
	return;
	}
	
	print $args->{'type'}." is not a valid type.\n";
	return;
}

