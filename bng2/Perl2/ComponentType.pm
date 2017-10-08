# $Id: ComponentType.pm,v 1.3 2006/05/01 03:32:43 faeder Exp $

# Objects for typing and checking Components of Molecules
package ComponentType;

use Class::Struct;
use FindBin;
use lib $FindBin::Bin;
#use List::MoreUtils qw(first_index);


#copied from list::moreutils since it doesnt come standard
sub first_index (&@) {
    my $f = shift;
    foreach my $i ( 0 .. $#_ ) {
        local *_ = \$_[$i];
        return $i if $f->();
    }
    return -1;
}


struct ComponentType=> {
  Name=> '$',
  States=> '@', # First element of States array is the default state, which 
                # is used in creating new molecules that don't have user-specified
                # state attributes.
  Edges=> '@',         # Last two are not currently used in checking
  Compartments=> '@'
};
sub readString{
  my $ctype= shift;
  my $strptr= shift;

  my $string_left=$$strptr;
  
  # Get component name (cannot start with a number)
  if ( $string_left =~ s/^([A-Za-z_]\w*)// )
  {
  	$ctype->Name($1);
  }
  else
  {
	return undef, "Invalid component name in '$string_left' (must begin with a letter or underscore).";
  }
	
  # Get component state (marked by ~) edges (marked by !)
  my @states=();
  my @compartments=();
  my @elabels=();
  while ($string_left){
    if ($string_left=~ s/^([~!@])([A-Za-z0-9_]+)//){
      my $type=$1;
      my $arg=$2;
      if ($type eq '~'){
	push @states, $arg;
      }
      elsif ($type eq '!'){
	push @elabels, $arg;
      }
      elsif ($type eq '@'){
	push @compartments, $arg;
      }
    }
    elsif ($string_left=~ /^[,)]/){ # Terminator characters for component
      last;
    }
    else {
      return("Invalid syntax at $string_left");
    }
  }

  if (@states){
    $ctype->States([@states]);
  }
  if (@elabels){
    $ctype->Edges([@elabels]);
  }
  if (@compartments){
    $ctype->Compartments([@compartments]);
  }
  
  $$strptr= $string_left;
  return("");
}


sub copy
{
    my $comp = shift;
    
    my $comp_copy = ComponentType::new();    
    $comp_copy->Name( $comp->Name );
    $comp_copy->States( [@{$comp->States}] );
    $comp_copy->Edges( [@{$comp->Edges}] );
    $comp_copy->Compartments( [@{$comp->Compartments}] );            
    
    return $comp_copy;   
}

sub toString{
  my $ctype= shift;
  my $string="";

  $string.= $ctype->Name;
  for my $state (@{$ctype->States}){
    $string.= "~$state";
  }
  return($string);
}

sub toStringSSC{
  my $ctype= shift;
  my $string="";
  $string.= $ctype->Name;
  $string .= "#";
  for my $state (@{$ctype->States}){
    $string.= "=\"$state\"";
  }
  return($string);
}

#####
# information used to create speciestype object in sbml multi
#####
sub toSBMLMultiSpeciesTypeFeatures
{
    my $ctype = shift @_; 
    my $cid = shift @_;
    my $mName = shift @_;
    my $sbmlMultiSpeciesInfo_ref = shift @_;
    my $speciesIdHash_ref = shift @_;
    my $indent = shift @_;

    my $ostring = '';
    my $ststring = '';

    #this parameter should eventually be sent from outside for symmetric components
    my $occur = 1;
    my $fullname = '';
    if (@{$ctype->States}){
      
      my $indent2  = "    ".$indent;
      my $indent3  = "    ".$indent2;
      my $sid = '';


      $ostring.= sprintf("%s<multi:speciesFeatureType multi:id=\"%s\" multi:name=\"%s\" multi:occur=\"%d\">\n", $indent, $cid, $ctype->Name, $occur);
      $ostring.= $indent2."<multi:listOfPossibleSpeciesFeatureValues>\n";
      my $exists;
      for my $state (@{$ctype->States}){
        $fullname = sprintf("%s(%s~", $mName, $ctype->Name). "${state})";
        $exists = first_index {$_  eq $cid } @{$speciesIdHash_ref->{'Components'}{$fullname}};
        if ($exists == -1){
          push @{$speciesIdHash_ref->{'Components'}{$fullname}}, $cid;
        }
        $sid = sprintf("%s_%s",$cid, $state);
        $ostring .= sprintf("%s<multi:possibleSpeciesFeatureValue multi:id=\"%s\" multi:name=\"%s\"/>\n",$indent3, $sid, $state);
      }
      $ostring.= $indent2."</multi:listOfPossibleSpeciesFeatureValues>\n";
      $ostring.= $indent . "</multi:speciesFeatureType>\n";
    }
    return $ostring;
}

sub toSBMLMultiSpeciesTypeBinding
{
    my $ctype = shift @_; 
    my $cid = shift @_;
    my $mName = shift @_;
    my $sbmlMultiSpeciesInfo_ref = shift @_;
    my $speciesIdHash_ref = shift @_;
    my $indent = shift @_;

    my $ostring = '';
    my $fullname = sprintf("%s(%s)", $mName, $ctype->Name);
    $ostring .= sprintf("%s<multi:speciesTypeInstance multi:id=\"%s_ist\" multi:name=\"%s\" multi:speciesType=\"%s\"/>\n", $indent, $cid, $ctype->Name, $cid);
    my $strstring;

    # pure binding site
    if (!@{$ctype->States}){
      $ststring = sprintf("<multi:bindingSiteSpeciesType multi:id=\"%s\" multi:name=\"%s\"/>\n", $cid, $ctype->Name);
    }
    # this binding site also has states!
    else{
      $ststring = sprintf("<multi:bindingSiteSpeciesType multi:id=\"%s\" multi:name=\"%s\">\n", $cid, $ctype->Name);
      my $featuretypes .= $ctype->toSBMLMultiSpeciesTypeFeatures($cid."_ft",$mName,$sbmlMultiSpeciesInfo_ref,$speciesIdHash_ref,$indent);

      if(! $featuretypes eq ''){
        $ststring .= $indent. "<multi:listOfSpeciesFeatureTypes>\n";
        $ststring .= $featuretypes;
        $ststring .= $indent. "</multi:listOfSpeciesFeatureTypes>\n";

      }
      $ststring .= $indent. "</multi:bindingSiteSpeciesType>\n";
    }
    my $exists = first_index {$_  eq $cid } @{$speciesIdHash_ref->{'Components'}{$fullname}};
    if ($exists == -1){
      push @{$speciesIdHash_ref->{'Components'}{$fullname}}, $cid;  
    }
    push @{$sbmlMultiSpeciesInfo_ref->{$cid}}, $cid;
    push @{$sbmlMultiSpeciesInfo_ref->{$cid}}, $ststring;


    return $ostring;
}


#####
#####
#####
#####

sub toXML{
  my $ctype= shift;
  my $indent= shift;
  my $string=$indent."<ComponentType";

  # Attributes
  # id
  $string.=" id=\"".$ctype->Name."\"";

  # Objects
  my $indent2= "  ".$indent;
  my $ostring="";
  if (@{$ctype->States}){
    $ostring.= $indent2."<ListOfAllowedStates>\n";
    for my $state (@{$ctype->States}){
      $ostring.=$indent2."  <AllowedState";
      $ostring.=" id=\"".$state."\"";
      $ostring.="/>\n";
    }
    $ostring.= $indent2."</ListOfAllowedStates>\n";
  }

  if ($ostring){
    $string.=">\n";
    $string.=$ostring;
    $string.=$indent."</ComponentType>\n";
  } else {
    $string.="/>\n";
  }

  return($string);
}

sub writeMDL
{
    my $ctype = shift; 
    my $string = $ctype->Name; 
    
    if (@{$ctype->States}){
       foreach my $state (@{$ctype->States}){
              $string .= "~".$state; 
	      }
     }
    return $string; 
}


1;
