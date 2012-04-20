/**
 * @file conversion.hh
 *
 * A library of standard converstions between primitive types and STL objects,
 * such as converting between std::string and double.
 *
 * @date Oct 14th, 2009   last edited: Oct 14th, 2009
 *
 * @author Michael Sneddon
 */

#ifndef CONVERSION_HH_
#define CONVERSION_HH_

#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>



using namespace std;


namespace Util {


	//!  Parses and converts std::string objects to double values.
	/*!
	 * Just uses the standard library operations for converting strings to
	 * double values.  Throws a run time exception if the parse failed.
	 * @param s the string to act on
	 * @author Michael Sneddon
	*/
	double convertToDouble(const std::string& s);

	//!  Parses and converts std::string objects to int values.
	/*!
	 * Just uses the standard library operations for converting strings to
	 * integers.  Throws a run time exception if the parse failed.
	 * @param s the string to act on
	 * @author Michael Sneddon
	*/
	int convertToInt(const std::string& s);


	//!  Converts double values to their string representations
	/*!
	 * @param x the double number to act on
	 * @author Michael Sneddon
	*/
	string toString(double x);

	//!  Converts integer values to their string representations
	/*!
	 * @param x the integer number to act on
	 * @author Michael Sneddon
	*/
	string toString(int x);

};




#endif /* CONVERSION_HH_ */
