#!/usr/bin/perl

use strict;
use warnings;

# get Module directory
use File::Spec;
use lib File::Spec->catdir("BNGcore");

# load BNGcore module
use BNGcore;

print "testing BNGcore module\n";

# instantiate ancestor state types
my $null_state_type = BNGcore::NullStateType->new();
my $bond_state_type  = BNGcore::BondStateType->new();

my $unbound_state = BNGcore::BondState->new( $bond_state_type, "-" );
my $bound_state   = BNGcore::BondState->new( $bond_state_type, "+" );

my $entity_node_type = BNGcore::NodeType->new( "Entity", $null_state_type );
my $link_node_type   = BNGcore::NodeType->new( "Link", $null_state_type );

my $bond_typing_fcn = BNGcore::BondNodeFunction->new();
my $bond_node_type = BNGcore::BondType->new( $bond_typing_fcn );
  
# type ligand
my $type_L = BNGcore::EntityType->new( "L", $entity_node_type, $null_state_type );
my $type_Lr = BNGcore::EntityType->new( "r", $entity_node_type, $null_state_type );
$type_L->add_edges_out( $type_Lr, 2 );
$type_Lr->add_edges_in( $type_L, 1 );
$type_Lr->add_edges_out( $bond_node_type, 1 );

# build ligand
my $ligand = BNGcore::PatternGraph->new();
# add molecules
my $mol_L    = $ligand->add_node( BNGcore::Node->new($type_L) );
my $comp_Lr1 = $ligand->add_node( BNGcore::Node->new($type_Lr) );
my $bond_Lr1 = $ligand->add_node( BNGcore::Node->new($bond_node_type) );
my $comp_Lr2 = $ligand->add_node( BNGcore::Node->new($type_Lr) );
my $bond_Lr2 = $ligand->add_node( BNGcore::Node->new($bond_node_type) );
# add edges
$ligand->add_edge( $mol_L, $comp_Lr1 );
$ligand->add_edge( $mol_L, $comp_Lr2 );          
$ligand->add_edge( $comp_Lr1, $bond_Lr1  );          
$ligand->add_edge( $comp_Lr2, $bond_Lr2  );
# display molecule
print "Ligand molecule: ", $ligand->get_BNG2_string(), "\n";

# type receptor
my $type_R = BNGcore::EntityType->new( "R", $entity_node_type, $null_state_type );
my $type_Rl = BNGcore::EntityType->new( "l", $entity_node_type, $null_state_type );
$type_R->add_edges_out( $type_Rl, 2 );
$type_Rl->add_edges_in( $type_R, 1 );
$type_Rl->add_edges_out( $bond_node_type, 1 );

# build receptor
my $receptor = BNGcore::PatternGraph->new();
# add molecules
my $mol_R    = $receptor->add_node( BNGcore::Node->new($type_R) );
my $comp_Rl1 = $receptor->add_node( BNGcore::Node->new($type_Rl) );
my $bond_Rl1 = $receptor->add_node( BNGcore::Node->new($bond_node_type) );
my $comp_Rl2 = $receptor->add_node( BNGcore::Node->new($type_Rl) );
my $bond_Rl2 = $receptor->add_node( BNGcore::Node->new($bond_node_type) );
# add edges
$ligand->add_edge( $mol_R, $comp_Rl1 );
$ligand->add_edge( $mol_R, $comp_Rl2 );          
$ligand->add_edge( $comp_Rl1, $bond_Rl1  );          
$ligand->add_edge( $comp_Rl2, $bond_Rl2  );
# display molecule
print "Receptor molecule: ", $receptor->get_BNG2_string(), "\n";


# build ligand-receptor complex
{
    # build ligand
    my $ligrec = BNGcore::PatternGraph->new();
    # add molecules
    my $mol_L    = $ligrec->add_node( BNGcore::Node->new($type_L) );
    my $comp_Lr1 = $ligrec->add_node( BNGcore::Node->new($type_Lr) );
    my $bond_Lr1 = $ligrec->add_node( BNGcore::Node->new($bond_node_type) );
    my $comp_Lr2 = $ligrec->add_node( BNGcore::Node->new($type_Lr) );
    my $mol_R    = $ligrec->add_node( BNGcore::Node->new($type_R) );
    my $comp_Rl1 = $ligrec->add_node( BNGcore::Node->new($type_Rl) );
    my $comp_Rl2 = $ligrec->add_node( BNGcore::Node->new($type_Rl) );
    my $bond_Rl2 = $ligrec->add_node( BNGcore::Node->new($bond_node_type) );

    my $bond_Lr2Rl1 = $ligrec->add_node( BNGcore::Node->new($bond_node_type) );
    $bond_Lr2Rl1->set_state( $bound_state );

    # add edges
    $ligrec->add_edge( $mol_L, $comp_Lr1 );
    $ligrec->add_edge( $mol_L, $comp_Lr2 );          
    $ligrec->add_edge( $comp_Lr1, $bond_Lr1  );          
    $ligrec->add_edge( $comp_Lr2, $bond_Lr2Rl1  );
    $ligrec->add_edge( $mol_R, $comp_Rl1 );
    $ligrec->add_edge( $mol_R, $comp_Rl2 );          
    $ligrec->add_edge( $comp_Rl1, $bond_Lr2Rl1  );          
    $ligrec->add_edge( $comp_Rl2, $bond_Rl2  );
    # display molecule
    print "Ligand-Receptor complex: ", $ligrec->get_BNG2_string(), "\n";

    print "label = ", $ligrec->get_label(), "\n";

}
