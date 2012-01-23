# $Id: RxnRule.pm,v 1.20 2007/07/06 06:09:48 faeder Exp $

package RxnRule;

use strict;
use warnings;


###
###
###


# parse population mapping string and return a RxnRule object
sub newPopulationMappingRule
{
    # get input arguments
	my $string = shift;
	my $model  = shift;

	# define return arguments
	my $err = undef;
	my $rr  = undef;
	
	# Remove leading whitespace
	$string =~ s/^\s*//;

	my $name = '';
    # check for rule identifier or index
	if ( $string =~ s/^([\w\s*]+):\s*// )
	{   # if the first token is alphanumeric and followed by a colon,
	    #  then its the name of the rule
		$name = $1;
	}
	elsif ( $string =~ s/^(\d+)\s+// )
	{   # if the first token is a number, this is the rule index
		$name = $1;
	}


	# some local variables
	my ( $ipatt, $g, $sep, $reversible );

    # data structures for reactants, products and references
    my $reac  = [];
    my $prod  = [];
    my $rrefs = {};
    my $prefs = {};
    # temporary structure to map references from reactants to products
	my %labels = ();


	# Read reactant patterns
	$sep = '^\s*[+]\s*|^\s*([<]?-[>])\s*';    #  "+"  or "->"  or "<->"
	$ipatt = -1;
	while ($string)
	{
		$g = SpeciesGraph->new();
		# use the MoleculeTypes list for reactant validation
		$err = $g->readString( \$string, $model->CompartmentList, $SpeciesGraph::IS_SPECIES,
		                         $sep, $model->MoleculeTypesList );
		if ($err) {  return ($err, $rr);  }
		++$ipatt;

        # Check Labels for this Reactant:
		if ( defined $g->Label  and  $g->Label ne '' )
		{
            $rrefs->{ $g->Label } = $ipatt;
        }

		# Checking Molecules...
		my $imol = -1;
		foreach my $mol ( @{ $g->Molecules } )
		{
			++$imol;
			my $label = $mol->Label;
			if ( defined $label  and  $label ne '' )
			{
				if ( $labels{$label} )
				{
					return ("Repeated label $label in reactants of reactant rule", $rr);
				}
				$labels{$label} = 'M';
				$rrefs->{$label} = join '.', ($ipatt, $imol);
			}
			my $icomp = -1;
				
			# Checking Components...
			foreach my $comp ( @{ $mol->Components } )
			{
				++$icomp;
				$label = $comp->Label;
				if ( defined $label  and  $label ne '' )
				{				
					if ( $labels{$label} )
					{
						return ("Repeated label $label in reactants of reaction rule", $rr);
					}
					$labels{$label} = 'C';
					$rrefs->{$label} = join '.', ($ipatt, $imol, $icomp);
				}
			}
		}
			
		# Save this reactant
		push @$reac, $g;
			
		# Find next separator (some problem with string if there is no separator)
		unless ( $string =~ s/$sep// )
		{
			return ("Invalid syntax in reaction rule: $string", $rr);
		}
			
		# If the arrow operator was the next separator, then it's captured in $1
		if ($1)
		{   # Check if this reaction is reversible
			$reversible = ( $1 eq "<->" ) ? 1 : 0;
            if ( $reversible )
            {   return ("Population mapping rule should not be reversible!", $rr);   }
		    last;
	    }
	}
    # only 1 reactant pattern allowed in population mapping rule
    unless (@$reac == 1)
    {   return ("Incorrect number of reactants in population mapping rule!", $rr);   }



    # Read Product Patterns
	$sep   = '^\s*([+])\s*|^\s+';    # "+" or "  "
	$ipatt = -1;
	while ($string)
	{
		my $g = SpeciesGraph->new();	
		# Validate product graphs using population types
		$err = $g->readString( \$string, $model->CompartmentList, $SpeciesGraph::IS_SPECIES,
		                       $sep, $model->PopulationTypesList );
		if ($err) {  return ($err, $rr);  }
		++$ipatt;


        # Check that pattern is a single molecule
        unless ( @{$g->Molecules} == 1 )
        {
            return ("Incorrect number of molecules in product of population mapping rule!", $rr);
        }
        # Check that population type does not collide with anything in the molecule type list
        if ( defined $model->MoleculeTypesList->findMoleculeType($g->Molecules->[0]->Name) )
        {
            return ("Population product type collides with a previously defined molecule type!", $rr);
        }
        # Check that the molecule does not have components
        unless ( @{$g->Molecules->[0]->Components} == 0 )
        {
            return ("Components are not permitted in product of population mapping rule!", $rr);
        }


        # Checking Labels for this Species..
		if ( defined $g->Label  and  $g->Label ne '' )
		{
			$prefs->{ $g->Label } = $ipatt;
		}

		# Checking Molecules... (verify correspondence and uniqueness!)
		my $imol = -1;
		foreach my $mol ( @{$g->Molecules} )
		{
			++$imol;
			my $label = $mol->Label;
			if ( defined $label  and  $label ne '' )
			{
				if ( $labels{$label} eq 'M' )
				{
					$labels{$label} = 'found';
				}
				elsif ( $labels{$label} eq 'found' )
				{
					return ("Repeated label $label in products of reaction rule $name", $rr);
				}
				else
				{
					return ("Mis- or un-matched label $label in products of reaction rule $name", $rr);
				}
				$prefs->{$label} = join '.', ($ipatt, $imol);
			}
				
			# Checking Components...
			my $icomp = -1;
			foreach my $comp ( @{$mol->Components} )
			{
				++$icomp;
				$label = $comp->Label;

				#print "label=$label\n";
				if ( defined $label  and  $label ne '' )
				{
					if ( $labels{$label} eq 'C' )
					{
						$labels{$label} = 'found';
					}
					elsif ( $labels{$label} eq 'found' )
					{
						return ("Repeated label $label in products of reaction rule $name", $rr);
					}
					else
					{
						return ("Mis- or un-matched label $label in products of reaction rule $name", $rr);
					}
					$prefs->{$label} = join '.', ($ipatt, $imol, $icomp);
				}
			}
		}
			
		# Save this Product pattern
		push @$prod, $g;
			
		# Check for next separator (store a "+" separator in $1)
		$string =~ s/$sep//;
		
		# See if there's more Products to parse..
		last unless $1;
	}
    # only 1 product pattern allowed in population mapping rule
    unless (@$prod == 1)
    {   return ("Incorrect number of products in population mapping rule!", $rr);   }



	# Read rateLaw
	my $rl;
	
    # Look for TotalRate attribute
    # (default behavior is PerSite rate)
    my $TotalRate  = 0;
    if ( $string =~ s/(^|\s)TotalRate(\s|$)/$2/ )
    {   $TotalRate = 1;   }
 
    # temporarily add Reference tags as names in the ParamList
	if (%$rrefs) {  setRefs( $rrefs, '', $model->ParamList );  }
 
    # Parse and Create the ratelaw
    ( $rl, $err ) = RateLaw::newRateLaw( \$string, $model, $TotalRate, $reac );
	if ($err) {  return ($err, $rr);  }
 
    # unset temporary names of Reference tags
    if (%$rrefs) {  unsetRefs( $rrefs, $model->ParamList );  } 


	# Check for syntax error due to extra ratelaw tokens
    if ( $string =~ /^\s*[,]/ )
	{   return ("Unidirection reaction may have only one rate law", $rr);   }
		
    # String should be empty now, otherwise there may be a problem.
	if ( $string =~ /\S+/ )
	{   return ("Syntax error in reaction rule at $string", $rr);   }
        

        
    # Construct RxnRule
	$rr = RxnRule->new();
	if ($name) {  $rr->Name( $name );  }
	$rr->Reactants( $reac );
	$rr->Products(  $prod );
	$rr->Priority( 0 );
	$rr->RateLaw( $rl );
	$rr->Rexclude( [([])] );
	$rr->Pexclude( [([])] );
	$rr->Rinclude( [([])] );
	$rr->Pinclude( [([])] );
	$rr->TotalRate( $TotalRate );
	$rr->DeleteMolecules( 0 );
	$rr->MoveConnected( 0 );
	$rr->RRefs( $rrefs );
	$rr->PRefs( $prefs );

	# Find mapping of reactants onto products
	#  (pass population types list for handling of created populations
	if ( $err = $rr->findMap($model->PopulationTypesList) )
	{   return ($err, $rr);   }

    # Return normally
	return ($err, $rr);
}



###   /--\  /-\ /-- o \/-\ /-\
###   |--/  |-/ |-  | |  | |-/
###   |  \_ \__ |   | |  | \__



##-----------------##
## Rule refinement ##
##-----------------##
sub refineRule
{
    # get input arguments
	my $rr       = shift @_;      # this reaction rule
	my $sg_list  = shift @_;      # refine rules w.r.t. these species graphs
	my $model    = shift @_;      # model
	my $hybrid_model = shift @_;  # new hybrid model
    my $user_params = (@_) ? shift : {};
    

    # Specify default params
	my $params = {   verbose => 0,
	                 indent  => ' ' x 8
	             };
	# overwrite defaults with user params
	while ( my ($opt,$val) = each %$user_params )
	{   $params->{$opt} = $val;   }


    # define return values
    my $err = undef;
    my $refinements = [];
    my $refinement_map = {};

    
    # clear out the RxnLabels hash
    %{$rr->RxnLabels} = ();

    
    # remember number of reactants
    my $n_reactants = scalar @{$rr->Reactants};


    # Map Reactant Patterns into SpeciesGraphs in @$patterns
	my $ipatt = 0;
	foreach my $rpatt ( @{$rr->Reactants} )
	{
	    # make sure rmatches is empty!
		$rr->Rmatches->[$ipatt] = [];
        
        # check if reactant is isomorphic to anything in sg_list
        my $is_isomorph = 0;
        foreach my $sg (@$sg_list)
        {
            # NOTE: since $sg is a fully specified species, looking for a subgraph isomorphism in $rpatt
            #  is sufficient for establishing isomorphism. This is slower than the method 'isomorphicTo'
            #  but does not require sorting molecules (which is problematic for reactant patterns).
            if ( $sg->isomorphicToSubgraph($rpatt) )
            {
                $is_isomorph = 1;
                last;
            }
        }

        # if not, then create a copy and self-map and add it to the list of matches
        # (add only one automorphism, even if many exist!)
        unless ($is_isomorph)
        {
            # copy reactant pattern and label canonically
            my $copy_rpatt = $rpatt->copy(0);
        
            # get self-match
            (my $copy_match) = $rpatt->isomorphicToSubgraph($copy_rpatt);
            push @{$rr->Rmatches->[$ipatt]}, $copy_match;
        }
	

	    # find all matches in $sg_list. load maps into Rmatches.
	    my $n_matches = $rr->find_reactant_matches( $ipatt, $sg_list, $model );

		if ( $params->{verbose} )
		{
		    my $tot_matches = @{$rr->Rmatches->[$ipatt]};
		    printf $params->{indent} . "  ..found %d match%s to reactant %d\n",
		        $tot_matches, ($tot_matches==1 ? '' : 'es'), $ipatt+1;
		}
		++$ipatt;
	}


    # create crossproduct object to iterate over all reactant sets
    my $crossprod_reactants = CrossProduct::new();
    $crossprod_reactants->initialize( $rr->Rmatches );
    
    # loop over every reactant set
    my $reactant_set = [];
    while ( $crossprod_reactants->getNext($reactant_set) )
    {
        # apply rule to reactant set and get the resulting refined rule
        my $refined_rule = $rr->apply_to_reactants( $reactant_set, $model, $hybrid_model, $params );
        next unless ( defined $refined_rule );

        if ( exists $refinement_map->{$refined_rule->StringID} )
        {
            my $multiplicity = $refined_rule->RateLaw->Factor;
            my $prior_refinement = $refinement_map->{$refined_rule->StringID};
            $prior_refinement->RateLaw->Factor( $prior_refinement->RateLaw->Factor + $multiplicity );
        }
        else
        {           
            $refinement_map->{$refined_rule->StringID} = $refined_rule;
            push @$refinements, $refined_rule;
        }
    }

    foreach my $refinement ( @$refinements )
    {
        # add statFactor into the RateLaw constant
        my $ratelaw = $refinement->RateLaw;
        unless ( $ratelaw->Factor == 1.0 )
        {
            # get rate parameter
            my $param_name = $ratelaw->Constants->[0];
            my ($rate_param,$err) = $hybrid_model->ParamList->lookup($param_name); 
            # multiply rate parameter by Factor
            my $new_rate_expr = Expression::operate( '*', [$ratelaw->Factor, $rate_param->Expr], $hybrid_model->ParamList );
            my $new_param_name = $new_rate_expr->getName( $hybrid_model->ParamList, $param_name.'_' );
            $ratelaw->Constants->[0] = $new_param_name;
            # set statFactor to 1
            $ratelaw->Factor(1);
	    }
	}

    %{$refinement_map} = ();
    return ($err, $refinements);
}	



###
###
###


# Find matches to reactant pattern $ipatt in the list $sg_list.
#  Store maps in $rr->Rmatches.  Returns the number of matches.
sub find_reactant_matches
{
    my $rr      = shift;
    my $ipatt   = shift;
    my $sg_list = shift;
    my $model   = shift;

    # include by default?  (yes, if there are no include patterns)
    my $include_default = @{$rr->Rinclude->[$ipatt]} ? 0 : 1;

    my $n_matches = 0;
    # loop over species-graphs
    foreach my $sg ( @$sg_list )
    {
        # check for subgraph isomorphisms
        my @matches = $rr->Reactants->[$ipatt]->isomorphicToSubgraph($sg);
        
        if (@matches)
        {   
            # check for inclusion
            my $include = $include_default;
            foreach my $patt_incl ( @{$rr->Rinclude->[$ipatt]} )
	        {
		        unless ( $patt_incl->isomorphicToSubgraph($sg) )
		        {   # Include this match from list since graph matches included graph
	    		    $include = 1;
	    		    last;
	       		}
			}

            # check for exclusion
            if ($include)
            {
			    foreach my $patt_excl ( @{ $rr->Rexclude->[$ipatt] } )
			    {
			        if ( $patt_excl->isomorphicToSubgraph($sg) )
				    {
                        # Exclude this match from list since graph matches excluded graph
	      		        $include = 0;
				        last;
				    }
				}
	    	}
	    	
	    	# add to matches
	    	if ($include)
	    	{
	    	    push @{$rr->Rmatches->[$ipatt]}, @matches;
                ++$n_matches;
            }
        }
	}

	return $n_matches;
}


###
###
###



# apply rule to a set of reactants in @$reactant_set
#  returns refined rule if everything is ok, otherwise returns undefined.
sub apply_to_reactants
{
    # get input arguments
    my $rr = shift @_;
    my $reactant_set = shift @_;
    my $model = shift @_;
    my $hybrid_model = shift @_;
    my $user_params = (@_) ? shift : {};


    # Specify default params
	my $params = {  max_agg    => 1e99,
	                max_stoich => {},
	                verbose    => 0 
	             };
	# overwrite defaults with user params
	while ( my ($opt,$val) = each %$user_params )
    {   $params->{$opt} = $val;   }
    
    
    # remember number of reactants and products
    my $n_reactants = @{$rr->Reactants};
    my $n_products  = @{$rr->Products};

    
    # copy the target reactants and transfer labels from patterns to the copied reactants.
    my $matches   = [];   # holds maps from reactant patterns to (copies of) target SpeciesGraphs
    my $reactants = [];   # holds (copies of) target SpeciesGraphs
    foreach my $match ( @$reactant_set )
    {
        # copy the map and target graph (map copy points to the target copy)
        my $match_copy = $match->copy_map_and_target();
        # tranfer labels and attributes on the source graph to the target graph
        $match_copy->transferLabels();
        # save the map copy and the target copy
        push @$matches,   $match_copy;
        push @$reactants, $match_copy->Target;
    }
    
    
    # next, let's collect all the labels in reactant and product patterns
    my $temp_labels = {};
    {
        my $used_labels = {};
        
        foreach my $pattR ( @{$rr->Reactants} )
        {   $pattR->gatherLabels( $used_labels );   }

        foreach my $pattP ( @{$rr->Products} )
        {   $pattP->gatherLabels( $used_labels );   }    
    
        # assign unique labels to everything that isn't labeled
        my $i_label = 0;
        foreach my $reactant ( @$reactants )
        {
            $i_label += $reactant->assignLabels( $i_label, $temp_labels, $used_labels );
        }
    }    
    

    # TODO: we want to check that reactants can interact here, but compartments aren't yet 
    # supported in NFsim.  So we'll cross this bridge later.  --Justin



	# create permutations of identical reactants and process each variant
	my $reactant_permutations = $rr->permute_identical_reactants( $reactants, $model, 1 );

    # Apply rule transformations and calculate labels.
    my $products   = undef;
    my $rxn_labels = {};
	foreach my $permute (@$reactant_permutations)
    {
		my ( $products_perm, $rxn_label ) = $rr->apply_operations( $matches, $permute );
        unless ( defined $products )
        {
            return undef  unless ( defined $products_perm );
            return undef  if ( exists $rr->RxnLabels->{$rxn_label} );
            $products = $products_perm;
        }
	    # add to the label hash
	    $rxn_labels->{$rxn_label} = 1;
    }


    # Check for correct number of product graphs
    if ( @$products != $n_products )
    {
        # If Molecules are being deleted, it is allowed to have more subgraphs than product patterns
        if ( $rr->DeleteMolecules  and  @$products > $n_products )
        {
            if ($params->{verbose})
            {   printf "Deleting molecules in Rule %s\n", $rr->Name();  }
        }   
        # Otherwise, the reaction shouldn't happen
        else
        {
	        if ($params->{verbose})
	        {   printf "Rule %s: n_sub (%d)!= n_prod (%d)\n", $rr->Name, @$products, $n_products;   }
  	        return undef;
        }
    }


	# Check and Process Product Graphs
    for ( my $ip = 0; $ip < @$products; ++$ip )
	{
	    my $p = $products->[$ip];
	    my $iprod = ($ip < @{$rr->Products}) ? $ip : -1;

        # check for Max Aggregation violations
		if ( @{$p->Molecules} > $params->{max_agg} )
		{
            #printf "%d molecules exceeds max_agg of %d\n", $#{$p->Molecules}+1, $max_agg;
 		    return undef;
		}

		# check for Max Stoichiometry violations
		while ( my ($key,$max) = each %{$params->{max_stoich}} )
		{
			next if $max =~ /^unlimited/;
			if ( $p->stoich($key) > $max )
			{
                #printf "Stoichometry of $key in %s exceeds max of %d\n", $p->toString(), $max;
		  	    return undef;
			}
		}

        # Put product graph in canonical order (quasi-canonical for the time being)
	    if ( my $err = $p->sortLabel() )
	    {   # mysterious problem
	        print "WARNING: some problem in sortLabels.\n"
	    	    . ">> $err\n"
        	    . ">>", $rr->toString(), "\n";
	   	    return undef;
		}

		# Does product match excluded pattern?
		if ( $iprod >= 0 )
		{
			foreach my $patt_excl ( @{$rr->Pexclude->[$iprod]} )
			{
				if ( $patt_excl->isomorphicToSubgraph($p) )
				{
		            # Abort this reaction
                    return undef;
				}
			}

            # Does product match included pattern?  Must do so if include patterns are specified.
            if ( @{$rr->Pinclude->[$iprod]} )
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
			    return undef unless $include;
		    }
		}
    }


    # if reactant is isomorphic to a population graph, then substitute
    if ( defined $model->PopulationList )
    {
        # first replace reactants
        foreach my $reactant ( @$reactants )
        {
            foreach my $pop ( @{$model->PopulationList->List} )
            {
                if ( $pop->Species->isomorphicToSubgraph($reactant) )
                {
                    # replace reactant with population and transfer pattern label
                    my $label = $reactant->Label();
                    $reactant = $pop->Population->copy();
                    $reactant->Label( $label );
                    last;
                }
            }
        }
        # now products
        foreach my $product ( @$products )
        {
            foreach my $pop ( @{$model->PopulationList->List} )
            {
                if ( $pop->Species->isomorphicToSubgraph($product) )
                {       
                    # replace product with population and transfer pattern label
                    my $label = $product->Label();   
                    $product = $pop->Population->copy();
                    $product->Label( $label );
                    last;
                }
            }    
        }
    }
    # TODO: if population obscures a molecule rooted local function, we'll need to
    #  pre-compile the local function. But this can wait until we handle molecule rooted
    #  observables
    
    # filter out tags that no longer map from R to P.
    {
        # gather labels in reactants
        my $labelsR = {};
        foreach my $reactant ( @$reactants )
        {   $reactant->gatherLabels( $labelsR );   }
        
        # also gather labels in products
        my $labelsP = {};
        foreach my $product ( @$products )
        {   $product->gatherLabels( $labelsP );    }      
        
        # find labels that do not have a partner
        my $deleteLabels = {};
        foreach my $label ( keys %$labelsR )
        {
            next unless ( exists $temp_labels->{$label} );
            unless ( exists $labelsP->{$label} )
            {   $deleteLabels->{$label} = 1;   }
        }
        foreach my $label ( keys %$labelsP )
        {
            next unless ( exists $temp_labels->{$label} );
            unless ( exists $labelsR->{$label} )
            {   $deleteLabels->{$label} = 1;   }
        }
        
        # remove unpartnered labels
        foreach my $reactant ( @$reactants )
        {   $reactant->removeLabels( $deleteLabels );   }
        
        foreach my $product ( @$products )
        {   $product->removeLabels( $deleteLabels );    }        
    }        
    # finally, we can remove unnecessary temporary labels
    SpeciesGraph::removeRedundantLabels( $reactants, $products, $temp_labels );


    # We can create the refinement now, but first remember that we encountered
    #  this reaction already
    foreach my $rxn_label (keys %$rxn_labels)
    {
        $rr->RxnLabels->{$rxn_label} = 1;
    }
 
    # Create rule-refinement
    my $refined_rule = RxnRule::new();

    # write rule name (remove parantheses around "reverse")
    my $name = $rr->Name . '_v' . scalar (keys %{$rr->RxnLabels});
    $name =~ s/\(reverse\)/_rev/g;
	    
    $refined_rule->Name( $name );
    $refined_rule->Reactants( $reactants );
    $refined_rule->Products( $products );
    $refined_rule->RateLaw( $rr->RateLaw->copy() );
    $refined_rule->TotalRate( $rr->TotalRate );
    $refined_rule->Priority( $rr->Priority );	    
    $refined_rule->DeleteMolecules( $rr->DeleteMolecules );
    $refined_rule->MoveConnected( $rr->MoveConnected );
        	    
    my $rinclude = [];
    my $rexclude = [];
    for ( my $ir = 0;  $ir < @$reactants; ++$ir )
    {
        my $reactant = $reactants->[$ir];
        if ( $reactant->isPopulationType($hybrid_model->MoleculeTypesList) )
        {   # no need to recheck include/exclude if reactant is a population
            push @$rinclude, [];
            push @$rexclude, [];
        }
        else
        {   
            push @$rinclude, [ map {$_->copy()->relinkCompartments($hybrid_model->CompartmentList)} @{$rr->Rinclude->[$ir]} ];
            push @$rexclude, [ map {$_->copy()->relinkCompartments($hybrid_model->CompartmentList)} @{$rr->Rexclude->[$ir]} ];
        }
    }
	    
    my $pinclude = [];
    my $pexclude = [];
    for ( my $ip = 0;  $ip < @$products; ++$ip )
    {
        my $product = $products->[$ip];
     
        if ( $ip >= @{$rr->Products}  or  $product->isPopulationType($hybrid_model->MoleculeTypesList) )
        {   # no need to recheck include/exclude if product is a population
            push @$pinclude, [];
            push @$pexclude, [];
        }
        else
        {
            push @$pinclude, [ map {$_->copy()->relinkCompartments($hybrid_model->CompartmentList)} @{$rr->Pinclude->[$ip]} ];
            push @$pexclude, [ map {$_->copy()->relinkCompartments($hybrid_model->CompartmentList)} @{$rr->Pexclude->[$ip]} ];
        }
    }	    
	    
    $refined_rule->Rinclude( $rinclude );
    $refined_rule->Pinclude( $rexclude );
    $refined_rule->Rexclude( $pinclude );
    $refined_rule->Pexclude( $pexclude );
   

    # gather references
    my $rrefs = {};
    for ( my $i_sg = 0; $i_sg < @$reactants;  ++$i_sg)
    {
        my $sg = $reactants->[$i_sg];
        $sg->gatherLabels( $rrefs, $i_sg );
    }
    my $prefs = {};
    for ( my $i_sg = 0; $i_sg < @$products;   ++$i_sg)
    {
        my $sg = $products->[$i_sg];
        $sg->gatherLabels( $prefs, $i_sg );
    }

    $refined_rule->RRefs( $rrefs );
    $refined_rule->PRefs( $prefs );

    # construct an ID for this rule
    my $stringID = join '+', (map {$_->StringExact} @$reactants);
    $stringID .= '->';
    $stringID .= join '+', (map {$_->StringExact} @$products[0..$n_products-1]);
    if ( @$products > $n_products )
    {
        $stringID .= '+' . join( '+', sort (map {$_->StringExact} @$products[$n_products..$#$products]) );
    }
    $refined_rule->StringID( $stringID );
            
            
    # Find mapping of reactants onto products (is this necessary? ..eh, why not)
    # TODO: if this fails, it's an error
    if ( my $err = $refined_rule->findMap( $hybrid_model->MoleculeTypesList ) )
    {   return undef;   }
    	
    	
    # Does refined rule include a delete molecule transform?
    my $delmol_refined = grep {$_ =~ /\./} @{$refined_rule->MolDel};
    if ( $delmol_refined )
    {
        unless ( $refined_rule->DeleteMolecules )
        {
            # set delete molecules flag (NFsim requires this)
            $refined_rule->DeleteMolecules(1);
            if ( @{$rr->MolDel} )
            {   # warn user since this may produce conflics with delete species transforms
                send_warning( sprintf "DeleteMolecules flag added to rule %s.", $refined_rule->Name );
            }
        }
    }  	
    
    # add multiplicity
    $refined_rule->RateLaw->Factor( scalar keys %$rxn_labels ); 
	
	# all done!
    return $refined_rule;
}



###
###
###



sub permute_identical_reactants
# $reactant_permutations = $rr->permute_identical_reactants(\@targets)
{
	my $rr      = shift;
	my $targets = shift;
	my $model   = (@_) ? shift : undef;
	# if true, check targets to see if they are patterns or species.
	#  If Patterns, do not permute!
	my $fixPatterns = (@_) ? shift : 0;
	
	my $n_targets = scalar @$targets;

	# group reactants by StringExact
	my $identical_reactants = {};
	my $ir                  = 0;
	foreach my $targ (@$targets)
	{
	    if ( $fixPatterns  and  !($targ->checkSpecies($model)) )
	    {   # Don't permute patterns
   	        push @{ $identical_reactants->{"|$ir|"} }, $ir;
	    }
	    else
	    {
		    push @{ $identical_reactants->{$targ->StringExact} }, $ir;
		}
		++$ir;
	}

	# create permuations of all identical reactants
	my $ii                  = 0;
	my $map_reactants       = [];
	my $permutation_subsets = [];
	foreach my $rset ( values %$identical_reactants )
	{
		# construct the map from a reactant's index
		# to its identical reactant set and index in that set.
		my $kk = 0;
		foreach my $jj (@$rset) 
        {
			$map_reactants->[$jj] = [ $ii, $kk ];
			$kk++;
		}

		# get all permutations of these identical reactants
		$permutation_subsets->[$ii] = generate_permutations($rset);
		$ii++;
	}

	# get crossproduct of permutation sets
	my $permutation_sets = generate_crossproduct($permutation_subsets);

	# reformat permutation sets into a single vector
	my $reactant_permutations = [];
	foreach my $perm (@$permutation_sets)
	{
		my $perm_vec = [];
		my $kk       = 0;
		foreach my $map (@$map_reactants)
		{
			push @$perm_vec, $perm->[ $map->[0] ]->[ $map->[1] ];
			$kk++;
		}
		push @$reactant_permutations, $perm_vec;
	}

	return $reactant_permutations;

	##
	##

	# generate permutations
	sub generate_permutations
	{
		my $elements     = shift;
		my $permutations = [];

		foreach my $ii ( 0 .. $#{$elements} )
		{
			my $elements_ii = [];
			push @$elements_ii, @{$elements}[ 0 .. $ii - 1 ];
			push @$elements_ii, @{$elements}[ $ii + 1 .. $#{$elements} ];

			my $perm_ii;
			if ( @$elements_ii > 1 )
            {
				$perm_ii = generate_permutations($elements_ii);
			}
			else
            {
                $perm_ii = [$elements_ii];
            }

			foreach my $perm (@$perm_ii)
            {
				unshift @$perm, $elements->[$ii];
				push @$permutations, $perm;
			}
		}
		return $permutations;
	}

	##
	##

	sub generate_crossproduct
	{
		my $sets          = shift;
		my $cross_product = [];

		unless (@$sets) { return $cross_product; }

		my $curr_set        = shift @$sets;
		my $cross_remaining = generate_crossproduct($sets);

		foreach my $elem (@$curr_set)
        {
			if (@$cross_remaining)
            {
				foreach my $subcross (@$cross_remaining)
                {
                    push @$cross_product, [ $elem, @$subcross ];
                }
			}
			else
            {
                push @$cross_product, [$elem];
            }
		}

		return $cross_product;
	}

}




###
###
###



sub factorial
{
	my $n = shift;
	if   ( $n == 0 ) {  return 1;  }
	else             {  return $n * factorial( $n - 1 );  }
}



###
###
###

1;
