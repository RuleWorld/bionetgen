# $Id: SpeciesList.pm,v 1.14 2007/02/20 17:37:01 faeder Exp $

package SpeciesList;

# pragmas
use strict;
use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;

# BNG Modules
use BNGUtils;
use Species;
use SpeciesGraph;
use MoleculeTypesList;
use ParamList;



# Members
struct SpeciesList =>
{
    Array      => '@',
    Hash       => '%',
    Hash_exact => '%'
};



###
###
###


sub getNumSpecies
{
    my $slist = shift;
    return scalar @{$slist->Array};
}

sub size
{
    my $slist = shift @_;
    return scalar @{$slist->Array};
}

###
###
###


sub sort
{
    my $slist = shift;

    print "Sorting species list\n";
    my @newarr = sort {$a->SpeciesGraph->StringExact cmp $b->SpeciesGraph->StringExact} @{$slist->Array};
    $slist->Array(\@newarr);
    my $ispec = 1;
    foreach my $spec ( @{$slist->Array} )
    {
        $spec->Index($ispec);
        ++$ispec;
    }

    return $slist;
}



###
###
###



# Returns pointer to matching species in $slist or null if no match found
sub lookup
{
    my $slist = shift;
    my $sg = shift;
    my $check_iso = (@_) ? shift : 1;

    if( $sg->IsCanonical ) {  $check_iso = 0;  }

    my $spec = undef;
    my $sstring = $sg->StringID;
    if ( exists $slist->Hash->{$sstring} )
    {
        # Since string is not completely canonical, need to check isomorphism with other list members
        # Determine whether the graph is isomorphic to any on the current list
        if ($check_iso)
        {
            my $found_iso=0;
            foreach my $spec2 ( @{$slist->Hash->{$sstring}} )
            {
	            if ($sg->isomorphicTo($spec2->SpeciesGraph))
	            {
                    $spec = $spec2;
                    $found_iso=1;
                    last;
                }
            }
        }
        else
        {
            #print "Not checking isomorphism\n";
            $spec = $slist->Hash->{$sstring}->[0];
        }
    }
    return $spec;
}



###
###
###



sub lookup_bystring
{
    my $slist   = shift @_;
    my $sstring = shift @_;

    my $species = undef;
    if ( exists $slist->Hash_exact->{$sstring} )
    {
        $species = $slist->Hash_exact->{$sstring};
    }
    return $species;
}

# Returns reference to Species object either newly created or found in $slist
# Should check if species already exists in list before adding
sub add
{
    my $slist = shift @_;
    my $sg    = shift @_;
    my $conc  = @_ ? shift @_ : 0;

    # Create new species from SpeciesGraph
    my $spec = Species->new;
    push @{$slist->Array}, $spec;
    push @{$slist->Hash->{$sg->StringID}}, $spec;
     # Can only be one entry. TODO: generate exit_error if there is a clash!
    $slist->Hash_exact->{$sg->StringExact} = $spec;
    $spec->SpeciesGraph($sg);
    $spec->Concentration($conc);
    $spec->Index( scalar @{$slist->Array} );
    $spec->RulesApplied(0);
    # Put ref to species in SpeciesGraph to bind it
    $sg->Species($spec);

    return $spec;
}

sub remove
{
    my $slist = shift @_;
    my $spec  = shift @_;

    # Remove from Array
    splice( @{$slist->Array}, $spec->Index-1, 1 );

    # Remove from Hash 
    my $harray = $slist->Hash->{$spec->SpeciesGraph->StringID};
    foreach my $i (0..$#$harray)
    {
        if ($spec == $harray->[$i])
        {
            splice( @$harray, $i, 1 );
            unless (@$harray)
            {
                undef $slist->Hash->{$spec->SpeciesGraph->StringID};
            }
            last;
        }
    }

    # Remove from Hash_exact
    undef $slist->Hash_exact->{$spec->SpeciesGraph->StringExact};

    return;
}



# Read entry from input species block
sub readString
{
    my $slist  = shift @_;
    my $string = shift @_;
    my $plist  = shift @_;
    my $clist  = shift @_;
    my $mtlist = shift @_;

    my ($conc, $sg, $err);

    my $name = '';

    # remove leading index
    if ( $string =~ s/^\s*(\d+)\s+// ) {}

    # Read species string
    $sg = SpeciesGraph->new;
    $string =~ s/^\s+//;
    $err = $sg->readString( \$string, $clist, 1, '^\s+', $mtlist );
    if ($err) { return $err; }

    if ( $sg->isNull() )
    {   # this is the null pattern
        send_warning( "Found useless instance of null pattern in SpeciesList" );
        return '';
    }

    # Check if isomorphic to existing species
    my $existing= $slist->lookup($sg);
    if ($existing)
    {
        my $sstring = $sg->StringExact;
        my $index = $existing->Index;
        return "Species $sstring isomorphic to previously defined species index $index";
    }

    # Read species concentration as math expression.
    # Set species concentration to number or variable name.
    if ( $string =~ /\S+/ )
    {
        # Read expression
        my $expr = Expression->new();
        if ( my $err = $expr->readString(\$string, $plist) ) {  return ('', $err);  }
        if ( $expr->Type eq 'NUM' )
        {
            $conc = $expr->evaluate();
        }
        else
        {
            $conc = $expr->getName( $plist, 'InitialConc' );
        }
    }
    else
    {
        return "Species is missing expression for population or concentration";
    }

    # Create new Species entry in SpeciesList
    $slist->add($sg, $conc);
  
    return '';
}



###
###
###


# string = $slist->writeBNGL( *Array, [*ParamList, bool, *Hash] ) 
sub writeBNGL
{
    my $slist       = shift @_;
    my $conc        = @_ ? shift @_ : [];
    my $plist       = @_ ? shift @_ : undef;
    my $user_params = @_ ? shift @_ : { 'TextSpecies'=>0, 'pretty_formatting'=>0 };

    # this method loads @$conc with initial conditions (if @$conc is empty)
    #  OR checks that @$conc is okay (if @$conc has elements)
    $slist->checkOrInitConcentrations($conc);

    # Determine length of longest species string
    my $maxlen = 0;
    foreach my $spec ( @{$slist->Array} )
    {
        my $len = 1;
        $len += defined $spec->SpeciesGraph->Name        ? length $spec->SpeciesGraph->Name        : 0;
        $len += defined $spec->SpeciesGraph->StringExact ? length $spec->SpeciesGraph->StringExact : 0;
        $maxlen = ($len > $maxlen) ? $len : $maxlen;
    }

    my $out .= "begin species\n";
    foreach my $spec ( @{$slist->Array} )
    {
        if ( $user_params->{'pretty_formatting'} )
        {   # no species index, ident
            $out .= '  ';
        }
        else
        {   # include index
            $out .= sprintf "%5d ", $spec->Index;
        }

        # get species graph string
        my $sexact = $spec->SpeciesGraph->toString();
        $out .= sprintf "%-${maxlen}s", $sexact;
        
        my $c = $conc->[ $spec->Index - 1 ];
        $out .= sprintf " %s\n", $c;
    }  
    $out .= "end species\n";
    
    return $out;
}



###
###
###



sub writeSSC
{
    my $slist = shift @_;
    my $conc  = @_ ? shift @_ : [];
    my $plist = @_ ? shift @_ : undef;
    my $print_names = @_ ? shift : 0;

    my $string = '';

    # this method loads @$conc with initial conditions (if @$conc is empty)
    #  OR checks that @$conc is okay (if @$conc has elements)
    $slist->checkOrInitConcentrations($conc);

    my $idx=0;
    foreach my $spec (@{$slist->Array})
    {
        my $sname;
        my $sexact= $spec->SpeciesGraph->toStringSSCMol();
        $sname=$sexact;
        $string .= "new $sname at ";
        my $c;
        $c = $conc->[$idx];
        $string .= $c;
        $string.= "\n";
        ++$idx;
    }

    return $string;
}


sub print
{
    my $slist = shift @_;
    my $fh = shift @_;
    my $i_start = @_ ? shift : 0;

    print $fh "begin species\n";
    foreach my $i ( $i_start..$#{$slist->Array} )
    {
        my $spec = $slist->Array->[$i];
        printf $fh "%5d %s %s\n", $i-$i_start+1, $spec->SpeciesGraph->StringExact, $spec->Concentration;
    }
    print $fh "end species\n";
    return '';
}

sub toXML
{
    my $slist  = shift @_;
    my $indent = @_ ? shift @_ : "";
    # Use concentration array if provided
    my $conc   = @_ ? shift @_ : [];

    # this method loads @$conc with initial conditions (if @$conc is empty)
    #  OR checks that @$conc is okay (if @$conc has elements)
    $slist->checkOrInitConcentrations($conc);

    my $string = $indent . "<ListOfSpecies>\n";

    my $i=0;
    foreach my $spec (@{$slist->Array})
    {
        $string.= $spec->toXML("  ".$indent, $conc->[$i]);
        ++$i;
    }

    $string .= $indent."</ListOfSpecies>\n";
    return $string;
}


###
###
###


sub toCVodeString
{
    my $slist       = shift;
    my $rlist       = shift;
    my $stoich_hash = shift;
    my $plist       = (@_) ? shift : undef;

    my $deriv_defs = '';
    my $indent = '    ';
    my $err;

    # construct derivative definition for each species
    foreach my $species ( @{ $slist->Array } )
    {
        # get species vector in stoich hash
        my $species_vector = $stoich_hash->{ $species->Index };
        my $species_deriv = '';
        
        if ( $species->SpeciesGraph->Fixed )
        {   # handle species with fixed population
            $species_deriv = 0.0;
        }
        else
        {   # handle all other species...
            # add rates and stoich for each reaction that influences this speices
            foreach my $i_rxn ( keys %$species_vector )
            {
                # get species stoichiometry under this reaction
                my $stoich = $species_vector->{$i_rxn};
                
                # look up reaction object
                my $i_rxn0 = $i_rxn - 1;
                my $rxn = $rlist->Array->[$i_rxn0];
                
                # add this reaction flux to the species derivative
                if    ( $stoich == 1 )
                {   $species_deriv .= " +" . $rxn->getCVodeName();             }
                elsif ( $stoich == 0 )
                {                                                              }
                elsif ( $stoich == -1 )
                {   $species_deriv .= " -" . $rxn->getCVodeName();             }
                elsif ( $stoich > 0 )
                {   $species_deriv .= " +$stoich.0*" . $rxn->getCVodeName();   }
                elsif ( $stoich < 0 )
                {   $species_deriv .= " $stoich.0*" . $rxn->getCVodeName();    } 
            } 

            # trim leading " +"
            $species_deriv =~ s/^ \+?//;
        
            # replace empty string with a zero rate
            if ($species_deriv eq '')
            {   $species_deriv = '0.0';   }
        }
            
        # add derivative to list of definitions
        $deriv_defs .= $indent . $species->getCVodeDerivName() . " = $species_deriv;\n"; 
    }

    return ( $deriv_defs, $err );
}


###
###
###


sub toMatlabString
{
    my $slist       = shift;
    my $rlist       = shift;
    my $stoich_hash = shift;
    my $plist       = (@_) ? shift : undef;

    my $deriv_defs = '';
    my $indent = '    ';
    my $err;

    # construct derivative definition for each species
    foreach my $species ( @{ $slist->Array } )
    {
        # get species vector in stoich hash
        my $species_vector = $stoich_hash->{ $species->Index };
        my $species_deriv = '';
        
        if ( $species->SpeciesGraph->Fixed )
        {   # handle species with fixed population
            $species_deriv = '0.0';
        }
        else
        {   # handle all other species...
            # add rates and stoich for each reaction that influences this speices
            foreach my $i_rxn ( keys %$species_vector )
            {
                # get species stoichiometry under this reaction
                my $stoich = $species_vector->{$i_rxn};
                
                # look up reaction object
                my $i_rxn0 = $i_rxn - 1;
                my $rxn = $rlist->Array->[$i_rxn0];
                
                # add this reaction flux to the species derivative
                if    ( $stoich == 1 )
                {   $species_deriv .= " +" . $rxn->getMatlabName();             }
                elsif ( $stoich == 0 )
                {                                                               }
                elsif ( $stoich == -1 )
                {   $species_deriv .= " -" . $rxn->getMatlabName();             }
                elsif ( $stoich > 0 )
                {   $species_deriv .= " +$stoich.0*" . $rxn->getMatlabName();   }
                elsif ( $stoich < 0 )
                {   $species_deriv .= " $stoich.0*" . $rxn->getMatlabName();    } 
            } 

            # trim leading " +"
            $species_deriv =~ s/^ \+?//;
        
            # replace empty string with a zero rate
            if ($species_deriv eq '')
            {   $species_deriv = '0.0';   }
        }
            
        # add derivative to list of definitions
        $deriv_defs .= $indent . $species->getMatlabDerivName() . " = $species_deriv;\n"; 
    }

    return ( $deriv_defs, $err );
}


###
###
###


# get names of species and formulas for initial concentrations for matlab
#  NOTE: this method ALWAYS writes the original initial concentration definitions
#   and ignores any changes made by setConcentrations
sub getMatlabSpeciesNames
{
    my $slist = shift @_;
    my $model = shift @_;
    
    my $plist = $model->ParamList;

    my $err;
    my $species_names = '';
    my $species_init = '';
    my $indent = '    ';
    
    # TODO: this matlab output is a hack.  improve this.  --justin

    # generate a map from param names to matlab references
    my $ref_map = {};
    my $m_idx = 1;
    foreach my $param ( @{$plist->Array} )
    {
        if ( $param->Type eq 'Constant' )
        {
            $ref_map->{ $param->Name } = "params($m_idx)";
            ++$m_idx;
        }
    }
    
    # gather names and init expressions for all species
    $m_idx = 1;
    my @species_names = ();    
    foreach my $species ( @{ $slist->Array } )
    {
        push @species_names, "'" . $species->SpeciesGraph->StringExact . "'";    
        (my $param) = $plist->lookup( $species->Concentration );    
    
        if ( $param )
        {   # initial concentration is given by a Parameter
            # expand the expression (recursively past parameters!)
            $species_init .= $indent . "species_init($m_idx) = " . $param->toString( $plist, 0, 2 ) . ";\n";          
        }
        else
        {   # initial concentration is a number
            $species_init .= $indent . "species_init($m_idx) = " . $species->Concentration . ";\n";
        }  
        ++$m_idx;
    }
    
    # replace param names with Matlab references   
    foreach my $pname ( keys %$ref_map )
    {
        my $matlab_ref = $ref_map->{$pname};
        my $regex = 
        $species_init =~ s/(^|\W)$pname(\W|$)/$1$matlab_ref$2/g;
    }
    
    return (  join(', ', @species_names), $species_init, $err );
}

sub getMatlabSpeciesNamesOnly
{
	my $slist = shift;
	my $err;
	my @species_names = ();    
    foreach my $species ( @{ $slist->Array } )
    	{
    		push @species_names, "'" . $species->SpeciesGraph->StringExact . "'";
    	}
    
    return (  join(', ', @species_names), $err );

}

# Put copy of initial concentrations (as defined in model at @$conc
# If @$conc is non-empty, then check if @$conc is okay (pad with zeros if too short)
#
# $slist->checkOrInitConcentrations( $conc )
sub checkOrInitConcentrations
{
    my $slist = shift @_;
    my $conc  = shift @_;

    # decide to use concentration vector or direct values from species
    if ( @$conc )
    {   # check concentration vector
        if ( @$conc > @{$slist->Array} )
        {   # this case is not well-defined
            exit_error( "SpeciesList->initConcentrations(): concentration array is longer than species array" );
        }
        elsif ( @$conc < @{$slist->Array} )
        {   # pad with zeros
            my $n_zeros = @{$slist->Array} - @$conc;
            push @$conc, (0) x $n_zeros;
        }
    }
    else
    {   # get concentrations from species
        @$conc = map {$_->Concentration} @{$slist->Array};
    }
}


1;
