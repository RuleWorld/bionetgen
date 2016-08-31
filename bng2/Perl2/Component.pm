# $Id: Component.pm,v 1.10 2007/01/22 19:22:48 faeder Exp $

package Component;

# pragmas
use strict;
use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;

# BNG Modules
use BNGUtils;
use Compartment;

# constants
use constant { TRUE                 => 1,
               FALSE                => 0
             };


struct Component => {
    Name  => '$',
    State => '$',
    #Type  => 'ComponentType',
    Edges => '@',
    Label => '$', 
    Compartment => 'Compartment'
};



###
###
###



sub newComponent
{
    my $strptr = shift;

    my $comp = Component->new();
    my $err = $comp->readString($strptr);

    return ($comp, $err);
}



###
###
###



sub readString
{
    my $comp   = shift @_;
    my $strptr = shift @_;
    my $clist  = @_ ? shift @_ : undef;

    my $string_left = $$strptr;
    
    # Get component name (cannot start with a number)
	if ( $string_left =~ s/^([A-Za-z_]\w*)// )
	{
		$comp->Name($1);
	}
	else
	{
		return undef, "Invalid component name in '$string_left' (must begin with a letter or underscore).";
	}

    # Get component state (marked by ~) edges (marked by !) and label (marked by %) 
    my $edge_wildcard = 0;
    my $edge_labels = 0;
    while ($string_left)
    {
        if ( $string_left =~ s/^([~%!@])(\w+|\+|\?)// )
        {
            my $type = $1;
            my $arg  = $2;
            if ($type eq '~')
            {
                # State label
	            if (defined $comp->State)
                {   return undef, "Multiple state definitions";   }
	            $comp->State($arg);
            }
            elsif ($type eq '!')
            {
                # Bond label or wildcard
	            if ( $arg =~ /^[+?]$/ )
                { 
	                if ($edge_wildcard or $edge_labels)
                    {   return undef, "Multiple edge wildcards in component";   }
	                $edge_wildcard=1;
	            }
                else
                {
                    if ($edge_wildcard)
                    {   return undef, "Labeled edges mixed with wildcard";   }
                }
	            push @{$comp->Edges}, $arg;
            }
            elsif ($type eq '%')
            {
                # Tag label
	            if (defined $comp->Label)
                {   return undef, "Multiple label definitions";   }
	            $comp->Label($arg);
            }
            elsif ($type eq '@')
            {
                # Compartment label
	            if (defined $comp->Compartment)
                {   return undef, "Multiple compartment definitions";   }

                if ( defined $clist )
                {
				    if ( my $compart = $clist->lookup($arg) )
                    {   $comp->Compartment($compart);   }
                }
				else
                {
					return undef, "Undefined compartment $arg";
				}
            }
        }
        elsif ( $string_left =~ /^[,)]/ )
        {   # Terminator characters for component
            last;
        }
        else
        {   return undef, "Invalid syntax at $string_left";   }
    }

    $$strptr = $string_left;
    return '';
}



###
###
###



sub toString
{
    my $comp = shift @_;
    my $print_edges      = @_ ? shift @_ : TRUE;
    my $print_attributes = @_ ? shift @_ : TRUE;

    my $string .= $comp->Name;

    if (defined $comp->State )
    {   $string .= sprintf "~%s", $comp->State;   }

    if ($print_attributes)
    {   $string .= sprintf("%%%s", $comp->Label)  if (defined $comp->Label);   }

    if ( defined $comp->Compartment )
    {   $string .= sprintf "@%s", $comp->Compartment->Name;   }

    if (defined $comp->Edges )
    {
        if ($print_edges)
        {
            my $wildcard='';
            foreach my $edge (@{$comp->Edges})
            {
                if ($edge=~ /^\d+$/)
                {   $string .= sprintf "!%d", $edge+1;   }
                else
                {   $wildcard = "!$edge";   }
            }
            $string .= $wildcard;
        } 
        else
        {   $string .= "!" x @{$comp->Edges};   }
    }

    return $string;
}

sub toSBMLMultiSpeciesTypeString
{
    my $comp = shift @_;
    my $attributesString = '';
    my $bondsString = '';
    if (defined $comp->State )
    {   
        $attributesString .= " ";
        #$attributesString .= sprintf "~%s", $comp->State;   
    }
    if (defined $comp->Edges )
    {
        foreach my $edge (@{$comp->Edges})
        {
            if ($edge=~ /^\d+$/)
            {   $bondsString .= sprintf "!%d", $edge+1;   }
            else
            {   
                # we dont care about wildcards for sbml multi species types
                return '';
            }
        }
    }

    # we might have to do something about multiplebonds/states
    my $tailStr = $attributesString . $bondsString;
    if($tailStr eq ''){
        return ''
    }
    else{
        return $comp->Name . $attributesString . $bondsString;    
    }
    
}



###
###
###



sub toStringSSC
{
    my $comp        = shift @_;
    my $comp_label  = @_ ? shift @_: 0;
    my $print_edges = @_ ? shift @_ : TRUE;

    my $string      = $comp->Name;
    if ( $comp_label != 0 ) { $string .= $comp_label; } # for checking components with same name
                                                        # if comp_label != 0 same component exists

    if (defined $comp->Label)
    {   printf STDERR "ERROR: SSC Does not implement Label \n";   }

    if (defined $comp->Compartment)
    {   printf STDERR "ERROR: Compartments not implemented \n";   }

    if (defined $comp->State)
    {
        $string.= sprintf "=\"%s\"", $comp->State;
        if ($comp_label ==0 )
        {   # just appends <component name>Binds, as SSC doesnt allow states to bind
            $string .= ", " . $comp->Name . "Binds";
        }
        else
        {
            $string .= ", " . $comp->Name . $comp_label ."Binds";
        }
    }

    if (defined $comp->Edges)
    {
        $string .= "#"; #As SSC considers every non-bound component as empty, so attaching #
        if ($print_edges)
        {
            my $wildcard="";
            foreach my $edge (@{$comp->Edges})
            {
                if ($edge=~ /^\d+$/)
                {
                    $string.= sprintf "%d", $edge+1;
                }
                else
                {
                    $wildcard="$edge";
                }
                $string.=$wildcard;
            }
        }
        else
        {
            $string.= ""x scalar(@{$comp->Edges});
        }
    }
    $string =~ s/\+/_/; # SSC equivalent of '+' is '_', substituting that
    return $string;
}





########
# returns information for the  outwardBindingSite belonging to species in sbml multi
######
sub getSBMLMultiOutwardBonds
{
    my $comp   = shift @_;
    my $indent = shift @_;
    my $mName     = shift @_;
    my $index  = shift @_;
    my $speciesIdHash_ref = shift @_;
    my $multiComponentHash_ref = shift @_;

    my $string = "";
    my $fullstring = sprintf("%s(%s)",$mName, $comp->Name);
    my $externalId = $speciesIdHash_ref->{'Components'}->{$fullstring};


    # Attributes
    # id

    # NumberOfBonds
    my $nbonds = 0;
    my $bindingstatus = '';
    foreach my $edge (@{$comp->Edges})
    {
        if ($edge=~ /^[?+]$/)
        {
            $bindingstatus = "either";
        }
        else
        {
            ++$nbonds;
        }
    }
    
    if ($nbonds == 0 && $bindingstatus eq '')
    {
        $bindingstatus = "unbound";
    }
    
    if($nbonds > 1 )
    {
        print "Can't handle components with more than one bond, skipping\n";
        return "";
        #sbml multi cant handle components with more than one bond. we should somehow return an error
    }    
    # if($nbonds == 1 && (defined $comp->State)){
    #     print "Can't handle components with states and bonds, skipping\n";
    #     return "";
    # }
    if (! $bindingstatus eq "" )
    {
        #get a component from the component pool
        my $reverseReference = $multiComponentHash_ref->{'reverseReferences'}->{$fullstring}[0];
        my $indent2 = '  ' . $indent;
        #get a speciestype id based on that component
        if(defined $reverseReference){
            my $externalComponentId =  $multiComponentHash_ref->{'Components'}->{$reverseReference}{'id'};
            #we used this component so remove it from the pool
            splice(@{$multiComponentHash_ref->{'reverseReferences'}->{$fullstring}}, 0, 1);

            
            $string = $indent2 . sprintf("<multi:outwardBindingSite multi:bindingStatus=\"%s\" multi:component=\"%s\"/>\n", $bindingstatus, $externalComponentId);
        }
        else{
            $string = $indent2 . sprintf("<multi:outwardBindingSite multi:bindingStatus=\"%s\" multi:component=\"%s\"/>\n", $bindingstatus, $externalId);
        }
        # Objects contained
    }
    return $string;
}

####
# returns a string containg the speciesfeature properties associated with this compartment
# for construction of a sbml species object in an sbml-multi file
#####
sub getSBMLMultiSpeciesFeature
{
    my $comp   = shift @_;
    my $indent = shift @_;
    my $mName     = shift @_;
    my $index  = shift @_;
    my $speciesIdHash_ref = shift @_;
    # list of references to sbml multi species type objects for this species
    my $multiComponentHash_ref = shift @_;
    
    my $string = "";
    # state
    if (defined $comp->State)
    {
        my $fullstring = sprintf("%s(%s~", $mName, $comp->Name). ${comp}->State . ")";
        my $partialtring = sprintf("%s(%s~", $mName, $comp->Name);
        #my $fullstring = sprintf("%s(%s~%s)",$mName, $comp->Name, $comp->State);
        #calculate external id

        #get a component from the component queue
        my $reverseReference = $multiComponentHash_ref->{'reverseReferences'}->{$partialtring}[0];
        my $externalComponentId;

        my $externalId = $speciesIdHash_ref->{'Components'}->{$fullstring};
        my $indent2 = '  ' . $indent;

        if(defined $reverseReference){

            # get the species type id associated with this top level component
            if(exists $multiComponentHash_ref->{'Components'}->{$reverseReference}{'id'}){
                $externalComponentId =  $multiComponentHash_ref->{'Components'}->{$reverseReference}{'id'};
            }
            else{
                # if its a state-only component it doesnt have a explicit speciestype reference so we refer to its parent molecule instead
                # we do it a little hack~ish here by manipulating the $reverseReference id to point to the molecule id instaed
                # this expects an id that follows the sid_mid_cid convention
                my @splitArray = split(/_/, $reverseReference);
                $splitArray[0] ="cmp_" . $splitArray[0];
                $externalComponentId  = join('_', @splitArray[0..$#splitArray-1]);

            }
            #regardless we used up this component hash reference so pop it to flag its been used
            splice(@{$multiComponentHash_ref->{'reverseReferences'}->{$partialtring}}, 0, 1);
            #get an id  to the species type associated component based on the reverse reference
            
            $string = $indent2 . sprintf("<multi:speciesFeature id=\"%s\" multi:speciesFeatureType=\"%s\" multi:occur=\"%d\" multi:component=\"%s\">\n",$index, $externalId, 1, $externalComponentId);
            
        }
        else{
            $string = $indent2 . sprintf("<multi:speciesFeature id=\"%s\" multi:speciesFeatureType=\"%s\" multi:occur=\"%d\">\n",$index, $externalId, 1);
        }
        
        my $indent3 = "  " . $indent2;
        # Objects contained
        my $stateDefStr = $indent3 . "<multi:listOfSpeciesFeatureValues>\n";
        $stateDefStr = $stateDefStr. $indent3 . "    " . sprintf("<multi:speciesFeatureValue multi:value=\"%s\"/>\n",$externalId ."_" .$comp->State);
        $stateDefStr = $stateDefStr. $indent3 . "</multi:listOfSpeciesFeatureValues>\n";

        $string = $string . $stateDefStr;
        $string = $string . $indent2 . "</multi:speciesFeature>\n";
    }
    
    # Attributes
    # id
    
    return $string;
}


###
###
###
sub toXML
{
    my $comp   = shift @_;
    my $indent = shift @_;
    my $id     = shift @_;
    my $index  = (@_) ? shift @_ : '';

    my $string = $indent . "<Component";

    # Attributes
    # id
    my $cid = sprintf "${id}_C%d", $index;
    $string .= " id=\"".$cid."\"";
    # type
    $string .= " name=\"" . $comp->Name . "\"";
    # state
    if (defined $comp->State)
    {
        $string .= " state=\"" . $comp->State . "\"";
    }
    if (defined $comp->Label)
    {
        $string .= " label=\"" . $comp->Label . "\"";
    }
    if ($comp->Compartment)
    {
        $string .= " compartment=\"" . $comp->Compartment->Name . "\"";
    }

    # NumberOfBonds
    my $nbonds = 0;
    my $wildcard = '';
    foreach my $edge (@{$comp->Edges})
    {
        if ($edge=~ /^[?+]$/)
        {
            $wildcard = $edge;
        }
        else
        {
            ++$nbonds;
        }
    }
    if ($wildcard)
    {
        $nbonds = ($nbonds>0) ? $nbonds.$wildcard : $wildcard;
    }
    $string .= " numberOfBonds=\"" . $nbonds . "\"";

    # Objects contained
    my $indent2 = '  ' . $indent;
    my $ostring = '';

    # Termination
    if ($ostring)
    {   # terminate tag opening
        $string .= ">\n";
        $string .= $ostring;
        $string .= $indent . "</Component>\n";
    }
    else
    {   # short tag termination
        $string .= "/>\n"; 
    }

    return $string;
}



###
###
###



# Make an exact copy of a component.
sub copy
{
    # get component that we want to copy
    my $comp = shift @_;
    # should we copy labels?
    my $copy_labels = (@_) ? shift @_ : 1;
    # prefix for edge names?
    my $prefix = (@_) ? shift @_ : '';

    # create a new component
    my $comp_copy = Component->new();

    # copy scalar attributes
    $comp_copy->Name( $comp->Name );
    $comp_copy->Label( $comp->Label ) if ($copy_labels);
    $comp_copy->State( $comp->State );
    $comp_copy->Compartment( $comp->Compartment ) if (defined $comp->Compartment);

    # copy edges
    if ( @{$comp->Edges} )
    {
        # add prefix to edge label, unless its a wildcard
        $comp_copy->Edges( [map {$_=~/^[*+?]$/ ? $_ : $prefix.$_} @{$comp->Edges}] );
    }

    # return the copy component  
    return $comp_copy;
}



###
###
###



# call this method to link Compartments to a new CompartmentList
sub relinkCompartments
{
    my $comp  = shift @_;
    my $clist = shift @_;
    
    my $err;
    unless ( ref $clist eq 'CompartmentList' )
    {   return "Component->relinkCompartments: Error!! Method called without CompartmentList object";   }
    
    if ( defined $comp->Compartment )
    {
        my $new_compart = $clist->lookup( $comp->Compartment->Name );
        unless ($new_compart)
        {   return "Molecule->relinkCompartments: Error!! could not find compartment name in list";   }
        $comp->Compartment( $new_compart );
    }
    
    return undef;
}



###
###
###



# Component comparison for isomorphism
sub compare_local
{
    my $a = shift;
    my $b = shift;

    my $cmp;
    # Component name
    if ( $cmp = ($a->Name cmp $b->Name) )
    {   return $cmp;   }

    # Component state
    if ( defined $a->State )
    {
        unless (defined $a->State) { return 1; }
        if ( $cmp = ($a->State cmp $b->State) )
        {   return $cmp;   }
    }
    elsif ( defined $b->State )
    {
        return -1;
    }

    # Component compartment
    if ( defined $a->Compartment )
    {
        unless (defined $b->Compartment) { return 1; }
        if ( $cmp = ($a->Compartment <=> $b->Compartment) )
        {   return $cmp;   }
    }
    elsif ( defined $b->Compartment )
    {
        return -1;
    }

    # Number of edges
    if ( $cmp = (@{$a->Edges} <=> @{$b->Edges}) )
    {   return $cmp;   }

    return 0;
}



###
###
###

1;
