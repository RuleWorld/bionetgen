#ifndef BNGCORE_VECTOR_HPP_
#define BNGCORE_VECTOR_HPP_

#include <vector>

namespace BNGcore
{

template < class T >
class Vector
{
    typedef typename std::vector < T * >                  vector_t;
    typedef typename std::vector < T * >::iterator        iter_t;
    typedef typename std::vector < T * >::const_iterator  const_iter_t;    
                   
    public:
        // constructor
        explicit Vector ( ) { };
      
        // construct and initialize with uniform value
        explicit Vector ( size_t n, const T & value = T() )
        {
            for ( size_t ii = 0;  ii < n;  ++ii )
                push_back ( value );
        }
        
        // construct with elements from iterator range
        template < class InputIterator >
        explicit Vector ( InputIterator first, const InputIterator last )
        {
            while ( first != last )
            {
                push_back ( *first );
                ++first;
            }
        }

        // construct from another container?
        template < class ContainerType >
        explicit Vector ( const ContainerType & x )
        {
            typename ContainerType::const_iterator pos;
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );             
        };

        // copy constructor
        Vector ( const Vector & x )
        {
            const_iterator pos;
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );
        };
               
        // copy assignment constructor
        Vector & operator= ( const Vector & x )
        {
            clear();
            const_iterator pos;
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );
            return *this;
        };
                
        // destructor
        ~Vector ( ) {  clear();  };

        // clone method
        Vector * clone ( ) const { return new Vector ( *this ); };

        // an iterator class for Vector
        class iterator
        {                
            public:
                iterator ( ) {};           
                iterator ( const iter_t & _iter ) : iter(_iter) {};
                iterator ( const iterator & x ) { iter = x.base(); };
                iterator & operator= ( const iterator & x ) { iter = x.base();  return *this; };           
                iterator * clone ( ) const { return new iterator ( *this ); };
    
                // check equality of iterators
                bool operator== ( const iterator & it2 ) const { return (base() == it2.base()); };
                bool operator!= ( const iterator & it2 ) const { return (base() != it2.base()); };
                // increment
                iterator & operator++ ( )   {  ++iter; return *this;  };
                iterator & operator++ (int) { iterator copy = iterator(*this);  ++iter;  return copy; };
                // decrement
                iterator & operator-- ( )   {  --iter; return *this;  };
                iterator & operator-- (int) { iterator copy = iterator(*this);  --iter;  return copy; };
                // dereference
                T &  operator*  ( ) const { return **iter; };
                T *  operator-> ( ) const { return *iter;  };                
                // get reference to the underlying iterator!
                const iter_t &  base ( ) const {  return iter;  };
    
            private:
                iter_t  iter;                
        };

        // an iterator class for Vector
        class const_iterator
        {                
            public:
                const_iterator ( ) {};           
                const_iterator ( const const_iter_t & _iter ) : iter(_iter) {};
                const_iterator ( const const_iterator & x ) { iter = x.base(); };
                const_iterator ( const iterator & x ) { iter = x.base(); };
                const_iterator & operator= ( const const_iterator & x ) { iter = x.base();  return *this; };                    
                const_iterator & operator= ( const iterator & x ) { iter = x.base();  return *this; }; 
                const_iterator * clone ( ) const { return new const_iterator ( *this ); };

                // check equality of iterators
                bool operator== ( const const_iterator & it2 ) const { return (base() == it2.base()); };
                bool operator!= ( const const_iterator & it2 ) const { return (base() != it2.base()); };
                // increment
                const_iterator & operator++ ( )   {  ++iter; return *this;  };
                const_iterator & operator++ (int) { const_iterator copy = const_iterator(*this);  ++iter;  return copy; };
                // decrement
                const_iterator & operator-- ( )   {  --iter; return *this;  };
                const_iterator & operator-- (int) { const_iterator copy = const_iterator(*this);  --iter;  return copy; };
                // dereference
                const T &  operator*  ( ) const { return **iter; };
                const T *  operator-> ( ) const { return *iter;  };
                // get reference to the underlying iterator!
                const const_iter_t &   base ( ) const {  return iter;  };
    
            private:
                const_iter_t  iter;                
        };

        // get iterators
        iterator        begin ( )       { return iterator ( contents.begin() ); }; 
        const_iterator  begin ( ) const { return const_iterator ( contents.begin() ); }; 
        iterator        end   ( )       { return iterator ( contents.end() );   };
        const_iterator  end   ( ) const { return const_iterator ( contents.end() );   };

        // deposit an item to the front or back of the vector (insert by copy!)
        void  push_back  ( T & item )       {  contents.push_back( item.clone() );  };
        void  push_back  ( const T & item ) {  contents.push_back( item.clone() );  };        
        // delete element on the front or back of the vector
        void  pop_back   ( ) {  delete contents.back();  contents.pop_back();   };

        // add item to the front or back of the vector by pointer! (no copy!)
        void  deposit_back  ( T * item ) {  contents.push_back( item );  };
        
        // withdraw element from the front of back of the vector
        T * withdraw_back  ( )
        {
            T * item = contents.back();
            contents.pop_back();
            return item;
        };

        // get a reference to the element on the front or back of the vector
        T &     front ( ) const {  return *(contents.front()); };    
        T &     back  ( ) const {  return *(contents.back());  };

        // get info about vector size
        size_t  size  ( ) const {  return contents.size();  };
        bool    empty ( ) const {  return contents.empty(); };

        // access element by index.
        T &     operator[] ( size_t n ) const {  return *(contents[n]);  };      
        T &     at         ( size_t n ) const {  return *(contents[n]);  }; 
        
        // insert element into the vector using an iterator
        iterator insert ( iterator position, const T & x )
        {  
            return iterator ( contents.insert( position.base(), x.clone() ) );
        };

        // insert n copies of x into the container
        void insert ( iterator position, size_t n, const T & x )
        {
            for ( size_t ii = 0;  ii < n;  ++ii )
                position = insert ( position, x );
        }

        // insert into vector from a range given by iterators
        template <class InputIterator>
        void insert ( iterator position, InputIterator first, InputIterator last )
        {
            while ( first != last )
            {
                position = insert ( position, *first );
                ++first;
            } 
        }
        
        // erase element from the vector using an iterator
        iterator erase ( iterator position ) 
        {
            delete *(position.base());
            return iterator ( contents.erase( position.base() ) );
        };
        
        // erase a range of elements
        iterator erase ( iterator first, iterator last ) 
        {
            iterator position = first;
            while ( position != last )
            {
                delete *(position.base());
                ++position;
            }
            return iterator ( contents.erase( first.base(), last.base() ) );
        };

        // clear the container and destroy contents      
        void  clear ( )
        {
            while ( !contents.empty() )
            {
                delete contents.back();
                contents.pop_back();
            }                
        };

        // clear the container, but don't destroy contents (make sure something else has pointers
        //  to the contents or the contents are lost to a memory leak!)
        void forget ( ) 
        {
            contents.clear();
        };

        // add contents of list2 to the back of this list
        template < class InputContainer >
        const Vector & operator+= ( const InputContainer container2 )
        {
            // copy contents into a list
            Vector copy = Vector( container2 );
            // annex copy into this
            annex ( copy );
            // return reference to this
            return *this;
        }
         
        // swipe contents from another container!
        template < class InputContainer >
        void annex ( InputContainer & container2 )
        {
            typename InputContainer::iterator  iter2;
            
            // deposit contents of container2 into this container
            iter2 = container2.begin();
            while ( iter2 != container2.end() )
            {
                deposit_back ( &(*iter2) );
                ++iter2;
            }
            // tell container2 to forget about its contents! 
            container2.forget ( );            
        } 

        // put default element of type T on the back of the List
        void push_back_default_element ( )
        {
             deposit_back( new T() );
        };

    private:    
        vector_t      contents;

};   

}

#endif /* BNGCORE_VECTOR_HPP_ */
