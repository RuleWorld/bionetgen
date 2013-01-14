package Species;

# pragmas
use strict;
use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;

# BNG Modules
use SpeciesGraph;



struct Species =>
{
    SpeciesGraph        => 'SpeciesGraph',
    Concentration       => '$',              # this will either be a number or a parameter name (I think)
    Index               => '$',		  
    RulesApplied        => '$',
    ObservablesApplied  => '$',
};



###
###
###


sub toXML
{
    my $spec = shift @_;
    my $indent = @_ ? shift @_ : "";
    my $conc   = @_ ? shift @_ : $spec->Concentration;
    my $id = @_ ? shift : "S".$spec->Index;  # what's this for??

    my $type = 'Species';
    my $attributes = '';

    # Attributes
    # concentration
    $attributes .= " concentration=\"" . $conc . "\"";
    # name
    $attributes .= " name=\"" . $spec->SpeciesGraph->toString() . "\"";

    # Objects contained
    my $string = $spec->SpeciesGraph->toXML($indent,$type,$id,$attributes);

    return $string;
}


###
###
###


sub getCVodeName
{
    my $species = shift;
    my $offset = -1;
    return 'NV_Ith_S(species,' . ($species->Index + $offset). ')';
}

sub getCVodeDerivName
{
    my $species = shift;
    my $offset = -1;
    return 'NV_Ith_S(Dspecies,' . ($species->Index + $offset). ')';
}


###
###
###


sub getMatlabName
{
    my $species = shift;
    my $offset  = 0;
    return 'species(' . ($species->Index + $offset). ')';
}

sub getMatlabDerivName
{
    my $species = shift;
    my $offset  = 0;
    return 'Dspecies(' . ($species->Index + $offset). ')';
}


###
###
###

1;
