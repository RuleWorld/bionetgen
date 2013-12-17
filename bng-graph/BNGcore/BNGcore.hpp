/*
 * BNGcore.hpp
 *
 *  Created on: May 11, 2010
 *  Last Update: Dec 12, 2012
 *      Author: Justin
 */



#ifndef BNGCORE_HPP_
#define BNGCORE_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <list>

#include "List.hpp"
#include "Vector.hpp"


// set digraph option for nauty
#define  BNGCORE_NAUTY_DIGRAPH  0
// set debug option for nauty
#define  BNGCORE_DEBUG_NAUTY    0
// set debug option for Reactionrule
#define  BNGCORE_DEBUG_RULE     0
// set debug option for Overlap
#define  BNGCORE_DEBUG_OVERLAP  0


namespace BNGcore
{
    // forward declarations for cyclic dependencies
    class State;
    class NodeType;
    class Node;  
    class PatternGraph;
    class Map;
    class Transformation;
    
    class NodeFunction; 
    class ConstantNodeFunction;
    class BondNodeFunction;
   
    
    // useful type definitions
    typedef  std::pair <Node*, Node*>          node_pair_t;

    // Node Container
    typedef  std::vector <Node*>               node_container_t;
    typedef  node_container_t::iterator        node_iter_t;
    typedef  node_container_t::const_iterator  node_const_iter_t;

    // Node vector (stl)
    typedef  std::vector <Node*>               node_vector_t;
    typedef  node_vector_t::iterator           node_vector_iter_t;
    typedef  node_vector_t::const_iterator     node_vector_const_iter_t;

    // Mappinf: Node->Node
    typedef  std::map <Node*, Node*>           nodemap_t;
    typedef  nodemap_t::iterator               nodemap_iter_t;
    typedef  nodemap_t::const_iterator         nodemap_const_iter_t;
    
    // Map Container
    typedef  Vector < Map >                    map_container_t;
    typedef  map_container_t::iterator         map_iter_t;
    typedef  map_container_t::const_iterator   map_const_iter_t;

    // PatternGraph Container
    typedef  Vector < PatternGraph >                   patterngraph_container_t;
    typedef  patterngraph_container_t::iterator        patterngraph_iter_t;
    typedef  patterngraph_container_t::const_iterator  patterngraph_const_iter_t;

    // Transformation Container
    typedef  Vector < Transformation >                   transformation_container_t;
    typedef  transformation_container_t::iterator        transformation_iter_t;
    typedef  transformation_container_t::const_iterator  transformation_const_iter_t;
    
    // Map: string -> Map (for use with LabeledList <PatternGraph>)
    typedef  std::map  < std::string, std::list <PatternGraph*>::iterator >  label_map_t;
    typedef  label_map_t::iterator                                           label_map_iter_t;
    typedef  std::pair < std::string, std::list <PatternGraph*>::iterator >  label_map_element_t;

    // Types for Ullmann subgraph algorithm
    typedef  std::map < Node*, node_container_t* >   ullmann_M_t;
    typedef  ullmann_M_t::iterator                   row_iter_t;
    typedef  ullmann_M_t::reverse_iterator           row_rev_iter_t;       
    typedef  node_container_t::iterator              col_iter_t;
    
    // Types for the edge type maps in NodeType
    typedef  std::map < NodeType*, NodeFunction* >   typemap_t;
    typedef  typemap_t::iterator                     typemap_iter_t;
    typedef  typemap_t::const_iterator               typemap_const_iter_t;
    
    // A type for mapping link nodes to indices
    typedef std::map < const Node*, int >            link_index_t;


    ////
    ////
    ////


    // template for negation of equality operator
    template < class T >
    bool  operator!= ( const T & x1, const T & x2 )
    {   return !( x1 == x2 );   };


    ////
    ////
    ////
    

    // base class for StateTypes
    class StateType
    {
        public:
            StateType ( const std::string & _type_name )
               : type_name(_type_name), parent_type( 0 ) { }; 
            StateType ( const std::string & _type_name, const StateType & _parent_type )
               : type_name(_type_name), parent_type( &_parent_type ) { };            
            //StateType ( const StateType & _x )
            //   : type_name(_x.type_name), parent_type(_x.parent_type) { };
            //StateType & operator= (const StateType & _x) 
            //{ type_name = _x.type_name; parent_type = _x.parent_type; };
            virtual ~StateType ( ) { };

            // get new state object
            virtual State * get_state_object ( ) const;
    
            // state type equivalence operator
            bool  operator== ( const StateType & type2 ) const
            {   return get_label() == type2.get_label();   };

            // partial ordering operator (is this node a subtype of node2?)
            bool  operator< ( const StateType & type2 ) const;
            
            // less-than method for sorting (note that this isn't static!)
            bool  less ( const StateType & type2 ) const
            {   return ( type_name < type2.type_name );   };

            // check states
            virtual bool  check_state ( int state ) const                 { return false; };
            virtual bool  check_state ( const std::string & state ) const { return false; }; 
                        
            // get state_type label
            std::string  get_label ( ) const { return type_name; };

        protected:
            const std::string  type_name;
            const StateType    * const parent_type;
    };

    // a state type with a finite set of possible labels
    class NullStateType : public StateType
    {
        public:
            // constructor
            NullStateType ( ) : StateType("0") { };
            // destructor
            virtual ~NullStateType ( ) { };            
            // get new state object
            virtual State * get_state_object ( ) const;
    };

    // a state type with a finite set of possible labels
    class LabelStateType : public StateType
    {
        public:
            // constructor
            LabelStateType ( const std::string & type_name, const std::string & _default_state );
            // constructor with parent argument
            LabelStateType ( const std::string & type_name, const StateType & parent_type,
                             const std::string & _default_state );        
            // destructor
            virtual ~LabelStateType ( ) { };            
            // get new state object
            virtual State *  get_state_object ( ) const;
            // add an allowed label_state to this type  
            bool  add_state ( const std::string & state );
            // check a label_state is valid
            virtual bool  check_state ( const std::string & state ) const;
            
        protected:
            const std::string            default_state;
            std::set < std::string >     states;
    };

    class BondStateType : public LabelStateType
    {
        public:
            BondStateType ( );
            virtual ~BondStateType ( ) { };
        
            // get new state object
            virtual State *  get_state_object ( ) const;
            State *  get_state_object ( bool bound ) const;              
    };

    // a state type for non-negative integers
    class CounterStateType : public StateType
    {
        public:
            CounterStateType ( const std::string & type_name )
               : StateType( type_name ) { };        
            CounterStateType ( const std::string & type_name, const StateType & parent_type )
               : StateType( type_name, parent_type ) { };
            virtual ~CounterStateType ( ) { };

            // get new state object
            virtual State *  get_state_object ( ) const;
            // check if state value is valid
            virtual bool  check_state ( int state_value ) const { return (state_value >= 0); };          
    };

    
    ////
    ////
    ////

    
    class State
    {
        public:
            // constructors
            State ( const StateType & _state_type );
            State ( const State & x );
            virtual State * clone ( ) const { return new State(*this); };
            virtual ~State ( ) {};
            // get type
            const StateType &  get_type ( ) const { return *state_type; };
            // modify state
            virtual bool set_state ( const std::string & state_label ) {  return false;  };
            virtual bool set_state ( int state_value ) {  return false;  };
            virtual bool add ( int x ) {  return false;  };
            virtual bool increment ( ) {  return false;  };
            virtual bool decrement ( ) {  return false;  };
            // get labels, BNG2 string
            virtual std::string  get_label ( ) const;
            virtual std::string  get_BNG2_string ( ) const;         
            // test equality, compare for sorting
            virtual bool  operator== ( const State & state2 ) const;
            virtual bool  less ( const State & state2 ) const;
            virtual bool  is_wildcard ( ) const { return false; };

        protected:
            const StateType  *const state_type;
    };
    
    class NullState : public State
    {
        public:
            // constructors, etc
            NullState ( const NullStateType & _state_type );
            NullState ( const NullState & x );
            virtual NullState * clone ( ) const { return new NullState(*this); };
            virtual ~NullState ( ) {};
    };

    class LabelState : public State
    {
        public:
            // constructors, etc
            LabelState ( const LabelStateType & state_type, const std::string & _state );
            LabelState ( const LabelState & x );
            virtual LabelState * clone ( ) const { return new LabelState(*this); };
            virtual ~LabelState ( ) {};
            // set/modify state
            virtual bool  set_state ( const std::string & _state );   
            // get labels, BNG2 string
            virtual std::string  get_label ( ) const;
            virtual std::string  get_BNG2_string ( ) const; 
            // test equality, compare for sorting
            virtual bool  operator== ( const State & state2 ) const;
            virtual bool  less ( const State & state2 ) const;
            virtual bool  is_wildcard ( ) const;            
 
        protected:
            std::string  state;
    };

    class BondState : public LabelState
    {
        public:
            // constructors
            BondState ( const BondStateType & state_type, const std::string & state );         
            BondState ( const BondState & x );
            virtual BondState * clone ( ) const { return new BondState(*this); };
            virtual ~BondState ( ) {};
            // get labels, BNG2 string
            virtual std::string  get_label ( ) const;
            virtual std::string  get_BNG2_string ( ) const;
    };
    
    class CounterState : public State
    {
        public:
            // constructors,etc
            CounterState ( const CounterStateType & _state_type, int _state );
            CounterState ( const CounterState & x );                         
            virtual CounterState * clone ( ) const { return new CounterState(*this); };
            virtual ~CounterState ( ) {};
            // set/modify state value
            virtual bool set_state ( int new_state_value );
            virtual bool add ( int x );
            virtual bool increment ( );
            virtual bool decrement ( );            
            // test equivalence or compare for sorting
            virtual bool  operator== ( const State & state2 ) const;
            virtual bool  less ( const State & state2 ) const;
            // get labels, BNG2 string
            virtual std::string  get_label ( ) const;
            virtual std::string  get_BNG2_string ( ) const; 
    
        protected:
            int  state;
    };


    ////
    ////
    ////


    class NodeType
    {    
        public:
            // constructor 
            NodeType ( const std::string & type_name, const StateType & _state_type );
            // constructor w/ parent type
            NodeType ( const std::string & type_name, const NodeType & parent_type,
                                                      const StateType & state_type  ); 
            // destructor
            virtual ~NodeType ( ) { };
            
            // equivalent operator
            bool  operator== ( const NodeType & type2 ) const;
            // less than comparator for sorting
            bool  less ( const NodeType & type2 ) const;
            // partial ordering operator (not same as sorting less than!)
            bool  operator< ( const NodeType & type2 ) const;
            
            // get basic info
            const std::string &  get_type_name     ( ) const { return type_name;     };
            const NodeType &     get_parent_type   ( ) const { return *parent_type;  };                     
            const StateType &    get_state_type    ( ) const { return *state_type;   };  
            std::string          get_label         ( ) const;
            bool                 get_instance_flag ( ) const { return instance_flag; }; 

            // set instance flag
            void  set_instance_flag ( bool value ) { instance_flag = value; };

            // check a state
            virtual bool  check_state ( const State & state ) const
            {  return ( get_state_type() == state.get_type() );  };

            // add allowed edges
            bool add_edges_in  ( NodeType & node_type, int multiplicity );
            bool add_edges_in  ( NodeType & node_type, NodeFunction & nodefcn );
            bool add_edges_out ( NodeType & node_type, int multiplicity );         
            bool add_edges_out ( NodeType & node_type, NodeFunction & nodefcn );
                  
            // get const_iterators for the edge typemaps
            typemap_const_iter_t  edges_in_begin  ( ) const {  return edges_in.begin();   };
            typemap_const_iter_t  edges_in_end    ( ) const {  return edges_in.end();     };
            typemap_const_iter_t  edges_out_begin ( ) const {  return edges_out.begin();  };
            typemap_const_iter_t  edges_out_end   ( ) const {  return edges_out.end();    };            
        
            // write NodeType as a BNG2 string            
            virtual std::string  get_BNG2_string ( bool instance ) const;
            
        protected:
        private:
            const std::string        type_name;
            const NodeType * const   parent_type;
            const StateType * const  state_type;
            bool                     instance_flag;     
            typemap_t                edges_in;
            typemap_t                edges_out;
            typemap_iter_t           type_iter;
    };  

    ///
    ///

    class EntityType : public NodeType
    {
        public:
            EntityType ( const std::string & type_name,
                         const NodeType & parent_type, const StateType & state_type );
            virtual ~EntityType ( ) { };
            
            virtual std::string  get_BNG2_string ( bool instance ) const;
    };

    ///
    ///

    class BondType : public NodeType
    {
        public:    
            BondType ( BondNodeFunction & typing_fcn );
            virtual ~BondType ( ) { };

            virtual std::string  get_BNG2_string ( bool instance ) const;
    };

          
    ////
    ////
    ////    
    
    
    // A class that defines graph nodes (i.e. vertices).
    //  Each node contains knows its neighbors, which are stored in an adjacency lists (implemented as sets)s.
	class Node
	{	        
		public:
            // structors
			Node ( const NodeType & _type );
			Node ( const Node & orig );
			Node * clone ( ) const { return new Node(*this); };
			~Node ( );

		    // get methods
            const NodeType &  get_type  ( ) const {  return *type;   };
            State &           get_state ( )       {  return *state;  };
            const State &     get_state ( ) const {  return *state;  };        
            int               get_index ( ) const {  return index;   };                
            // set methods
            bool  set_state ( const State & new_state );
            void  set_index ( int _index );
            // query in and out degree of node
            size_t  in_degree   ( ) const { return edges_in.size();  };
            size_t  out_degree  ( ) const { return edges_out.size(); };
            // add and delete edges
            void  add_edge_in     ( Node * node );
            void  add_edge_out    ( Node * node );
            bool  delete_edge_in  ( Node * node );
            bool  delete_edge_out ( Node * node );
            
            // check two nodes for local equivalence (checks type and state)
            bool  operator== ( const Node & node2 ) const;                  
            // TODO: implement comparison operator  
  
            // compare nodes for canonical sorting (compares type, state, and index)
            static bool  less ( const Node * node1, const Node * node2 );
            // compare nodes for index sorting (compares index only)
            static bool  less_by_index ( const Node * node1, const Node * node2 );
            // validate typing of this node
            bool  validate_typing ( bool instance ) const;

            // get node label (for canonical labeling)
            std::string  get_label ( ) const;
            // write node as a BNG2 string
            std::string  get_BNG2_string ( const link_index_t & link_index ) const;

            // get iterators to look at edges
            node_const_iter_t  edges_in_begin  ( ) const { return edges_in.begin();  };
            node_const_iter_t  edges_in_end    ( ) const { return edges_in.end();    };
            node_const_iter_t  edges_out_begin ( ) const { return edges_out.begin(); };
            node_const_iter_t  edges_out_end   ( ) const { return edges_out.end();   };    
            // find edges
            node_const_iter_t  find_in_edge    ( Node * node ) const;  
            node_const_iter_t  find_out_edge   ( Node * node ) const;  
		protected:
		private:
		    // members
            const NodeType      * const type;
            State               * state;
            node_container_t    edges_in;
            node_container_t    edges_out;   		
		    mutable int         index;
		    // static iterators
		    static node_iter_t  node_iter;
		    static node_iter_t  nodes_end;
	};



    //////////////////
    // PatternGraph //
    //////////////////
	class PatternGraph
	{
		public:
		    // structors
			PatternGraph ( );
			PatternGraph ( const PatternGraph & source );
            PatternGraph * clone ( ) const { return new PatternGraph(*this); };
			~PatternGraph ( );
            
            // copy graph and get map from original (this) to the copy
            std::pair < PatternGraph*, Map* >   copy_and_get_map ( ) const;
            
            // Build a tree graph with nodetype at root and all its descendents below
            //  Returns a reference to the root node.
            Node *  build_tree_graph ( const NodeType & nodetype );

		    // methods for manipulation of graph
            Node *  add_node       ( Node & node );  // safest way, since node is copied
            bool    delete_node    ( Node * node );
            bool    add_edge       ( Node * node1, Node * node2 );
            size_t  delete_edge    ( Node * node1, Node * node2 );
            bool    set_node_state ( Node * node, const State & state );            
            // add node by pointer (use carefully! once the pointer is passed to the graph,
            //   memory deallocation is handled by graph)
		    void    add_node ( Node * node );
		    
            // query canonical status, or reset canonical indexing
            bool  is_canonical ( ) const;
            void  reset_index ( ) const;  

            // write graph to a string
            std::string  get_BNG2_string ( ) const;
            std::string  get_label ( ) const ;                
            std::string  get_label ( bool preserve_prior_order ) const;    
            // print detailed graph information
            void   print ( ) const;
            
            // direct access to nodes (careful!!)
            node_iter_t        begin ( ) {  return nodes.begin();  };
            node_iter_t        end   ( ) {  return nodes.end();    };
            // safer access to nodes
            node_const_iter_t  begin ( ) const {  return nodes.begin();  };
            node_const_iter_t  end   ( ) const {  return nodes.end();    };
            size_t             size  ( ) const {  return nodes.size();   };
            bool               empty ( ) const {  return nodes.empty();  };

		    // gather nodes for molecule and species deletions
		    void  gather_subtree   ( Node * curr_node, node_container_t & visited, node_container_t & bonds );
		    void  gather_connected ( Node * curr_node, node_container_t & visited );

            // validate a PatternGraph
            bool validate ( bool instance ) const;
            bool validate_instance_types ( ) const;           
            // check if overlap_map embeds this graph in the target graph
            bool  check_embedding ( const Map & overlap_map ) const;
            // check if graph contains instance nodes
            bool  contains_instance_nodes ( ) const;
            
            
            // quick merge graphs in container (graph container is empty after merge!)
            void quick_merge ( patterngraph_container_t & graphs );
            
            // quick split into connected components. (this graph is empty after split!)
            // On return, connected graphs are loaded into graphs container.
            void split_connected ( patterngraph_container_t & split_graphs );

            // splice g2 onto this graph according to overlap_map
            bool  splice ( PatternGraph & g2, Map & overlap_map ); 


		protected:
            // core copying mechanism
            Map * copy_from_source ( const PatternGraph & source );		    
		    // find canonical ordering of the graph nodes (this info stored internal to nodes)
		    void  find_canonical_order ( bool preserve_prior_order ) const;

		    // members
            node_container_t     nodes;
            mutable std::string  label;
            mutable bool         canonical_flag;  

		private:
		    // members
            static node_iter_t          node_iter;
            static node_const_iter_t    node_const_iter; 
	};




    ////
    ////
    ////
        

    // A class that maps nodes in a source graph into nodes in a target graph.
    class Map
    {
        public:
            // structors
            Map ( );
            Map ( PatternGraph * source, PatternGraph * target ); 
            Map ( const Map & source_map );
            // construct an automap for this graph
            Map ( const PatternGraph & graph );
            Map * clone ( ) const { return new Map(*this); };
            ~Map ( );

            // map a node to its target
            Node * mapf ( Node * node ) const;
            // compose maps (i.e. remap f through this map) or invert map
            Map * mapf ( const Map & f ) const;
            Map * invert ( ) const;
            // get image of a range of nodes
            void  image ( node_const_iter_t  nodes_begin,
                          node_const_iter_t  nodes_end,
                          node_container_t & image ) const;
            
            // find node in domain and get iterator pointing to nodemap
            nodemap_iter_t        find ( Node * node );
            nodemap_const_iter_t  find ( Node * node ) const;
            
            // add or erase a node mapping
            bool     insert ( Node * node1, Node * node2 );
            size_t   erase  ( Node * node );  

            // get/set source and target graphs
            PatternGraph  * get_source_graph ( ) const;
            PatternGraph  * get_target_graph ( ) const;
            void     set_source_graph ( PatternGraph * source );
            void     set_target_graph ( PatternGraph * target );

            // iterators and size
            nodemap_iter_t        begin ( ) {  return nodemap.begin();  };        
            nodemap_iter_t        end   ( ) {  return nodemap.end();    };
            nodemap_const_iter_t  begin ( ) const {  return nodemap.begin();  };    
            nodemap_const_iter_t  end   ( ) const {  return nodemap.end();    };            
            size_t                size  ( ) const {  return nodemap.size();   };  
            bool                  empty ( ) const {  return nodemap.empty();  }; 

            // quick merge
            void  quick_merge ( map_container_t & maps );

            // copy the target graph of this map, then get a copy of this map
            //  that is targeted to the copy graph.
            std::pair < Map*, PatternGraph* >  copy_map_and_target ( ) const;
            
            // print a map
            void  print ( ) const;   
        
        protected:
            // members
            nodemap_t              nodemap;
            mutable PatternGraph * source_graph;          
            mutable PatternGraph * target_graph;

        private:
            // static iterators
            static nodemap_iter_t        nodemap_iter;
            static nodemap_const_iter_t  nodemap_const_iter;            
    };


    ////
    ////
    //// 
    

    // a polymorphic family of operations on graphs
    class Transformation
    {
        public:
            Transformation  ( ) {};
            Transformation  ( const Transformation & orig ) {};         
            virtual ~Transformation ( ) {};

			virtual Transformation * clone ( ) const { return new Transformation(*this); };
			virtual Transformation * clone_and_remap ( Map & map ) const { return new Transformation(*this); };

            virtual void transform ( Map & map ) {};
            virtual void remap ( Map & map ) {}; 
            virtual void print ( ) { std::cout << "> Transformation(Transformation)" << std::endl; };
            virtual void get_center ( node_container_t & center_nodes ) {  };
            //virtual void get_elementary_rule ( );            
    };  
    ///
    ///
    class AddNode : public Transformation
    {
        public:
            AddNode  ( Node & new_node );
            AddNode  ( const AddNode & source_op );
            virtual ~AddNode ( );
            
            virtual AddNode * clone ( ) const { return new AddNode ( *this ); };
			virtual AddNode * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( );
            virtual void get_center ( node_container_t & center_nodes );
            //virtual void get_elementary_rule ( );                                     
        
            Node * get_pointer ( ) { return node; };
            
        private:
            Node *  node;
    };
    ///
    ///
    class DeleteNode : public Transformation
    {
        public:
            DeleteNode  ( Node * node );
            DeleteNode  ( const DeleteNode & source_op );
            virtual ~DeleteNode ( ) {};
            
            virtual DeleteNode * clone ( ) const  { return new DeleteNode ( *this ); };
			virtual DeleteNode * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( ); 
            virtual void get_center ( node_container_t & center_nodes );
            //virtual void get_elementary_rule ( );                                    
            
        private:
            Node *  node; 
    };
    ///
    ///
    class AddEdge : public Transformation
    {
        public:
            AddEdge  ( Node * _node1, Node * _node2 );
            AddEdge  ( const AddEdge & source_op );
            virtual ~AddEdge ( ) {};
            
            virtual AddEdge * clone ( ) const  { return new AddEdge ( *this ); };
			virtual AddEdge * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( );
            virtual void get_center ( node_container_t & center_nodes );
            //virtual void get_elementary_rule ( );                                   
            
        private:
            Node *  node1;
            Node *  node2; 
    };
    ///
    ///
    class AddBond : public Transformation
    {
        public:
            AddBond  ( Node * _node1, Node * _bond1, Node * _node2, Node * _bond2 );
            AddBond  ( const AddBond & source_op );
            virtual ~AddBond ( ) {};
            
            virtual AddBond * clone ( ) const  { return new AddBond ( *this ); };
			virtual AddBond * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( );
            virtual void get_center ( node_container_t & center_nodes );
            
        private:
            Node *  node1;
            Node *  bond1;
            Node *  node2;
            Node *  bond2;             
    };    
    ///
    ///
    class DeleteEdge : public Transformation
    {
        public:
            DeleteEdge  ( Node * _node1, Node * _node2 );
            DeleteEdge  ( const DeleteEdge & source_op );
            virtual ~DeleteEdge ( ) {};
            
            virtual DeleteEdge * clone ( ) const { return new DeleteEdge ( *this ); };
			virtual DeleteEdge * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( );
            virtual void get_center ( node_container_t & center_nodes );                                 
            
        private:
            Node *  node1;
            Node *  node2;     
    }; 
    ///
    ///
    class DeleteBond : public Transformation
    {
        public:
            DeleteBond  ( Node * _node1, Node * _bond, Node * _node2 );
            DeleteBond  ( const DeleteBond & source_op );
            virtual ~DeleteBond ( ) {};
            
            virtual DeleteBond * clone ( ) const { return new DeleteBond ( *this ); };
			virtual DeleteBond * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( );
            virtual void get_center ( node_container_t & center_nodes );                                 
            
        private:
            Node *  node1;
            Node *  bond; 
            Node *  node2;    
    }; 
    ///
    ///      
    class ChangeState : public Transformation
    {
        public:
            ChangeState  ( Node * _node, const State & _state );         
            ChangeState  ( const ChangeState & source_op );
            virtual ~ChangeState ( ) {};
            
            virtual ChangeState * clone ( ) const { return new ChangeState ( *this ); };
			virtual ChangeState * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( );
            virtual void get_center ( node_container_t & center_nodes );                               
            
        private:
            Node *   node;
            State *  state; 
    };
    ///
    ///
    class DeleteSubtree : public Transformation
    {
        public:
            DeleteSubtree  ( Node * _node );
            DeleteSubtree  ( const DeleteSubtree & source_op );
            virtual ~DeleteSubtree ( ) {};
            
            virtual DeleteSubtree * clone ( ) const { return new DeleteSubtree ( *this ); };
			virtual DeleteSubtree * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( );
            virtual void get_center ( node_container_t & center_nodes );                                   
            
        private:
            Node *  node; 
    };
    ///
    ///  
    class DeleteConnected : public Transformation
    {
        public:
            DeleteConnected  ( Node * _node );
            DeleteConnected  ( const DeleteConnected & source_op );
            virtual ~DeleteConnected ( ) {};
            
            virtual DeleteConnected * clone ( ) const { return new DeleteConnected ( *this ); };
			virtual DeleteConnected * clone_and_remap ( Map & map ) const; 
            
            virtual void transform ( Map & map );            
            virtual void remap ( Map & map );
            virtual void print ( );
            virtual void get_center ( node_container_t & center_nodes );                        
            
        private:
            Node *  node;  
    };

    
    ////
    ////
    ////
    
    // a class for defining functions on Nodes
    class NodeFunction
    {
        public:
            NodeFunction ( ) { };
            NodeFunction ( const NodeFunction & x ) { };
            virtual NodeFunction * clone ( ) const { return new NodeFunction(*this); };
            virtual ~NodeFunction ( ) { };
            
            virtual int  map ( const Node & node ) const { return 0; };
            virtual int  map ( const Node & node1, const Node & node2 ) const { return 0; };
    }; 

    class ConstantNodeFunction : public NodeFunction
    {
        public:
            ConstantNodeFunction ( int x ) : value( x ) { };
            ConstantNodeFunction ( const ConstantNodeFunction & x ) : value( x.value ) { };
            virtual ConstantNodeFunction * clone ( ) const { return new ConstantNodeFunction(*this); };                      
            virtual ~ConstantNodeFunction ( ) { };
            virtual int  map ( const Node & node ) const { return value; };
            virtual int  map ( const Node & node1, const Node & node2 ) const { return value; };            
        private:
            const int  value;
    };

    class BondNodeFunction : public NodeFunction
    {
        public:
            BondNodeFunction ( ) { };
            BondNodeFunction ( const BondNodeFunction & x ) { };
            virtual BondNodeFunction * clone ( ) const { return new BondNodeFunction(*this); };             
            virtual ~BondNodeFunction ( ) { };
            // this method is defined in NodeType.cpp (along with BondType class)
            virtual int  map ( const Node & node ) const;
            virtual int  map ( const Node & node1, const Node & node2 ) const;             
    };


    ////
    ////
    ////


    // define some useful static objects
    
    // define some common strings
    static const std::string NULL_STRING = std::string("");
    static const std::string BOUND_STRING = std::string("+");
    static const std::string UNBOUND_STRING = std::string("-");
    static const std::string WILDCARD_STRING = std::string("?");

    // instantiate ancestor state types
    static StateType  LABEL_STATE_TYPE   = StateType ( "Label" );
    static StateType  COUNTER_STATE_TYPE = StateType ( "Counter" );

    // create state types for Null and Bonds  
    static NullStateType  NULL_STATE_TYPE = NullStateType ( );
    static BondStateType  BOND_STATE_TYPE = BondStateType ( );

    // declare static states for bound and unbound states
    static BondState  UNBOUND_STATE = BondState ( BOND_STATE_TYPE, UNBOUND_STRING );
    static BondState  BOUND_STATE   = BondState ( BOND_STATE_TYPE, BOUND_STRING );

    // instantiate ancestor node types for each category
    static NodeType  ENTITY_NODE_TYPE = NodeType ( "Entity", NULL_STATE_TYPE );
    static NodeType  LINK_NODE_TYPE   = NodeType ( "Link", NULL_STATE_TYPE );
    
    // create static BondTyping object
    static BondNodeFunction  BOND_TYPING_FCN;    
    // instantiate static bond node type
    static BondType  BOND_NODE_TYPE = BondType ( BOND_TYPING_FCN );

    ////
    ////
    ////
}



#endif /* BNGCORE_HPP_ */
