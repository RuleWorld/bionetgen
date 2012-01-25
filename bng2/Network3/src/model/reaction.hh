/*
 * reaction.hh
 *
 *  Created on: Aug 22, 2010
 *      Author: Leonard Harris
 */

#ifndef REACTION_HH_
#define REACTION_HH_

#include "simpleSpecies.hh"
#include "rateExpression.hh"

namespace network3{

	class Reaction{
	public:
		string type;
		map<SimpleSpecies*,int> stoichSpecies;
		vector<SimpleSpecies*> rateSpecies;
		RateExpression* re;
		Reaction();
		Reaction(map<SimpleSpecies*,int> stoichSpecies, vector<SimpleSpecies*> rateSpecies, RateExpression* re);
		virtual ~Reaction();
		double getRate();
		double get_dRate_dX(int which);
		void fire(double K);
		string toString(){ return string_ID; }
	protected:
		string string_ID;
		static string getStringID(vector<SimpleSpecies*> stoichSpecies, vector<int> stoich, string type_string);
		static string getStringID(vector<pair<SimpleSpecies*,int> > stoichSpecies, string type_string);
		static string getStringID(map<SimpleSpecies*,int> stoichSpecies, string type_string);
		// Old type_string getter
		static string getStringID(vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS, string type_string);
	};
}

#endif /* REACTION_HH_ */
