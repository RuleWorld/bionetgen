/*
 * rateMuParser.hh
 *
 *  Created on: Jul 22, 2011
 *      Author: Leonard Harris
 */

#ifndef RATEMUPARSER_HH_
#define RATEMUPARSER_HH_

#include "../rateExpression.hh"
#include "muParser.h"

namespace network3{

	class RateMuParser : public RateExpression{
	public:
		mu::Parser* p;
		RateMuParser(mu::Parser* p, vector<SimpleSpecies*> r, vector<int> rS);
		~RateMuParser();
		double getRate(vector<double> X);
	protected:
		vector<int> rStoich;
	};
}
#endif /* RATEMUPARSER_HH_ */
