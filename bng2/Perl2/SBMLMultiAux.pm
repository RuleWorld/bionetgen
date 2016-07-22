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
            $pid = substr($pid, 1, length($pid));

            push @pindices, $pid #$spec->Index;

        }
        @pindices = sort { $a <=> $b } @pindices;

        if (scalar(@rindices) > 0){
            $SBML .=  "        <listOfReactants>\n";
            foreach my $i (@rindices)
            {
                $SBML .= sprintf  "          <speciesReference species=\"S%d\" constant=\"false\"/>\n", $i;
            }
            $SBML .=  "        </listOfReactants>\n";
        }
        
        if (scalar(@pindices) > 0){
            $SBML .=  "        <listOfProducts>\n";
            foreach my $i (@pindices)
            {
                $SBML .= sprintf  "          <speciesReference species=\"S%d\" constant=\"false\"/>\n", $i;
          # <speciesReference species="sp_cpx_000002" constant="false">
          #   <multi:listOfSpeciesTypeComponentMapsInProduct>
          #     <multi:speciesTypeComponentMapInProduct multi:reactant="spr1_cpx_000001" multi:reactantComponent="st_mol_000001" multi:productComponent="stci_cps_000002_1_mol_000001"/>
          #     <multi:speciesTypeComponentMapInProduct multi:reactant="spr2_cpx_000001" multi:reactantComponent="st_mol_000001" multi:productComponent="stci_cps_000002_2_mol_000001"/>
          #   </multi:listOfSpeciesTypeComponentMapsInProduct>
          # </speciesReference>

    # {
    #     $ostring .= $indent2 . "<Map>\n";
    #     my $index = 1;
    #     foreach my $source ( sort keys %{ $rr->MapF } )
    #     {
    #         $ostring .= $indent3 . "<MapItem";
    #         my $target = $rr->MapF->{$source};
    #         $ostring .=
    #             " sourceID=\"" . pointer_to_ID( $id . "_R", $source ) . "\"";
    #         $target = pointer_to_ID( $id . "_P", $target );
    #         if ( $target ne "Null" ) {
    #             $ostring .= " targetID=\"" . $target . "\"";
    #         }
    #         $ostring .= "/>\n";
    #         ++$index;
    #     }
    #     $ostring .= $indent2 . "</Map>\n";
    # }

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

