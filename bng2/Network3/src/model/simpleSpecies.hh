/*
 * simpleSpecies.hh
 *
 *  Created on: Oct 21, 2009
 *      Author: Leonard Harris
 */

#ifndef SIMPLESPECIES_HH_
#define SIMPLESPECIES_HH_

#include "../std_include.hh"

namespace network3{

	class SimpleSpecies {
	public:
		string name;
		double population;
		SimpleSpecies(){};
		SimpleSpecies(string name, double population);
		~SimpleSpecies();
		void setName(string name);
		void setPopulation(double newPopulation);
		void changePopulation(double deltaPopulation);
	};
}
#endif /* SIMPLESPECIES_HH_ */
