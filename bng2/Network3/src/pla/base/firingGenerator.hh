/*
 * firingGenerator.hh
 *
 *  Created on: Feb 12, 2011
 *      Author: Leonard Harris
 */

#ifndef FIRINGGENERATOR_HH_
#define FIRINGGENERATOR_HH_

#include "../../std_include.hh"
#include "../../model/reaction.hh"

namespace network3{

	class FiringGenerator{
	public:
		FiringGenerator();
		FiringGenerator(const FiringGenerator& fg);
		virtual ~FiringGenerator();
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau) = 0;
//		virtual FiringGenerator* clone() const{ return new FiringGenerator(*this); }
	};
}

#endif /* FIRINGGENERATOR_HH_ */
