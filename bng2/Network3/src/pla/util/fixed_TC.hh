/*
 * fixed_TC.hh
 *
 *  Created on: Dec 10, 2013
 *      Author: Len
 */

#ifndef FIXED_TC_HH_
#define FIXED_TC_HH_

#include "../../std_include.hh"
#include "preleap_TC.hh"

namespace network3{

	class Fixed_TC : public Preleap_TC{
	public:
//		Fixed_TC(){}
		Fixed_TC(double tau);
		Fixed_TC(const Fixed_TC& tc);
		~Fixed_TC();
		virtual void getNewTau(double& tau){ tau = this->eps; }
		virtual Preleap_TC* clone() const{ return new Fixed_TC(*this); }
	};
}

#endif /* FIXED_TC_HH_ */
