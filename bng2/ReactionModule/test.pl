#!/usr/bin/perl

# pragmas
use strict;
use warnings;

print "Goodbye Cruel World\n";

# Perl Modules
use FindBin;
use lib $FindBin::Bin;
use IO::Handle;
use File::Spec;
use Class::Struct;

use ReactionModule;


BEGIN
	{
	# add Perl2 subdirectory to Module seach path
	{
	    my ($volume,$directories,$file) = File::Spec->splitpath( $FindBin::Bin );
	    my @dirs = File::Spec->splitdir( $directories );
	    pop @dirs;
	    push @dirs,'Perl2' ;
	    unshift @INC, File::Spec->catdir( $volume, File::Spec->catdir( @dirs ) );
	}
	#	print join("\n",@INC);
	}

use BNGModel;

# BNG Modules
use BNGUtils;
use MoleculeTypesList;
use MoleculeType;
use ParamList;
use Function;
use Compartment;
use CompartmentList;
use SpeciesList;
use RxnRule;
use EnergyPattern;
use Observable;
use PopulationList;

struct ReactionModule =>
{
	Name 				=> '$',
	IsBasic 			=> '$',
	MoleculeTypesList	=> 'MoleculeTypesList'
};

#struct ReactionModule =>
#{
#	Name                => '$',
#	Time                => '$',
#	Concentrations      => '@',
#	MoleculeTypesList   => 'MoleculeTypesList',
#	SpeciesList         => 'SpeciesList',
#	SeedSpeciesList     => 'SpeciesList',
#	RxnList             => 'RxnList',
#	RxnRules            => '@',
#	ParamList           => 'ParamList',
#	Observables         => '@',
#	EnergyPatterns      => '@',  # energyBNG: Holds a list of energy patterns  --Justin
#	CompartmentList     => 'CompartmentList',    # list of reaction compartments (volumes and surfaces)
#	PopulationTypesList => 'MoleculeTypesList',  # list of population molecule types
#	PopulationList      => 'PopulationList',     # list of population species
#	SubstanceUnits      => '$',
#	UpdateNet           => '$',  # This variable is set to force update of NET file before simulation.
#	Version             => '@',  # Indicates set of version requirements- output to BNGL and NET files
#	Options             => '%',  # Options used to control behavior of model and associated methods
#    Params              => '%',  # run-time parameters (not to be saved)
#};




