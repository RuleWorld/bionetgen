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
				unsigned int func_index;
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
	//				 Minimum of 4 (eps,apx1,gg1,p).
	//				 Optional 3 additional (pp,q,w).
	// 5th argument (optional): Path to butcher tableau file (required if 'custom' chosen in arg 1).

	cout << "Initializing PLA simulation. Configuration = " << config << endl;

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
	if (arg.size() != 4 && arg.size() != 5){
		cout << "Oops, a minimum of 4 arguments are required, a maximum of 5 are allowed. ";
		cout << "You've specified " << arg.size() << ". Please try again." << endl;
		exit(1);
	}
	cout << "Read " << arg.size() << " arguments." << endl;

	// Process arguments
	if (verbose) cout << "Processing..." << endl;
	vector<vector<double> > alpha; 	// Butcher tableau
	vector<double> beta;			// Butcher tableau
	vector<double> param;			// PLA parameters (eps,approx1,gg1,etc.)

	// Argument 1: Method type
	if (verbose) cout << "1: ";
	if (arg[0] == "fEuler"){
		if (verbose) cout << "You've chosen 'fEuler', very good." << endl;
		// alpha
		alpha.resize(1);
		alpha[0].push_back(0.0);
		// beta
		beta.push_back(1.0);
	}
	else if (arg[0] == "midpt"){
		if (verbose) cout << "You've chosen 'midpt', this should be interesting." << endl;
		// alpha
		alpha.resize(2);
		alpha[0].push_back(0.0); alpha[0].push_back(0.0);
		alpha[1].push_back(0.5); alpha[1].push_back(0.0);
		// beta
		beta.push_back(0.0); beta.push_back(1.0);
	}
	else if (arg[0] == "rk4"){
		if (verbose) cout << "You've chosen 'rk4', go get 'em cowboy." << endl;
		// alpha
		alpha.resize(4);
		alpha[0].push_back(0.0); alpha[0].push_back(0.0); alpha[0].push_back(0.0); alpha[0].push_back(0.0);
		alpha[1].push_back(0.5); alpha[1].push_back(0.0); alpha[1].push_back(0.0); alpha[1].push_back(0.0);
		alpha[2].push_back(0.0); alpha[2].push_back(0.5); alpha[2].push_back(0.0); alpha[2].push_back(0.0);
		alpha[3].push_back(0.0); alpha[3].push_back(0.0); alpha[3].push_back(1.0); alpha[3].push_back(0.0);
		// beta
		beta.push_back(1.0/6.0); beta.push_back(1.0/3.0); beta.push_back(1.0/3.0); beta.push_back(1.0/6.0);
	}
	else if (arg[0] == "custom"){
		// Error check
		if (arg.size() != 5){
			cout << "Oops, you've chosen 'custom' but you haven't specified a Butcher tableau input file." << endl;
			cout << "It should be the 5th argument but I only see 4. Please try again." << endl;
			exit(1);
		}
		//
		if (verbose) cout << "You've chosen 'custom', the adventurous type I see." << endl;
	}
	else{
		cout << "Uh oh, I don't recognize your choice of method (" << arg[0] << "). Currently supported methods are" << endl;
		cout << "  'fEuler', 'midpt', 'rk4' and 'custom'. Please try again." << endl;
		exit(1);
	}

	// Argument 2: RB or SB
	if (verbose) cout << "2: ";
	if (arg[1] == "rb" || arg[1] == "RB"){
		if (verbose) cout << "You've chosen a reaction-based approach. I like it, I like it a lot." << endl;
	}
	else if (arg[1] == "sb" || arg[1] == "SB"){
		if (verbose) cout << "You've chosen a species-based approach. Excellent choice sir." << endl;
	}
	else{
		cout << "Oops, the 2nd argument must specify rxn-based ('rb' or 'RB') or species-based ('sb' or 'SB')." << endl;
		cout << "You entered '" << arg[1] << "'. Please try again." << endl;
		exit(1);
	}

	// Argument 3: PLA configuration
	if (verbose) cout << "3: ";
	if (arg[2] == "pre:neg"){
		if (verbose){
			cout << "You've chosen to use a preleap tau calculator and a negative-population postleap checker." << endl;
			cout << "   Very good, a straightforward but effective choice." << endl;
		}
	}
	else if (arg[2] == "pre:post"){
		if (verbose){
			cout << "You've chosen to use a preleap tau calculator and an epsilon-based postleap checker." << endl;
			cout << "   Excellent, this should improve the accuracy of your results." << endl;
		}
	}
	else if (arg[2] == "post:post"){
		if (verbose){
			cout << "Ahh, you must be hungry, you've chosen the combo meal: an Anderson-style combined" << endl;
			cout << "   tau calculator/postleap checker. I like how you think." << endl;
		}
	}
	else{
		cout << "Uh oh, I don't recognize your preleap:postleap configuration. Supported configurations are" << endl;
		cout << "'pre:neg', 'pre:post' and 'post:post'. You entered '" << arg[2] << "'. Please try again." << endl;
		exit(1);
	}

	// Argument 4: PLA parameters
	if (verbose) cout << "4: ";
	last = -1;
	arg[3].append(",");
	string s, name, val;
	param.resize(7,NAN);
	// Default values
	param[1] = 3.0;   // apx1
	param[2] = 100.0; // gg1
	param[3] = 0.5;   // p
	param[4] = 0.8;   // pp
	param[5] = 1.5;   // q
	param[6] = 0.75;  // w
	vector<bool> definedParam;
	definedParam.resize(7,false);
	while ((next = arg[3].find(',',last+1)) != (int)string::npos){
		s = arg[3].substr(last+1,next-(last+1));
		unsigned int equal = s.find("=");
		if (equal == string::npos){ // Error check
			cout << "Oops, PLA parameters must be input in the form 'param=val'. "
					"You input " << s << ". Please try again." << endl;
		}
		name = s.substr(0,equal);
		Util::remove_whitespace(name);
		val = s.substr(equal+1,string::npos);
		double valDbl;
		if (val == "INFINITY") valDbl = INFINITY;
		else valDbl = atof(val.c_str());
		if (name == "eps"){
			param[0] = valDbl;
			definedParam[0] = true;
		}
		else if (name == "apx1"){
			param[1] = valDbl;
			definedParam[1] = true;
		}
		else if (name == "gg1"){
			param[2] = valDbl;
			definedParam[2] = true;
		}
		else if (name == "p"){
			param[3] = valDbl;
			definedParam[3] = true;
		}
		else if (name == "pp"){
			param[4] = valDbl;
			definedParam[4] = true;
		}
		else if (name == "q"){
			param[5] = valDbl;
			definedParam[5] = true;
		}
		else if (name == "w"){
			param[6] = valDbl;
			definedParam[6] = true;
		}
		else{ // Error check
			cout << "Oops, I don't recognize the parameter " << name << " in argument 4: \"" << arg[3] << "\". "
				 << "Please try again." << endl;
			exit(1);
		}
		last = next;
	}

	// Error check
	if (!definedParam[0]){ // User required to define 'eps'. All other parameters have default values.
		cout << "Oops, must provide a value for 'eps' (<< 1). Please try again." << endl;
		exit(1);
	}

	if (verbose){
		cout << "Your simulation parameters for this run are:" << endl;
		for (unsigned int i=0;i < param.size();i++){
			if (i==0) cout << "   eps = "	<< param[0];
			if (i==1) cout << "   apx1 = "	<< param[1];
			if (i==2) cout << "   gg1 = " 	<< param[2];
			if (i==3) cout << "   p = " 	<< param[3];
			if (arg[2] == "post:post"){
				if (i==4) cout << "   pp = " 	<< param[4];
				if (i==5) cout << "   q = " 	<< param[5];
				if (i==6) cout << "   w = " 	<< param[6];
			}
			if (i < 4 || arg[2] == "post:post"){
				if (!definedParam[i]) cout << " (default)";
				cout << endl;
			}
		}
	}

	// Argument 5 (optional): Path to Butcher tableau input file
	if (arg.size() == 5){
		if (verbose) cout << "5: ";
		if (verbose) cout << "You've specified a Butcher tableau input file: " << arg[4] << endl;

		// Continue
		if (arg[0] != "custom"){ // skip
			if (verbose) cout << "   You haven't chosen 'custom', so I'll ignore it." << endl;
		}
		else
		{
			// Try to open the file
			if (fopen(arg[4].c_str(),"r")){
				if (verbose) cout << "   Ok, I see it." << endl;
			}
			else{
				cout << "   Sorry , I can't find it: \"" << arg[4].c_str() << "\". Please try again." << endl;
				exit(1);
			}
			// Read file
			read_Butcher_tableau(arg[4],alpha,beta,verbose);
		}
	}

	// Build the PLA simulator
	TauCalculator* tc = 0;
	RxnClassifier* rc = 0;
	FiringGenerator* fg = 0;
	PostleapChecker* pl = 0;
	//
	ButcherTableau bt(alpha,beta);
	double eps = param[0];
	double approx1 = param[1];
	double gg1 = param[2];
	double p = param[3];
	//
	// Preleap TC (required for all configurations)
	Preleap_TC* ptc;
	if (arg[1] == "rb" || arg[1] == "RB"){
		static fEulerPreleapRB_TC fe_ptc(eps,REACTION);
		ptc = &fe_ptc;
	}
	else{
		static fEulerPreleapSB_TC fe_ptc(eps,SPECIES,REACTION);
		ptc = &fe_ptc;
	}
	//
	// Configuration 1: preTC_RC_FG_negPL
	if (arg[2] == "pre:neg"){
		static eRungeKutta_preTC_RC_FG_negPL erk_tc_rc_fg_pl(bt,eps,approx1,gg1,p,ptc,SPECIES,REACTION);
		tc = &erk_tc_rc_fg_pl;	rc = &erk_tc_rc_fg_pl;	fg = &erk_tc_rc_fg_pl;	pl = &erk_tc_rc_fg_pl;
	}
	// Configuration 2: preTC_RC_FG_postPL
	if (arg[2] == "pre:post"){
		if (arg[1] == "rb" || arg[1] == "RB"){
			static eRungeKutta_preTC_RC_FG_rbPL erk_tc_rc_fg_pl(bt,eps,approx1,gg1,p,ptc,SPECIES,REACTION);
			tc = &erk_tc_rc_fg_pl;	rc = &erk_tc_rc_fg_pl;	fg = &erk_tc_rc_fg_pl;	pl = &erk_tc_rc_fg_pl;
		}
		else{
			static eRungeKutta_preTC_RC_FG_sbPL erk_tc_rc_fg_pl(bt,eps,approx1,gg1,p,ptc,SPECIES,REACTION);
			tc = &erk_tc_rc_fg_pl;	rc = &erk_tc_rc_fg_pl;	fg = &erk_tc_rc_fg_pl;	pl = &erk_tc_rc_fg_pl;
		}
	}
	// Configuration 3: postTC_RC_FG_postPL
	if (arg[2] == "post:post"){
		double pp = param[4];
		double q = param[5];
		double w = param[6];
		if (arg[1] == "rb" || arg[1] == "RB"){
			static eRungeKutta_postTC_RC_FG_rbPL erk_tc_rc_fg_pl(bt,eps,approx1,gg1,p,pp,q,w,ptc,SPECIES,REACTION);
			tc = &erk_tc_rc_fg_pl; rc = &erk_tc_rc_fg_pl; fg = &erk_tc_rc_fg_pl; pl = &erk_tc_rc_fg_pl;
		}
		else{
			static eRungeKutta_postTC_RC_FG_sbPL erk_tc_rc_fg_pl(bt,eps,approx1,gg1,p,pp,q,w,ptc,SPECIES,REACTION);
			tc = &erk_tc_rc_fg_pl; rc = &erk_tc_rc_fg_pl; fg = &erk_tc_rc_fg_pl; pl = &erk_tc_rc_fg_pl;
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

	// Functions file (optional)
/*	FILE* fdat = NULL;
	string fFile = outpre + ".fdat";
	if ((fdat = fopen(fFile.c_str(),"r"))){
		fclose(fdat);
		fdat = fopen(fFile.c_str(),"a");
	}
	else{
//		cout << "Warning: Functions file \"" << fFile << "\" doesn't exist." << endl;
	}*/

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
		PLA_SIM->nextStep();

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
				cout << "\t" << fixed << setprecision(6) << time << "\t" << setprecision(0) << step;
//				for (unsigned int i=0;i < OBSERVABLE.size();i++){
//					cout << "\t" << OBSERVABLE[i]->second;
//				}
				if (print_save_net){
					fprintf(stdout, "%s", print_net_message.c_str());
				}
				cout << endl;
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
		cout << "   -----" << endl;
		cout << "   Columns:\t" << dim << endl;
		cout << "   Rows:\t" << nRows << endl;
		cout << "   -----" << endl;
		for (unsigned int i=0;i < alpha.size();i++){
			cout << "   ";
			for (unsigned int j=0;j < alpha[i].size();j++){
				cout << alpha[i][j] << "\t";
			}
			cout << endl;
		}
		cout << "   -----" << endl;
		cout << "   ";
		for (unsigned int i=0;i < beta.size();i++){
			cout << beta[i] << "\t";
		}
		cout << endl;
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
