/*
 * Node.cpp
 *
 *  Created on: May 11, 2010
 *      Author: justin
 */

#include "BNGcore.hpp"
using namespace BNGcore;


// define static members
node_iter_t  Node::node_iter;
node_iter_t  Node::nodes_end;


//////////
// NODE //
//////////

// Constructor
Node::Node ( const NodeType & _type  )
  : type( &_type )
{
    state = type->get_state_type().get_state_object();
    index = -1;
}


// Copy Constructor
Node::Node ( const Node & orig )
  : type( orig.type )
{
    state = orig.state->clone();
    index = orig.index;
}


// Destructor
Node::~Node ( )
{
    delete state;
}

// set index
void
Node::set_index ( int _index )
{
    index = _index;
}


// set state
bool 
Node::set_state ( const State & new_state )
{
    // check compatible state typing!
    // Ask NodeType to do this, because some NodeTypes are more flexible than
    //  others.  For example, ChangeStateNodes may have a state of any LabelStateType.
    if ( type->check_state( new_state ) )
    {
        state = new_state.clone();
        return true;
    }
    else  return false;
}


// add inbound edge method
void
Node::add_edge_in ( Node * node )
{
    return edges_in.push_back( node );   
}

// add outbound edge method
void
Node::add_edge_out ( Node * node )
{
    return edges_out.push_back( node );   
}

// remove inbound edge method
bool
Node::delete_edge_in ( Node * node )
{
    node_iter = std::find( edges_in.begin(), edges_in.end(), node );
    if ( node_iter == edges_in.end() )
    {   // edges from node was not found
        return false;
    }
    else
    {   // removing edge
        (*node_iter) = edges_in.back();
        edges_in.pop_back();
        return true;
    }
}

// remove outbound edge method
bool
Node::delete_edge_out ( Node * node )
{
    node_iter = std::find( edges_out.begin(), edges_out.end(), node );
    if ( node_iter == edges_out.end() )
    {   // edges from node was not found
        return false;
    }
    else
    {   // removing edge
        (*node_iter) = edges_out.back();
        edges_out.pop_back();
        return true;
    }
}



// find an inbound edges by node address
node_const_iter_t
Node::find_in_edge ( Node * node ) const
{
    return std::find( edges_in.begin(), edges_in.end(), node );
}
// find an inbound edges by node address
node_const_iter_t
Node::find_out_edge ( Node * node ) const
{
    return std::find( edges_out.begin(), edges_out.end(), node );
}


// node equal operator (compares type and state only).
bool
Node::operator== ( const Node & node2 ) const
{
    // check type
    if ( get_type()  != node2.get_type()  )  return false;
    // check state (wildcards allowed)
    if ( get_state() != node2.get_state() )  return false;
    // at this point the nodes are locally equivalent
    return  true;
}


// "less than" method for sorting nodes before canonical labeling.
// This function is static because sort algorithms require this prototype.
bool
Node::less ( const Node * node1, const Node * node2 )
{
    if ( node1->get_type() == node2->get_type() )
    {   
        if ( node1->get_state() == node2->get_state() )
            // different state and type
            return ( node1->index < node2->index );
        
        else // different state, same type
            return ( node1->get_state().less(node2->get_state()) );
    }
    else // different type
        return ( node1->get_type().less( node2->get_type() ) );
}


// "less than" method for comparing nodes by index.
// This function is static because sort algorithms require this prototype.
bool
Node::less_by_index ( const Node * node1, const Node * node2 )
{
    return  (node1->index < node2->index); 
}


// get Node label (canonical)
std::string
Node::get_label ( ) const
{
    std::stringstream s;
    s << index << ":" << type->get_label() << state->get_label();
    return s.str();
}





// check node typing
bool
Node::validate_typing ( bool instance ) const
{
    // define alias for *this
    const Node &      node = *this;
    const NodeType &  type = node.get_type();
    // local variables
    typemap_const_iter_t  type_iter;
    NodeType *            adj_type;
    node_const_iter_t     node_iter;
    NodeFunction *        typing_fcn;    
    int                   type_mult;
    int                   n_type;
    
    // if this is a instance of a node (rather than a pattern)
    //   check that the state not wildcard
    if ( instance )
        if ( node.get_state().is_wildcard() )  return false;
    
    // loop over child types
    for ( type_iter = type.edges_out_begin();  type_iter != type.edges_out_end();  ++type_iter )
    {
        // get type and function typing object
        adj_type   = type_iter->first;
        typing_fcn = type_iter->second;
        // reset n_type and get type multiplicity
        n_type = 0;
        type_mult = typing_fcn->map(node);
        
        // loop over children of node and find children of this type
        for ( node_iter = node.edges_out_begin();  node_iter != node.edges_out_end();  ++node_iter )
        {           
            // check type multiplcity
            if ( (*node_iter)->get_type() < *adj_type )  ++n_type;
        }
    
        // check that the number of named nodes of this type is okay. 
        if ( instance )
        {   if ( type_mult != n_type )
                return false;
        }
        else
        {   if ( type_mult < n_type  )
                return false;
        }
       
    } // done handling children
    
    
    // loop over parent types
    for ( type_iter = type.edges_in_begin();  type_iter != type.edges_in_end();  ++type_iter )
    {        
        // get type and typing function object
        adj_type   = type_iter->first;
        typing_fcn = type_iter->second;
        // reset n_type and get type multiplicity
        n_type = 0;      
        type_mult = typing_fcn->map(node);             
       
        // loop over parents of node1 and find parents of this type
        for ( node_iter = node.edges_in_begin();  node_iter != node.edges_in_end();  ++node_iter )
        {
            // check type multiplcity
            if (  (*node_iter)->get_type() < *adj_type  )  ++n_type;
        }
        
        // check that the number of named nodes of this type is okay. 
        if ( instance )
        {   if ( type_mult != n_type )
                return false;
        }
        else
        {   if ( type_mult < n_type  )
                return false;
        }
                
    } // done handling parents
    
    // overlap is ok!
    return true;
}



// write Node to BNG2 string
std::string
Node::get_BNG2_string ( const link_index_t & link_index ) const
{
    std::stringstream  s;

    if ( get_type() < LINK_NODE_TYPE )
    {   // handle a link node
        if ( get_state() == BOUND_STATE )
            s << "!" << link_index.find(this)->second;
        //else
        //    std::cout << "not bound" << std::endl;
    }
    else
    {   // handle other node types.
        // local variables:
        std::stringstream  t;
        
        bool  found_entity_child;
        Node  *child_node;
    
        std::vector<Node*>            node_order;
        std::vector<Node*>::iterator  node_iter;
        
        // add type_name
        s << get_type().get_BNG2_string( true );
        // add state
        s << get_state().get_BNG2_string( );  
        
        // sort out_edges
        node_order = std::vector<Node*>( edges_out_begin(), edges_out_end() );
        std::sort ( node_order.begin(), node_order.end(), Node::less );
  
        // iterate over child entities
        found_entity_child = false;
        for ( node_iter = node_order.begin();  node_iter != node_order.end();  ++node_iter )
        {
            child_node = *node_iter;
            if ( child_node->get_type() < ENTITY_NODE_TYPE )
            {
                if ( found_entity_child )  t << ",";
                t << child_node->get_BNG2_string( link_index );
                found_entity_child = true;
            }
            else if ( child_node->get_type() < LINK_NODE_TYPE )
                s << child_node->get_BNG2_string( link_index );
        }
    
        // if there are any children, include in string 
        if ( found_entity_child )
            s << "(" << t.str() << ")";
    }
    // return string
    return s.str();
}
