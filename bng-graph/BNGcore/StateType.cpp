/*
 * StateType.cpp
 *
 *  Created on: July 3, 2010
 *      Author: justin
 */

#include "BNGcore.hpp"
using namespace BNGcore;



///////////////
// STATETYPE //
///////////////

//StateType nulltype = StateType ("Null");


// get new State object
State *
StateType::get_state_object ( ) const
{
    return new State ( *this );
}


// partial ordering operator (is this node a subtype of node2?)
bool
StateType::operator< ( const StateType & type2 ) const
{
    if (  *this == type2  )
        return true;
    else
    {
        if ( parent_type != 0 )
            return (  *parent_type < type2  );
        else
            return false;
    }
}


///////////////////
// NULLSTATETYPE //
///////////////////

// get new State object
State *
NullStateType::get_state_object ( ) const
{
    return new NullState ( *this );
}


////////////////////
// LABELSTATETYPE //
////////////////////

// Constructor
LabelStateType::LabelStateType ( const std::string & type_name, const std::string & _default_state )
   : StateType( type_name ),
     default_state( _default_state )
{
    states.insert( std::string("?") );
    states.insert( _default_state );
}


// Constructor with parent argument
LabelStateType::LabelStateType ( const std::string & type_name,
                                 const StateType & parent_type, const std::string & _default_state )
   : StateType( type_name, parent_type ),
     default_state( _default_state )
{
    states.insert( std::string("?") );
    states.insert( _default_state );
}


// add an allowed label_state to this type  
bool
LabelStateType::add_state ( const std::string & state )
{
    return states.insert( state ).second;
}


// get new LabelState object
State *
LabelStateType::get_state_object ( ) const
{
    return new LabelState ( *this, default_state );
    
}

// check a label_state is valid
bool
LabelStateType::check_state ( const std::string & state ) const
{
    return ( states.find( state ) == states.end() );
}    


///////////////////
// BONDSTATETYPE //
///////////////////

// constructor   
BondStateType::BondStateType ( )
    : LabelStateType("!", std::string("-") )
{
    add_state( std::string("+") );
} 


//// constructor with parent type
//BondStateType::BondStateType ( const std::string & type_name, const StateType & parent_type )
//    : LabelStateType ( type_name, parent_type, std::string("-") )
//{
//    add_state( std::string("+") ); 
//}

// get new BondState object
State *
BondStateType::get_state_object ( ) const
{
    return new BondState ( *this, default_state );
}

// get new BondState object w/ bound or unbound state
State *
BondStateType::get_state_object ( bool bound ) const
{
    if ( bound )
        return new BondState ( *this, std::string("+") );
    else
        return new BondState ( *this, std::string("-") );
}



//////////////////////
// COUNTERSTATETYPE //
//////////////////////

// get new CounterState object
State *
CounterStateType::get_state_object ( ) const
{
    return new CounterState ( *this, 0 );
}

