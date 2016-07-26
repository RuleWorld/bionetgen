package BNGModel;


# pragmas
use strict;
use warnings;

#####
# Extracts a hash containg all the species graphs in the system and enumerates them
#####
sub extractAllSpeciesGraphs
{
    my $model  = shift @_;
    my $speciesSet = shift @_;
    my $speciesTypeSet = shift @_;

    my $speciesCounter = 1;
    my $speciesTypeCounter = 1;

    my %print_options = (
        "sbml_multi",  "1",
    );
    foreach my $rset ( @{$model->RxnRules} )
    {
        foreach my $rr ( @$rset )
        {
            foreach my $reactant (@{$rr->Reactants})
            {
                $reactant->labelQuasi();
                if (! defined $speciesSet->{$reactant->StringExact}){
                    my $speciesTypeId = 0;
                    push @{$speciesSet->{$reactant->StringExact}}, $speciesCounter;
                    push @{$speciesSet->{$reactant->StringExact}}, $reactant;
                    # calculate species type and add it to our set as necessary
                    my $sbmlMultiSpeciesTypeStr = $reactant->toString(1, 1, \%print_options);
                    if(exists $speciesTypeSet->{$sbmlMultiSpeciesTypeStr})
                    {
                        $speciesTypeId = $speciesTypeSet->{$sbmlMultiSpeciesTypeStr};
                    }
                    else
                    {
                        push @{$speciesTypeSet->{$sbmlMultiSpeciesTypeStr}}, $speciesTypeCounter;
                        push @{$speciesTypeSet->{$sbmlMultiSpeciesTypeStr}}, $reactant;
                        $speciesTypeId = $speciesTypeCounter;
                        $speciesTypeCounter++;
                    }

                    push @{$speciesSet->{$reactant->StringExact}}, @{$speciesTypeSet->{$sbmlMultiSpeciesTypeStr}}[0];                    
                    $speciesCounter += 1;
                }
            }
            foreach my $product (@{$rr->Products})
            {
                $product->labelQuasi();
                if (! defined $speciesSet->{$product->StringExact}){
                    my $speciesTypeId = 0;
                    push @{$speciesSet->{$product->StringExact}}, $speciesCounter;
                    push @{$speciesSet->{$product->StringExact}}, $product;

                    # calculate species type and add it to our set as necessary
                    my $sbmlMultiSpeciesTypeStr = $product->toString(1, 1, \%print_options);
                    if(exists $speciesTypeSet->{$sbmlMultiSpeciesTypeStr})
                    {
                        $speciesTypeId = $speciesTypeSet->{$sbmlMultiSpeciesTypeStr};
                    }
                    else
                    {
                        push @{$speciesTypeSet->{$sbmlMultiSpeciesTypeStr}}, $speciesTypeCounter;
                        push @{$speciesTypeSet->{$sbmlMultiSpeciesTypeStr}}, $product;
                        $speciesTypeId = $speciesTypeCounter;
                        $speciesTypeCounter++;
                    }

                    push @{$speciesSet->{$product->StringExact}}, @{$speciesTypeSet->{$sbmlMultiSpeciesTypeStr}}[0];                    

                    $speciesCounter += 1;
                }    

            }
        }
    }

}


####
## Walks through the list of species graph in the system and extracts binding components
####
sub extractBindingComponents
{
    my $model  = shift @_;
    my $speciesSet = shift @_;
    my $bindingComponents_ref = shift @_;

    foreach my $speciesStr (keys %{$speciesSet}){
        my $sg = $speciesSet->{$speciesStr}[1];
        foreach my $mol (@{$sg->Molecules}){
            foreach my $comp (@{$mol->Components}){
                if (@{$comp->Edges}){
                    $bindingComponents_ref->{$mol->Name}->{$comp->Name}  = 1;
                }

            }

        }
    }

}

#####
# print sbml and sbml multi parameters and observables
#####
sub writeSBMLParameters
{
    my $model = shift @_;
    my $plist   = $model->ParamList;
    my $SBML = '';

    # 3. Parameters
    # A. Rate constants
    $SBML .= "    <listOfParameters>\n";
    if ($plist->countType('Constant')){
        $SBML .= "      <!-- Independent variables -->\n";
        foreach my $param ( @{$plist->Array} )
        {
            next unless ( $param->Type eq 'Constant' );
            $SBML .= sprintf("      <parameter id=\"%s\" constant=\"false\" value=\"%.8g\"/>\n", $param->Name, $param->evaluate([], $plist));
        }
    }
    if ($plist->countType('ConstantExpression')){
        $SBML .= "      <!-- Dependent variables -->\n";
        foreach my $param ( @{$plist->Array} )
        {
            next unless ( $param->Type eq 'ConstantExpression' );   
            $SBML .= sprintf("      <parameter id=\"%s\" constant=\"true\" value=\"0\" />\n", $param->Name);
        }
    }
    
    # B. Observables
    if ( @{$model->Observables} )
    {
        $SBML .= "      <!-- Observables -->\n";
        foreach my $obs ( @{$model->Observables} )
        {
            $SBML .= sprintf("      <parameter id=\"%s\" constant=\"false\" value=\"0\" />\n", $obs->Name);
        }
    }
    
    # C. Global functions
    if ($plist->countType('Function')){
        $SBML .= "      <!-- Global functions -->\n";
        foreach my $param ( @{$plist->Array} )
        {
            next unless ( $param->Type eq 'Function');
            next if ( @{$param->Ref->Args} ); # Don't print local functions
            $SBML .= sprintf("      <parameter id=\"%s\" constant=\"false\"/>\n", $param->Name);
        }
    }
    
    $SBML .= "    </listOfParameters>\n";
    

    # 3.5. Initial assignments (for dependent variables)
    if ($plist->countType('ConstantExpression')){
        $SBML .= "    <listOfInitialAssignments>\n";
        $SBML .= "      <!-- Dependent variables -->\n";
        foreach my $param ( @{$plist->Array} )
        {
    #       next if ( $param->Expr->Type eq 'NUM' );
            next unless ( $param->Type eq 'ConstantExpression');
            $SBML .= sprintf("      <initialAssignment symbol=\"%s\">\n", $param->Name);
            #print  $SBML "        <notes>\n";
            #print  $SBML "          <xhtml:p>\n";
            #$SBML .= sprintf  "            %s=%s\n", $param->Name,$param->toString($plist);
            #print  $SBML "          </xhtml:p>\n";
            #print  $SBML "        </notes>\n";
            $SBML .= sprintf($param->toMathMLString( $plist, "        " ));
            $SBML .= "      </initialAssignment>\n";
        }
        $SBML .= "    </listOfInitialAssignments>\n";
    }

    return $SBML;
}

# needs to be modified to iterate over rules instead of reactions
sub writeSBMLReactions
{
    use Storable qw(dclone);

    my $model = shift @_;
    my $speciesIdHash_ref = shift @_;
    my $SBML = '';
    my %print_options = (
        "sbml_multi",  "1",
    );

    # 5. Reactions
    $SBML .=  "    <listOfReactions>\n";
    my $index = 0;
    foreach my $rset ( @{$model->RxnRules} )
    {
        foreach my $rxn ( @$rset )
        {
        #keeps track of the species components ids that we will use in this reaction mapping



        ++$index;
        $SBML .= sprintf  "      <reaction id=\"R%d\" reversible=\"false\" fast=\"false\">\n", $index;

        #Get indices of reactants
        my @rindices = ();
        foreach my $spec ( @{$rxn->Reactants} )
        {
            my $rid = $speciesIdHash_ref->{'Species'}->{$spec->StringExact};
            $rid = substr($rid, 1, length($rid));

            push @rindices, $rid #$spec->Index;
        }
        @rindices = sort { $a <=> $b } @rindices;

        #Get indices of products
        my @pindices = ();
        foreach my $spec ( @{$rxn->Products} )
        {
            my $pid = $speciesIdHash_ref->{'Species'}->{$spec->StringExact};
            $pid = substr($pid, 1, length($pid)); #extract the id without the 'S'

            push @pindices, $pid #$spec->Index;

        }
        @pindices = sort { $a <=> $b } @pindices;
        
        #define a reaction hash that contains a speciesIdHash clone corresponding to each reactant in the list
        my %reactantHashHash;
        my $localCounter = 0;

        if (scalar(@rindices) > 0){
            $SBML .=  "        <listOfReactants>\n";
            foreach my $i (@rindices)
            {
                $SBML .= sprintf  "          <speciesReference id=\"RR%d_R%d\" species=\"S%d\" constant=\"false\"/>\n", $index,$localCounter+1, $i;
                $reactantHashHash{$localCounter}{'id'} = sprintf("RR%d_R%d",$index,$localCounter+1);
                $reactantHashHash{$localCounter}{'sg'} = \%{dclone(\%{$speciesIdHash_ref->{'References'}})};
                $localCounter += 1;
            }
            $SBML .=  "        </listOfReactants>\n";
        }

        
        if (scalar(@pindices) > 0){
            $SBML .=  "        <listOfProducts>\n";
            my $dots;
            my $target;
            my $sfirstdot;
            my $tfirstdot;
            my $counter = 0;
            foreach my $i (@pindices)
            {

                my %productHash = %{dclone(\%{$speciesIdHash_ref->{'References'}})};

                $SBML .= sprintf  "          <speciesReference id=\"RR%d_P%d\" species=\"S%d\" constant=\"false\">\n", $index, $counter+1, $i;
                $SBML .= sprintf  "            <multi:listOfSpeciesTypeComponentMapsInProduct>\n";
                #FIXME: If i want to be efficient i should be constructing this outside of the loop and making a 
                #hash containg this information
                foreach my $source ( sort keys %{ $rxn->MapF } )
                {
                    my $dots = $source =~ tr/././; #number of dots
                    if($dots == 1){
                        my $target = $rxn->MapF->{$source};
                        my $sfirstdot = index($source, ".");
                        my $smolecule = substr($source, $sfirstdot+1, length($source));
                        $source = substr($source, 0, $sfirstdot);
                        
                        my $tfirstdot = index($target, ".");
                        my $tmolecule = substr($target, $sfirstdot+1, length($target));
                        $target = substr($target, 0, $tfirstdot);
                        
                        if($target == $counter){
                            #the species graph for the reactant-product graph pattern pair
                            my $reactant = @{$rxn->Reactants}[$source];
                            my $product = @{$rxn->Products}[$target];
                            my $indent4 = "              ";
                            my $sbmlMultiSpeciesTypeStr = $reactant->toString(1, 1, \%print_options);
                            my $psbmlMultiSpeciesTypeStr = $product->toString(1, 1, \%print_options);
                            #get the multi species type id associated with this graph pattern
                            my $rspeciesType = $speciesIdHash_ref->{'SpeciesType'}->{$sbmlMultiSpeciesTypeStr};
                            my $pspeciesType = $speciesIdHash_ref->{'SpeciesType'}->{$psbmlMultiSpeciesTypeStr};

                            #get the species hash associated with this reatant element
                            my $reactantHash = $reactantHashHash{$source}{'sg'};
                            my $reactantId = $reactantHashHash{$source}{'id'};

                            my $reactantName = @{$reactant->Molecules}[$smolecule]->Name;
                            my $productName = @{$product->Molecules}[$tmolecule]->Name;
                            #use Data::Dumper;
                            #print Dumper $reactantHashHash{$source}{'sg'},'...';
                            # fixme: this shouldnt be zero, we should actually index the component we want, important if there are labels that change mapping order
                            # that said, the index here is a $local$ index within the repeated elements of the same molecule type in a species

                            #get the sbml multi id associated with this graph pattern's component id in the species type
                            my $rreverseReference = $reactantHash->{$rspeciesType}{'moleculeReverseReferences'}{$reactantName}[0];
                            my $preverseReference = $productHash{$pspeciesType}{'moleculeReverseReferences'}{$productName}[0];

                            #fixme: once again it is not necessarely the first one that we are removing
                            #we used this component so remove it from the available components pool
                            splice(@{$reactantHash->{$rspeciesType}{'moleculeReverseReferences'}{$reactantName}}, 0, 1);
                            splice(@{$productHash{$pspeciesType}{'moleculeReverseReferences'}{$productName}}, 0, 1);


                            $SBML .= sprintf "${indent4}<multi:speciesTypeComponentMapInProduct multi:reactant=\"$reactantId\" multi:reactantComponent=\"cmp_${rreverseReference}\" multi:productComponent=\"cmp_${preverseReference}\"/>\n";

                        }
                        

                    }
                }
                $counter += 1;
                $SBML .= sprintf  "            </multi:listOfSpeciesTypeComponentMapsInProduct>\n";
                $SBML .= "          </speciesReference>\n";


            }
            $SBML .=  "        </listOfProducts>\n";
        }
        
        $SBML .=  "        <kineticLaw>\n";
        my ( $rstring, $err ) = $rxn->RateLaw->toMathMLString( \@rindices, \@pindices);
        if ($err) { return $err; }

        foreach my $line ( split "\n", $rstring )
        {
            $SBML .=  "          $line\n";
        }
        $SBML .=  "        </kineticLaw>\n";

        $SBML .=  "      </reaction>\n";
    }

    }
    $SBML .=  "    </listOfReactions>\n";
    return $SBML;
}

1;

