/*
 * preleap_TC.hh
 *
 *  Created on: Feb 7, 2011
 *      Author: Leonard Harris
 */

#ifndef PRELEAP_TC_HH_
#define PRELEAP_TC_HH_

#include "../../std_include.hh"
#include "../base/tauCalculator.hh"

namespace network3{

	class Preleap_TC : public TauCalculator{
	public:
//		Preleap_TC(){}
		Preleap_TC(double eps);
		Preleap_TC(const Preleap_TC& ptc);
		~Preleap_TC();
		virtual Preleap_TC* clone() const = 0; //{ return new Preleap_TC(*this); }
	protected:
		double eps;
	};
}

#endif /* PRELEAP_TC_HH_ */
