/*
 * network3.cpp
 *
 *  Created on: Jul 30, 2011
 *      Author: Leonard Harris
 */

#include "network3.hh"
#include "util/util.hh"

vector<SimpleSpecies*> Network3::SPECIES;
vector<pair<Observable*,double>*> Network3::OBSERVABLE;
vector<pair<Function*,double>*> Network3::FUNCTION;
vector<Reaction*> Network3::REACTION;
PLA* Network3::PLA_SIM = NULL;

void Network3::init_Network3(double* t, bool verbose){
	//
	cout << "*** Initializing Network3 ***" << endl;
	//
	// SPECIES
	if (verbose){
		cout << "------------\nSPECIES\n------------\n";
	}
//	vector<SimpleSpecies*> SPECIES;
	vector<bool> fixed;
    {
		Elt* elt = network.species->list;
		for (int i=0;i < network.species->n_elt;i++){
			SPECIES.push_back(new SimpleSpecies(elt->name,floor(elt->val+0.5)));
			fixed.push_back(elt->fixed);
			if (verbose) cout << i << ". " << SPECIES[i]->name << "\t" << SPECIES[i]->population << endl;
			elt = elt->next;
		}
    }
//  if (verbose) cout << endl;
    //
    // OBSERVABLES
    if (verbose) cout << "------------\nOBSERVABLES\n------------\n";
//    vector<pair<Observable*,double> > OBSERVABLE;
    {
		Group* grp = network.spec_groups;
		vector<SimpleSpecies*> sp;
		vector<double> mult;
		int off = network.species->offset;
		for (int i=0;i < network.n_groups;i++){
			sp.clear(); mult.clear();
			for (int j=0;j < grp->n_elt;j++){
				sp.push_back(SPECIES.at(grp->elt_index[j]-off));
				mult.push_back(grp->elt_factor[j]);
			}
			OBSERVABLE.push_back(new pair<Observable*,double>(new Observable(grp->name,sp,mult),0.0));
			OBSERVABLE[i]->second = OBSERVABLE[i]->first->getValue();
			//
			if (verbose) cout << i << ". " << OBSERVABLE[i]->first->toString() << endl;
			//
			grp = grp->next;
		}
    }
//  if (verbose) cout << endl;
	//
	// FUNCTIONS
    if (verbose) cout << "------------\nFUNCTIONS\n------------\n";
//	vector<pair<Function*,double> > FUNCTION;
	{
    	int off = network.species->offset;
		for (unsigned int i=0;i < network.functions.size();i++){
//			cout << network.functions[i].GetExpr() << "= " << network.functions[i].Eval() << "\t";
			//
			FUNCTION.push_back(new pair<Function*,double>(
					new Function(network.rates->elt[network.var_parameters[i]-off]->name),0.0));
			//
			if (i==0){ // 'time' function
				FUNCTION[0]->first->p->DefineVar("time",t);
			}
			else{
				map<string,double*> var = network.functions[i].GetUsedVar();
/*				map<string,double*>::iterator iter;
				for (iter = var.begin();iter != var.end();iter++){
					cout << "{" << (*iter).first << " = " << *(*iter).second << "}\t";
				}
				cout << endl;
//*/
				// Search observables
				for (unsigned int j=0;j < OBSERVABLE.size();j++){
					if (var.find(OBSERVABLE[j]->first->name) != var.end()){
	//					cout << "\t" << OBSERVABLE[j]->first->name << " = " << OBSERVABLE[j]->second << endl;
						FUNCTION[i]->first->p->DefineVar(OBSERVABLE[j]->first->name,&OBSERVABLE[j]->second);
					}
				}
				// Search parameters
				for (Elt* elt=network.rates->list;elt != NULL;elt=elt->next){
					if (var.find(elt->name) != var.end()){
	//					cout << "\t" << "rates[" << elt->index << "] = " << elt->name << " (";
						bool func = false;
						// Is it a function?
						for (unsigned int j=0;j < network.var_parameters.size() && !func;j++){
							if (elt->index == network.var_parameters[j]){
								// YES
	//							cout << "function[" << j <<"] = " << network.functions[j].GetExpr() << ")" << endl;
								func = true;
								bool found = false;
								// Which one?
								for (unsigned int k=0;k < FUNCTION.size() && !found;k++){
									if (network.functions[j].GetExpr() == FUNCTION[k]->first->GetExpr()){
										found = true;
										FUNCTION[i]->first->p->DefineVar(elt->name,&FUNCTION[k]->second);
									}
								}
								// Error check
								if (!found){
									cout << "Error in Network3::init_Network3(): Couldn't find function "
										 << network.functions[j].GetExpr() << ". Exiting." << endl;
									exit(1);
								}
							}
						}
						// NO, it's a constant
						if (!func){
	//						cout << "constant)" << endl;
							FUNCTION[i]->first->p->DefineConst(elt->name,elt->val);
						}
					}
				}
			}
			// Set expression
			string expr = network.functions[i].GetExpr();
			expr.erase(expr.size()-1); // Trim last character (muParser adds a null to the end)
			FUNCTION[i]->first->p->SetExpr(expr);
			FUNCTION[i]->second = FUNCTION[i]->first->Eval();
			if (verbose) cout << i << ". " << FUNCTION[i]->first->GetExpr() << "= " << FUNCTION[i]->second << endl;
		}
	}
//	if (verbose) cout << endl;
	//
    // REACTIONS
	if (verbose) cout << "------------\nREACTIONS\n------------\n";
//	vector<Reaction*> REACTION;
    {
		int off = network.species->offset;
		Rxn* rxn = network.reactions->list;
		REACTION.resize(network.reactions->n_rxn);
		//
		// Original rates (for comparison)
		double orig_rates[n_rxns_network()];
		rxn_rates_network(orig_rates,1);
		//
		// Loop over reactions
		for (int i=0;i < network.reactions->n_rxn;i++){
			if (verbose) cout << i << ". ";
			double fixed_factor = 1.0; // Populations of any fixed species (incorporate into rate constant)
			//
			// Collect reactants
			double n = 0.0; // for repeated species
			vector<SimpleSpecies*> re; // reactants
			vector<int> reS; // reactant stoichiometries
			for (int j=0;j < rxn->n_reactants;j++){
				if (verbose){
					if (j != 0) cout << " + ";
					cout << SPECIES.at(rxn->r_index[j]-off)->name;
				}
				// Fixed species
				if (fixed.at(rxn->r_index[j]-off)){
					if (j != 0){
						if (rxn->r_index[j] == rxn->r_index[j-1]){ // repeated species
							n += 1.0;
						}
						else{
							n = 0.0;
						}
					}
					fixed_factor *= (SPECIES.at(rxn->r_index[j]-off)->population - n) / (n + 1.0);
				}
				 // Not fixed
				else{
					re.push_back(SPECIES.at(rxn->r_index[j]-off));
					reS.push_back(-1);
				}
			}
			if (verbose) cout << " -> ";
			//
			// Collect products
			vector<SimpleSpecies*> pr; // products
			vector<int> prS; // product stoichiometries
			for (int j=0;j < rxn->n_products;j++){
				if (verbose){
					if (j != 0) cout << " + ";
					cout << SPECIES.at(rxn->p_index[j]-off)->name;
				}
				// Not a fixed species
				if(!fixed.at(rxn->p_index[j]-off)){
					pr.push_back(SPECIES.at(rxn->p_index[j]-off));
					prS.push_back(1);
				}
			}
			if (verbose) cout << endl;
/*			cout << "\t" << rxn->n_reactants << "\t" << rxn->n_products << "\t" << rxn->rateLaw_type;
			cout << "\t" << rxn->n_rateLaw_params << "\t" << rxn->stat_factor << "\t(" << rxn->rateLaw_params[0];
			for (int j=1;j < rxn->n_rateLaw_params;j++){ cout << ", " << rxn->rateLaw_params[j]; }
			cout << ")\t(" << rxn->rateLaw_indices[0];
			for (int j=1;j < rxn->n_rateLaw_params;j++){ cout << ", " << rxn->rateLaw_indices[j]; }
			cout << ")";
*/			//
			// Remove combinatorial factor from stat factor
			double path_factor = rxn->stat_factor;
			n = 1.0;
			for (int j=1;j < rxn->n_reactants;j++){
				if (SPECIES.at(rxn->r_index[j]-off) == SPECIES.at(rxn->r_index[j-1]-off)){
					n += 1.0;
				}
				else{
					n = 1.0;
				}
				path_factor *= n;
			}
			if (verbose) cout << "stat_factor = " << rxn->stat_factor << ", path_factor = " << path_factor << endl;
			//
			// Build reaction
			if (rxn->rateLaw_type == ELEMENTARY){
				if (verbose) cout << "]]] Elementary rxn type [[[" << endl;
				// Error check
				if (rxn->n_rateLaw_params < 1){
					cout << "Error in Network3::init_Network3(): Elementary rxns must have at least 1 parameter. You have "
						 << rxn->n_rateLaw_params << ". Exiting." << endl;
					exit(1);
				}
				//
				double c = fixed_factor*path_factor*rxn->rateLaw_params[0];
				REACTION.at(i) = new ElementaryRxn(c,re,reS,pr,prS);
				if (verbose) cout << REACTION.at(i)->toString() << endl;
			}
			else if (rxn->rateLaw_type == SATURATION){
				if (verbose) cout << "]]] Saturation rxn type [[[" << endl;
				// Error check
				if (rxn->n_rateLaw_params < 1){
					cout << "Error in Network3::init_Network3(): Saturation rxns must have at least 1 parameter. You have "
						 << rxn->n_rateLaw_params << ". Exiting." << endl;
					exit(1);
				}
				//
				double kcat = fixed_factor*path_factor*rxn->rateLaw_params[0];
				vector<double> Km;
				for (int j=1;j < rxn->n_rateLaw_params;j++){
					Km.push_back(rxn->rateLaw_params[j]);
				}
				REACTION.at(i) = new SaturationRxn(kcat,Km,re,reS,pr,prS);
				if (verbose) cout << REACTION.at(i)->toString() << endl;
			}
			else if (rxn->rateLaw_type == MICHAELIS_MENTEN){
				if (verbose) cout << "]]] Michaelis-Menten rxn type [[[" << endl;
				// Error check
				if (rxn->n_rateLaw_params != 2){
					cout << "Error in Network3::init_Network3(): Michaelis-Menten rxns must have exactly 2 parameters. You have "
						 << rxn->n_rateLaw_params << ". Exiting." << endl;
					exit(1);
				}
				//
				double kcat = fixed_factor*path_factor*rxn->rateLaw_params[0];
				double Km = rxn->rateLaw_params[1];
				REACTION.at(i) = new MichaelisMentenRxn(kcat,Km,re,reS,pr,prS);
				if (verbose) cout << REACTION.at(i)->toString() << endl;
			}
			else if (rxn->rateLaw_type == HILL){
				if (verbose) cout << "]]] Hill rxn type [[[" << endl;
				// Error check
				if (rxn->n_rateLaw_params != 3){
					cout << "Error in Network3::init_Network3(): Hill rxns must have exactly 3 parameters. You have "
						 << rxn->n_rateLaw_params << ". Exiting." << endl;
					exit(1);
				}
				//
				double k = fixed_factor*path_factor*rxn->rateLaw_params[0];
				double Kh = rxn->rateLaw_params[1];
				double h = rxn->rateLaw_params[2];
				REACTION.at(i) = new HillRxn(k,Kh,h,re,reS,pr,prS);
				if (verbose) cout << REACTION.at(i)->toString() << endl;
			}
			else if (rxn->rateLaw_type == FUNCTIONAL){
				if (verbose) cout << "]]] Function rxn type [[[" << endl;
				// Find the function
				unsigned int func_index = 0;
				for (unsigned int j=0;j < network.var_parameters.size();j++){
					if (network.var_parameters[j] == rxn->rateLaw_indices[0]){
						func_index = j;
						break;
					}
				}
				// Prepend path_factor to parser expression and reset expression
				string new_expr = Util::toString(fixed_factor) + "*" + Util::toString(path_factor) + "*"
						+ FUNCTION[func_index]->first->GetExpr();
				new_expr.erase(new_expr.size()-1); // Erase Null character appended to expression by muParser
				FUNCTION[func_index]->first->p->SetExpr(new_expr);
				FUNCTION[func_index]->second = FUNCTION[func_index]->first->Eval();
				REACTION.at(i) = new FunctionalRxn(FUNCTION[func_index]->first,re,reS,pr,prS);
				if (verbose) cout << REACTION.at(i)->toString() << endl;
			}
			else{
				cout << "Error in Network3::init_Network3(): Rate law type for reaction " << rxn->index
					 << " not recognized (Type " << rxn->rateLaw_type << "). Exiting." << endl;
				exit(1);
			}
			//
			if (verbose) cout << "rate = " << REACTION.at(i)->getRate()
					          << " (orig: " << orig_rates[i] << ")" << endl << endl;
			rxn = rxn->next;
		}
    }
}

void Network3::init_PLA(string config, bool verbose){

	// 1st argument: Method type (fEuler, midpt, rk4, custom).
	// 2nd argument: Rxn-based (rb) or species-based (sb).
	// 3rd argument: TauCalculator:PostleapChecker type (pre:neg, pre:post, post:post).
	// 4th argument: PLA parameters:
	//				 1 required (eps)
	//				 6 optional (apx1,gg1,p,pp,q,w)
	// 5th argument (optional): Path to butcher tableau file (required if 'custom' chosen in arg 1).

	string method;					// Method (fEuler,midpt,etc.)
	string prepost;					// Preleap/postleap config (preNeg:sb,fixed,etc.)
	map<string,string> params;		// Parameters (eps,approx1,gg1,etc.)

	cout << "Initializing PLA simulation. Configuration = \"" << config << "\"" << endl;

	// Remove '|' from beginning of string, if present
	if (config.at(0) == '|') config.erase(0,1);

	// Add '|' to end of string, if not present
	if (config.at(config.size()-1) != '|') config.append("|");

	// Collect arguments
	vector<string> arg;
	int last = -1, next;
	while ((next = config.find('|',last+1)) != (int)string::npos){
		string s = config.substr(last+1,next-(last+1));
		Util::remove_whitespace(s);
		arg.push_back(s);
		last = next;
//		cout << arg[arg.size()-1] << endl;
	}

	// Error check
	if (arg.size() != 3){
		cout << "Oops, 3 arguments are required, you've specified " << arg.size() << "." << endl;
		cout << "Please try again." << endl;
		exit(1);
	}
	else{
		method = arg[0];
		prepost = arg[1];
	}

	// Process arguments
	if (verbose) cout << "Processing..." << endl;

	// Recognized arguments:
	string _METHODS_;
	_METHODS_ = "fEuler midpt rk4 custom";
	transform(_METHODS_.begin(), _METHODS_.end(), _METHODS_.begin(), ::tolower); // convert to lower case
	string _PREPOST_;
	_PREPOST_ =  "pre-neg:rb pre-neg:sb pre-eps:rb pre-eps:sb post-eps:rb post-eps:sb ";
	_PREPOST_ += "Anderson:rb Anderson:sb fixed fixed:no-ES default";
	transform(_PREPOST_.begin(), _PREPOST_.end(), _PREPOST_.begin(), ::tolower); // convert to lower case
	string _PARAMS_;
	_PARAMS_ = "eps apx1 gg1 p pp q w tau bt";
	// Defaults
	params["apx1"] = "3.0";
	params["gg1"]  = "100.0";
	params["p"]    = "0.5";
	params["pp"]   = "0.8";
	params["q"]    = "1.5";
	params["w"]    = "0.75";

	// Argument 0: Method type
	if (verbose) cout << "1: ";
	vector<vector<double> > alpha; 	// Butcher tableau
	vector<double> beta;			    // Butcher tableau
	transform(method.begin(), method.end(), method.begin(), ::tolower); // convert to lower case
	if ((" "+_METHODS_+" ").find(" "+method+" ")==string::npos){ // match whole words only
		cout << "Uh oh, I don't recognize your choice of method (" << arg[0] << ")." << endl;
		cout << "Currently supported methods are ==> " << _METHODS_ << endl;
		cout << "Please try again." << endl;
		exit(1);
	}
	else if (method == "feuler"){
		if (verbose) cout << "You've chosen 'fEuler'. Very good." << endl;
		// alpha
		alpha.resize(1);
		alpha[0].push_back(0.0);
		// beta
		beta.push_back(1.0);
	}
	else if (method == "midpt"){
		if (verbose) cout << "You've chosen 'midpt'. This should be interesting." << endl;
		// alpha
		alpha.resize(2);
		alpha[0].push_back(0.0); alpha[0].push_back(0.0);
		alpha[1].push_back(0.5); alpha[1].push_back(0.0);
		// beta
		beta.push_back(0.0); beta.push_back(1.0);
	}
	else if (method == "rk4"){
		if (verbose) cout << "You've chosen 'rk4'. Go get 'em cowboy." << endl;
		// alpha
		alpha.resize(4);
		alpha[0].push_back(0.0); alpha[0].push_back(0.0); alpha[0].push_back(0.0); alpha[0].push_back(0.0);
		alpha[1].push_back(0.5); alpha[1].push_back(0.0); alpha[1].push_back(0.0); alpha[1].push_back(0.0);
		alpha[2].push_back(0.0); alpha[2].push_back(0.5); alpha[2].push_back(0.0); alpha[2].push_back(0.0);
		alpha[3].push_back(0.0); alpha[3].push_back(0.0); alpha[3].push_back(1.0); alpha[3].push_back(0.0);
		// beta
		beta.push_back(1.0/6.0); beta.push_back(1.0/3.0); beta.push_back(1.0/3.0); beta.push_back(1.0/6.0);
	}
	else if (method == "custom"){
		if (verbose) cout << "You've chosen 'custom'. The adventurous type I see." << endl;
	}

	// Argument 1: Preleap/postleap configuration
	if (verbose) cout << "2: ";
	transform(prepost.begin(), prepost.end(), prepost.begin(), ::tolower); // convert to lower case
	if ((" "+_PREPOST_+" ").find(" "+prepost+" ")==string::npos){ // // match whole words only
		cout << "Uh oh, I don't recognize your preleap/postleap configuration (" << arg[1] << ")." << endl;
		cout << "Supported configurations are ==> " << _PREPOST_ << endl;
		cout << "Please try again." << endl;
		exit(1);
	}
	else if (prepost == "fixed"){
		if (verbose){
			cout << "You've chosen a fixed time step method. Back to basics." << endl;
		}
	}
	else if (prepost == "default"){
		if (verbose){
			cout << "You've chosen the default preleap/postleap configuration: 'preNeg:sb'" << endl;
		}
		//
		prepost = "pre-neg:sb";
	}
	else if (prepost.find(":rb")!=string::npos || prepost.find(":sb")!=string::npos){
		if (prepost.find(":rb")!=string::npos){
			if (verbose) cout << "You've chosen a reaction-based ";
		}
		else if (prepost.find(":sb")!=string::npos){
			if (verbose) cout << "You've chosen a species-based ";
		}
		if (prepost.find("pre-neg:")!=string::npos){
			if (verbose){
				cout << "preleap tau calculator with a negative-population postleap checker." << endl;
				cout << "   Very good, a straightforward but effective choice." << endl;
			}
		}
		else if (prepost.find("pre-eps:")!=string::npos){
			if (verbose){
				cout << "preleap tau calculator with an epsilon-based postleap checker." << endl;
				cout << "   Excellent, this should improve the accuracy of your results." << endl;
			}
		}
		else if (prepost.find("post-eps:")!=string::npos || prepost.find("anderson:")!=string::npos){
			if (verbose){
				cout << "Anderson-style postleap checker/tau calculator." << endl;
				cout << "   I like how you think." << endl;
			}
		}
	}

	// Argument 2: parameters
	if (verbose) cout << "3: User-defined parameters:" << endl;
	last = -1;
	arg[2].append(",");
	string s, name, val;
	while ((next = arg[2].find(',',last+1)) != (int)string::npos){
		s = arg[2].substr(last+1,next-(last+1));
		size_t equal = s.find("=");
		if (equal == string::npos){ // Error check
			cout << "Oops, PLA parameters must be input in the form 'param=val'. "
					"You input " << s << ". Please try again." << endl;
		}
		name = s.substr(0,equal);
		Util::remove_whitespace(name);
		transform(name.begin(), name.end(), name.begin(), ::tolower); // convert to lower case
		val = s.substr(equal+1,string::npos);
		// Set parameter values
		if ((" "+_PARAMS_+" ").find(" "+name+" ")==string::npos){ // match whole words only
			cout << "Oops, I don't recognize the parameter '" << name << "' (=" << val << ")." << endl;
			cout << "Currently supported parameters are ==> " << _PARAMS_ << endl;
			cout << "Please try again." << endl;
			exit(1);
		}
		else {
			if (verbose) cout << "   " << name << " = " << val << endl;
			params[name] = val;
		}
		last = next;
	}
	// Error check
	if (prepost == "fixed"){
		if (params.find("tau")==params.end()){
			cout << "Oops, 'fixed' requires that 'tau' be defined. Please try again." << endl;
			exit(1);
		}
	}
	else if (params.find("eps")==params.end()){
		cout << "Oops, '" << prepost << "' requires that 'eps' be defined. Please try again." << endl;
		exit(1);
	}
	if (method == "custom"){
		if (params.find("bt")==params.end()){
			cout << "Oops, 'custom' requires that 'bt' (Butcher tableau input file) be defined. Please try again." << endl;
		}
		else{
			// Read Butcher tableau input file
			if (verbose) cout << "  >You've specified a Butcher tableau input file: " << params["bt"] << endl;
			// Try to open the file
			if (fopen(params["bt"].c_str(),"r")){
				if (verbose) cout << "   Ok, I see it." << endl;
				read_Butcher_tableau(params["bt"],alpha,beta,verbose);
			}
			else{
				cout << "   Sorry, I can't find it. Please try again." << endl;
				exit(1);
			}
		}
	}

	// Build the PLA simulator
	TauCalculator* tc = 0;
	RxnClassifier* rc = 0;
	FiringGenerator* fg = 0;
	PostleapChecker* pl = 0;
	ButcherTableau bt(alpha,beta);
	double apx1 = atof(params["apx1"].c_str());
	double gg1  = atof(params["gg1"].c_str());
	double p    = atof(params["p"].c_str());
	//
	if (verbose){
		cout << "  >Parameters for this run:" << endl;
		if (prepost == "fixed") cout << "   tau  = " << params["tau"] << endl;
		else cout << "   eps  = " << params["eps"] << endl;
		cout << "   apx1 = " << params["apx1"] << endl;
		cout << "   gg1  = " << params["gg1"] << endl;
		cout << "   p    = " << params["p"] << endl;
	}
	// Create preleap tau calculator first
	Preleap_TC* ptc;
	double eps_tau;
	if (prepost.find("fixed")!=string::npos){
		eps_tau = atof(params["tau"].c_str());
		static Fixed_TC fixed_ptc(eps_tau);
		ptc = &fixed_ptc;
	}
	else{
		eps_tau = atof(params["eps"].c_str());
		if (prepost.find(":rb")!=string::npos){
			static fEulerPreleapRB_TC fe_ptc(eps_tau,REACTION);
			ptc = &fe_ptc;
		}
		else{
			static fEulerPreleapSB_TC fe_ptc(eps_tau,SPECIES,REACTION);
			ptc = &fe_ptc;
		}
	}
	// Configuration 1: preTC_RC_FG_negPL
	if (prepost.find("pre-neg:")!=string::npos || prepost.find("fixed")!=string::npos){
		static eRungeKutta_preTC_RC_FG_negPL erk_tc_rc_fg_pl(bt,eps_tau,apx1,gg1,p,ptc,SPECIES,REACTION);
		tc = &erk_tc_rc_fg_pl;	rc = &erk_tc_rc_fg_pl;	fg = &erk_tc_rc_fg_pl;	pl = &erk_tc_rc_fg_pl;
	}
	// Configuration 2: preTC_RC_FG_postPL
	else if (prepost == "pre-eps:rb"){
		static eRungeKutta_preTC_RC_FG_rbPL erk_tc_rc_fg_pl(bt,eps_tau,apx1,gg1,p,ptc,SPECIES,REACTION);
		tc = &erk_tc_rc_fg_pl;	rc = &erk_tc_rc_fg_pl;	fg = &erk_tc_rc_fg_pl;	pl = &erk_tc_rc_fg_pl;
	}
	else if (prepost == "pre-eps:sb"){
		static eRungeKutta_preTC_RC_FG_sbPL erk_tc_rc_fg_pl(bt,eps_tau,apx1,gg1,p,ptc,SPECIES,REACTION);
		tc = &erk_tc_rc_fg_pl;	rc = &erk_tc_rc_fg_pl;	fg = &erk_tc_rc_fg_pl;	pl = &erk_tc_rc_fg_pl;
	}
	// Configuration 3: postTC_RC_FG_postPL
	else if (prepost.find("post-eps:")!=string::npos || prepost.find("anderson:")!=string::npos){
		double pp = atof(params["pp"].c_str());
		double q  = atof(params["q"].c_str());
		double w  = atof(params["w"].c_str());
		if (arg[1] == "rb" || arg[1] == "RB"){
			static eRungeKutta_postTC_RC_FG_rbPL erk_tc_rc_fg_pl(bt,eps_tau,apx1,gg1,p,pp,q,w,ptc,SPECIES,REACTION);
			tc = &erk_tc_rc_fg_pl; rc = &erk_tc_rc_fg_pl; fg = &erk_tc_rc_fg_pl; pl = &erk_tc_rc_fg_pl;
		}
		else{
			static eRungeKutta_postTC_RC_FG_sbPL erk_tc_rc_fg_pl(bt,eps_tau,apx1,gg1,p,pp,q,w,ptc,SPECIES,REACTION);
			tc = &erk_tc_rc_fg_pl; rc = &erk_tc_rc_fg_pl; fg = &erk_tc_rc_fg_pl; pl = &erk_tc_rc_fg_pl;
		}
		if (verbose){
			cout << "   pp   = " << params["pp"] << endl;
			cout << "   q    = " << params["q"] << endl;
			cout << "   w    = " << params["w"] << endl;
		}
	}
	//
	PLA_SIM = new PLA(*tc,*rc,*fg,*pl,SPECIES,REACTION);
	if (verbose) cout << "...Ok done, let's go for it." << endl;
}

int Network3::run_PLA(double& time, double maxTime, double sampleTime,
					  double& step, double maxStep, double stepInterval,
					  mu::Parser& stop_condition, bool print_on_stop,
					  char* prefix,
					  bool print_cdat, bool print_func, bool print_save_net, bool print_end_net,
					  bool additional_pla_output,
					  bool verbose){

	// Output files
	string outpre(prefix);
	bool print_classif = additional_pla_output;
	// ...

	// Species file (must exist)
	FILE* cdat = NULL;
	string cFile = outpre + ".cdat";
	if ((cdat = fopen(cFile.c_str(),"r"))){
		fclose(cdat);
		cdat = fopen(cFile.c_str(),"a");
	}
	else {
		cout << "Error in Network3::run_PLA(): Concentrations file \"" << cFile << "\" doesn't exist. Exiting." << endl;
		exit(1);
	}

	// Observables file (optional)
	FILE* gdat = NULL;
	string gFile = outpre + ".gdat";
	if ((gdat = fopen(gFile.c_str(),"r"))){
		fclose(gdat);
		gdat = fopen(gFile.c_str(),"a");
	}
	else{
//		cout << "Warning: Groups file \"" << gFile << "\" doesn't exist." << endl;
	}

	// PLA-specific output files
	FILE* classif = NULL;
	if (print_classif){
		if ((classif = fopen((outpre+"_classif.pla").c_str(),"r"))){
			fclose(classif);
			classif = fopen((outpre+"_classif.pla").c_str(),"a");
		}
		else{
			cout << "Error in Network3::run_PLA(): 'print_classif' flag set but classifications file \""
				 << (outpre+"_classif.pla") << "\" doesn't exist. Exiting." << endl;
			exit(1);
		}
	}
	// ...

	// Identify observables involved in functions
	vector<unsigned int> funcObs;
	for (unsigned int i=0;i < FUNCTION.size();i++){
		map<string,double*> var = FUNCTION[i]->first->p->GetUsedVar();
		for (unsigned int j=0;j < OBSERVABLE.size();j++){
			if (var.find(OBSERVABLE[j]->first->name) != var.end()){
				bool already = false;
				for (unsigned int k=0;k < funcObs.size() && !already;k++){
					if (funcObs[k] == j){
						already = true;
					}
				}
				if (!already){ // add to the list
					funcObs.push_back(j);
				}
			}
		}
	}

	// Prepare for simulation
	double nextOutputTime = time + sampleTime;
	double nextOutputStep = stepInterval;
	while (nextOutputStep <= step) nextOutputStep += stepInterval;
	bool lastOut = true;

	// Simulation loop
//	PLA_SIM->rc.forceClassifications(RxnClassifier::EXACT_STOCHASTIC);
	string print_net_message;
	while (time < maxTime && step < maxStep && !stop_condition.Eval())
	{
		// Next step
		step++;
		PLA_SIM->nextStep(nextOutputTime-time);

		if (PLA_SIM->tau < INFINITY && PLA_SIM->tau > -INFINITY){
			time += PLA_SIM->tau;
		}
		else break;

		// Error check
		if (PLA_SIM->tau <= 0.0){
			cout << "Error in Network3::run_PLA(): tau = " << PLA_SIM->tau << ". Shouldn't happen. Exiting." << endl;
			exit(1);
		}

		// Is it time to output?
		lastOut = false;
		if (time >= nextOutputTime || step >= nextOutputStep) // YES
		{
			// Update all observables
			for (unsigned int i=0;i < OBSERVABLE.size();i++){
				OBSERVABLE[i]->second = OBSERVABLE[i]->first->getValue();
			}
			// Update all functions
			for (unsigned int i=0;i < FUNCTION.size();i++){
				FUNCTION[i]->second = FUNCTION[i]->first->Eval();
			}
			// Output to file
			if (print_cdat) Network3::print_species_concentrations(cdat,time);
			if (gdat) Network3::print_observable_concentrations(gdat,time,print_func);
			if (print_func) Network3::print_function_values(gdat,time);
			if (print_save_net){ // Write current system state to .net file
				// Collect species populations and update network concentrations vector
				double* pops = new double[SPECIES.size()];
				for (unsigned int j=0;j < SPECIES.size();j++){
					pops[j] = SPECIES[j]->population;
				}
				set_conc_network(pops);
				delete pops;
				// Print network w/ current species populations using network::print_network()
				char buf[1000];
				sprintf(buf, "%s_save.net", prefix);
				FILE* out = fopen(buf, "w");
				print_network(out);
				fclose(out);
				print_net_message = " Wrote NET file to " + (string)buf;
//				fprintf(stdout, " Wrote NET file to %s", buf);
			}
			if (print_classif){
				fprintf(classif,"%19.12e",time);
				fprintf(classif," %19.19g",step);
				for (unsigned int v=0;v < PLA_SIM->classif.size();v++){
					fprintf(classif, " %10d", PLA_SIM->classif[v]);
				}
				fprintf(classif,"\n");
				fflush(classif);
			}
			// Output to stdout
			if (verbose){
				printf("\t %f \t %d",time,(int)step);
//				for (unsigned int i=0;i < OBSERVABLE.size();i++){
//					cout << "\t" << OBSERVABLE[i]->second;
//				}
				if (print_save_net){
					fprintf(stdout, "%s", print_net_message.c_str());
				}
				printf("\n");
				fflush(stdout);
			}
			// Get next output time and step
			if (time >= nextOutputTime) nextOutputTime += sampleTime;
			if (step >= nextOutputStep) nextOutputStep += stepInterval;
			lastOut = true;
		}
		else{ // NO
			// Only update observables that are involved in functions
			for (unsigned int i=0;i < funcObs.size();i++){
				OBSERVABLE[funcObs[i]]->second = OBSERVABLE[funcObs[i]]->first->getValue();
			}
			// Update all functions
			for (unsigned int i=0;i < FUNCTION.size();i++){
				FUNCTION[i]->second = FUNCTION[i]->first->Eval();
			}
		}
	}

	// Final output
	if (!lastOut)
	{
		// Update all observables
		for (unsigned int i=0;i < OBSERVABLE.size();i++){
			OBSERVABLE[i]->second = OBSERVABLE[i]->first->getValue();
		}
		// Update all functions
		for (unsigned int i=0;i < FUNCTION.size();i++){
			FUNCTION[i]->second = FUNCTION[i]->first->Eval();
		}
		// Output to file
		if (print_cdat) Network3::print_species_concentrations(cdat,time);
		// Don't print if stopping condition met and !print_on_stop (must print to CDAT)
		if (!(stop_condition.Eval() && !print_on_stop)){
			if (gdat) Network3::print_observable_concentrations(gdat,time,print_func);
			if (print_func) Network3::print_function_values(gdat,time);
			string print_net_message;
			if (print_save_net){ // Write current system state to .net file
				// Collect species populations and update network concentrations vector
				double pops[SPECIES.size()];
				for (unsigned int j=0;j < SPECIES.size();j++){
					pops[j] = SPECIES[j]->population;
				}
				set_conc_network(pops);
				// Print network w/ current species populations using network::print_network()
				char buf[1000];
				sprintf(buf, "%s_save.net", prefix);
				FILE* out = fopen(buf, "w");
				print_network(out);
				fclose(out);
				print_net_message = " Wrote NET file to " + (string)buf;
			}
			if (print_classif){
				fprintf(classif,"%19.12e",time);
				for (unsigned int v=0;v < PLA_SIM->classif.size();v++){
					fprintf(classif, " %10d", PLA_SIM->classif[v]);
				}
				fprintf(classif,"\n");
				fflush(classif);
			}
		}
		// Output to stdout
		if (verbose){
			cout << "\t" << fixed << setprecision(6) << time << "\t" << setprecision(0) << step;
//			for (unsigned int i=0;i < OBSERVABLE.size();i++) cout << "\t" << OBSERVABLE[i]->second;
			if (print_save_net) fprintf(stdout, "%s", print_net_message.c_str());
			cout << endl;
		}
	}

	// Messages if stopping conditions met
	if (stop_condition.Eval()){ // Stop condition satisfied
		cout << "Stopping condition " << stop_condition.GetExpr() << "met in PLA simulation." << endl;
	}
//	else if (step >= startStep + maxSteps){ // maxSteps limit reached
//		cout << "Maximum step limit (" << maxSteps << ") reached in PLA simulation." << endl;
//	}

	// If print_end_net = true, collect species populations and update network concentrations vector
	if (print_end_net){
		double pops[SPECIES.size()];
		for (unsigned int j=0;j < SPECIES.size();j++){
			pops[j] = SPECIES[j]->population;
		}
		set_conc_network(pops);
	}

	// Close files
	fclose(cdat);
	if (gdat) fclose(gdat);
//	if (fdat) fclose(fdat);
	if (classif) fclose(classif);

	// Return value
	if (time >= maxTime) return 0;
	else if (step >= maxStep) return -1;
	else return -2; // stop condition met
}
/*
bool Network3::all_inactive(){
	for (unsigned int j=0;j < REACTION.size();j++){
		if (REACTION[j]->getRate() > network3::TOL){
			return false;
		}
	}
	return true;
}
*/
void Network3::read_Butcher_tableau(string filename, vector<vector<double> >& alpha, vector<double>& beta, bool verbose){

	// Make sure alpha and beta are empty
	alpha.clear();
	beta.clear();

	// Add a row to alpha matrix (Butcher tableau)
	alpha.push_back(vector<double>());

	// Read from file
	ifstream strm(filename.c_str(),ifstream::in);
	string s; double x; char c;
	unsigned int dim = 0, nColumns = 0, nRows = 0;
	bool foundDim = false;
	while (!strm.eof()){
		s.clear();
		strm >> s;
		if (s.size() != 0){ // skip blank lines
			// skip commented (#) lines
			while (s.at(0) == '#'){
				strm.get();
				while (strm.peek() != '\n'){
					strm.get();
				}
				strm.get();
				strm >> s;
			}
			// Continue
			int div = s.find('/');
			if (div != (int)string::npos){
				double num = atof(s.substr(0,div).c_str());
				double denom = atof(s.substr(div+1,s.size()-(div+1)).c_str());
				x = num/denom;
//				cout << "(=" << (num/denom) << ")";
//				cout << "(=" << num << "/" << denom << ")";
//				cout << "(=" << x.substr(0,found) << "/" << x.substr(found+1,x.size()-(found+1)) << ")";
			}
			else{
				x = atof(s.c_str());
			}
//			cout << x << ",";
			if (foundDim && nRows == dim){ // beta value (nRows is # of *completed* rows, not current row)
				beta.push_back(x);
			}
			else{ // alpha value
				alpha[alpha.size()-1].push_back(x);
			}
			//
			nColumns++;
			if (!foundDim) dim++;
			c = strm.peek();
			while (c == ' ' || c == '\t'){
				strm.get();
				c = strm.peek();
			}
			if (c == '\n' || c == EOF){
//				cout << "\n";
				if (foundDim && nColumns != dim){
					cout << "Uh oh, line " << (nRows+1) << " of " << filename << " has " << nColumns << " columns." << endl;
					cout << "Already read " << dim << " entries in line 1. Butcher tableau must be square. "
						 << "Please try again." << endl;
					exit(1);
				}
				nRows++;
				nColumns = 0;
				foundDim = true;
				if (c == '\n' && nRows < dim) alpha.push_back(vector<double>());
			}
//			else cout << "\t";
		}
	}
	// Print to screen
	if (verbose){
		printf("  --------------------------\n");
		printf("   Columns: %d\n",dim);
		printf("   Rows:    %d\n",nRows);
		printf("  --------------------------\n");
		for (unsigned int i=0;i < alpha.size();i++){
			printf("   ");
			for (unsigned int j=0;j < alpha[i].size();j++){
				printf("%-7.4g  ",alpha[i][j]);
			}
			printf("\n");
		}
		printf("  --------------------------\n");
		printf("   ");
		for (unsigned int i=0;i < beta.size();i++){
			printf("%-7.4g  ",beta[i]);
		}
		printf("\n");
		printf("  --------------------------\n");
	}
}

void Network3::print_species_concentrations(FILE* out, double t){
	// Error check
	if (!out){
		cout << "Error in Network3::print_species_concentrations(): 'out' file does not exist. Exiting." << endl;
		exit(1);
	}
	//
	const char *fmt = "%19.12e";
	fprintf(out, fmt, t);
	for (unsigned int i=0;i < SPECIES.size();i++) {
		fprintf(out, " ");
		fprintf(out, fmt, SPECIES[i]->population);
	}
	fprintf(out,"\n");
	fflush(out);
}

void Network3::print_observable_concentrations(FILE* out, double t, bool no_newline){
	// Error check
	if (!out){
		cout << "Error in Network3::print_observable_concentrations(): 'out' file does not exist. Exiting." << endl;
		exit(1);
	}
	//
	const char *fmt = "%19.12e";
	fprintf(out, fmt, t);
	for (unsigned int i=0;i < OBSERVABLE.size();i++){
		fprintf(out, " ");
		fprintf(out, fmt, OBSERVABLE[i]->second);
	}
	if (!no_newline) fprintf(out,"\n"); // Yes, a double negative
	fflush(out);
}

void Network3::print_function_values(FILE* out, double t){
	// Error check
	if (!out){
		cout << "Error in Network3::print_function_values(): 'out' file does not exist. Exiting." << endl;
		exit(1);
	}
	//
	const char *fmt = "%19.12e";
//	fprintf(out, fmt, t);
	for (unsigned int i=1;i < FUNCTION.size();i++){ // Don't print 'time' function (i=0)
		fprintf(out, " ");
		fprintf(out, fmt, FUNCTION[i]->second);
	}
	fprintf(out,"\n");
	fflush(out);
}

void Network3::close_Network3(bool verbose){
    for (unsigned int i=0;i < SPECIES.size();i++){
    	if (verbose) cout << "Deleting Network3::SPECIES[" << i << "]: " << SPECIES[i]->name << endl;
    	delete SPECIES[i];
    }
    for (unsigned int i=0;i < OBSERVABLE.size();i++){
    	if (verbose) cout << "Deleting Network3::OBSERVABLE[" << i << "]: " << OBSERVABLE[i]->first->name << endl;
    	delete OBSERVABLE[i]->first;
    	delete OBSERVABLE[i];
    }
    for (unsigned int i=0;i < FUNCTION.size();i++){
    	if (verbose) cout << "Deleting Network3::FUNCTION[" << i << "]: " << FUNCTION[i]->first->name << " = "
						  << FUNCTION[i]->first->GetExpr() << endl;
    	delete FUNCTION[i]->first;
    	delete FUNCTION[i];
    }
    for (unsigned int i=0;i < REACTION.size();i++){
    	if (verbose) cout << "Deleting Network3::REACTION[" << i << "]: " << REACTION[i]->toString() << endl;
    	delete REACTION[i];
    }
    if (PLA_SIM){
    	if (verbose) cout << "Deleting Network3::PLA_SIM" << endl;
    	delete PLA_SIM;
    }
}
