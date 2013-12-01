/*
 * Transformation.cpp
 *
 *  Created on: May 25, 2010
 *      Author: justin
 */

#include "BNGcore.hpp"
using namespace BNGcore;


// define static members



//-------------------------//
// FAMILY:  Transformation //   A polymorphic family of Transformations on PatternGraphs.
//-------------------------//

//-----------------//
// CLASS:  AddNode // 
//-----------------//

// Constuctor
AddNode::AddNode ( Node & _node )
{
    node = _node.clone();
}

// Copy constructor
AddNode::AddNode ( const AddNode & source_op )
{
    node = source_op.node->clone();
}

// Destructor
AddNode::~AddNode ( )
{
    delete node;
}

// Clone operation and remap to a new target
AddNode *
AddNode::clone_and_remap ( Map & map ) const
{
    AddNode* new_t = new AddNode(*this);
    map.insert ( node, new_t->get_pointer() );
    new_t->remap ( map );
    return new_t;
}

// apply operation to a target under map
void
AddNode::transform ( Map & map )
{
    // local variables
    Node *  new_node;

    // create copy of the new node
    new_node = node->clone();

    // add new node to graph and map
    map.get_target_graph()->add_node ( new_node );
    map.insert ( node, new_node );
}

// remap the operation to a new copy of the target
void
AddNode::remap ( Map & map )
{
    // nothing to do
}


// print out operation details
void
AddNode::print ( )
{
    std::cout << "> Transformation(AddNode): " << node << " [" << node->get_label() << "]" << std::endl;
}


// load operation center nodes into the container passes as argument
void
AddNode::get_center ( node_container_t & center_nodes )
{
    center_nodes.push_back( node );
}




//------------------------//
// CLASS:  DeleteNode     // 
//------------------------//

// Constuctor
DeleteNode::DeleteNode ( Node * _node )
{
    node = _node;
}
            
// Copy constructor
DeleteNode::DeleteNode ( const DeleteNode & source_op )
{
    node = source_op.node;
}

// Clone operation and remap to a new target
DeleteNode *
DeleteNode::clone_and_remap ( Map & map ) const
{
    DeleteNode* new_t = new DeleteNode(*this);
    new_t->remap ( map );
    return new_t;
}

// apply operation to a target under map
void
DeleteNode::transform ( Map & map )
{  
    map.get_target_graph()->delete_node( map.mapf(node) );
    map.erase( node );
}

// remap the operation to a new copy of the target
void
DeleteNode::remap ( Map & map )
{
    node = map.mapf( node );
}

// print out operation details
void
DeleteNode::print ( )
{
    std::cout << "> Transformation(DeleteNode): " << node << std::endl;
}

// load operation center nodes into the container passes as argument
void
DeleteNode::get_center ( node_container_t & center_nodes )
{
    center_nodes.push_back( node );
}




//------------------------//
// CLASS:  AddEdge        // 
//------------------------//

// Constuctor
AddEdge::AddEdge ( Node * _node1, Node * _node2 )
{
    node1 = _node1;
    node2 = _node2;
}
            
// Copy constructor
AddEdge::AddEdge ( const AddEdge & source_op )
{
    node1 = source_op.node1;
    node2 = source_op.node2;
}

// Clone operation and remap to a new target
AddEdge *
AddEdge::clone_and_remap ( Map & map ) const
{
    AddEdge* new_t = new AddEdge(*this);
    new_t->remap ( map );
    return new_t;
}

// apply operation to a target under map
void
AddEdge::transform ( Map & map )
{
    map.get_target_graph()->add_edge( map.mapf( node1 ), map.mapf( node2 ) );
}

// remap the operation to a new copy of the target
void
AddEdge::remap ( Map & map )
{
    node1 = map.mapf( node1 );
    node2 = map.mapf( node2 );
}

// print out operation details
void
AddEdge::print ( )
{
    std::cout << "> Transformation(AddEdge): " << node1 << " -> " << node2 << std::endl;
}

// load operation center nodes into the container passes as argument
void
AddEdge::get_center ( node_container_t & center_nodes )
{
    center_nodes.push_back( node1 );
    center_nodes.push_back( node2 );
}




//------------------//
// CLASS:  AddBond  // 
//------------------//

// Constuctor
AddBond::AddBond ( Node * _node1, Node * _bond1, Node * _node2, Node * _bond2 )
{
    node1 = _node1;
    bond1 = _bond1;    
    node2 = _node2;
    bond2 = _bond2;
}
            
// Copy constructor
AddBond::AddBond ( const AddBond & source_op )
{
    node1 = source_op.node1;
    bond1 = source_op.bond1;
    node2 = source_op.node2;
    bond2 = source_op.bond2;    
}

// Clone operation and remap to a new target
AddBond *
AddBond::clone_and_remap ( Map & map ) const
{
    AddBond* new_t = new AddBond(*this);
    new_t->remap ( map );
    return new_t;
}

// apply operation to a target under map
void
AddBond::transform ( Map & map )
{
    // local variables
    PatternGraph * graph;
    Node * bond_node;
    //Node * target_node1;
    //Node * target_node2;
    node_iter_t  node_iter;

    // get target graph   
    graph = map.get_target_graph();
    // get target nodes
    //Node * target_node1 = map.mapf( node1 );
    //Node * target_node2 = map.mapf( node2 );

    // delete old bond nodes
    graph->delete_node( map.mapf(bond1) );
    graph->delete_node( map.mapf(bond2) );

    //// delete old child bond nodes, if any
    //for ( node_iter = target_node1->edges_out_begin();  node_iter != target_node1->edges_out_end();  ++node_iter )
    //{
    //    if ( (*node_iter)->get_type() < BOND_NODE_TYPE )
    //        graph->delete_node( *node_iter );
    //}
    //for ( node_iter = target_node2->edges_out_begin();  node_iter != target_node2->edges_out_end();  ++node_iter )
    //{
    //    if ( (*node_iter)->get_type() < BOND_NODE_TYPE )
    //        graph->delete_node( *node_iter );
    //}

    // create bond node
    bond_node = new Node( BOND_NODE_TYPE );
    bond_node->set_state( BOUND_STATE );
    // add node to target graph
    graph->add_node( bond_node );
    // connect target nodes to bond_node             
    graph->add_edge( map.mapf( node1 ), bond_node );
    graph->add_edge( map.mapf( node2 ), bond_node );
}

// remap the operation to a new copy of the target
void
AddBond::remap ( Map & map )
{
    node1 = map.mapf( node1 );
    bond1 = map.mapf( bond1 );
    node2 = map.mapf( node2 );
    bond2 = map.mapf( bond2 );
}

// print out operation details
void
AddBond::print ( )
{
    std::cout << "> Transformation(AddBond): " << (node1->get_type()).get_type_name() << " ! " << bond1
              << " -> " << (node2->get_type()).get_type_name() << " ! " << bond2 << std::endl;
}

// load operation center nodes into the container passes as argument
void
AddBond::get_center ( node_container_t & center_nodes )
{
    center_nodes.push_back( node1 );
    center_nodes.push_back( node2 ); 
}




//---------------------//
// CLASS:  DeleteEdge  // 
//---------------------//

// Constuctor
DeleteEdge::DeleteEdge ( Node * _node1, Node * _node2 )
{
    node1 = _node1;
    node2 = _node2;
}

// Copy constructor
DeleteEdge::DeleteEdge ( const DeleteEdge & source_op )
{
    node1 = source_op.node1;
    node2 = source_op.node2;
}

// Clone operation and remap to a new target
DeleteEdge *
DeleteEdge::clone_and_remap ( Map & map ) const
{
    DeleteEdge* new_t = new DeleteEdge(*this);
    new_t->remap ( map );
    return new_t;
}

// apply operation to a target under map
void
DeleteEdge::transform ( Map & map )
{    
    map.get_target_graph()->delete_edge ( map.mapf( node1 ), map.mapf( node2 ) );
}

// remap the operation to a new copy of the target
void
DeleteEdge::remap ( Map & map )
{
    node1 = map.mapf( node1 );
    node2 = map.mapf( node2 );
}

// print out operation details
void
DeleteEdge::print ( )
{
    std::cout << "> Transformation(DeleteEdge): " << node1 << " -> " << node2 << std::endl;
}

// load operation center nodes into the container passes as argument
void
DeleteEdge::get_center ( node_container_t & center_nodes )
{
    center_nodes.push_back( node1 );
    center_nodes.push_back( node2 );    
}





//---------------------//
// CLASS:  DeleteBond  // 
//---------------------//

// Constuctor
DeleteBond::DeleteBond ( Node * _node1, Node * _bond, Node * _node2 )
{
    node1 = _node1;
    bond  = _bond;    
    node2 = _node2;
}
            
// Copy constructor
DeleteBond::DeleteBond ( const DeleteBond & source_op )
{
    node1 = source_op.node1;
    bond  = source_op.bond;
    node2 = source_op.node2;
}

// Clone operation and remap to a new target
DeleteBond *
DeleteBond::clone_and_remap ( Map & map ) const
{
    DeleteBond * new_t = new DeleteBond(*this);
    new_t->remap ( map );
    return new_t;
}

// apply operation to a target under map
void
DeleteBond::transform ( Map & map )
{
    // local variables
    PatternGraph * graph;
    Node * target_node1;
    Node * target_node2;
    Node * nullbond1;
    Node * nullbond2;
    node_iter_t  node_iter;

    // get target graph   
    graph = map.get_target_graph();
    // get target nodes
    //Node * target_node1 = map.mapf( node1 );
    //Node * target_node2 = map.mapf( node2 );

    // delete bond node
    graph->delete_node( map.mapf(bond) );

    //// delete old bond nodes, if any
    //for ( node_iter = target_node1->edges_out_begin();  node_iter != target_node1->edges_out_end();  ++node_iter )
    //{
    //    if ( (*node_iter)->get_type() < BOND_NODE_TYPE )
    //        graph.delete_node( *node_iter );
    //}
    //for ( node_iter = target_node2->edges_out_begin();  node_iter != target_node2->edges_out_end();  ++node_iter )
    //{
    //    if ( (*node_iter)->get_type() < BOND_NODE_TYPE )
    //        graph.delete_node( *node_iter );
    //}

    // create null bonds
    nullbond1 = new Node( BOND_NODE_TYPE );
    nullbond2 = new Node( BOND_NODE_TYPE );
    nullbond1->set_state( UNBOUND_STATE );
    nullbond2->set_state( UNBOUND_STATE );
    // add node to target graph
    graph->add_node( nullbond1 );
    graph->add_node( nullbond2 );
    // connect target nodes to nullbonds   
    graph->add_edge( map.mapf(node1), nullbond1 );
    graph->add_edge( map.mapf(node1), nullbond2 );
}

// remap the operation to a new copy of the target
void
DeleteBond::remap ( Map & map )
{
    node1 = map.mapf( node1 );
    bond  = map.mapf( bond  );    
    node2 = map.mapf( node2 );
}

// print out operation details
void
DeleteBond::print ( )
{
    std::cout << "> Transformation(DeleteBond): " << node1 << " ! " << bond
              << " -> " << node2 << " ! " << bond << std::endl;
}

// load operation center nodes into the container passes as argument
void
DeleteBond::get_center ( node_container_t & center_nodes )
{
    center_nodes.push_back( node1 );
    center_nodes.push_back( node2 );
}






//---------------------//
// CLASS:  ChangeState // 
//--------------------//

// Constuctor
ChangeState::ChangeState ( Node * _node, const State & _state )
{
    node  = _node;
    state = _state.clone();
    // TODO: check state type
    //if (  !(node->get_type().get_state_type().check_state( state ))  ) throw;
}

// Copy constructor
ChangeState::ChangeState ( const ChangeState & source_op )
{
    node  = source_op.node;
    state = source_op.state->clone();
}

// Clone operation and remap to a new target
ChangeState *
ChangeState::clone_and_remap ( Map & map ) const
{
    ChangeState* new_t = new ChangeState(*this);
    new_t->remap( map );
    return new_t;
}

// apply operation to a target under map
void
ChangeState::transform ( Map & map )
{
    map.get_target_graph()->set_node_state( map.mapf( node ), *state );
}

// remap the operation to a new copy of the target
void
ChangeState::remap ( Map & map )
{
    node = map.mapf( node );
}


// print out operation details
void
ChangeState::print ( )
{
    std::cout << "> Transformation(ChangeState): " << node << " : " << node->get_label()
              << " -> " << state << " : " << state->get_label() << std::endl;
}

// load operation center nodes into the container passes as argument
void
ChangeState::get_center ( node_container_t & center_nodes )
{
    center_nodes.push_back( node );
}




//-------------------------//
// CLASS:  DeleteSubtree   // 
//-------------------------//

// Constuctor
DeleteSubtree::DeleteSubtree ( Node * _node )
{
    node = _node;
}
            
// Copy constructor
DeleteSubtree::DeleteSubtree ( const DeleteSubtree & source_op )
{
    node = source_op.node;
}


// Clone operation and remap to a new target
DeleteSubtree *
DeleteSubtree::clone_and_remap ( Map & map ) const
{
    DeleteSubtree* new_t = new DeleteSubtree(*this);
    new_t->remap ( map );
    return new_t;
}

// apply operation to a target under map
void
DeleteSubtree::transform ( Map & map )
{
    // local vars
    PatternGraph *    graph;
    Node *            target_node;
    node_container_t  subtree, dangling_bonds;
    node_iter_t       node_iter;

    // get graph
    graph = map.get_target_graph();
    // get target node
    target_node = map.mapf( node );
    // gather subtree of this node
    graph->gather_subtree( target_node, subtree, dangling_bonds );

    // deleted all nodes in the subtree
    for ( node_iter = subtree.begin(); node_iter != subtree.end(); ++node_iter )
        graph->delete_node( *node_iter );
    
    // now tidy up dangling bonds
    for ( node_iter = dangling_bonds.begin(); node_iter != dangling_bonds.end(); ++node_iter )
    {
        if ( (*node_iter)->in_degree() == 0 )
            graph->delete_node( *node_iter );
        else if ( (*node_iter)->in_degree() == 1 )
            graph->set_node_state( *node_iter, UNBOUND_STATE );
    }
}

// Remap the operation to a new copy of the target.
//  The "map" argument is a map object from the old target to the copy.
void
DeleteSubtree::remap ( Map & map )
{
    node = map.mapf( node );
}


// print out operation details
void
DeleteSubtree::print ( )
{
    std::cout << "> Transformation(DeleteSubtree): " << node << " : " << node->get_label() << std::endl;
}

// load operation center nodes into the container passes as argument
void
DeleteSubtree::get_center ( node_container_t & center_nodes )
{
    center_nodes.push_back( node );
}


//--------------------------//
// CLASS:  DeleteConnected  // 
//--------------------------//

// Constuctor
DeleteConnected::DeleteConnected ( Node * _node )
{
    node = _node;
}
            
// Copy constructor
DeleteConnected::DeleteConnected ( const DeleteConnected & source_op )
{
    node = source_op.node;
}


// Clone operation and remap to a new target
DeleteConnected *
DeleteConnected::clone_and_remap ( Map & map ) const
{
    DeleteConnected* new_t = new DeleteConnected(*this);
    new_t->remap ( map );
    return new_t;
}

// apply operation to a target under map
void
DeleteConnected::transform ( Map & map )
{
    // local vars
    PatternGraph * graph;
    Node * target_node;
    node_container_t  connected_set;
    node_iter_t       node_iter;

    // get graph
    graph = map.get_target_graph();
    // get target node
    target_node = map.mapf(node);
    // gather connected nodes
    graph->gather_connected ( target_node, connected_set );
    // delete all nodes in connected set
    for ( node_iter = connected_set.begin(); node_iter != connected_set.end(); ++node_iter )
        graph->delete_node ( *node_iter );
}

// Remap the operation to a new copy of the target.
//  The "map" argument is a map object from the old target to the copy.
void
DeleteConnected::remap ( Map & map )
{
    node = map.mapf( node );
}


// print out operation details
void
DeleteConnected::print ( )
{
    std::cout << "> Transformation(DeleteConnected): " << node << " : " << node->get_label() << std::endl;
}


// load operation center nodes into the container passes as argument
void
DeleteConnected::get_center ( node_container_t & center_nodes )
{
    // TODO: hmm...
    center_nodes.push_back( node );
}




