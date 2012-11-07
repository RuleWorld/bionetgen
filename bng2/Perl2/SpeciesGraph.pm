package SpeciesGraph;

# pragmas
use strict;
use warnings;

# Perl Modules
use Class::Struct;
use FindBin;
use lib $FindBin::Bin;
use List::Util qw(min max sum);

# BNG Modules
use Molecule;
use Component;
use Compartment;
use BNGUtils;
use Map;
use MoleculeTypesList;
use HNauty;



# give names to some options that we pass to SpeciesGraph methods
$SpeciesGraph::IS_SPECIES     = 1;
$SpeciesGraph::IS_NOT_SPECIES = 0;


struct SpeciesGraph =>
{
    Name  => '$',
    Label => '$',   # Local variable to assign to this Object (analogous to Label atrribute of Moleules
	                # and Components, not to be confused with graph label)
    Compartment => 'Compartment',
    Molecules   => '@',
    Edges       => '@',    # an array whose only element is an array of edge definitions (why a 2d array?)
    Adjacency   => '%',
    StringID    => '$',    # This is for the Canonical label.
    StringExact => '$',    # This is also related to canonical labeling (what's the difference?)  --justin
    Quantifier  => '$',
    Species     => 'Species', # Set only if this SpeciesGraph is bound to a particular Species
    MatchOnce   => '$',       # Map this pattern at most 1 time to a given species
    Fixed       => '$',       # Concentration of species referred to by this pattern is constant.
    IsCanonical => '$',
    Automorphisms => '$'
};



# Set default species label method:
#   Allowed values are Auto, HNauty, Quasi.
my $SpeciesLabel = 'Auto';



###
###
###



# set the method used to generate SpeciesGraph Lables
#  HNauty == canonical labeling via HNauty method
#  Auto   == quasi-canonical labeling with isomorphism checks similar graphs
#  Quasi  == quasi-canonical labeling, no checks (unsafe method)
sub setSpeciesLabel
{
	my $label = shift;
	my %valid = ( 'Auto' => 1, 'HNauty' => 1, 'Quasi' => 1 );
	if ( defined( $valid{$label} ) )
	{
		$SpeciesLabel = $label;
		print "SpeciesLabel method set to $label.\n";
	}
	else {
		return ("Invalid value for SpeciesLabel function: $label");
	}
	return ("");
}




###
###
###

# check if this is a null speciesGraph
sub isNull
{
    my $sg = shift;
    
    if ( @{$sg->Molecules} == 0 )
    {   return 1;  }
    else
    {   return 0;  }
}



###
###
###



sub readString
{
	my $sg            = shift;
	my $strptr        = shift;
	my $clist         = shift;
	my $is_species    = shift;
	my $stop          = shift;
	my $mtlist        = shift;
    # should we count the number of automorphisms?  (do this for molecule observables!)
	my $CountAutos = (@_) ? shift : 0;

    # Allow new molecule types?
	my $AllowNewTypes = $mtlist->StrictTyping ? 0 : 1;

    # holds error for return value
    my $err = undef;

	my $string_left = $$strptr;

	# remove leading whitespace
	$string_left =~ s/^\s+//;

    # check for premature termination
    if ( !$string_left  or  ($stop  and  $string_left =~ /$stop/) )
    {   return ( "Expecting SpeciesGraph but found termination characters instead\n"
                ."  in string '$string_left'." );
	}

    # Header (all characters up to ":")
    # Fixed bug here  --Justin
    # restrict pre ":" characters to word characters, whitespace, "%", "*" and "@",
    # QUESTION: should white space be disallowed since it is the stop character after product patterns?
	if ( $string_left =~ s/^([\w\s%@*]+)(::|:)\s*// )
	{
		my $head = $1;

		# Optional species name
		# COMMENT: why is "*" allowed in a Name?
		if ( $head =~ s/^([\w\s*]+)// ) {  $sg->Name($1);  }

		# Optional Label (variable tag name)
		if ( $head =~ s/^\%(\w+)// ) {  $sg->Label($1);  }

		# Optional species compartment
		if ( $head =~ s/^\@(\w+)// )
        {
			my $comp = $clist->lookup($1);
			if   ($comp) { $sg->Compartment($comp); }
			else         { return ("Undefined compartment $1"); }
		}
		if ($head)
        {
			return "Improper syntax for SpeciesGraph header at $head";
		}
	}

    # check for premature termination
    if ( !$string_left  or  ($stop  and  $string_left =~ /$stop/)  )
    {   return "Expecting SpeciesGraph but found termination characters instead\n"
              ."  in string '$string_left'.";
	}
    
    # parse molecules
	while ($string_left)
	{
		# Handle continuation and stopping
		# molecule separator characters
		next if ( $string_left =~ s/^\.// );
		last if ( $stop  and  $string_left =~ /$stop/ );

		# Handle modifier syntax
		if ( $string_left =~ s/^\$// )
		{   # Set Fixed attribute
		    $sg->Fixed(1);
		}
		elsif ( $string_left =~ s/^(=|==|<|<=|>|>=)(\d+)// )
		{   # Quantifier
			my $op = $1;
			if ( $op eq '=' ) {  $op = '=='  }
			$sg->Quantifier( $op . $2 );
			if ($is_species) {  return "Quantifier not allowed in species";  }
		}
		elsif ( $string_left =~ s/^[{]// )
		{   # Attributes listed in {}
			while ( !( $string_left =~ s/^\}// ) )
			{
				my $attr  = '';
				my $value = '';

				# Get attribute name
				if ( $string_left =~ s/^([^,=\}]+)// ) {  $attr = $1;  }
				else
                {
					return "Null attribute for SpeciesGraph at $string_left";
				}

				# Get (optional) attribute value
				if ( $string_left =~ s/^=([^,\}]+)// ) { $value = $1; }

				# Remove trailing comma
				$string_left =~ s/^,//;

				if ( $attr eq 'MatchOnce' )
                {
					my $val = booleanToInt($value);
					if ( $val == -1 )
                    {
						return "Invalid value $value assigned to Boolean attribute $attr";
					}
					$sg->MatchOnce($val);
				}
				elsif ( $attr eq 'Fixed' )
                {
					my $val = booleanToInt($value);
					if ( $val == -1 )
                    {
						return "Invalid value $value assigned to Boolean attribute $attr";
					}
					$sg->Fixed($val);
				}
				else {  return "Invalid attribute $attr for SpeciesGraph";  }
			}
		}
		else
		{
			# Read molecule
			my ( $mol, $err ) = Molecule::newMolecule( \$string_left, $clist );
            if ( $err ) {  return $err;  }

		    push @{$sg->Molecules}, $mol;
		}
	}


	# Check edge labels for correct definition
	# Sorts molecules and components and contructs edges an adjacency matrix.
	if ($is_species)
	{
	    # a real species cannot be null!
	    unless ( @{$sg->Molecules} )
	    {
	        return "The null species is not a valid species in $$strptr";
	    }
	
		# get Species Compartment (undefined is possible). --justin
		# FUTURE: check for well-defined compartment in "strict" mode.  --justin
		$err = $sg->assignCompartment();
		return $err if ($err);

		# NOTE: sortLabel sorts the molecules in a canonical or semi-canonical fashion,
		#  in the process the edges are updated (so no need to call updateEdges here!)
		if ( $err = $sg->sortLabel() ) {  return $err;  }

		# Check for correct number of subgraphs
		my ($nsub) = $sg->findConnected();
	    unless ( $nsub == 1 )
		{
			return "Species $$strptr is not connected";
		}
	}
	else
	{
	    my $dangling_error = 0;
	    $err = $sg->updateEdges( $dangling_error ); 
		if ($err) {  return $err;  }
	}

	# Check that molecules match declared types
	if ($mtlist)
	{
		$err = $mtlist->checkSpeciesGraph( $sg, { IsSpecies     => $is_species,
                                                  AllowNewTypes => $AllowNewTypes } );
		if ($err) {  return ($err);  }
	}

    # count automorphisms
    if ( $CountAutos )
    {
        my @autos = $sg->isomorphicToSubgraph($sg);
        $sg->Automorphisms( scalar @autos );
    }

	$$strptr = $string_left;
	return $err;
}



###
###
###



# check if a speciesGraph represents a fully-specified species.
# TODO: need to check that compartments are specified, if we're using compartments!
sub checkSpecies
{
    my $sg = shift;
    my $model = shift;

    my $err;
    my $comp;
    my $params = { IsSpecies            => 1,
                   AllowNewTypes        => 0  };
    
    # check that components are fully specified
    $err = $model->MoleculeTypesList->checkSpeciesGraph( $sg, $params );                           
	return 0 if ($err);

    # check compartment validity
    ($comp, $err) = $sg->inferSpeciesCompartment();
    return 0 if ($err);

    # everything checks out! this is a species
    return 1;
}



###
###
###


# Convert a pointer to a SpeciesGraph component or molecule to a label (used in XML output)
sub p_to_label
{
	my $sg     = shift;
	my $p      = shift;
	my $string = shift;

	my @inds = split( '\.', $p );

	if ( $#inds == 0 ) {
		$string .= sprintf "_M%d", ( $inds[0] + 1 );
	}
	elsif ( $#inds == 1 ) {
		$string .= sprintf "_M%d_C%d", ( $inds[0] + 1 ), $inds[1] + 1;
	}
	else {
		die "Error in p_to_label";
	}

	return ($string);
}



###
###
###



sub labelHNauty
{
	use strict;
	use Data::Dumper;
	
	my $sg = shift;
	my $dangling_error = (@_) ? shift : 1;
	my $trim_dangling  = (@_) ? shift : 0;
	
	# holds error value for return
	my $err;

	# Construct adjacency matrix and partition
	my %adj       = ();
	my @partition = ();

	# Contains partitions for molecules
	my %mtypes = ();

	# Contains partitions for components
	my %ctypes = ();

	# Hash to convert pointers to node index
	my %pointer_index = ();

	# Bonds hashed by name
	my %bonds = ();

	my $n_mol = scalar( @{ $sg->Molecules } );
	my $imol  = 0;
	my $icomp = $n_mol;

    # Loop over molecules and components to
    # (1) determine pointer to index conversions
    # (2) partition by Molecule and Componoent names (could add connectivity later, if it improves performance)
    # (3) create edges from Molecules to its contained Components
    # (4) collect bonds in a hash (precludes need to call update edges beforehand).
	foreach my $mol ( @{ $sg->Molecules } )
    {
		my $mname = $mol->Name . "."
                    . ( defined $mol->State ? $mol->State : '' ) . "."
                    . ( defined $mol->Compartment ? $mol->Compartment : '' );
		$pointer_index{$imol} = $imol;
		push @{ $mtypes{$mname} }, $imol;
		foreach my $edge ( @{ $mol->Edges } )
        {
			push @{ $bonds{$edge} }, $imol;
		}
		my $jcomp = 0;
		foreach my $comp ( @{$mol->Components} )
        {
			my $cname = $mname . "."
			            . $comp->Name . "."
			            . ( defined $comp->State ? $comp->State : '' ) . "."
			            . ( defined $comp->Compartment ? $comp->Compartment : '' );
			my $p = "$imol.$jcomp";
			$pointer_index{$p} = $icomp;
			push @{ $ctypes{$cname} }, $icomp;
			$adj{$imol}{$icomp} = [0];
			$adj{$icomp}{$imol} = [0];
			foreach my $edge ( @{$comp->Edges} )
            {
				push @{ $bonds{$edge} }, $p;
			}
			++$icomp;
			++$jcomp;
		}
		++$imol;
	}

	# Loop over Edges to determine adjacency matrix for bonds
	foreach my $ename ( keys %bonds )
    {
		my $earr   = $bonds{$ename};
		my $istart = $pointer_index{ $$earr[0] };
		my $iend   = $pointer_index{ $$earr[1] };
		$adj{$istart}{$iend} = [1];
		$adj{$iend}{$istart} = [1];
	}

	# Set up partitions; sort order molecules then components.
	foreach my $type ( sort keys %mtypes )
    {
		push @partition, $mtypes{$type};
	}
	foreach my $type ( sort keys %ctypes )
    {
		push @partition, $ctypes{$type};
	}

	# Call HNauty to obtain canonical ordering
	( my $perm ) = HNauty( \%adj, \%adj, \@partition );


	# Reorder Components of each Molecule according to canonical order
	my $icomp_start = $n_mol;
	foreach my $mol ( @{ $sg->Molecules } )
    {
		my $icomp_end = $icomp_start + $#{ $mol->Components };
		my @perm_c = map { $perm->{$_} } ( $icomp_start .. $icomp_end );

		my @comp_perm =  map { [ $perm_c[$_], $mol->Components->[$_] ] } ( 0 .. $#{ $mol->Components } );
		@comp_perm = sort { $a->[0] <=> $b->[0] } @comp_perm;
		$mol->Components( [ map { $_->[1] } @comp_perm ] );
		$icomp_start = $icomp_end + 1;
	}

	# Reorder Molecules according to canonical order
	# This is probably inefficient since we have the exact mapping
	my @mol_perm =
	  map { [ $perm->{$_}, $sg->Molecules->[$_] ] }
	  ( 0 .. $#{ $sg->Molecules } );

	#map {print $_->[0];} @mol_perm;  print "\n";
	@mol_perm = sort { $a->[0] <=> $b->[0] } @mol_perm;
	$sg->Molecules( [ map { $_->[1] } @mol_perm ] );

    # update edges
	$err = $sg->updateEdges( $dangling_error, $trim_dangling );
	if ( $err ) {  return $err  };

	# Create exact string representation:  include edge labels and strip attributes
	#  first argument tells "toString" whether to ignore edge labels (true) or not (false)
	#  second argument tells "toString" whether to strip attributes (true) or not (false)
	my $string = $sg->toString( 0, 1 );

	$sg->StringExact($string);
	$sg->StringID($string);
	$sg->IsCanonical(1);

	return $err;
}



###
###
###



sub labelQuasi
{
	my $sg = shift;
	my $isCanonical = (@_) ? shift : 0;
	my $dangling_error = (@_) ? shift : 1;
	my $trim_dangling  = (@_) ? shift : 0;

    my $err = undef;

	# Sort components of each molecule
	foreach my $mol (@{$sg->Molecules})
	{
		@{$mol->Components} = sort by_component @{$mol->Components};
	}

	# Sort molecules
	@{$sg->Molecules} = sort by_molecule @{$sg->Molecules};

    # update edges after sort!
	$err = $sg->updateEdges( $dangling_error, $trim_dangling );
	if ( $err ) {  return $err;  }


	# Create quasi-canonical string representation
	my $string = $sg->toString( 1, 1 );
	$sg->StringID($string);

	# Create exact string representation:  include edge labels and strip attributes
	#  first argument tells "toString" whether to ignore edge labels (true) or not (false)
	#  second argument tells "toString" whether to strip attributes (true) or not (false)
	$string = $sg->toString( 0, 1 );
	
	$sg->StringExact($string);
	$sg->IsCanonical($isCanonical);

	return $err;
}



###
###
###



# Modified version using different labeling functions
sub sortLabel
{ 
	my $sg = shift;
	my $dangling_error = (@_) ? shift : 1;
	my $trim_dangling  = (@_) ? shift : 0;

    my $err = undef;
	if    ( $SpeciesLabel eq 'Auto' )
	{
	    my $is_canonical = 0;
		$err = $sg->labelQuasi( $is_canonical, $dangling_error, $trim_dangling );
	}
	elsif ( $SpeciesLabel eq 'Quasi' )
	{   # Equivalent to setting check_iso=>0
	    my $is_canonical = 1;
		$err = $sg->labelQuasi( $is_canonical, $dangling_error, $trim_dangling );
	}
	elsif ( $SpeciesLabel eq 'HNauty' )
	{
		$err = $sg->labelHNauty( $dangling_error, $trim_dangling );
	}
	
	return $err;
}



###
###
###



sub assignCompartment
 # $err = $sg->assignCompartment()
 # $err = $sg->assignCompartment( $force_comp )
 #
 # Calls inferSpeciesCompartment to determine Species Compartment and then makes
 # the assignment, if valid.  Returns an error string if there are any problems.
 # Note that the assignment is allowed to be undefined!
 #
 # If $force_comp is supplied, the SpeciesGraph is forced to that compartment,
 #  unless it is incompatible with any molecule compartments.
 #
 # Note (Justin): separated assign and infer Compartment into two methods.
{
	my $sg = shift;                                # SpeciesGraph
	my $force_comp  = scalar(@_) ? shift : undef;

	# get Species Compartment (return any errors)
	my ( $species_comp, $err ) = $sg->inferSpeciesCompartment();
	return $err if ($err);

	if ( defined $force_comp  and  defined $species_comp )
	{
		unless ( $species_comp == $force_comp )
		{
			return sprintf "Attempt to force incompatible species compartment on SpeciesGraph %s.", $sg->toString();
		}
	}
	elsif ( defined $force_comp  and  !defined($species_comp) )
	{
	    # set species compartment to the forced value
		$species_comp = $force_comp;
	}
	elsif ( !defined $force_comp and !defined($species_comp) )
	{
		# Currently nothing to do here!
        # in strict mode: generate error
        # return sprintf "Species Compartment for SpeciesGraph %s could not be determined.",
        #                $sg->toString();
		return '';
	}

	# Assign Compartment
	$sg->Compartment($species_comp);

    # Set Compartment of any Molecules without explicit Compartment definitions.
    # NOTE: This handles compartment specification when syntax "@C:Species" is used.
	foreach my $mol ( @{ $sg->Molecules } )
	{
		unless ( defined $mol->Compartment )
		{
			$mol->Compartment($species_comp);
		}
	}
	return '';
}



###
###
###



sub inferSpeciesCompartment
# (Compartment, err) = $sg->inferCompartment()
#
# Infers Species Compartment of a SpeciesGraph.  Returns an undefined Compartment
# if the Compartment cannot be inferred or is invalid.  Sets err=1 if Species
# Compartment is invalid and err=0 otherwise.
#
# TODO: There is no check for bond validity here!  SHould be added in future.
# TODO: What about verifying that all compartments are specified?
{
	my $sg = shift;

	my $inferred_comp;    # Species Compartment inferred from molecules
	my %volumes  = ();    # molecule volume compartments found in $sg
	my %surfaces = ();    # molecule surface compartments found in $sg
	my $err = '';  # return error (set string if species compartment is invalid)

	# Gather molecule compartments
	foreach my $mol ( @{ $sg->Molecules } )
	{
		my $comp = $mol->Compartment;

		# fixed BUG: if compartment molecule isn't explictly defined,
		# transfer the explicit species compartment (if any)  --justin
		if ( !(defined $comp)  and  defined $sg->Compartment )
		{
			$comp = $sg->Compartment;
		}

		next unless ( defined $comp );

		if    ( $comp->SpatialDimensions == 2 ) { $surfaces{$comp} = $comp; }
		elsif ( $comp->SpatialDimensions == 3 ) { $volumes{$comp}  = $comp; }
	}

	my $n_surfaces = scalar( keys %surfaces );
	my $n_volumes  = scalar( keys %volumes );

	# infer Species Compartment
	if ( $n_surfaces == 0 )
	{
		if ( $n_volumes == 0 )
		{   # no inferred compartment
			$inferred_comp = undef;
		}
		elsif ( $n_volumes == 1 )
		{   # unique volume compartment is inferred
			($inferred_comp) = ( values %volumes );
		}
		else #( $n_volumes > 1 )
		{   # error: multiple volumes with no surface
			$err =
			  sprintf
			  "Molecule Compartments of %s define invalid Species Compartment.",
			  $sg->toString();
			return ( undef, $err );
		}
	}
	elsif ( $n_surfaces == 1 )
	{   # unique surface compartment is inferred
		($inferred_comp) = ( values %surfaces );

		# check adjacency of volumes
		foreach my $comp ( values %volumes )
		{
			unless ( $inferred_comp->adjacent($comp) )
			{   # error: volume not adjacent to surface
				$err = sprintf "Molecule Compartments of %s define invalid Species Compartment.", $sg->toString();
				return ( undef, $err );
			}
		}
	}
	else #( $n_surfaces > 1 )
	{   # error: multiple surfaces
		$err = sprintf "Molecule Compartments of %s include more than 1 Surface.", $sg->toString();
		return ( undef, $err );
	}

    # Check that inferred Species Compartment assignment is Compatible with any explicit declaration
	if ( defined $sg->Compartment  and  defined $inferred_comp )
	{
		unless ( $inferred_comp == $sg->Compartment )
		{
			$err = sprintf "Explicit Species Compartment (%s)"
			              ." is not compatible with inferred Species Compartment (%s)",
			              $sg->Compartment->Name, $inferred_comp->Name;
			return ( undef, $err );
		}
	}

	# if no compartment was inferred, then use explicit compartment (if any)
	if ( defined $sg->Compartment and !( defined $inferred_comp ) )
	{
		$inferred_comp = $sg->Compartment;
	}

	# return inferred compartment
	return ( $inferred_comp, $err );
}



###
###
###



sub interactingSet

# bool = interactingSet ( $sg1, $sg2, .. )
#
# determine if list of species graphs forms an interacting Set.  Return 1 if
# yes, and 0 otherwise.
#
# NOTE: if all species have undefined compartment, then Return 1.  But return 0
# if only some of the species have undefined compartment.  This lets us run
# compartmentBNG in a "sloppy" mode where compartments may or may not be defined.
{
	my @sgs = @_;

	my $volume  = undef;    # volume compartment
	my $surface = undef;    # surface compartment

	# find surface and volume compartments
	my $missing = 0;        # track species without compartment
	foreach my $sg (@sgs)
	{
		my $comp = $sg->Compartment;
		unless ( defined $comp )
		{   # species has undefined compartment
			$missing++;
			next;
		}

		if ( $comp->SpatialDimensions == 2 )
		{   # this compartment is a surface
			if ( defined $surface )
			{   # Surface found previously.  This surface should be the same.
				return 0 unless ( $surface == $comp );
			}
			else
			{   # No surface found previously.
				$surface = $comp;
			}
		}
		elsif ( $comp->SpatialDimensions == 3 )
		{   # this compartment is a volume
			if ( defined $volume )
			{   # Volume found previously.  This volume should be the same.
				return 0 unless ( $volume == $comp );
			}
			else
			{   # No volume found previously.
				$volume = $comp;
			}
		}
	}

	if ( defined $surface and defined $volume )
	{   # surface and volume must be adjacent
		return 0 unless ( $surface->adjacent($volume) );
	}

	if ( $missing and ($missing < @sgs) )
	{   # mixture of reactants with and without compartment
		return 0;
	}

	# species are an interacting set!
	return 1;
}



###
###
###



sub find_compartment_connected
# @compartment_connected = $sg->find_compartment_connected( $i_mol )
#
# find the set of molecules in $sg that are connected to the molecule $i_mol by
# a path contained in the same compartment as $i_mol.  Returns a list of
# indexes corresponding to the molecules in the set.
{
	my ( $sg, $i_mol ) = @_;

	# get compartment of reference molecule
	my $comp = $sg->Molecules->[$i_mol]->Compartment;
	return () unless ( defined $comp );

	# build a hash that maps each molecule in compartment to the set
	# of adjacent vertices (also in the compartment)
	my $adjacency_hash = {};
	foreach my $edge ( @{ $sg->Edges } ) {
		my ( $mol1, $mol2 ) = ( $edge =~ /^(\d+)\.\d+\s+(\d+)\.\d+/ );
		if (    $sg->Molecules->[$mol1]->Compartment == $comp
			and $sg->Molecules->[$mol2]->Compartment == $comp )
		{
			push @{ $adjacency_hash->{$mol1} }, $mol2;
			push @{ $adjacency_hash->{$mol2} }, $mol1;
		}
	}

    # initialize hash map for collection of molecules in the compartment connected component
	my $compartment_connected = {};

    # find the compartment connected component using a depth first search (recursive)
	&depth_first_search( $i_mol, $adjacency_hash, $compartment_connected );
	return ( keys %$compartment_connected );


	sub depth_first_search
    # recursive method to find connected component of graph
    #  vertex:  index of the starting vertex.
    #  adjacency:  reference to hash that maps vertices to the set of adjacent vertices.
    #  connected_component:  reference to a hash whose keys are vertices in the connected component.
	{
		my ( $vertex, $adjacency, $connected_component ) = @_;

		# do nothing if vertex was already found
		return if ( exists $connected_component->{$vertex} );

		# vertex is in connected component
		$connected_component->{$vertex} = 1;

		# search edges at this vertex
		foreach my $adjacent_vertex ( @{ $adjacency->{$vertex} } ) {
			&depth_first_search( $adjacent_vertex, $adjacency,
				$connected_component );
		}
	}
}




###
###
###



# Copy a list of graphs and merge into one supergraph.
#  SpeciesGraph labels, IDs, and attributes are ignored.
#  But Labels for molecules and components are preserved.
#
# Copy merge also builds an offsets array, indicating the
#  first molecule associated with the ith connected component
sub copymerge
{
    # get list of species to copy/merge
	my @sgs = @_;

    # build offsets array:
    #  offset[i] = index of first molecule of reactant i in the aggregate graph
    my $offsets = [ (0) x @sgs ];
	
	# create new speciesgraph to hold copy
	my $sg_copy = SpeciesGraph->new();
	
	# create array to hold copied molecules
	my $mol_copy = [];
	$sg_copy->Molecules($mol_copy);
	
	# loop over subgraphs and copy
	my $dangling_error = 1;	
	my $offset = 0;
	for ( my $i_sg = 0;  $i_sg < @sgs;  ++$i_sg )
	{
	    my $sg = $sgs[$i_sg];
	    
	    # get offsets
	    $offsets->[$i_sg] = $offset;
	    $offset += @{$sg->Molecules};
	    
	    # if we find a non-species graph, then allow dangling!
	    unless ( defined $sg->Species  and  ref $sg->Species eq 'SPECIES' )
	    {   $dangling_error = 0;  }
	
	    # copy molecules
		foreach my $mol ( @{$sg->Molecules} )
		{
			# This form of Molecule->copy adds a prefix to the edges to avoid
			# overlap of edge names between different SpeciesGraphs
			push @$mol_copy, $mol->copy( 1, "${i_sg}_" );
		}
	}    # END loop over SpeciesGraphs

		
	# update graph edges
	my $err = $sg_copy->updateEdges( $dangling_error );
	if ( $err ) {  print "SpeciesGraph::copymerge(): updateEdges failed with error:\n  $err\n";  }
	
	# return copy
	return ($sg_copy, $offsets);
}



###
###
###



# Make an exact copy of a SpeciesGraph, including all labels, IDs, etc
#  NOTE: associated Species object, if exists, will not be copied!
sub copy
{
    # get speciesGraph to copy
	my $sg = shift;
    # copy labels?
    my $copy_labels = (@_) ? shift : 1;
    # should we generate a label if we're not copying one?
    my $get_label = (@_) ? shift : 1;

	# create new speciesgraph to hold copy
	my $sg_copy = SpeciesGraph->new();

    # copy scalar members
    $sg_copy->Name( $sg->Name );
    $sg_copy->Compartment( $sg->Compartment ) if (defined $sg->Compartment);
    $sg_copy->Quantifier( $sg->Quantifier );
    $sg_copy->MatchOnce( $sg->MatchOnce );
    $sg_copy->Fixed( $sg->Fixed );
    $sg_copy->Automorphisms( $sg->Automorphisms );

	# copy molecules
	if ( @{$sg->Molecules} )
	{
	    $sg_copy->Molecules( [map {$_->copy($copy_labels)} @{$sg->Molecules}] );
	}
	
	if ($copy_labels)
	{
        # copy labels, IDs, etc..
        $sg_copy->Label( $sg->Label );
        $sg_copy->StringID( $sg->StringID );
        $sg_copy->StringExact( $sg->StringExact );
        $sg_copy->IsCanonical( $sg->IsCanonical );
        	
	    # copy edges
	    foreach my $edge ( @{$sg->Edges} )
	    {
	        if ( ref $edge eq 'ARRAY' )
	        {   push @{$sg_copy->Edges}, [@$edge];   }
	        else
	        {   push @{$sg_copy->Edges}, $edge;   }
	    }
	    # copy adjacency
	    while ( my ($key,$val) = each %{$sg->Adjacency} )
	    {
	        if ( ref $val eq 'HASH' )
	        {   %{ $sg_copy->Adjacency->{$key} } = %{$val};   }
	        else
	        {   $sg_copy->Adjacency->{$key} = $val;   }
	    }
	}
	else
	{    
		my $dangling_error = 0;
		my $trim_dangling = 0;
	    if ($get_label)
	    {
	        # generate label and, as side-effect, build Edges and Adjacencies
		    my $err = $sg_copy->sortLabel( $dangling_error, $trim_dangling );
		    if ($err) {  print "SpeciesGraph::copy(): sortLabel failed with error:\n  $err\n";  }
		}
		else
		{
	        # Build Edges and Adjacencies, but don't get label
		    my $err = $sg_copy->updateEdges( $dangling_error, $trim_dangling );
		    if ($err) {  print "SpeciesGraph::copy(): updateEdges failed with error:\n  $err\n";  }		
		}
    }
	
	# return copy
	return $sg_copy;
}



###
###
###


# call this method to link Compartments to a new CompartmentList
sub relinkCompartments
{
    my $sg = shift;
    my $clist = shift;
    
    if ( defined $sg->Compartment )
    {
        my $new_comp = $clist->lookup( $sg->Compartment->Name );
        $sg->Compartment( $new_comp );
    }
    
    foreach my $mol ( @{$sg->Molecules} )
    {
        $mol->relinkCompartments( $clist );
    }
    
    return $sg;
}



###
###
###



# gather all labels associated with this Species
sub gatherLabels
{
    my $sg = shift;
    my $label_map = shift;
    my $i_sg = (@_) ? shift : 0;
    
    $label_map->{$sg->Label} = $i_sg  if (defined $sg->Label);
    for ( my $i_mol = 0;  $i_mol < @{$sg->Molecules}; ++$i_mol )
    {
        my $mol = $sg->Molecules->[$i_mol];
        $label_map->{$mol->Label} = "$i_sg.$i_mol"  if (defined $mol->Label);
        
        for ( my $i_comp = 0; $i_comp < @{$mol->Components}; ++$i_comp )
        {
            my $comp = $mol->Components->[$i_comp];
            $label_map->{$comp->Label} = "$i_sg.$i_mol.$i_comp"  if (defined $comp->Label);            
        }
    }
}


###
###
###


sub isPopulationType
{
    my $sg = shift;
    my $mtlist = shift;
 
    my $mt = $mtlist->MolTypes->{$sg->Molecules->[0]->Name};
    return (defined $mt->PopulationType) ? $mt->PopulationType : 0;
}



###
###
###



# assign unique labels to molecules and components of this species
sub assignLabels
{
    my $sg = shift;
    my $start_index = shift;
    my $new_labels = shift;
    my $used_labels = shift;
    
    my $base = 'T';
    my $i_label = $start_index;
    foreach my $mol ( @{$sg->Molecules} )
    {
        unless ( defined $mol->Label )
        {
            while ( exists $used_labels->{"$base$i_label"} )
            {  ++$i_label;  }
            $mol->Label( "$base$i_label" );
            $new_labels->{"$base$i_label"} = 1;
            ++$i_label;
        }
        foreach my $comp ( @{$mol->Components} )
        {
            unless ( defined $comp->Label )
            {
                while ( exists $used_labels->{"$base$i_label"} )
                {  ++$i_label;  }
                $comp->Label( "$base$i_label" );
                $new_labels->{"$base$i_label"} = 1;                
                ++$i_label;
            }
        }
    }
    return $i_label;
}



###
###
###



# search for and remove specific labels
sub removeLabels
{
    my $sg = shift;
    my $delete_labels = shift;

    foreach my $mol ( @{$sg->Molecules} )
    {
        if ( defined $mol->Label )
        {
            if ( exists $delete_labels->{$mol->Label} )
            {   $mol->Label(undef);   }
        }
        foreach my $comp ( @{$mol->Components} )
        {
            if ( defined $comp->Label )
            {
                if ( exists $delete_labels->{$comp->Label} )
                {   $comp->Label(undef);   }
            }
        }
    }
}



###
###
###



# try to map molecules and components in sgs1 into sgs2 without looking at labels.
#  When this can be done without confusion, delete temporary labels
sub removeRedundantLabels
{
    my $sgs1 = shift;
    my $sgs2 = shift;
    my $temp_labels = shift;
    
    my $autolabels1 = {};
    for ( my $i_sg = 0;  $i_sg < @$sgs1;  ++$i_sg )
    {
        my $sg = $sgs1->[$i_sg];
	    
	    # label molecules
	    for ( my $im = 0 ; $im < @{$sg->Molecules} ; ++$im )
	    {
		    my $mol   = $sg->Molecules->[$im];                  # molecule to be labeled
		    my @comps = (map {$_->Name} @{$mol->Components});   # list of component names
		    my $mlabel = $mol->Name;                            # molecule label
            # now add component names
			$mlabel .= '_' . join( '_', sort @comps )  if (@comps);

    		# write map:  molecule label -> (ptr)
            $autolabels1->{$mlabel} = []  unless (exists $autolabels1->{$mlabel});
		    push @{$autolabels1->{$mlabel}}, "$i_sg.$im";

		    # Component labels have syntax mlabel|cname
		    for ( my $ic = 0 ; $ic < @comps ; $ic++ )
		    {
				#my $clabel = $mlabel . '_' . scalar @{$autolabels1->{$mlabel}} . '|' . $comps[$ic];
                my $clabel = $mlabel . '|' . $comps[$ic];
                $autolabels1->{$clabel} = []  unless (exists $autolabels1->{$clabel});
		        push @{$autolabels1->{$clabel}}, "$i_sg.$im.$ic";
		    }
		}
    }

    my $autolabels2 = {};
    for ( my $i_sg = 0;  $i_sg < @$sgs2;  ++$i_sg )
    {
        my $sg = $sgs2->[$i_sg];
	    
	    # label molecules
	    for ( my $im = 0 ; $im < @{$sg->Molecules} ; ++$im )
	    {
		    my $mol   = $sg->Molecules->[$im];                  # molecule to be labeled
		    my @comps = (map {$_->Name} @{$mol->Components});   # list of component names
		    my $mlabel = $mol->Name;                            # molecule label
            # now add component names
			$mlabel .= '_' . join( '_', sort @comps )  if (@comps);

    		# write map:  molecule label -> (ptr)
            $autolabels2->{$mlabel} = []  unless (exists $autolabels2->{$mlabel});
		    push @{$autolabels2->{$mlabel}}, "$i_sg.$im";

		    # Component labels have syntax mlabel|cname
		    for ( my $ic = 0 ; $ic < @comps ; $ic++ )
		    {
				#my $clabel = $mlabel . '_' . scalar @{$autolabels2->{$mlabel}} . '|' . $comps[$ic];
				my $clabel = $mlabel . '|' . $comps[$ic];
                $autolabels2->{$clabel} = []  unless (exists $autolabels2->{$clabel});
		        push @{$autolabels2->{$clabel}}, "$i_sg.$im.$ic";
		    }
		}
    }
    
    #print STDERR "reactants: ", join(",", map { "$_=>".scalar @{$autolabels1->{$_}} } keys %$autolabels1 ), "\n";
    #print STDERR "products:  ", join(",", map { "$_=>".scalar @{$autolabels2->{$_}} } keys %$autolabels2 ), "\n";

    # remove temporary labels, if mapping is unambiguous
    foreach my $key ( keys %$autolabels1 )
    {   
        if ( exists $autolabels2->{$key} )
        {   # corresponding objects found in both products and reactants,
            # so check if there's a possibility for multiple correspondence maps
            my $set1 = $autolabels1->{$key};
            my $set2 = $autolabels2->{$key};

            # check how many objects in reactants and products have this autolabel
            next unless ( @$set1 == 1  and  @$set2 == 1 );
            
            # There's a 1-1 correspondence between objects with this autolabel.
            # Let's get the objects and remove temporary tags

            # get pointer to object in set1 
            my ($p1,$m1,$c1) = split /\./, $set1->[0];
            my $obj1 = (defined $c1) ? $sgs1->[$p1]->Molecules->[$m1]->Components->[$c1] : $sgs1->[$p1]->Molecules->[$m1];   

            # get pointer to object in set2
            my ($p2,$m2,$c2) = split /\./, $set2->[0];
            my $obj2 = (defined $c2) ? $sgs2->[$p2]->Molecules->[$m2]->Components->[$c2] : $sgs2->[$p2]->Molecules->[$m2];

            # are Labels (i.e. "tags") the same?  If not, then these are non-corresponding objects!
            next unless ( $obj1->Label eq $obj2->Label );

            # labels must be temporary (only need to check obj1 since the labels are equal
            next unless ( exists $temp_labels->{$obj1->Label} );
            
            # remove label from obj1 and obj2
            $obj1->Label(undef);
            $obj2->Label(undef);
        }
        else
        {   # No corresponding objects found in products.
            # We can go ahead and remove temporary labels.
            my $set1 = $autolabels1->{$key};
            foreach my $ptr1 (@$set1)
            {   # get pointer to object in set1
                my ($p1,$m1,$c1) = split /\./, $ptr1;
                my $obj1 = (defined $c1) ? $sgs1->[$p1]->Molecules->[$m1]->Components->[$c1] : $sgs1->[$p1]->Molecules->[$m1];
                next unless ( defined $obj1->Label );
                next unless ( exists $temp_labels->{$obj1->Label} );
                # there's no possible ambiguity here; remove label from obj1
                $obj1->Label(undef);
            }
        }
    }

    # remove temporary labels from products, if mapping is unambiguous
    foreach my $key ( keys %$autolabels2 )
    {
        # label must appear in both sets
        if ( exists $autolabels1->{$key} )
        {   # Corresponding objects found in both products and reactants.
            # Already handled this case.
            next;
        }
        else
        {   # No corresponding objects found in reactants.
            # We can go ahead and remove temporary labels.
            my $set2 = $autolabels2->{$key};
            foreach my $ptr2 (@$set2)
            {   # get pointer to object in set2
                my ($p2,$m2,$c2) = split /\./, $ptr2;
                my $obj2 = (defined $c2) ? $sgs2->[$p2]->Molecules->[$m2]->Components->[$c2] : $sgs2->[$p2]->Molecules->[$m2];
                # only remove temporary labels
                next unless ( defined $obj2->Label );
                next unless ( exists $temp_labels->{$obj2->Label} );
                # there's no possible ambiguity here; remove label from obj1
                $obj2->Label(undef);
            }
        }
    }
}


###
###
###


sub copySubgraph
# given a speciesGraph $sg with n-connected components, an array @$subgraph
#  that maps the ith molecule to the jth graph component, and a
#  component index $nsub:  Creates a copy of component $nsub and returns
#  the corresponding species graph.
#
# NOTE: it is assumed the components have no dangling-edges!!  If updateEdges
#   fails a warning is printing to the default output.
{
	my $sg       = shift;
	my $subgraph = shift;
	my $nsub     = shift;
	
	# holds error value
	my $err;
	
	# Create SG to hold subgraph copy
	my $sg_copy = SpeciesGraph->new();

    # copy molecules in the subgraph
	my $mol_copy = [];
	$sg_copy->Molecules( $mol_copy );
	foreach my $imol ( 0 .. $#{$sg->Molecules} )
	{
		next unless ( $$subgraph[$imol] == $nsub );
		push @$mol_copy, $sg->Molecules->[$imol]->copy();
	}
	
	# update graph edges
	my $dangling_error = 1;
	$err = $sg_copy->updateEdges( $dangling_error );
	if ( $err ) {   print "WARNING: $err.\n";   }
	
	return $sg_copy;
}





###
###
###



# returns the number of molecules with name $mname 
#  in species graph $sg.
sub stoich
{
	my $sg    = shift;
	my $mname = shift;
	my $count = 0;

	foreach my $mol ( @{ $sg->Molecules } )
	{
		next if ( $mname cmp $mol->Name );
		++$count;
	}
	return ($count);
}





###
###
###



sub updateEdges
# this method matches speciesGraph edges by labels, then sorts them, numbers them,
# and stores the list at @{$sg->Edges}.  An adjacency hash is also created
# and stored at @{$sg->Adjacency}.  Returns undefined value if edges are ok.  Returns
# error message otherwise.
#
# Notes:  edge array format = "m1.c1 m2.c2"
#         adjacency hash keys = {m1.c1}{m2.c2}, except for dangling edge keys = {m1.c1}
#         Molecules and Components should be pre-sorted before calling this routine.
#
# Q: why are edges sorted? For quasi-canonical labeling?

{
	my $sg = shift;
	# should updateEdges return an error if a dangling edge is found?  default = YES
	my $dangling_error = (@_) ? shift : 1;
	# should updateEdges trim dangling edges or leave them intact?  default = NO
	my $trim_dangling  = (@_) ? shift : 0;

    #printf STDERR "SpeciesGraph->updateEdges( %s, $dangling_error, $trim_dangling ).. ", $sg->toString();

    # holds error value for return
    my $err = undef;

	# Loop through molecules and components and create map
	#  from edge labels to a list of molecule and component pointers:
	#  i.e.  %labeled_edges: elabel -> [m1, c1, m2, c2 ...]
	my %labeled_edges = ();
	my $i_mol  = 0;
	foreach my $mol ( @{$sg->Molecules} )
	{
	    # Loop over components
		my $i_comp = 0;
		foreach my $comp ( @{$mol->Components} )
		{
		    my @wildcards = ();
		    # loop over edge labels at this component			
			foreach my $elabel ( @{$comp->Edges} )
			{
				if ( $elabel =~ /^[*+?]$/ )
				{   # keep wildcard to put on component edge list
				    push @wildcards, $elabel;
				}
				else
				{   # handle labeled edge
				    push @{$labeled_edges{$elabel}}, $i_mol, $i_comp;
				}
			}

            # clear out edges! and restore wildcards, if any.
            $comp->Edges( [@wildcards] );
			++$i_comp;
		}
		++$i_mol;
	}


	# clear sg edges array
	my $edges = [];
	$sg->Edges($edges);

	# clear adjacency hash
	my $adjacency = {};
	$sg->Adjacency($adjacency);

    # sort edges and reindex from 0
	my $iedge = 0;
	foreach my $edge ( sort edge_sort values %labeled_edges )
	{
		my ($p1, $p2);
		# get molecule and component indices
		my ($m1, $c1, $m2, $c2) = @$edge;
		# create pointer to bond end1
		$p1 = "$m1.$c1";

	    # check for too many components!
	    if ( @$edge > 4 ) 
	    {
	        $err = "SpeciesGraph::updateEdges(): edge binds more than 2 components!";
	    }
		elsif ( @$edge == 4 )
		{
		    # create pointer to bond end2
			$p2 = "$m2.$c2";
			# update adjacency hash
			$adjacency->{$p1}{$p2} = $iedge;
			$adjacency->{$p2}{$p1} = $iedge;
			# add edge label to components
			push @{ $sg->Molecules->[$m1]->Components->[$c1]->Edges }, $iedge;
			push @{ $sg->Molecules->[$m2]->Components->[$c2]->Edges }, $iedge;
			# add edge definition to edge array
			push @$edges, "$p1 $p2";
		    ++$iedge;			
		}
		elsif ( @$edge == 2  and  !$dangling_error )
		{
		    if ($trim_dangling)
		    {   # trim dangling edge
		        # don't increment $iedge counter
		    }
		    else
		    {   # keep dangling edges
			    $adjacency->{$p1} = $iedge;
			    push @{ $sg->Molecules->[$m1]->Components->[$c1]->Edges }, $iedge;
			    push @$edges, "$p1";
		        ++$iedge;			    
		    }
		}
		elsif ( @$edge == 2  and  $dangling_error )
		{
		    $err = "SpeciesGraph::updateEdges(): illegal dangling edge in species graph!";
		}
		else
	    {
		    $err = "SpeciesGraph::updateEdges(): unknown error!";
		}
	}

    #print STDERR "done\n";
	return $err;
}



###
###
###



sub toString
{
	# get this species graph
	my $sg = shift;
	# get arguments
	my $suppress_edge_names = (@_) ? shift : 0;   # if true, egde labels and species attributes are omitted from the string
	my $suppress_attributes = (@_) ? shift : 0;   # if true, species attributes are omitted (use this for Canonical labeling!!)
	
	
	# initialize string
	my $string = '';

	# header
	# NOTE: printing name messes up use of StringExact for hashing species
	unless ($suppress_attributes)
	{
	    if ($sg->Name)
	    {   $string.=$sg->Name;   }
	    
	    if ( $sg->Label )
	    {   $string .= '%' . $sg->Label;   }
    }
	
	if ( $sg->Compartment )
	{   $string .= '@' . $sg->Compartment->Name;   }
	
	if ($string)
	{   $string .= "::";   }

	# attributes
	# (suppression is required for generating canonical labels!)
	unless ( $suppress_edge_names  or  $suppress_attributes )
	{
	    # gather attributes
		my @attr = ();
		if ( $sg->MatchOnce )
		{   push @attr, "MatchOnce";   }
		
		# put additional attributes here!
		
		# write attributes to string
		if (@attr)
		{   $string .= '{' . join( ',', @attr ) . '}';   }
		
		# Handle "Fixed" by prepending '$'
		if ( $sg->Fixed )
		{   $string .= '$';   }
	}

    # write the molecules
	my $imol = 0;
	foreach my $mol ( @{$sg->Molecules} )
	{
		if ($imol) {   $string .= '.';   }
		$string .= $mol->toString( $suppress_edge_names, $sg->Compartment, $suppress_attributes );
		++$imol;
	}

    # write quantifier (if any), unless we're suppressing attributes
	unless ($suppress_attributes)
	{
        if ( $sg->Quantifier )
        {   $string .= $sg->Quantifier;   }
	}
	
	return ($string);
}




###
###
###



sub toStringSSC {
	my $sg                  = shift @_;
	my $suppress_edge_names = (@_) ? shift @_ : 0;

	my $string              = '';
	my $imol = 0;

	# attributes
	if ( $sg->MatchOnce )
    {
		print STDOUT"\n WARNING: SSC does not implement MatchOnce. Though this rule has been translated, ";
		print STDOUT"\n          Please remove any usage of MatchOnce, as otherwise SSC will not compile the rule. ";
		print STDOUT"\n          See .rxn file for more details. ";
	}

    my $checkSameComp;
	foreach my $mol ( @{$sg->Molecules} )
    {
		if ($imol) { $string .= '';	}
		( my $tempstring, $checkSameComp ) = $mol->toStringSSC();
		$string .= $tempstring;
		++$imol;
	}
	unless ( $checkSameComp == 0 ) { return ($string, $checkSameComp); }
	return ( $string, 0 );
}



###
###
###



# this toString is just used in corresponding seed species block.
# As in SSC one only specifies molecules, molecules if they hava a defined states
# Or molecules with bonds.
sub toStringSSCMol
{
	my $sg                  = shift @_;
	my $suppress_edge_names = (@_) ? shift @_ : 0;

	my $string = '';
	my $imol = 0;
	foreach my $mol ( @{$sg->Molecules} )
    {
		if ($imol) { $string .= ''; }
		$string .= $mol->Name;
		$string .= $mol->toStringSSCMol($suppress_edge_names); #Calls toStringSSCMol of Molecule.pm

		++$imol;
	}
	return $string;
}



###
###
###



# NOTE: Doesn't return enclosing contained because it is assumed that this is called from specific
#       class containing a SpeciesGraph, e.g. Species, or Reactants or Products in RxnRule.

sub toXML
{
	my $sg         = shift @_;
	my $indent     = shift @_;
	my $type       = shift @_;
	my $id         = shift @_;
	my $attributes = shift @_;

	my $string = $indent . "<$type";

	# Attributes
	# id
	$string .= " id=\"" . $id . "\"";

	# other attributes
	unless ( $attributes eq '' ) { $string .= ' ' . $attributes; }

	# Label
	if ( $sg->Label )
    {
		$string .= " label=\"" . $sg->Label . "\"";
	}

	# Compartment
	if ( $sg->Compartment )
    {
		$string .= " compartment=\"" . $sg->Compartment->Name . "\"";
	}

	# add support for MatchOnce keyword
	if ( $sg->MatchOnce )
    {
		$string .= ' matchOnce="1"';
	}

	# add support for Fixed
	if ( $sg->Fixed )
    {
		$string .= ' Fixed="1"';
	}

	# add quantifiers
	if ( $sg->Quantifier )
    {
		my ( $relation, $quantity ) = ( $sg->Quantifier =~ /(=|==|<=|>=|<|>)(\d+)/ );
		$string .= ' relation="' . $relation . '" quantity="' . $quantity . '"';
	}

	# Objects contained
	my $indent2 = '  ' . $indent;
	my $ostring = '';

	# Molecules
	if ( @{$sg->Molecules} )
    {
		$ostring .= $indent2 . "<ListOfMolecules>\n";
		my $index = 1;
		foreach my $mol ( @{$sg->Molecules} )
        {
			$ostring .= $mol->toXML( "  " . $indent2, $id, $index );
			++$index;
		}
		$ostring .= $indent2 . "</ListOfMolecules>\n";
	}

	# Bonds
	if ( @{$sg->Edges} )
    {
		my $bstring = '';
		my $index   = 1;
		my $indent3 = '  ' . $indent2;
		foreach my $edge ( @{$sg->Edges} )
        {
			my ($p1, $p2) = split ' ', $edge;
            next unless (defined $p2); # Only print full bonds; half-bonds handled by BindingState variable in Components
			my $bid = sprintf "${id}_B%d", $index;
			$bstring .= $indent3 . "<Bond";
			$bstring .= " id=\"" . $bid . "\"";
			$bstring .= " site1=\"" . $sg->p_to_label( $p1, $id ) . "\"";
			$bstring .= " site2=\"" . $sg->p_to_label( $p2, $id ) . "\"";
			$bstring .= "/>\n";
			++$index;
		}
		if ($bstring)
        {
			$ostring .=   $indent2 . "<ListOfBonds>\n"
			            . $bstring
			            . $indent2 . "</ListOfBonds>\n";
		}
	}

	# Termination
	if ($ostring)
    {
		$string .= ">\n";                    # terminate tag opening
		$string .= $ostring;
		$string .= $indent . "</$type>\n";
	}
	else
    {
		$string .= "/>\n";                   # short tag termination
	}

	return $string;
}




###
###
###



sub addEdge
{
	my $sg    = shift @_;
	my $ename = shift @_;

	foreach my $pmc (@_)
    {
		my ( $im, $ic ) = split( '\.', $pmc );
		push @{ $sg->Molecules->[$im]->Components->[$ic]->Edges }, $ename;
	}

	return $sg;
}




###
###
###



sub deleteEdge
{
	my ($sg, $p1, $p2) = @_;

	return unless exists $sg->Adjacency->{$p1}{$p2};
	my $ename = $sg->Adjacency->{$p1}{$p2};

	my $ndrop = 0;
	foreach my $p ( $p1, $p2 )
    {
		my ($im, $ic) = split '\.', $p;
		my $cedges = $sg->Molecules->[$im]->Components->[$ic]->Edges;
		foreach my $ie (0 .. $#$cedges)
        {
			if ( $cedges->[$ie] eq $ename )
            {
				splice @$cedges, $ie, 1;
				++$ndrop;
				last;
			}
		}
	}

	unless ( $ndrop == 2 )
    {
		exit_error("deleteEdge acted $ndrop times instead of the correct 2");
	}

	return $sg;
}



###
###
###



# NOTE: the original findConnected was removed
#  and replaces with findConnected2.  --Justin, 25mar2011

# Modified version that returns n_subgraphs
# and single array containing index of subgraph
# to which molecule with each index corresponds
sub findConnected
{
	my $sg = shift @_;

	my @subgraph   = (0) x @{$sg->Molecules};
	my $n_subgraph = 0;
	while (1)
    {
		# Find first molecule that hasn't been included in a subgraph
		my $imol_start = -1;
		foreach my $imol ( 0 .. $#subgraph )
        {
			if ( $subgraph[$imol] == 0 )
            {
				$imol_start = $imol;
				last;
			}
		}
		last if ( $imol_start < 0 );
		++$n_subgraph;

		# Start search from first unreached molecule
		$subgraph[$imol_start] = $n_subgraph;
		my @mol_new = ($imol_start);
		while (@mol_new)
        {
			my @mol_next = ();

			# Loop over molecules in mol_new
			foreach my $imol (@mol_new)
            {
				my $mol = $sg->Molecules->[$imol];

				# Loop over components in molecule
				for ( my $icomp=0;  $icomp < @{$mol->Components};  ++$icomp )
                {
					my $p = "$imol.$icomp";

					# Loop over edges from component
					foreach my $q ( keys %{$sg->Adjacency->{$p}} )
                    {
						my ($im, $ic) = split '\.', $q;
						if ( $subgraph[$im] == 0 )
                        {
							$subgraph[$im] = $n_subgraph;
							push @mol_next, $im;
						}
					}
				}
			}
			@mol_new = @mol_next;
		}
	}
	return ($n_subgraph, [@subgraph]);
}



###
###
###



# check the graph is connected!
sub isConnected
{
    my $sg = shift;
    
    # find the connected component
    my $connected_component = {};
    $sg->depthFirstMoleculeSearch( 0, $connected_component );
   
    for ( my $imol = 0;  $imol < @{$sg->Molecules};  ++$imol )
    {
        return 0  unless ( exists $connected_component->{$imol} );
    }

    return 1;
}


###
###
###


# Split graph into connected components.
# Pass an optional array reference to forces molecule i into component map[i].
# (Returns an error if forced mapping introduces a complex).
# NOTE: this method does create canonical labels.
sub splitConnectedComponents
{
    my $sg = shift @_;
    # force a subset of molecules to map to specific patterns
    my $map_imol_to_ipatt = (@_) ? shift @_ : undef;
     
    # Do some error checking
    my $n_patt;
    if ( defined $map_imol_to_ipatt )
    {
        unless ( @$map_imol_to_ipatt == @{$sg->Molecules} )
        {   # error if map size isn't equal to number of molecules
            return "SpeciesGraph->splitConnectedComponents: ERROR! map size is not equal to number of molecules", [];
        }
        my $max_ipatt = -1;
        foreach my $ipatt ( @$map_imol_to_ipatt )
        {
            next unless (defined $ipatt);
            if ($ipatt > $max_ipatt) { $max_ipatt = $ipatt; }
        }
        $n_patt = ($max_ipatt == -1) ? 0 : $max_ipatt + 1;
    }
    else
    {   # no map specified, create an undefined map
        $map_imol_to_ipatt = [(undef) x @{$sg->Molecules}];
        $n_patt = 0;
    }       
 
    # initialize vector of species graphs (speed optimization)
    my $sgs = [ (undef) x $n_patt ];


    # Find all connected components: 
    #   Loop over molecules and do a breadth first search for connected molecules
    my $visited = {};
    for ( my $imol = 0;  $imol < @{$sg->Molecules};  ++$imol )
    {
        # skip molecule if already visited
        next if ( exists $visited->{$imol} );
    
        # find the connected component
        my $connected_component = [];
        $sg->breadthFirstMoleculeSearch( $imol, $connected_component, $visited );

        # loop over connected component and see if any of them have
        #  a pre-assigned pattern index
        my $ipatt = undef;
        foreach my $imol2 ( @$connected_component )
        {
            if ( defined $ipatt  and  defined $map_imol_to_ipatt->[$imol2] )
            {   # make sure there's no conflict
                if ( $ipatt != $map_imol_to_ipatt->[$imol2] )
                {
                    return "SpeciesGraph->splitConnectedComponents: ERROR!!  Connected molecules mapped to different patterns ", [];
                }
            }
            elsif ( defined $map_imol_to_ipatt->[$imol2] )
            {
                $ipatt = $map_imol_to_ipatt->[$imol2];
            }
        }
        
        # if ipatt isn't defined, get the next index available
        unless ( defined $ipatt )
        {
            $ipatt = $n_patt;
            ++$n_patt;
        }
        
        # create speciesGraph, if not already created
        unless ( defined $sgs->[$ipatt] ) {  $sgs->[$ipatt] = SpeciesGraph::new();  }

        # add molecules to the SpeciesGraph
        foreach my $imol2 ( sort {$a<=>$b} @$connected_component )
        {   
            # map imol to ipatt
            $map_imol_to_ipatt->[$imol2] = $ipatt;
            # add imol to species graph
            push @{$sgs->[$ipatt]->Molecules}, $sg->Molecules->[$imol2];
        }
        # Don't finalize SpeciesGraph!  We might add more molecules later!
    }

    # tell original SpeciesGraph to forget about its molecules
    # (just to make sure no one tampers with them)
    @{$sg->Molecules} = ();
    
    # make sure all the products are defined
    #  then build adjacency hash and edge array
    my $ipatt = 0;   
    foreach my $sg2 ( @$sgs )
    {
        unless (defined $sg2)
        {
            return "SpeciesGraph->splitConnectedComponents: ERROR!! No molecules corresponding to SpeciesGraph $ipatt", [];
        }

	    # Put SpeciesGraph in canonical order
	    my $err = $sg2->updateEdges();
		if ($err)
    	{
		    return "SpeciesGraph->splitConnectedComponents: ERROR!! some problem in updateEdges:\n" . $sg2->toString() . "\n", [];
		}
        ++$ipatt;
    }
    
    # return
    return undef, $sgs;
}          



###
###
###



# search through a complex for molecules that satify a condition
sub breadthFirstMoleculeSearch
{
    my $sg        = shift;    # this SpeciesGraph
    my $root_mol  = shift;    # index of molecule we're starting at
    my $found     = (@_) ? shift : [];   # array of molecules found that satisfy our condition
    my $visited   = (@_) ? shift : {};   # hashmap of molecules that we've visited
    # by default, collect every molecule we encounter
    my $condition = (@_) ? shift : sub { return 1; };
    
    # initialize a molecule queue
    my @mol_queue = ($root_mol);
    
    # process until until empty
    while ( @mol_queue )
    {
        # get next molecule on the queue
        my $imol = shift @mol_queue;
        # skip molecule if already visited
        next if ( exists $visited->{$imol} );   
        # remember that we visited this molecule
        $visited->{$imol} = 1;

        # does this molecule satisfy the condition?
        push @$found, $imol  if ($condition->($imol));
   
        # search through components and look at adjacent molecules
        my $mol = $sg->Molecules->[$imol];
        for ( my $icomp = 0;  $icomp < @{$mol->Components};  ++$icomp )
        {
            my $ptr = "$imol.$icomp";
            foreach my $ptr2 ( keys %{$sg->Adjacency->{$ptr}} )
            {
                my ($imol2,$icomp2) = split /\./, $ptr2;
                push @mol_queue, $imol2;
            }
        }        
    } 
}



###
###
###



# search through a complex for molecules that satify a condition
sub depthFirstMoleculeSearch
{
    my $sg    = shift;    # this SpeciesGraph
    my $imol  = shift;    # index of molecule we're looking at
    my $found     = (@_) ? shift : {};   # hashmap of molecules found that satisfy our condition
    my $visited   = (@_) ? shift : {};   # hashmap of molecules we've already visited
    # by default, collect every molecule we encounter
    my $condition = (@_) ? shift : sub { return 1; };

    # don't visit a molecule twice
    return if ( exists $visited->{$imol} );
    $visited->{$imol} = 1;
    
    # add this molecule to 'found' if it satifies 'condition'
    $found->{$imol} = 1  if ( $condition->($imol) );
    
    # search through components and look at adjacent molecules
    my $mol = $sg->Molecules->[$imol];
    for ( my $icomp = 0;  $icomp < @{$mol->Components};  ++$icomp )
    {
        my $ptr = "$imol.$icomp";
        foreach my $ptr2 ( keys %{$sg->Adjacency->{$ptr}} )
        {
            my ($imol2,$icomp2) = split /\./, $ptr2;
            $sg->depthFirstMoleculeSearch( $imol2, $found, $visited, $condition );
        }
    }
}



###
###
###



# Determine if SpeciesGraph is isomorphic to another SpeciesGraph
# Assume that molecules have already been sorted by molecule name and component state
# using cmp_molecule and cmp_component and cmp_edge
#
# TODO: make sure isomorphicTo works correctly on patterns!
sub isomorphicTo
{
	my ($sg1, $sg2) = @_;

    # inequality of StringIDs is sufficient for rejecting isomorphism!
	if (      defined $sg1->StringID
	     and  defined $sg2->StringID
	     and  ($sg1->StringID cmp $sg2->StringID)  )
	{
		#print "[not isomorphic] failed string test: ", $sg1->StringID, " vs ", $sg2->StringID, "\n";
		return 0;
	}

	# equality of StringExacts is sufficient for accepting isomorphism!
	if (      defined $sg1->StringExact
	     and  defined $sg2->StringExact
	     and  !($sg1->StringExact cmp $sg2->StringExact)  )
    {
		#print "[isomorphic] passed exact match: ", $sg1->StringExact, " vs ", $sg2->StringExact, "\n";
		return 1;
	}

	# Nested depth first search, first molecules, then components to find match
	my $molecules1 = $sg1->Molecules;
	my $molecules2 = $sg2->Molecules;
	my $nmol   = $#$molecules1;

	my @maps   = ();
	my @mused  = (0) x @$molecules1;
	my @mptr   = (0) x @$molecules1;
	my @cptrs  = ();
	my @cuseds = ();

	my $im1  = 0;
	my ($im2, $ic1, $ic2);

	my $components1 = $molecules1->[$im1]->Components;
    my $components2;
    my $ncomp = $#$components1;

	my $adj1 = $sg1->Adjacency;
	my $adj2 = $sg2->Adjacency;

	# depth first search over Molecules
  MITER:
	while (1)
	{
		# find a match at the current level
		# Currently loop is done over all possible molecules, but this could be
		# changed to loop over molecules adjacent to molecules higher level to
		# limit search.
		my $mmatch = 0;

		for ( $im2 = $mptr[$im1];  $im2 <= $nmol;  ++$im2 )
		{
			next if $mused[$im2];    # Continue if this molecule already mapped
			next if ( $molecules1->[$im1]->Name  cmp  $molecules2->[$im2]->Name );
            if ( defined $molecules1->[$im1]->Compartment )
            {   # compartment defined for mol1, now compare to mol2
                next unless ( defined $molecules2->[$im2]->Compartment );
                next unless ( $molecules1->[$im1]->Compartment == $molecules2->[$im2]->Compartment );
            }
            elsif ( defined $molecules2->[$im2]->Compartment )
            {   # compartment defined for mol2, but not mol1
                next;
            }

            # skip if different number of components (this can happen when comparing patterns)
            next unless ( @{$molecules1->[$im1]->Components} == @{$molecules2->[$im2]->Components} );

			# Initialize data for component match at this level
			$mptr[$im1]   = $im2;
			$components2  = $molecules2->[$im2]->Components;
			$cptrs[$im1]  = [ (0) x @$components1 ];
			$cuseds[$im1] = [ (0) x @$components2 ];
			$ic1          = 0;
			$mmatch       = 1;
			last;
		}

		# Move up a level (to last component of molecule at previous level)
		# if no match molecules found
		if ( $mmatch == 0 )
		{
			last MITER if ( $im1 == 0 );

			# Reset molecule pointer at current level
			$mptr[$im1] = 0;
			--$im1;
			$components1 = $molecules1->[$im1]->Components;
			$ncomp       = $#$components1;
			$ic1         = $ncomp;
			$im2         = $mptr[$im1];
			++$cptrs[$im1][$ic1];
			$mused[ $mptr[$im1] ] = 0;
			$components2 = $molecules2->[$im2]->Components;
		}

        # The CITER loop tries to access undefined values if there are 0 components..
        #  in which case we can just skip CITER.
        
        unless ( @{$molecules1->[$im1]->Components} == 0 )
        {
	        # Do depth first search over components of molecule 2
	        my $cptr  = $cptrs[$im1];
	        my $cused = $cuseds[$im1];
          CITER:
	        while (1)
	        {
		        my $cmatch = 0;

		        for ( $ic2 = $cptr->[$ic1];  $ic2 <= $ncomp;  ++$ic2 )
                {
			        next if $cused->[$ic2];
			        if ( $components1->[$ic1]->compare_local( $components2->[$ic2] ) )
                    {   next;   }

			        #Check component edges
			        my $ematch = 1;
			        my $p1 = "$im1.$ic1";
			        my $p2 = "$im2.$ic2";
		          EDGE:
			        foreach my $q1 ( keys %{$adj1->{$p1}} )
                    {
				        my ($jm1, $jc1) = split '\.', $q1;
				        next if ( $jm1 > $im1 );
				        if ( $jm1 == $im1 )
                        {
					        next if ( $jc1 >= $ic1 );
                            #	  exit_error("isomorphicTo can't handle bonds among components of same molecule");
				        }
				        my $q2 = "$mptr[$jm1].$cptrs[$jm1][$jc1]";
				        unless ( defined $adj2->{$p2}{$q2} )
                        {
					        $ematch = 0;
					        last EDGE;
				        }
			        }
			        next unless ($ematch);

			        $cptr->[$ic1] = $ic2;

			        # Complete mapping of this molecule if $ic1==$ncomp
			        if ( $ic1 == $ncomp )
			        {
				        $cmatch = 1;
				        last;
			        }
			        else
			        {
				        # descend to next component
				        $cused->[$ic2] = 1;
				        ++$ic1;
				        #	print "ic1=$ic1 cptr=$cptr[$ic1] ncomp=$ncomp\n";
				        next CITER;
			        }
		        }

		        # Move up a component level if no match found
		        if ( $cmatch == 0 )
		        {
			        # Move to next molecule at current level if up exhausted
			        # component search
			        if ( $ic1 == 0 )
			        {
				        # Increment molecule pointer at current level
				        ++$mptr[$im1];
				        next MITER;
			        }

			        # Reset component pointer at current level
			        $cptr->[$ic1] = 0;
			        --$ic1;
			        $cused->[ $cptr->[$ic1] ] = 0;    # Reset pointers at new level
			        ++$cptr->[$ic1];

			        #      print "set ic1 to $ic1 cptr is $cptr[$ic1]\n";
			        next CITER;
		        }
		        last CITER;
	        }
        }

		# If $im1==$nmol, then graphs are isomorhpic and we can return
		if ( $im1 == $nmol ) { return 1; }

		# Move down a level in molecules (increment $im1)
		$mused[$im2] = 1;
		++$im1;
		$components1 = $molecules1->[$im1]->Components;
		$ncomp       = $#$components1;
	}

	return 0;
}



###
###
###



# Determine if SpeciesGraph is isomorphic
# to a portion of another SpeciesGraph
sub isomorphicToSubgraph
{   
	my $sg1       = shift @_;

	my $MatchOnce  = $sg1->MatchOnce;
	my $molecules1 = $sg1->Molecules;
 	my $edges1     = $sg1->Edges;
	my $adj1       = $sg1->Adjacency;

	my @maps = ();

  GRAPH:
    while ( my $sg2 = shift @_ )
    {
		my $molecules2 = $sg2->Molecules;

		# Number of molecules
		if ( scalar @$molecules1 > scalar @$molecules2 )
        {   next;   }

		# Check that number of edges is same
		my $edges2 = $sg2->Edges;
		if ( scalar @$edges1 > scalar @$edges2 )
        {   next;   }

		# If sg1 has species compartment, check that sg2 matches
		if ( defined $sg1->Compartment )
        {
            next unless ( defined $sg2->Compartment );
            next unless ( $sg1->Compartment == $sg2->Compartment );
        }


		# Nested depth first search, first molecules, then components to find match
		my $nmol1  = $#$molecules1;
		my $nmol2  = $#$molecules2;

		my @mptr   = (0) x @$molecules1;
		my @mused  = (0) x @$molecules2;

		my @cptrs  = ();
		my @cuseds = ();

		my $im1 = 0;
        my $im2;

		my $components1 = $molecules1->[$im1]->Components;
        my $components2;

        my $ncomp1 = $#$components1;
        my $ncomp2;

		my ($ic1, $ic2);

		my $adj2 = $sg2->Adjacency;

		# depth first search over Molecules
	  MITER:
		while (1)
        {
		    # find a match at the current level
		    # Currently loop is done over all possible molecules, but this could be
		    # changed to loop over molecules adjacent to molecules higher level to
		    # limit search.
			my $mmatch = 0;
			for ( $im2 = $mptr[$im1] ; $im2 <= $nmol2 ; ++$im2 )
            {
				next if $mused[$im2]; # Continue if this molecule already mapped
				my $namestring = $molecules1->[$im1]->Name;
				$namestring =~ s/\*$/.*/;

				#print "namestring=$namestring\n";
				next unless ( $molecules2->[$im2]->Name =~ /^${namestring}$/ );

				#next if ($molecules1[$im1]->Name cmp $molecules2[$im2]->Name);
				if ( $molecules1->[$im1]->Compartment )
                {
					next unless ( $molecules1->[$im1]->Compartment == $molecules2->[$im2]->Compartment );
				}

	            #print "$im1 $im2: $molecules1[$im1]->Name -> $molecules2[$im2]->Name\n";;
	             # Initialize data for component match at this level
				$mptr[$im1]   = $im2;
				$components2  = $molecules2->[$im2]->Components;
				$cptrs[$im1]  = [ (0) x @$components1 ];
				$cuseds[$im1] = [ (0) x @$components2 ];
				$ic1 = ( $ncomp1 >= 0 ) ? 0 : -1;
				$mmatch = 1;
				last;
			}

			# Move up a level (to last component of molecule at previous level)
			# if no match molecules found
			if ( $mmatch==0 )
            {
				last MITER if ( $im1 == 0 );

				# Reset molecule pointer at current level
				$mptr[$im1] = 0;
				--$im1;
				$components1          = $molecules1->[$im1]->Components;
				$ncomp1               = $#$components1;
				$ic1                  = $ncomp1;
				$im2                  = $mptr[$im1];
				$mused[ $mptr[$im1] ] = 0;
				if ( $ic1 >= 0 )
                {
                    ++$cptrs[$im1][$ic1];
                }
				else
                {
                    ++$mptr[$im1];
                    next MITER;
				}
				$components2 = $molecules2->[$im2]->Components;
			}

			# Do depth first search over components of molecule 2
			my $cptr  = $cptrs[$im1];
			my $cused = $cuseds[$im1];
		  CITER:
			while (1)
            {
                my $cmatch;
				if ( $ncomp1 >= 0 )
                {
                    my ($ci1, $ci2);
					$ci1    = $components1->[$ic1];
					$ncomp2 = $#$components2;
					$cmatch = 0;
					for ( $ic2 = $cptr->[$ic1];  $ic2 <= $ncomp2;  ++$ic2 )
                    {
						next if $cused->[$ic2];
						$ci2 = $components2->[$ic2];

						# Component name
						next if ( $ci1->Name cmp $ci2->Name );

						# Component state only if present in sg1
						if ( defined $ci1->State )
                        {
							if ( $ci1->State =~ /[*+?]/ )
                            {
								if ( $ci1->State eq '+' )
                                {
									next if ( $ci2->State eq '' );
								}
							}
							else
                            {
								next if ( $ci1->State cmp $ci2->State );
							}
						}
                        # compare compartments
						if ( defined $ci1->Compartment )
                        {
                            next unless ( defined $ci2->Compartment );
							next unless ( $ci1->Compartment == $ci2->Compartment );
						}

		                # Number of component edges must match (primarily used to look for free
		                # binding sites
		                # Number of edges
						my $diff = @{$ci2->Edges} - @{$ci1->Edges};
						if ($diff)
                        {
							# Mismatch unless first Edge is wildcard
                            if ( @{$ci1->Edges} )
                            {
							    my $wild = $ci1->Edges->[0];
							    next unless ( $wild =~ /^[*+?]$/ );

						        # + wildcard requires $diff>=0 (= case handled above)
							    if ( $wild eq '+' )
                                {
								    next unless ( $diff > 0 );
							    }
							    else
                                {   # *? (equivalent) wildcard requires $diff>-1, #c2 edges >= #c1 edges - 1 (for wildcard)
								    next unless ( $diff >= -1 );
							    }
                            }
                            else
                            {
                                next;
                            }
						}

						#Check component edges
						my $ematch = 1;
						my $p1 = "$im1.$ic1";
						my $p2 = "$im2.$ic2";
                        if (ref $adj1->{$p1} eq 'HASH')
    					{
                          EDGE:
                            foreach my $q1 ( keys %{$adj1->{$p1}} )
                            {
							    my ($jm1, $jc1) = split /\./, $q1;
							    next if ( $jm1 > $im1 );
							    if ( $jm1 == $im1 )
                                {
								    next if ( $jc1 >= $ic1 );
							    }
							    my $q2 = "$mptr[$jm1].$cptrs[$jm1][$jc1]";
							    unless ( (ref $adj2->{$p2} eq 'HASH') and (defined $adj2->{$p2}{$q2}) )
                                {
								    $ematch = 0;
								    last EDGE;
							    }
						    }
                        }
						next if ($ematch==0);

						$cptr->[$ic1] = $ic2;

						# Complete mapping of this molecule if $ic1==$ncomp1
						if ( $ic1 == $ncomp1 )
                        {
							$cmatch = 1;
							last;
						}
						else
                        {   # descend to next component
							$cused->[$ic2] = 1;
							++$ic1;
							next CITER;
						}
					}
				}
				else
                {   # No components in pattern
					$cmatch = 1;
				}

				# Move up a component level if no match found
				if ( $cmatch==0 )
                {   # Move to next molecule at current level if up exhausted
					# component search
					if ( $ic1 <= 0 )
                    {
						# Increment molecule pointer at current level
						++$mptr[$im1];
						next MITER;
					}

					# Reset component pointer at current level
					$cptr->[$ic1] = 0;
					--$ic1;
					$cused->[ $cptr->[$ic1] ] = 0;    # Reset pointers at new level
					++($cptr->[$ic1]);
					next CITER;
				}

				# If $im1==$nmol, then graphs are isomorhpic and we can return
				# Modify to save map for subgraph isomorphism case
				if ( $im1 == $nmol1 )
                {
					my $map = Map->new;
					$map->Source($sg1);
					$map->Target($sg2);
					my %mapf = ();
					foreach my $im ( 0 .. $nmol1 )
                    {
						my $im2 = $mptr[$im];
						$mapf{$im} = $im2;
						my $cptr = $cptrs[$im];

						foreach my $ic ( 0 .. $#$cptr )
                        {
							$mapf{"$im.$ic"} = sprintf "%d.%d", $im2, $cptr->[$ic];
						}
					}
					$map->MapF( {%mapf} );
					push @maps, $map;

					if ($MatchOnce)
                    {   next GRAPH;   }

					if ( $ncomp1 >= 0 )
                    {
						++($cptr->[$ic1]);
						next CITER;
					}
					else
                    {
				        # Go to next molecule if no components in the current molecule
						++$mptr[$im1];
						next MITER;
					}
				}
				last CITER;
			}

			# Move down a level in molecules (increment $im1)
			$mused[$im2] = 1;
			++$im1;
			$components1 = $molecules1->[$im1]->Components;
			$ncomp1      = $#$components1;
		}
	}
	
	return @maps;
}



###
###
###



sub getIdentityMap
{
    # get input arguments
    my $sg = shift;

    # initialize map hashes
    my $mapF = {};
    my $mapR = {};
    
    # create map object
	my $map = Map::new();
	$map->Source($sg);
	$map->Target($sg);    
    $map->MapF( $mapF );
    $map->MapR( $mapR );
    
    # build identity maps
    my $im = 0;
    foreach my $mol ( @{$sg->Molecules} )
    {
        $mapF->{$im} = $im;
        $mapR->{$im} = $im;
        
        my $ic = 0;
        foreach my $comp ( @{$mol->Components} )
        {
            my $ptr = "$im.$ic";
            $mapF->{$ptr} = $ptr;
            $mapR->{$ptr} = $ptr;
            ++$ic;
        }
        ++$im;
    }
    
    return $map;
}


###
###
###



sub findMaps

# (Map) = SpeciesGraph1->findMaps(SpeciesGraph2)
#
# NOTE:  This method was originally called "findMaps2".  Since findMaps was
#  deprecated, it was removed from the code based and findMaps2 was promoted to
#  findMaps!
#
# A simplified replacement for findMaps. Finds mapping by first labeling the
# molecules and components and building a map from object labels to the object index
# w.r.t. the species graph.  By matching labels in two species graphs,
# the forward and/or reverse map can be generated (map from pointer index to pointer index).
#
# should this be in Map module?
# REVISED by justinshogg@gmail.com 19feb2009
# TODO: findMaps respects molecule and component tags, but does not attempt to reconile Pattern tags.
{

	# get species graphs
	my ( $sg1, $sg2 ) = @_;

	# for each speciesGraph, build a map from object labels to object indices
	my $labelmap1 = $sg1->buildLabelMap;
	my $labelmap2 = $sg2->buildLabelMap;

	# create and setup a new Map object
	my $map = Map->new;
	$map->Source($sg1);
	$map->Target($sg2);
	$map->MapF( buildPointerMap( $labelmap1, $labelmap2 ) );
	$map->MapR( buildPointerMap( $labelmap2, $labelmap1 ) );

	# all done, return map (in a list, for deprecated reasons)
	return $map;


	sub buildPointerMap
	  # pointermap = buildPointerMap( labelmap1, labelmap2 );
	  # DESCR: given 2 label maps, returns a map from label indices in $lmap1
	  #  to label indices in $lmap2.
	  # NOTES: the pointer map is a hash (not a Map object)
	{
		my ( $lmap1, $lmap2 ) = @_;
		my $pmap = {};
		foreach my $label ( keys %$lmap1 )
		{
			# map label index1 to label index2, or -1 if index2 is not defined
			$pmap->{ $lmap1->{$label} } =
			  ( exists $lmap2->{$label} ) ? $lmap2->{$label} : -1;
		}
		return $pmap;
	}
}



###
###
###


# What Happened to findMaps2??
#   In revision 354 (distribution 2.1.8+), the original and deprecated findMaps was eliminated from
#   the code and findMaps2 was promoted to findMaps!  For the old code, see the
#   repository or any distribution prior to 2.1.8   --Justin, 16 Dec 2010


###
###
###



sub buildLabelMap

  # labelmap = SpeciesGraph->buildLabelMap()
  # this outsources the labeling loop from findMaps
  # returns a map from labels to indices
{
	my $sg       = shift;    # species graph
	my $labelmap = {};       # initialize map from labels to indices
	my %labels   = ();       # a map of labels  (up to replicate index)
	                         #  to the number of objects with that label.

	# label molecules
	for ( my $im = 0 ; $im < @{$sg->Molecules} ; ++$im )
	{
		my $mol     = $sg->Molecules->[$im];    # molecule to be labeled
		my @clabels = ();                       # list of component labels
		my $mlabel;                             # molecule label string

		# Get component labels (substitute name, if no label)
		foreach my $comp ( @{ $mol->Components } )
		{
			if   ( my $clabel = $comp->Label ) { push @clabels, '%' . $clabel; }
			else                               { push @clabels, $comp->Name; }
		}

		# User provided label supercedes other labeling
		# It must be unique for each molecule and
		# each componentl
		if ( $mlabel = $mol->Label ) { $mlabel = '%' . $mlabel; }
		else
		{   # Automatic mol label starts with molname
			$mlabel = $mol->Name;
			$mlabel .= '_' . join( '_', sort @clabels ) . '_';
			$mlabel .= ++$labels{$mlabel};
		}

		# write map:  molecule label -> molecule index
		$labelmap->{$mlabel} = $im;

		# Component labels have syntax mlabel|clabel.  If user label is
		# provided, it has precedence and mlabel is omitted. Otherwise, $clabel
		# is name of component plus a number that indicates its order in
		# occurence of identical components
		for ( my $ic = 0 ; $ic < @clabels ; $ic++ )
		{
			my $clabel = $clabels[$ic];
			unless ( $clabel =~ /^\%/ )
			{
				# Automatic label
				$clabel = $mlabel . '|' . $clabel . '_';
				$clabel .= ++$labels{$clabel};
			}
			$labelmap->{$clabel} = "$im.$ic";
		}
	}

	return $labelmap;
}



###
###
###



# Canonical order for molecules
sub cmp_molecule
{
	my $a = shift;
	my $b = shift;
	    
	my $cmp;
	# compare molecule names
	if ( $cmp = ( $a->Name cmp $b->Name ) )
	{   return $cmp;   }
	
	# compare number of components
	if ( $cmp = (@{$a->Components} <=> @{$b->Components}) )
	{   return $cmp;   }

	# compare by compartment names
	if ( defined $a->Compartment )
	{
		unless ( defined $b->Compartment ) { return 1 };
		if ( $cmp = ($a->Compartment->Name cmp $b->Compartment->Name) )
		{   return $cmp;   }
	}
	elsif ( defined $b->Compartment )
	{
		return -1;
	}

    # compare by components
    for ( my $ic = 0;  $ic < @{$a->Components};  ++$ic )
	{
		if ( $cmp = cmp_component($a->Components->[$ic], $b->Components->[$ic]) )
		{   return $cmp;   }
	}
	
	# no difference found
	return 0;
}

##
##

sub by_molecule
{
	return cmp_molecule( $a, $b );
}



###
###
###



# Canonical order for components of molecules
sub cmp_component
{
	my $a = shift;
	my $b = shift;

	my $cmp;

	# Lexical comparison of name
	if ( $cmp = ($a->Name cmp $b->Name) )
	{   return $cmp;   }

	# Lexical comparison of state labels
    if ( defined $a->State )
    {
        unless ( defined $b->State ) { return 1; }
	    if ( $cmp = ($a->State cmp $b->State) )
	    {   return $cmp;   }
    }
    elsif ( defined $b->State )
    {
        return -1;
    }

	# Comparison of number of edges
	# NOTE: the usual order of a and b are switched!!
	#  so the components with more edges are before components with fewer edges
	if ( $cmp = ( @{$b->Edges} <=> @{$a->Edges} ) )
	{   return $cmp;   }

	# Comparison of edges
	#  for my $i (0..$#a_edges){
	#    if ($cmp=($a_edges[$i] cmp $b_edges[$i])){
	#      return($cmp);
	#    }
	#  }

    # no difference found
	return 0;
}

##
##

sub by_component
{
	return cmp_component($a, $b);
}




###
###
###



sub cmp_edge
{
	my ($a,$b) = @_;
	
	my $cmp;
    # split pointers
	my (@a_p) = split /[ \.]/, $a;
	my (@b_p) = split /[ \.]/, $b;
    # dangling edges (2 elements in array) should be "greater than" true edges (4 elements in array)
    if ($cmp = (@b_p <=> @a_p)) { return $cmp; }
    # if both edges are the same true, compare element by element..
	foreach my $i ( 0 .. $#a_p )
	{
		if ( $cmp = ($a_p[$i] <=> $b_p[$i]) )
		{   return $cmp;   }
	}
	# Getting here means edges are identical, which shouldn't happen.
	return 0;
}

##
##

sub by_edge
{
	return cmp_edge($a, $b);
}




###
###
###




# This sub assumes edges are stored in array references like this:
#   true edges = [m1, c1, m2, c2]
#   dangling edges = [m1, c1]
#  --Justin, 17 dec 2010
#
# Should be a little faster since there's no splitting here!

sub edge_sort
{
	# $a, $b arguments should be array references
	my $cmp;
    # dangling edges (2 elements in array) should be "greater than" true edges (4 elements in array)
    if ($cmp = (@$b <=> @$a)) { return $cmp; }
    # if both edges are the same true, compare element by element..
	for ( my $i=0;  $i < @$a;  ++$i )
	{
		if ( $cmp = ($a->[$i] <=> $b->[$i]) ) { return $cmp; }
	}
	# Getting here means edges are identical (which shouldn't happen?).
	return 0;
}



###
###
###

1;
