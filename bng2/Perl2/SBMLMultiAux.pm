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

1;
