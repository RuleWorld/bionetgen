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

my %args = ('type'=>'rule_network', 'background'=>0, 'collapse'=>0,'groups'=>0,'each'=>0,'textonly'=>0,'suffix'=>'') ;

GetOptions(	\%args,
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
#