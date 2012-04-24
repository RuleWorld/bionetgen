/*
 * misc.hh
 *
 *  Created on: Apr 20, 2012
 *      Author: Leonard Harris
 */

#ifndef MISC_HH_
#define MISC_HH_

#include <string>

using namespace std;

namespace Util {

	// Check whether a variable is NAN
	bool isNAN(double x);

	// Remove leading and trailing whitespace from a string
	void remove_whitespace(string& s);
}

#endif /* MISC_HH_ */
