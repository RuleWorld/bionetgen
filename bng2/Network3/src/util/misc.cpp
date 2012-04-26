/*
 * misc.cpp
 *
 *  Created on: Apr 20, 2012
 *      Author: Leonard Harris
 */

#include "misc.hh"

// Check whether a variable is NAN
bool Util::isNAN(double x){
	return x != x;
}

// Remove leading and trailing whitespace from a string
void Util::remove_whitespace(string& s){
	// Remove leading whitespace
	while (s.at(0) == ' ' || s.at(0) == '\t') s.erase(0,1);
	// Remove trailing whitespace
	while (s.at(s.size()-1) == ' ' || s.at(s.size()-1) == '\t') s.erase(s.size()-1,1);
}
