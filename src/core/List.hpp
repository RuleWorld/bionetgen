#ifndef BNGCORE_LIST_HPP_
#define BNGCORE_LIST_HPP_

#include <string>
#include <map>
#include <list>

namespace BNGcore
{

// forward declaration??
template < class T >
class LabeledList;


template < class T >
class List
{
    typedef typename std::list < T * >                  list_t;
    typedef typename std::list < T * >::iterator        iter_t;
    typedef typename std::list < T * >::const_iterator  const_iter_t;
                   
    public:
        // constructor
        explicit List ( ) { };

        // construct with elements from iterator range
        template < class InputIterator >
        explicit List ( InputIterator first, const InputIterator last )
        {
            while ( first != last )
            {
                push_back ( *first );
                ++first;
            }
        }
          
        // construct from another container
        template < class ContainerType >
        explicit List ( const ContainerType & x )
        {
            typename ContainerType::const_iterator pos;
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );                 
        };
               
        // copy constructor
        List ( const List & x )
        {
            const_iterator pos;
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );
        }; 
           
        // copy assignment constructor
        List & operator= ( const List & x )
        {
            clear();
            const_iterator pos;
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );
            return *this;
        };
                
        // destructor
        ~List ( ) {  clear();  };

        // clone method
        List * clone ( ) const  { return new List ( *this ); };

        // an iterator class for List
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

        // an iterator class for List
        class const_iterator
        {                
            public:
                const_iterator ( ) {};           
                const_iterator ( const const_iter_t & _iter ) : iter(_iter) {};
                const_iterator ( const const_iterator & x ) { iter = x.base(); };
                const_iterator ( const iterator & x ) { iter = x.base(); };
                const_iterator & operator= ( const const_iterator & x ) { iter = x.base();  return *this; };                    
                const_iterator & operator= ( const iterator & x )       { iter = x.base();  return *this; };                
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

        // get iterators to the beginning or end of this list
        iterator        begin ( )       { return iterator ( contents.begin() ); }; 
        const_iterator  begin ( ) const { return const_iterator ( contents.begin() ); };         
        iterator        end   ( )       { return iterator ( contents.end() );   };
        const_iterator  end   ( ) const { return const_iterator ( contents.end() );   };

        // deposit an item to the front or back of the list (insert by copy!)
        bool  push_front ( const T & item ) {  contents.push_front( item.clone() );  return true;  };
        bool  push_back  ( const T & item ) {  contents.push_back( item.clone() );   return true;  };

        // add item to the front or back of the list by pointer! (no copy!)
        bool  deposit_front ( T * item ) {  contents.push_front( item );  return true;  };
        bool  deposit_back  ( T * item ) {  contents.push_back( item );   return true;  };

        // delete element on the front or back of the list
        void  pop_front      ( ) {  delete contents.front(); contents.pop_front();  };
        void  pop_back       ( ) {  delete contents.back();  contents.pop_back();   };

        // withdraw element from the front of back of the list
        T *   withdraw_front ( ) {  T * item = contents.front(); contents.pop_front(); return item;  };
        T *   withdraw_back  ( ) {  T * item = contents.back();  contents.pop_back();  return item;  };

        // get a reference to the element on the front or back of the list
        T &         front ( )       {  return *(contents.front()); };
        const T &   front ( ) const {  return *(contents.front()); };
        T &         back  ( )       {  return *(contents.back());  };        
        const T &   back  ( ) const {  return *(contents.back());  };

        // get info about list size
        size_t  size  ( ) const {  return contents.size();  };
        bool    empty ( ) const {  return contents.empty(); };
  
        // insert element into the list using an iteraotr
        iterator insert ( iterator position, const T & x )
        {  
            return iterator ( contents.insert(position.base(), x.clone()) );
        };
        
        // erase element from the list using an iterator
        iterator erase ( iterator position ) 
        {
            delete *(position.base());
            return iterator ( contents.erase( position.base() ) );
        };

        // erase elements from a range given by iterators
        iterator erase ( iterator first, iterator last ) 
        {
            iter_t iter = first.base();
            while ( iter != last.base() )
            {
                delete *iter;
                ++iter;
            }
            return iterator ( contents.erase (first.base(), last.base()) );
        };

        // clear the container and destroy all contents!        
        void  clear ( )
        {
            while ( !contents.empty() )
            {
                delete contents.back();
                contents.pop_back();
            }
        };

        // clear the container, but don't destoy contents (make sure something else has pointers
        //  to the contents or the contents are lost to a memory leak!)
        void forget ( ) 
        {
            contents.clear();
        };

        // add contents of list2 to the back of this list
        template < class InputContainer >
        const List & operator+= ( const InputContainer container2 )
        {
            // copy contents into a list
            List copy = List( container2 );
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

    protected:
        list_t        contents;
        
};   



////
////
////


template < class T >
class LabeledList
{
        typedef typename std::list < T* >                       list_t;
        typedef typename list_t::iterator                       iter_t;
        typedef typename list_t::const_iterator                 const_iter_t;
        typedef typename std::map < std::string, iter_t >       map_t;
        typedef typename map_t::iterator                        map_iter_t;
        typedef typename map_t::const_iterator                  map_const_iter_t;
        typedef typename std::pair < std::string, iter_t >      map_element_t;

    public:
        // constructor
        explicit LabeledList  ( ) { };

        // construct with elements from iterator range
        template < class InputIterator >
        explicit LabeledList ( InputIterator first, const InputIterator last )
        {
            while ( first != last )
            {
                push_back ( *first );
                ++first;
            }       
        }

        // construct from another container
        template < class ContainerType >
        explicit LabeledList ( const ContainerType & x )
        {
            typename ContainerType::const_iterator pos;
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );               
        };

        // copy constructor
        LabeledList ( const LabeledList & x )
        {
            const_iterator pos;
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );            
        }; 
        
        // copy assignment constructor
        LabeledList & operator= ( const LabeledList & x )
        {
            clear();
            const_iterator pos;            
            for ( pos = x.begin();  pos != x.end();  ++pos )
                push_back ( *pos );             
            return *this;
        };    
        
        // destructor
        ~LabeledList ( ) {  clear();  };

        // clone method
        LabeledList * clone ( ) const { return new LabeledList( *this ); };

        // an iterator class for List
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

        // an iterator class for List
        class const_iterator
        {                
            public:
                const_iterator ( ) {};           
                const_iterator ( const const_iter_t & _iter ) : iter(_iter) {};
                const_iterator ( const const_iterator & x ) { iter = x.base(); };
                const_iterator ( const iterator & x ) { iter = x.base(); };
                const_iterator & operator= ( const const_iterator & x ) { iter = x.base();  return *this; };                    
                const_iterator & operator= ( const iterator & x )       { iter = x.base();  return *this; };                
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

        // get iterators to the beginning or end of this list
        iterator        begin ( )       { return iterator ( contents.begin() ); }; 
        const_iterator  begin ( ) const { return const_iterator ( contents.begin() ); };         
        iterator        end   ( )       { return iterator ( contents.end() );   };
        const_iterator  end   ( ) const { return const_iterator ( contents.end() );   };

        // deposit an item to the front of the list (insert by copy!)
        bool  push_front ( const T & x )       
        {   
            if ( label_map.find( x.get_label() ) == label_map.end() )
            {
                T * x_copy = x.clone();
                contents.push_front ( x_copy );
                label_map.insert ( map_element_t( x_copy->get_label(), contents.begin() ) );
                return true;
            }
            else
                return false;
        };
        // deposit an item to the back of the list (insert by copy!)        
        bool  push_back  ( const T & x )
        {   
            if ( label_map.find( x.get_label() ) == label_map.end() )
            {
                T * x_copy = x.clone();
                iter = contents.insert ( contents.end(), x_copy );
                label_map.insert ( map_element_t( x_copy->get_label(), iter ) );
                return true;
            }
            else
                return false;
        };
                
        // add item to the front of the list by pointer! (no copy!)
        bool  deposit_front ( T * x )
        {
            if ( label_map.find( x->get_label() ) == label_map.end() )
            {
                contents.push_front ( x );
                label_map.insert ( map_element_t( x->get_label(), contents.begin() ) );
                return true;
            }
            else
                return false;
        };
        // add item to the back of the list by pointer! (no copy!)        
        bool  deposit_back  ( T * x )
        {
            if ( label_map.find( x->get_label() ) == label_map.end() )
            {
                iter = contents.insert ( contents.end(), x );
                label_map.insert ( map_element_t( x->get_label(), iter ) );
                return true;
            }
            else  return false;
        };
                
        // delete element on the front of the list
        void  pop_front ( )
        {
            T * item = contents.front();
            label_map.erase ( item->get_label() );
            contents.pop_front();
            delete item;
        };
        // delete element on the back of the list       
        void  pop_back  ( )
        {
            T * item = contents.back();
            label_map.erase ( item->get_label() );
            contents.pop_back();
            delete item;
        };                  
        
        // withdraw element from the front of the list
        T * withdraw_front  ( )
        {
            T * item = contents.front();
            label_map.erase ( item->get_label() );
            contents.pop_front();
            return item;
        };
        // withdraw element from the back of the list    
        T * withdraw_back  ( )
        {
            T * item = contents.back();
            label_map.erase ( item->get_label() );
            contents.pop_back();
            return item;
        };  

        // get a reference to the element on the front or back of the list
        T &         front ( )       {  return *(contents.front()); };
        const T &   front ( ) const {  return *(contents.front()); };
        T &         back  ( )       {  return *(contents.back());  };        
        const T &   back  ( ) const {  return *(contents.back());  };

        // get info about list size
        size_t  size  ( ) const    {  return contents.size();  };
        bool    empty ( ) const    {  return contents.empty(); };
  
        // insert element into the list using an iteraotr
        iterator insert ( iterator position, const T & x )
        {
            if ( label_map.find( x.get_label() ) == label_map.end() )
            {
                T * x_copy = x.clone();
                iter = contents.insert( position.base(), x_copy );
                label_map.insert ( map_element_t( x_copy->get_label(), iter ) );
                return iterator( iter );
            }
            else  return iterator( contents.end() );
        };
        
        // erase element from the list using an iterator
        iterator erase ( iterator position ) 
        {
            label_map.erase ( position->get_label() );
            delete *(position.base());
            return iterator( contents.erase(position.base()) );    
        };

        // erase elements from a range given by iterators
        iterator erase ( iterator first, iterator last ) 
        {
            iter = first.base();
            while ( iter != last.base() )
            {
                label_map.erase ( (*iter)->get_label() );
                delete *iter;
                ++iter;
            }
            return iterator ( contents.erase (first.base(), last.base()) );
        };

        // find an element by its label
        iterator  find ( const std::string & label )
        {   
            label_iter = label_map.find( label );
            if ( label_iter != label_map.end() )
                return iterator ( label_iter->second );
            else
                return iterator ( contents.end() );
        };

        // erase element from the list by label
        void  erase ( const std::string & label )
        {
            label_iter = label_map.find ( label );
            if ( label_iter != label_map.end() )
            {
                delete *(label_iter->second);
                contents.erase( label_iter->second );
                label_map.erase ( label_iter );
            }
        };

        // clear the list and destroy all contents
        void  clear ( )
        {
            label_map.clear();
            while ( !contents.empty() )
            {
                delete contents.back();
                contents.pop_back();
            }
        };

        // clear the container, but don't destoy contents (make sure something else has pointers
        //  to the contents or the contents are lost to a memory leak!)
        void forget ( ) 
        {
            label_map.clear();
            contents.clear();
        };

        // add contents of list2 to the back of this list
        template < class InputContainer >
        const LabeledList & operator+= ( const InputContainer container2 )
        {
            // copy contents into a list
            // (use list so we don't have to make a label_map twice!)
            List<T> copy = List<T>( container2 );
            // annex copy into this
            annex ( copy );
            // return reference to this
            return *this;
        }
            
         
        // swipe contents from another container!
        template < class InputContainer >
        void  annex ( InputContainer & container2 )
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

        // remove elements if the label is in a reference list
        void  hipster_filter ( LabeledList & ref_list )
        {
            std::string  label;
            iter = contents.begin();
            while ( iter != contents.end() )
            {   
                label = (**iter).get_label();
                if ( ref_list.find( label ) != ref_list.end() )
                {
                    iter = contents.erase ( iter );
                    label_map.erase( label );
                }
                else  ++iter;
            }   
        };

        // put default element of type T on the back of the List
        void push_back_default_element ( )
        {
             deposit_back( new T() );
        };   

    protected:
        list_t      contents;
        map_t       label_map;
            
    private:
        iter_t      iter;
        map_iter_t  label_iter;
};

}

#endif /* BNGCORE_LIST_HPP_ */
