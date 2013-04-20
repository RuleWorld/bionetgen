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
  Reactants      => '@',         # Array of reactant Species
  Products       => '@',         # Array of product Species
  RateLaw        => 'RateLaw',   # Reference to a RateLaw object
  StatFactor     => '$',	     # Statistical (multiplicity) factor for multiple rxn pathways
  Priority       => '$',         # Priority of this Rxn (Deprecated?)
  RxnRule        => '$',         # RxnRule that generated this Rxn
  Index          => '$',         # Reaction Index for writing network output
};


sub toString
{
    my $rxn   = shift @_;
    my $text  = @_ ? shift @_ : 0;
    my $plist = @_ ? shift @_ : undef;
  
    my $err;
    my $string;
  
    if ($text)
    {   # write Reactants and Products as BNGL strings
        my @rstrings=();
        my @pstrings=();
        foreach my $r ( @{$rxn->Reactants} )
        {   push @rstrings, $r->SpeciesGraph->toString();   }
        foreach my $p ( @{$rxn->Products} )
        {   push @pstrings, $p->SpeciesGraph->toString();   }
        $string = join(' + ', @rstrings) . " -> " . join(' + ', @pstrings);
    }
    else
    {   # just write Reactant and Product species indexes
        $string = $rxn->stringID(); 
    }

    $string .= ' ';

    # Write the Ratelaw...
    if (defined $rxn->RateLaw)
    {   $string .= $rxn->RateLaw->toString($rxn->StatFactor, 1, $plist);   }   
    
    # Write source RxnRule
    if (defined $rxn->RxnRule) {  $string .= " #" . $rxn->RxnRule->Name;  }

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
    my $rxn   = shift @_;
    my $plist = shift @_;

    # get reference to RxnRule RRef hash (TODO: may be obsolete)
    my $rrefs = undef;
    if ( $rxn->RxnRule )
    {   $rrefs = $rxn->RxnRule->RRefs;   }
    # get ratelaw string   
    return $rxn->RateLaw->toCVodeString( $rxn->StatFactor, $rxn->Reactants, $rrefs, $plist );
}


###
###
###


sub getMatlabRate
{
    my $rxn   = shift @_;
    my $plist = shift @_;

    # get reference to RxnRule RRef hash
    my $rrefs = undef;
    if ( $rxn->RxnRule )
    {   $rrefs = $rxn->RxnRule->RRefs;   }
    # get ratelaw string  
    return $rxn->RateLaw->toMatlabString( $rxn->StatFactor, $rxn->Reactants, $rrefs, $plist );
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
    if ( $type eq "Ele" )
    {   # don't sort MM, Sat, or Hill...  TODO: sort Function ratelaws? (since local context is already evaluated)
        @rstrings = sort {$a<=>$b} @rstrings;
        @pstrings = sort {$a<=>$b} @pstrings;
    }
    
    $string .= join(',', @rstrings) . " " . join(',', @pstrings);
    return ($string);
}

###
###
###

1;
