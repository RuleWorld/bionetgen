/*
 * tauCalculator.hh
 *
 *  Created on: Feb 7, 2011
 *      Author: Leonard Harris
 */

#ifndef TAUCALCULATOR_HH_
#define TAUCALCULATOR_HH_

#include "../../std_include.hh"
#include "../../model/reaction.hh"

namespace network3{

	class TauCalculator{
	public:
		TauCalculator();
		TauCalculator(const TauCalculator& tc);
		virtual ~TauCalculator();
		virtual void getNewTau(double& tau) = 0;
//		virtual TauCalculator* clone() const{ return new TauCalculator(*this); }
	};
}

#endif /* TAUCALCULATOR_HH_ */
