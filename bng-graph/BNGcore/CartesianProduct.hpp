#ifndef BNGCORE_CARTESIANPRODUCT_HPP_
#define BNGCORE_CARTESIANPRODUCT_HPP_

namespace BNGcore
{

//////////////////////
// CartesianProduct //
//////////////////////
template < class T >
class CartesianProduct
{
    public:
        // constructor
        explicit CartesianProduct ( std::vector < std::list < T > > & _lists );
                
        // destructor
        ~CartesianProduct ( ) { };

        // get next element of the cartesian product
        bool  get_next ( std::vector < T > & cartesian_element );
        // add elements to a list
        void  update_list ( size_t list_idx ); 
        // initialize (or reset) to first element
        void  initialize ( );
        // check if there are more elements
        bool  more ( ) const { return more_elements; };
        // how many lists?
        size_t  num_lists ( ) const { return lists.size(); };
        
    protected:
        // advance iterators to the next element
        void  advance ( );

        // a vector of lists
        std::vector < std::list < T > >  lists;
        // more elements in the cartesian product?
        bool  more_elements;
        // vector of iterators pointing to elements of each list
        std::vector < std::list < T >::iterator >  begin_iters;
        std::vector < std::list < T >::iterator >  curr_iters;
        std::vector < std::list < T >::iterator >  last_iters;

    private:
        // iterator over lists
        std::vector < std::list < T > >::reverse_iterator  list_reviter;
        // iterator over elements
        std::vector < T >::reverse_iterator                elem_reviter;
        // reverse iterator over vectors of iterators...
        std::vector < std::list < T >::iterator >::reverse_iterator  begin_iters_reviter
        std::vector < std::list < T >::iterator >::reverse_iterator  curr_iters_reviter
        std::vector < std::list < T >::iterator >::reverse_iterator  last_iters_reviter
};


// Constructor
CartesianProduct::CartesianProduct ( std::vector < std::list < T > > & _lists )
  : lists( _lists ),
    begin_iters( num_lists ),
    curr_iters( num_lists ),
    end_iters( num_lists ),
    more_elements( true )
{
    // initialize iterators
    initialize();   
};


// initialize (or reset) iterator to the first element
void
CartesianProduct::initialize ( )
{
    begin_iters_reviter = begin_iters.rbegin();
    curr_iters_reviter  = curr_iters.rbegin();
    end_iters_reviter   = end_iters.rbegin();
    for ( list_reviter = lists.rbegin(); list_reviter != lists.rend(); ++list_reviter )
    {
        *begin_iters_reviter = (*list_reviter).begin();
        *curr_iters_reviter  = (*list_reviter).begin();
        *end_iters_reviter   = (*list_reviter).end();

        // check for empty list
        if ( *begin_iters_reviter == *end_iters_reviter )
        {   more_elements = false;   }

        ++begin_iters_reviter;
        ++curr_iters_reviter;
        ++end_iters_reviter;
    }
};


// get next map set method
bool
CartesianProduct::get_next ( std::vector < T > & cartesian_element )
{
    if ( more_elements )
    {
        // retreive next element of the cartesian product
        elem_reviter       = cartesian_element.rbegin();
        curr_iters_reviter = curr_iters.rbegin();
        while ( elem_reviter != cartesian_element.rend() )
        {
            *elem_iter = **curr_iters_reviter;
            ++elem_reviter;
            ++curr_iters_reviter;
        }
        // advance the iterator
        advance();
        return true;
    }
    else
    {   return false;   }
};
              

// try to advance cartesian product iterator
void
CartesianProduct::advance ( )
{
    if (more_elements)
    {
        begin_iters_reviter = begin_iters.rbegin(); 
        curr_iters_reviter  = curr_iters.rbegin(); 
        end_iters_reviter   = end_iters.rbegin(); 
        while ( curr_iters_reviter != curr_iters.rend() )
        {
            // try to advance the current list
            ++(*curr_iters_reviter);
            if ( *curr_iters_reviter != *end_iters_reviter ) {  break;  }
            // otherwise reset to beginning of list
            *curr_iters_reviter = *begin_iters_reviter;            
            // and try to advance the previous list
            ++begin_iters_reviter;            
            ++curr_iters_reviter;
            ++end_iters_reviter;
        }
        
        // if there are no more lists to try and update, then we're out of elements
        if ( curr_iters_reviter == curr_iters.rend() )
        {   more_elements = false;   }
    }
};


// update cartesian product after adding to a list
bool
CartesianProduct::update_list ( size_t list_idx )
{
    // bad idea to do this if there are more elements
    if (more_elements) { return false; }
    // can't do if the list_idx is invalid
    if (list_idx >= num_lists()) { return false; }

    // find iterator of the updated list
    std::vector < std::list < T > >::reverse_iterator update_list_reviter = list.rbegin();
    update_list_reviter += num_lists() - (list_idx + 1);
    
    // update all iterators
    more_elements = true;
    lists_reviter = lists.rbegin();
    begin_iters_reviter = begin_iters.rbegin(); 
    curr_iters_reviter  = curr_iters.rbegin(); 
    end_iters_reviter   = end_iters.rbegin(); 
    while ( curr_iters_reviter != curr_iters.rend() )
    {
        if (lists_reviter == update_list_reviter)
        {   // begin where we ended
            *begin_iters_reviter = *end_iters_reviter;
            *curr_iters_reviter  = *end_iters_reviter;
            // end at the tail of the new elements
            *end_iters_reviter   = (*lists_reviter).end();
            // check for new elements
            if ( *begin_iters_reviter == *end_iters_reviter )
            {   more_elements = false;   }
        }
        else
        {   // go back to the beginning
            *begin_iters_reviter = (*lists_reviter).begin();
            *curr_iters_reviter  = (*lists_reviter).begin();
            // if this list is emtpy, there can't be new elements
            if ( (*lists_reviter).empty() )
            {   more_elements = false;   }
        }

        ++begin_iters_reviter;            
        ++curr_iters_reviter;
        ++end_iters_reviter;
    }
};

} /* end of BNGcore namespace */

#endif /* BNGCORE_CARTESIANPRODUCT_HPP_ */

