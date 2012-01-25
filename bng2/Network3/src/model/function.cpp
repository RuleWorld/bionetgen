/*
 * function.cpp
 *
 *  Created on: Aug 7, 2011
 *      Author: Leonard Harris
 */

#include "function.hh"

Function::Function(string name) : name(name), p(new mu::Parser()){
	if (debug)
		cout << "Function constructor called." << endl;
}

Function::~Function(){
	if (debug)
		cout << "Function destructor called." << endl;
	delete this->p;
}
