/*
 * rateExpression.h
 *
 *  Created on: Oct 5, 2010
 *      Author: Leonard Harris
 */

#ifndef RATEEXPRESSION_HH_
#define RATEEXPRESSION_HH_

#include "simpleSpecies.hh"

namespace network3{

	class RateExpression{
	public:
		string type;
		RateExpression();
		virtual ~RateExpression();
		virtual double getRate(vector<double> X);
		virtual double get_dRate_dX(unsigned int which, vector<double> X);
		string toString(){ return this->type; }
		static double numerical_deriv(unsigned int which, vector<double> X, RateExpression* re);
	};

	class RateElementary : public RateExpression{
	public:
		double c;
		RateElementary(double c, vector<SimpleSpecies*> r, vector<int> rS);
		~RateElementary();
		double getRate(vector<double> X);
		double get_dRate_dX(unsigned int which, vector<double> X);
	protected:
		vector<int> rStoich;
	//		bool numerical_derivatives;
	};

	class RateSaturation : public RateExpression{
	public:
		double kcat;
		vector<double> Km;
		RateSaturation(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS);
		RateSaturation(double kcat, vector<double> Km, vector<SimpleSpecies*> r, vector<int> rS);
		~RateSaturation();
		double getRate(vector<double> X);
		double get_dRate_dX(unsigned int which, vector<double> X);
	protected:
		vector<int> rStoich;
	//		bool numerical_derivatives;
		void construct(vector<SimpleSpecies*> r, vector<int> rS);
	};

	class RateHill : public RateExpression{
	public:
		double Vmax;
		double Kh;
		double h;
		RateHill(double Vmax, double Kh, double h, vector<SimpleSpecies*> r, vector<int> rS);
		~RateHill();
		double getRate(vector<double> X);
		double get_dRate_dX(unsigned int which, vector<double> X);
	protected:
		vector<int> rStoich;
	//		bool numerical_derivatives;
	};

	class RateMM : public RateExpression{
	public:
		double kcat;
		double Km;
		RateMM(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS);
		~RateMM();
		double getRate(vector<double> X);
		double get_dRate_dX(unsigned int which, vector<double> X);
	protected:
	//		bool numerical_derivatives;
	};
}

#endif /* RATEEXPRESSION_HH_ */
