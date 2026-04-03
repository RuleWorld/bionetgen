/*
 * Map.cpp
 *
 *  Created on: May 19, 2010
 *      Author: justin
 */

#include "BNGcore.hpp"
using namespace BNGcore;


         

// define static members
nodemap_iter_t        Map::nodemap_iter;
nodemap_const_iter_t  Map::nodemap_const_iter;



//////////
// MAP //
////////

// Constructor
Map::Map ( )
{
    source_graph = 0;
    target_graph = 0;
}


// Constructor with arguments
Map::Map ( PatternGraph * source, PatternGraph * target )
{
    source_graph = source;
    target_graph = target;
}


// Copy constructor
Map::Map ( const Map & source_map )
{
    source_graph = source_map.source_graph;
    target_graph = source_map.target_graph;
    nodemap = source_map.nodemap;
}

// construct an automap for this graph
Map::Map ( const PatternGraph & graph )
{
    node_const_iter_t  node_iter;

    // set target and source
    source_graph = (PatternGraph*)&graph;
    target_graph = (PatternGraph*)&graph;
    // create automap
    for ( node_iter = graph.begin();  node_iter != graph.end();  ++node_iter )
        nodemap.insert( node_pair_t(*node_iter, *node_iter) );  
}


// Destructor
Map::~Map ( )
{

}


// map a node in the domain to its target
Node *
Map::mapf ( Node * node ) const
{
    // the map 
    nodemap_const_iter = nodemap.find( node );
    if ( nodemap_const_iter == nodemap.end() )
        return 0;
    else
        return nodemap_const_iter->second;
}




// map a set of nodes
void
Map::image ( node_const_iter_t  nodes_begin,
             node_const_iter_t  nodes_end,
             node_container_t & image ) const
{
    // local variable
    Node *  node_image;
    node_const_iter_t node_iter;

    // put image of each node in the container "images"
    for ( node_iter = nodes_begin; node_iter != nodes_end; ++node_iter )
    {
        node_image = mapf( *node_iter );
        if ( node_image != 0 )  image.push_back( node_image );
        ++node_iter;
    }
}



// find map iterator with key "node"
nodemap_iter_t
Map::find ( Node * node )
{
    return nodemap.find( node );
}

// find map iterator with key "node"
// "const" version
nodemap_const_iter_t
Map::find ( Node * node ) const
{
    return nodemap.find( node );
}





// Compose maps:

//   If the target of f is not the source of g, then the null pointer is returned.
//    
//   If x is not in the domain of f, then x is not in the domain of gof,
//   Also, if y is in the domain of g, but y is not in the image of f,
//    then g(y) is not (necessarily) in the image of gof.

Map *
Map::mapf ( const Map & f ) const
{
    const Map & g = *this;
    // return empty map if target of f_map is not the source of g_map;
    if ( f.get_target_graph() != g.get_source_graph() )
        return 0; 

    // create composition map
    Map * gof = new Map();
    // other local variables...
    Node              *gof_node;
    nodemap_const_iter_t  nodemap_const_iter;    
    
    // define source and target graphs for composition
    gof->set_source_graph( f.get_source_graph() );
    gof->set_target_graph( g.get_target_graph() );        

    // loop over the domain of f
    for ( nodemap_const_iter = f.begin();  nodemap_const_iter != f.end();  ++nodemap_const_iter )
    {
        gof_node = g.mapf ( nodemap_const_iter->second );
        if ( gof_node != 0 )
            gof->insert ( nodemap_const_iter->first, gof_node );
    }
    return gof;
}



// get inverse of map
//  NOTE: maps are generally 1-1, although this isn't enforced.
//   So be careful for now!

Map *
Map::invert ( ) const
{
    // create inverted map
    Map * inverse_map = new Map();
    // other local variables
    nodemap_const_iter_t  nodemap_const_iter;
    
    // set source and target graphs of inverse
    inverse_map->set_source_graph ( get_target_graph() );
    inverse_map->set_target_graph ( get_source_graph() );    
    // loop over domain of original map
    for ( nodemap_const_iter = begin();  nodemap_const_iter != end();  ++nodemap_const_iter )
        inverse_map->insert (nodemap_const_iter->second, nodemap_const_iter->first );
    // return inverse map
    return inverse_map;
}



// a insert a new map element
bool
Map::insert ( Node * node1, Node * node2 )
{
    return nodemap.insert( node_pair_t(node1, node2) ).second;
}


// erase a node from the map domain
size_t
Map::erase ( Node * node )
{
    return nodemap.erase( node );
}


// get,set source,target graphs
PatternGraph *
Map::get_source_graph ( ) const
{
    return source_graph;
}

PatternGraph *
Map::get_target_graph ( ) const
{
    return target_graph;
}

void
Map::set_source_graph ( PatternGraph * source )
{
    source_graph = source;
}

void
Map::set_target_graph ( PatternGraph * target )
{
    target_graph = target;
}


// create a copy of the target
//  and build a map that points to the copy       
std::pair < Map*, PatternGraph* >
Map::copy_map_and_target ( ) const
{        
    // local variables
    std::pair < PatternGraph*, Map* >  copy_pair;
    PatternGraph  *graph_copy;
    Map    *orig_to_copy, *map_copy;

    // create a copy of the target and get map from original to target   
    copy_pair = get_target_graph()->copy_and_get_map();    
    graph_copy   = copy_pair.first;
    orig_to_copy = copy_pair.second;

    // create a copy of this map
    map_copy = orig_to_copy->mapf ( *this );
    
    // return pair
    return std::pair < Map*, PatternGraph* > ( map_copy, graph_copy );
}



// print to stdout
void
Map::print ( ) const
{
    node_const_iter_t  node_iter;
    nodemap_const_iter_t   nodemap_iter;

    std::cout << "/// Map::print_map ///" << std::endl;
    std::cout << "PatternGraph:  " << source_graph << " -> " << target_graph << "\n" << std::endl;
    
    std::cout << "source graph: "<< source_graph << std::endl;
    if ( source_graph != 0 ) source_graph->print();
    std::cout << "target graph: " << target_graph << std::endl;    
    if ( target_graph != 0 ) target_graph->print();
        
    std::cout << "// detailed map spec: //" << std::endl;
    if ( source_graph == 0 )
    {   // if source graph is not defined
        for ( nodemap_iter = begin();  nodemap_iter != end();  ++nodemap_iter )
        {
            std::cout << nodemap_iter->first->get_label() << " : " << nodemap_iter->first << " -> ";
            std::cout << nodemap_iter->second->get_label() << " : " << nodemap_iter->second;
            std::cout << std::endl;      
        }
    }
    else
    {   // if source graph IS defined
        for ( node_iter = source_graph->begin();  node_iter !=source_graph->end();  ++node_iter )
        {
            std::cout << (*node_iter)->get_label() << " : " << *node_iter << " -> ";
        
            nodemap_iter = nodemap.find( *node_iter );
            if ( nodemap_iter == nodemap.end() )
                std::cout << "NULL";
            else
                std::cout << (nodemap_iter->second)->get_label() << " : " << *node_iter;
        
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}


// quick merge maps
void
Map::quick_merge ( map_container_t & maps )
{
    Map            *curr_map;
    nodemap_iter_t  nodemap_iter;
    while ( !maps.empty() )
    {   // remove graph from back of graphs
        curr_map = maps.withdraw_back();
        // insert nodes of this graph into aggregate graph
        for ( nodemap_iter = curr_map->begin();  nodemap_iter != curr_map->end();  ++nodemap_iter )
            nodemap.insert ( *nodemap_iter );
        // clear current map
        delete curr_map;
    };
};

