/*
 * eRK_FG.hh
 *
 *  Created on: Mar 22, 2012
 *      Author: Leonard Harris
 */

#ifndef ERK_FG_HH_
#define ERK_FG_HH_

#include "../../base/rxnClassifier.hh"
#include "../../../model/reaction.hh"
#include "../../../util/util.hh"

namespace network3{

	class eRK_FG{
	public:
		eRK_FG(vector<Reaction*>& rxn);
		eRK_FG(vector<Reaction*>& rxn, bool round);
		eRK_FG(const eRK_FG& fg);
		~eRK_FG();
		void setRounding(bool round){ this->round = round; }
		void fireRxns(vector<double>& k, vector<int>& classif, double tau, vector<double>& a_eff);
		unsigned int nRxns(){ return this->rxn.size(); }
	protected:
		bool round;
		double fireRxn(unsigned int u, double a_tau, int classif);
	private:
		vector<Reaction*>& rxn;
	};
}

#endif /* ERK_FG_HH_ */
