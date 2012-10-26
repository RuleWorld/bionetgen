# $Id: RxnRule.pm,v 1.20 2007/07/06 06:09:48 faeder Exp $

package RxnRule;

# pragmas
use strict;
use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;
# consider using for reference address comparison?
#use Scalar::Util 'refaddr';

# BNG Modules
use BNGUtils;
use SpeciesGraph;
use RateLaw;
use EnergyPattern;
use SpeciesList;
use MoleculeTypesList;
use RxnList;
use CrossProduct;
use RefineRule;



# TODO: implement TotalRate feature for Network simulations
# (currently implemented only for XML network-free output)
#   --Justin  2dec2010

struct RxnRule =>
{
	Name      => '$',

	Reactants => '@',    # Arrays of SpeciesGraphs
	Products  => '@',    # Arrays of SpeciesGraphs

	Rmatches  => '@',    # Array of Maps onto SpeciesGraphs selected by Reactants SpeciesGraphs

	Rinclude  => '@',    # Array of patterns to use as context for selecting reactants
	Pinclude  => '@',    # Array of patterns to use as context for selecting products
	Rexclude  => '@',    # Array of patterns to exclude from reactant matches
	Pexclude  => '@',    # Array of patterns to exclude from products

	MapF      => '%',    # Map from reactants to products 'rP.rM.rC' -> 'pP.pM.pC'
	MapR      => '%',    # Map from products to reactants 'pP.pM.pC' -> 'rP.rM.rC'

	RateLaw   => 'RateLaw',    # Array of rate laws, 1 if unidirectional, 2 if reversible
    TotalRate => '$',          # indicates whether the ratelaws corresponding to this rule define a TotalRate (1)
                               #   or a PerSite rate (default=0)

	Priority  => '$',
	MultScale => '$',    # Factor by which to scale multiplicity to account for symmetry in rule
    
	# The operations performed by the rule
	EdgeAdd           => '@',
	EdgeDel           => '@',
	MolAdd            => '@',
	MolDel            => '@',
	CompStateChange   => '@',
	ChangeCompartment => '@',

	RRefs => '%',    # Refs to objects on reactants side of rule
	PRefs => '%',    # Refs to objects on products side of rule

	# operation modifiers
	DeleteMolecules => '$', # If True, deleting a molecule in a species will remove only that molecule.
                            # Default is unset, and behavior of deletion is determined automatically.
	MoveConnected   => '$', # If True, transporting a molecule in a species will move all molecules
                            # connected to the first molecule by a path contained in the same compartment.
	ReactionCenter  => '@', # array of reaction centers for each reactant pattern in RxnRule  --justin
	RxnLabels       => '%', # track canonical reaction labels to eliminate reduncant reactions
	
	StringID        => '$'
};


###
###
###


# create copy of a reaction rule
sub copy
{
    my $rr = shift;
    
    my $rr_copy = RxnRule::new();
    $rr_copy->Name( $rr->Name );

    @{$rr_copy->Reactants} = map {$_->copy()} @{$rr->Reactants};
    @{$rr_copy->Products}  = map {$_->copy()} @{$rr->Products};

    my $ir = 0;
    while ( $ir < @{$rr->Reactants} )
    {
        push @{$rr_copy->Rinclude}, [ map {$_->copy()} @{$rr->Rinclude->[$ir]} ];
        push @{$rr_copy->Rexclude}, [ map {$_->copy()} @{$rr->Rexclude->[$ir]} ];
        ++$ir;
    }

    my $ip = 0;
    while ( $ip < @{$rr->Products} )
    {
        push @{$rr_copy->Pinclude}, [ map {$_->copy()} @{$rr->Pinclude->[$ip]} ];
        push @{$rr_copy->Pexclude}, [ map {$_->copy()} @{$rr->Pexclude->[$ip]} ];
        ++$ip;
    }

    %{$rr_copy->MapF} = %{$rr->MapF};
    %{$rr_copy->MapR} = %{$rr->MapR};

    $rr_copy->RateLaw( $rr->RateLaw->copy() ) if ( defined $rr->RateLaw );

    $rr_copy->TotalRate( $rr->TotalRate );
    $rr_copy->Priority( $rr->Priority );
    $rr_copy->MultScale( $rr->MultScale );
    
    @{$rr_copy->EdgeAdd} = map { [@$_] } @{$rr->EdgeAdd};
    @{$rr_copy->EdgeDel} = map { [@$_] } @{$rr->EdgeDel};       
    @{$rr_copy->MolAdd}  = map { [@$_] } @{$rr->MolAdd};            
    @{$rr_copy->MolDel}  = @{$rr->MolDel};
    @{$rr_copy->CompStateChange}   = map { [@$_] } @{$rr->CompStateChange};      
    @{$rr_copy->ChangeCompartment} = map { [@$_] } @{$rr->ChangeCompartment};      

    %{$rr_copy->RRefs} = %{$rr->RRefs};
    %{$rr_copy->PRefs} = %{$rr->PRefs};

    $rr_copy->DeleteMolecules( $rr->DeleteMolecules );
    $rr_copy->MoveConnected( $rr->MoveConnected );

    @{$rr_copy->ReactionCenter} = @{$rr->ReactionCenter};
    %{$rr_copy->RxnLabels} = %{$rr->RxnLabels};

    $rr_copy->StringID( $rr->StringID );

    return $rr_copy;
}



###
###
###


sub resetLabels
{
    my $rr = shift;
    
    %{$rr->RxnLabels} = ();
    $rr->StringID( undef );
}



###
###
###



sub newRxnRule
{
	my $string = shift;
	my $model  = shift;
	
	my ( $err, $sep, $reversible );
	my $name = '';

	my $DeleteMolecules = 0;
	my $MoveConnected   = 0;
	my $TotalRate       = 0;

    # get parameter list, compartment list and molecule type list
	my $plist  = $model->ParamList;
	my $clist  = $model->CompartmentList;
	my $mtlist = $model->MoleculeTypesList;

	# Remove leading whitespace
	$string =~ s/^\s*//;

    # Check for a ReactionRule label or index at the beginning of the string
	if ( $string =~ s/^([\w\s]+):\s*// )
	{
	    # We found an alphanumeric label
		$name = $1;
	}
	elsif ( $string =~ m/^\d+\s+(\+|->|<->)/ )
	{
        # We found a numerical token that appears to be a species pattern (perhaps the null species).
        #  Don't strip this token!
	}
    elsif ( $string =~ s/^(\d+)\s+// )
    {   
        # We found a numerical token that appears to be a Reaction Rule index.
        #  Strip the index and assign it as the name.
        $name = $1;
    }

	if ( $string =~ s/CreateSpecies\s*\(\s*//i )
	{
		# Syntax CreateSpecies(SpeciesGraph, RateLaw)
		# Read SpeciesGraph- checking that it represents a valid species
		my $g = SpeciesGraph->new();
		my $err = $g->readString( \$string, $clist, 1, '^\s*[,]\s*', $mtlist );
		if ($err) { return ( [], $err ) }

		unless ( $string =~ s/^\s*[,]\s*// ) {
			return ( [],
				"Arguments must be separated by , in CreateSpecies: $string" );
		}

		# Read RateLaw
		my $rl;
		( $rl, $err ) = RateLaw::newRateLaw( \$string, $model, 0, [] );
		if ($err) { return ( [], $err ); }

		unless ( $string =~ s/^\s*\)\s*// ) {
			return ( [], "No end parenthesis in CreateSpecies: $string" );
		}

		if ($string) {
			return ( [], "Syntax error in CreateSpecies: $string" );
		}

		return ( [], "CreateSpecies not yet implemented." );

		# What's this?  $rr does not exist yet.  --Justin
		# Create mapping
		# Problem here arising from null Reactant list
		#if ( my $err = $rr->findMap($mtlist) )
		#{  return ( [], $err );  }

		# Construct RxnRule
		my $rr = RxnRule->new();
		$rr->Name($name);
		$rr->Reactants( [] );
		$rr->Products(  [$g] );
		$rr->RateLaw($rl);

		return ( [$rr], '' );
	}
	elsif ( $string =~ s/DeleteSpecies\s*\(\s*//i ) {
		return ( [], "DeleteSpecies not yet implemented." );
	}
	elsif ( $string =~ s/DeleteMolecules\s*\(\s*//i ) {
		return ( [], "DeleteMolecules not yet implemented." );
	}

	# READ REACTION IN STANDARD SYNTAX
	else
	{
		# NOTE: a small parsing bug here when compartments are specified for
		# some, but not all species in the rxn rule.  --Justin.  22feb2009
		# DId I ever fix this??  --Justin  3nov2010.

		# read reactant patterns
		my @reac   = ();
		my %labels = ();
		my %rrefs  = ();
		my %prefs  = ();
		$sep = '^\s*[+]\s*|^\s*([<]?-[>])\s*';    #  "+"  or "->"  or "<->"
		my $g;
		my $ipatt = -1;
		
		# Parse Reactant Patterns
		while ($string)
		{
			# print STDERR " $ipatt : $string\n";
			$g = SpeciesGraph->new();
			$err = $g->readString( \$string, $clist, 0, $sep, $mtlist );
			if ($err) {  return ( [], $err );  }
			
			# check for null species
			#next if ( $g->isNull() );
			
			# increment pattern count!
			++$ipatt;

            # Check Labels for this Reactant:
			if ( defined $g->Label  and  $g->Label ne '' )
			{
				$rrefs{ $g->Label } = $ipatt;
			}

			# Checking Molecules...
			my $imol = -1;
			foreach my $mol ( @{ $g->Molecules } )
			{
				++$imol;
				my $label = $mol->Label;
				if ( defined $label  and  $label ne '' )
				{
					if ( $labels{$label} ) {
						return ( [], "Repeated label $label in reactants of reactant rule" );
					}
					$labels{$label} = "M";
					$rrefs{$label} = join( '.', ( $ipatt, $imol ) );
				}
				my $icomp = -1;
				
				# Checking Components...
				for my $comp ( @{ $mol->Components } )
				{
					++$icomp;
					$label = $comp->Label;
					if ( defined $label  and  $label ne '' )
					{				
						if ( $labels{$label} ) {
							return ( [], "Repeated label $label in reactants of reaction rule" );
						}
						$labels{$label} = "C";
						$rrefs{$label} = join( '.', ( $ipatt, $imol, $icomp ) );
					}
				}
			}
			
			# Save this reactant
			push @reac, $g;
			
			# Find next separator (some problem with string if there is no separator)
			unless ( $string =~ s/$sep// ) {
				return ( [], "Invalid syntax in reaction rule: $string" );
			}
			
			# If the arrow operator was the next separator, then it's captured in $1
			if ($1)
			{   # Check if this reaction is reversible
				$reversible = ( $1 eq "<->" ) ? 1 : 0;
			    # check reversibility in energyBNG mode.  Does this matter?  Maybe issue warning?  --Justin, 9nov2010
			    #if ( !$reversible  and  ($model->Options->{EnergyBNG}) )
			    #{
			    #    return ( [], "All rules must be reversible in energyBNG mode.  Reaction rule $name is not reversible." );
			    #}
				# We're done with Reactants, so exit this loop
				last;
			}
		}


		# Read Product Patterns
		my @prod = ();
		$sep   = '^\s*([+])\s*|^\s+';    # "+" or "  "
		$ipatt = -1;
		while ($string) {

			# print STDERR " $ipatt : $string\n";
			my $g = SpeciesGraph->new();	
			$err = $g->readString( \$string, $clist, 0, $sep, $mtlist );
			if ($err) { return ( [], $err ) }
			++$ipatt;

            # Checking Labels for this Species..
			if ( defined $g->Label  and  $g->Label ne '' )
			{
				$prefs{ $g->Label } = $ipatt;
			}

			# Checking Molecules... (verify correspondence and uniqueness!)
			my $imol = -1;
			foreach my $mol ( @{ $g->Molecules } )
			{
				++$imol;
				my $label = $mol->Label;
				if ( defined $label  and  $label ne '' )
				{
					if ( $labels{$label} eq 'M' )
					{
						$labels{$label} = 'found';
					}
					elsif ( $labels{$label} eq 'found' ) {
						return ( [], "Repeated label $label in products of reaction rule $name"	);
					}
					else {
						return ( [], "Mis- or un-matched label $label in products of reaction rule $name" );
					}
					$prefs{$label} = join( '.', ( $ipatt, $imol ) );
				}
				
				# Checking Components...
				my $icomp = -1;
				for my $comp ( @{ $mol->Components } )
				{
					++$icomp;
					$label = $comp->Label;

					#print "label=$label\n";
					if ( defined $label  and  $label ne '' )
					{
						if ( $labels{$label} eq 'C' )
						{
							$labels{$label} = "found";
						}
						elsif ( $labels{$label} eq "found" ) {
							return ( [], "Repeated label $label in products of reaction rule $name" );
						}
						else {
							return ( [], "Mis- or un-matched label $label in products of reaction rule $name" );
						}
						$prefs{$label} = join( '.', ( $ipatt, $imol, $icomp ) );
					}
				}
			}
			
			# Save this Product pattern
			push @prod, $g;
			
			# Check for next separator (store a "+" separator in $1)
			$string =~ s/$sep//;
			# See if there's more Products to parse..
			last unless $1;
		}
		
		
		# TODO
		# Check for labels found in Reactants but not Products and vice versa...


		# Read rateLaw
		my $rl;
		my @rate_laws = ();


        # Look for TotalRate attribute
        # (default behavior is PerSite rate)
        if ( $string =~ s/(^|\s)TotalRate(\s|$)/$2/ )
        {   $TotalRate = 1;   }

	
		# temporarily add Reference tags as names in the ParamList
		if (%rrefs) {  setRefs( \%rrefs, '', $plist );  }
        # Parse and Create the ratelaw
		( $rl, $err ) = RateLaw::newRateLaw( \$string, $model, $TotalRate, \@reac );
		if ($err) {  return ( [], $err );  }
        # unset temporary names of Reference tags
		if (%rrefs) {  unsetRefs( \%rrefs, $plist );  } 
		# Save the ratelaw
		push @rate_laws, $rl;


		# Reversible reaction requires two valid rate laws
		if ($reversible)
		{
			# Remove separator for backward compatibility
			$string =~ s/^\s*[,]\s*//;
			if (%prefs) { setRefs( \%prefs, '', $plist ); }
			( $rl, $err ) = RateLaw::newRateLaw( \$string, $model, $TotalRate, \@prod );
			if ($err) { return ( [], $err ); }
			if (%prefs) { unsetRefs( \%prefs, $plist ); }
			push @rate_laws, $rl;
		}
		else
		{
			if ( $string =~ /^\s*[,]/ ) {
				return ( [], "Unidirection reaction may have only one rate law" );
			}
		}


        # NOTE1: keywords can be in any order following the ratelaws.

        # NOTE2: made keyword extraction safer by checking for white space characters
        #   or end of string around the keyword.  Also, if keyword is extracted midstring
        #   a space is left in place of keyword.  --Justin 

		# Set optional priority of rule
		my $priority = 0;
		if ( $string =~ s/(^|\s)priority\s*=\s*(\S+)(\s|$)/$3/ ) { $priority = $2; }

		# Set DeleteMolecules attribute
		if ( $string =~ s/(^|\s)DeleteMolecules(\s|$)/$2/ ) { $DeleteMolecules = 1; }

        # Set MoveConnected attribute
        # This flag controls the behavior of a molecule transport rule.
        # If true, then a molecule transport will carry all molecules in the species
        # connected to the first molecule through a path in the same compartment which
        # are NOT named in the pattern.
		if ( $string =~ s/(^|\s)MoveConnected(\s|$)/$2/ ) { $MoveConnected = 1; }


		# Process exclude_reactants/products
		my @Rexclude = ();
		foreach my $reac (@reac)
		{   push @Rexclude, [];   }
		
		my @Pexclude = ();
		foreach my $prod (@prod)
		{   push @Pexclude, [];   }
		
		while ( $string =~ s/exclude_(\w+)(\S+)// )
		{
			my $type = $1;
			my $arg  = $2;
			if ( !( $arg =~ s/^\(// ) ) {
				return ( [],
					"Arguments to exclude_$type must be enclosed in parenthesis"
				);
			}
			if ( !( $arg =~ s/\)$// ) ) {
				return ( [], "exclude_$type missing close parenthesis" );
			}

			# First argument is number of reactant or product
			if ( !( $arg =~ s/^\s*(\d+)\s*[,]// ) ) {
				return ( [],
					"exclude_$type first argument must be index of $type" );
			}
			my $index = $1 - 1;

			if ( $type eq "reactants" ) {
				my $max_index = $#reac;
				if ( $index < 0 || $index > $max_index ) {
					return ( [], "exclude_$type index $index out of range" );
				}
				my $sep = '^,';
				while ($arg) {
					$g = SpeciesGraph->new();
					$err = $g->readString( \$arg, $clist, 0, $sep, $mtlist );
					$arg =~ s/$sep//;
					if ($err) { return ( [], $err ) }
					push @{ $Rexclude[$index] }, $g;
				}
			}
			elsif ( $type eq "products" ) {
				my $max_index = $#prod;
				if ( $index < 0 || $index > $max_index ) {
					return ( [], "exclude_$type index $index out of range" );
				}
				my $sep = '^,';
				while ($arg) {
					$g = SpeciesGraph->new();
					$err = $g->readString( \$arg, $clist, 0, $sep, $mtlist );
					$arg =~ s/$sep//;
					if ($err) { return ( [], $err ) }
					push @{ $Pexclude[$index] }, $g;
				}
			}
			else {
				return ( [], "Unrecognized function exclude_$type" );
			}
		}


		# Process include_reactants/products
		my @Rinclude = ();
		foreach my $reac (@reac)
		{   push @Rinclude, [];   }
		
		my @Pinclude = ();
		foreach my $prod (@prod)
		{   push @Pinclude, [];   }
		
		while ( $string =~ s/include_(\w+)(\S+)// )
		{
			my $type = $1;
			my $arg  = $2;
			if ( !( $arg =~ s/^\(// ) ) {
				return ( [],
					"Arguments to include_$type must be enclosed in parenthesis"
				);
			}
			if ( !( $arg =~ s/\)$// ) ) {
				return ( [], "include_$type missing close parenthesis" );
			}

			# First argument is number of reactant or product
			if ( !( $arg =~ s/^\s*(\d+)\s*[,]// ) ) {
				return ( [],
					"include_$type first argument must be index of $type" );
			}
			my $index = $1 - 1;

			if ( $type eq "reactants" ) {
				my $max_index = $#reac;
				if ( $index < 0 || $index > $max_index ) {
					return ( [], "include_$type index $index out of range" );
				}
				my $sep = '^,';
				while ($arg) {
					$g = SpeciesGraph->new();
					$err = $g->readString( \$arg, $clist, 0, $sep, $mtlist );
					$arg =~ s/$sep//;
					if ($err) { return ( [], $err ) }
					push @{$Rinclude[$index]}, $g;
				}
			}
			elsif ( $type eq "products" ) {
				my $max_index = $#prod;
				if ( $index < 0 || $index > $max_index ) {
					return ( [], "include_$type index $index out of range" );
				}
				my $sep = '^,';
				while ($arg) {
					$g = SpeciesGraph->new();
					$err = $g->readString( \$arg, $clist, 0, $sep, $mtlist );
					$arg =~ s/$sep//;
					if ($err) { return ( [], $err ) }
					push @{ $Pinclude[$index] }, $g;
				}
			}
			else {
				return ( [], "Unrecognized function include_$type" );
			}
		}

        # String should be empty now, otherwise there may be a problem.
		if ( $string =~ /\S+/ ) {
			my $err = sprintf "Syntax error in reaction rule at $string";
			return ( [], $err );
		}
        

        
        # Construct Forward RxnRule
		my @rrs = ();
		my $rr  = RxnRule->new();
		if ( $name ne '' ) {  $rr->Name($name);  }
		$rr->Reactants( [@reac] );
		$rr->Products(  [@prod] );
		$rr->Priority($priority);
		$rr->RateLaw( $rate_laws[0] );
		$rr->Rexclude( [@Rexclude] );
		$rr->Pexclude( [@Pexclude] );
		$rr->Rinclude( [@Rinclude] );
		$rr->Pinclude( [@Pinclude] );
		$rr->TotalRate($TotalRate);
		$rr->DeleteMolecules($DeleteMolecules);
		$rr->MoveConnected($MoveConnected);
		$rr->RRefs( {%rrefs} );
		$rr->PRefs( {%prefs} );


		# debug code: show reactants, products and compartments (if any)
		#print STDERR "\n\nRxn Rule:  ", $rr->toString(), "\n";
		#print STDERR "## Reactants ##\n";
		#foreach my $sg ( @{$rr->Reactants} )
		#{
		#   print STDERR $sg->toString();
		#   if ( defined $sg->Compartment )
		#   {  print STDERR "  Compartment: ", $sg->Compartment->Name, "\n";  }
		#   else
		#   {  print STDERR "  No Compartment\n";  }
		#}
		#print STDERR "## Products ##\n";
		#foreach my $sg ( @{$rr->Products} )
		#{
		#   print STDERR $sg->toString();
		#   if ( defined $sg->Compartment )
		#   {  print STDERR "  Compartment: ", $sg->Compartment->Name, "\n";  }
		#   else
		#   {  print STDERR "  No Compartment\n";  }
		#}

		# Find mapping of reactants onto products
		if ( $err = $rr->findMap($mtlist) ) {
			return ( [], $err );
		}
		push @rrs, $rr;

		#print "rrule: ",$rr->toString(),"\n";

		if ($reversible)
		{
			$rr = RxnRule->new();
			if ( $name ne '' ) { $rr->Name("${name}(reverse)"); }
			$rr->Reactants( [@prod] );
			$rr->Products(  [@reac] );
			$rr->Priority($priority);
			$rr->RateLaw( $rate_laws[1] );
			$rr->Pexclude( [@Rexclude] );
			$rr->Rexclude( [@Pexclude] );
			$rr->Pinclude( [@Rinclude] );
			$rr->Rinclude( [@Pinclude] );
			$rr->TotalRate($TotalRate);
			$rr->DeleteMolecules($DeleteMolecules);
			$rr->MoveConnected($MoveConnected);		
			$rr->RRefs( {%prefs} );
			$rr->PRefs( {%rrefs} );

			# Find mapping of reactants onto products
			if ( $err = $rr->findMap($mtlist) ) {
				return ( [], $err );
			}
			push @rrs, $rr;

			#print "rrule: ",$rr->toString(),"\n";
		}
		return [@rrs];
	}    # END OF STANDARD REACTION SYNTAX

	# Should not get here
	return ( [], "Unknown error in RxnRule::readString" );
}



###
###
###



sub listActions
{
	my $rr  = shift;
	my $out = '';

	#  my %map_reverse=();
	#  for my $s (keys %{$rr->MapF}){
	#    my $t= $rr->MapF->{$s};
	#    $map_reverse{$t}= $s;
	#  }

	# Species Creation
	foreach my $molAddRef ( @{ $rr->MolAdd } )
	{
		my @compInhRefs = @$molAddRef;
		my $molRef      = shift(@compInhRefs);
		my ( $p, $m ) = split /\./, $molRef;
		my $newMol = $rr->Products->[$p]->Molecules->[$m];
		$out .= "  # Add Species " . $newMol->toString() . "\n";
	}

	# State changes
	foreach my $sc ( @{ $rr->CompStateChange } )
	{
		( my $p, my $sR, my $sP ) = @{$sc};
		if ( !defined($sR) )
		{   # print wildcard explicitly
		    $sR = '?';
		}
		$out .= "  # StateChange($p,$sR->$sP)\n";
	}

    # changes by justinshogg@gmail.com
    # 14 Feb 2009
    # notes:
    # Compartment changes
    # format
    # ------
    #  species:    ["ipatt", origin_compartment, destination_compartment, cytosis]
    #  molecule:   ["ipatt.imol", origin_compartment, destination_compartment, cytosis]
    #  component:  ["ipatt.imol.icomp", origin_compartment, destination_compartment, cytosis]
    #
    #  cytosis = 1  if endocytosis (flip orientation of baggage molecules)
    #  cytosis =-1  if exocytosis  (no flip)
    #  cytosis = 0  otherwise
	foreach my $compartment_change ( @{ $rr->ChangeCompartment } )
	{
		my ( $ref, $compartR, $compartP, $cytosis ) = @{$compartment_change};
		( $compartR, $compartP ) = ( $compartR->Name, $compartP->Name );
		$out .= "  # ChangeCompartment($ref,$compartR,$compartP,$cytosis)\n";
	}

	# end changes by Justin

	# Binding
	foreach my $ea ( @{ $rr->EdgeAdd } )
	{
		( my $p1, my $p2 ) = @{$ea};
		$out .= "  # Bind($p1,$p2)\n";

		#    $p1= $map_reverse{$p1};
		#    $p2= $map_reverse{$p2};
		#    $out.= "# Bind($p1,$p2) (reactants)\n";
	}

	# Dissociation
	foreach my $ed ( @{ $rr->EdgeDel } )
	{
		( my $p1, my $p2 ) = @{$ed};
		$out .= "  # Unbind($p1,$p2)\n";
	}

	# Molecule Deletion
	foreach my $molRef ( @{ $rr->MolDel } )
	{
		my ( $p, $m ) = split /\./, $molRef;
		if ( defined($m) ) {
			$out .= "  # Delete Molecule "
			  . $rr->Reactants->[$p]->Molecules->[$m]->toString() . "\n";
		}
		else {
			$out .=
			  "  # Delete Pattern " . $rr->Reactants->[$p]->toString() . "\n";
		}
	}

	return ($out);
}


##
##



# write RxnRule to a string
sub toString
{
	my $rr     = shift;
	my $rr_rev = (@_) ? shift : undef;
	my $string = '';

	# Reaction name
	# Name is an index unless it contains non-digit character
	if ( defined $rr->Name  and  $rr->Name ne '' )
	{
	    if ( $rr->Name =~ /\D/ )
	    {
		    $string .= $rr->Name . ":  ";
	    }
	    else
	    {
		    $string .= sprintf "%5d: ", $rr->Name;
	    }
	}
	
	my $i = 0;
	foreach my $g ( @{$rr->Reactants} )
	{
		if ( $i != 0 ) { $string .= " + "; }
		$string .= $g->toString();
		++$i;
	}
	$string .= ($rr_rev) ? " <-> " : " -> ";

	$i = 0;
	foreach my $g ( @{$rr->Products} )
	{
		if ( $i != 0 ) { $string .= " + "; }
		$string .= $g->toString();
		++$i;
	}

	$string .= "  " . $rr->RateLaw->toString();
	if ($rr_rev)
	{
		$string .= ", " . $rr_rev->RateLaw->toString();
	}

	if ( $rr->Priority != 0 )
	{
		$string .= sprintf " priority=%d", $rr->Priority;
	}

	foreach my $i ( 0 .. $#{$rr->Rexclude} )
	{
		next unless ( @{ $rr->Rexclude->[$i] } );
		$string .= " exclude_reactants(";
		$string .= $i + 1;
		foreach my $g ( @{ $rr->Rexclude->[$i] } )
		{
			$string .= ',' . $g->toString();
		}
		$string .= ")";
	}

	foreach my $i ( 0 .. $#{$rr->Pexclude} )
	{
		next unless ( @{ $rr->Pexclude->[$i] } );
		$string .= " exclude_products(";
		$string .= $i + 1;
		foreach my $g ( @{ $rr->Pexclude->[$i] } )
		{
			$string .= ',' . $g->toString();
		}
		$string .= ")";
	}

	foreach my $i ( 0 .. $#{$rr->Rinclude} )
	{
		next unless ( @{ $rr->Rinclude->[$i] } );
		$string .= " include_reactants(";
		$string .= $i + 1;
		foreach my $g ( @{ $rr->Rinclude->[$i] } )
		{
			$string .= ',' . $g->toString();
		}
		$string .= ")";
	}

	foreach my $i ( 0 .. $#{$rr->Pinclude} )
	{
		next unless ( @{ $rr->Pinclude->[$i] } );
		$string .= " include_products(";
		$string .= $i + 1;
		foreach my $g ( @{ $rr->Pinclude->[$i] } )
		{
			$string .= ',' . $g->toString();
		}
		$string .= ")";
	}

	if ( $rr->TotalRate )
	{
		$string .= " TotalRate";
	}

	if ( $rr->DeleteMolecules )
	{
		$string .= " DeleteMolecules";
	}

	if ( $rr->MoveConnected )
	{
		$string .= " MoveConnected";
	}

	return $string;
}



###
###
###



sub toStringSSC
{
	my $rr          = shift;              
	my $rr_rev      = (@_) ? shift : 0;
	my $pstring     = "";
	my $rstring     = "";
	my $rstring2    = "";
	my $finalString = "";
	my $errmsg      = 0;
	my %pattern1; #A hash which stores molecue index for pattern1 as value and key as $mol_index
	my %pattern2;
	my $mol_index = 1;
	my $seen_bond = 0;    #This keeps track of the highest edge reached in a molecule
	my $checkEdge = 0; #equated to 1 to resolve the molecule that are identified with same edge
	my $delMolCase = 0;
	my @sameReac1;    #while permuting the ractants which do not need any permutation
	my %combinations1;    #stores the permuted reactant as value, with key as component
	my @sameReac2;
	my %combinations2;
	my %prod1;   #stores the permutation of the product as value, with compoennt as key
	my %prod2;
	my $expR1 = 0;    #index to increment 1st reactant permutations
	my $expR2 = 0;
	my $expP1 = 0;    #index to increment ist product permutations
	my $expP2 = 0;
	my $sameProd1;    #string storing not permutated products
	my $sameProd2;

	# --Molecule Deletion
	my $i = 0;
	my $curLabel;
	for my $molRef ( @{ $rr->MolDel } ) {
		my ( $p, $m ) = split /\./, $molRef;
		if ( $i != 0 ) { $pstring .= ";"; }
		if ( defined $m )
		{
			my $delMol = $rr->Reactants->[$p]->Molecules->[$m]->toStringSSC();
			$pstring .= "destroy ";
			$delMolCase = 1;
		}
		else
		{
			my $delMol = $rr->Reactants->[$p]->toStringSSC();
			$pstring .= "destroy complex ";
			$delMolCase = 2;

		    # SSC needs a tagged molecule for the deletion, so we'll pick
		    # the first one in the pattern. --Justin
		    $m = 0;
		}

        # just identifying patterns, putting them in teh same hash and giving the labels
        # also checks if reaction is on same molecule, assign it as a different label
	    if ( $p == 0 ) {
		    if ( exists $pattern1{$m} ) {
		    	$curLabel = $pattern1{$m};
		    	++$mol_index;
		    }
		    else {
		    	$pattern1{$m} = $mol_index;
		    	$curLabel = $mol_index;
		    	++$mol_index;
		    }
	    }
	    if ( $p == 1 ) {
		    if ( exists $pattern2{$m} ) {
		    	$curLabel = $pattern2{$m};
		    	++$mol_index;   # TODO: missing increment here, is this correct now?  --Justin, 15dec2010
		    }
		    else {
		    	$pattern2{$m} = $mol_index;
		    	$curLabel = $mol_index;
		    	++$mol_index;
    	    }
        }

		++$i;
		$pstring .= "Mol" . $curLabel;
	}

	# --Species Creation
	for my $molAddRef ( @{ $rr->MolAdd } ) {
		if ( $i != 0 ) { $pstring .= ";"; }
		my @compInhRefs = @$molAddRef;
		my $molRef      = shift(@compInhRefs);
		my ( $p, $m ) = split /\./, $molRef;
		my $newMol = $rr->Products->[$p]->Molecules->[$m];
		my $addMol = $newMol->toString();
		$pstring .= " new ";
		$pstring .= $addMol;
		++$i;
	}

	for my $ea ( @{ $rr->EdgeAdd } ) {
		( my $p1, my $p2 ) = @{$ea};
		my $pstring1 = $rr->MapR->{$p1};    #Mapping back to reactants
		my $pstring2 = $rr->MapR->{$p2};

		( my $pat1, my $mol1, my $comp1 ) = split /\./, $pstring1;
		( my $pat2, my $mol2, my $comp2 ) = split /\./, $pstring2;

		if ( $i != 0 ) { $pstring .= ";"; }
		my $newComp1 = $rr->Reactants->[$pat1]->Molecules->[$mol1]->Components->[$comp1];
		my $newComp2 = $rr->Reactants->[$pat2]->Molecules->[$mol2]->Components->[$comp2];

        my $tempstring;
		( $tempstring, my $checkComp1 ) = $rr->Reactants->[$pat1]->Molecules->[$mol1]->toStringSSC();
		( $tempstring, my $checkComp2 ) = $rr->Reactants->[$pat2]->Molecules->[$mol2]->toStringSSC();

		my $label1;
		my $label2;

		if ( $pat1 == 0 ) {
			if ( exists $pattern1{$mol1} ) {
				$label1 = $pattern1{$mol1};
				++$mol_index;
			}
			else {
				$pattern1{$mol1} = $mol_index;
				$label1 = $mol_index;
				++$mol_index;
			}
		}
		if ( $pat2 == 0 ) {
			if ( exists $pattern1{$mol2} ) {
				$label2 = $pattern1{$mol2};
				++$mol_index;
			}
			else {
				$pattern1{$mol2} = $mol_index;
				$label2 = $mol_index;
				++$mol_index;
			}
		}

		if ( $pat1 == 1 ) {
			if ( exists $pattern2{$mol1} ) {
				$label1 = $pattern2{$mol1};
				++$mol_index;
			}
			else {
				$pattern2{$mol1} = $mol_index;
				$label1 = $mol_index;
				++$mol_index;
			}
		}
		if ( $pat2 == 1 ) {
			if ( exists $pattern2{$mol2} ) {
				$label2 = $pattern2{$mol2};
				++$mol_index;
			}
			else {
				$pattern2{$mol2} = $mol_index;
				$label2 = $mol_index;
				++$mol_index;
			}
		}
		if ( defined $newComp1->State ) {
			$pstring .= " " . "Mol" . $label1 . "\." . $newComp1->Name . "Binds";
		}
		else { $pstring .= " " . "Mol" . $label1 . "\." . $newComp1->Name; }
		$pstring .= "#";

		if ( defined $newComp2->State ) {
			$pstring .= "Mol" . $label2 . "\." . $newComp2->Name . "Binds";
		}
		else {
			$pstring .= "Mol" . $label2 . "\." . $newComp2->Name;
		}
		my $compName1 = $newComp1->Name;
		my $compName2 = $newComp2->Name;

		if ( $checkComp1 != 0 ) {
			#permute products if its the component with same name
			# get the information for same component for that molecule
			( my %compHash ) = $rr->Reactants->[$pat1]->Molecules->[$mol1]->getCompHash();
			foreach my $key ( keys(%compHash) ) {
				if ( ( $compHash{$key} ) > 0 ) {
					if ( $compName1 == $key ) {
						#now permute products
						my $iter = $compHash{$key};
						for ( $i = 0 ; $i < $iter ; ++$i ) {
							my $tempPString;
							my $addLabel = $i + 1;
							if ( defined $newComp1->State ) {
								$tempPString .= " " . "Mol" . $label1 . "\." . $newComp1->Name . $addLabel . "Binds";
							}
							else {
								$tempPString .= " " . "Mol" . $label1 . "\." . $newComp1->Name . $addLabel;
							}
							$tempPString .= "#";
							$prod1{$addLabel} = $tempPString;
                            # print"\n product permutation for $compName1  for $addLabel is $tempPString \n";
							++$expP1;
						}
					}
				}
			}
		}

		if ( $checkComp1 == 0 ) {
			if ( defined $newComp1->State ) {
				$sameProd1 .= " " . "Mol" . $label1 . "\." . $newComp1->Name . "Binds";
			}
			else {
				$sameProd1 .= " " . "Mol" . $label1 . "\." . $newComp1->Name;
			}
			$sameProd1 .= "#";
		}

		if ( $checkComp2 == 0 ) {
			if ( defined $newComp2->State ) {
				$sameProd2 .= " " . "Mol" . $label2 . "\." . $newComp2->Name . "Binds";
			}
			else {
				$sameProd2 .= " " . "Mol" . $label2 . "\." . $newComp2->Name;
			}
		}

		if ( $checkComp2 != 0 ) {
			#permute products if its the component with same name
			( my %compHash ) = $rr->Reactants->[$pat2]->Molecules->[$mol2]->getCompHash();
			foreach my $key ( keys(%compHash) ) {
				if ( ( $compHash{$key} ) > 0 ) {
					if ( $compName2 == $key ) {
						#now permute products
						my $iter = $compHash{$key};
						for ( $i = 0 ; $i < $iter ; ++$i ) {
							my $tempPString;
							my $addLabel = $i + 1;

							if ( defined $newComp2->State ) {
								$tempPString .="Mol" . $label2 . "\.". $newComp2->Name . $addLabel . "Binds";
							}
							else {
								$tempPString .= "Mol" . $label2 . "\.". $newComp2->Name. $addLabel;
							}
							$prod2{$addLabel} = $tempPString;
							++$expP2;
						}
					}
				}
			}
		}
		++$i;

		if ( $expP1 != 0 && $expP2 == 0 ) {
			for ( my $i = 0 ; $i < $expP1 ; ++$i ) {
				my $temp = $prod1{ ( $i + 1 ) };
				$temp .= $sameProd2;
				$prod1{ ( $i + 1 ) } = $temp;
				#print "\n final permutated string is $temp \n";
			}
		}
		if ( $expP1 == 0 && $expP2 != 0 ) {
			for ( my $i = 0 ; $i < $expP2 ; ++$i ) {
				my $temp = $sameProd1;
				$temp .= $prod2{ ( $i + 1 ) };
				$prod2{ ( $i + 1 ) } = $temp;
				#print "\n final permutated string is $temp \n";
			}
		}
		if ( $expP1 != 0 && $expP2 != 0 ) {
			#see how to iterate if we have to permute both the products
		}
        # also to check for dangling edges and same edge label for reactions with same component
	}

	# --Dissociation
	for my $ed ( @{ $rr->EdgeDel } ) {
		( my $p1, my $p2 ) = @{$ed};

		( my $pat1, my $mol1, my $comp1 ) = split /\./, $p1;
		( my $pat2, my $mol2, my $comp2 ) = split /\./, $p2;

		my $newComp1 = $rr->Reactants->[$pat1]->Molecules->[$mol1]->Components->[$comp1];
		my $newComp2 = $rr->Reactants->[$pat2]->Molecules->[$mol2]->Components->[$comp2];
		my $unbMol1 .= $pat1 . $mol2;
		my $unbMol2 .= $pat2 . $mol2;
		( my $name, my $bond ) = split /#/, $newComp1->toStringSSC();
		# check for molecules not in delMolecules
		if ( $delMolCase == 0 ) {
			if ( $i != 0 ) { $pstring .= ";"; }
			$pstring .= " break " . $bond;
			++$i;
		}
	}
	
	my $stateLabel = -1;
	# --State changes
	for my $sc ( @{ $rr->CompStateChange } ) {
		my $checkComp1;
		my $checkComp2;
		( my $p, my $sR, my $sP ) = @{$sc};
		( my $pat1, my $mol1, my $comp1 ) = split /\./, $p;
		if ( $pat1 == 0 ) {
			if ( exists $pattern1{$mol1} ) {
				$stateLabel = $pattern1{$mol1};
				++$mol_index;
			}
			else {
				$pattern1{$mol1} = $mol_index;
				$stateLabel = $mol_index;
				++$mol_index;
			}

			( my $tempstring, $checkComp1 ) =$rr->Reactants->[$pat1]->Molecules->[$mol1]->toStringSSC();
		}

		if ( $pat1 == 1 ) {
			if ( exists $pattern2{$mol1} ) {
				$stateLabel = $pattern2{$mol1};
				++$mol_index;
			}
			else {
				$pattern2{$mol1} = $mol_index;
				$stateLabel = $mol_index;
				++$mol_index;
			}
			( my $tempstring, $checkComp2 ) =$rr->Reactants->[$pat1]->Molecules->[$mol1]->toStringSSC();
		}

		my $newComp = $rr->Reactants->[$pat1]->Molecules->[$mol1]->Components->[$comp1]->toStringSSC();
		( my $my_Comp ) = split /=/, $newComp;
		if ( $i != 0 ) {
			$pstring .= ";";
		}    # As two state changes are separated, cannot bedefined in one line

		++$i;
		$pstring .= " " . "Mol" . $stateLabel . "." . $my_Comp . "=\"" . $sP . "\"";

		if ( defined($checkComp2)  and  $checkComp2 != 0 ) {    #print "\n comes here \n";
			    #permute products if its the component with same name
			( my %compHash ) = $rr->Reactants->[$pat1]->Molecules->[$mol1]->getCompHash();
			foreach my $key ( keys(%compHash) ) {
				if ( ( $compHash{$key} ) > 0 ) {
					if ( $key == $my_Comp ) {
						#now permute products
						my $iter = $compHash{$key};
						for ( $i = 0 ; $i < $iter ; ++$i ) {
							my $tempPString;
							my $addLabel = $i + 1;
							$tempPString .= " " . "Mol". $stateLabel . "." . $my_Comp . $addLabel . "=\"" . $sP . "\"";
							$prod2{$addLabel} = $tempPString;
                            #print"\n product permutation!!! for $my_Comp for $addLabel is $tempPString \n";
							++$expP2;
						}
					}
				}
			}
		}
		if ( defined($checkComp2)  and  $checkComp1 != 0 ) {    #print "\n comes here in 1 \n";
			    #permute products if its teh component with same name
			( my %compHash ) = $rr->Reactants->[$pat1]->Molecules->[$mol1]->getCompHash();
			foreach my $key ( keys(%compHash) ) {
				if ( ( $compHash{$key} ) > 0 ) {
					if ( $my_Comp == $key ) {
						#now permute products
						my $iter = $compHash{$key};
						for ( $i = 0 ; $i < $iter ; ++$i ) {
							my $tempPString;
							my $addLabel = $i + 1;
							$tempPString .= " " . "Mol" . $stateLabel . ".". $my_Comp . $addLabel . "=\"" . $sP . "\"";
							$prod1{$addLabel} = $tempPString;
                            #print"\n product permutation !!!for $my_Comp for $addLabel is $tempPString \n";
							++$expP1;
						}
					}
				}
			}
		}
	}

	# Starting reactant side from here
	if ( $mol_index <= 2 ) {
		my $count = 0;
		while ( defined( $rr->Reactants->[0]->Molecules->[$count] ) ) {
			foreach my $key ( keys(%pattern1) ) {
				if ( $key == $count ) {
					my $temp = "Mol" . $pattern1{$key} . ":";
					push( @sameReac1, $temp );
					$rstring .= $temp;
				}
			}
			( my $temprstring, my $checkComp1 ) = $rr->Reactants->[0]->Molecules->[$count]->toStringSSC();
			$rstring .= $temprstring;
			if ( $checkComp1 == 0 ) { push( @sameReac1, $temprstring ) }
			if ( $checkComp1 != 0 ) {
				#permute products if its teh component with same name
				( my %compHash ) = $rr->Reactants->[0]->Molecules->[$count]->getCompHash();
				foreach my $key ( keys(%compHash) ) {
					if ( ( $compHash{$key} ) > 0 ) {
						my $searchLabel .= $key . "#";
						my $stateSearchLabel .= $key . "Binds" . "#";
						if ( $temprstring =~ /(?<=$searchLabel)[\d\_]/ ) {
							$temprstring =~ s/(?<=$searchLabel)[\d\_]//;
							my $putBond;
							for my $compObject (@{$rr->Reactants->[0]->Molecules->[$count]->Components}){
								my $tempName = $compObject->Name;
								if ( $tempName == $key ) {
									for my $edge ( @{ $compObject->Edges } ) {
										if ( $edge =~ /^\d+$/ ) {
											$putBond = $edge + 1;
										}
										else { $putBond = "$edge"; }
									}
								}
							}
		                    # here is just to tell that ok at this spot put the permuted molecule
							push( @sameReac1, "here" );
							#now permute reactants
							my $iter = $compHash{$key};
							for ( $i = 0 ; $i < $iter ; ++$i ) {
								my $tempRString;
								++$expR1;
								my $addLabel = $i + 1;
								$tempRString = $temprstring;
								my $newsearchLabel .= $key . $addLabel . "#";
								my $newaddLabel .= $newsearchLabel . $putBond;
								$tempRString =~ s/$newsearchLabel/$newaddLabel/;
								$combinations1{$addLabel} = $tempRString;
	                            #print "\n reactant permutation for $key for $addLabel is $tempRString \n";
							}
						}
						if ( $temprstring =~ /(?<=$stateSearchLabel)\d/ ) {
							$temprstring =~ s/(?<=$stateSearchLabel)\d//;
							my $putBond;
							for my $compObject (@{$rr->Reactants->[0]->Molecules->[$count]->Components}){
								my $tempName = $compObject->Name;
								if ( $tempName == $key ) {
									for my $edge ( @{ $compObject->Edges } ) {
										$putBond = $edge + 1;
									}
								}
							}
							push( @sameReac1, "here" );
							#now permute reactants
							my $iter = $compHash{$key};
							for ( $i = 0 ; $i < $iter ; ++$i ) {
								my $tempRString;
								++$expR1;
								my $addLabel = $i + 1;
								$tempRString = $temprstring;
								my $newsearchLabel .=
								  $key . $addLabel . "Binds" . "#";
								my $newaddLabel .= $newsearchLabel . $putBond;
								$tempRString =~ s/$newsearchLabel/$newaddLabel/;
								$combinations1{$addLabel} = $tempRString;

	                            #	print "\n reactant permutation for $key for $addLabel is $tempRString \n";
							}
						}
						else { push( @sameReac1, $temprstring ); }
					}
				}
			}
			++$count;
		}
	}

	elsif ( $mol_index > 2 ) {
		my $count = 0;
		while ( defined( $rr->Reactants->[0]->Molecules->[$count] ) ) {
			foreach my $key ( keys(%pattern1) ) {
				if ( $key == $count ) {
					my $temp = "Mol" . $pattern1{$key} . ":";
					push( @sameReac1, $temp );
					$rstring .= $temp;
				}
			}
			( my $temprstring, my $checkComp1 ) = $rr->Reactants->[0]->Molecules->[$count]->toStringSSC();
			$rstring .= $temprstring;
			if ( $checkComp1 == 0 ) { push( @sameReac1, $temprstring ) }
			if ( $checkComp1 != 0 ) {

				#permute products if its teh component with same name
				( my %compHash ) = $rr->Reactants->[0]->Molecules->[$count]->getCompHash();
				foreach my $key ( keys(%compHash) ) {
					if ( ( $compHash{$key} ) > 0 ) {
						my $searchLabel .= $key . "#";
						my $stateSearchLabel .= $key . "Binds" . "#";
						if ( $temprstring =~ /(?<=$searchLabel)[\d\_]/ ) {
							$temprstring =~ s/(?<=$searchLabel)[\d\_]//;
							my $putBond;
							for my $compObject (@{$rr->Reactants->[0]->Molecules->[$count]->Components}){
								my $tempName = $compObject->Name;
								if ( $tempName == $key ) {
									for my $edge ( @{ $compObject->Edges } ) {
										if ( $edge =~ /^\d+$/ ) {
											$putBond = $edge + 1;
										}
										else { $putBond = "$edge"; }
									}
								}
							}
							push( @sameReac1, "here" );
							#now permute products
							my $iter = $compHash{$key};
							for ( $i = 0 ; $i < $iter ; ++$i ) {
								++$expR1;
								my $tempRString;
								my $addLabel = $i + 1;
								$tempRString = $temprstring;
								my $newsearchLabel .= $key . $addLabel . "#";
								my $newaddLabel .= $newsearchLabel . $putBond;
								$tempRString =~ s/$newsearchLabel/$newaddLabel/;
								$combinations1{$addLabel} = $tempRString;
							}
						}
						if ( $temprstring =~ /(?<=$stateSearchLabel)\d/ ) {
							$temprstring =~ s/(?<=$stateSearchLabel)\d//;
							my $putBond;
							for my $compObject (@{$rr->Reactants->[0]->Molecules->[$count]->Components}){
								my $tempName = $compObject->Name;
								if ( $tempName == $key ) {
									for my $edge ( @{ $compObject->Edges } ) {
										$putBond = $edge + 1;
									}
								}
							}
							push( @sameReac1, "here" );
							#now permute products
							my $iter = $compHash{$key};
							for ( $i = 0 ; $i < $iter ; ++$i ) {
								++$expR1;
								my $tempRString;
								my $addLabel = $i + 1;
								$tempRString = $temprstring;
								my $newsearchLabel .=$key . $addLabel . "Binds" . "#";
								my $newaddLabel .= $newsearchLabel . $putBond;
								$tempRString =~ s/$newsearchLabel/$newaddLabel/;
								$combinations1{$addLabel} = $tempRString;
							}
						}
						else { push( @sameReac1, $temprstring ); }
					}
				}
			}
			++$count;
			#print "\n array is @sameReac1\n";
		}
		$count = 0;
		foreach my $key ( keys(%pattern2) ) {
			if ( defined($key) ) {
				$rstring2 .= "Mol";
			}
		}

		if ( $rstring2 ne "" ) {
			$rstring2 = "";
			$count    = 0;
			while ( defined( $rr->Reactants->[1]->Molecules->[$count] ) ) {
				foreach my $key ( keys(%pattern2) ) {
					if ( $key == $count ) {
						$rstring2 .= "Mol" . $pattern2{$key} . ":";
					}
				}
				( my $temprstring, my $checkComp2 ) = $rr->Reactants->[1]->Molecules->[$count]->toStringSSC();
				$rstring2 .= $temprstring;
				if ( $checkComp2 == 0 ) { push( @sameReac2, $temprstring ) }
				if ( $checkComp2 != 0 ) {

					#permute products if its teh component with same name
					( my %compHash ) = $rr->Reactants->[1]->Molecules->[$count]->getCompHash();
					foreach my $key ( keys(%compHash) ) {
						if ( ( $compHash{$key} ) > 0 ) {
							my $searchLabel .= $key . "#";
							my $stateSearchLabel .= $key . "Binds" . "#";
							if ( $temprstring =~ /(?<=$searchLabel)[\d\_]/ ) {
								$temprstring =~ s/(?<=$searchLabel)[\d\_]//;
								my $putBond;
								for my $compObject (@{$rr->Reactants->[1]->Molecules->[$count]->Components}){
									my $tempName = $compObject->Name;
									if ( $tempName == $key ) {
										for my $edge ( @{ $compObject->Edges } )
										{
											if ( $edge =~ /^\d+$/ ) {
												$putBond = $edge + 1;
											}
											else { $putBond = "$edge"; }
										}
									}
								}
								push( @sameReac2, "here" );
								#now permute products
								my $iter = $compHash{$key};
								for ( $i = 0 ; $i < $iter ; ++$i ) {
									++$expR2;
									my $tempRString;
									my $addLabel = $i + 1;
									$tempRString = $temprstring;
									my $newsearchLabel .=$key . $addLabel . "#";
									my $newaddLabel .=$newsearchLabel . $putBond;
									$tempRString =~ s/$newsearchLabel/$newaddLabel/;
									$combinations2{$addLabel} = $tempRString;
								}
							}
							if ( $temprstring =~ /(?<=$stateSearchLabel)\d/ ) {
								$temprstring =~ s/(?<=$stateSearchLabel)\d//;
								my $putBond;
								for my $compObject (@{$rr->Reactants->[1]->Molecules->[$count]->Components}){
									my $tempName = $compObject->Name;
									if ( $tempName == $key ) {
										for my $edge ( @{ $compObject->Edges } )
										{
											$putBond = $edge + 1;
										}
									}
								}
								push( @sameReac2, "here" );
								#now permute products
								my $iter = $compHash{$key};
								for ( $i = 0 ; $i < $iter ; ++$i ) {
									++$expR2;
									my $tempRString;
									my $addLabel = $i + 1;
									$tempRString = $temprstring;
									my $newsearchLabel .=$key . $addLabel . "Binds" . "#";
									my $newaddLabel .= $newsearchLabel . $putBond;
									$tempRString =~ s/$newsearchLabel/$newaddLabel/;
									$combinations2{$addLabel} = $tempRString;
								}
							}
							else { push( @sameReac2, $temprstring ); }
						}
					}
				}
				++$count;
			}
		}
	}
	$i = 0;
	
	
	for my $g ( @{ $rr->Reactants } ) {
		my %seen_edge;    # to resolve dangling edges
		++$i;
		( my $num, my $array ) = $g->findConnected();
		if ( $num > 1 ) {
			$errmsg = 1;
			print STDOUT"\n WARNING: This rule has been omitted as the translator has detected pattern which is disconnectd according to SSC\n";
			print STDOUT"\n          Please expnad the necessary rule patterns. See .rxn file for more details.\n";
		}

		# for seeing same edge labels in a reaction
		for my $mol ( @{ $g->Molecules } ) {
			for my $comp ( @{ $mol->Components } ) {
				for my $edge ( @{ $comp->Edges } ) {
					if ( $edge =~ /^\d+$/ ) {
						my $edge1 = $edge + 1;
						$seen_edge{$edge1} = $edge1
						  if exists $seen_edge{$edge1};

                        #the above statement see that in case of a dangling edge, each edge is seen twice;
                        #It stores the pair as a key and value.
                        #And whichever key doesn't has a value,that edge is dangling and to be replaced by _.
						$seen_edge{$edge1} = 0 if not exists $seen_edge{$edge1};
						if (   $i == 1
							&& $seen_bond <= $edge + 1
						  ) #keeping record of highest edges in the first pattern
						{
							$seen_bond = $edge + 1;
						}

                        #this checks if any edge in second pattern is <= seen_bond, it is a case of edge label,
                        #and increase all the edges by seen_bond
						elsif ( $i != 1 && $edge <= $seen_bond ) {
							$checkEdge = 1;
						}
					}
				}
			}
		}

		my @dangling; #iterating over the seen_edge has to record dangling edges
		while ( my ( $key, $value ) = each(%seen_edge) ) {
			if ( $value == 0 ) {
				push( @dangling, $key );
			}
		}
		if ( $i == 1 ) {
			my $size = @dangling;
			if ( $size != 0 ) {
				for ( my $iter = 0 ; $iter < $size ; $iter++ ) {
					my $dang = pop(@dangling);
					$rstring =~ s/(?<=\#)$dang/_/;
				}
			}
		}
		if ( $i == 2 ) {
			my $size = @dangling;
			if ( $size != 0 ) {
				for ( my $iter = 0 ; $iter < $size ; $iter++ ) {
					my $dang = pop(@dangling);
					$rstring2 =~ s/(?<=\#)$dang/_/;
				}
			}
			if ( $checkEdge == 1 ) {
				$rstring2 =~ s/(?<=\#)\d/$& + $seen_bond/eg;
			}
		}
	}

	$finalString .= "rxn " . $rstring . " " . $rstring2;
	$finalString .= " at " . $rr->RateLaw->toString();
	$finalString .= " -> ";
	$finalString .= $pstring;
	if ( $finalString =~ /\?/ ) {
		$errmsg = 1;
		print STDOUT"\n WARNING: This rule has been omitted as SSC does not implement ?. Please see .rxn for more detail \n";
	}

	if ( $rr->Priority != 0 ) {
		$errmsg = 1;
		print STDOUT" \n WARNING: This rule has been omitted as SSC does not implement priority. Please see .rxn file for more details \n";
	}

	for my $i ( 0 .. $#{ $rr->Rexclude } ) {
		next unless ( @{ $rr->Rexclude->[$i] } );
		$errmsg = 1;
		print STDOUT"\n WARNING: This rule has been omitted as SSC does not implement Rexclude. Please see .rxn file for more details \n";
	}

	for my $i ( 0 .. $#{ $rr->Pexclude } ) {
		next unless ( @{ $rr->Pexclude->[$i] } );
		$errmsg = 1;
		print STDOUT"\n WARNING: This rule has been omitted as SSC does not implement Pexclude. Please see .rxn file for more details \n";
	}

	for my $i ( 0 .. $#{ $rr->Rinclude } ) {
		next unless ( @{ $rr->Rinclude->[$i] } );
		$errmsg = 1;
		print STDOUT"\n WARNING: This rule has been omitted as SSC does not implement Rinclude. Please see .rxn file for more details \n";
	}

	for my $i ( 0 .. $#{ $rr->Pinclude } ) {
		next unless ( @{ $rr->Pinclude->[$i] } );
		$errmsg = 1;
		print STDOUT"\n WARNING: This rule has been omitted as SSC does not implement Pinclude. Please see .rxn file for more details \n";
	}

	if ( $expR1 == 0 && $expP1 == 0 && $expR2 == 0 && $expP2 == 0 ) {
		return ( $finalString, $errmsg );
	}
	if ( $expP1 != 0 && $expP2 == 0 ) {
		for ( my $i = 1 ; $i <= $expP1 ; ++$i ) {
			$finalString .= "\n";
			$finalString .= "rxn " . $rstring . " " . $rstring2;
			$finalString .= " at " . $rr->RateLaw->toString();
			$finalString .= "->";
			my $tempstring = $prod1{$i};
			$finalString .= $tempstring;
			$finalString .= "\n";
		}
		return ( $finalString, $errmsg );
	}

	if ( $expP1 == 0 && $expP2 != 0 ) {
		for ( my $i = 1 ; $i <= $expP2 ; ++$i ) {
			$finalString .= "\n";
			$finalString .= "rxn " . $rstring . " " . $rstring2;
			$finalString .= " at " . $rr->RateLaw->toString();
			$finalString .= "->";
			my $tempstring = $prod2{$i};
			$finalString .= $tempstring;
			$finalString .= "\n";
		}
		return ( $finalString, $errmsg );
	}

	if ( $expR1 != 0 && $expR2 == 0 ) {
		my $size = @sameReac1;
		for ( my $i = 1 ; $i <= $expR1 ; ++$i ) {
			$finalString .= "\n";
			$finalString .= "rxn ";
			for ( my $j = 0 ; $j < $size ; ++$j ) {
				my $tempstring = $sameReac1[$j];
				if ( $tempstring ne 'here' ) {
					$finalString .= $tempstring;
				}
				if ( $tempstring eq 'here' ) {
					my $put = $combinations1{$i};
					$finalString .= $put;
				}
			}
			$finalString .= " " . $rstring2;
			$finalString .= " at " . $rr->RateLaw->toString() . "->" . $pstring;
			$finalString .= "\n";
		}
		return ( $finalString, $errmsg );
	}

	if ( $expR1 == 0 && $expR2 != 0 ) {
		my $size = @sameReac2;
		for ( my $i = 1 ; $i <= $expR2 ; ++$i ) {
			$finalString .= "\n";
			$finalString .= "rxn ";
			$finalString .= " " . $rstring;
			for ( my $j = 0 ; $j < $size ; ++$j ) {
				my $tempstring = $sameReac2[$j];
				if ( $tempstring ne 'here' ) {
					$finalString .= $tempstring;
				}
				if ( $tempstring eq 'here' ) {
					my $put = $combinations2{$i};
					$finalString .= $put;
				}
			}
			$finalString .= " at " . $rr->RateLaw->toString() . "->" . $pstring;
			$finalString .= "\n";
		}
		return ( $finalString, $errmsg );
	}
}



###
###
###



sub toXML
{
	my $rr     = shift;
	my $indent = shift;
	my $index  = shift;
	my $plist  = (@_) ? shift : "";

	my $string = $indent . "<ReactionRule";

	# Attributes
	# id
	my $id = "RR" . $index;
	$string .= " id=\"" . $id . "\"";

	# name
	if ( $rr->Name ) {
		$string .= " name=\"" . $rr->Name . "\"";
	}

	# priority
	if ( $rr->Priority != 0 ) {
		$string .= " priority=\"" . $rr->Priority . "\"";
	}

	# multstale
	if ( defined $rr->MultScale ) {
		$string .= " symmetry_factor=\"" . $rr->MultScale . "\"";
	}

	# Objects contained
	my $ostring = "";
	my $indent2 = "  " . $indent;
	my $indent3 = "  " . $indent2;
	my $indent4 = "  " . $indent3;

	# Reactants
	{
	    $ostring .= $indent2 . "<ListOfReactantPatterns>\n";
	    my $index = 1;
	    foreach my $patt ( @{ $rr->Reactants } )
	    {
		    my $pid = $id . "_RP" . $index;
		    $ostring .= $patt->toXML( $indent3, "ReactantPattern", $pid, "" );
		    ++$index;
	    }
	    $ostring .= $indent2 . "</ListOfReactantPatterns>\n";
    }

	# Products
	{
	    $ostring .= $indent2 . "<ListOfProductPatterns>\n";
	    my $index = 1;
	    foreach my $patt ( @{ $rr->Products } )
	    {
		    my $pid = $id . "_PP" . $index;
		    $ostring .= $patt->toXML( $indent3, "ProductPattern", $pid, "" );
		    ++$index;
	    }
	    $ostring .= $indent2 . "</ListOfProductPatterns>\n";
	}

	# Add support for Exclude/Include Reactants/Products
	# --justin 29may2008
	#Exclude Reactants
	foreach my $iReac ( 1 .. @{ $rr->Rexclude } )
	{
		if ( @{ $rr->Rexclude->[ $iReac - 1 ] } )
		{
			my $pid = $id . '_RP' . $iReac;
			$ostring .= $indent2 . "<ListOfExcludeReactants id=\"$pid\">\n";
			my $iPatt = 1;
			foreach my $patt ( @{ $rr->Rexclude->[ $iReac - 1 ] } ) {
				my $patt_id = $pid . "_Exclude$iPatt";
				$ostring .= $patt->toXML( $indent3, 'Pattern', $patt_id, "" );
				$iPatt++;
			}
			$ostring .= $indent2 . "</ListOfExcludeReactants>\n";
		}
	}

	#Include Reactants
	foreach my $iReac ( 1 .. @{ $rr->Rinclude } )
	{
		if ( @{ $rr->Rinclude->[ $iReac - 1 ] } )
		{
			my $pid = $id . '_RP' . $iReac;
			$ostring .= $indent2 . "<ListOfIncludeReactants id=\"$pid\">\n";
			my $iPatt = 1;
			foreach my $patt ( @{ $rr->Rinclude->[ $iReac - 1 ] } ) {
				my $patt_id = $pid . "_Include$iPatt";
				$ostring .= $patt->toXML( $indent3, 'Pattern', $patt_id, "" );
				$iPatt++;
			}
			$ostring .= $indent2 . "</ListOfIncludeReactants>\n";
		}
	}

	#Write Exclude Products
	foreach my $iProd ( 1 .. @{ $rr->Pexclude } )
	{
		if ( @{ $rr->Pexclude->[ $iProd - 1 ] } )
		{
			my $pid = $id . '_PP' . $iProd;
			$ostring .= $indent2 . "<ListOfExcludeProducts id=\"$pid\">\n";
			my $iPatt = 1;
			foreach my $patt ( @{ $rr->Pexclude->[ $iProd - 1 ] } ) {
				my $patt_id = $pid . "_Exclude$iPatt";
				$ostring .= $patt->toXML( $indent3, 'Pattern', $patt_id, "" );
				$iPatt++;
			}
			$ostring .= $indent2 . "</ListOfExcludeProducts>\n";
		}
	}

	# Write Include Products
	foreach my $iProd ( 1 .. @{ $rr->Pinclude } )
	{
		if ( @{ $rr->Pinclude->[ $iProd - 1 ] } )
		{
			my $pid = $id . '_PP' . $iProd;
			$ostring .= $indent2 . "<ListOfIncludeProducts id=\"$pid\">\n";
			my $iPatt = 1;
			foreach my $patt ( @{ $rr->Pinclude->[ $iProd - 1 ] } ) {
				my $patt_id = $pid . "_Include$iPatt";
				$ostring .= $patt->toXML( $indent3, 'Pattern', $patt_id, "" );
				$iPatt++;
			}
			$ostring .= $indent2 . "</ListOfIncludeProducts>\n";
		}
	}

	# RateLaw (need to pass parameter list and reference hash for functional ratelaws!)
	$ostring .= $rr->RateLaw->toXML( $indent2, $id, $plist, $rr->RRefs );


	# Write Reactant to Product Map
	{
	    $ostring .= $indent2 . "<Map>\n";
	    my $index = 1;
	    foreach my $source ( sort keys %{ $rr->MapF } )
	    {
		    $ostring .= $indent3 . "<MapItem";
		    my $target = $rr->MapF->{$source};
		    $ostring .=
		        " sourceID=\"" . pointer_to_ID( $id . "_R", $source ) . "\"";
		    $target = pointer_to_ID( $id . "_P", $target );
		    if ( $target ne "Null" ) {
			    $ostring .= " targetID=\"" . $target . "\"";
		    }
		    $ostring .= "/>\n";
		    ++$index;
	    }
	    $ostring .= $indent2 . "</Map>\n";
	}

	# Write Operations
    $ostring .= $indent2 . "<ListOfOperations>\n";	


    # first create reverse map	
    my %map_reverse = ();
	{
	    my $index = 1;
	    # construct map from product molecules and components onto reactants
	    foreach my $s ( keys %{ $rr->MapF } )
	    {
		    my $t = $rr->MapF->{$s};
		    $map_reverse{$t} = $s;
	    }
    }

	# Re-ordered operations to match the order of application!
	# --Justin, 29may2009

	# Component State Changes
	foreach my $sc ( @{ $rr->CompStateChange } )
	{
		( my $p, my $sR, my $sP ) = @{$sc};
		$ostring .= $indent3 . "<StateChange";
		$ostring .= " site=\"" . pointer_to_ID( $id . "_R", $p ) . "\"";
		$ostring .= " finalState=\"" . $sP . "\"";
		$ostring .= "/>\n";
	}

    # Compartment Changes
    # changes by justinshogg@gmail.com
    # 14 Apr 2009
    # notes:
    # ChangeCompartment, three types:
    #
    #  species transport:
    #  <ChangeCompartment id="RR#_RP#" source="Comp" destination="Comp" flipOrientation="#" moveConnected="#"/>
    #
    #  molecule transport:
    #  <ChangeCompartment id="RR#_RP#_M#" source="Comp" destination="Comp" flipOrientation="#" moveConnected="#"/>
    #
    #  component transport:
    #  <ChangeCompartment id="RR#_RP#_M#_C#" source="Comp" destination="Comp" flipOrientation="#" moveConnected="#"/>
    #
    #  flipOrientation = 1  if species surface transport up or down a path in the compartment tree.
    #                         (flip orientation of baggage molecules, Outside<=>Inside, like endocytosis)
    #  flipOrientation =-1  if species surface transport to an adjacent branch in the compartment tree.
    #                         (no flip, like exocytosis)
    #  flipOrientation = 0  if not species surface transport.
    #
    #  moveConnected = 1    if molecule transport and the "compartment connected component" should be co-transported.
    #  moveConnected = 0    otherwise.
	foreach my $compartment_change ( @{ $rr->ChangeCompartment } )
	{
		my ( $ref, $compartR, $compartP, $cytosis ) = @{$compartment_change};
		( $compartR, $compartP ) = ( $compartR->Name, $compartP->Name );
		$ostring .= $indent3 . '<ChangeCompartment';
		$ostring .= ' id="' . pointer_to_ID( $id . "_R", $ref ) . '"';
		$ostring .= ' source="' . $compartR . '"';
		$ostring .= ' destination="' . $compartP . '"';
		$ostring .= ' flipOrientation="' . $cytosis . '"';
		$ostring .= ' moveConnected="' . $rr->MoveConnected . '"';
		$ostring .= "/>\n";
	}

	# end changes by Justin

	# Edge Deletion
	foreach my $ed ( @{ $rr->EdgeDel } )
	{
		( my $p1, my $p2 ) = @{$ed};
		$ostring .= $indent3 . "<DeleteBond";
		$ostring .= " site1=\"" . pointer_to_ID( $id . "_R", $p1 ) . "\"";
		$ostring .= " site2=\"" . pointer_to_ID( $id . "_R", $p2 ) . "\"";
		$ostring .= "/>\n";
	}

	# Molecule Addition
	# Fixed bug (always returned ID to first molecule of first product pattern)
	# --justin 29may2009
	foreach my $molAddRef ( @{ $rr->MolAdd } )
	{
		$ostring .= $indent3 . "<Add";
		$ostring .=
		  " id=\"" . pointer_to_ID( $id . "_P", $molAddRef->[0] ) . "\"";
		$ostring .= "/>\n";
	}

	# Edge Addition
	foreach my $ea ( @{ $rr->EdgeAdd } )
	{
		( my $p1, my $p2 ) = @{$ea};
		
		# Bug (reported by Michael): no reference to component if the target is
		# on a new molecule.
		# Fix: if bond connects to a component of a new molecule, then point into the 
		# product pattern.  Otherwise, get pointer into the reactant patterns.
		# --Justin, 11 oct 2010.
		if ( exists $map_reverse{$p1} )
		{   $p1 = pointer_to_ID( $id . "_R", $map_reverse{$p1} );   }
		else
		{   $p1 = pointer_to_ID( $id . "_P", $p1 );   }
		
		if ( exists $map_reverse{$p2} )
		{   $p2 = pointer_to_ID( $id . "_R", $map_reverse{$p2} );   }
		else
		{   $p2 = pointer_to_ID( $id . "_P", $p2 );	  }
	
		$ostring .= $indent3 . "<AddBond";
		$ostring .= " site1=\"$p1\"";
		$ostring .= " site2=\"$p2\"";
		$ostring .= "/>\n";
	}

	# Molecule/Pattern Deletion
	# update: export DeleteMolecules keyword.
	# --justin 29may2009
	foreach my $molRef ( @{ $rr->MolDel } )
	{
		my $delMolFlag = $rr->DeleteMolecules;
		$ostring .= $indent3 . "<Delete";
		$ostring .= ' id="'
		  . pointer_to_ID( $id . '_R', $molRef )
		  . '" DeleteMolecules="'
		  . $delMolFlag . '"';
		$ostring .= "/>\n";
	}

	$ostring .= $indent2 . "</ListOfOperations>\n";

	# Termination
	if ($ostring)
	{
		$string .= ">\n";                           # terminate tag opening
		$string .= $ostring;
		$string .= $indent . "</ReactionRule>\n";
	}
	else {
		$string .= "/>\n";                          # short tag termination
	}
}



###
###
###



sub pointer_to_ID
{
	# converts pointer format (e.g. 0.0.0) to ID format (e.g. "P1_M1_C1" )
	my $id      = shift;                            # Use first string as header
	my $pointer = shift;
	my @names   = ( "P", "M", "C" );

	my @arr = split /\./, $pointer;
	if ( $arr[0] == -1 ) {
		$id = "Null";
	}
	else {
		$id .= sprintf "%s%d", $names[0], $arr[0] + 1;
		for my $i ( 1 .. $#arr ) {
			$id .= sprintf "_%s%d", $names[$i], $arr[$i] + 1;
		}
	}
	return ($id);
}



##   __
##  |   o  _    |       _
##  |_  | | | /\| |\/| / | |/\
##  |   | | | \/| |  | \/| |\/
##                         |


# find mapping from reactant patterns to product patterns
#  and parse out transformations.
sub findMap
{
	my $rr     = shift @_;    # reaction rule
	my $mtlist = shift @_;    # molecule-types list

    # clear out transformations
    @{$rr->MolDel} = ();
	@{$rr->MolAdd} = ();    
	@{$rr->EdgeDel} = ();
    @{$rr->EdgeAdd} = ();
	@{$rr->CompStateChange} = ();
    @{$rr->ChangeCompartment} = ();	   


	# Aggregate reactant and product pattern graphs
	my ($rg) = SpeciesGraph::copymerge( @{$rr->Reactants} );
	my ($pg) = SpeciesGraph::copymerge( @{$rr->Products} );
	#printf "%s -> %s\n", $rg->toString(), $pg->toString();


    # Set up mapping between molecules of aggregate graphs and molecules of patterns
	my @aggMapR = ();
	foreach my $ipatt ( 0 .. $#{$rr->Reactants} )
	{
		foreach my $imol ( 0 .. $#{$rr->Reactants->[$ipatt]->Molecules} )
		{
			push @aggMapR, "$ipatt.$imol";
			#print "$#aggMapR -> $ipatt.$imol\n";
		}
	}

	my @aggMapP = ();
	foreach my $ipatt ( 0 .. $#{$rr->Products} )
	{
		foreach my $imol ( 0 .. $#{$rr->Products->[$ipatt]->Molecules} )
		{
			push @aggMapP, "$ipatt.$imol";
			#print "$#aggMapP -> $ipatt.$imol\n";
		}
	}


	# Find map between reactant and product graphs
	my $map = $rg->findMaps($pg);
	unless (defined $map) {  return "No valid mapping could be found for this rule.";  }
	# print "Found mapping: ", $maps[0]->toString(), "\n";




	# Molecules that are destroyed
	# map comments (jhogg): this can be based soley on mapF
	my @pused = (0) x @{$pg->Molecules};
	my $i_r = 0;
	foreach my $p_r ( 0 .. $#{$rr->Reactants} )
	{
	    my $molecules = $rr->Reactants->[$p_r]->Molecules;
		
		my @mol_delete;
		@mol_delete = ();
		foreach my $m_r ( 0 .. $#{$molecules} )
		{
			my $i_p = $map->MapF->{$i_r};
			if ( $i_p < 0 ) {
				push @mol_delete, $m_r;
			}
			else {
				$pused[$i_p] = 1;
			}
			++$i_r;
		}
		if ( $rr->DeleteMolecules  or  ($#mol_delete < $#{$molecules}) )
		{
			# Delete only molecules by placing pointer to each on deletion list
			foreach my $m_r (@mol_delete)
			{
				push @{$rr->MolDel}, "${p_r}.${m_r}";
                #printf "Rule %s: Reactant molecule %s is deleted\n", 
                #       $rr->Name, $rr->Reactants->[$p_r]->Molecules->[$m_r]->toString();
			}
		}
		else
		{
            # Delete whole species matching pattern
            #printf "Rule %s: Reactant pattern %s is deleted\n", $rr->Name, $rr->Reactants->[$p_r]->toString();
			push @{$rr->MolDel}, $p_r;
		}
	}



	# Molecules that are created
	foreach my $imP ( 0 .. $#pused )
	{
		next unless ( $pused[$imP] == 0 );

        # Check that this molecule is fully specified, i.e. all components and component states
		my $ref = $aggMapP[$imP];
		my ( $p, $m ) = split /\./, $ref;
		my $mol = $rr->Products->[$p]->Molecules->[$m];

        #printf "Rule %s: Product molecule %s is created\n", $rr->Name,$mol->toString();
        # List of pointers to reactant components from which to inherit attributes of each component
		my $ic    = 0;
		my @ilist = ();
		foreach my $comp ( @{ $mol->Components } )
		{
			if ( ( my $cref = $map->MapR->{"$imP.$ic"} ) ne "-1" )
			{
				#print "cref=$cref\n";
				my ( $imR, $icR ) = split /\./, $cref;
				$cref = $aggMapR[$imR] . ".$icR";
				push @ilist, $cref;
			    #printf "   Component $ic inherits from reactant pointer $cref\n";
			}
			else
			{
				# Is component state specified
				push @ilist, '';
			}
			++$ic;
		}
		if ( my $err =
		         $mtlist->checkMolecule($mol, { IsSpecies => 1, InheritList => \@ilist }) )
		{
			return ( "Molecule created in reaction rule: " . $err );
		}
		push @{$rr->MolAdd}, [ ($ref, @ilist) ];
	}
	#printf "%d edges reac %d edges prod\n", scalar(@edgesR), scalar(@edgesP);



	# Edges deleted
	my @eused = (0) x @{$pg->Edges};
	foreach my $edgeR ( @{$rg->Edges} )
	{
	    my ($p1R,$p2R) = split / /, $edgeR;
	    next unless ( defined $p1R  and  defined $p2R );
		
		my $p1P = $map->MapF->{$p1R};
		my $p2P = $map->MapF->{$p2R};
		next if ( $p1P < 0  or  $p2P < 0 );

		if ( ref $pg->Adjacency->{$p1P} eq 'HASH'  and  exists $pg->Adjacency->{$p1P}{$p2P} )
		{   # remember that edge label was found on the product side
			$eused[ $pg->Adjacency->{$p1P}{$p2P} ] = 1;
			next;
		}
		
		# edge not found! create an edge deletion operation!
		#  get molecule and component index (within reactant aggregate)
		my ( $im1, $ic1 ) = split /\./, $p1R;
		my ( $im2, $ic2 ) = split /\./, $p2R;
		
		# Check if either molecule is deleted.
		#  if so, the egde is removed as a side-effect and we don't need to
		#  explicitly remove the edge		
		next if ( grep {$_ eq $aggMapR[$im1]} @{$rr->MolDel}  or  grep {$_ eq $aggMapR[$im2]} @{$rr->MolDel} );
		
		# check if bond is part of a complex that is deleted
		my ($ip1) = split /\./, $aggMapR[$im1];
		next if ( grep {$_ eq $ip1 } @{$rr->MolDel} );	        

        # edge delete operation points to the reactant molecules
		push @{$rr->EdgeDel}, [ $aggMapR[$im1] . ".$ic1", $aggMapR[$im2] . ".$ic2" ];
	}


    # Edges added : Need to convert pointers to products back to pointers to reactants
    # There is a problem for molecules that appear in products but not in reactants.
    # For these, the only way to add the edge is to assign the mapping of product molecules
    # onto the selected graphs
	foreach my $iedgeP ( 0 .. $#{$pg->Edges} )
	{
		next unless ( $eused[$iedgeP] == 0 );
		my @ps = split / /, $pg->Edges->[$iedgeP];
		next unless ( @ps==2 );
		my $p1P = $ps[0];
		my $p2P = $ps[1];
		my ( $im1, $ic1 ) = split /\./, $p1P;
		my ( $im2, $ic2 ) = split /\./, $p2P;
		$p1P = $aggMapP[$im1] . ".$ic1";
		$p2P = $aggMapP[$im2] . ".$ic2";

        #printf "Rule %s: Edge %s %s is added (product side)\n", $rr->Name,$p1P, $p2P;
		push @{$rr->EdgeAdd}, [ $p1P, $p2P ];
	}


    # Check for invalid half-bonds
    while ( my ($ptr_P,$adj_P) = each %{$pg->Adjacency} )
    {
        unless ( ref $adj_P eq 'HASH' )
        {   # ptr_P has a half-bond. Find corresponding reactant, if any.
            my $ptr_R = $map->MapR->{$ptr_P};

            # get molecule and component index
            my ($im,$ic) = split /\./, $ptr_P;
            # convert to p.m.c format
            my $comp = $aggMapP[$im] . ".$ic";

            if ( $ptr_R == -1 )
            {   # product component is newly synthesized; half-bond is invalid.
                exit_error( "New product component $comp has invalid half-bond in rule:", $rr->toString() );
            }
            elsif ( ! exists $rg->Adjacency->{$ptr_R}  or  ref $rg->Adjacency->{$ptr_R} eq 'HASH' )
            {   # corresponding reactant component has a full-bond; half-bond is invalid
                exit_error( "Product component $comp has new or redefined half-bond in rule:", $rr->toString() );
            }
        }
    }


	# Parse component state changes
	foreach my $imR ( 0 .. $#{$rg->Molecules} )
	{
		my $imP = $map->MapF->{$imR};
		#print "$imR->$map->MapF->{$imR}\n";
		next unless ( $imP >= 0 );
		
		# grab components
		my $componentsR = $rg->Molecules->[$imR]->Components;
		my $componentsP = $pg->Molecules->[$imP]->Components;
		
		# loop over components
		foreach my $icR ( 0 .. $#{$componentsR} )
		{   
		    # get product component index
			my ( $imP, $icP ) = split /\./, $map->MapF->{"$imR.$icR"};

            # Get reactant and product state
            my $stateR = $componentsR->[$icR]->State;
			my $stateP = $componentsP->[$icP]->State;

            # check for defined state transforming to a wildcard (NOT ALLOWED!)
			if ( defined($stateR)  and  !defined($stateP) )
			{
				exit_error( "Component with state attribute defined in reactant pattern cannot map"
					        ." to component with undefined state attribute in product pattern",
					        $rr->toString()
				          );
			}  

            # if stateR is different than stateP, write state change transformation
            # (be careful that we're not comparing undefined things!)
			if ( (defined $stateR ? $stateR : '') cmp (defined $stateP ? $stateP : '') )
			{
				my $pR = "$aggMapR[$imR].$icR";
	            #printf "Component %s changed state from %s to %s\n", $pR, $stateR, $stateP;
				push @{$rr->CompStateChange}, [ ($pR, $stateR, $stateP) ];
			}
		}
	}



    # Compartment changes
    # code implemented by: justinshogg@gmail.com
    # 21 feb 2009
    #
    # There are two types of compartment changes.
    # (1) Species transport:  The entire speces is transported.
    #      syntax:  @C1:SpeciesX -> @C2:SpeciesX
    #
    #      Restrictions:
    #      (a) Species transport is only allowed if the rule
    #           does not add or remove molecules from the species pattern.
    #      (b) Transport is volume to volume  OR  surface to surface only.
    #      (c) surface to surface transport is restricted to surface pairs
    #          connected by a volume.
    #
    #      Note: molecules bound to a surface species will be transported to
    #       a new compartment based on topological inference (endo/exo-cytosis)
    #
    #      At this time we can check for illegal movement between 2-D and 3-D compartments.
    #
    #      To implement species compartment change:
    #        push @changeCompartment, ["i_pattR", origin_compart, destination_compartment, cytosis]
    #
    #      cytosis =  1  surface-to-surface endocytic species transport (invert Outside/Inside)
    #                -1  surface-to-surface exocytic species transport  (preserve Outside/Inside)
    #                 0  otherwise (volume-to-volume species transport and all molecule transport).    
    #
    # (2) Molecule transport:  Only the molecule is transported.
    #      syntax:  MoleculeX()@C1 -> MoleculeX()@C2
    #
    #      Restrictions:
    #      (a) Molecule transport only between adjacent compartments.
    #      (b) Molecule compartment must be compatible with explicit species compartment
    #             [final check occurs at time of rule application]
    #
    #      To implement molecule compartment change:
    #        push @changeCompartment, ["i_pattR.i_molR", origin_compart, destination_compart, cytosis]
    #

    # Implementation: (1) Species Transport
    #
    # We need to be careful here.
    # Define mappings:
    #   mapPattR:  {0,1,..,|ReactantPatterns|}  ->  {-2,-1,0,1,..,|ProductPatterns|}
    #
    #   mapPattR(I) =  K >= 0  if for all molecules j in ReactantPattern[I]:  mapF(I.j) == K.l  for some l,
    #                            and for all molecules l in ProductPattern[K]:  mapF(K.l) == I.j  for some j.
    #   mapPattR(I) = -1       if ReactantPattern[i] is deleted by the reaction rule.
    #   mapPattR(I) = -2       otherwise.
    #
    #  We'll construct this map in three steps.
    #  (i)   build the forward map R->P and store at @mapPattR
    #  (ii)  build the reverse map P->R and store at @mapPattP
    #  (iii) construct the final map $mapPattR by comparing the forward and reverse maps.

	# First Rewrite forward map entries into form:
	# mapf{pR.mR.cR}= "pP.mP.cP";
	my $mapf_new = {};
	foreach my $im1 ( 0 .. $#{$rg->Molecules} )
	{
		my $im2 = $map->MapF->{$im1};
		my $pm1 = $aggMapR[$im1];
		my $pm2 = ( $im2 >= 0 ) ? $aggMapP[$im2] : $im2;
		$mapf_new->{$pm1} = $pm2;

		#print "map_new: $pm1 -> $pm2\n";
		my $components = $rg->Molecules->[$im1]->Components;
		foreach my $ic1 ( 0 .. $#{$components} )
		{
			my ( $im2, $ic2 ) = split /\./, $map->MapF->{"$im1.$ic1"};
			if ( $im2 >= 0 )
			{
				$mapf_new->{"$pm1.$ic1"} = "$pm2.$ic2";
				#print "map_new: $pm1.$ic1 -> $pm2.$ic2\n";
			}
			else
			{
				$mapf_new->{"$pm1.$ic1"} = $im2;
				#print "map_new: $pm1.$ic1 -> $pm2.$ic2\n";
			}
		}
	}

	# Rewrite reverse map to entries in form:
	# mapr{pP.mP.cP}= "pR.mR.cR";
	my $mapr_new = {};
	foreach my $im1 ( 0 .. $#{$pg->Molecules} )
	{
		my $im2 = $map->MapR->{$im1};
		my $pm1 = $aggMapP[$im1];
		my $pm2 = ( $im2 >= 0 ) ? $aggMapR[$im2] : $im2;
		$mapr_new->{$pm1} = $pm2;

		#print "map_new: $pm1 -> $pm2\n";
		my $components = $pg->Molecules->[$im1]->Components;
		foreach my $ic1 ( 0 .. $#{$components} )
		{
			my ( $im2, $ic2 ) = split /\./, $map->MapR->{"$im1.$ic1"};
			if ( $im2 >= 0 )
			{
				$mapr_new->{"$pm1.$ic1"} = "$pm2.$ic2";
				#print "map_new: $pm1.$ic1 -> $pm2.$ic2\n";
			}
			else
			{
				$mapr_new->{"$pm1.$ic1"} = $im2;
				#print "map_new: $pm1.$ic1 -> $pm2.$ic2\n";
			}
		}
	}


	{
		my @mapPattR = (); # map from reactant patterns to product patterns (-1 if deleted, -2 if no consistent map)
		my @mapPattP = (); # map from product patterns to reactant patterns (-1 if added, -2 if no consistent map)

        # step (i)
        # first find consistent mappings from each Reactant pattern into a Product pattern (or null pattern).
		my $i_R = 0;
		for ( my $i_pattR = 0;  $i_pattR < @{$rr->Reactants};  $i_pattR++ )
		{
		    foreach my $molR ( @{$rr->Reactants->[$i_pattR]->Molecules} )
			{
				my $i_pattP_molP = $mapf_new->{ $aggMapR[ $i_R++ ] };
				my ($i_pattP) = split /\./, $i_pattP_molP;

				# if pattR mapping has already failed, do nothing
				if ( exists $mapPattR[$i_pattR] )
				{
					# if inconsistent, set mapPattR = -2
					unless ( $mapPattR[$i_pattR] == $i_pattP )
					{   $mapPattR[$i_pattR] = -2;   }
				}
				else
				{   # assign pattern mapping ( i_pattP, or -1 if deletion )
					$mapPattR[$i_pattR] = $i_pattP;
				}
			}
		}

        # step (ii)
        # now find consistent mappings from a Product pattern into a Reactant pattern (or null pattern)
		my $i_P = 0;
		for ( my $i_pattP = 0;  $i_pattP < @{$rr->Products};  $i_pattP++ )
		{
		    foreach my $molP ( @{$rr->Products->[$i_pattP]->Molecules} )
			{
				my $i_pattR_molR = $mapr_new->{ $aggMapP[ $i_P++ ] };
				my ($i_pattR) = split /\./, $i_pattR_molR;

				# if pattR mapping has already failed, do nothing
				if ( exists $mapPattP[$i_pattP] )
				{
					# if inconsistent, set mapPattR = -2
					unless ( $mapPattP[$i_pattP] == $i_pattR )
					{   $mapPattP[$i_pattP] = -2;   }
				}
				else
				{   # assign pattern mapping ( i_pattP, or -1 if deletion )
					$mapPattP[$i_pattP] = $i_pattR;
				}
			}
		}

        # step (iii)
        # Now check that that reverse mapping is inverse of forward mapping (where defined).
        #  Pattern mapping is okay if:  $mapPattP[ $mapPattR[i] ] = i  OR  $mapPattR[i] == -1.
		for ( my $i_pattR = 0;  $i_pattR < @{$rr->Reactants};  $i_pattR++ )
		{
			next if ( $mapPattR[$i_pattR] < 0 );
			unless ( $i_pattR == $mapPattP[ $mapPattR[$i_pattR] ] )
			{
			    # NOTE: the assignment was orginally an equality, is this right now?? --justin
				$mapPattR[$i_pattR] = -2;
			}
		}

        # Now check that that forward mapping is inverse of reverse mapping (where defined).
        #  Pattern mapping is okay if:  $mapPattR[ $mapPattP[i] ] = i  OR  $mapPattP[i] == -1.
		for ( my $i_pattP = 0;  $i_pattP < @{$rr->Products};  $i_pattP++ )
		{
			next if ( $mapPattP[$i_pattP] < 0 );
			unless ( $i_pattP == $mapPattR[ $mapPattP[$i_pattP] ] )
			{
				$mapPattP[$i_pattP] = -2;
			}
		}

		# debug: print out Pattern Map
		#for (my $i=0; $i < @mapPattR; $i++)
		#{
		#   print STDERR "mapPattR  $i  $mapPattR[$i]\n";
		#}

		##  Pattern Mapping completed
		##   ..now looking for transport reactions.

		# Now that we have a Pattern map, we can check for Compartment changes
		for ( my $i_pattR = 0; $i_pattR < @{$rr->Reactants}; $i_pattR++ )
		{
			# case 1: reactant pattern is deleted  =>  no transport.
			if ( $mapPattR[$i_pattR] == -1 ) {  next;  }

            # case 2: reactant pattern does not have valid map to product pattern
            #  => can't change a species' location if the molecule composition has changed.
			elsif ( $mapPattR[$i_pattR] == -2 )
			{
                # Removed by Justin -- we're being permissive about species syntax.
                # # species compartment declaration is invalid without map to product pattern
                # if ( defined $rr->Reactants->[$i_pattR]->Compartment )
                # {
                #   exit_error(
                #     "Reaction Rule specifies a species compartment for reactant in which"
                #     ." molecules are removed or added.", $rr->toString()
                #   );
                # }
				next;
			}

			# case 3: reactant pattern has valid map to product pattern
			# get index of corresponding product pattern
			my $i_pattP = $mapPattR[$i_pattR];

			# species compartment defined on both sides of reaction
			my $compR = $rr->Reactants->[$i_pattR]->Compartment;
			my $compP = $rr->Products->[$i_pattP]->Compartment;

			# if product compartment is not defined, there is no species transport.
			#  (reactant compartment, if defined, will provide matching context.)
			next if ( !defined($compP) );

            # if product compartment defined, but not reactant, then no Transport.
            # Assume that species compoartment is just a post-check on the final product.
            # NOTE: this could potentially define a generic transport.
			next if ( !defined($compR) and defined($compP) );

            # Removed by Justin -- we're being permissive about species syntax.
            # # error if compartment is defined for one, but not both.
            # if (  defined($compR)  xor  defined($compP)  )
            # {
            #   exit_error(
            #     "Reaction Rule specifies a species compartment for a pattern on one"
            #     ." side of the reaction but not for the corresponding species on the"
            #     ." other side of the reaction.", $rr->toString()
            #   );
            # }

			# case 3A: compartments are the same.  no transport
			next if ( $compR == $compP );

			# transport must move species from surface to surface
			#  OR  volume to volume
			unless ( $compR->SpatialDimensions == $compP->SpatialDimensions )
			{
				exit_error( "Reaction Rule attempts to transfer a species between compartments"
					        ." of non-equal SpatialDimensions.",
					        $rr->toString()
				          );
			}

			# case 3B: volume to volume
			# (no restrictions on adjacency here!)
			if ( $compR->SpatialDimensions == 3 )
			{
				push @{$rr->ChangeCompartment}, [ $i_pattR, $compR, $compP, 0 ];
			}

			# case 3C: surface to surface
			if ( $compR->SpatialDimensions == 2 )
			{
				# only allowed between surfaced connected by volume (this is a mimick of endo/exo-cytosis)
				my $cytosis = $compR->separated_by_volume($compP);
				unless ($cytosis) {
					exit_error( "Reaction Rule attempts to transport a membrane species to a membrane"
						         ." which is not connected to the source compartment by a volume.",
						        $rr->toString()
					          );
				}
				push @{$rr->ChangeCompartment}, [ $i_pattR, $compR, $compP, $cytosis ];
			}
		}

        # Removed by Justin -- we're being permissive about species syntax.
        # # One last thing:  check for invalid compartment specification on Product side
        # for ( my $i_pattP = 0; $i_pattP < @{$rr->Products}; $i_pattP++ )
        # {
        #   # does product pattern not have a valid map to a reactant pattern?
        #   if ( $mapPattP[$i_pattP] == -2 )
        #   {
        #     # species compartment declaration is invalid without map to product pattern
        #     if ( defined $rr->Products->[$i_pattP]->Compartment )
        #     {
        #       exit_error(
        #         "Reaction Rule specifies a species compartment for product from which"
        #         ." molecules have been removed or added.", $rr->toString()
        #       );
        #     }
        #     # otherwise okay.  nothing to do.
        #     next;
        #   }
        # }

	}    # done handling species transport



	# (2) Molecule Transport
	# only allowed between adjacent compartments or compartments
	# separated by a surface
	# begin loop though reactant molecules (track by index)
	for ( my $i_molR = 0;  $i_molR < @{$rg->Molecules};  $i_molR++ )
	{
		# get reactant molecule compartment
		my $molR_comp = $rg->Molecules->[$i_molR]->Compartment;

		# get reactant pattern index and compartment (if any)
		my ( $i_pattR ) = split /\./, $aggMapR[$i_molR];
		my $pattR_comp = $rr->Reactants->[$i_pattR]->Compartment;

		# check that molecule and pattern compartments are compatible
		if ( defined($pattR_comp)  and  defined($molR_comp) )
		{
			# this isn't perfect.  need a "verify_compartments" method
			unless ( $pattR_comp->adjacent($molR_comp) )
			{
				exit_error( "Reaction Rule molecule and pattern compartments are not compatible.",
					        $rr->toString()
				          );
			}
		}

		# get mapping from reactant molecule to product molecule
		my $i_molP = $map->MapF->{$i_molR};

		# skip if reactant does not map to a product
		next unless ( $i_molP >= 0 );

		# get product molecule compartment
		my $molP_comp = $pg->Molecules->[$i_molP]->Compartment;

		# get product pattern index and compartment (if any)
		( my $i_pattP ) = split /\./, $aggMapP[$i_molP];
		my $pattP_comp = $rr->Products->[$i_pattP]->Compartment;

		# check that molecule and pattern compartments are compatible.
		if ( defined($pattP_comp)  and  defined($molP_comp) )
		{
			# this isn't perfect.  need a "verify_compartments" method
			unless ( $pattP_comp->adjacent($molP_comp) )
			{
				exit_error( "Reaction Rule molecule and pattern compartments are not compatible.",
					        $rr->toString()
				          );
			}
		}

		# cases
		# (1) neither compartment defined.  no transport.
		next if ( !defined($molR_comp) and !defined($molP_comp) );

        # (2) error if compartment specified for reactant or product, but not both.
		if ( defined($molR_comp)  xor  defined($molP_comp) )
		{
			exit_error( "Reaction Rule specifies compartment for either the reactant or"
				        ." product molecule, but not both.",
        				$rr->toString()
			          );
		}

		# (3A) both product and reactant molecule compartments defined.
		# do nothing if compartments are equal.
		next if ( $molR_comp == $molP_comp );

		# (3B) compartment changes.  verify integrity of compartment change
		# check if compartments are adjacent or separated by a surface
		#if (    $molR_comp->adjacent($molP_comp)
	    #     or $molR_comp->separated_by_surface($molP_comp) )
		#{
			push @{$rr->ChangeCompartment},
			    [ "$aggMapR[$i_molR]", $molR_comp, $molP_comp, 0 ];
		#}
		#else
		#{
		#	exit_error( "Reaction rule moves reactant molecule to a compartment that is not"
        #                ." adjacent to or separated by a surface from the orginal compartment.",
	    #		        $rr->toString()
		#	          );
		#}
	}    # end loop through reactant molecules

    ## debug: show compartment change operations
    #foreach my $op (@{$rr->ChangeCompartment})
    #{
    #    print STDERR "\n", $rr->toString(), "\n";
    #    print STDERR "ptr: ", $op->[0], "  comp_origin: ", $op->[1]->Name,
    #                 "  comp_dest: ", $op->[2]->Name, "  cytosis: ", $op->[3], "\n";
    #}

	## finished compartment changes.



	### NOTE: replaced symmetry handler with canonical reaction labeling ###
	# However, this is still useful for NFsim!  So we'll calculate the MultScale
	#  and pass it to NFsim through the XML file.

    # Determine the reaction center of this rule
    my $err = $rr->find_reaction_center( $mapr_new );
    if ($err) { exit_error( "Some problem finding reaction center: $err", $rr->toString() ); }

	## DETERMINE CORRECTION FACTOR FOR SYMMETRY ##
    #  |AutR| = number of automorphisms of the reactant patterns
    #  |AutR| = [AutR:RG]*|RG| = [AutR:RG]*|Stab(RxnCntr)|*|CG|
    #
    #  where  RG = RuleGroup (see below)
    #         Stab(RxnCenter) = stabilizer of the reaction center in RG
    #         CG = CenterGroup (symmetries in reaction center).

    #  In the rule application phase, we lump reactants matches that are equivalent
    #  up to reaction center.  Therefore it's important to only correct for
    #  symmetries in the reaction center. (but context consideration is important!)

	# Find reactant graph automorphisms
	my @r_auto   = $rg->isomorphicToSubgraph($rg);
	# Find product graph automorphisms
	my @p_auto   = $pg->isomorphicToSubgraph($pg);
	

	# Set up product automorphism hash (includes identity - trivial autmorphism)
	my %p_auto_hash = ();
	foreach my $auto (@p_auto)
	{  $p_auto_hash{ $auto->toString() } = 1;  }

	# For each R automorphism we're checking the existence
	# of a P automorphism.  Specifically, find autoP s.t for all r
	# in the domain of map:
	#    map o autoR == autoP o map
	#
	# If we can do this, then we have a symmetry and autoR is an element
	# of the Rule Group, RG.  If not, the automorphism corresponds to a new
	# class of reaction mechanism.
	#
	# See "getInducedPermutation" for more notes.
	my @RuleGroup = ();
	foreach my $auto ( @r_auto )
	{
	    my ($permP, $err) = $auto->get_induced_permutation( $map );
	    if ( exists $p_auto_hash{ $permP->toString() } )
	    {   push @RuleGroup, $auto;   }
	}

	# next find the Stabilizer of the Reaction Center
	# i.e. the automorphisms in RG that preserve the reaction center
	#  Stab(RxnCntr) = { f in RG | f(x) = x  for all x in RxnCenter }
	my @StabRxnCntr = ();

    RG_LOOP:
    # check each automorphism
    foreach my $auto (@RuleGroup)
    {
        # need to figure out pattern mapping that is implicit in the molecule mapping
        my %patt_map = ();
        while ( my ($src_ptr, $targ_ptr) = each %{$auto->MapF} )
        {
            next unless ( $src_ptr =~ /^\d+$/ );
            
            my ($src_patt)  = ( $aggMapR[$src_ptr]  =~ /^(\d+)/ );
            my ($targ_patt) = ( $aggMapR[$targ_ptr] =~ /^(\d+)/ );
        
            $patt_map{$src_patt} = $targ_patt;
        }
    
        #print join( ', ', map { "$_ -> $patt_map{$_}" } (keys %patt_map) ), "\n";
    
        # loop over reaction patterns (one array for each reactant pattern)
        foreach my $Rpatt (@{$rr->ReactionCenter})
        {
            # check each center element on this pattern
            foreach my $center_element (@$Rpatt)
            {
                if ($center_element =~ /\./)
                {   # handle molecule and component elements
                    
                    # get pointer, excluding component
                    (my $ipatt_imol) = ( $center_element =~ /^(\d+\.\d+)/ );
    
                    # need to convert p.m.c format into m.c format. Ideally, a map
                    # should be set up simulateous with the construction of aggMapR
    
                    # find p.m in @aggMapR
                    my $imolAgg;
                    for (my $i=0; $i < @aggMapR; $i++)
                    {
                        if ($aggMapR[$i] eq $ipatt_imol)
                        {
                            $imolAgg = $i;
                            last;
                        }
                    }
    
                    # subsitute p.m with m
                    my $imolAgg_icomp = $center_element;
                    $imolAgg_icomp =~ s/^(\d+\.\d+)/$imolAgg/;
    
                    # toss automorphism if center element is not conserved
                    next RG_LOOP unless ( $auto->MapF->{$imolAgg_icomp} eq $imolAgg_icomp );
                }
                else
                {
                    # handle pattern elements
                    next RG_LOOP unless ( $patt_map{$center_element} eq $center_element );
                }
            }
        }
        push @StabRxnCntr, $auto;
    }


    # determine is any reactant graphs are pure context
    my @context_rgs = ();
    for (my $iR = 0; $iR < @{$rr->Reactants}; ++$iR )
    {
        if ( @{$rr->ReactionCenter->[$iR]}==0 )
        {   # this reactant pattern has no reaction center!
            push @context_rgs, $rr->Reactants->[$iR];
        }
    }

    # count the instances of each unqiue context graph (up-to-isomorphism)
    #  and calculuate the permutations of the context graph set
    my $crg_permutations = 1;
    while ( my $crg = shift @context_rgs )
    {
        my $instances = 1;
        my $iR = 0;
        while ( $iR < @context_rgs )
        {
            # TODO: make sure isomorphicTo works correctly on patterns!
            if ( $crg->isomorphicTo($context_rgs[$iR]) )
            {   # toss out this graph and increment instances
                splice @context_rgs, $iR, 1;
                ++$instances;
            }
            else
            {   ++$iR;   }
        }
        $crg_permutations *= RxnRule::factorial($instances);
    }


	my $multScale = 1 / (@RuleGroup/@StabRxnCntr) / $crg_permutations;
	$rr->MultScale($multScale);
	
	## debug symmetry output
    #print $rr->toString(), "\n";
    #print STDERR "|autR|    = ", scalar(@r_auto), "\n";
    #print STDERR "|autP|    = ", scalar(@p_auto), "\n";
    #print STDERR "|RG|      = ", scalar(@RuleGroup), "\n";
    #print STDERR "|StabRC|  = ", scalar(@StabRxnCntr), "\n";
    #print STDERR "CRG perms = ", $crg_permutations, "\n";
    #print STDERR "multScale = ", $multScale, "\n";


	# save maps
	$rr->MapF( $mapf_new );
	$rr->MapR( $mapr_new );

	# print "=====\n", $rr->toString(), "\n";
	# foreach my $key ( keys %{$rr->MapF} )
	# {
	#    print $key, " -> ", $rr->MapF->{$key}, "\n";
	# }
	# print "-----\n";
	# foreach my $key ( keys %{$rr->MapR} )
	# {
	#    print $key, " -> ", $rr->MapR->{$key}, "\n";
	# }

	return '';
}





##-----------------------##
## Rule application loop ##
##-----------------------##
{
	my $pattDepth = -1;
	my $maxDepth;
	my $check_iso  = 1;
	my $max_agg    = 1e99;
	my $max_stoich = '';
	my $verbose;
	my ( $rr, @sgs, @matches, @isnew, $new_level, @targets, @reactant_species );
	my $n_new_rxns;
	my @rxns;
	my ( $slist, $rlist );    # RxnList and SpeciesList
	my ( $rtype, $dlist );
	my $plist;
    my $rmatches;


	# $rr->applyRule(SpeciesList, RxnList, (Species))
	sub applyRule
	{
        # Execute this block on entry to applyRule.  i.e.
        # Only need to read arguments when $pattDepth is negative
        #
        # GOAL:  find matches to reactant patterns in species list
        # store matches in @rmatches, where $rmatches[i] is an array of matches to pattern i.
		if ( $pattDepth == -1 )
		{
			$rr    = shift @_;    # reaction rule
			$slist = shift @_;    # species list
			$rlist = shift @_;    # reaction list (to be filled?)
			$plist = shift @_;    # parameter list

            # DEBUG
            #print STDERR $rr->toString(), "\n";

			unless ( ref $plist eq 'ParamList' )
			{
				exit_error( "plist argument is not type ParamList.",
					        $rr->toString );
			}

			$maxDepth   = $#{$rr->Reactants};
			$n_new_rxns = 0;

			# Handle deletion rules
			$rtype = $rr->RateLaw->Type;
			if   ( $rtype eq "Zero" ) { $dlist = RxnList->new; }
			else                      { $dlist = ''; }

			# Determine to which species rules are to be applied
			my $species;
			if (@_)
			{   # User-provided array
				$species = shift;
			}
			else
			{   # Array in species list
				$species = $slist->Array;
			}


			# only look for matches in species that we haven't
			# applied the rules to yet.
			my @sgs = ();
			foreach my $spec (@$species)
			{
				unless ( $spec->RulesApplied )
				{
					push @sgs, $spec->SpeciesGraph;
				}
			}


	        # Next argument is optional parameter hash
	        # Any parameters that don't have defaults set here will inherit values set
	        # by previous calls to this function
	        # SAFER would be to make these attributes of BNGModel and automatically
	        # pass them to this function if they are defined
			if (@_)
			{
				my $params = shift;
				if ( defined $params->{max_agg} ) 
				{
					$max_agg = $params->{max_agg};
				}
				else
				{
					#$max_agg=1e99;
				}
				if ( defined $params->{check_iso} )
				{
					$check_iso = $params->{check_iso};
				}
				else
				{
					#$check_iso=1;
				}
				if ( defined $params->{max_stoich} )
				{
					$max_stoich = $params->{max_stoich};
				}
				else
				{
					#$max_stoich="";
				}
				if ( defined $params->{verbose} )
				{
					$verbose = $params->{verbose};
				}
				else
				{
					#$verbose=0;
				}

                # initialize Rmatches with empty array references
                foreach my $reac ( @{$rr->Reactants} )
                {
                    push @{$rr->Rmatches}, [];
                }
			}

			@rxns = ();

			# Map rule patterns onto SpeciesGraphs
			my $ipatt = 0;
			$rmatches = $rr->Rmatches;
			my $new_matches_found = 0;
			$new_level = -1;
			my $nsets = 1;

			foreach my $patt ( @{$rr->Reactants} )
			{
				my @matches         = ();
				my $include_default = scalar @{$rr->Rinclude->[$ipatt]} ? 0 : 1;
				my $targ_last       = undef;
				my $include_last    = '';

                # Check if $match->Target has subgraph isomorphic to excluded graph
                # keep matches if included and not excluded
				foreach my $match ( $patt->isomorphicToSubgraph(@sgs) )
				{
					my $targ    = $match->Target;
					my $include = $include_default;

					# Only do new isomorphism check if $targ has changed
			        if ( !defined($targ_last)  or  $targ != $targ_last )
			        {
						foreach my $patt_incl ( @{ $rr->Rinclude->[$ipatt] } )
						{
							if ( $patt_incl->isomorphicToSubgraph($targ) )
							{
                                # Include this match from list since graph matches included graph
                                #print "Reactant ", $targ->toString()," matches included pattern\n";
								$include = 1;
								last;
							}
						}
						foreach my $patt_excl ( @{ $rr->Rexclude->[$ipatt] } )
						{
							if ( $patt_excl->isomorphicToSubgraph($targ) )
							{
                                # Exclude this match from list since graph matches excluded graph
                                #print "Reactant ", $targ->toString()," matches excluded pattern\n";
								$include = 0;
								last;
							}
						}
					}
					else { $include = $include_last; }
					# done checking include and exclude graphs


					if ($include)
					{
						push @matches, $match;
						#print "Included ", $targ->toString(),"\n";
					}
					else
					{
						#print "Excluded ", $targ->toString(),"\n";
					}
					$targ_last    = $targ;
					$include_last = $include;
				}
                # end match loop


                # group matches by target
                my %matches = ();
                my @target_order = ();                
                while (my $match = shift @matches)
                {
                    unless ( exists $matches{$match->Target} )
                    {
                        $matches{$match->Target} = [];
                        push @target_order, $match->Target;
                    }
                    push @{$matches{$match->Target}}, $match;
                }

                # compare matches to the same target species to see if
                # the reaction center is identical.
                #
                # specifically, let f[i] be the map from reactant pattern into
                # a species induced by match[i].  Let f[i]|rxn_center be the
                # restriction of f[i] to the subset of nodes in the rxn_center
                # of the reactant pattern.
                #
                # If match[i]->Target == match[j]->Target
                #  and
                # f[i]|rxn_center == f[j]|rxn_center
                #
                # then the match[j] is the same up to the reaction center as match[i]
                # and we can, wlog, filter match[j].
                foreach my $match_set (values %matches)
                {
                    my $err = $rr->filter_identical_by_rxn_center( $match_set, $ipatt );
                    if ($err) {  exit_error( "Some error comparing matches by reaction center: $err.", $rr->toString );  }
                }

				# count matches
				# add matches to rmatches->[$ipatt]
				my $n_new = 0;
                foreach my $target ( @target_order )
                {
                    my $match_set = $matches{$target};
				    $n_new += scalar @$match_set;
				    push @{$rmatches->[$ipatt]}, @$match_set;
				}

				#my $n_new = scalar @matches;  # count of matches to this pattern
				$new_matches_found += $n_new;    # cumulative count of new matches to all patterns

				# save new matches to "rmatches" structure
				#push @{$rmatches->[$ipatt]}, @matches;

				# what's this for?  --justin
				if ($n_new) {  $new_level = $ipatt;  }

				# total matches to this pattern
                my $n_tot = scalar @{$rmatches->[$ipatt]};

			    # calculate number of sets (crossproduct of matches to each pattern)
				$nsets *= $n_tot;

				if ($verbose)
				{
					printf " %4d new (%4d total) matches for reactant %d\n",
					  $n_new, $n_tot, $ipatt + 1;
				}

				++$ipatt;
			}

			# done looking for matches to reactant patterns
			#print "new_level=$new_level max_level= $maxDepth\n";

			# Don't do any more work unless $matches_found
			unless ($new_matches_found)
			{
				#print "No new matches found.\n";
				return (0);
			}

			# Don't do any more work unless at least one possible reactant set
			# ($nsets) is found
			unless ($nsets)
			{
				#print "No complete match sets.\n";
				return (0);
			}

			++$pattDepth;
			@matches   = ();
			@isnew     = ();
			@targets   = ();
			@reactant_species = ();
		}
		# END applyRule entry block


		# BEGIN handling matches
		# GOAL: apply rule to crossproduct of PI_i{ $rmatches[i] }

		# Loop over matches at current level
	  MATCH:
		foreach my $match ( @{$rmatches->[$pattDepth]} )
		{
			$matches[$pattDepth]          = $match;                   # a map
			$targets[$pattDepth]          = $match->Target;           # a species graph
			$reactant_species[$pattDepth] = $match->Target->Species;  # a species
			$isnew[$pattDepth]            = !( $match->Target->Species->RulesApplied );

			#printf "isnew=%d\n", $match->Target->Species->RulesApplied;

            # Only use current set if it contains a match to a new species or there are new
            # matches below the current level
			my $new_set = 0;
			foreach my $depth ( 0 .. $pattDepth )
			{
				if ( $isnew[$depth] )
				{
					$new_set = 1;
					last;
				}
			}

			# Skip set if no new matches below the current level
			unless ($new_set)
			{
				if ( $new_level <= $pattDepth )
				{
					#print "skip\n";
					next MATCH;
				}
			}


			# We need to find a match to every reactant, if not at maxDepth,
			# call applyRule recursively to get a match to the next reactant.
			# Descend to next level:
			if ( $pattDepth < $maxDepth )
			{
				++$pattDepth;
				&applyRule;
				next MATCH;    # next MATCH?
			}

			##----------------------------------------------##
			##  END of RECURSIVE search for a reactant set  ##
			##----------------------------------------------##
                


			if ( @targets > 1 )
            {   # check that reactants form an interacting Set (compartment check)
				next MATCH unless ( $targets[0]->interactingSet( @targets[1 .. $#targets] ) );
			}


            # Determine if we are using compartments.
            # (it's sufficient to see if the first reactant has a compartment assignment)
			my $using_compartments = defined $targets[0]->Compartment ? 1 : 0;

			# gather compartments to pass to Rxn
			# NOTE: this really isn't necessary?
			my $compartments = [];
			if ($using_compartments)
			{
				foreach my $targ (@targets)
				{
					push @{$compartments}, $targ->Compartment;
				}
			}


			## Apply transformations
			my $products = undef;
		    ($products) = $rr->apply_operations( \@matches );
		    # if undefined products are returned, it probably means we violated the "+" or "." operator
		    next MATCH unless (defined $products);


            # Check for correct number of product graphs
	        my $nprod_patterns = scalar @{$rr->Products};
	        if ( @$products != $nprod_patterns )
	        {
                # If Molecules are being deleted, it is allowed to have more subgraphs than product patterns
                if ( $rr->DeleteMolecules  and  @$products > $nprod_patterns )
		        {
			        if ($verbose)
   		            {   printf "Deleting molecules in rule %s\n", $rr->Name();  }
		        }   
		        # Otherwise, the reaction shouldn't happen
		        else
		        {
			        if ($verbose)
			        {   printf "Rule %s: n_sub (%d)!= n_prod (%d)\n", $rr->Name, @$products, $nprod_patterns;   }
			        next MATCH;
		        }
	        }            
            

			# Check and Process Product Graphs
			my $new_prod = 0;
			my $product_species = [];
		    for ( my $ip = 0; $ip < @$products; ++$ip )
			{
			    my $p = $products->[$ip];
			    my $iprod = ($ip >= @{$rr->Products}) ? -1 : $ip;

                # check that the product is connected
                # NOTE: molecules are placed into products according to (1) a mapping from product molecules
                #  to product product patterns and (2) connectivity. Therefore it's possible the disconnected
                #  components have been lumped into the same graph.  So at this point we want to check for connectivity.
                next MATCH  unless ( $p->isConnected() );

                # check for Max Aggregation violations
				if ( @{$p->Molecules} > $max_agg )
				{
                    #printf "%d molecules exceeds max_agg of %d\n", $#{$p->Molecules}+1, $max_agg;
					next MATCH;
				}
				
				# check for Max Stoichiometry violations
				if ($max_stoich)
				{
					foreach my $key ( keys %{$max_stoich} )
					{
						my $max = $max_stoich->{$key};
						next if $max =~ /^unlimited/;
						if ( $p->stoich($key) > $max )
						{
                            #printf "Stoichometry of $key in %s exceeds max of %d\n", $p->toString(), $max;
							next MATCH;
						}
					}
				}

				# verify compartments here
				# justinshogg@gmail.com 18feb2009
				# updated to handle auto assignment of new molecules  2dec2009

                # check is here because we have a complete product speciesGraph available
                # and problems occur during canonical ordering if compartment is not valid.

				## TASKS ##
				# (1) Verify that all bonded molecules are in the same or adjacent compartments.
				#      If verification fails, skip this MATCH.  A compartment is inferred for
				#      unassigned molecules if bound to an assigned molecule (otherwise left unassigned).
				# (2) Infer, assign and verify Species Compartment.

                # (1a) Find molecules that share a common edge.
                #      For molecules without compartments, make a list of its binding
                #       partners' compartments.
				my %edges                = ();
				my %partner_compartments = ();

				foreach my $mol ( @{$p->Molecules} )
				{   # if molecule has edge "index", then put molecule ref into array at $edges[index]
					foreach my $component ( @{$mol->Components} )
					{
						foreach my $edge_idx ( @{$component->Edges} )
						{
							push @{ $edges{$edge_idx} }, $mol;
						}
					}
				}

                # (1b) check that bound molecules are in same or adjacent compartments
				foreach my $binding_set ( values %edges )
				{
					# how should I handle warnings?  ABORT?
					if ( @$binding_set > 2 )
					{
						print "WARNING: more than two molecules bound to edge.\n"
						    . ">>", $rr->toString(), "\n";
						next MATCH;
					}

					my $mol0  = $binding_set->[0];
					my $comp0 = $mol0->Compartment;
					my $mol1  = $binding_set->[1];
					my $comp1 = $mol1->Compartment;

					if ( defined $comp0  and  !(defined $comp1) )
					{
						$partner_compartments{$mol1}{$comp0} = $comp0;
					}
					elsif ( !(defined $comp0)  and  defined $comp1  )
					{
						$partner_compartments{$mol0}{$comp1} = $comp1;
					}
					elsif ( defined $comp0  and  defined $comp1 )
					{   # Both compartments are defined,
						# this is the usual case when comaprtments are defined.
						unless (  $comp0 == $comp1  or $comp0->adjacent($comp1) )
						{
							print "WARNING: rule application generated a species with edges between"
							    . " molecules in non-adjacent compartments. Removing ill-formed"
							    . " species from network. This isn't necessarily a problem, but it's"
							    . " a good idea to check your rules.\n"
							    . "RxnRule>", $rr->toString(), "\n" . "Species> ", $p->toString(), "\n";
							next MATCH;
						}
					}
				}

				# infer compartments for unassigned molecules, if possible
				foreach my $mol ( @{$p->Molecules} )
				{
					next unless ( exists $partner_compartments{$mol} );

					my @surfaces = ( grep {$_->SpatialDimensions == 2} ( values %{$partner_compartments{$mol}} ) );
					my @volumes  = ( grep {$_->SpatialDimensions == 3} ( values %{$partner_compartments{$mol}} ) );

                    # if neighbors are in a single volume, assign molecule to that volume
					if ( @volumes == 1  and  @surfaces == 0 )
					{
						$mol->Compartment( $volumes[0] );
					}
                    # if neighbors are in a single surface, assign molecule to that surface
					elsif ( @volumes == 0  and  @surfaces == 1 )
					{
						$mol->Compartment( $surfaces[0] );
					}
                    # if neighbors are in a single volume and an adjacent surface,
                    # assign molecule to that volume.
					elsif ( @volumes == 1  and  @surfaces == 1  and  $volumes[0]->adjacent($surfaces[0]) )
					{
						$mol->Compartment( $volumes[0] );
					}
                    # if neighbors are in a single surface and two adjacent volumes,
                    # assign molecule to that surface.
					elsif (     @volumes == 2  and  @surfaces == 1
						    and $volumes[0]->adjacent($surfaces[0])
						    and $volumes[1]->adjacent($surfaces[0]) )
					{
						$mol->Compartment( $surfaces[0] );
					}
					# neighbors are in some disallowed configuration
					else
					{
						print "WARNING: rule application created a species containing invalid "
						    . "compartment connectivity.\n"
						    . "RxnRule> ", $rr->toString(), "\n" . "Species> ", $p->toString(), "\n";
						next MATCH;
					}
				}

				# (2) infer species compartment, assign, and verify validity.
				my ( $infer_comp, $err ) = $p->inferSpeciesCompartment();

                # if a compartment cannot be inferred and the reactants are using compartments,
                # then we must pick a compartment for the product species.  By convention, if
                # all reactants are in the same compartment, assign product to that compartment.
                # But if the reactants are divided between a volume and a surface, choose the surface.
				if ( !defined($infer_comp)  and  $using_compartments )
				{
                    # find the surface, if any (just take the first one, there should only be one),
					foreach my $comp ( (map { $_->Compartment } @targets) )
					{
						next unless ( defined $comp );
						if ( $comp->SpatialDimensions == 2 )
						{
							$infer_comp = $comp;
							last;
						}
					}

                    # if no surface, choose the volume (should be only one)
                    # NOTE: we previous checked that these reactants are an interacting set
					unless ( defined $infer_comp )
					{
						$infer_comp = $targets[0]->Compartment;
					}
				}

                # Assign product species to inferred compartment (possibly undefined).  Note that
                #  this will force all unassigned molecules to the inferred compartment.
				$err = $p->assignCompartment($infer_comp);
				if ($err)
				{
					print "WARNING: product has invalid Species Compartment.\n"
					    . ">> $err\n"
					    . ">>", $rr->toString(), "\n";
					next MATCH;
				}

                # at this point, we've verified that the product SpeciesGraph is valid w.r.t. compartments
                # done with Justin's changes

				# Check that product species is in the correct compartment!
				if ( $iprod >= 0 ) 
				{   # notesure if iprod check is needed, but it's used below, so I'll follow.
					if ( defined $rr->Products->[$iprod]->Compartment )
					{
						next MATCH unless ( $infer_comp == $rr->Products->[$iprod]->Compartment );
					}
				}



	            # Put product graph in canonical order (quasi-canonical for the time being)
				if ( my $err = $p->sortLabel() )
				{   # mysterious problem
					print "WARNING: some problem in sortLabels.\n"
					    . ">> $err\n"
					    . ">>", $rr->toString(), "\n";
					next MATCH;
				}

				# Does product match excluded pattern?
				if ( $iprod >= 0 )
				{
					foreach my $patt_excl ( @{$rr->Pexclude->[$iprod]} )
					{
						if ( $patt_excl->isomorphicToSubgraph($p) )
						{
				            # Abort this reaction
				            #print "Product ", $p->toString()," matches excluded pattern\n";
							next MATCH;
						}
					}

                    # Does product match included pattern?  Must do so if include patterns are specified.
                    if (@{$rr->Pinclude->[$iprod]})
                    { 
					    my $include = 0;
					    foreach my $patt_incl ( @{$rr->Pinclude->[$iprod]} )
					    {
						    if ( $patt_incl->isomorphicToSubgraph($p) )
						    {
						    	$include = 1;
						    	last;
						    }
					    }
					    next MATCH unless $include;
					}
				}


				# Update species list; add checks whether this sg isomorphic to
				# existing species and creates new species if not.  It returns
				# a pointer to new or existing species with isomorphic sg.
				# If we want mapping for this reaction need to update to
				# account for reordering of molecules and components
				my $spec;
				unless ( $spec = $slist->lookup( $p, $check_iso ) )
				{
					$spec = $slist->add( $p, 0 );
					++$new_prod;
				}
				# Add the product Species
			    if ($iprod >= 0)
			    {
			        unless ($rr->Products->[$iprod]->Fixed)
			        {
			            push @$product_species, $spec;
			        }
			    }
			    else
			    {
			        push @$product_species, $spec;
			    }
			}

            

			# Add any reactants with Fixed attribute to products list
			# to insure concentration does not change
			my $ri = 0;
			foreach my $rpatt ( @{$rr->Reactants} )
			{
				if ( $rpatt->Fixed )
				{
				    push @$product_species, $reactant_species[$ri];
				}
				++$ri;
			}
                

            # get rateLaw for this reaction
            # By default, use the same ratelaw as the rule
            my $rl = $rr->RateLaw;
            
            # If this rule has reactant tags and the ratelaw has type function,
            #  then we may need to create a local ratelaw . . .
            if ( %{$rr->RRefs}  and  $rr->RateLaw->Type eq 'Function' )
            {
                my $err = '';

                # get parameter corresponding to ratelaw function
                (my $rl_param) = $plist->lookup( $rr->RateLaw->Constants->[0] );
                
                unless ( $rl_param->Type eq 'Function' )
                {   die "Error in RxnRule->ApplyRule(): cannot find parameter for RateLaw!";   }
                               
                # get function object
                my $fcn = $rl_param->Ref;                     
                

                # check for local dependency
                if ( $fcn->checkLocalDependency($plist) )
                {
                    # need to create a new ratelaw with locally evaluated observables
                    
                    # get local values for the function
                    my @local_refs = ($fcn->Name);
                    for ( my $ii=0; $ii < @{$fcn->Args}; ++$ii )
                    {   
                        push @local_refs, $reactant_species[ $rr->RRefs->{$fcn->Args->[$ii]} ]->Index;
                    }                   
                
                    # get local function
                    (my $local_fcn) = $fcn->evaluate_local( \@local_refs, $plist );

                    # add local_fcn to the parameter list
                    #  (so we can lookup the local function in the future!)
                    $plist->set( $local_fcn->Name, $local_fcn->Expr, 1, 'Function', $local_fcn );                  
               
                    my $lf_name = $local_fcn->Name;
                    (my $lf_param) = $plist->lookup($lf_name); 
                    
                    # replace fcn name with new local funct name
                    $local_refs[0] = $local_fcn->Name;
                    
                    # create param for local function
                    $rl = RateLaw->new();
                    $rl->Type('Function');
                    $rl->Constants( [@local_refs] );
                    $rl->Factor( $rr->RateLaw->Factor );
                
                    # increment number of ratelaws (in the RateLaw class)
                    ++$RateLaw::n_Ratelaw;
                }
            }


			# Create reaction
			my $rxn = Rxn->new();
			$rxn->Reactants( [@reactant_species] );
			$rxn->Products(  $product_species );
			$rxn->RateLaw( $rl );
			$rxn->Priority( $rr->Priority );
			$rxn->RxnRule( $rr );
            $rxn->StatFactor( $rr->MultScale );
			$rxn->Compartments( $compartments );


			# Add reaction to reaction list
			if ($dlist) { $dlist->add( $rxn, 1, $plist ); }
			else
			{   # NOTE: is there a way to warn about reactions that are ommitted
			    #  due to priority?  --Justin                                  
				$n_new_rxns += $rlist->add($rxn, 0, $plist );				
			}
            
		} # end MATCH loop


		# Ascend if not at top
		if ( $pattDepth > 0 )
		{
			--$pattDepth;
			return;
		}

		# Delete reactions if this rule has RateLaw->Type = Zero
		if ($dlist)
		{
			foreach my $rxn ( @{$dlist->Array} )
			{
				$n_new_rxns += $rlist->add($rxn, 0, $plist);
			}
		}

		$pattDepth = -1;
		return ($n_new_rxns);
	}
}



###
###
###



# THIS MAY BE DEPRICATED under the new canonical rule labeling scheme
sub find_reaction_center
# err = $rr->find_reaction_center( \%mapr )
#
# Finds the reaction center of each reactant pattern in a reaction rule.
# Results are stored at the array "RxnRule->ReactionCenter", which has
# length #Patterns.  Element [i] of the array is a list
# of reaction center elements for pattern[i].  Rxn_center elements are indexed
# using Jim's pointer notation "iPatt.iMol.iComp"  (Mac ripoff)
#
# Rxn_center includes:
#   (1) all components whose state, compartment or bond is changed by the rule.
#   (2) molecules whose compartment is changed by the rule.
#   (3) molecules that are deleted by the rule.
#   (4) species whose compartment is changed (explicitly).
#   (5) species that are deleted by the rule.
#
# This is subject to change!  --justin
#
# This could be done more efficiently in coordination with rule operation construction.
{
	my ( $rr, $mapr ) = @_;    # get RxnRule and reverse map (product->reactant)
	my $err = '';

	# initialize create a hash dictionary for each reactant pattern
	foreach my $iR ( 0 .. $#{$rr->Reactants} )
	{
		$rr->ReactionCenter->[$iR] = {};
	}

	foreach my $action ( @{$rr->EdgeAdd} )
	{   # edge additions are referenced to product space
        # components where edge added are reaction center
		my ( $targ1_pointer, $targ2_pointer ) = @$action;

		# map product pointers to reactant pointers
		$targ1_pointer = $mapr->{$targ1_pointer};
		$targ2_pointer = $mapr->{$targ2_pointer};

		unless ( $targ1_pointer eq -1 ) {
			my ($iPatt1) = ( $targ1_pointer =~ /^(\d+)/ );
			$rr->ReactionCenter->[$iPatt1]->{$targ1_pointer} = 1;
		}

		unless ( $targ2_pointer eq -1 ) {
			my ($iPatt2) = ( $targ2_pointer =~ /^(\d+)/ );
			$rr->ReactionCenter->[$iPatt2]->{$targ2_pointer} = 1;
		}
	}

	foreach my $action ( @{ $rr->EdgeDel } )
	{   # edge deletions are referenced to reactant space
		# components where edge deleted are reaction center
		my ( $targ1_pointer, $targ2_pointer ) = @$action;
		my ($iPatt1) = ( $targ1_pointer =~ /^(\d+)/ );
		$rr->ReactionCenter->[$iPatt1]->{$targ1_pointer} = 1;

		my ($iPatt2) = ( $targ2_pointer =~ /^(\d+)/ );
		$rr->ReactionCenter->[$iPatt2]->{$targ2_pointer} = 1;
	}

	foreach my $targ_pointer ( @{ $rr->MolDel } )
	{   # molecule deletions are referenced to reactant space
		my ($iPatt) = ( $targ_pointer =~ /^(\d+)/ );
		$rr->ReactionCenter->[$iPatt]->{$targ_pointer} = 1;

        # NOTE1: a Species delete rule will add Species as "Rxn Center"
        # NOTE2: edges implicitly deleted by molecule delete are not reaction center.
	}

	foreach my $action ( @{ $rr->CompStateChange } )
	{       # components with state changes are rxn center
		    # component changes are referenced to reactant space
		my ($targ_pointer) = @$action;
		my ($iPatt) = ( $targ_pointer =~ /^(\d+)/ );
		$rr->ReactionCenter->[$iPatt]->{$targ_pointer} = 1;
	}

	foreach my $action ( @{ $rr->ChangeCompartment } )
	{       # molecules with compartment changes are rxn center
		    # compartment changes are referenced to reactant space
		my ($targ_pointer) = @$action;
		my ($iPatt) = ( $targ_pointer =~ /^(\d+)/ );
		$rr->ReactionCenter->[$iPatt]->{$targ_pointer} = 1;

		# NOTE: a Species transport will add Species as "Rxn Center"
	}

	# rewrite each hash dictionary as an array
	foreach my $iR ( 0 .. $#{ $rr->ReactionCenter } ) {
		$rr->ReactionCenter->[$iR] = [ keys %{ $rr->ReactionCenter->[$iR] } ];
	}

	# debug
	#print "## Reaction Center: ", $rr->toString(), "\n";
	#foreach my $iR ( 0..$#{$rr->ReactionCenter} )
	#{
	#   print "# $iR\n";
	#   foreach my $item ( @{$rr->ReactionCenter->[$iR]} )
	#   {
	#      print "$item\n";
	#   }
	#}

	return $err;
}


###
###
###


# THIS IS DEPRICATED under the new canonical rule labeling scheme
#  .. ACTUALLY .. may be useful for efficiency?
sub filter_identical_by_rxn_center
# err = $rr->filter_identical_by_rxn_center( \@matches, $iPatt )
#
# Let @matches be a set of matches from Reactant[$iPatt] to a single target
# species.  This routine removes extra matches which are identical up to
# the rxn center.  --justin
#
# specifically, let f[i] be the map from reactant pattern into
# a species induced by match[i].  Let f[i]|rxn_center be the
# restriction of f[i] to the subset of nodes in the rxn_center
# of the reactant pattern.
#
# If match[i]->Target == match[j]->Target
#  and
# f[i]|rxn_center == f[j]|rxn_center
#
# then the match[j] is the same up to the reaction center as match[i]
# and we can, wlog, filter match[j].
{
	my ( $rr, $match_set, $iPatt ) = @_;
	my $err = '';

	# save time if there's only one match
	return $err unless ( @$match_set > 1 );

	# array to store images of reaction center under match maps.
	my @images = ();

	# debug
	#print "RULE >> ", $rr->toString(), "\n";
	#print "Src  >> ", $match_set->[0]->Source->toString(), "\n";
	#print "Targ >> ", $match_set->[0]->Target->toString(), "\n";
	#print "initial match set: ", scalar(@$match_set), "\n";

	# (1) build image of reaction center under each match map.
	foreach my $match (@$match_set)
    {
		my $image = [];
		push @images, $image;

		# generate image by mapping reaction center nodes into the match Target
		foreach my $node_pointer ( @{ $rr->ReactionCenter->[$iPatt] } )
        {
			my ( $iP, $iMC ) = ( $node_pointer =~ /(\d+)\.?(.*)/ );

			#print " # $node_pointer : $iP . $iMC \n";
			#print " #  -> ", $match->MapF->{$iMC}, "\n";

			if ( $iMC ne '' ) {    # molecule or component node
				push @$image, "${iPatt}." . $match->MapF->{$iMC};
			}
			else {                 # species node
				push @$image, "${iPatt}";
			}
		}
	}

    # debug
    #print "IMAGES >>\n";
    #for ( my $iMatch = 0;  $iMatch < @images;  $iMatch++ )
    #{  # print out the mapping from reaction center to matching species
    #   print join( ', ', @{$rr->ReactionCenter->[$iPatt]}), " -> ", join( ', ', @{$images[$iMatch]} ), "\n";
    #}

	# (2) Compare images and filter identical.
	# All against all comparison:  N*(N-1) compares, where N=#matches.
	# Some efficiency gained by tossing repeats as found.
	for ( my $iMatch = 0 ; $iMatch < @images ; $iMatch++ )
	{   # each match gets a turn as the template
		my $template = $images[$iMatch];

	  JMATCH:

		# compare template to all images with higher index
		for ( my $jMatch = $iMatch + 1 ; $jMatch < @images ; $jMatch++ )
		{    # get image for comparison
			my $image = $images[$jMatch];

			# compare element by element
			for ( my $k = 0 ; $k < @{$template} ; $k++ ) {
				next JMATCH unless ( $template->[$k] eq $image->[$k] );
			}

			# images are the same.  toss jMatch from @$match_set and @images;
			splice( @$match_set, $jMatch, 1 );
			splice( @images,     $jMatch, 1 );

			# decrement jMatch to keep things in sync
			$jMatch--;
		}
	}

	# debug
	#print "final match set: ", scalar(@$match_set), "\n\n";

	# all done.  yay!
	return $err;
}


##
##


sub setRefs
{
	my $refs  = shift;
	my $id    = shift;
	my $plist = shift;

	# what is $id???  -justin
	for my $arg ( keys %{$refs} ) {
		my $oid = pointer_to_ID( $id . "_R", $refs->{$arg} );
		$plist->set( $arg, $oid, 1, "Local" );
	}
	return '';
}


##
##


sub unsetRefs
{
	my $refs  = shift;    # added by justin..  why not here?
	my $plist = shift;

	foreach my $arg ( keys %{$refs} ) { $plist->deleteLocal($arg); }
	return '';
}


###
###
###


sub apply_operations

# ($products, $label) = $rr->apply_operations(\@matches,\@permute)
#
# Apply rule operations to the reactant set in @matches and return
# products. Also generates an "almost" canonical labeling for the reaction.
# NOTE: canonical labeling isn't fully general w.r.t. operations on newly created molecules.
#
# @matches is an array with elements that are maps from the nth pattern to the nth reactant
# @permute is an array that defines a permutation of the reactants
{

	#print STDERR "enter apply ops :";
	my $rr      = shift;
	my $matches = shift;
	my $permute = @_ ? shift : undef;

    # get number of reactants
    my $n_reactants = @$matches;

    # get targets
    my $targets = [ (map {$_->Target} @$matches) ];

	# Aggregate matching SpeciesGraphs
	my ($g, $offsets) = SpeciesGraph::copymerge(@$targets);
    my $mol = $g->Molecules;

    # Build map from product patterns into aggregate product graph
    #   mapP: product pattern (p.m.c) -> product species (m.c)        
    my $mapP = {};
    for ( my $ir = 0; $ir < $n_reactants; ++$ir )
    {
        # get match to this reactant
        my $match = $matches->[$ir];
        
	    # Map molecules
		for ( my $im = 0;  $im < @{$match->Source->Molecules}; ++$im )
		{
		    # nothing to do if molecule ain't in the map
			next unless ( exists $rr->MapF->{"$ir.$im"} );
			
			# map molecule in reactant pattern to product pattern
			my $pP = $rr->MapF->{"$ir.$im"};
			
			# nothing to do if maps to -1 (i.e. "delete")
			next if ( $pP == -1 );
			
	        # map molecule in product pattern to aggregate product graph
		    $mapP->{$pP} = $match->mapF( "$im", $offsets->[$ir] );

            # Map components
    	    for ( my $ic = 0;  $ic < @{$match->Source->Molecules->[$im]->Components};  ++$ic )
		    {
			    # map component in reactant pattern to product pattern
			    my $pP = $rr->MapF->{"$ir.$im.$ic"};
			    # map molecule in product pattern to aggregate product graph
			    $mapP->{$pP} = $match->mapF( "$im.$ic", $offsets->[$ir] );	
			}
		}        
    }



	# Canonical Labeling, part 1:
	#  order reactants by canonical label,
	#   remap offsets to the canonical order.
	my $label                    = '';
	my $canonical_reactant_order = [];
	{
		my $stack = [];

		# get canonical labels (paired with reactant indices)
		my $iR = 0;
		foreach my $targ (@$targets)
		{
			push @$stack, [ $targ->StringExact, $iR ];
			$iR++;
		}

		# sort by label
		@$stack = ( sort { $a->[0] cmp $b->[0] } @$stack );

		# build map from original reactant order to canonical reactant order.
		#   map is needed for later steps.
		$iR = 0;
		foreach my $targ (@$stack) {
			$canonical_reactant_order->[ $targ->[1] ] = $iR;
			$iR++;

			# toss out original index
			$targ = $targ->[0];
		}

		# concatenate labels
		$label .= join( '//', @$stack );
	}


	# Apply molecule additions
	{
		my $stack = [];

		foreach my $molAddRef ( @{$rr->MolAdd} )
		{
			#print STDERR "molAdd\n";
			my @compInhRefs = @$molAddRef;
			my $molRef      = shift @compInhRefs;
			my ( $ip, $im ) = split /\./, $molRef;
			my $newMol = $rr->Products->[$ip]->Molecules->[$im]->copy($molRef);
			
			# Clear out edges to avoid label clashing!! We'll add bonds later.
			@{$newMol->Edges} = ();
			foreach my $comp ( @{$newMol->Components} )
			{
			    @{$comp->Edges} = ();
			}

			# Inherit component atributes
			my $ic_newMol = 0;
			foreach my $compInhRef (@compInhRefs)
			{
				next unless $compInhRef;
				my ( $ip, $im, $ic ) = split /\./, $compInhRef;

				# permute reactants
				if ( defined $permute ) { $ip = $permute->[$ip]; }

				my ( $im_targ, $ic_targ ) = split /\./,
				  $matches->[$ip]->mapF( "$im.$ic", $offsets->[$ip] );

				# Copy component state
				my $stateNew = $mol->[$im_targ]->Components->[$ic_targ]->State;
				$newMol->Components->[$ic_newMol]->State($stateNew);
				++$ic_newMol;
			}

			# Remove labels and set map elements
			$newMol->Label(undef);
			my $nmol = @{$g->Molecules};
			$mapP->{$molRef} = $nmol;
			$ic_newMol = 0;
			foreach my $comp ( @{ $newMol->Components } )
			{
				$comp->Label(undef);
				$mapP->{"$molRef.$ic_newMol"} = "$nmol.$ic_newMol";
				++$ic_newMol;
			}

			push @{$g->Molecules}, $newMol;

			# printf "prod: %s\n", $g->toString();
			# Should add pointer from product pattern to new molecules in $g

			# save molecule addition for canonical labeling
			# TODO: get real canonical label for molecule!!
			push @$stack, $newMol->toString;
		}

		# add molecule addition operations to the canonical label
		if (@$stack)
		{
			@$stack = ( sort { $a cmp $b } @$stack );
			$label .= '|MA/' . join( '/', @$stack );
		}
	}


	# Apply component state changes
	{
		my $stack = [];
		foreach my $sc ( @{$rr->CompStateChange} )
		{
			my ( $ref, $stateR, $stateP ) = @$sc;
			my ( $ip, $im, $ic ) = split /\./, $ref;

			# permute reactants
			if ( defined $permute ) { $ip = $permute->[$ip]; }

			my ( $im_targ, $ic_targ ) =
			        split /\./, $matches->[$ip]->mapF( "$im.$ic", $offsets->[$ip] );
				    
		    $mol->[$im_targ]->Components->[$ic_targ]->State($stateP);

			# save compoent state change for canonical ordering
			my $canonical_targ = $canonical_reactant_order->[$ip] . '.' . $matches->[$ip]->mapF("$im.$ic");
			push @$stack, $canonical_targ . ',' . $stateP;
		}

		if (@$stack) {
			@$stack = ( sort { $a cmp $b } @$stack );
			$label .= '|SC/' . join( '/', @$stack );
		}
	}


	# Apply compartment changes
	{
		my $stack = [];

		foreach my $comp_change ( @{$rr->ChangeCompartment} )
		{
			# parse state change array
			my ( $ref, $compartR, $compartP, $cytosis ) = @$comp_change;

			# parse pattern.molecule indexing
			my ( $ip, $im, $ic ) = ( split /\./, $ref );

			# permute reactants
			if ( defined $permute ) { $ip = $permute->[$ip]; }

		    # debug:
		    #print STDERR "P: $ip  M: $im  C: $ic  cytosis: $cytosis\n";
		    #print STDERR "Origin Compartment: ", $compartR->Name, "\n";
		    #print STDERR "Dest Compartment: ", $compartP->Name, "\n";

	        # note:$matches($i_patt) is the map from pattern (with index $idx_pattern)
	        # to instances in a species graph
			if ( defined($ic)  and  ($ic ne '') )
			{   # component compartment chanfge
				# implementation of component compartments is not completed (here and elsewhere)
				#print STDERR ">> Component Transport <<\n";
				my ( $im_targ, $ic_targ ) =
				        split /\./, $matches->[$ip]->mapF( "$im.$ic", $offsets->[$ip] );
				
				$mol->[$im_targ]->Components->[$ic_targ]->Compartment($compartP);

				# save compartment change for canonical ordering
				push @$stack, $canonical_reactant_order->[$ip] . '.'
				              . $matches->[$ip]->mapF("$im.$ic") . ','
				              . $compartP->Name;
			}
			elsif ( defined($im)  and  ($im ne '') )
			{   # molecule compartment change
				if ( $rr->MoveConnected )
				{   # move connected compartment component of $i_mol
					# excluding molecules named in reactant pattern.

					# (1) map pattern molecule index to target index
					my ($im_targ) = $matches->[$ip]->mapF( "$im", $offsets->[$ip] );

		            # (2) exclude molecules named in reactant pattern (aside from $i_mol)
					my @exclude_molecules = ();
					for ( my $jm = 0;  $jm < @{ $rr->Reactants->[$ip]->Molecules };  $jm++ )
					{
						# skip $i_mol
						next if ( $jm == $im );

						# map $j_mol into target
						my $jm_targ = $matches->[$ip]->mapF( "$jm", $offsets->[$ip] );

						# nothing to do if this molecule is deleted
						next if ( $jm_targ < 0 );

						# otherwise, add $j_molT to exclude molecules
						push @exclude_molecules, $jm_targ;
					}

					# (3) find connected compartment component of molecule
					my @compartment_connected = $matches->[$ip]->Target->find_compartment_connected($im_targ);

					# (4) transport molecules
					foreach my $jm_targ (@compartment_connected)
					{
						# skip if $j_molT is excluded
						next if ( grep { $_ == $jm_targ } @exclude_molecules );

						# otherwise transport molecule
						$mol->[$jm_targ]->Compartment($compartP);
					}

					# save compartment change for canonical ordering
					push @$stack, $canonical_reactant_order->[$ip] . '.'
					              . $matches->[$ip]->mapF($im) . ','
					              . $compartP->Name . ',' . 'mc';
				}
				else
				{   # move single molecule
					my ($im_targ) = $matches->[$ip]->mapF( "$im", $offsets->[$ip] );
					
					$mol->[$im_targ]->Compartment($compartP);

					# save compartment change for canonical ordering
					push @$stack, $canonical_reactant_order->[$ip] . '.'
					              . $matches->[$ip]->mapF($im) . ','
					              . $compartP->Name;
				}
			}
			else
			{   # species compartment change
			    #print STDERR ">> Species Transport <<\n";

				# save compartment change for canonical ordering
				push @$stack, $canonical_reactant_order->[$ip] . ',' . $compartP->Name;

				if ( $cytosis == 0 )
				{   # volume to volume transport
					# transport all molecules to $compartP
					for (  my $im = $offsets->[$ip];
						   $im < $offsets->[$ip] + @{ $matches->[$ip]->Target->Molecules };
						   $im++
					    )
					{
						$g->Molecules->[$im]->Compartment($compartP);
					}
				}
				else
				{   # surface to surface transport
				    # if $cytosis= 1  [endocytosis]: map Outside to Inside, Inside to Outside
					# if $cytosis=-1  [exocytosis]:  map Outside to Outside, Inside to Inside

					my $outsideR = $compartR->Outside;
					my $insideR  = $compartR->Inside->[0];    # should be only one compartment here

					my $outsideP = $compartP->Outside;
					my $insideP  = $compartP->Inside->[0];

					if ( $cytosis == 1 )
					{
						( $insideP, $outsideP ) = ( $outsideP, $insideP );
					}

					for (  my $im = $offsets->[$ip];
						   $im < $offsets->[$ip] + @{ $matches->[$ip]->Target->Molecules };
						   $im++
					    )
					{
						if ( $g->Molecules->[$im]->Compartment == $compartR )
						{
						 	 $g->Molecules->[$im]->Compartment($compartP);
						}

						elsif ( $g->Molecules->[$im]->Compartment == $outsideR )
						{
							$g->Molecules->[$im]->Compartment($outsideP);
						}

						elsif ( $g->Molecules->[$im]->Compartment == $insideR )
						{
							$g->Molecules->[$im]->Compartment($insideP);
						}

						else
						{
							exit_error( "surface to surface species transport attempted to move molecule"
								        . "that is not on or adjacent to the reaction surface.",
								        $rr->toString
							          );
						}
					}
				}    # end species transport
			}    # end compartment change
		}    # end compartment loop

		# add compartment change operations to the canonical label
		if (@$stack)
		{
			@$stack = ( sort { $a cmp $b } @$stack );
			$label .= '|CC/' . join( '/', @$stack );
		}
	}


	# Apply edge deletions
	{
		my $stack = [];

		foreach my $edel ( @{$rr->EdgeDel} )
		{
			my @targs           = ();
			my @targs_canonical = ();

			# loop over components connected to the edge
			foreach my $ref ( @$edel )
			{
				my ( $ip, $im, $ic ) = split /\./, $ref;

				# permute reactants
				if ( defined $permute ) { $ip = $permute->[$ip]; }

                # Find pointer to molecule and component in $g corresponding to $im.$ic in pattern
				push @targs,
				  $matches->[$ip]->mapF( "$im.$ic", $offsets->[$ip] );

				# get pointer in canonical form
				my $ref_canonical = $canonical_reactant_order->[$ip] . '.'
				                    . $matches->[$ip]->mapF("$im.$ic");
				push @targs_canonical, $ref_canonical;
			}

			#print "Deleting edge $comps[0] $comps[1]\n";
			$g->deleteEdge(@targs);

			# sort components for canonical ordering
			@targs_canonical = ( sort cmp_pointer @targs_canonical );
			push @$stack, join( ',', @targs_canonical );
		}

		# add edge deletion operations to the canonical label
		if (@$stack)
		{
			@$stack = ( sort cmp_edge @$stack );
			$label .= '|ED/' . join( '/', @$stack );
		}
	}


	# Apply edge additions
	{
		my $stack = [];

		my $nedge = 1;
		foreach my $eadd ( @{$rr->EdgeAdd} )
		{
			# get pointers to components in the super-graph
			my $g_ref1 = $mapP->{ $eadd->[0] };
			my $g_ref2 = $mapP->{ $eadd->[1] };

            # need to map edge adds from super-graph back to reactant patterns, if possible
            # (need to do this since reactants are rearranged.. hence its not sufficient to just
            #    use the pointers into the supergraph)
			my $ref1 = undef;
			my $ip1  = 0;
		  EDGE_ADD_LOOP1:
			foreach my $map (@$matches)
			{
				foreach my $ptr ( keys %{$map->MapF} )
				{
					if ( $map->mapF( $ptr, $offsets->[$ip1] ) eq $g_ref1 )
					{
						$ref1 = $canonical_reactant_order->[ defined $permute
						                                     ? $permute->[$ip1]
						                                     : $ip1             ] .'.'. $map->mapF($ptr);		         
						last EDGE_ADD_LOOP1;
					}
				}
				$ip1++;
			}
			unless ( defined $ref1 ) {  $ref1 = $g_ref1;  }

			# now for the other end of the edge...
			my $ref2 = undef;
			my $ip2  = 0;
		  EDGE_ADD_LOOP2:
			foreach my $map (@$matches)
			{
				foreach my $ptr ( keys %{$map->MapF} )
				{
					if ( $map->mapF( $ptr, $offsets->[$ip2] ) eq $g_ref2 )
					{
						$ref2 = $canonical_reactant_order->[ defined $permute
						                                     ? $permute->[$ip2]
						                                     : $ip2             ] .'.'. $map->mapF($ptr);
						last EDGE_ADD_LOOP2;
					}
				}
				$ip2++;
			}
			unless ( defined $ref2 ) {  $ref2 = $g_ref2;  }

			#print "EDGE: $ref1 $ref2\n"

			# APPLY edge addition now
			#printf "Adding edge p1=$p1(%s) p2=$p2(%s)\n", $refs[0],$refs[1];
			$g->addEdge( "ne${nedge}", $g_ref1, $g_ref2 );
			++$nedge;

			# TODO: edges added to new molecules may not be in canonical form!
			# save edge addition for canonical labeling
			push @$stack, join( ',', sort cmp_pointer ( $ref1, $ref2 ) );
		}

		# add edge addition operations to the canonical label
		if (@$stack)
		{
			@$stack = ( sort cmp_edge @$stack );
			$label .= '|EA/' . join( '/', @$stack );
		}
	}


    my $temp = 0;
	# Apply molecule/pattern deletions
	# NOTE: do this last because molecule indices will change after deletion.
    my $del_list = [];   # list of molecules to delete (by index) in the aggregate product
	{
		my $stack = [];

		foreach my $molref ( @{$rr->MolDel} )
		{
			#print STDERR "molDel\n";
			my ( $ip, $im ) = split /\./, $molref;

			# permute reactants
			if ( defined $permute ) {  $ip = $permute->[$ip];  }

			#print STDERR "ip: $ip  im: $im\n";
			if ( defined $im )
			{
				# Delete molecule
				my $im_targ = $matches->[$ip]->mapF( "$im", $offsets->[$ip] );

                #print "Deleting molecule ", $g->Molecules->[$im_targ]->toString(),"\n";
				$g->Molecules->[$im_targ] = undef;
				push @$del_list, $im_targ;

				# save molecule delete for canonical labeling
				push @$stack, $canonical_reactant_order->[$ip] . '.'
				    . $matches->[$ip]->mapF($im);
				    
				# update edges
			}
			else
			{
				#print STDERR "delete whole pattern\n";
				# Delete whole pattern
				my $im_targ_max =
				    $offsets->[$ip] + $#{ $matches->[$ip]->Target->Molecules };
				foreach my $im_targ ( $offsets->[$ip] .. $im_targ_max )
				{
                    #print STDERR "mol_delete: $im_targ\n";
                    #print "Deleting molecule ", $g->Molecules->[$im_targ]->toString(),"\n";
					$g->Molecules->[$im_targ] = undef;
					push @$del_list, $im_targ;
				}

				# save pattern delete for canonical labeling
				push @$stack, $canonical_reactant_order->[$ip];
            }
        }

		# compress product graph (eliminate undefined molecules!)
		my $im = 0;
		while ( $im < @{$g->Molecules} )
		{
			unless ( defined $g->Molecules->[$im] )
			{
			    $temp = 1;
				splice @{$g->Molecules}, $im, 1;
				next;
			}
			$im++;
		}

		# add molecule deletion operations to the canonical label
		if (@$stack)
		{
			@$stack = ( sort { $a cmp $b } @$stack );
			$label .= '|MD/' . join( '/', @$stack );
		}
	}
	# done applying changes to speciesGraph	


    # Remove dangling edges and rebuild Adjacency hash and Edge array
    my $dangling_error = 0;
    my $trim_dangling  = 1;
	$g->updateEdges($dangling_error, $trim_dangling);


	# Build map of molecules in supergraph to the product patterns they should belong to
	#   mapP_reverse: product molecules (m) -> product pattern (p)
			
    # initalize map with an array of zeros (length = number of product molecules plus deleted molecules)
	my $mapP_reverse = [ (undef) x (@{$g->Molecules} + @$del_list) ];
    # build map by inverting %mapP for all molecule values. recall thaat mapP: p.m.c -> m.c
    while ( my ($pmc, $mc) = each %$mapP )
    {
        # skip component pointers
        next if ( $mc =~ /\./ );
        # get the pattern index
        (my $p) = ($pmc =~ /^(\d+)\./);
        # point molecule in supergraph to a product pattern
        $mapP_reverse->[$mc] = $p;
    }
	# splice deleted molecules from mapP_reverse
	foreach my $imol (sort {$b <=> $a} @$del_list)
	{   # do this in reverse order so remaining delete indices remain valid
	    splice @$mapP_reverse, $imol, 1;
	}
    # done builing mapP_reverse


	# Split aggregate graph into connected subgraphs using
	#   mapP_reverse as a guide to matching the products with patterns.
	# On return product graphs will have the same order as the patterns they
    #   correspond to.  Any extra component will be places at the end of the array.
    # (this method will not create labels!)
    my ($err, $products) = $g->splitConnectedComponents($mapP_reverse);
    if ($err) {  return (undef, undef);  }

    
	# return product graph
	return ( $products, $label );
}



###
###
###



sub cmp_pointer
# compare two pointers
{
    # split pointers into elements
	my @a = split /\./, $a;
	my @b = split /\./, $b;

    # set $cmp=0, i.e. no difference found yet
	my $cmp = 0;
	# loop until break
	while (1)
	{
	    # if no elements in pointer A, set cmp=0 if pointer B is empty and -1 otherwise, then break.    	
		unless (@a) {  $cmp = @b ? -1 : 0;  last;  }
		# if no elements in pointer B, set cmp=1
		unless (@b) {  $cmp = 1;  last;            }
        # shift pointer elements and compare
		$cmp = (shift @a <=> shift @b);
        # break if we found a difference in these elements		
		last if $cmp;
	}
    # return comparison
	return $cmp;
}



###
###
###



sub cmp_edge
# compare two edges, where an edge has format:  "p1.m1.c1,p2.m2.c2"
#  note, edge pointers must be sorted prior to comparison!
{
    # split edges into pointer arrays
	my @a = split /,/, $a;
	my @b = split /,/, $b;
    
    # initialize $cmp=0, i.e. no difference found yet
	my $cmp = 0;
    # loop until break
	while (1)
	{	
	    # if no pointers in edge A:  set cmp=0 if edge B is empty and -1 otherwise, then break.
	    unless (@a) {  $cmp = @b ? -1 : 0;  last;  }
	    # if no pointers in edge B: set cmp=1, then break.
	    unless (@b) {  $cmp = 1;  last;            }
	
	    # shift pointers and split into elements
	    my @pa = split /\./, shift @a;
	    my @pb = split /\./, shift @b;
	    
	    # loop until break
	    while (1)
	    {	
	        # if no elements in pointer A, set cmp=0 if pointer B is empty and -1 otherwise, then break.    
		    unless (@pa) {  $cmp = @pb ? -1 : 0;  last;  }
		    # if no elements in pointer B, set cmp=1
		    unless (@pb) {  $cmp = 1;  last;             }
		    # shift pointer elements and compare
		    $cmp = (shift @pa <=> shift @pb);
		    # break if we found a difference in these elements
		    last if $cmp;
	    }
		
		# break if we found a difference in these pointers
		last if $cmp;
	}

    # return comparison
	return $cmp;
}



###
###
###

1;
