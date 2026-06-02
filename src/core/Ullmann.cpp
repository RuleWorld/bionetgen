/*
 * Ullmann.cpp
 *
 *  Created on: May 12, 2010
 *      Author: justin
 *
 *
 * Ullman refinement algorithm
 *   J.R.Ullman, "An Algorithm for Subgraph Isomorphism"
 *   J.Assoc.Computing Machinery, 1976 (p34).
 */

#include "BNGcore.hpp"
#include "Ullmann.hpp"
using namespace BNGcore;



/////////////////
// UllmannBase //
/////////////////

// base constructor
 UllmannBase::UllmannBase ( const PatternGraph & _Ga, const PatternGraph & _Gb )
  : Ga( _Ga ),
    Gb( _Gb ),
    pa( Ga.size() ),
    pb( Gb.size() ),
    M_vec( pa, ullmann_M_t( pa, node_container_t() ) ),
    M( pa, node_container_t() ),
    targets_mask( pb, false )
 {
     map.set_source_graph( (PatternGraph *)&Ga );
     map.set_target_graph( (PatternGraph *)&Gb );
 
     // Assign temporary indices to Gb nodes for fast bitset access
     node_const_iter_t it;
     int i = 0;
     for (it = Gb.begin(); it != Gb.end(); ++it, ++i) {
         (*it)->set_index(i);
     }

     // Assign temporary indices and collect Ga nodes
     Ga_nodes.reserve(pa);
     i = 0;
     for (it = Ga.begin(); it != Ga.end(); ++it, ++i) {
         (*it)->set_index(i);
         Ga_nodes.push_back(*it);
     }

     // Build fast O(1) adjacency lookup matrices for Gb
     adj_matrix_out.assign(pb, std::vector<bool>(pb, false));
     adj_matrix_in.assign(pb, std::vector<bool>(pb, false));
     for (node_const_iter_t it_u = Gb.begin(); it_u != Gb.end(); ++it_u) {
         Node *u = *it_u;
         int u_idx = u->get_index();
         for (node_const_iter_t it_v = u->edges_out_begin(); it_v != u->edges_out_end(); ++it_v) {
             adj_matrix_out[u_idx][(*it_v)->get_index()] = true;
         }
         for (node_const_iter_t it_v = u->edges_in_begin(); it_v != u->edges_in_end(); ++it_v) {
             adj_matrix_in[u_idx][(*it_v)->get_index()] = true;
         }
     }

     // Pre-reserve capacities to completely eliminate heap reallocations during backtracking
     for (size_t i = 0; i < pa; ++i) {
         M[i].reserve(pb);
         for (size_t d = 0; d < pa; ++d) {
             M_vec[d][i].reserve(pb);
         }
     }
 }



// initialize intermediate storage for M matrix objects
void
UllmannBase::initialize_M_vec ( )
{
    // M and M_vec are already sized in the constructor initializer list.
    // We just clear their internal vectors to make them ready for build_M0.
    for (size_t i = 0; i < pa; ++i) {
        M[i].clear();
        for (size_t d = 0; d < pa; ++d) {
            M_vec[d][i].clear();
        }
    }
}



// print the M matrix to cout
void
UllmannBase::print_M ( ullmann_M_t & M )
{
    std::cout << std::endl << " M = " << std::endl;
    
    // loop over nodes in Ga
    for ( size_t i = 0; i < pa; ++i )
    {
        const node_container_t & possible_matches = M[i];
        
        std::vector<bool> is_match(pb, false);
        for (auto match : possible_matches) {
            is_match[match->get_index()] = true;
        }

        // loop over nodes in Gb
        node_const_iter_t node_iter_b;
        for ( node_iter_b = Gb.begin();  node_iter_b != Gb.end();  ++node_iter_b )
        {
            // see if M_ij=1
            if ( is_match[(*node_iter_b)->get_index()] )
                std::cout << "1,";
            else
                std::cout << "0,";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



// copy orig M onto ullmann_M_t structure reference given by copy
void
UllmannBase::copy_M ( ullmann_M_t & orig, ullmann_M_t & copy )
{
    // Fast vector copy assignment
    copy = orig;
}






//////////////////
// UllmannSGIso //
//////////////////

size_t
UllmannSGIso::find_maps ( List <Map> & maps )
{    
    bool return_val;
    
    // initialize M_vec
    initialize_M_vec ( );
    
    // build M0
    return_val = build_M0 ( );        
    if ( !return_val ) return 0;
                       
    // refine M
    return_val = refine_M ( );
    if ( !return_val )  return 0;
    
    // call recursive core of the subgraph isomorphism algorithm
    return next_node( 0, maps );
}



// core recursive algorithm
size_t
UllmannSGIso::next_node ( size_t d, List <Map> & maps )
{
    size_t num_sg_iso = 0;
    Node *node_a = Ga_nodes[d];

    // put a pristine copy of M at M_vec[d];
    copy_M( M, M_vec[d] );
    
    // look for next way to map node Ga_d into graph Gb
    node_container_t & possible_matches = M[d];
    col_iter_t  col_iter = possible_matches.begin();
    
    while (  find_next_match( col_iter, possible_matches.end() )  )
    {           
        // record the distance before possible_matches is modified
        size_t col_index = std::distance( possible_matches.begin(), col_iter );

        // get match node_b first (before we screw with possible_matches set)
        Node *node_b = *col_iter;
    
        // delete possible matches, except for col_node
        //  NOTE: this will be restored later when we call copy_M
        possible_matches.clear();
        possible_matches.push_back( node_b );
        
        // refine M and continue, if possible
        if ( refine_M() )
        {
            // extend subgraph isomorphism map
            map.insert( node_a, node_b );
            // flag col_node as mapped
            targets.push_back( node_b );
            targets_mask[node_b->get_index()] = true;
        
            if ( d+1 < pa )
            {   // goto next row_node      
                num_sg_iso += next_node( d+1, maps );
            }
            else
            {   // we have a subgraph isomorphism!!
                maps.push_back( map );
                ++num_sg_iso;
            }   
      
            // remove map of row_node
            map.erase( node_a );
            // free up col_node
            targets.pop_back();
            targets_mask[node_b->get_index()] = false;
        }

        // advance column iterator        
        // retrieve a pristine copy of M from storage
        copy_M( M_vec[d], M );
        
        // restore col_iter! NOTE that we don't need to restore possible matches since this
        //  happens as a side-effect of copy_M
        col_iter = possible_matches.begin() + col_index;
        ++col_iter;
    }
    
    return num_sg_iso;
}





bool
UllmannSGIso::find_next_match ( col_iter_t & col_iter, const col_iter_t & col_end )
{    
    while (  col_iter != col_end  )
    {
        // see if match node is already occupied
        if ( !targets_mask[(*col_iter)->get_index()] )
            return true;
        
        // if occupied, look for next possible match
        ++col_iter;
    }
    return false;
}



// build Ullman's M0 matrix (load into member M)
bool
UllmannSGIso::build_M0 ( )
{
    //   m_ij = 1   if and only if:
    //     (1)   ga_i == gb_j (type/state compatibility check)
    //     (2)   in_degree(ga_i) <= in_degree(gb_j)
    //     (3)   out_degree(ga_i) <= out_degree(gb_j)
    
    node_const_iter_t  node_iter_b;
    Node         *node_a, *node_b;
    size_t       in_deg_a, out_deg_a;
    
    // loop over nodes in G_alpha
    for ( size_t i = 0; i < pa; ++i )
    {
        node_a = Ga_nodes[i];
        in_deg_a  = node_a->in_degree();
        out_deg_a = node_a->out_degree();
        
        node_container_t & possible_match_nodes = M[i];
        possible_match_nodes.clear();
           
        // loop over nodes in G_beta
        for ( node_iter_b = Gb.begin(); node_iter_b != Gb.end(); ++node_iter_b )
        {
            node_b = *node_iter_b;      
            // check degree 
            if (  (in_deg_a <= node_b->in_degree())  &&  (out_deg_a <= node_b->out_degree())  &&  (*node_a == *node_b)  )
                // add node_b as a possible match to node_a 
                possible_match_nodes.push_back(node_b);
        }
    }
    return true;
}



// refine the M matrix
bool
UllmannSGIso::refine_M ( )
{
    col_iter_t        col_iter;

    // refine M until converged
    bool converged = false;
    while ( !converged )
    {
        // assume converged until we find a bad m_ij
        converged = true;
        
        // loop over nodes in Ga (i.e. rows of M)
        for ( size_t i = 0; i < pa; ++i )
        {
            // loop over possible matches in Gb
            node_container_t & possible_matches = M[i];
            col_iter = possible_matches.begin();
            while ( col_iter != possible_matches.end() )
            {
                // refine M for each possible match
                if ( !refine_M( i, col_iter ) )
                {
                    // this match is no good — erase returns valid iterator to next element
                    col_iter = possible_matches.erase( col_iter );

                    //  check if this search branch is futile
                    if (  possible_matches.size() == 0  )
                        return false;

                    converged = false;
                }
                else
                    ++col_iter;
            }
        }
    }
    return true;
}



// determine if node_a matched to node_b is valid
bool
UllmannSGIso::refine_M ( size_t row_idx, col_iter_t & col_iter )
{
    // get nodes
    Node * node_a = Ga_nodes[row_idx];
    Node * node_b = *col_iter;

    // declare some iterators
    node_const_iter_t  adj_a_iter, adj_a_end;
    node_iter_t  matches_iter, matches_end;
    node_container_t  *possible_adj_a_matches;
    
    // ends up false if a cannot match b
    bool  a_match_b;    
    
    // loop over nodes adjacent to node_a via OUT-edges
    adj_a_end = node_a->edges_out_end();
    for ( adj_a_iter = node_a->edges_out_begin();  adj_a_iter != adj_a_end;  ++adj_a_iter )
    {
        // get set of possible matches to adj_a via direct O(1) vector index
        possible_adj_a_matches = &M[(*adj_a_iter)->get_index()];
        
        // only change to true if we find a possible match to adj_a that is adjacent to node_b
        a_match_b = false;
        // loop over possible matches of adj_a
        matches_end = possible_adj_a_matches->end();
        for ( matches_iter = possible_adj_a_matches->begin();
                matches_iter != matches_end;  ++matches_iter  )
        {
            // see if there is an edge from  node_b to match
            if (  adj_matrix_out[node_b->get_index()][(*matches_iter)->get_index()]  )
            {
                a_match_b = true;
                break;
            }
        }

        if ( !a_match_b )
            return false;
    }

    // Also check IN-edges: for each in-neighbor of node_a, there must be a
    // corresponding in-neighbor of node_b among the possible matches.
    // This is essential for nodes with zero out-degree (e.g., bond nodes)
    // and for full graph isomorphism of undirected graphs stored as directed edges.
    adj_a_end = node_a->edges_in_end();
    for ( adj_a_iter = node_a->edges_in_begin();  adj_a_iter != adj_a_end;  ++adj_a_iter )
    {
        size_t adj_idx = (*adj_a_iter)->get_index();
        if ( adj_idx >= pa )
            continue;  // in-neighbor not in pattern (subgraph matching) — skip
            
        possible_adj_a_matches = &M[adj_idx];
        a_match_b = false;
        matches_end = possible_adj_a_matches->end();
        for ( matches_iter = possible_adj_a_matches->begin();
                matches_iter != matches_end;  ++matches_iter  )
        {
            if (  adj_matrix_in[node_b->get_index()][(*matches_iter)->get_index()]  )
            {
                a_match_b = true;
                break;
            }
        }
        if ( !a_match_b )
            return false;
    }
    return true;
}
