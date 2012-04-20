/**
 * @file util.hh
 *
 * A header file that references all the other header files in the Util namespace.
 * This allows you to include only this file and get all the functionality of
 * the Util namespace.  Conversly, you could include the specific util header
 * files that you want to use.
 *
 * @date Oct 14th, 2009   last edited: Oct 14th, 2009
 *
 * @author Michael Sneddon
 */

#ifndef UTIL_HH_
#define UTIL_HH_


#include "constants.hh"
#include "conversion.hh"
#include "rand.hh"
#include "rand2/rand2.hh"
//#include "matrix/matrix.hh"
#include "misc.hh"

//!  General utility function library.
/*!
 *  The set of functions included in the Util namespace inculdes a random number
 *  generator, constants that are often used, function and xml parsing libraries,
 *  conversions between primitive types, and other uncategorized utilites.
 *  @author Michael Sneddon
 */
namespace Util { };


#endif /* UTIL_HH_ */
