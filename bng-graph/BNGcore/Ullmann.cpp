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
   M_vec( pa, ullmann_M_t() )
{
    // cast constant pointer to graphs as non-constant pointer
    //  otherwise we won't be able to use the pointers later
    //  BUT, Ullmann itself shouldn't modify graphs.    
    map.set_source_graph( (PatternGraph *)&Ga );
    map.set_target_graph( (PatternGraph *)&Gb );
}



// base destructor
UllmannBase::~UllmannBase ( )
{
    row_iter_t row_iter;
    std::vector < ullmann_M_t >::iterator  M_vec_iter;    
    
    // clear M
    for ( row_iter_t row_iter = M.begin();  row_iter != M.end();  ++row_iter )
        delete row_iter->second;
    M.clear();
        
    // clear M_vec
    for ( M_vec_iter = M_vec.begin();  M_vec_iter != M_vec.end();  ++M_vec_iter )
    {
        // loop over maps
        for ( row_iter = M_vec_iter->begin();  row_iter != M_vec_iter->end();  ++row_iter )
            delete row_iter->second;
        M_vec_iter->clear();
    }  
}



// initialize intermediate storage for M matrix objects
void
UllmannBase::initialize_M_vec ( )
{
    node_const_iter_t  node_iter;
    std::vector < ullmann_M_t >::iterator  M_vec_iter;

    // iterator over elements of Mvec
    for ( M_vec_iter = M_vec.begin();  M_vec_iter != M_vec.end();  ++M_vec_iter )
    {
        // loop over nodes in G_alpha
        for ( node_iter = Ga.begin(); node_iter != Ga.end(); ++node_iter )
            (*M_vec_iter).insert ( std::pair <Node*, node_container_t*> (*node_iter, new node_container_t) );
    }    
}



// print the M matrix to cout
void
UllmannBase::print_M ( ullmann_M_t & M )
{
    std::cout << std::endl << " M = " << std::endl;;
    
    // loop over nodes in Ga
    row_iter_t   row_iter_m1;
    col_iter_t   col_end, col_iter_m1;
    node_const_iter_t  node_iter_a, node_iter_b;
    for ( node_iter_a = Ga.begin();  node_iter_a != Ga.end();  ++node_iter_a )
    {
        // set row iter to Ith row of M
        row_iter_m1 = M.find( *node_iter_a );        
        // get iter to last element in column of Ith row
        col_end = row_iter_m1->second->end();
        
        // loop over nodes in Gb
        for ( node_iter_b = Gb.begin();  node_iter_b != Gb.end();  ++node_iter_b )
        {
            // see if M_ij=1
            col_iter_m1 = std::find( row_iter_m1->second->begin(), row_iter_m1->second->end(), *node_iter_b );
            //col_iter_m1 = row_iter_m1->second->find( *node_iter_b );
            if ( col_iter_m1 != col_end )
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
    // IMPORTANT: assumes the row keys are the same in orig and copy, so
    // we can just iterate through both simultaneously

    // Copy contents from original.
    row_iter_t  row_iter1 = orig.begin();
    row_iter_t  row_iter2 = copy.begin();
    while (  row_iter1 != orig.end()  )
    {   
        *(row_iter2->second) = *(row_iter1->second);
        ++row_iter1;  ++row_iter2;
    }
}



// copy orig M onto ullmann_M_t structure reference given by copy,
//  beginning at row_iter (row_iter should point into rows of copy!!)
//  
void
UllmannBase::copy_M ( ullmann_M_t & orig, ullmann_M_t & copy, row_iter_t row_iter )
{
    // IMPORTANT: assumes the row keys are the same in orig and copy, so
    // we can just iterate through both simultaneously

    // Copy contents from original.
    row_rev_iter_t  row_iter1 = orig.rbegin();
    row_rev_iter_t  row_iter2 = copy.rbegin();
    while (  row_iter2.base() != row_iter  )
    {   
        *(row_iter2->second) = *(row_iter1->second);
        ++row_iter1;  ++row_iter2;
    }
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
    
    // debug
    //std::cout << "construct M0: " << std::endl;
    //print_M();
                       
    // refine M
    return_val = refine_M ( );
    if ( !return_val )  return 0;
    
    // debug
    //std::cout << "first refinement results: " << std::endl;
    //print_M();
    
    // call recursive core of the subgraph isomorphism algorithm
    row_iter_t row_iter = M.begin();
    return next_node( 0, row_iter, maps );
}



// core recursive algorithm
size_t
UllmannSGIso::next_node ( size_t d, row_iter_t & row_iter, List <Map> & maps )
{
    size_t num_sg_iso = 0;
    Node *node_a = row_iter->first;

    // note: first index of vectors is 0, rather than 1.
    //std::cout << "next_node, level: " << d << std::endl;
 
    // put a pristine copy of M at M_vec[d];
    //push_M();
    copy_M( M, M_vec[d] );
    
    // look for next way to map node Ga_d into graph Gb
    node_container_t  *possible_matches = row_iter->second;
    col_iter_t  col_iter = possible_matches->begin();
    
    // debug
    //if ( d == 4 ) print_M();
    
    while (  find_next_match( col_iter, possible_matches->end() )  )
    {           
        // get match node_b first (before we screw with possible_matches set)
        Node *node_b = *col_iter;
    
        // delete possible matches, except for col_node
        //  NOTE: this will be restored later when we call copy_M
        possible_matches->clear();
        //possible_matches->insert( node_b );
        possible_matches->push_back( node_b );
        
        // refine M and continue, if possible
        if ( refine_M() )
        {
            // debug
            //if ( d==4 ) print_M(); 
         
            // extend subgraph isomorphism map
            map.insert( node_a, node_b );
            // flag col_node as mapped
            //targets.insert(  node_b  );
            targets.push_back( node_b );
        
            if ( d+1 < pa )
            {   // goto next row_node      
                num_sg_iso += next_node( d+1, ++row_iter, maps );
                --row_iter;
            }
            else
            {   // we have a subgraph isomorphism!!
                //std::cout << "subgraph isomorphism!" << std::endl;
                maps.push_back( map );
                ++num_sg_iso;
            }   
      
            // remove map of row_node
            map.erase( node_a );
            // free up col_node
            //targets.erase( node_b ); 
            targets.pop_back();
        }

        // advance column iterator        
        // retrieve a pristine copy of M from storage
        //refresh_M ( );
        copy_M( M_vec[d], M );
        
        // restore col_iter! NOTE that we don't need to restore possible matches since this
        //  happens as a side-effect of copy_M
        //col_iter = possible_matches->find( node_b );
        col_iter = std::find( possible_matches->begin(), possible_matches->end(), node_b );
        ++col_iter;
    }
    
    //debug
    //std::cout << "return from level " << d << std::endl;
    
    // remove stored M from the stack
    //pop_M( );
    
    return num_sg_iso;
}





bool
UllmannSGIso::find_next_match ( col_iter_t & col_iter, const col_iter_t & col_end )
{    
    //std::cout << "find_next_match:" << std::endl;
    // NOTE: col_iter should be pointing to the next possible match
    while (  col_iter != col_end  )
    {
        // see if match node is already occupied
        //if ( targets.find( *col_iter ) == targets.end() )
        if ( std::find(targets.begin(), targets.end(), *col_iter) == targets.end() )
            return true;
        
        // if occupied, look for next possible match
        ++col_iter;
    }
    //std::cout << "false" << std::endl;
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
    
    node_const_iter_t  node_iter_a, node_iter_b;
    Node         *node_a, *node_b;
    size_t       in_deg_a, out_deg_a;
    col_iter_t   col_iter;
    
    // loop over nodes in G_alpha
    for ( node_iter_a = Ga.begin(); node_iter_a != Ga.end(); ++node_iter_a )
    {
        node_a = *node_iter_a;
        in_deg_a  = node_a->in_degree();
        out_deg_a = node_a->out_degree();
        
        node_container_t * possible_match_nodes = new node_container_t;
        M.insert ( std::pair <Node*, node_container_t*> (node_a, possible_match_nodes) );
           
        // loop over nodes in G_beta
        col_iter = possible_match_nodes->begin();
        for ( node_iter_b = Gb.begin(); node_iter_b != Gb.end(); ++node_iter_b )
        {
            node_b = *node_iter_b;      
            // check degree TODO: switch to node comparator (instead of equality!)
            if (  (in_deg_a <= node_b->in_degree())  &&  (out_deg_a <= node_b->out_degree())  &&  (*node_a == *node_b)  )
                // add node_b as a possible match to node_a 
                col_iter = possible_match_nodes->insert ( col_iter, node_b );
        }
    }    
    return true;
}



// refine the M matrix
bool
UllmannSGIso::refine_M ( )
{
    //std::cout << "refine_M: " << std::endl;

    // declare some local iterators (be safe)
    row_iter_t        row_iter, row_end;
    col_iter_t        col_iter, col_end, col_next;
    node_container_t  possible_matches;

    // refine M until converged
    bool converged = false;
    while ( !converged )
    {
        // assume converged until we find a bad m_ij
        converged = true;
        
        // loop over nodes in Ga (i.e. rows of M)
        row_end  = M.end();
        for ( row_iter = M.begin();  row_iter != row_end;  ++row_iter  )
        {
            // loop over possible matches in Gb
            node_container_t * possible_matches = row_iter->second;
            col_iter = possible_matches->begin();
            col_end = possible_matches->end();
            while ( col_iter != col_end )
            {     
                // refine M for each possible match
                if ( !refine_M( row_iter, col_iter ) )
                {
                    // this match is no good . . .
                    //  get iterator to the next element in the set now
                    (col_next = col_iter)++;
                    possible_matches->erase( col_iter );
                    
                    //  check if this search branch is futile            
                    if (  possible_matches->size() == 0  )
                        return false;
                        
                    converged = false;
                    col_iter = col_next;                    
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
UllmannSGIso::refine_M ( row_iter_t & row_iter, col_iter_t & col_iter )
//Ullmann::refine_M ( Node * node_a , Node * node_b  )
{
    // get nodes
    Node * node_a = row_iter->first;
    Node * node_b = *col_iter;

    // declare some iterators
    node_const_iter_t  adj_a_iter, adj_a_end;
    node_iter_t  matches_iter, matches_end;
    node_container_t  *possible_adj_a_matches;
    
    // ends up false if a cannot match b
    bool  a_match_b;    
    
    // loop over node that are adjacent to node_a
    adj_a_end = node_a->edges_out_end();
    for ( adj_a_iter = node_a->edges_out_begin();  adj_a_iter != adj_a_end;  ++adj_a_iter )
    {
        // get set of possible matches to adj_a
        possible_adj_a_matches = M.find( *adj_a_iter )->second;
        // only change to true if we find a possible match to adj_a that is adjacent to node_b
        a_match_b = false;
        // loop over possible matches of adj_a
        matches_end = possible_adj_a_matches->end();
        for ( matches_iter = possible_adj_a_matches->begin();
                matches_iter != matches_end;  ++matches_iter  )
        {            
            // see if there is an edge from  node_b to match
            // TODO:  need to check that match isn't mapped already?
            if (  node_b->find_out_edge( *matches_iter )  !=  node_b->edges_out_end()  )
            {
                a_match_b = true;
                break;
            }
        }
         
        if ( !a_match_b )
            // a cannot match b, return false now
            return false;
    }
    return true;
}



