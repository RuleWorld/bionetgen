#ifndef BNGCORE_ULLMANN_HPP_
#define BNGCORE_ULLMANN_HPP_

#include "BNGcore.hpp"

namespace BNGcore
{

    /////////////////
    // UllmannBase //
    /////////////////
    
    class UllmannBase
    {   
        public:
            // structors
            UllmannBase  ( const PatternGraph & _Ga, const PatternGraph & _Gb  );
            virtual ~UllmannBase ( );
          
            virtual size_t  find_mappings ( List <Map> & maps ) { return 0; };

        protected:       
            void  initialize_M_vec ( );
            void  copy_M ( ullmann_M_t & orig, ullmann_M_t & copy );
            void  copy_M ( ullmann_M_t & orig, ullmann_M_t & copy, row_iter_t row_iter );
            void  print_M ( ullmann_M_t & M );

            // Ga is the subgraph, Gb is the graph
            const PatternGraph &  Ga;
            const PatternGraph &  Gb;            
            // number of nodes in each graph
            const size_t   pa;
            const size_t   pb;
            // permutation matrix M
            ullmann_M_t                  M;
            // stack of intermediate permutation matrices 
            std::vector< ullmann_M_t >   M_vec;
            // tracking structures (F=targets, H=maps are defined in Ullmann 1976)
            node_container_t  targets;
            Map               map;
    };         




    //////////////////
    // UllmannSGIso //
    //////////////////
    
    // Subgraph Isomorphism using Ullmann's 1976 method (basically)
    class UllmannSGIso : public UllmannBase
    {   
        public:
            // structors
            UllmannSGIso  ( const PatternGraph & Ga, const PatternGraph & Gb  ) : UllmannBase ( Ga, Gb ) {};
            virtual ~UllmannSGIso ( ) {};
          
            // find subgraph isomorphisms
            virtual size_t  find_maps ( List <Map> & maps );

        protected:
            size_t   next_node ( size_t d, row_iter_t & row_iter, List <Map> & sg_iso_maps );
            bool     find_next_match ( col_iter_t & col_iter, const col_iter_t & col_end );    
            bool     build_M0  ( );
            bool     refine_M  ( );
            bool     refine_M ( row_iter_t & row_iter, col_iter_t & col_iter );
    };   
   

}


#endif /* BNGCORE_ULLMANN_HPP_ */
