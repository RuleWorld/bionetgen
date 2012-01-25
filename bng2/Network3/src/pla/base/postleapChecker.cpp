/*
 * postleapChecker.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: Leonard Harris
 */

#include "postleapChecker.hh"

using namespace std;

PostleapChecker::PostleapChecker(){
	if (debug)
		cout << "PostleapChecker constructor called." << endl;
}

PostleapChecker::PostleapChecker(const PostleapChecker& pl){
	if (debug)
		cout << "PostleapChecker copy constructor called." << endl;
}

PostleapChecker::~PostleapChecker(){
	if (debug)
		cout << "PostleapChecker destructor called." << endl;
}

bool PostleapChecker::check(){
	cout << "Error in PostleapChecker::check(): Cannot use base method, must overload. Exiting." << endl;
	exit(1);
	return false;
}

void PostleapChecker::correct(vector<double>& k, double& tau){
	cout << "Error in PostleapChecker::correct(): Cannot use base method, must overload. Exiting." << endl;
	exit(1);
}

void PostleapChecker::update(){
	if (debug)
		cout << "Using base method PostleapChecker::update(): Doing nothing." << endl;
}

