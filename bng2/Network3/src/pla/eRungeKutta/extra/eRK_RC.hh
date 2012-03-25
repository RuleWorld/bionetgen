/*
 * eRK_RC.hh
 *
 *  Created on: Mar 22, 2012
 *      Author: Leonard Harris
 */

#ifndef ERK_RC_HH_
#define ERK_RC_HH_

#include "../../base/rxnClassifier.hh"

namespace network3{

	class eRK_RC{
	public:
		eRK_RC(double approx1, double gg1);
		eRK_RC(const eRK_RC& rc);
		~eRK_RC();
		void classifyRxns(vector<int>& classif, double tau, bool initial, vector<double>& a_eff);
	protected:
		double approx1;
		double gg1;
	};
}

#endif /* ERK_RC_HH_ */
