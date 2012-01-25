/*
 * function.hh
 *
 *  Created on: Aug 7, 2011
 *      Author: Len
 */

#ifndef FUNCTION_HH_
#define FUNCTION_HH_

#include "../std_include.hh"
#include "../../include/muParser.h"

namespace network3{

	class Function{
	public:
		string name;
		mu::Parser* p;
		Function(string name);
		~Function();
		double Eval(){ return this->p->Eval(); }
		string GetExpr(){ return this->p->GetExpr(); }
	};
}

#endif /* FUNCTION_HH_ */
