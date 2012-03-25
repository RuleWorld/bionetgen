/*
 * butcherTableau.hh
 *
 *  Created on: Mar 23, 2011
 *      Author: Leonard Harris
 */

#ifndef BUTCHERTABLEAU_HH_
#define BUTCHERTABLEAU_HH_

#include "../../../std_include.hh"
#include "../../../util/util.hh"

namespace network3{

	class ButcherTableau{
	public:
		vector<vector<double> > alpha;
		vector<double> beta;
//		ButcherTableau();
		ButcherTableau(vector<vector<double> > alpha, vector<double> beta);
		unsigned int size(){ return this->uint_size; }
		bool isExplicit(){ return this->is_explicit; }
		string toString();
	protected:
		unsigned int uint_size;
		vector<double> gamma;
		bool is_explicit;
	};
}

#endif /* BUTCHERTABLEAU_HH_ */
