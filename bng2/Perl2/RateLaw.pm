# $Id: RateLaw.pm,v 1.14 2007/07/06 04:47:47 faeder Exp $

package RateLaw;

# pragmas
#use strict;
#use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;
use Scalar::Util ("looks_like_number");
use Carp qw(cluck);

# BNG Modules
use BNGUtils;
use SpeciesGraph;


### THE COMMENTED CODE IS AN EXPERIMENT IN REPLACING THE STRUCT MODULE 
###  WITH OUR OWN CUSTOM CLASSES!!!

# Constants
#use constant TYPE=>0, CONSTANTS=>1, FACTOR=>2, TOTALRATE=>3, ENERGYHASH=>4;

## $rl = RateLaw::new( )
## $rl = RateLaw::new( $init )
#sub new
#{
#    my $class = shift @_;
#    my $init  = @_ ? shift @_ : {};

#    my $rl => [ undef, [], undef, undef, {} ];
#    bless $rl, $class;
#           
#    unless (ref $init eq "HASH")
#    {   confess("expecting hash reference for INIT argument.");  }

#    while ( my ($key, $value) = each %$init )
#    {
#        if    ($key eq "Type")       { $rl->setTotalRate( $value );  }
#        elsif ($key eq "Constants")  { $rl->setConstants( $value );  }
#        elsif ($key eq "Factor")     { $rl->setFactor( $value );     }
#        elsif ($key eq "TotalRate")  { $rl->setTotalRate( $value );  }
#        elsif ($key eq "EnergyHash") { $rl->setEnergyHash( $value ); }
#        else {   carp("unrecognized member '$key'.");   }
#    }

#    ++$n_RateLaw;
#    return $rl;
#}


###################
### get/set methods
#sub setType
#{
#    my ($rl, $value) = @_;
#    unless (ref \$value eq 'SCALAR')
#    {   confess("expecting scalar value.");   }
#    $rl->[TYPE] = $value;
#}
#sub getType
#{
#    my ($rl) = @_;
#    return $rl->[TYPE];
#}
##
#sub setConstants
#{
#    my ($rl, $value) = @_;
#    unless (ref $value eq 'ARRAY')
#    {   confess("expecting array reference.");   }
#    $rl->[CONSTANTS] = $value;
#}
#sub getConstants
#{
#    my ($rl) = @_;
#    return $rl->[CONSTANTS];
#}
##
#sub setFactor
#{
#    my ($rl, $value) = @_;
#    unless (ref \$value eq 'SCALAR')
#    {   confess("expecting scalar value.");   }
#    $rl->[FACTOR] = $value;
#    return undef;
#}
#sub getFactor
#{
#    my ($rl) = @_;
#    return $rl->[FACTOR];
#}
##
#sub setTotalRate
#{
#    my ($rl, $value) = @_;
#    unless (ref \$value eq 'SCALAR')
#    {   confess("expecting scalar value.");   }
#    $rl->[TOTALRATE] = $value;
#    return undef;
#}
#sub getTotalRate
#{
#    my ($rl) = @_;
#    return $rl->[TOTALRATE];
#}
##
#sub setEnergyHash
#{
#    my ($rl, $value) = @_;
#    unless (ref \$value eq 'HASH')
#    {   confess("expecting hash reference.");   }
#    $rl->[ENERGYHASH] = $value;
#}
#sub getEnergyHash
#{
#    my ($rl) = @_;
#    return $rl->[ENERGYHASH];
#}

### END OF EXPERIMENT
###


struct RateLaw =>
{
    Type        => '$',       # Ele, Sat, Hill, MM, Arrhenius, FunctionProduct
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
    my $rl = shift @_;
    
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
    my $strptr     = shift @_;
    my $model      = shift @_;
    my $totalRate  = @_ ? shift @_ : 0;
    my $reactants  = @_ ? shift @_ : undef;
    my $basename   = @_ ? shift @_ : "rateLaw";


    my $string_left = $$strptr;
    my ($name, $rate_law_type, $rate_fac);
    my @rate_constants = ();
    my ($param, $err);

    $rate_fac = 1;

    # if totalRate, we need to force expression into a function,
    #  even if it's a constant function.
    my $force_fcn = $totalRate ? 1 : 0;

    # are we in energy BNG mode?
    my $energyBNG = exists $model->Options->{energyBNG} ? $model->Options->{energyBNG} : 0;

    # Handle legacy Sat and MM RateLaw types
    # TODO: convert Sat, MM and Hill into regular functions?
    if ( $string_left =~ s/^(Sat|MM|Hill)\(// )
    {
        $rate_law_type = $1;
        
        if ($totalRate) { return undef, "TotalRate keyword is not compatible with $rate_law_type type RateLaw."; }

        # Validate remaining rate constants
        my $found_end = 0;
        while ( $string_left =~ s/^(\w+)\s*// )
        {
            my $rc = $1;
            ($param, $err) = $model->ParamList->lookup($rc);
            if ($err) {  return '', $err;  }
            push @rate_constants, $rc;
            next if ($string_left =~ s/^,\s*//);
            if ($string_left =~ s/\)//){
                $found_end=1;
            }
        }
        unless ($found_end) {
            return undef, "RateLaw not terminated at $string_left";
        }
    }
    elsif ( $string_left =~ s/^FunctionProduct\(\s*// )
    {
        # Ratelaw is defined by a product of two local functions:
        #  each local function in the product may depend on (at most) one reactant.

        $string_left =~ s/^"([^"]+)"\s*,\s*//;
        my $expr_string_1 = $1;

        $string_left =~ s/^"([^"]+)"\s*\)\s*//;
        my $expr_string_2 = $1;

        # Read function 1
        my $expr1 = Expression->new();
        my $err = $expr1->readString( \$expr_string_1, $model->ParamList );
        if ($err) { return '', $err }
        # get name for ratelaw
        my $name1 = $expr1->getName( $model->ParamList, "localFuncL", $force_fcn );
        # retreive param with this name
        (my $param1, my $err) = $model->ParamList->lookup($name1);

        # Read function 2
        my $expr2 = Expression->new();
        my $err = $expr2->readString( \$expr_string_2, $model->ParamList );
        if ($err) { return '', $err }
        # get name for ratelaw
        my $name2 = $expr2->getName( $model->ParamList, "localFuncR", $force_fcn );
        # retreive param with this name
        (my $param2, my $err) = $model->ParamList->lookup($name2);

        # set ratelaw type
        $rate_law_type = "FunctionProduct";
        # add both functions to the list of "constants"
        push @rate_constants, ($name1, $name2);
    }
    elsif ( $string_left =~ s/^Arrhenius\(\s*// )
    {
        #print STDERR "Parseing Arrhenius RateLaw..\n";
        if ($totalRate)     { return undef, "TotalRate keyword is not compatible with Arrhenius type RateLaw."; }
        unless ($energyBNG) { return undef, "Arrhenius type RateLaw is not available unless energyBNG is enabled."; }       

        my $err;
        $rate_law_type = "Arrhenius"; 

        # extract phi term
        #print STDERR "  string_left = $string_left\n";
        my $phi_expr = Expression->new();
        $err = $phi_expr->readString( \$string_left, $model->ParamList, "," );
        if ($err) { return undef, "Unable to parse Arrhenius ratelaw (expecting phi string): $err"; }
        my $phi_name = $phi_expr->getName( $model->ParamList, "phi" );
        (my $phi_param, $err) = $model->ParamList->lookup($phi_name);

        # remove separating comma and quotes
        #$string_left =~ s/^\s*\"\s*,\s*\"\s*//;
        $string_left =~ s/^\s*,\s*//;

        # extract activation energy expression
        #print STDERR "  string_left = $string_left\n";
        my $actE_expr = Expression->new();
        $err = $actE_expr->readString( \$string_left, $model->ParamList );
        if ($err) { return undef, "Unable to parse Arrhenius ratelaw (expecting activation energy string): $err"; }
        my $actE_name = $actE_expr->getName( $model->ParamList, "actEnergy" );
        (my $actE_param, $err) = $model->ParamList->lookup($actE_name);

        # remove trailing parens
        #$string_left =~ s/^\s*\"\s*\)\s*//;
        $string_left =~ s/^\s*\)\s*//;
        #print STDERR "  string_left = $string_left\n";

        # put name of rate parameter (or fcn) on the constants array
        push @rate_constants, $phi_name, $actE_name;
        if ($actE_param->Type eq "Function")
        {   push @rate_constants, @{$actE_param->Ref->Args};  }

        #print STDERR "  rate constants = ", join(",", @rate_constants), "\n";
        #print STDERR "..done\n";
    }
    elsif ( $string_left =~ /\S+/ )
    {
        # Handle expression for rate constant of elementary reaction
        # Read expression
        my $expr = Expression->new();
        my $err = $expr->readString( \$string_left, $model->ParamList, "," );
        if ($err) { return '', $err; }

        # get name for ratelaw
        my $name = $expr->getName( $model->ParamList, $basename, $force_fcn );
        # retreive param with this name
        (my $param, $err) = $model->ParamList->lookup($name);


        my @local_args = ();
        # determine ratelaw type
        if ( $param->Type eq "Constant"  or  $param->Type eq "ConstantExpression" )
        {   # this is an elementary expression
            $rate_law_type = "Ele";
        }
        else
        {   # this is a function expression..            
            # check for local functions
            if ( $totalRate   and  $expr->checkLocalDependency($model->ParamList) )
            {   return undef, "TotalRate keyword is not compatible with local functions.";   }
            
            $rate_law_type = "Function";
            push @local_args, @{$param->Ref->Args};
        }
        
        # put name of rate parameter (or fcn) on the constants array
        push @rate_constants, $name, @local_args;
    }
    else
    {   # REMOVED IMPLICIT RATELAWS!!  --Justin
        return undef, "No RateLaw supplied for Reaction Rule.";
    }

    # Remove leading whitespace
    $string_left =~ s/^\s*//;
    # Done processing input string
    $$strptr = $string_left;

    # Create new RateLaw object
    my $rl = RateLaw->new( Type=>$rate_law_type, Constants=>[@rate_constants], TotalRate=>$totalRate, Factor=>$rate_fac );

    # Validate rate law type
    if ( $err = $rl->validate($reactants,$model) )
    {   return undef, $err;   }

    ++$n_RateLaw;
    return $rl;
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
    if ( $string_left =~ s/^(\w+)\s*// )
    {
        $name = $1;
        #print "name=$name|$string_left\n";
    }
    else
    {   return '', "Invalid rate law specification in $string_left";   }

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
        if ( looks_like_number($name) )
        {   # treat as a raw number
            $rate_law_type = "Ele";
            push @rate_constants, $name;
        }
        else
        {   # look up parameter
            ($param, $err) = $plist->lookup($name);
            if ($err) {  return '', $err;  }

            # determine ratelaw type
            if ( $param->Type =~ /^Constant/ )
            {  # this is an elementary expression
                $rate_law_type = "Ele";
            }
            else
            {   # this is a function
                $rate_law_type = "Function";
            }
            # TODO: handling for type "FunctionProduct"
    

            # put name of rate parameter (or fcn) on the constants array
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
    if ( $err = $rl->validate ) {  return '', $err;  }

    ++$n_RateLaw;
    return $rl;
}


###
###
###



# evaluate the RateLaw for the local context
sub evaluate_local
{
    my ($rl, $rxn, $ref_map, $model) = @_;

    # two things to accomplish
    # (1) evaluate deltaG_rxn
    # (2) evaluate local activation energy

    my $local_rl = $rl;
    if ($rl->Type eq "Arrhenius" )
    { 
        # get deltaG fingerprint
        my $fingerprint = "$dir;";
        $fingerprint .= $rl->get_deltaG_fingerprint( $model->EnergyPatterns );
     
        # evaluate activation energy in local context and get activation energy fingerprint
        my $local_fcn;
        my ($param) = $model->ParamList->lookup($rl->Constants->[1]);
        if (defined $param  and  $param->Type eq "Function" )
        {
            my $fcn = $param->Ref;
            if ( $fcn->checkLocalDependency($model->ParamList) )
            {
                # get local values for function evaluation
                my @local_args = ( $fcn->Name, map {$ref_map->{$_}} @{$fcn->Args} );
                # evaluate function with local values to get local function
                ($local_fcn) = $param->Ref->evaluate_local( \@local_args, $model->ParamList );
                # add to localfunc string to fingerprint
                $fingerprint .= ";" . $local_fcn->Expr->toString();
            }
            else
            {
                # add to localfunc string to fingerprint
                $fingerprint .= ";" . $fcn->Name;
            }
        }
        else
        {   # no local context in this parameter, create 
            # add to localfunc string to fingerprint
            $fingerprint .= ";" . $rl->Constants->[1];
        }

        #  lookup fingerprint in RateLaw hash
        if ( exists $rl->EnergyHash->{$fingerprint} )
        {   # fetch ratelaw with this fingerprint
            #print STDERR "..found fingerprint.\n";
            $local_rl = $rl->EnergyHash->{$fingerprint};
        }
        else
        {   
            #print STDERR "..didn't find fingerprint. creating new ratelaw.\n";
            # building arrhenius expression..
            # 1) get baseline activation energy
            my $arrhenius_expr;
            if (defined $local_fcn)
            {   $arrhenius_expr = $local_fcn->Expr;   }
            else
            {   $arrhenius_expr = Expression::newNumOrVar( $rl->Constants->[1], $model->ParamList );   }

            # 2a) compute deltaG terms
            my @deltaG_terms = ();
            foreach my $epatt ( @$epatts )
            {
                (my $expr, $err) = $epatt->getEnergyExpression( $rxn, $model->Paramlist );
                if (defined $expr)
                {   push @deltaG_terms, $expr;   }
            }
            # 2b) computer overall deltaG
            my $deltaG_expr;
            if (@deltaG_terms)
            {   $deltaG_expr = Expression::operate( "+", [@deltaG_terms], $model->ParamList );   }

            # 3) compute phi term
            my $phi_expr;
            if ($rxn->RxnRule->Direction==1)
            {   # forward rule:  phi
                $phi_expr = Expression::newNumOrVar( $rl->Constants->[0], $model->ParamList );
            }
            elsif ($rxn->RxnRule->Direction==-1)
            {   # reverse rule: (phi-1).  NOTE: we use (phi-1) rather than (1-phi) since dG(+) = -dG(-)
                $phi_expr = Expression::newNumOrVar( $rl->Constants->[0], $model->ParamList );
                $phi_expr = Expression::operate( "-", [$phi_expr, 1], $model->ParamList );
            }
            else
            {   die "ERROR: invalid direction (must be 1 or -1)";   }

            # 4) computer total activation energy
            my $phi_deltaG_expr;
            if (defined $deltaG_expr)
            {
                my $phi_deltaG_expr = Expression::operate("*", [$phi_expr, $deltaG_expr], $model->ParamList);
                $arrhenius_expr  = Expression::operate("+", [$arrhenius_expr, $phi_deltaG_expr], $model->ParamList);
            }

            # 5) get negative exponential
            $arrhenius_expr = Expression::operate("-", [$arrhenius_expr], $model->ParamList);
            $arrhenius_expr = Expression::operate("FUN", ["exp", $arrhenius_expr], $model->ParamList);

            #print STDERR "  arrhenius expr = ", $arrhenius_expr->toString($model->ParamList), "\n";

            # assign local expr to a parameter
            #my $local_paramname = $model->ParamList->getName($rl->Constants->[1]);
            my $local_paramname = $model->ParamList->getName("Arrhenius");
            $model->ParamList->set( $local_paramname, $arrhenius_expr, 1);

            # create new ratelaw
            $local_rl = RateLaw->new( Type=>"Ele", Constants=>[$local_paramname], Factor=>$rl->Factor );
            ++$RateLaw::n_Ratelaw;

            # add this local ratelaw to the energyhash
            $rl->EnergyHash->{$fingerprint} = $local_rl;
        }
    }
    elsif ( $rl->Type eq 'Function' )
    {
        # get parameter corresponding to ratelaw function
        (my $rl_param) = $model->ParamList->lookup( $rl->Constants->[0] );
        unless ( $rl_param->Type eq 'Function' )
        {   die "Error in RateLaw->evaluate_local(): cannot find parameter for functional RateLaw!";   }
                       
        # get function object
        my $fcn = $rl_param->Ref;                   
        
        # check for local dependency
        if ( $fcn->checkLocalDependency($model->ParamList) )
        {
            # need to create a new ratelaw with locally evaluated observables.
            # evaluate function with local values to get local function.
            my @local_args = ( $fcn->Name, map {$ref_map->{$_}} @{$fcn->Args} );
            (my $local_fcn) = $fcn->evaluate_local( \@local_args, $model->ParamList );
            #  lookup fingerprint in RateLaw hash.
            my $fingerprint = $local_fcn->Expr->toString();
            if ( exists $rl->EnergyHash->{$fingerprint} )
            {   # fetch ratelaw with this fingerprint
                $local_rl = $rl->EnergyHash->{$fingerprint};
            }
            else
            {   # add local_fcn to the parameter list
                #  (so we can lookup the local function in the future!)
                $model->ParamList->set( $local_fcn->Name, $local_fcn->Expr, 1, 'Function', $local_fcn );    
                # create new ratelaw
                $local_rl = RateLaw->new( Type=>'Function', Constants=>[$local_fcn->Name], Factor=>$local_rl->Factor, TotalRate=>0 );
                ++$RateLaw::n_Ratelaw;
            }
        }
    }
    return $local_rl;
}


###
###
###


sub get_deltaG_fingerprint
{
    my ($rl, $rxn, $epatts) = @_;
    # gather stoichiometry of each energy pattern under this reaction
    my ($err, $stoich);
    my @fingerprint = (0) x @$epatts;
    foreach my $ii (0 .. $#$epatts)
    {   ($fingerprint[$ii], $err) = $epatts->[$ii]->getStoich($rxn);   }
    # return fingerprint
    return join(",", @fingerprint);
}


###
###
###


sub equivalent
{
    my $rl1   = shift @_;
    my $rl2   = shift @_;
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
    return 0  unless ( $rl1->Factor == $rl2->Factor );   
    # compare totalrate flag
    return 0  unless ( $rl1->TotalRate eq $rl2->TotalRate );
    
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
    # TODO: handling for FunctionProduct    

    # no difference found, the ratelaws are equivalent
    return 1;
}


###
###
###


# write ratelaw as a string
sub toString
{
    my $rl        = shift @_;
    my $rxn_mult  = @_ ? shift @_ : undef;
    my $netfile   = @_ ? shift @_ : 0;
    my $plist     = @_ ? shift @_ : undef;

    my $string = '';

    # handle ratelaw multiplier
    my $ratelaw_mult = undef;
    if ( (defined $rl->Factor)  and  ($rl->Factor ne '1')  and  ($rl->Factor ne '') )
    {   
        $ratelaw_mult = Expression::newNumOrVar($rl->Factor);
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
    if ( $type eq "Ele" )
    {
        # nothing more to do.. just write the rate constant 
        $string .= sprintf "%s", $rcs;
    }
    elsif ( $type eq 'Function' )
    {
        # we need to write function name and pointer arguments!
        my $last = @{$rl->Constants}-1;
        my @local_refs = @{$rl->Constants}[1..$last];

        $string .= $rcs;
        unless ($netfile)
        {   $string .= "(" . join(",", @local_refs) . ")";   }
    }
    #TODO FunctionProduct
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

    if ( $type eq "Ele" )
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

    if ( $type eq "Ele" )
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

    # separate handling for function product ratelaws
    if ( $rl->Type eq "FunctionProduct" )
    {
        return ( $rl->toXMLFunctionProduct( $indent, $rr_id, $plist, $rrefs ) );
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


sub toXMLFunctionProduct
{
    my $rl      = shift @_;
    my $indent  = shift @_;
    my $rr_id   = @_ ? shift @_ : '';       # rxn rule id
    my $plist   = @_ ? shift @_ : undef;    # parameter list
    my $rrefs   = @_ ? shift @_ : undef;    # rxnrule reference hash

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
    #  references
    $string .= " name1=\"" . $rl->Constants->[0] . "\"";
    $string .= " name2=\"" . $rl->Constants->[1] . "\"";
    #  total rate attribute
    $string .= " totalrate=\"" . $rl->TotalRate . "\"";
    # end of attributes
    $string .= ">\n";

    # Write References
    my $indent2 = $indent.'  ';
    my ( $param1, $err ) = $plist->lookup( $rl->Constants->[0] );
    my $fun1 = $param1->Ref;
    $string .= $indent2 . "<ListOfArguments1>\n";
    foreach my $arg ( @{$fun1->Args} )
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
    $string .= $indent2 . "</ListOfArguments1>\n";

    # Write References
    my $indent2 = $indent.'  ';
    my ( $param2, $err ) = $plist->lookup( $rl->Constants->[1] );
    my $fun2 = $param2->Ref;
    $string .= $indent2 . "<ListOfArguments2>\n";
    foreach my $arg ( @{$fun2->Args} )
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
    $string .= $indent2 . "</ListOfArguments2>\n";

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
    my $rl = shift @_;
    my $reactants = @_ ? shift @_ : undef;
    my $model     = @_ ? shift @_ : undef;

    if ( $rl->Type eq "Ele" )
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
    elsif ( $rl->Type eq 'Function' )
    {
        # Validate local arguments here ?
    }
    elsif ( $rl->Type eq 'FunctionProduct' )
    {
        # Validate local arguments here ?
    }
    elsif ( $rl->Type eq 'Arrhenius' )
    {
        # Validate local arguments here ?
    }
    else
    {
        return "Unrecognized RateLaw type $type";
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
        return "", "Unrecognized RateLaw type $type";
    }
    return $string, '';
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

    if ( $type eq "Ele" )
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
        return ( '', "Unrecognized RateLaw type $type" );
    }

    $string .= "</math>\n";

    return ( $string, '' );
}



###
###
###

1;
