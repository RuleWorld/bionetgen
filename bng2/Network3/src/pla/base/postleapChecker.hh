/*
 * postleapChecker.hh
 *
 *  Created on: Feb 15, 2011
 *      Author: Leonard Harris
 */

#ifndef POSTLEAPCHECKER_HH_
#define POSTLEAPCHECKER_HH_

#include "../../std_include.hh"
#include "../../model/reaction.hh"

namespace network3{

	class PostleapChecker{
	public:
		PostleapChecker();
		PostleapChecker(const PostleapChecker& pl);
		virtual ~PostleapChecker();
		virtual bool check();
		virtual void correct(vector<double>& k, double& tau);
		virtual void update();
		virtual PostleapChecker* clone() const{ return new PostleapChecker(*this); }
	};
}

#endif /* POSTLEAPCHECKER_HH_ */
