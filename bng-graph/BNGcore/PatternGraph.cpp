/*
 * PatternGraph.cpp
 *
 *  Created on: May 11, 2010
 *      Author: justin
 */

#include "BNGcore.hpp"
#include "Ullmann.hpp"
#include "nausparse.h"  /* which includes nauty.h */
using namespace BNGcore;


// define static members
node_iter_t        PatternGraph::node_iter;
node_const_iter_t  PatternGraph::node_const_iter;


// Constructor
PatternGraph::PatternGraph ( )
{
    label = "";
    canonical_flag = false;
}


// Copy constructor
PatternGraph::PatternGraph ( const PatternGraph & source )
{
    // copy source graph to this 
    Map * copy_map = copy_from_source ( source );
    delete copy_map;
}
   

// Destructor
PatternGraph::~PatternGraph ( )
{
    //std::cout << "PatternGraph destructor" << std::endl;
    // delete nodes explicitly
    node_iter_t  node_iter;
    for ( node_iter = nodes.begin();  node_iter != nodes.end();  ++node_iter )
        delete *node_iter;
}



// core machinery for graph copy
// copy source graph into "this" graph
//  return map from nodes in source to the copies in this.
Map *
PatternGraph::copy_from_source ( const PatternGraph & source )
{
    // initialize map    
    Map   * copy_map  = new Map ( );
    // don't set source graph, so we preserve const-ness of source
    copy_map->set_target_graph ( this );

    node_const_iter_t  node_iter, edge_iter, edges_end; 
    Node               *orig_node, *copy_node, *orig_edge;

    // copy nodes and build map from original nodes to copied nodes    
    for ( node_iter = source.begin();  node_iter != source.end();  ++node_iter )
    {
        orig_node = *node_iter;     
        // insert copy into the new graph
        copy_node = add_node ( *orig_node );          
        // map original node to copy
        copy_map->insert( orig_node, copy_node );    
    }
 
    // relink edges
    for ( node_iter = source.begin();  node_iter != source.end();  ++node_iter )
    { 
        orig_node = *node_iter;
        // out bound edges
        edges_end = orig_node->edges_out_end();
        for ( edge_iter = orig_node->edges_out_begin();  edge_iter != edges_end;  ++edge_iter )
        {
            orig_edge = *edge_iter;        
            add_edge ( copy_map->mapf(orig_node), copy_map->mapf(orig_edge) );
        }

        // in bound edges
        //std::cout << "    edges_in:" << std::endl;        
        edges_end = orig_node->edges_in_end();
        for ( edge_iter = orig_node->edges_in_begin();  edge_iter != edges_end;  ++edge_iter )
        {
            orig_edge = *edge_iter;                
            add_edge ( copy_map->mapf(orig_edge), copy_map->mapf(orig_node) );         
        }
    }

    // copy canonical label
    if ( (canonical_flag = source.is_canonical()) )
        label = source.get_label();
    else
        label = "";
      
    return copy_map;
}


// copy graph and get a correspondence map between original and copy
std::pair < PatternGraph*, Map* >
PatternGraph::copy_and_get_map ( ) const
{
    // create new graph
    PatternGraph * copy_graph = new PatternGraph ( );
    // copy and get make from original to copy
    Map * copy_map = copy_graph->copy_from_source ( *this );
    // set map target and source
    // NOTE: this is done here (not inside copy_from_source
    // to protect the "const-ness" of source parameter in copy_from_source.  
    copy_map->set_source_graph ( (PatternGraph*)this );
    copy_map->set_target_graph ( copy_graph );
    // return copy graph and map    
    return std::pair <PatternGraph*,Map*> (copy_graph, copy_map);
}


// add node to graph
//   NOTE: this is the safe version for use outside the CLass.
//   Pass by reference insures that the Node is instantiated!
Node *
PatternGraph::add_node ( Node & node )
{
    canonical_flag = false;
    nodes.push_back(node.clone());
    return nodes.back();
}


// delete node from graph
bool
PatternGraph::delete_node ( Node * node )
{
    canonical_flag = false;

    node_iter = std::find( begin(), end(), node );
    if ( node_iter == end() )
        return false;

    // remove in edges
    for ( node_const_iter = node->edges_in_begin();  node_const_iter != node->edges_in_end();  ++node_const_iter  )
        delete_edge ( *node_iter, node  );

    // remove out edges
    for ( node_const_iter = node->edges_out_begin();  node_const_iter != node->edges_out_end();  ++node_const_iter  )
        delete_edge ( node, *node_iter );
    
    // erase node from graph
    delete node;
    (*node_iter) = nodes.back();
    nodes.pop_back();
    return true;
}


// add directed edge to graph
bool
PatternGraph::add_edge ( Node * node1, Node * node2 )
{
    canonical_flag = false;
    // add out edge to node1
    node1->add_edge_out ( node2 ); 
    // add in edge to node2
    node2->add_edge_in ( node1 );
    return true;
}


// delete edge from graph
size_t
PatternGraph::delete_edge ( Node * node1, Node * node2 )
{
    canonical_flag = false;
    // delete out edge from node1
    node1->delete_edge_out ( node2 );
    // delete in edge to node2
    node2->delete_edge_in ( node1 );
    return 1;
}


// set the state of a node
bool
PatternGraph::set_node_state ( Node * node, const State & state )
{
    return node->set_state ( state );
}


// add node to graph by address (careful!)
void
PatternGraph::add_node ( Node * node )
{
    canonical_flag = false;
    nodes.push_back( node );
}


// query canonical status of graph
bool
PatternGraph::is_canonical ( ) const
{
    return canonical_flag;
}



// gather descendents of a node
void
PatternGraph::gather_subtree ( Node * curr_node, node_container_t & visited, node_container_t & bonds )
{
    node_iter_t  node_iter;
    
    // if current node is an entity, add to "visited" and continue gathering children
    if ( curr_node->get_type() < ENTITY_NODE_TYPE )
    {
        if ( std::find( visited.begin(), visited.end(), curr_node ) == visited.end() )
        {
            visited.push_back( curr_node );
            for ( node_const_iter = curr_node->edges_out_begin();  node_const_iter != curr_node->edges_out_end();  ++node_const_iter )
                gather_subtree ( *node_iter, visited, bonds );
        }
    } 
    
    // if current node is a bond, add to "bonds" container and terminate this search branch.
    else if ( curr_node->get_type() < LINK_NODE_TYPE )
    {
        if ( std::find( bonds.begin(), bonds.end(), curr_node ) == bonds.end())
        {
            bonds.push_back( curr_node );
        }
    }    
}


// gather nodes connected to a node by any path (forward of reverse)
void
PatternGraph::gather_connected ( Node * curr_node, node_container_t & visited )
{
    if ( std::find( visited.begin(), visited.end(), curr_node ) == visited.end() )
    {
        visited.push_back( curr_node );

        for ( node_const_iter = curr_node->edges_in_begin();  node_const_iter != curr_node->edges_in_end();  ++node_const_iter )
            gather_connected ( *node_iter, visited );

        for ( node_const_iter = curr_node->edges_out_begin();  node_const_iter != curr_node->edges_out_end();  ++node_const_iter )
            gather_connected ( *node_iter, visited );
    }  
}


// write graph as string
std::string
PatternGraph::get_BNG2_string ( ) const
{    
    // nothing to do if there are zero nodes
    if ( nodes.size() == 0 )
        return "";

    // otherwise . . .
    // declare local variables
    int   ii;
    Node  *node;
    bool  first_node, has_parents;
    std::stringstream             s;
    node_container_t              node_order;
    node_const_iter_t             node_iter;
    node_const_iter_t             edge_iter;
    std::map <const Node*,int>    bond_index;            

    // sort nodes
    node_order = std::vector<Node*> ( nodes.begin(), nodes.end() );
    std::sort ( node_order.begin(), node_order.end(), Node::less );

    // set up bond index map.
    ii = 0;
    for ( node_iter = node_order.begin();  node_iter != node_order.end();  ++node_iter )
    {
        node = *node_iter;
        if ( node->get_type() < LINK_NODE_TYPE )
        {    
            bond_index.insert ( std::pair<const Node*,int>( node, ii ) );
            ++ii;
        }
    }
    
    // iterate over nodes and print Entities (and their descendants)
    first_node = true;
    for ( node_iter = node_order.begin();  node_iter != node_order.end();  ++node_iter )
    {
        node = *node_iter;
        if ( node->get_type() < ENTITY_NODE_TYPE )
        {
            // skip node if it has parents (this will be found by the depth search from each parent node)
            has_parents = false;
            for ( edge_iter = node->edges_in_begin();  edge_iter != node->edges_in_end();  ++edge_iter )
            {
                if ( (*edge_iter)->get_type() < ENTITY_NODE_TYPE )
                {   has_parents = true;  break;   }
            }
            if ( has_parents ) continue;

            // this node has no parents, so begin recursive string fetch
            if ( !first_node )  s << ".";
            s << node->get_BNG2_string( bond_index );
            first_node = false;
        }
    }
    
    return s.str(); 
}



// Generate a canonical string for graph (requires that canonical ordering has been determined.
std::string
PatternGraph::get_label ( ) const
{
    return get_label(false);
}


// Returns a canonical string for this graph.
//  Generates canonical ordering if not already done.
std::string
PatternGraph::get_label ( bool preserve_prior_order ) const
{   
    // return canonical label if it's available
    if ( canonical_flag ) return label;
    
    // else
    find_canonical_order( preserve_prior_order );

    // initialize stringstream
    std::stringstream  s;

    // some data structures
    std::vector <Node*>            node_vec,  edge_vec;
    std::vector <Node*>::iterator  node_iter, edge_iter;
    Node                           *node;
    bool                           first_node, first_edge;
    
    // sort nodes
    node_vec = std::vector <Node*> ( nodes.begin(), nodes.end() );
    std::stable_sort ( node_vec.begin(), node_vec.end(), Node::less_by_index );
    
    // write nodes
    first_node = true;
    for ( node_iter = node_vec.begin();  node_iter != node_vec.end();  ++node_iter )
    {   
        node = *node_iter;
        if ( first_node )
        {
            s << node->get_label();
            first_node = false;
        }
        else
            s << "; " << node->get_label();
    
        // sort outbound edges
        edge_vec = std::vector <Node*> ( node->edges_out_begin(), node->edges_out_end() );
        std::sort ( edge_vec.begin(), edge_vec.end(), Node::less_by_index );

        // write edges
        first_edge = true;
        for ( edge_iter = edge_vec.begin();  edge_iter != edge_vec.end();  ++edge_iter )
        {
            if ( first_edge )
            {
                s << "->" << (*edge_iter)->get_index(); 
                first_edge = false;
            }
            else
                s << ","  << (*edge_iter)->get_index();
        }
    }    
    
    // flag this graph as canonical (returns to false if anything modifies the graph)
    canonical_flag = true;
    label = s.str();
    
    // return string
    return label;
}


// find a canonical ordering of the nodes 
void
PatternGraph::find_canonical_order ( bool preserve_prior_order ) const
{
    #if BNGCORE_DEBUG_NAUTY==1
    std::cout << "find_canonical_order.  preserve_prior_order=" << preserve_prior_order << std::endl;
    #endif
    
    // reset the any prior node indexing, unless preservation is desired
    if ( !preserve_prior_order )
        reset_index();

    //std::cout << "find_canonical_order" << std::endl;
    int   nv, m, nde;
    int   v_index, e_index;
    int   *lab, *ptn;
    bool  nauty_required;
    Node  *curr_node, *prev_node;

    node_const_iter_t              edge_iter;
    std::vector <Node*>::iterator  node_iter;

    // a map from node address to index.
    std::map <Node*,int>  node_index;

    // nodes in a vector (element index is the node index)
    std::vector <Node*>   node_vec ( begin(), end() );
    // sort nodes
    std::stable_sort ( node_vec.begin(), node_vec.end(), Node::less );
    
    #if BNGCORE_DEBUG_NAUTY==1
    //std::cout << "preliminary_node_sort:" << std::endl;
    //for ( node_iter = node_vec.begin();  node_iter != node_vec.end();  ++node_iter )
    //    std::cout << (*node_iter)->get_label() << ",";
    //std::cout << std::endl;
    #endif
    
    // number of vertices and directed edges
    nv = size();
    nde = 0;

    // build a map from node addresses to order index
    //  ..count edges while we're at it (store count in variable 'nde')
    v_index = 0;
    node_iter = node_vec.begin();
    while ( node_iter != node_vec.end() )
    {
        #if BNGCORE_NAUTY_DIGRAPH==1
        nde += (*node_iter)->out_degree();       
        #else
        nde += (*node_iter)->in_degree() + (*node_iter)->out_degree(); 
        #endif
        
        node_index.insert( std::pair <Node*, int> ( *node_iter, v_index ) );
        ++v_index;  ++node_iter;
    }

    // build "lab" and "ptr" array to indicate color cells
    //  ..recall that vec is already sorted, so we just need to check equality
    lab    = new int [nv];
    ptn    = new int [nv];

    nauty_required = false;
    v_index = 0;
    for ( node_iter = node_vec.begin();  node_iter != node_vec.end();  ++node_iter )
    {
        curr_node = *node_iter;
        lab[v_index] = v_index;
        if ( v_index != 0 )
        {
            if ( Node::less( prev_node, curr_node ) )
                ptn[v_index-1] = 0;
            else
            {
                nauty_required = true;
                ptn[v_index-1] = 1;
            }
        }
        prev_node = curr_node;
        ++v_index;
    }
    ptn[nv-1] = 0;
    
    #if BNGCORE_DEBUG_NAUTY==1
    std::cout << "lab: ";    
    for ( int i = 0; i != nv; ++i )
        std::cout << lab[i] << ",";
    std::cout << std::endl;
    std::cout << "ptn: ";    
    for ( int i = 0; i != nv; ++i )
        std::cout << ptn[i] << ",";
    std::cout << std::endl;    
    #endif 

    if ( nauty_required )
    {
        // declare various data elements for Nauty
        static DEFAULTOPTIONS_SPARSEGRAPH(options);
        statsblk stats;
    
        // Select option for canonical labelling
        options.getcanon     = TRUE;
        options.defaultptn   = FALSE;
        
        // SIMPLE GRAPH:
        #if BNGCORE_NAUTY_DIGRAPH==1
        options.digraph       = TRUE;
        options.invarproc     = adjacencies_sg;
        options.mininvarlevel = 1;
        options.maxinvarlevel = 50;
        #else
        options.digraph = FALSE;        
        #endif

        // define m (see Nauty documentation)
        m = (nv + WORDSIZE - 1) / WORDSIZE;
        nauty_check( WORDSIZE, m, nv, NAUTYVERSIONID );

        // allocate data structures for Nauty
        SG_DECL(sg);  SG_DECL(cg);
        SG_ALLOC( sg, nv, nde, "malloc" );
        sg.nv  = nv;   // Number of vertices
        sg.nde = nde;  // Number of directed edges

        int * orbits = new int [nv];
        setword * workspace = new setword [10*m];
    
        // build sparse graph
        v_index = 0;
        e_index = 0;
        for ( node_iter = node_vec.begin();  node_iter != node_vec.end();  ++node_iter )
        {
            curr_node = *node_iter;
            
            #if BNGCORE_NAUTY_DIGRAPH==1
            sg.d[v_index] = curr_node->out_degree();
            #else
            sg.d[v_index] = curr_node->in_degree() + curr_node->out_degree();
            #endif
            
            sg.v[v_index] = e_index;
                        
            // build in edges
            #if BNGCORE_NAUTY_DIGRAPH==1
            // skip in edges for digraph
            #else
            for ( edge_iter = curr_node->edges_in_begin();  edge_iter != curr_node->edges_in_end();  ++edge_iter )
            {
                sg.e[ e_index ] = node_index.find( *edge_iter )->second;
                ++e_index;
            }
            #endif
            
            // build out edges
            for ( edge_iter = curr_node->edges_out_begin();  edge_iter != curr_node->edges_out_end();  ++edge_iter )
            {
                sg.e[ e_index ] = node_index.find( *edge_iter )->second;
                ++e_index;
            }

            ++v_index;
        }

        #if BNGCORE_DEBUG_NAUTY==1
        std::cout << "d: "; 
        for ( int i = 0; i != nv; ++i )
            std::cout << sg.d[i] << ",";
        std::cout << std::endl;    
        std::cout << "v: ";    
        for ( int i = 0; i != nv; ++i ) 
            std::cout << sg.v[i] << ",";
        std::cout << std::endl;  
        std::cout << "e: ";    
        for ( int i = 0; i != nde; ++i )
            std::cout << sg.e[i] << ",";
        std::cout << std::endl;
        std::cout << "nv:  " << sg.nv  << std::endl;
        std::cout << "nde: " << sg.nde << std::endl;
        #endif

       /*
        *  Label sg, result in cg and labelling in lab.
        *  It is not necessary to pre-allocate space in cg1 and cg2, but
        *  they have to be initialised as we did above.
        */
        //std::cout << "begin nauty" << std::endl;
        nauty( (graph*)&sg, lab, ptn, NULL, orbits, &options, &stats,
                      workspace, 10*m, m, nv, (graph*)&cg );
        //std::cout << "end nauty" << std::endl;

        // Free Nauty data
        SG_FREE( sg );    
        SG_FREE( cg );
        delete [] orbits;
        delete [] workspace;
    }

    // set index for each node in graph
    for ( v_index = 0;  v_index < nv;  ++v_index )
        node_vec[ lab[v_index] ]->set_index ( v_index );
 
    #if BNGCORE_DEBUG_NAUTY==1
    std::cout << "lab: ";    
    for ( int i = 0; i != nv; ++i )
        std::cout << lab[i] << ",";
    std::cout << std::endl;
    #endif
 
    delete [] lab;
    delete [] ptn;
}


// reset the canonical indexing of nodes
void
PatternGraph::reset_index ( ) const
{
    canonical_flag = false;
    for ( node_const_iter = nodes.begin();  node_const_iter != nodes.end();  ++node_const_iter )
        (*node_const_iter)->set_index( -1 );
}


// print graph information to STDOUT
void
PatternGraph::print ( ) const
{
    node_const_iter_t  edge_iter;

    std::cout << "/// PatternGraph::print_graph : " << this << " ///" << std::endl;
    std::cout << "number of nodes: " << nodes.size() << std::endl;
    std::cout << "canonical? " << canonical_flag << std::endl;    
    if ( canonical_flag )
        std::cout << "Canonical string:\n" << get_label() << std::endl;
    std::cout << "BNG2 string:  " << get_BNG2_string() << std::endl;

    // print detailed graph info 
    std::cout << "// detailed graph spec //" << std::endl;
    for ( node_const_iter = nodes.begin(); node_const_iter != nodes.end(); ++node_const_iter )
    {
        Node * node = *node_const_iter;
        std::cout << node->get_label() << " : " << node << " -> ";
        for ( edge_iter = node->edges_out_begin(); edge_iter != node->edges_out_end(); ++edge_iter )
        {
            Node * edge = *edge_iter;
            std::cout << edge->get_label() << " : " << edge << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


// splice g2 onto this graph according to overlap map.
//  NOTE: overlap_map: g2 -> g1.
//
//  g2 is splice onto g1 by copy.  At return, g2 is unmodified.
//  However,"this" graph (i.e. g1) is spliced in place.
bool
PatternGraph::splice ( PatternGraph & g2, Map & overlap_map )
{
    // get alias to this
    PatternGraph & g1 = (*this);
    
    // local variables
    Node               *node1, *node2;
    node_iter_t        node_iter;
    node_const_iter_t  node_const_iter;
    // a map from nodes in g2 to their copy in g1. 
    Map                  node_map;
    // a vector of nodes added to g1 from g2.
    node_vector_t        add_nodes;
    node_vector_iter_t   add_iter;        
    
    // Loop over nodes in g2 and build map to their counterpart in g1.
    //  Add node to g1 if its missing.
    for ( node_iter = g2.begin();  node_iter != g2.end();  ++node_iter )
    {
        // get graph2 node.
        node2 = *node_iter;   
        // get overlap node in graph1
        node1 = overlap_map.mapf( node2 );
        if ( node1 == 0 )
        {
            // node2 is not in graph1
            // remember that node2 is being added to graph1
            add_nodes.push_back( node2 );
            // make a copy of node2
            node1 = node2->clone();
            // insert in graph1
            g1.add_node( node1 );
        }
        else
        {
            // node2 is already part of graph1
            // overwrite wildcards in graph1 with state of node in graph2
          if ( node1->get_state().is_wildcard() )
                g1.set_node_state ( node1, node2->get_state() );
        }
        
        // add map element from node2 in graph2 to node1 in graph1
        node_map.insert( node2, node1 );        
    }
    
    // now loop over added nodes and insert edges.
    for ( add_iter = add_nodes.begin();  add_iter != add_nodes.end();  ++add_iter )
    {
        // get node from graph2
        node2 = *add_iter;
        // get overlap node in graph1
        node1 = node_map.mapf( node2 );  
        // add inbound edges
        for ( node_const_iter = node2->edges_in_begin();  node_const_iter != node2->edges_in_end();  ++node_const_iter )
            g1.add_edge ( node_map.mapf( *node_iter ), node1 );
        // add outbound edges
        for ( node_const_iter = node2->edges_out_begin();  node_const_iter != node2->edges_out_end();  ++node_const_iter )
            g1.add_edge ( node1, node_map.mapf( *node_iter ) );
        // NOTE: some edges may be added twice, but this won't cause any problems (edges will simply be
        //  ignored when added the second time).
    }
    
    // return
    return true;
}


// Check if this graph is "embedded" in target graph by the overlap_map.
//  A source_graph is embedded in a target_graph if there is an overlap_map
//  from the source_graph into target_graph whose domain includes all nodes
//  of the source_graph.  Furthermore, a node in the image of the overlap_map
//  has wildcard state only if the corresponding node in the source_graph has
//  a wildcard state.
bool
PatternGraph::check_embedding ( const Map & overlap_map ) const
{
    // local variables
    nodemap_const_iter_t  nodemap_iter;

    // first check that all nodes of this graph are included in the overlap_map !
    if (  (*this).size() != overlap_map.size()  ) return false;
    
    // now check for wildcards!
    for ( nodemap_iter = overlap_map.begin();  nodemap_iter != overlap_map.end();  ++nodemap_iter )
    {   
        if (  nodemap_iter->second->get_state().is_wildcard() &&
             !(nodemap_iter->first->get_state().is_wildcard())    )  return false;
    }
    
    // the overlap map is an embedding!
    return true;
}


// validate graph typing as an instance (or not)
bool
PatternGraph::validate ( bool instance ) const
{
    // local variables
    node_const_iter_t  node_iter;

    // loop over nodes
    for ( node_iter = nodes.begin();  node_iter != nodes.end();  ++node_iter )
    {   // validate typing of this node 
        if ( !((**node_iter).validate_typing(instance)) )
            return false;
    }    
    return true;
}


// validate graph typing, and verify that any instance type nodes are fully specified
bool
PatternGraph::validate_instance_types ( ) const
{
    // TODO: need to handle bond neighbors of instance nodes
    // local variables
    const Node *       node;
    const Node *       child_node;
    node_const_iter_t  node_iter;
    node_const_iter_t  child_iter;
    
    // loop over nodes
    for ( node_iter = nodes.begin();  node_iter != nodes.end();  ++node_iter )
    {
        node = *node_iter;
        // check the nodetype instance flag
        if ( node->get_type().get_instance_flag() )
        {   
            // validate typing of this node as an instance
            if ( !(node->validate_typing(true)) )
                return false;
            
            // check that bonds are well-defined
            for ( child_iter = node->edges_out_begin();  child_iter != node->edges_out_end();  ++child_iter )
            {
                child_node = *child_iter;
                if ( child_node->get_type() < LINK_NODE_TYPE )
                {
                    if ( !(child_node->validate_typing(true)) )
                        return false;
                }
            }
        }
        else
        {   // validate typing of this node as a non-instance
            if ( !(node->validate_typing(false)) )
                return false;        
        } 
    }
    return true;
}


// build a tree graph with nodetype at root and all its descendents below.
//  returns a reference to the root node.
Node *
PatternGraph::build_tree_graph ( const NodeType & nodetype )
{
    Node *                node;
    Node *                child_node;
    typemap_const_iter_t  type_iter;
    NodeType *            child_type;
    NodeFunction *        type_fcn;
    size_t                ii;
    size_t                n_copies;

    node = new Node ( nodetype );
    add_node ( node );
    
    for ( type_iter = nodetype.edges_out_begin();  type_iter != nodetype.edges_out_end();  ++type_iter )
    {
        child_type = type_iter->first;
        type_fcn = type_iter->second;
        
        n_copies = type_fcn->map( Node(*child_type) );
        for ( ii = 0; ii < n_copies; ++ii )
        {
            child_node = build_tree_graph( *child_type );
            add_edge( node, child_node );
        }
    }
    
    return node;
}



// check if graph contains instance nodes
bool
PatternGraph::contains_instance_nodes ( ) const
{
    node_const_iter_t  node_iter;    
    for ( node_iter = nodes.begin();  node_iter != nodes.end();  ++node_iter )
    {
        if (  (**node_iter).get_type().get_instance_flag()  )
            // instance node found.  return true
            return true;
    }
    // no instance nodes found
    return false;
}


// quick merge graphs in container (graph container is empty after merge!)
void
PatternGraph::quick_merge ( patterngraph_container_t & graphs )
{
    PatternGraph * curr_graph;
    node_iter_t    node_iter;
    
    canonical_flag = false;    
    while ( !graphs.empty() )
    {   // remove graph from back of graphs
        curr_graph = graphs.withdraw_back();
        // insert nodes of this graph into aggregate graph
        for ( node_iter = curr_graph->nodes.begin();  node_iter != curr_graph->nodes.end();  ++node_iter )
            nodes.push_back( *node_iter );
        // clear nodes of this graph (being careful not to erase nodes!), then destroy graph
        curr_graph->nodes.clear();
        delete curr_graph;
    };
}


// quick split into connected components. (this graph is empty after split!)
// On return, connected graphs are loaded into graphs container.
void
PatternGraph::split_connected ( patterngraph_container_t & split_graphs )
{
    node_container_t  connected_nodes;
    node_iter_t       node_iter;

    canonical_flag = false;
    while ( !nodes.empty() )
    {   // start new graph
        split_graphs.push_back_default_element();
        // gather a new connected component
        gather_connected( nodes.front(), connected_nodes );
        // move components to new graph (by pointer)
        for ( node_iter = connected_nodes.begin();  node_iter != connected_nodes.end();  ++node_iter )
        {   // add node to new split graph
            split_graphs.back().add_node( *node_iter );
            // remove node from the old graph  (move node at back into the old space)
            *std::find(nodes.begin(), nodes.end(), *node_iter) = nodes.back();
            nodes.pop_back();
        }
        connected_nodes.clear();
    }
}



