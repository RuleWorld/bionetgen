# $Id: RateLaw.pm,v 1.14 2007/07/06 04:47:47 faeder Exp $

package RateLaw;

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



struct RateLaw =>
{
    Type        => '$',
    Constants   => '@',       # If function, first constant is the function name and the following are local args
    Factor      => '$',
    TotalRate   => '$',       # If true, this ratelaw specifies the Total reaction rate.
                              #   If false (default), the ratelaw specifies a Per Site reaction rate.
    EnergyHash  => '%'        # For energyBNG: Stores a map from rxn energy stoichiometry fingerprints 
                              #   to customized ratelaws for rxns with such a fingerprint.  This allows
                              #   us to lookup and reuse ratelaws (rather than creating a unique ratelaw
                              #   for every reaction induced by a rule).
};


# track total number of ratelaws
my $n_RateLaw = 0;



###
###
###


sub copy
{
    my $rl = shift;
    
    $rl_copy = RateLaw::new();
    $rl_copy->Type( $rl->Type );
    @{$rl_copy->Constants} = @{$rl->Constants};
    $rl_copy->Factor( $rl->Factor );
    $rl_copy->TotalRate( $rl->TotalRate );
    %{$rl_copy->EnergyHash} = %{$rl_copy->EnergyHash};
    
    ++$n_RateLaw;
    return $rl_copy;
}



###
###
###


# Two basic formats are possible
#  [stat_factor*]rate_constant_forward
#   or
#  [stat_factor*]rate_law_type(rate_constant1,...,rate_constantN)

# create a new ratelaw from a BNGL string
sub newRateLaw
{
    my $strptr    = shift;
    my $model     = shift;
    my $totalRate = (@_) ? shift : 0;
    my $reactants = (@_) ? shift : undef;
    my $basename  = (@_) ? shift : 'rateLaw';

    my $string_left = $$strptr;
    my ( $name, $rate_law_type, $rate_fac );
    my @rate_constants = ();
    my ( $param, $err );

    my $plist = $model->ParamList;
    $rate_fac = 1;

    # if totalRate, we need to force expression into a function,
    #  even if it's a constant function.
    my $force_fcn = $totalRate ? 1 : 0;        


    # Handle legacy Sat and MM RateLaw types
    # TODO: convert Sat, MM and Hill into regular functions?
    if ( $string_left =~ s/^(Sat|MM|Hill)\(// )
    {
        $rate_law_type = $1;
        
        if ( $totalRate )
        {   return ( undef, "TotalRate keyword is not compatible with $rate_law_type type RateLaw." );   }
        
        # Validate remaining rate constants
        my $found_end = 0;
        while ( $string_left =~ s/^([A-Za-z0-9_]+)\s*// )
        {
            my $rc = $1;

            #print "rc=$rc\n";
            ( $param, $err ) = $plist->lookup($rc);
            if ($err) {  return ( '', $err );  }
            push @rate_constants, $rc;
            next if ($string_left =~ s/^,\s*//);
            if ($string_left =~ s/\)//){
                $found_end=1;
            }
        }
        unless ($found_end) {
            return ( undef, "RateLaw not terminated at $string_left" );
        }
    }
    elsif ( $string_left =~ /\S+/ )
    {
        # Handle expression for rate constant of elementary reaction
        # Read expression
        my $expr = Expression->new();
        my $err = $expr->readString( \$string_left, $plist, "," );
        if ($err) { return ( '', $err ) }
       
        # get name for ratelaw
        my $name = $expr->getName( $plist, $basename, $force_fcn );
        # retreive param with this name
        ( my $param, my $err ) = $plist->lookup( $name );

        # determine ratelaw type
        if ( $param->Type =~ /^Constant/ )
        {
            # this is an elementary expression
            $rate_law_type = "Ele";
        }
        else
        {            
            #  return error if we're in EnergyBNG mode
            if ( $model->Options->{energyBNG} )
            {   return ( undef, "Functional ratelaws not supported in energyBNG mode." );   }
            
            # check for local functions
            if ( $totalRate   and  $expr->checkLocalDependency($plist) )
            {   return ( undef, "TotalRate keyword is not compatible with local functions." );   }
            
            $rate_law_type = 'Function';
        }
        
        # put name of rate parameter (or fcn) on the constants array
        push @rate_constants, $name;
    }
    else
    {
        # REMOVED IMPLICIT RATELAWS!!  --Justin
        return ( undef, "No RateLaw supplied for Reaction Rule." );
    }

    # Remove leading whitespace
    $string_left =~ s/^\s*//;

    # Done processing input string
    $$strptr = $string_left;

    # Create new RateLaw object
    my $rl;
    $rl = RateLaw->new();
    $rl->Type($rate_law_type);
    $rl->Constants( [@rate_constants] );
    $rl->TotalRate($totalRate);
    $rl->Factor($rate_fac);

    
    # Validate rate law type
    if ( $err = $rl->validate($reactants,$model) )
    {   return ( '', $err );   }

    ++$n_RateLaw;
    return ($rl);
}


###
###
###


# Two basic formats are possible
#  [stat_factor*]rate_constant
#   or
#  [stat_factor*]rate_law_name rate_constant1 rate_constant2 ... rate_constantN

# create a new ratelaw from a .NET string
sub newRateLawNet
{
    my $strptr = shift;
    my $plist  = shift;

    my $string_left = $$strptr;
    my $name, $rate_law_type, $rate_fac;
    my @rate_constants = ();
    my $param, $err;

    # Find statistical factor for rate law
    if ( $string_left =~ s/^([^*\s,]*)\*// )
    {
        my $value;
        ( $param, $err ) = $plist->lookup($1);
        if ($param)
        {
            $value = $param->evaluate([], $plist);
            #$value= $1;
            #print "val=$value\n";
        }
        else
        {
            unless (isReal($1))
            {   return ( '', "Coefficient for rate law ($1) must be a parameter or a real number");   }
            
            $value = $1;
        }
        $rate_fac = $value;
    }
    else
    {
        $rate_fac = 1;
    }

    #print "rf=$rate_fac\n";

    # Get rate law type or name
    if ( $string_left =~ s/^([A-Za-z0-9_]+)\s*// )
    {
        $name = $1;
        #print "name=$name|$string_left\n";
    }
    else
    {   return ( '', "Invalid rate law specification in $string_left" );   }

    if ( $string_left =~ /^\S+/ )
    {
        # Handle rate law type
        $rate_law_type = $name;

        # Validate remaining rate constants
        while ( $string_left =~ s/^([A-Za-z0-9_]+)\s*// )
        {
            my $rc = $1;
            #print "rc=$rc\n";
            ( $param, $err ) = $plist->lookup($rc);
            if ($err) {  return ( '', $err );  }
            push @rate_constants, $rc;
        }

        if ( $string_left =~ /\S+/ )
        {   return ( '', "Invalid rate law syntax in $string_left" );   }
    }
    else 
    {
        # Handle single rate constant for elementary reaction
        $rate_law_type = "Ele";

        # Rate constant
        if ( $name eq '0' )
        {
            # Allow reactions with zero rate for purpose of deleting
            # existing reaction
            $rate_law_type = "Zero";
        }
        else
        {
            ( $param, $err ) = $plist->lookup($name);
            if ($err) {  return ( '', $err );  }
            push @rate_constants, $name;
        }
    }

    # Remove leading whitespace
    $string_left =~ s/^\s*//;

    # Done processing input string
    $$strptr = $string_left;

    # Create new RateLaw object
    my $rl;
    $rl = RateLaw->new();
    $rl->Type($rate_law_type);
    $rl->Constants( [@rate_constants] );
    $rl->TotalRate(0);  
    $rl->Factor($rate_fac);

    # Validate rate law type
    if ( $err = $rl->validate )
    {
        return ( '', $err );
    }

    ++$n_RateLaw;
    return ($rl);
}


###
###
###


sub equivalent
{
    my $rl1   = shift;
    my $rl2   = shift;
    my $plist = (@_) ? shift : undef;

    # first make sure we're dealing with defined ratelaws
    return 0  unless (      defined $rl1  and  (ref $rl1 eq 'RateLaw') 
                       and  defined $rl2  and  (ref $rl2 eq 'RateLaw')  );

    # shortcut  return true if we're looking at the same ratelaw object
    return 1  if ( $rl1 == $rl2 );

    # compare type
    return 0  unless ( $rl1->Type  eq  $rl2->Type );
    # compare number of constants
    return 0  unless ( @{$rl1->Constants} == @{$rl2->Constants} );
    # compare factor
    return 0  unless ( $rl1->Factor  ==  $rl2->Factor );    
    # compare totalrate flag
    return 0  unless ( $rl1->TotalRate  eq  $rl2->TotalRate );
    
    
    if ( $rl1->Type eq 'Function' )
    {
        # check function equivalence: (ignore name)
        my ($par1, $err) = $plist->lookup( $rl1->Constants->[0] );
        return 0  if ($err);
        
        my ($par2, $err) = $plist->lookup( $rl2->Constants->[0] );
        return 0  if ($err);

        return 0  unless ( Function::equivalent($par1->Ref, $par2->Ref, $plist) );
    }
    else
    {
        # compare arguments (all arguments are parameter names)
        for ( my $i = 0;  $i < @{$rl1->Constants};  ++$i )
        {
            return 0  unless ( $rl1->Constants->[$i] eq $rl2->Constants->[$i] );
        }
    }
    
    # no difference found, the ratelaws are equivalent
    return 1;
}


###
###
###


# write ratelaw as a string
sub toString
{
    my $rl        = shift;
    my $rxn_mult  = (@_) ? shift : undef;
    my $netfile   = (@_) ? shift : 0;
    my $plist     = (@_) ? shift : undef;

    my $string = '';

    # handle ratelaw multiplier
    my $ratelaw_mult = undef;
    if ( (defined $rl->Factor)  and  ($rl->Factor ne '1')  and  ($rl->Factor ne '') )
    {   
        $ratelaw_mult = Expression::newNumOrVar( $rl->Factor );
    }
    
    if ( defined $rxn_mult )
    {
        if ( defined $ratelaw_mult )
        {
            $ratelaw_mult = Expression::operate( '*', [($ratelaw_mult, $rxn_mult)], $plist );
        }
        else
        {   $ratelaw_mult = $rxn_mult;   }
    }

    # evaluate ratelaw multiplier and add to the string
    if ( defined $ratelaw_mult ) { $string .= $ratelaw_mult->evaluate($plist) . "*"; }

    # now write the ratelaw to the string
    my $type = $rl->Type;
    my $rcs  = $rl->Constants->[0];
    if ( $type eq 'Ele' )
    {
        # nothing more to do.. just write the rate constant 
        $string .= sprintf "%s", $rcs;
    }
    elsif ( $type eq 'Function' )
    {
        # we need to write function name and pointer arguments!
        my $last = @{$rl->Constants}-1;
        my @local_refs = @{$rl->Constants}[1..$last]; 
        
        #TODO: this is correct, but NET files can't parse it.
        #$string .= $rcs . '(' . join(',', @local_refs) . ')';
        $string .= $rcs;
    }
    else
    {
        if ($netfile){
            $string.= $type." ".join(' ', @{$rl->Constants});
        } else {
            $string.= $type."(".join(',', @{$rl->Constants}).")";
        }
    }

    return $string;
}


###
###
###


# write ratelaw as a CVode formula
sub toCVodeString
{
    my $rl         = shift;
    my $rxn_mult   = shift;
    my $reactants  = shift;
    my $rrefs      = shift;
    my $plist      = (@_) ? shift : undef;


    # handle ratelaw multiplier
    my $ratelaw_mult = undef;
    if ( (defined $rl->Factor)  and  ($rl->Factor ne '1')  and  ($rl->Factor ne '') )
    {   
        $ratelaw_mult = Expression::newNumOrVar( $rl->Factor );
    }
    
    if ( defined $rxn_mult )
    {
        if ( defined $ratelaw_mult )
        {
            $ratelaw_mult = Expression::operate( '*', [($ratelaw_mult, $rxn_mult)], $plist );
        }
        else
        {   $ratelaw_mult = $rxn_mult;   }
    }
    
    
    # begin gather ratelaw terms
    my @rl_terms = ();    
    if (defined $ratelaw_mult) {  push @rl_terms, $ratelaw_mult->toCVodeString( $plist );  }
    
    my $type = $rl->Type;
    my $rate_constants = $rl->Constants;

    if ( $type eq 'Ele' )
    {
        # look up parameter
        (my $const) = $plist->lookup( $rate_constants->[0] );
    
        # get rate constant
        push @rl_terms, $const->toCVodeString($plist);
      
        # get reactant species    
        foreach my $reactant ( @$reactants )
        {
            # add CVode ref to this reactant species
            push @rl_terms, $reactant->getCVodeName;             
        }
    }
    elsif ( $type eq 'Function' )
    {
        # look up parameter
        (my $fcn_param) = $plist->lookup( $rate_constants->[0] ); 
        
        my $fcn = $fcn_param->Ref;

        # get CVodeRefs for tagged reactants
        my @fcn_args = ();
        if ( @{$fcn->Args} )
        {
            if ( ref $rrefs eq 'HASH' )
            {
                foreach my $tag (  @{$fcn->Args} )
                {
                    unless ( (exists $rrefs->{$tag}) and (exists $reactants->[$rrefs->{$tag}]) )
                    {   return "could not find reactant or tag corresponding to ratelaw argument!";   }
                    
                    push @fcn_args, ($reactants->[$rrefs->{$tag}])->getCVodeName;
                }
            }
            else
            {   return "ratelaw depends on tagged reactants and RRefs hash is missing!";   }
        }

        # add references to the expressions and observables arrays
        #push @rl_terms, $fcn->toCVodeString( $plist, {'fcn_mode' => 'call'});
        push @fcn_args, 'expressions', 'observables';
        push @rl_terms, $fcn->Name . '(' . join( ',', @fcn_args ) . ')';

        # get reactant species  
        foreach my $reactant ( @$reactants )
        {
            # add CVode ref to this reactant species
            push @rl_terms, $reactant->getCVodeName;
        }
    }
    else
    {
        #$string.= $type."(".join(',', @{$rl->Constants}).")";
        return  "Ratelaws of type $type are not yet supported by writeMexfile()";
    }
        
    # build ratelaw string
    return join( '*', @rl_terms );
}


###
###
###


# write ratelaw as a CVode formula
sub toMatlabString
{
    my $rl         = shift;
    my $rxn_mult   = shift;
    my $reactants  = shift;
    my $rrefs      = shift;
    my $plist      = (@_) ? shift : undef;


    # handle ratelaw multiplier
    my $ratelaw_mult = undef;
    if ( (defined $rl->Factor)  and  ($rl->Factor ne '1')  and  ($rl->Factor ne '') )
    {   
        $ratelaw_mult = Expression::newNumOrVar( $rl->Factor );
    }
    
    if ( defined $rxn_mult )
    {
        if ( defined $ratelaw_mult )
        {
            $ratelaw_mult = Expression::operate( '*', [($ratelaw_mult, $rxn_mult)], $plist );
        }
        else
        {   $ratelaw_mult = $rxn_mult;   }
    }
    
    # begin gather ratelaw terms
    my @rl_terms = ();    
    if (defined $ratelaw_mult) {  push @rl_terms, $ratelaw_mult->toMatlabString( $plist );  }
    
    my $type = $rl->Type;
    my $rate_constants = $rl->Constants;

    if ( $type eq 'Ele' )
    {
        # look up parameter
        (my $const) = $plist->lookup( $rate_constants->[0] );
    
        # get rate constant
        push @rl_terms, $const->toMatlabString($plist);
        
        # get reactant species    
        foreach my $reactant ( @$reactants )
        {
            # add CVode ref to this reactant species
            push @rl_terms, $reactant->getMatlabName();          
        }
    }
    elsif ( $type eq 'Function' )
    {
        # look up parameter
        (my $fcn_param) = $plist->lookup( $rate_constants->[0] ); 
        
        my $fcn = $fcn_param->Ref;

        # get CVodeRefs for tagged reactants
        my @fcn_args = ();
        if ( @{$fcn->Args} )
        {
            if ( ref $rrefs eq 'HASH' )
            {
                foreach my $tag (  @{$fcn->Args} )
                {
                    unless ( (exists $rrefs->{$tag}) and (exists $reactants->[$rrefs->{$tag}]) )
                    {   return "could not find reactant or tag corresponding to ratelaw argument!";   }
                    
                    push @fcn_args, ($reactants->[$rrefs->{$tag}])->getMatlabName();
                }
            }
            else
            {   return "ratelaw depends on tagged reactants and RRefs hash is missing!";   }
        }

        # TODO: move this functionality to Function class.        
        #push @rl_terms, $fcn->toMatlabString( $plist, {'fcn_mode' => 'call'});
        push @fcn_args, 'expressions', 'observables';
        push @rl_terms, $fcn->Name . '(' . join( ',', @fcn_args ) . ')';

        # get reactant species  
        foreach my $reactant ( @$reactants )
        {
            # add CVode ref to this reactant species
            push @rl_terms, $reactant->getMatlabName();
        }
    }
    else
    {
        #$string.= $type."(".join(',', @{$rl->Constants}).")";
        return  "Ratelaws of type $type are not yet supported by writeMfile()";
    }
        
    # build ratelaw string
    return join( '*', @rl_terms );
}



###
###
###


# write ratelaw in XML format
sub toXML
{
    my $rl      = shift;
    my $indent  = shift;
    my $rr_id   = (@_) ? shift : '';       # rxn rule id
    my $plist   = (@_) ? shift : undef;    # parameter list
    my $rrefs   = (@_) ? shift : undef;    # rxnrule reference hash


    # separate handling for function ratelaws
    if ( $rl->Type eq "Function" )
    {
        return ( $rl->toXMLFunction( $indent, $rr_id, $plist, $rrefs ) );
    }

    # handle other ratelaw types
    
    # define ratelaw id
    my $id;
    if ( $rr_id ) 
    {   $id = $rr_id . "_RateLaw";   }
    else
    {   $id = "RateLaw" . $RateLaw::n_RateLaw;   }

    my $string = $indent . "<RateLaw";


    # Attributes
    # id
    $string .= " id=\"" . $id . "\"";

    # type
    $string .= " type=\"" . $rl->Type . "\"";

    # total rate attribute
    $string .= " totalrate=\"" . $rl->TotalRate . "\"";

    # StatFactor (Is this used anymore??  --Justin)
    unless ( $rl->Factor == 1 )
    {   $string .= " statFactor=\"" . $rl->Factor . "\"";   }

    # Objects contained
    my $ostring = "";
    my $indent2 = "  " . $indent;

    $ostring .= $indent2 . "<ListOfRateConstants>\n";
    foreach my $rc ( @{ $rl->Constants } )
    {
        my $indent3 = "  " . $indent2;
        ### How to handle references to named parameters?
        $ostring .= $indent3 . "<RateConstant";
        $ostring .= " value=\"" . $rc . "\"";
        $ostring .= "/>\n";
    }
    $ostring .= $indent2 . "</ListOfRateConstants>\n";

    # Termination
    if ($ostring)
    {
        $string .= ">\n";                      # terminate tag opening
        $string .= $ostring;
        $string .= $indent . "</RateLaw>\n";
    }
    else
    {
        $string .= "/>\n";                     # short tag termination
    }
    
    return ( $string );
}


###
###
###


sub toXMLFunction
{
    my $rl      = shift;
    my $indent  = shift;
    my $rr_id   = (@_) ? shift : '';       # rxn rule id
    my $plist   = (@_) ? shift : undef;    # parameter list
    my $rrefs   = (@_) ? shift : undef;    # rxnrule reference hash

    # define ratelaw id
    my $id;
    if ( $rr_id ) 
    {   $id = $rr_id . "_RateLaw";   }
    else
    {   $id = "RateLaw" . $RateLaw::n_RateLaw;   }


    # Write ratelaw header and attributes:
    my $string = $indent . "<RateLaw";
    #  id
    $string .= " id=\"" . $id . "\"";
    #  type
    $string .= " type=\"" . $rl->Type . "\"";
    #  reference
    $string .= " name=\"" . $rl->Constants->[0] . "\"";
    #  total rate attribute
    $string .= " totalrate=\"" . $rl->TotalRate . "\"";
    # end of attributes
    $string .= ">\n";

    # Write References
    my $indent2 = $indent.'  ';
    my ( $param, $err ) = $plist->lookup( $rl->Constants->[0] );
    my $fun = $param->Ref;
    $string .= $indent2 . "<ListOfArguments>\n";
    foreach my $arg ( @{$fun->Args} )
    {
        my $indent3 = $indent2.'  ';
        my $ptr = $rrefs->{$arg};
        my $oid = RxnRule::pointer_to_ID( $rr_id . "_R", $ptr );
        $string .= $indent3 . "<Argument";
        $string .= " id=\"" . $arg . "\"";
        $string .= " type=\"ObjectReference\"";
        $string .= " value=\"" . $oid . "\"";
        $string .= "/>\n";
    }
    $string .= $indent2 . "</ListOfArguments>\n";

    # Termination
    $string .= $indent . "</RateLaw>\n";

    return ($string);
}


###
###
###


# WARNING: Checking here is minimal
sub validate
{
    my $rl = shift;
    my $reactants = (@_) ? shift : undef;
    my $model     = (@_) ? shift : undef;

    if ( $rl->Type eq 'Ele' )
    {
  
    }
    elsif ( $rl->Type eq 'Sat' )
    {
        if ( @{$rl->Constants} < 2 )
        {   # Too few rate constants
            return "Saturation type ratelaws require at least 2 rate constants";
        }
        if ( defined $reactants )
        {
            if ( @{$rl->Constants} > @{$reactants} + 1 )
            {   # more saturations constants than reactants
                return "Saturation type ratelaws cannot have more saturation constants than reactants";
            }
        }
    }
    elsif ( $rl->Type eq 'MM' )
    {
        if ( @{$rl->Constants} != 2 )
        {   # incorrect number of rate constants
            return "Michaelis-Menton type ratelaws require exactly 2 rate constants";
        }
        if ( defined $reactants )
        {
            if (@{$reactants} != 2 )
            {   # incorrect number of reactants
                return "Michaelis-Menton type ratelaw require exactly 2 reactants (e.g. Substrate + Enzyme)";
            }
            if ( ref $reactants->[0] eq 'SpeciesGraph'  and  ref $reactants->[1] eq 'SpeciesGraph' )
            {
                if (      $reactants->[0]->isomorphicToSubgraph($reactants->[1])
                     and  $reactants->[1]->isomorphicToSubgraph($reactants->[0]) )
                {   # reactants are identical!
                    return "Michaelis-Menton type ratelaw requires non-identical reactants (e.g. Substrate + Enzyme)";
                }

                if ( defined $model )
                {
		            my $err = $model->MoleculeTypesList->checkSpeciesGraph( $reactants->[0],
                                                                            { IsSpecies            => 1,
                                                                              AllowNewTypes        => 0  } );
                    if ( $err )
                    {   # substrate reactant can potentially match multiple species
                        send_warning( "In rule with Michaelis-Menton type ratelaw:\n"
                                     ."  substrate reactant pattern may match more than one species.\n"
                                     ."  This may yield unexpected results in Network simulations.");
	                }
                }
            }
        }
    }
    elsif ( $rl->Type eq 'Hill' )
    {
        if ( @{$rl->Constants} != 3 )
        {   # incorrect number of rate constants
            return "Hill type ratelaws require exactly 3 rate constants";
        }
    }
    elsif ( $rl->Type eq 'Zero' )
    {
        if ( @{$rl->Constants} != 0 )
        {
            return "Zero type ratelaws do not accept rate constants";
        }
    }
    elsif ( $rl->Type eq 'Function' )
    {
        # Validate local arguments here ?
    }
    else
    {
        return "Unrecogized RateLaw type $type";
    }

    return undef;
}



###
###
###


sub toLatexString
{
    my $rl         = shift;
    my $reactants  = shift;
    my $statFactor = shift;
    my $plist      = shift;
    my $string     = '';

    my @k    = @{ $rl->Constants };
    my $type = $rl->Type;

    $statFactor *= $rl->Factor;

    my %pindex = ();
    foreach my $index ( 0 .. $#{ $plist->Array } )
    {
        my $pname = $plist->Array->[$index]->Name;
        $pindex{$pname} = $index + 1;
    }

    if ( $type eq "Ele" )
    {
        if ( $statFactor != 1 )
        {
            $string .= "$statFactor ";
        }
        $string .= "p_{" . $pindex{ $k[0] } . "}";
        foreach my $reac (@$reactants)
        {
            $string .= " x_{" . $reac->Index . "}";
        }
    }
    else
    {
        return ( "", "Unrecogized RateLaw type $type" );
    }
    return ( $string, '' );
}


###
###
###


sub toMathMLString
{
    my $rl         = shift;
    my $rindices   = shift;
    my $pindices   = shift;
    my $statFactor = (@_) ? shift : 1;
    my $string     = '';

    my @k    = @{ $rl->Constants };
    my $type = $rl->Type;

    $string .= "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";

    $statFactor *= $rl->Factor;

    if ( $type eq 'Ele' )
    {
        # $statFactor*$k[0]*reactant1*...*reactantN
        $string .= "  <apply>\n    <times/>\n";
        if ( $statFactor != 1 )
        {
            $string .= "    <cn> $statFactor </cn>\n";
        }
        $string .= sprintf "    <ci> %s </ci>\n", $k[0];
        for my $i (@$rindices)
        {
            $string .= sprintf "    <ci> S%d </ci>\n", $i;
        }
        $string .= "  </apply>\n";
    }
    elsif ( $type eq 'Sat' )
    {
        # NOTE: THIS CODE IS NOT TESTED!
        # One parameter:
        #    rate = $statFactor*$k[0]
        # Two or more parameters (N denominator terms, N+1 parameters)
        #    rate = $statFactor*$k[0]*reactant1*...*reactantN/(($k[1]+reactant1)*...*($k[N]+reactantN)
        if ( $#k == 0 )
        {
            if ( $statFactor != 1 )
            {
                $string .= "    <apply>\n";
                $string .= "      <times/>\n";
                $string .= "      <cn> $statFactor </cn>\n";
                $string .= sprintf "      <ci> %s </ci>\n", $k[0];
                $string .= "    </apply>\n";
            }
            else
            {
                $string .= sprintf "      <ci> %s </ci>\n", $k[0];
            }
        }
        else
        {
            $string .= "  <apply>\n";
            $string .= "    <divide/>\n";

            $string .= "    <apply>\n";
            $string .= "      <times/>\n";
            if ( $statFactor != 1 )
            {
                $string .= "      <cn> $statFactor </cn>\n";
            }
            $string .= sprintf "      <ci> %s </ci>\n", $k[0];
            foreach my $i (@$rindices)
            {
                $string .= sprintf "      <ci> S%d </ci>\n", $i;
            }
            $string .= "    </apply>\n";
            my $indentp = "    ";
            if ( $#k > 1 )
            {
                $string  .= $indentp . "<apply>\n";
                $string  .= $indentp . "<times/>\n";
                $indentp .= "  ";
            }
            foreach my $ik ( 1 .. $#k )
            {
                $string .= $indentp . "<apply>\n";
                $string .= $indentp . "<plus/>\n";
                $string .= sprintf "%s  <ci> %s </ci>\n", $indentp, $k[$ik];
                $string .= sprintf "%s  <ci> S%d </ci>\n", $indentp,
                $$rindices[ $ik - 1 ];
                $string .= $indentp . "</apply>\n";
            }
            if ( $#k > 1 )
            {
                $indentp =~ s/  $//;
                $string .= $indentp . "</apply>\n";    # end times
            }
            $string .= "  </apply>\n";               # end divide
        }
    }
    elsif ( $type eq 'MM' )
    {
        return ( '', "Michaelis-Menton type reactions are not curently handled." );
    }
    else
    {
        return ( '', "Unrecogized RateLaw type $type" );
    }

    $string .= "</math>\n";

    return ( $string, '' );
}


###
###
###


1;
