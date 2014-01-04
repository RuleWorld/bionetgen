/*
 * State.cpp
 *
 *  Created on: July 3, 2010
 *      Author: justin
 */

#include "BNGcore.hpp"
using namespace BNGcore;


// define static members


///////////
// STATE //
///////////

// constructor
State::State ( const StateType & _state_type )
    : state_type( &_state_type )
{

}

// copy constructor
State::State ( const State & x )
    : state_type( x.state_type )
{

}

// get label for canonical sorting
std::string
State::get_label ( ) const 
{   
    std::stringstream s;
    s << "~<" << get_type().get_label() << ">";
    return s.str();
}

// get BNG2 string
std::string
State::get_BNG2_string ( ) const 
{
    return std::string("");
}

// test for equality
bool
State::operator== ( const State & state2 ) const
{
    if ( get_type() == state2.get_type() )  return true;
    else  return false;               
}

// compare for sorting
bool
State::less ( const State & state2 ) const
{
    return  get_type().less( state2.get_type() );
}


///////////////
// NULLSTATE //
///////////////

// Constructor
NullState::NullState ( const NullStateType & state_type )
   : State( state_type )
{
}


// Copy constructor            
NullState::NullState ( const NullState & x )
   : State ( x )
{

}


////////////////
// LABELSTATE //
////////////////

// Constructor
LabelState::LabelState ( const LabelStateType & state_type, const std::string & _state )
   : State( state_type )
{
    state = _state;
}


// Copy constructor            
LabelState::LabelState ( const LabelState & x )
   : State ( x )
{
    state = x.state;
}


// set state
bool
LabelState::set_state ( const std::string & _state )
{
    if ( get_type().check_state( _state ) )
    {
        state = _state;
        return true;
    }
    else
        return false;
} 


// compare states
bool
LabelState::operator== ( const State & state2 ) const
{
    if ( get_type() == state2.get_type() )
    {
        if ( state == std::string("?")  ||  ((const LabelState&)state2).state == std::string("?") )
            return true;
        else
            return ( state == ((const LabelState&)state2).state );
    }
    else
        return false;               
}

// less-than method for sorting
bool
LabelState::less ( const State & state2 ) const
{   
    if ( get_type() == state2.get_type() )
        return ( state < ((const LabelState&)state2).state );
    else
        return  get_type().less( state2.get_type() );
}


// get label (canonical
std::string
LabelState::get_label ( ) const 
{
    std::stringstream s;
    s << "~<" << get_type().get_label() << ">" << state;
    return s.str();
}

             
// get BNG2 string
std::string
LabelState::get_BNG2_string ( ) const 
{
    std::stringstream s;
    s << "~" << state;
    return s.str();
}  

// check if a wildcard
bool
LabelState::is_wildcard ( ) const
{
    return (state == std::string("?"));
} 





///////////////
// BONDSTATE //
///////////////

// constructor
BondState::BondState ( const BondStateType & state_type, const std::string & state )
   : LabelState( state_type, state )
{
    // TODO: validate state!
}


// copy constructor            
BondState::BondState ( const BondState & x )
   : LabelState( x )
{
    state = x.state;
}
            

// get label
std::string
BondState::get_label ( ) const 
{
    std::stringstream s;
    s << "~<" << get_type().get_label() << ">" << state;
    return s.str();
}

             
// get BNG2 string
std::string
BondState::get_BNG2_string ( ) const 
{   
    std::stringstream s;
    s << "!" << state;
    return s.str();
}  





//////////////////
// COUNTERSTATE //
//////////////////

// constructor
CounterState::CounterState ( const CounterStateType & _state_type, int _state )
   : State( _state_type )
{
    state = _state;
}

// copy constructor       
CounterState::CounterState ( const CounterState & x )
   : State( x )
{
    state = x.state;
}
                         

// set state
bool
CounterState::set_state ( int _state )
{   
    if ( get_type().check_state( _state ) )
    {   
        state = _state;
        return true;
    }
    else
        return false;
}     

// add integer to state value
bool
CounterState::add ( int x )
{
    if ( get_type().check_state( state + x ) )
    {
        state += x;
        return true;
    }
    else
        return false;
}

// increment state value
bool
CounterState::increment ( )
{
    if ( get_type().check_state( state + 1 ) )
    {
        ++state;
        return true;
    }
    else
        return false;
}

// decrement state value
bool
CounterState::decrement ( )
{
    if ( get_type().check_state( state - 1 ) )
    {
        --state;
        return true;
    }
    else
        return false;
}


// compare states
bool
CounterState::operator== ( const State & state2 ) const
{   
    if ( get_type() == state2.get_type() )
        return ( state == ((const CounterState&)state2).state );
    else
        return false;               
}


// less-than method for sorting
bool
CounterState::less ( const State & state2 ) const
{   
    if ( get_type() == state2.get_type() )
        return ( state < ((const CounterState&)state2).state );
    else
        return  get_type().less( state2.get_type() );
}


// get label
std::string
CounterState::get_label ( ) const 
{
    std::stringstream s;
    s << "~<" << get_type().get_label() << ">" << state;
    return s.str();
} 


// get BNG2 string
std::string
CounterState::get_BNG2_string ( ) const 
{   
    std::stringstream s;
    s << "~" << state;
    return s.str();
}




