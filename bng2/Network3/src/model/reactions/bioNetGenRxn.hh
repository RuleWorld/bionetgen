/*
 * bioNetGenRxn.hh
 *
 *  Created on: Feb 20, 2011
 *      Author: Leonard Harris
 */

#ifndef BIONETGENRXN_HH_
#define BIONETGENRXN_HH_

#include "../reaction.hh"
#include "../function.hh"
#include "../rateExpressions/rateMuParser.hh"

namespace network3{

	class BioNetGenRxn : public Reaction{
	public:
	//	BioNetGenRxn();
		BioNetGenRxn(vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS);
		virtual ~BioNetGenRxn();
	protected:
		vector<int> rStoich;
	};

	class ElementaryRxn : public BioNetGenRxn{
	public:
	//	ElementaryRxn();
		ElementaryRxn(double c, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS);
		ElementaryRxn(double c, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS);
		ElementaryRxn(double c, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS);
		ElementaryRxn(double c, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS);
		~ElementaryRxn();
	protected:
		void construct(double c);
	};

	class SaturationRxn : public BioNetGenRxn{
	public:
	//	SaturationRxn();
		//
		SaturationRxn(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS);
		SaturationRxn(double kcat, double Km, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS);
		SaturationRxn(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS);
		SaturationRxn(double kcat, double Km, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS);
		//
		SaturationRxn(double kcat, vector<double> Km, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS);
		SaturationRxn(double kcat, vector<double> Km, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS);
		SaturationRxn(double kcat, vector<double> Km, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS);
		SaturationRxn(double kcat, vector<double> Km, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS);
		//
		~SaturationRxn();
	protected:
		void construct(double kcat, vector<double> Km);
	};

	class HillRxn : public BioNetGenRxn{
	public:
	//	HillRxn();
		HillRxn(double Vmax, double Kh, double h, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS);
		HillRxn(double Vmax, double Kh, double h, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS);
		HillRxn(double Vmax, double Kh, double h, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS);
		HillRxn(double Vmax, double Kh, double h, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS);
		~HillRxn();
	protected:
		void construct(double Vmax, double Kh, double h);
	};

	class MichaelisMentenRxn : public BioNetGenRxn{
	public:
	//	MichaelisMentenRxn();
		MichaelisMentenRxn(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS);
		MichaelisMentenRxn(double kcat, double Km, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS);
		MichaelisMentenRxn(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS);
		MichaelisMentenRxn(double kcat, double Km, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS);
		~MichaelisMentenRxn();
	protected:
		void construct(double kcat, double Km);
	};

	class FunctionalRxn : public BioNetGenRxn{
	public:
	//	FunctionalRxn();
		FunctionalRxn(Function* func, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS);
		FunctionalRxn(Function* func, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS);
		FunctionalRxn(Function* func, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS);
		FunctionalRxn(Function* func, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS);
		~FunctionalRxn();
	protected:
		void construct(Function* func);
	};
}

#endif /* BIONETGENRXN_HH_ */
