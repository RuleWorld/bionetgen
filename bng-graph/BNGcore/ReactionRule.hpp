#ifndef BNGCORE_REACTIONRULE_HPP_
#define BNGCORE_REACTIONRULE_HPP_

#include "BNGcore.hpp"
#include "PatternGraph.hpp"
#include "Ullmann.hpp"

namespace BNGcore
{


///////////////////
// Reaction Rule //
///////////////////
class Reaction
{
    public:
        Reaction  ( ) {  label = "";  };
        Reaction  ( const Reaction & orig ) {  reactants  = orig.reactants;
                                               products   = orig.products;
                                               mechanisms = orig.mechanisms;   
                                               label = orig.label;            };
        ~Reaction ( ) {};
        Reaction * clone ( ) const {  return new Reaction(*this);  };
            
        std::string  get_label  ( ) const { return label; };
        void         set_label ( std::string & _label ) {  label = _label;  };
            
        void   add_reactant ( PatternGraph & reactant ) {  reactants.push_back( reactant.get_label() );  };
        void   add_product  ( PatternGraph & product  ) {  products.push_back(   product.get_label() );  }; 

        bool   add_mechanism ( std::string & mech_label )
        {  return mechanisms.insert( mech_label ).second;  };
                      
        size_t num_mechanisms ( ) const {  return mechanisms.size();  };            
            
    private:
        std::vector  <std::string>    reactants;
        std::vector  <std::string>    products;
        std::set     <std::string>    mechanisms;
        std::string                   label;
};






//////////////////////
// RuleInstanceIter //
//////////////////////
class RuleInstanceIter
{
    public:
        // constructor
        RuleInstanceIter ( Vector <PatternGraph> & _patterns, size_t _ir, List <Map> & _new_maps );
        // destructor
        ~RuleInstanceIter ( ) {};

        // method to load next map set (and graphs)    
        bool  get_next_maps ( Vector <Map> & maps, Vector <PatternGraph> & graphs );
        
    private:
        // members    
        Vector < PatternGraph > &   patterns;
        Vector < List < Map > > &   embeddings;
        const size_t                ir;
        List < Map > &              new_maps;
        bool                        more_map_sets;
        // vectors of iterators (the actual iterators, and vectors of begins and ends)
        Vector < List <Map>::iterator >                iter_vec, begin_vec, end_vec;
        // iterators over vectors of iterators
        Vector < List <Map>::iterator >::iterator      iter_vec_iter, begin_vec_iter, end_vec_iter;          
};






// Constructor
RuleInstanceIter::RuleInstanceIter ( Vector <PatternGraph> & _patterns, size_t _ir, List <Map> & _new_maps )
  : patterns( _patterns ),
    ir( _ir ),
    new_maps(  _new_maps ),
    iter_vec(  _patterns.size() ),
    begin_vec( _patterns.size() ),
    end_vec(   _patterns.size() )   
{
    size_t ii;
    // initialize iterators over maps 
    for ( ii = 0;  ii != patterns.size();  ++ii )
    {
        if ( ii == ir )
        {
            iter_vec[ii]  = new_maps.begin();
            begin_vec[ii] = new_maps.begin();       
            end_vec[ii]   = new_maps.end();
        }
        else
        {
            iter_vec[ii]  = patterns[ii].get_maps().begin();
            begin_vec[ii] = patterns[ii].get_maps().begin();       
            end_vec[ii]   = patterns[ii].get_maps().end();
        }
    }
                
    // check if there are any complete reactant sets
    more_map_sets = true;
    begin_vec_iter = begin_vec.begin();
    end_vec_iter   = end_vec.begin();
    while ( begin_vec_iter != begin_vec.end() )
    { 
        if ( *begin_vec_iter == *end_vec_iter )
        {
            more_map_sets = false;
            break;
        }
        ++begin_vec_iter;  ++end_vec_iter;
    }             
};






// get next map set method
bool
RuleInstanceIter::get_next_maps ( Vector <Map> & maps, Vector <PatternGraph> & graphs )
{
    //std::cout << "get_next_maps : more sets? " << more_map_sets << std::endl;
    if ( !more_map_sets ) return false;
              
    // load reactant_maps
    std::pair < Map*, PatternGraph* > copy_pair;
    for ( iter_vec_iter = iter_vec.begin();  iter_vec_iter != iter_vec.end();  ++iter_vec_iter )
    {   // copy target graph and get a map to the copy
        copy_pair = (**iter_vec_iter).copy_map_and_target();
        maps.deposit_back( copy_pair.first );  
        graphs.deposit_back( copy_pair.second );
    }
                            
    // advance iterator
    iter_vec_iter  = iter_vec.begin();
    begin_vec_iter = begin_vec.begin();
    end_vec_iter   = end_vec.begin();
    // advance the iterator for the first map list
    ++(*iter_vec_iter);
    // if the iterator is pointing to the end of the list . . .
    while ( *iter_vec_iter == *end_vec_iter )
    {
        // point iterator to beginning of this map container
        *iter_vec_iter = *begin_vec_iter;
        // then get the next map container
        ++iter_vec_iter;  ++begin_vec_iter;  ++end_vec_iter;
        // see if we're at the end of the map containers
        if ( iter_vec_iter == iter_vec.end() )
        {   // if so, then no more map sets
            more_map_sets = false;
            break;
        }   
        // otherwise, advance to the next match in this list
        ++(*iter_vec_iter);
    }
    //std::cout << "get_next_maps : done" << std::endl;                
    // return a true result since a new reactant set was loaded
    return true;            
};






///////////////////
// Reaction Rule //
///////////////////
class ReactionRule
{   
    public:    
        // Constructor (safe version): Patterns and Transformations
        //   Copies reactant_patterns and transformations.
        template < class PatternContainerType, class TransformationContainerType >        
        ReactionRule ( PatternContainerType   & _reactant_patterns,
                       TransformationContainerType & _transformations,  size_t _n_products );
        
        // copy constructor
        ReactionRule ( const ReactionRule & x );
        // clone
        ReactionRule * clone ( ) const { return new ReactionRule(*this); };
        // destructor
        ~ReactionRule ( ) {};

        // get info about ReactionRule                
        size_t  num_reactants  ( ) const {  return n_reactants;  };
        size_t  num_products   ( ) const {  return n_products;   };
        size_t  num_transformations ( ) const {  return transformations.size();  };    
        Pattern & get_reactant ( size_t ir ) const {  return reactant_patterns[ir];  };
        LabeledList <Reaction> & get_reactions ( ) {  return reactions;  };
        
        // get reaction rule label
        std::string  get_label ( ) const {  return label;  };
        
        //Pattern & get_product  ( size_t ip ) const {  return product_patterns[ip];   };              
        //LabeledList <ReactionRule> & get_refinements ( ) {  return refinements;  };      

        // Given a list of new graphs, generate all possible reactions that include a match to
        //  a new graph and some combination of previously processed matches. Load any new species
        //  into the "product_graphs" list. Return value is the number of new reactions.     
        template < class GraphContainerType1, class GraphContainerType2 >
        size_t  generate_new_reactions ( GraphContainerType1 & new_graphs,
                                         GraphContainerType2 & product_graphs  );                                    

        // apply reaction rule to a set of graphs and maps from the reactant patterns into the graphs
        template < class MapContainerType, class GraphContainerType >
        bool  apply_in_situ ( MapContainerType & maps, GraphContainerType & graphs,
                              bool check_n_products, bool get_label, bool get_mechanism );

        // apply reaction rule to copies of these graphs and maps from the reactant patterns into the graphs
        //  NOTE: products should be empty when passed as an argumnet
        template < class MapContainerType, class GraphContainerType >
        bool  apply_to_copy ( MapContainerType & maps, GraphContainerType & reactants, 
                              GraphContainerType & products, bool check_n_products,
                              bool get_label, bool get_mechanism                       );
                              
        // Generate all refined rules by splicing patch graphs over all reactant patterns.
        //  Load resulting rules into a List provided by argument "refined_rules".
        size_t  refine_rule ( List <PatternGraph> &         patches,
                              List <ReactionRule> &  refined_rules  );  

        // Generate a context-free rule for each basic operation in a rule
        //  and load rules into the container "elem_rules".
        template < class RuleContainerType >        
        size_t  get_elementary_rules ( RuleContainerType & elem_rules );  
        
        // print reactants and transformations
        void         print_rule ( );
        std::string  get_BNG2_string ( );     
                                              
    protected:
        // Constructor (unsafe version)
        //   Steals reactant patterns and transformations (these Containers are empty on return!)
        template < class PatternContainerType, class TransformationContainerType >        
        ReactionRule ( PatternContainerType   * _reactant_patterns,
                       TransformationContainerType * _transformations, size_t _n_products );
        
        // given a vector of new maps to graphs that match reactant pattern "nr", generate all reactions
        // with a new map at reactant "nr" and some combination of previous matches.  Any product
        // graphs are loaded into the "product_graphs" list.  Return value is the number of new
        // reactions.
        template < class GraphContainerType >
        size_t  generate_new_reactions ( size_t ir, List <Map>         & new_maps,
                                                    GraphContainerType & product_graphs );

        // Given a complete set of maps to reactants, along with the target graphs, apply the rule
        //  transformations and construct a corresponding Reaction object. Find canonical mechanism and reaction 
        //  labels and add reaction to the list.  Product graphs are loaded into the "graphs" container on return.
        //  The return value is true if the reaction was not previously in the list.
        template < class MapContainerType1, class GraphContainerType2 >
        bool    construct_reaction ( MapContainerType1 & maps, GraphContainerType2 & graphs  );

        // Generate refined rules by splicing patch graphs over reactant pattern "ir".
        //  Load resulting rules into a List provided by argument "refined_rules".
        void  refine_rule ( size_t  ir,
                            List <PatternGraph> &         patches,
                            List <ReactionRule> &  refined_rules  );
                           
        // refine a rule by substituting a reactant pattern with a more specific pattern     
        ReactionRule * refine_rule ( size_t  ir,
                                     Pattern * refined_reactant,
                                     Map *     refined_map       );    

        // generate canonical label for this rule (constructors call this!)
        void  generate_label ( );    
        
    private:    
        // members
        Vector < PatternGraph >         reactant_patterns;     
        Vector < PatternGraph >         product_patterns; 
        Vector < Transformation >       transformations;
        
        node_container_t           reaction_center;
        LabeledList < Reaction >   reactions;
        //LabeledList < ReactionRule >  refinements;
        const size_t               n_reactants;
        const size_t               n_products;
        std::string                label;
        std::set < std::string >   mechanisms;
};






// Constructor (safe version)
//   Copies reactant_patterns and transformations.
template < class PatternContainerType, class TransformationContainerType >        
ReactionRule::ReactionRule ( PatternContainerType   & _reactant_patterns,
                             TransformationContainerType & _transformations, size_t _n_products )
  : n_reactants ( _reactant_patterns.size() ),
    n_products  ( _n_products )
{
    typename PatternContainerType::iterator    rg_iter;
    typename TransformationContainerType::iterator  ops_iter;
    std::pair < Pattern*, Map* >  copy_pair;    
                  
    // copy reactant graphs and get mapping from originals to copy
    Vector <Map>  copy_maps;
    for ( rg_iter = _reactant_patterns.begin();  rg_iter != _reactant_patterns.end();  ++rg_iter )
    {
        // copy reactant graph and push copy map onto container
        copy_pair = rg_iter->copy_and_get_map();
        reactant_patterns.deposit_back ( copy_pair.first );
        copy_maps.deposit_back ( copy_pair.second );
    }
      
    // aggregate copy mappings                
    Map  aggregate_map;   
    aggregate_map.merge( copy_maps );
    
    // copy transformations into ReactionRule
    for ( ops_iter = _transformations.begin();  ops_iter != _transformations.end();  ++ops_iter )
        transformations.deposit_back ( ops_iter->clone_and_remap( aggregate_map ) );

    // identify reaction center
    for ( ops_iter = transformations.begin();  ops_iter != transformations.end();  ++ops_iter )
        ops_iter->get_center( reaction_center );
        
    // get label
    generate_label();
       
};




// Constructor (unsafe version)
//   Steals reactant patterns and transformations (these Containers are empty on return!)
template < class PatternContainerType, class TransformationContainerType >        
ReactionRule::ReactionRule ( PatternContainerType   * _reactant_patterns,
                             TransformationContainerType * _transformations, size_t _n_products )
  : n_reactants ( _reactant_patterns->size() ),
    n_products  ( _n_products )
{
    // local variables
    Vector <Transformation>::iterator  ops_iter;

    // move reactant patterns into this rule
    reactant_patterns.annex( *_reactant_patterns );
    // move transformations into this rule    
    transformations.annex( *_transformations );   

    // identify reaction center
    for ( ops_iter = transformations.begin();  ops_iter != transformations.end();  ++ops_iter )
        ops_iter->get_center( reaction_center );
        
    // get label
    generate_label();        
};




// copy constructor
ReactionRule::ReactionRule ( const ReactionRule & x )
  : n_reactants( x.n_reactants ),
    n_products(  x.n_products  )
{
    Vector <Pattern>::const_iterator    patt_iter;
    Vector <Transformation>::const_iterator  ops_iter;
    std::pair < Pattern*, Map* >        copy_pair;    
                   
    // copy reactant graphs and get mapping from originals to copy
    Vector <Map>  copy_maps;
    for ( patt_iter = x.reactant_patterns.begin();  patt_iter != x.reactant_patterns.end();  ++patt_iter )
    {
        // copy reactant graph and push copy map onto container
        copy_pair = patt_iter->copy_and_get_map();
        reactant_patterns.deposit_back ( copy_pair.first );
        copy_maps.deposit_back ( copy_pair.second );
    }
        
    // aggregate copy mappings                
    Map  aggregate_map;   
    aggregate_map.merge( copy_maps );
        
    // copy transformations into ReactionRule
    for ( ops_iter = x.transformations.begin();  ops_iter != x.transformations.end();  ++ops_iter )
        transformations.deposit_back ( ops_iter->clone_and_remap( aggregate_map ) );

    // copy reaction center
    aggregate_map.image( x.reaction_center.begin(), x.reaction_center.end(), reaction_center );

    // copy reactions
    reactions = x.reactions;
    // copy label
    label = x.label;
    // copy mechanisms;
    mechanisms = x.mechanisms;
    // copy products
    product_patterns = x.product_patterns;
};






// Given a list of new graphs, generate all possible reactions that include a match to
//  a new graph and some combination of previously processed matches. Load any new species
//  into the "product_graphs" list. Return value is the number of new reactions.     
template < class GraphContainerType1, class GraphContainerType2 >
size_t
ReactionRule::generate_new_reactions ( GraphContainerType1 & new_graphs,
                                       GraphContainerType2 & product_graphs  )
{
    size_t         ir;
    size_t         n_reactions;
    List <Map>     new_maps;
    n_reactions = 0;
    for ( ir = 0;  ir != n_reactants;  ++ir )
    {
        // Find new maps from reactant_pattern[ir] into graphs in 'new_graphs'.
        //  Save the maps in 'new_maps" container.
        reactant_patterns[ir].find_maps( new_graphs, new_maps );
        // apply the rule to the 'new_maps' (clears out new matches as a side effect!)
        //   load resulting products into 'product_graphs'    
        n_reactions += generate_new_reactions ( ir, new_maps, product_graphs );
        // output status
        //std::cout << "ir: " << ir << "  new reactions: " << n_reactions << std::endl;        
    }
    return n_reactions;
};  






// given a vector of new maps to graphs that match reactant pattern "nr", generate all reactions
// with a new map at reactant "nr" and some combination of previous matches.  Any product
// graphs are loaded into the "product_graphs" list.  Return value is the number of new
// reactions.
template < class GraphContainerType >
size_t 
ReactionRule::generate_new_reactions ( size_t ir,
                                       List <Map>         & new_maps,
                                       GraphContainerType & product_graphs )
{
    #if BNGCORE_DEBUG_RULE==1             
    std::cout << "    generate_new_reactions : reactant_level = " << ir << std::endl;
    #endif
    
    // local variables
    Vector <Map>      reactant_maps;
    Vector <PatternGraph>    reactant_graphs;
    size_t            n_reactions_found = 0;
    bool              is_new_rxn;

/*
	// new stuff
	ReactionRule *                new_rxn;
	LabeledList <ReactionRule>::iterator  old_rxn_iter;
    Map                           aggregate_map;
    Vector <Transformation>            remapped_transformations;
    Vector <Transformation>::iterator  ops_iter;
    bool                          is_new_rxn;
    size_t                        n_rxns_found;
*/    
    // create reaction set iterator
    RuleInstanceIter  rs_iter( reactant_patterns, ir, new_maps );
    // generate new reactions that include the new_maps
    //  ..load next set of reactants into "reactant_maps" and "reactant_graphs" containers
    while ( rs_iter.get_next_maps( reactant_maps, reactant_graphs ) )
    {      
        #if BNGCORE_DEBUG_RULE==1
        std::cout << "REACTANTS: " << std::endl;
        for ( size_t ii = 0;  ii < n_reactants;  ++ii )
            reactant_graphs[ii].print_graph();
        #endif
/*
// BEGIN NEW
	    // merge reactant maps
	    aggregate_map.merge( reactant_maps );
	
	    // copy transformations and remap to reactants
	    for ( ops_iter = transformations.begin();  ops_iter != transformations.end();  ++ops_iter )
	        remapped_transformations.deposit_back( ops_iter->clone_and_remap( aggregate_map ) );  
    
	    // create new reaction rule (without copy) and return
	    new_rxn = new ReactionRule ( &reactant_graphs, &remapped_transformations, n_products );

   	    // check number of product graphs
        if ( n_products != new_rxn->product_patterns.size() )
        {
            delete new_rxn;
            continue;	
        }
    	
        // try to add reaction to reaction list         
        is_new_rxn = refinements.deposit_back ( new_rxn );
     
        if ( !is_new_rxn )
        { 	// try to add this mechanism
            old_rxn_iter = refinements.find( new_rxn->get_label() );
            is_new_mech = old_rxn_iter->mechanisms.insert ( new_rxn->mechanisms.begin(), 
                                                             new_rxn->mechanisms.end()    );
            delete new_rxn;        	
    	}
    	
        #if BNGCORE_DEBUG_RULE==1
        std::cout << "PRODUCTS: " << std::endl;
        for ( size_t ii = 0;  ii < n_products;  ++ii )
            new_rxn->product_graphs[ii].print_graph();
        #endif
    
        if ( is_new_rxn )
        {   ++n_reactions_found;         
            // copy products over to the products graph container
            product_graphs += new_rxn->product_graphs;
        }
// END NEW
*/

        // apply transformations and return a reaction object
        //  on return, reactant graphs have been replaced with product graphs!
        is_new_rxn = construct_reaction ( reactant_maps, reactant_graphs );
            
        #if BNGCORE_DEBUG_RULE==1
        std::cout << "PRODUCTS: " << std::endl;
        for ( size_t ii = 0;  ii < n_products;  ++ii )
            reactant_graphs[ii].print_graph();
        #endif
    
        if ( is_new_rxn )
        {   ++n_reactions_found;         
            // move products over to the products graph container
            product_graphs.annex( reactant_graphs );
        }
        else
            // we've seen this reaction before.. no new products.
            reactant_graphs.clear();

    }
    
    // finally, add the new_maps to the set of reactant_matches (for following netgen iterations)
    //   ..this clears out new_maps so it's empty on return
    reactant_patterns[ir].get_maps().annex( new_maps );
    #if BNGCORE_DEBUG_RULE==1                
    std::cout << "    generate new reactions : DONE." << std::endl;    
    #endif
    return n_reactions_found;        
};





// TODO: create new rule instead
// Given a complete set of maps to reactants, along with the target graphs, apply the rule
//  transformations and construct a corresponding Reaction object. Find canonical mechanism and reaction 
//  labels and add reaction to the list.  Product graphs are loaded into the "graphs" container on return.
//  The return value is true if the reaction was not previously in the list.
template < class MapContainerType, class GraphContainerType >
bool
ReactionRule::construct_reaction ( MapContainerType & maps, GraphContainerType & graphs  )
{
    #if BNGCORE_DEBUG_RULE==1
    std::cout << "construct reaction" << std::endl;
    #endif
  
    // local variables                     
    Reaction     *rxn;
    size_t       ii;
    Map          map;
    PatternGraph        graph;
    std::string  canonical_mechanism;
    std::string  canonical_reaction;
    bool         is_new_rxn;
    bool         is_new_mech;
    
    // create new reaction
    rxn = new Reaction();
    for ( ii = 0;  ii < graphs.size();  ++ii )
        rxn->add_reactant ( graphs[ii] );
    
    // merge maps and graphs into aggregates     
    map.merge   ( maps   );        
    graph.merge ( graphs );    
    map.set_target_graph( &graph );
                
    //graph.print_graph();
    //map.print_map();
        
    // tag reactant graphs with operation flags                 
    for ( ops_iter_t  op_iter = transformations.begin();  op_iter != transformations.end();  ++op_iter )
        op_iter->apply_transformations( map );
    
    #if BNGCORE_DEBUG_RULE==1
    std::cout << "operation flagged graph:" << std::endl;
    graph.print_graph();
    #endif

    // get canonical mechanism    
    canonical_mechanism = graph.get_label( true );
    // get canonical reaction    
    graph.reset_index();
    canonical_reaction  = graph.get_label( false );
    rxn->set_label ( canonical_reaction );
    
    // finalize the reaction graph by applying the flagged transformations 
    graph.apply_transformations();

    #if BNGCORE_DEBUG_RULE==1
    std::cout << "post-operation graph:" << std::endl;
    graph.print_graph();
    std::cout << "canonical_reaction:  " << canonical_reaction << std::endl;
    std::cout << "canonical_mechanism: " << canonical_mechanism << std::endl;
    #endif


    // split connected compontns and put them back on graphs  
    graph.split_components ( graphs );
    #if BNGCORE_DEBUG_RULE==1
    std::cout << "number of products: " << graphs.size() << "  expected: " << n_products << std::endl;
    #endif
                
    // check number of product graphs
    if ( n_products != graphs.size() )  return false;
    
    // add reaction to reaction list         
    is_new_rxn = reactions.deposit_back ( rxn );
    
    if ( is_new_rxn )
    {   // add mechanism        
        is_new_mech = rxn->add_mechanism ( canonical_mechanism );
        // load products into reaction
        for ( ii = 0;  ii < graphs.size();  ++ii )  rxn->add_product ( graphs[ii] );
    }    
    else
    {   // delete this copy of the reaction
        delete rxn;
        // try to add this mechanism
        is_new_mech = reactions.find( canonical_reaction )->add_mechanism( canonical_mechanism );
    }
 
    #if BNGCORE_DEBUG_RULE==1
    std::cout << "canonical_mechanism:\n" << canonical_mechanism << "\n" << std::endl;    
    std::cout << "canonical_reaction:\n"  << canonical_reaction  << "\n" << std::endl;
    std::cout << "new_reaction?  " << is_new_rxn  << std::endl;
    std::cout << "new_mechanism? " << is_new_mech << std::endl;
    #endif   
    
    return is_new_rxn;
};






// apply reaction rule to copies of these graphs and maps from the reactant patterns into the graphs
//  NOTE: products should be empty when passed as an argumnet
template < class MapContainerType, class GraphContainerType >
bool
ReactionRule::apply_to_copy ( MapContainerType & maps, GraphContainerType & reactants, GraphContainerType & products,
                               bool check_n_products, bool get_label, bool get_mechanism  )
{
    // local variables
    MapContainerType                        copy_maps;   
    typename  MapContainerType::iterator    map_iter;
    typename  GraphContainerType::iterator  graph_iter;
    std::pair <Pattern*, Map*>              copy_pair;

    // clear products!
    products.clear();

    // copy reactants and collect maps to copies
    map_iter = maps.begin();
    graph_iter = reactants.begin();
    while (  graph_iter != reactants.end()  )
    {
        // copy reactants and get map from original to copy
        copy_pair = graph_iter->copy_and_get_map();
        products.deposit_back( copy_pair.first );
        copy_maps.deposit_back( copy_pair.second->mapf( *map_iter ) );
        // next loop
        ++map_iter;
        ++graph_iter;
    }
    
    // apply rule to copies and return
    return  apply_in_situ ( copy_maps, products, check_n_products, get_label, get_mechanism );
}






// apply reaction rule to a set of graphs and maps from the reactant patterns into the graphs
template < class MapContainerType, class GraphContainerType >
bool
ReactionRule::apply_in_situ ( MapContainerType & maps, GraphContainerType & graphs,
                              bool check_n_products, bool get_label, bool get_mechanism  )
{
    #if BNGCORE_DEBUG_RULE==1
    std::cout << "apply_in_situ" << std::endl;
    #endif
       
    // local variables                     
    Map        map;
    PatternGraph      graph;

    // merge maps and graphs into aggregates     
    map.merge   ( maps   );             
    graph.merge ( graphs );;         
    map.set_target_graph( &graph );
    ;    
    // tag reactant graphs with operation flags                 
    for ( ops_iter_t  op_iter = transformations.begin();  op_iter != transformations.end();  ++op_iter )
        op_iter->apply_transformations( map );
       
    #if BNGCORE_DEBUG_RULE==1
    std::cout << "operation flagged graph:" << std::endl;
    graph.print_graph();
    #endif

    // get canonical mechanism
    if ( get_mechanism )    
        mechanisms.insert( graph.get_label( true ) );
    
    // get canonical reaction    
    if ( get_label )
    {
        graph.reset_index();
        label = graph.get_label( false );
    }
      
    // finalize the reaction graph by applying the flagged transformations 
    graph.apply_transformations();

    #if BNGCORE_DEBUG_RULE==1
    std::cout << "product graph:" << std::endl;
    graph.print_graph();
    #endif
 
    // split connected components and put them back on graphs  
    graph.split_components ( graphs );
    #if BNGCORE_DEBUG_RULE==1
    std::cout << "number of products: " << graphs.size() << "  expected: " << n_products << std::endl;
    #endif
              
    // check number of product graphs
    if ( check_n_products )
    {
        if ( n_products != graphs.size() )
            return false;
    }
       
    return true;
}






// Generate all refined rules by splicing patch graphs over all reactant patterns
// NOTE: refined rules is cleared at the start!!
size_t
ReactionRule::refine_rule ( List <PatternGraph>         & patches,
                            List <ReactionRule>  & refined_rules  )
{
    // local variables
    size_t                          ii;
    List <ReactionRule>             working_rules;
    List <ReactionRule>::iterator   rule_iter;

    // begin with "this" in working_rules
    refined_rules.clear();
    refined_rules.push_back( *this );

    // refine rules iteratively
    //   ..loop over reactant patterns.
    for ( ii = 0;  ii < n_reactants;  ++ii )
    {
        // move refined rules from previous iteration into working_rules
        working_rules.annex( refined_rules );
        
        // refine each rule working list
        for ( rule_iter = working_rules.begin();  rule_iter != working_rules.end();  ++rule_iter )
            rule_iter->refine_rule( ii, patches, refined_rules );
    
        // clear working rules
        working_rules.clear();
    }
    
    // return number of refined rules
    return refined_rules.size();
}






// Generate all refined rules by splicing patch graphs over reactant pattern "ir".
//  Load resulting rules into a List provided by argument "refined_rules".
void
ReactionRule::refine_rule ( size_t  ir,
                            List <PatternGraph>         & patches,
                            List <ReactionRule>  & refined_rules  )
{
    // local variables
    List <Pattern>  refined_reactants;
    List <Map>      refined_maps;
    
    // generate patchworks for reactant pattern "ir"
    //reactant_patterns[ir].patchwork( reactant_patterns[ir], patches, refined_reactants, refined_maps );
    patchwork( reactant_patterns[ir], patches, reaction_center, refined_reactants, refined_maps );
    
    // for each patchwork, generate a rule that is refined by this patchwork at
    //  reactant pattern "ir"
    while ( !(refined_reactants.empty()) )
    {
        // NOTE: memory deallocation for the withdrawn reactant and map is
        //  handled by the method refine_rule.
        refined_rules.deposit_back
        (
            refine_rule( ir, refined_reactants.withdraw_front(),
                             refined_maps.withdraw_front()       )
        );
    }
}






// refine a rule by substituting a reactant pattern with a more specific pattern     
ReactionRule *
ReactionRule::refine_rule ( size_t  ir,
                            Pattern * refined_reactant,
                            Map     * refined_map       )
{
    // local variables
    size_t                        ii;
    Vector <Pattern>              new_patterns;
    Vector <Transformation>            new_transformations;
    Map                           aggregate_map;
    std::pair <Pattern*, Map*>    copy_pair;    
    Vector <Map>                  copy_maps;    
    Vector <Transformation>::iterator  ops_iter;
        
    // Copy reactants (substitute refined reactant at "ir".
    for ( ii = 0;  ii < n_reactants;  ++ii )
    {
        if ( ii == ir )
        {   // use the refined reactant here
            new_patterns.deposit_back( refined_reactant );
            copy_maps.deposit_back( refined_map );
        }
        else
        {   // copy reactant and get map from original to copy
            copy_pair = reactant_patterns[ii].copy_and_get_map();
            new_patterns.deposit_back( copy_pair.first );
            copy_maps.deposit_back( copy_pair.second );
        }
    }
    
    // merge copy maps
    aggregate_map.merge( copy_maps );

    // copy transformations and remap to refined reactant patterns
    for ( ops_iter = transformations.begin();  ops_iter != transformations.end();  ++ops_iter )
        new_transformations.deposit_back( ops_iter->clone_and_remap( aggregate_map ) );  
    
    // create new reaction rule (without copy) and return
    return  new ReactionRule ( &new_patterns, &new_transformations, n_products );
};






// Generate a context-free rule for each basic operation in a rule
//  and load rules into the container "elem_rules".
template < class RuleContainerType >
size_t
ReactionRule::get_elementary_rules ( RuleContainerType & elem_rules )
{
    Vector <Transformation>::iterator     ops_iter;
    Vector <Pattern>::iterator       patt_vec_iter;    
    List <Pattern>::iterator         patt_iter;
    List <Pattern>::iterator         curr_patt;
    node_iter_t                      node_iter;
    std::pair < Pattern*, Map* >     copy_pair;
    Vector < Map >                   copy_maps;
    Transformation *                      new_operation;
      
    for ( ops_iter = transformations.begin();  ops_iter != transformations.end();  ++ops_iter )
    { 
        // list to hold a copy of the patterns (for modification)
        List <Pattern>    new_patterns;      
        List <Transformation>  new_transformations;
        Map               aggregate_map;
        node_container_t  center_nodes;
        // copy reactant patterns
        for ( patt_vec_iter = reactant_patterns.begin();  patt_vec_iter != reactant_patterns.end();  ++patt_vec_iter )
        {
            // copy reactants and get map from original to copy
            copy_pair = patt_vec_iter->copy_and_get_map();
            new_patterns.deposit_back( copy_pair.first );
            copy_maps.deposit_back( copy_pair.second );        
        }
        // merge copy maps
        aggregate_map.merge( copy_maps );
        // copy operation and remap
        new_operation = ops_iter->clone_and_remap( aggregate_map );
        // get center nodes     
        new_operation->get_center( center_nodes );
        // move to next operation if there are no center nodes
        //if ( center_nodes.empty() )  continue;
        
        // trim reactants down to essential nodes!
        patt_iter = new_patterns.begin();
        while ( patt_iter != new_patterns.end() )
        {
            // a container to hold nodes which will be deleted
            node_container_t  delete_nodes;
               
            // find nodes in pattern that are not part of the center
            for ( node_iter = patt_iter->nodes_begin();  node_iter != patt_iter->nodes_end();  ++node_iter )
            {
                if (  center_nodes.find( *node_iter ) == center_nodes.end()  )
                    delete_nodes.insert( *node_iter );
            }
            
            // delete nodes that are not part of center
            for ( node_iter = delete_nodes.begin();  node_iter != delete_nodes.end();  ++node_iter )
                patt_iter->delete_node( *node_iter );
            
            if ( patt_iter->nodes_empty() )
            {   // remove empty patterns
                curr_patt = patt_iter;
                ++patt_iter;
                new_patterns.erase( curr_patt );
            }
            else
                ++patt_iter;
        }        
        
        // construct simplified rule
        new_transformations.deposit_back( new_operation );
        // TODO: figure out how to handle different number of products?
        elem_rules.deposit_back(  new ReactionRule ( &new_patterns, &new_transformations, n_products )  );
    }
    
    return elem_rules.size();
}





// Generate label for this reaction rule.
//   Construct reaction graph by applying transformations to reactants.
//   Get canonical label for reaction graph.
//   Then generate products and store.
void
ReactionRule::generate_label ()
{
    Vector <Pattern>::iterator  patt_iter;
    Vector <Map>  automaps;    
    
    // get reactant patterns
    for ( patt_iter = reactant_patterns.begin();  patt_iter != reactant_patterns.end();  ++patt_iter )
        automaps.deposit_back( new Map(*patt_iter) );

    // Apply rule transformations to reactant patterns to get product patterns
    //   Be sure to ask "apply" method to generate labels!
    apply_to_copy ( automaps, reactant_patterns, product_patterns, false, true, true );  
}




// print reactants and transformations
void
ReactionRule::print ( )
{
    std::cout << "\n>> Reaction Rule (n_reactants: " << n_reactants
              << ", n_products: " << n_products << ")" << std::endl;
    
    // print reactants
    size_t  ii = 0;
    Vector <PatternGraph>::iterator  patt_iter;
    for ( patt_iter = reactant_patterns.begin();  patt_iter != reactant_patterns.end();  ++patt_iter )
    {
        std::cout << "reactant index: " << ii << std::endl;
        patt_iter->print();
        ++ii;
    }
    
    // print transformations!
    Vector <Transformation>::iterator  trans_iter;
    for ( trans_iter = transformations.begin();  trans_iter != transformations.end();  ++trans_iter )
        trans_iter->print();
    
    std::cout << std::endl;
};






// get BNG2 string for this rule
std::string
ReactionRule::get_BNG2_string ( )
{
    std::stringstream s;
    bool  first_pattern;
    Vector <Pattern>::iterator  patt_iter;  
    
    // get reactant patterns
    first_pattern = true;
    for ( patt_iter = reactant_patterns.begin();  patt_iter != reactant_patterns.end();  ++patt_iter )
    {
        if ( first_pattern )  first_pattern = false;
        else  s << " + ";
        // get string for this pattern
        s << patt_iter->get_BNG2_string();
    }
   
    // add arrow to string
    s << " -> ";
   
    // get product patterns
    first_pattern = true;
    for ( patt_iter = product_patterns.begin();  patt_iter != product_patterns.end();  ++patt_iter )
    {
        if ( first_pattern )  first_pattern = false;
        else  s << " + ";
        // get string for this pattern
        s << patt_iter->get_BNG2_string();
    }
      
    return s.str();
};


} /* end of BNGcore namespace */

#endif /* BNGCORE_REACTIONRULE_HPP_ */
