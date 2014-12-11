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
use Visualization::Viz;
use Visualization::VisOptParse;

if (scalar (@ARGV) == 0) { display_help(); exit; }

my %args = ('help'=>0,'background'=>0,'collapse'=>0,'each'=>0,'groups'=>0,'textonly'=>0,'suffix'=>'','filter'=>0,'level'=>1,'mergepairs'=>0,'embed'=>0) ;

GetOptions(	\%args,
			'help!',
			'bngl=s',
			'db=s',
			'type=s',
			'opts=s@',
			'background!',
			'collapse!',
			'each!',
			'groups!',
			'textonly!',
			'suffix=s',
			'filter!',
			'level=i',
			'mergepairs!',
			'embed!',
		) or die "Error in command line arguments.";
		
if(defined $args{'db'})
{
	my $name = $args{'db'};
	$name =~ /(.*)\.db/;
	my $prefix = $1;
	
	filecheck($name,'DB');
	my $dbh = newDBFile($name);
	my $background = $args{'background'};
	my $groups = $args{'groups'};
	my $collapse = $args{'collapse'};
	my $bpg = getBPG($dbh,$background,$groups,$collapse);
	my $str = toGML_rule_network($bpg);
	my $suffix = $args{'suffix'};
	my $type = 'regulatory';
	my %params = ('str'=>$str,'suffix'=>$suffix,'type'=>$type,'prefix'=>$prefix);
	writeGML2(\%params);
	exit;
}

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
	if(not defined $file) { print $type." file not defined.\n"; exit;}
	if(not -e $file) { print $type." file ".$file." not found.\n"; exit;}
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
		$str .= "\n OPTIONS SET ONE\n\n";
		$str .= " visualize.pl [--type regulatory] [--background] [--opts FILE] [--filter [--level INT]] [--groups] \n\n";
		$str .= " --type is assumed as regulatory if not provided.\n";
		$str .= " --background turns background ON (OFF by default). When OFF, some patterns are\n";
		$str .= " determined to be background and removed from network graph. The assignment can\n";
		$str .= " be modified using the options file.\n";
		$str .= " --filter generates a subgraph starting from a defined set of nodes and \n";
		$str .= " propagating along the edges INT levels deep (default 1). The starting nodes are\n";
		$str .= " defined using the options file.\n";
		$str .= " --groups turns grouping ON (OFF by default). Patterns are grouped using classes\n";
		$str .= " that are provided in the options file. Rules are grouped automatically based on\n";
		$str .= " pattern relationships.\n";
		$str .= " --opts FILE inputs an option file with the following structure:\n\n";
		$str .= "\tbegin background \n\t\t <atomic patterns>\n\tend background\n\n";
		$str .= "\tbegin filter \n\t\t<atomic patterns>\n\tend filter\n\n";
		$str .= "\tbegin classes \n\t\t< begin classname \n\t\t\t<atomic patterns>\n\t\tend classname >\n\tend classes\n\n";
		$str .= " <object> indicates whitespace\\return separated list of objects. All blocks are optional.\n";
		$str .= " classname refers to arbitrary names for pattern classes.\n";
		
		$str .= "\n OPTIONS SET TWO\n\n";
		$str .= " visualize.pl --groups [--collapse] [--textonly]   \n\n";
		$str .= " --collapse computes a smaller network graph where groups of nodes are replaced\n";
		$str .= " by a single node representing the group. Requires --groups.\n"; 
		$str .= " --textonly provides a text-only version of the model network graph.\n";
		
		$str .= "\n OPTIONS SET THREE\n\n";
		$str .= " visualize.pl [--groups] --each   \n\n";
		$str .= " --each prints out each rule or rule group (if --groups is present).\n\n";
		
		$str .= " Option sets TWO and THREE are incompatible with each other, but compatible with ONE.\n";
		$str .= " Option sets TWO and THREE are incompatible with process graph (see --type process).\n\n";
		$str .= " For help on file input and output options try: visualize.pl --help \n";
		$str .= " To start working on a model try:\n\n";
		$str .= " visualize.pl --bngl BNGL --type regulatory --groups \n";
		print $str;
		return;
	}
	
	if($args->{'type'} eq 'process')
	{
		$str .= "\n visualize.pl --type process\n";
		$str .= " ---------------------------------------------/ HELP MENU /----------\n";
		$str .= " PURPOSE\n\n";
		$str .= " Used to generate a regulatory network of rules and atomic patterns.\n";
		$str .= "\n USAGE\n\n";
		$str .= " visualize.pl --type process [--textonly] [--background] [--opts FILE] [--groups [--mergepairs]]\n\n";
		$str .= " --type is assumed as regulatory if not provided.\n";
		$str .= " --textonly provides a text-only version of the model network graph.\n";
		$str .= " --background turns background ON (OFF by default). When OFF, some patterns are\n";
		$str .= " determined to be background and removed from network graph. The assignment can\n";
		$str .= " be modified using the options file.\n";
		$str .= " --groups turns grouping ON (OFF by default). Patterns are grouped using classes\n";
		$str .= " that are provided in the options file. Rules are grouped automatically based on\n";
		$str .= " pattern relationships.\n";
		$str .= " --opts FILE inputs an option file with the following structure:\n\n";
		$str .= "\tbegin background \n\t\t <atomic patterns>\n\tend background\n\n";
		$str .= "\tbegin filter \n\t\t<atomic patterns>\n\tend filter\n\n";
		$str .= "\tbegin classes \n\t\t< begin classname \n\t\t\t<atomic patterns>\n\t\tend classname >\n\tend classes\n\n";
		$str .= " <object> indicates whitespace\\return separated list of objects. All blocks are optional.\n";
		$str .= " classname refers to arbitrary names for pattern classes.\n";
	
		$str .= " visualize.pl --bngl BNGL --type process --groups \n";
		print $str;
		return;
	}
	
	print $args->{'type'}." is not a valid type.\n";
	return;
}

