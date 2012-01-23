package Population;
# an object for managing population species:
#   maps a species' graph to its representative population molecule
 
# pragmas
use strict;
use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;

# BNG Modules
use BNGUtils;
use Molecule;
use SpeciesGraph;
use RxnRule;
use BNGModel;




struct Population =>
{
    Species      => 'SpeciesGraph',
    Population   => 'SpeciesGraph',
    MappingRule  => 'RxnRule'
};

##
##

sub newPopulation
{
    # get input arguments
    my $string = shift;
    my $model = shift;
    my $index = shift;
    
    # define return arguments
    my $err = undef;
    my $pop = undef;


    # parse population mapping rule
    my $rr;
    ($err, $rr) = RxnRule::newPopulationMappingRule( $string, $model );
    if ($err) {  return ($err, $pop);  }
    
    unless ( defined $rr->Name )
    {   # give map rule a name
        $rr->Name( 'MapRule' . $index );
    }
    
    # define population
    $pop = Population->new();
    $pop->Species( $rr->Reactants->[0] );
    $pop->Population( $rr->Products->[0] );
    $pop->MappingRule( $rr );
    
    # return normally
    return ($err, $pop); 
};



1;
