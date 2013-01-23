package EnergyPattern;
# This class defines an energy pattern which contributes to the overall
#  energy of a Species and influences reaction rates.   --Justin

# pragmas
use strict;
use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;

# BNG Modules
use BNGUtils;
use BNGModel;
use SpeciesGraph;



struct EnergyPattern => 
{		   
    Pattern     => 'SpeciesGraph',    # pattern graph
    Keq         => 'Expression',      # change in free-energy parameter (per match)
    Phi         => 'Expression',      # rate distribution parameter
    SpecMatches => '@'                # number of matches to each species
};


sub readString
{
    my $epatt  = shift;
    my $string = shift;
    my $model  = shift;

    my $err = '';

    my $plist  = $model->ParamList;
    my $clist  = $model->CompartmentList;
    my $mtlist = $model->MoleculeTypesList;

    # Check if first token is an index
    if ($string=~ s/^\s*\d+\s+//)
    {
        # This index will be ignored
    }

    # strip any leading whitesace
    $string =~ s/^\s+//;

    # Next read the SpeciesGraph that will define the Energy Pattern
    my $sep = '^\s+';
    my $sg = SpeciesGraph->new();
    $err = $sg->readString( \$string, $clist, 0, $sep, $mtlist, 0 );
    if ($err) {  $err = "While reading Energy Pattern: $err";  return ($err);  }

    $epatt->Pattern( $sg );

    # Look for DeltaG expression
    my $Keq_expr = Expression->new();    
    if ( $string=~ /\S+/ )
    {
        # Read expression
        $err = $Keq_expr->readString( \$string, $plist );
        if ( $err ) { return (undef, $err); }
    }
    else
    {
        my $default_Keq = '0';
        $err = $Keq_expr->readString( \$default_Keq, $plist );
        if ( $err ) { return (undef, $err); }        
    }
    # set DeltaG expression
    $epatt->Keq( $Keq_expr );    


    # Look for Phi expression
    my $phi_expr = Expression->new();    
    if ( $string=~ /\S+/ )
    {
        # Remove separator for backward compatibility
        $string =~ s/^\s*[,]\s*//;
        # Read expression
        $err = $phi_expr->readString( \$string, $plist );
        if ( $err ) { return (undef, $err); }       
    }
    else
    {
        my $default_phi = '1/2';
        my $phi_expr = Expression->new();
        $err = $phi_expr->readString( \$default_phi, $plist );
        if ( $err ) { return (undef, $err); }        
    }
    # set Phi expression
    $epatt->Phi( $phi_expr );
    
    
    # set SpecMatches and RxnStoich to empty array
    $epatt->SpecMatches( [] );
    
    # return with error 
    return $err;
}



sub toString
{
    my $epatt = shift;
    my $plist = (@_) ? shift : undef;
    my $string = '';

    $string .=  $epatt->Pattern->toString() . "  "
              . $epatt->Keq->toString($plist) . ", "
              . $epatt->Phi->toString($plist);

    return $string;
}



sub toMatlabString
{
   my $epatt = shift;
   my $string = '';
   
   # TODO
     
   return $string, '';
}



sub toMexString
{
    my $epatt = shift;
    my $string = '';
   
    # TODO
   
    return $string, '';
}



sub toXML
{
    my $epatt  = shift;
    my $indent = shift;
    my $index  = shift;
    
    my $string = '';
    
    # TODO
    
    return $string, '';
}



sub toMathMLString
{
    my $epatt  = shift;
    my $string = '';

    # TODO

    return $string, '';
}


# reset the observable weights to zero
sub reset_weights
{
    my $epatt   = shift @_;
    my $alloc = @_ ? shift @_ : 0;
    $epatt->Weights( [(0) x $alloc] );
}



sub update
{
    my $epatt   = shift @_;
    my $species = shift @_;

    my $err = undef;
    # generate matches to species
    foreach my $spec (@$species)
    {
        my @matches = $epatt->Pattern->isomorphicToSubgraph($spec->SpeciesGraph);
        $epatt->SpecMatches->[ $spec->Index ] = scalar @matches;
    }
    return $err;
}



sub getStoich
# returns an integer value corresponding to the stoichiometry of
#  this pattern w.r.t. a given reaction.
{
    my $epatt = shift;
    my $rxn   = shift;
    
    my $err = '';
 
    my $stoich = 0;
    foreach my $reactant (@{$rxn->Reactants})
    {
        if ( defined $epatt->SpecMatches->[$reactant->Index] )
        {
            $stoich -= $epatt->SpecMatches->[$reactant->Index];
        }
    }
    foreach my $product (@{$rxn->Products})
    {
        if ( defined $epatt->SpecMatches->[$product->Index] )
        {
            $stoich += $epatt->SpecMatches->[$product->Index];
        }    
    }

    
    return $stoich, $err;
}



sub getRateExpression
{
    my $epatt  = shift;
    my $rxn    = shift;
    my $plist  = (@_) ? shift : undef;
    
    my $err = '';
    my $stoich;
 
    # get stoichiometry of this pattern w.r.t. this rxn
    ($stoich, $err) = $epatt->getStoich( $rxn );
    if ( $err ) {  return undef, $err;  }
    
    # construct a rate constant expression
    my $rate_expr = undef;
    if ( $stoich )
    {    
        my $rate_string;
        $rate_expr = Expression->new();
        if    ( $stoich > 0 )
        {
            if ( $stoich == 1 )
            {
                $rate_string = "(" . $epatt->Keq->toString($plist) . ")^("
                                   . $epatt->Phi->toString($plist) . ")" ;               
            }
            else
            {
                $rate_string = "(" . $epatt->Keq->toString($plist) . ")^($stoich*("
                                   . $epatt->Phi->toString($plist) . "))" ;   
            }
        }
        elsif ( $stoich < 0 )
        {
            if ( $stoich == -1 )
            {
                $rate_string = "(" . $epatt->Keq->toString($plist) . ")^("
                                   . $epatt->Phi->toString($plist) . "-1)" ;               
            }
            else
            {
                $rate_string = "(" . $epatt->Keq->toString($plist) . ")^(abs($stoich)*("
                                   . $epatt->Phi->toString($plist) . "-1))" ;   
            }
        } 
        $err = $rate_expr->readString( \$rate_string, $plist );  
        if ( $err ) { return undef, $err; }
    }
    
    return $rate_expr, $err;
}


1;
