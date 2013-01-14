# $Id: Rxn.pm,v 1.9 2007/07/06 04:46:32 faeder Exp $

package Rxn;

# pragmas
#use strict;
#use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;

# BNG Modules
use BNGUtils;
use SpeciesGraph;
use EnergyPattern;



struct Rxn => {
  Reactants      => '@',          # Array of reactant Species
  Products       => '@',          # Array of product Species
  RateLaw        => 'RateLaw',
  StatFactor     => '$',	       
  Priority       => '$',
  RxnRule        => '$',
  Index          => '$',          # Reaction Index for writing network output
};


sub toString
{
    my $rxn   = shift;
    my $text  = (@_) ? shift : 0;
    my $plist = (@_) ? shift : 0;
  
    my $err;
    my $string;
  
    if ($text)
    {
        my @rstrings=();
        my @pstrings=();
        foreach my $r ( @{$rxn->Reactants} )
        {   push @rstrings, $r->SpeciesGraph->toString();   }
        foreach my $p ( @{$rxn->Products} )
        {   push @pstrings, $p->SpeciesGraph->toString();   }
        $string = join(' + ', @rstrings) . " -> " . join(' + ', @pstrings);
    }
    else
    { 
        $string = $rxn->stringID(); 
    }

    $string .= ' ';


    # Write the Ratelaw...
    #   First prcoess reaction multipliers (statistical factor, compartment volumes, etc)
    my $err = undef;
    my $rxn_mult = undef;
    
    # handle the statistical factor
    if ( defined( $rxn->StatFactor ) and ($rxn->StatFactor ne '') and ($rxn->StatFactor ne '1') )
    {   $rxn_mult = Expression::newNumOrVar( $rxn->StatFactor );   }
  
    # handle the volume expression
    (my $volume_expr, $err) = $rxn->volume_scale($plist);    
    if ( defined $volume_expr )
    {
        if ( defined $rxn_mult )
        {
            $rxn_mult = Expression::operate( '*', [($rxn_mult, $volume_expr)], $plist );
        }
        else
        {   $rxn_mult = $volume_expr;   }
    }

    # get ratelaw string
    $string .= $rxn->RateLaw->toString( $rxn_mult, 1, $plist );

    if ($rxn->RxnRule)
    {  $string .= " #" . $rxn->RxnRule->Name;  }

    return $string;
}


###
###
###



sub getCVodeName
{
    my $rxn = shift;
    return 'NV_Ith_S(ratelaws,' . $rxn->Index . ')'
}


###
###
###


sub getMatlabName
{
    my $rxn = shift;
    my $offset = 1;
    return 'ratelaws(' . ($rxn->Index + $offset) . ')';
}


###
###
###

sub getCVodeRate
{
    my $rxn   = shift;
    my $plist = shift;

    my $err;
    
    # get multiplier factors
    my $rxn_mult = undef;
    
    # handle the statistical factor
    if ( defined( $rxn->StatFactor ) and ($rxn->StatFactor ne '') and ($rxn->StatFactor ne '1') )
    {   $rxn_mult = Expression::newNumOrVar( $rxn->StatFactor );   }
  
    # handle the volume expression
    (my $volume_expr, $err) = $rxn->volume_scale($plist);    
    if ( defined $volume_expr )
    {
        if ( defined $rxn_mult )
        {
            $rxn_mult = Expression::operate( '*', [($rxn_mult, $volume_expr)], $plist );
        }
        else
        {   $rxn_mult = $volume_expr;   }
    }

    # get reference to RxnRule RRef hash
    my $rrefs = undef;
    if ( $rxn->RxnRule )
    {   $rrefs = $rxn->RxnRule->RRefs;   }
    # get ratelaw string    
    return $rxn->RateLaw->toCVodeString( $rxn_mult, $rxn->Reactants, $rrefs, $plist );
}


###
###
###


sub getMatlabRate
{
    my $rxn   = shift;
    my $plist = shift;

    my $err;
    
    # get multiplier factors
    my $rxn_mult = undef;
    
    # handle the statistical factor
    if ( defined( $rxn->StatFactor ) and ($rxn->StatFactor ne '') and ($rxn->StatFactor ne '1') )
    {   $rxn_mult = Expression::newNumOrVar( $rxn->StatFactor );   }
  
    # handle the volume expression
    (my $volume_expr, $err) = $rxn->volume_scale($plist);    
    if ( defined $volume_expr )
    {
        if ( defined $rxn_mult )
        {
            $rxn_mult = Expression::operate( '*', [($rxn_mult, $volume_expr)], $plist );
        }
        else
        {   $rxn_mult = $volume_expr;   }
    }

    # get reference to RxnRule RRef hash
    my $rrefs = undef;
    if ( $rxn->RxnRule )
    {   $rrefs = $rxn->RxnRule->RRefs;   }

    # get ratelaw string  
    return $rxn->RateLaw->toMatlabString( $rxn_mult, $rxn->Reactants, $rrefs, $plist );
}


###
###
###



# Used to compare whether reactions are identical
#  (only in terms of species involved)
sub stringID
{
    my $rxn = shift;
    my $string = '';
  
    # Prior to 2.1.7, a reaction with zero reactants or zero products produced an
    # empty string for the respective field. Network2 does not recognize the null
    # string, leading to a parsing error. To resolve this issue, a null reactant (or product)
    # indicated by the index "0" will be output for the reactant (resp. product) field
    # if a reaction has zero reactants (or products).  --Justin, 29oct2010
  
    # QUESTION: will this effect sorting or other uses of this method?
  
    # get reactant indices
    my @rstrings=();
    if ( @{$rxn->Reactants} )
    {   foreach my $r (@{$rxn->Reactants}) { push @rstrings, $r->Index; }   }
    else
    {   push @rstrings, "0";   }
  
    # get product indices       
    my @pstrings=();
    if ( @{$rxn->Products} )
    {   foreach my $p (@{$rxn->Products}) { push @pstrings, $p->Index; };   }
    else
    {   push @pstrings, "0";   }
  
    # sort reactants and products (if ratelaw is elementary or zero-order)
    my $type= $rxn->RateLaw->Type;
    if ( ($type eq 'Ele') or ($type eq 'Zero') )
    {
        @rstrings = sort {$a<=>$b} @rstrings;
        @pstrings = sort {$a<=>$b} @pstrings;
    }
    
    $string .= join(',', @rstrings) . " " . join(',', @pstrings);
    return ($string);
}



sub volume_scale
# ($vol_expr, $err) = $rxn->volume_scale($plist)
# Construct volume scaling expression for compartment reactions.
#  Returns undefined if the volume scaling expression is '1'.
#
# NOTE: Should this be moved to RateLaw.pm?

# Handle volume-dependent rate constants for compartment reactions.
#   justinshogg@gmail.com  23feb2009
#
# we assume state variables are species counts (not concentrations) and
# that user has chosen consistent units for reaction constants which are
# independent of compartment volumes.
#
# for bi-molecular reactions, the reaction compartment is the 3-D volume [V]
# unless all reactants are at a 2-D surface [S].
#
#  rxn type                adjustment
#  ----------------------------------------------------
#   S                      none
#   V                      none
#   S + S                  /S
#   S + V                  /V
#   V + V                  /V 
#   S + S + S              /S/S
#   S + S + V              /S/V
#   S + V + V              /V/V
#   V + V + V              /V/V            etc...
#   0 -> S                 S  ??
#   0 -> V                 V  ??

{
    my $rxn = shift;
    my $plist = (@_) ? shift : undef;

    my $err;
    my $vol_expr = undef;

    # get all the defined compartments
    my @reactant_compartments = grep {defined $_} (map {$_->SpeciesGraph->Compartment} @{$rxn->Reactants});
    my @product_compartments  = grep {defined $_} (map {$_->SpeciesGraph->Compartment} @{$rxn->Products});
   
    # return undefined volume expr if there are no compartments
    if ( @reactant_compartments )
    { 
        # divide into surfaces and volumes
        my @surfaces = ( grep {$_->SpatialDimensions==2} @reactant_compartments );
        my @volumes  = ( grep {$_->SpatialDimensions==3} @reactant_compartments );

        # Pick and toss an anchor reactant.  If there's a surface reactant, toss it.
        # Otherwise toss a volume.
        (@surfaces) ? shift @surfaces : shift @volumes;

        # if there are surfaces or volumes remaining, we need to define a volume expression
        if ( @surfaces  or  @volumes )
        {
            # construct the volume expression
            my @vol_factors = (1.0);
            push @vol_factors, ( map {$_->Size} (@surfaces, @volumes) );
            $vol_expr = Expression::operate( '/', \@vol_factors, $plist );
            unless ( defined $vol_expr )
            {   $err = "Error in Rxn::volume_scale(): Some problem defing compartment volume expression.";  }
        }
    }
    elsif ( @product_compartments>0 )
    {
        # check if products are in the same compartment
        my $consistent = 1;
        my $comp1 = $product_compartments[0];
        foreach my $comp2 ( @product_compartments[1..$#product_compartments] )
        {
            unless ($comp1 == $comp2)
            {
                $consistent = 0;
                last;
            }
        }

        if ($consistent)
        {   # construct the volume expression
            $vol_expr = $comp1->Size;
        }
        else
        {   send_warning("compartmental BioNetGen doesn't know how to handle zero-order synthesis of products in multiple compartments.");  }
    }
    
    # return the expression (possibly undefined) and the error msg (if any).
    return ($vol_expr, $err);
}



# For energyBNG only!!  --Justin, 9nov2010
# When a rxn is created, it inherits a generic elementary rateLaw from its parent RxnRule.
# Each specific rxn must have a ratelaw that is compatible with the stoichiometry of 
# energy patterns.  This method constructs a ratelaw (specific to this rxn) that will
# guarantee energy compatibility (detailed balance, etc).
sub updateEnergyRatelaw
{
    my $rxn    = shift;
    my $model  = shift;
    
    my $epatts = $model->EnergyPatterns;
    my $plist = $model->ParamList;
    
    my $err = '';
    
    # get energy stoichiometry fingerprint
    my $fingerprint = $rxn->getEnergyFingerprint( $epatts );
     
    #  lookup fingerprint in RateLaw hash
    if ( exists $rxn->RateLaw->EnergyHash->{ $fingerprint } )
    {
        # assign ratelaw matching fingerprint
        $rxn->RateLaw( $rxn->RateLaw->EnergyHash->{ $fingerprint } );
    }
    else
    {
        # construct a new ratelaw and assign to this fingerprint   
        my @rate_strings = ();
        # get the rate constant expression from the generic ratelaw
        push @rate_strings, '('. $rxn->RateLaw->Constants->[0] .')';
        # now add terms to account for changes in energy pattern counts
        foreach my $epatt ( @$epatts )
        {
            (my $expr_term, $err) = $epatt->getRateExpression( $rxn, $plist );
        
            if ( defined $expr_term )
            {   push @rate_strings, '('. $expr_term->toString( $plist ) .')';   }
        }
        # the customized rate is calculated as the multiplication of all the above terms.
        my $rate_string = join( '*', @rate_strings );   
        # construct the updated rate law
        (my $updated_rate_law) = RateLaw::newRateLaw( \$rate_string, $model );
        
        # assign updated rate law in the energy hash (before we forget about the base ratelaw)
        $rxn->RateLaw->EnergyHash->{ $fingerprint } = $updated_rate_law;
        # point the RateLaw field to the updated law.
        $rxn->RateLaw( $updated_rate_law );
    }
    
    # return with any error messages
    return ( $err );
}



sub getEnergyFingerprint
{
    my $rxn   = shift;    # this Reaction object
    my $epatts = shift;   # reference to list of EnergyPatterns
    
    my $err = '';
   
    # gather stoichiometry of each energy pattern under this reaction
    my @fingerprint = ();
    foreach my $epatt ( @$epatts )
    {
        (my $stoich, $err) = $epatt->getStoich( $rxn );
        push @fingerprint, $stoich;
    }    

    # return fingerprint
    return join( ',', @fingerprint );
}


1;
