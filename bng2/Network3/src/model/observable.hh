/*
 * observable.hh
 *
 *  Created on: Jul 18, 2011
 *      Author: Leonard Harris
 */

#ifndef OBSERVABLE_HH_
#define OBSERVABLE_HH_

#include "simpleSpecies.hh"

namespace network3{

	class Observable{
	public:
		string name;
		Observable(string name, vector<SimpleSpecies*> species, vector<double> mult_factor);
		~Observable();
		double getValue();
		string toString();
	protected:
		vector<pair<SimpleSpecies*,double> > sp; // <species,mult_factor>
	};
}

#endif /* OBSERVABLE_HH_ */
