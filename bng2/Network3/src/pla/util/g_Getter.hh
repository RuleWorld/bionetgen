/*
 * g_Getter.hh
 *
 *  Created on: Apr 26, 2011
 *      Author: Leonard Harris
 */

#ifndef G_GETTER_HH_
#define G_GETTER_HH_

#include "../../std_include.hh"
#include "../../model/reaction.hh"

namespace network3{

	class g_Getter{
	public:
	//	g_Getter();
		g_Getter(vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		g_Getter(const g_Getter& gGet);
		~g_Getter();
		unsigned int size(){ return this->g.size(); }
		double get_g(unsigned int i);
	protected:
		vector<vector<double> > g;
		double get_const_g(unsigned int i);
		double get_const_g(unsigned int u, unsigned int i);
		double get_var_g(unsigned int i);
		double get_var_g(unsigned int u, unsigned int i);
	private:
		unsigned int nRxns;
		vector<SimpleSpecies*>& sp;
		vector<Reaction*>& rxn;
		void addRxn();
		void addSpecies();
	};
}

#endif /* G_GETTER_HH_ */
