# Objects for typing and checking Molecules
package MoleculeType;

# pragmas
use strict;
use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;

# BNG Modules
use Molecule;
use ComponentType;



struct MoleculeType =>
{
    Name           => '$',
    Components     => '@',     # Array of ComponentTypes
    PopulationType => '$', # Is this a population type molecule?
};


###
###
###


# read a molecule type from the Molecle Types block
sub readString
{
    my $mtype  = shift @_;
    my $strptr = shift @_;

    my $string_left = $$strptr;

    # Get molecule name (alphanumeric ID, must begin with a letter or underscore)
	if ( $string_left =~ s/^([A-Za-z_]\w*)// )
    {
        my $name= $1;
        $mtype->Name($1);
    }
    else
    {
        return ("Invalid MoleculeType name at '$$strptr' (must begin with a letter or underscore)." );
    }

    # By default, set population tag false
    $mtype->PopulationType(0);

    # Get molecule state (marked by ~) edges (marked by !) and label (marked by
    # %) and components (enclosed in ())
    my @elabels=();
    my @components=();
    while($string_left)
    {
        # Read components in parentheses
        if ( $string_left =~ s/^[(]// )
        {
            if ( @components )
            {
	            return("Multiple component definitions");
            }
            
            while ( $string_left )
            {
	            # Continue characters
	            if ( $string_left =~ s/^[,.]// )
	            {
	                next;
	            }
	            # Stop characters
	            elsif ( $string_left =~ s/^[)]\s*// )
	            {
	                last;
	            }
                # Read
	            else
	            {
	                my $comp = ComponentType->new;
	                my $err  = $comp->readString(\$string_left);
	                if ($err) {  return ($err);  }
	                push @components, $comp;
	            }
            }
        }
        # Strip white space
        # (NOTE: this used to be a stop character.  Will removing this cause problems?  --Justin, 7mar2011)
        elsif ( $string_left =~ s/^\s+// )
        {   
            # nothing to do here
        }
        # Stop characters
        elsif ( $string_left =~ /^[,.]|[<]?-[>]/ )
        {
            last;
        }
        # Look for population tag
        elsif ( $string_left =~ s/^population\s*//i ) 
        {
            $mtype->PopulationType(1);
        }
        else
        {
            return ("Invalid MoleculeType specification $$strptr");
        }
    }

    if (@components)
    {
        $mtype->Components([@components]);
    }
    $$strptr = $string_left;

    return '';
}


###
###
###

sub copy
{
    my $mt = shift;
    
    my $mt_copy = MoleculeType::new();
    $mt_copy->Name( $mt->Name );    
    foreach my $comp ( @{$mt->Components} )
    {
        push @{$mt_copy->Components}, $comp->copy();
    }    
    $mt_copy->PopulationType( $mt->PopulationType );
    
    return $mt_copy;
}


###
###
###


sub add
{
    my $mtype = shift @_;
    my $mol   = shift @_;

    $mtype->Name($mol->Name);
    my @ctarray = ();
    foreach my $comp (@{$mol->Components})
    {
        my $ctype=ComponentType->new( Name=>$comp->Name );
        # The first entry in the States array becomes the default state value
        my $state = $comp->State;
        if (defined $state and  $state ne '')
        {
            $ctype->States(0, $state);
        }
        push @ctarray, $ctype;
    }
    $mtype->Components([@ctarray]);
    return '';
}



###
###
###



sub check
{
    my $mtype  = shift @_;
    my $mol    = shift @_;
    my $params = @_ ? shift @_ : {};

    my $IsSpecies            = exists $params->{IsSpecies}     ? $params->{IsSpecies}     : 1;
    my $AllowNewStates       = exists $params->{AllowNewTypes} ? $params->{AllowNewTypes} : !$IsSpecies;
    my $AllowPartial         = !$IsSpecies;
    my $AllowWildcard        = !$IsSpecies;
    my $AllowUndefinedStates = !$IsSpecies;
    my $InheritList          = exists $params->{InheritList}   ? $params->{InheritList}   : 0;

    my @ctypes = @{$mtype->Components};

    foreach my $comp (@{$mol->Components})
    {
        my $found=0;
        my $index=0;
        # Check for match for each component
        foreach my $comp_type (@ctypes)
        {
            if ($comp->Name eq $comp_type->Name)
            {
    	        my $state = $comp->State;
    	        if ( !defined $state )
                {
    	            # If component state is undefined, check whether component states have been declared, meaning this 
    	            # component should not be stateless, unless AllowUndefinedStates is true
    	            my $InheritState = ($InheritList) ? $InheritList->[$index] : 0;
    	            #print "IS= $InheritState\n";
    	            if (@{$comp_type->States} && !$InheritState && !$AllowUndefinedStates)
                    {
    	                my $err= sprintf "State of component %s of molecule %s must be set", 
    	                $comp->Name, $mol->toString(); 
    	                return($err);
    	            }
	            } 
	            elsif ($state=~/^[?]$/)
                {
	                unless ($AllowWildcard)
                    {
	                    my $err="May not use wildcard for component state in species.";
	                    return $err;
	                }
	            }
	            else
                {
	                # If component state is defined, check whether no component states have been declared, meaning this
	                # component should be stateless
	                unless (@{$comp_type->States})
                    {
	                    my $err = sprintf "Component %s of molecule %s does not accept states", 
	                    $comp->Name, $mol->toString(); 
	                    return $err;
	                }
	  
	                # Check that $state matches allowed states for this component
	                my $sfound=0;
	                foreach my $comp_state (@{$comp_type->States})
                    {
	                    if ($state eq $comp_state)
                        {
	                        $sfound=1;
	                        last;
	                    }
	                }
	                unless ($sfound)
                    {
	                    if ($AllowNewStates)
                        {
	                        printf "Adding %s as allowed state of component %s of molecule %s\n", 
	                               $comp->State, $comp->Name, $mol->Name; 
	                        push @{$comp_type->States}, $comp->State;
	                    }
                        else
                        {
	                        my $err= sprintf "Component state %s of component %s of molecule %s not"
                                             . "defined in molecule declaration",
                                             $comp->State, $comp->Name, $mol->toString(); 
	                        return($err);
	                    }
	                } 
	            }

                if ( $IsSpecies )
                {   # check for bond wildcards
                    if ( grep {$_ =~ /^[+?]$/} @{$comp->Edges} )
                    {
                        my $err = sprintf "Component %s of molecule %s has an illegal bond wildcard.",
                                             $comp->Name, $mol->toString(); 
	                    return $err;
                    }
                }
                
	            # Delete component type from search array
	            splice @ctypes, $index, 1;
	            $found = 1;
	            last;
            } 
            ++$index;
        }
        unless ($found)
        {
            my $err = sprintf "Component %s of molecule %s not found in molecule declaration", $comp->Name, $mol->toString();
            return $err;
        }
    }

    # Incomplete specification of molecule components
    if (!$AllowPartial and @ctypes)
    {
        my $names = "";
        foreach my $ct (@ctypes)
        {
            $names .= " ".$ct->Name;
        }
        my $err = sprintf "Component(s)${names} missing from molecule %s", $mol->toString();
        return $err;
    }

    return '';
}



###
###
###



sub toString
{
    my $mtype  = shift;
    my $max_length = (@_) ? shift : 0;

    # get name
    my $string = $mtype->Name; 
    # get components
    my @cstrings = ();
    foreach my $comp ( @{$mtype->Components} )
    {
        push @cstrings, $comp->toString();
    }
    $string .= '(' . join(',', @cstrings) . ')';
    
    if ( $max_length )
    {   # pad with extra spaces so things line up
        my $string_len = length $string;
        if ( $string_len <= $max_length )
        {
            $string .= ' ' x ($max_length - $string_len);
        }
    }

    # write population tag is not written to output!
    if ( $mtype->PopulationType )
    {   $string .= "  population";   }

    return $string;
}


###
###
###


sub toStringSSC
{
    my $mtype  = shift;
    my $string = '';

    $string .= $mtype->Name;

    my @cstrings;
    @cstrings = ();
    foreach my $comp (@{$mtype->Components})
    {
        push @cstrings, $comp->toStringSSC();
    }
    $string .= '(' . join(',', @cstrings) . ')';

    # NOTE: SSC won't recognize population tag, so dont output. --Justin
    return $string;
}


#####
# information used to create speciestype object in sbml multi
#####
sub toSBMLMultiSpeciesType
{
    my $mtype = shift @_;
    my $sid = shift @_;
    my $mid = shift @_;    
    my $index = shift @_;
    my $indent = shift @_;
    my $sbmlMultiSpeciesInfo_ref = shift @_;
    my $speciesIdHash_ref = shift @_;

    

    # appends the length of the $sbmlMultiSpeciesInfo_ref hash
    my $stid = "ST_M" . keys(%$sbmlMultiSpeciesInfo_ref);
    my $stinstance_string = '';
    if (exists $sbmlMultiSpeciesInfo_ref->{$mtype->toString()} ){
        $stid = $sbmlMultiSpeciesInfo_ref->{$mtype->toString()}[0];
        $stinstance_string = sprintf("%s<multi:speciesTypeInstance multi:id=\"%s\" multi:name=\"%s\" multi:speciesType=\"%s\"/>\n",$indent, $mid, $mtype->Name, $stid);

    }else{
        $stinstance_string = sprintf("%s<multi:speciesTypeInstance multi:id=\"%s\" multi:name=\"%s\" multi:speciesType=\"%s\"/>\n",$indent, $mid, $mtype->Name, $stid);
    }
    my $st_string = sprintf("<multi:speciesType multi:id=\"%s\" multi:name=\"%s\" multi:compartment=\"%s\">\n", $stid, $mtype->Name, "cell");

    # add id information

    $speciesIdHash_ref->{'Molecules'}->{$mtype->Name} = $stid;
    
    push @{$speciesIdHash_ref->{'References'}->{$sid}->{'Molecules'}->{$stid}}, sprintf("cmp_%s_M", $sid) . "$index";
    push @{$speciesIdHash_ref->{'References'}->{$sid}->{'moleculeReverseReferences'}->{$mtype->Name()}}, $mid ;

    if (@{$mtype->Components})
    {
        my $indent2 = "   ".$indent;
        my $counter = 1;

        my $tempstr = '';
        my $cid = '';

        
        my $tmp_string = '';
        
        foreach my $comp (@{$mtype->Components})
        {
            $cid = sprintf("%s_C%d",$stid, $counter);
            # only check things that are explicit binding sites
            if (exists $speciesIdHash_ref->{'BindingInformation'}{$mtype->Name}{$comp->Name}){
                # store the species id for use in the species section
                $tempstr = $comp->toSBMLMultiSpeciesTypeBinding($cid, $mtype->Name, $sbmlMultiSpeciesInfo_ref, $speciesIdHash_ref, $indent2);
                if($tempstr ne ''){
                    push @{$speciesIdHash_ref->{'References'}->{$sid}->{'reverseReferences'}->{sprintf("%s(%s~", $mtype->Name, $comp->Name)}}, $mid . "_C${counter}";
                    push @{$speciesIdHash_ref->{'References'}->{$sid}->{'reverseReferences'}->{sprintf("%s(%s)", $mtype->Name, $comp->Name)}}, $mid . "_C$counter";
                    $speciesIdHash_ref->{'References'}->{$sid}->{'Components'}->{$mid . "_C$counter"}->{'id'} = sprintf("cmp_%s_M", $sid) . "$index" ."_C$counter";
                    $speciesIdHash_ref->{'References'}->{$sid}->{'Components'}->{$mid . "_C$counter"}->{'parent'} = $mid;
                    $speciesIdHash_ref->{'References'}->{$sid}->{'Components'}->{$mid . "_C$counter"}->{'parentMulti'} = $cid;

                    $tmp_string .= $tempstr;
                }
            }
            $counter += 1;    
            
        }
        $counter = 1;
        foreach my $comp (@{$mtype->Components}){
            $cid = sprintf("%s_C%d",$mid, $counter);
            $counter +=1;
        }
        unless ($tmp_string eq ''){
            $st_string .= $indent."<multi:listOfSpeciesTypeInstances>\n";    
            $st_string .= $tmp_string;
            $st_string .= $indent."</multi:listOfSpeciesTypeInstances>\n";
        }
        

        $counter = 1;
        my $featuretypes ='';
        foreach my $comp (@{$mtype->Components})
        {
            # only check things that are not binding sites. Hybrids are alraedy accoutned for
            if (! exists $speciesIdHash_ref->{'BindingInformation'}{$mtype->Name}{$comp->Name}){

                $cid = sprintf("%s_C%d",$stid, $counter);

                $tempstr = $comp->toSBMLMultiSpeciesTypeFeatures($cid, $mtype->Name, $sbmlMultiSpeciesInfo_ref, $speciesIdHash_ref, $indent2);
                if($tempstr ne ''){
                    push @{$speciesIdHash_ref->{'References'}->{$sid}->{'reverseReferences'}->{sprintf("%s(%s~", $mtype->Name, $comp->Name)}}, $mid . "_C${counter}";
                    push @{$speciesIdHash_ref->{'References'}->{$sid}->{'reverseReferences'}->{sprintf("%s(%s)", $mtype->Name, $comp->Name)}}, $mid . "_C$counter";
                    #$speciesIdHash_ref->{'References'}->{$sid}->{'Components'}->{$mid . "_C$counter"}->{'id'} = sprintf("%sI_M", $sid) . "$index" ."_C$counter";
                    #$speciesIdHash_ref->{'References'}->{$sid}->{'Components'}->{$mid . "_C$counter"}->{'parent'} = $mid;
                    #$speciesIdHash_ref->{'References'}->{$sid}->{'Components'}->{$mid . "_C$counter"}->{'parentMulti'} = $cid;
                    $featuretypes .= $tempstr;
                }
            }
            $counter += 1;
            
        }
      if(! $featuretypes eq ''){
        $st_string .= $indent. "<multi:listOfSpeciesFeatureTypes>\n";
        $st_string .= $featuretypes;
        $st_string .= $indent. "</multi:listOfSpeciesFeatureTypes>\n";

      }





    }


    $st_string .= $indent. "</multi:speciesType>\n";
    if (! exists $sbmlMultiSpeciesInfo_ref->{$mtype->toString()} ){

        push @{$sbmlMultiSpeciesInfo_ref->{$mtype->toString()}}, $stid;
        push @{$sbmlMultiSpeciesInfo_ref->{$mtype->toString()}}, $st_string;
    }
    return $stinstance_string;
}

###
###
###


sub toXML
{
    my $mtype  = shift;
    my $indent = shift;
    my $string = $indent."<MoleculeType";

    # Attributes
    # id
    $string.=" id=\"".$mtype->Name."\"";
    # population type
    if ( $mtype->PopulationType ) { $string .= " population=\"1\""; }

    # Objects
    my $indent2 = '  '.$indent;
    my $ostring = '';
    # Component list
    if (@{$mtype->Components})
    {
        $ostring .= $indent2."<ListOfComponentTypes>\n";
        foreach my $comp (@{$mtype->Components})
        {
            $ostring .= $comp->toXML("  ".$indent2);
        }
    
        $ostring .= $indent2."</ListOfComponentTypes>\n";
    }

    if ($ostring)
    {
        $string .= ">\n"; # terminate tag opening
        $string .= $ostring;
        $string .= $indent."</MoleculeType>\n";
    }
    else
    {
        $string .= "/>\n"; # short tag termination
    }

    return $string;
}

sub writeMDL
{
    my $mtype  = shift;
    my $string = "";
    
    $string = $mtype->Name; 
    $string .= "("; 
    
    if (@{$mtype->Components})
    {
       my $k = 0; 
       foreach my $comp (@{$mtype->Components})
       {
              if ($k)
	      {$string .=",";}
	      $string .= $comp->writeMDL();
	      $k = 1;  
       }
    
    }
    $string .= ")"; 
    return $string;
}


1;
