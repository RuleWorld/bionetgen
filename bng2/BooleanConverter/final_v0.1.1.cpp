#include"BoolExprParser.h"
#include<assert.h>
#include<stdlib.h>
#include<cstdio>

#ifdef HAVE_GETOPT_LONG
#include<unistd.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include<getopt.h>
#endif

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;
using namespace boolstuff;

ifstream infile;
ofstream outfile;
vector<string> var;
vector<int> state;
vector<string> boolRules;
vector<string> strRulesFinal;
vector<string> strSpecies;
vector<string> strMethod;
vector<vector<vector<string> > > fullRule;
vector<vector<string> > orRule;
vector<string> andRule;
string strHold;
string strTemp;
int lineCount = 1;
char c;
stringstream streamTemp;
vector<int> ruleLine;
vector<size_t> divPlace;
int ruleCount = 0;

int main(int argc, char* argv[]){
    char* fileName = "test.bngb";//argv[1];
    char inFileAppend[] = ".bngb";
    char outFileAppend[] = ".bngl";
    char* cstrTemp;
    bool readRules();
    bool readSpecies();
    bool readMethod();
    void writeOut();

    infile.open(fileName);
	if(!infile.is_open()){
		cout << "File not found\n";
		return 1;
	}
	if(strstr(fileName, inFileAppend) != NULL){
	    cstrTemp = new char[strlen(fileName) - strlen(inFileAppend) + 1 + strlen(outFileAppend)];
        for(size_t i = 0; i < strlen(fileName) - strlen(inFileAppend); i++)
            cstrTemp[i] = fileName[i];
        cstrTemp[strlen(fileName) - strlen(inFileAppend)] = '\0';
        strncat(cstrTemp, outFileAppend, strlen(outFileAppend));
	}
    else{
        cstrTemp = new char[strlen(fileName) + strlen(outFileAppend) + 1];
        strcpy(cstrTemp, fileName);
        strncat(cstrTemp, outFileAppend, strlen(outFileAppend));
    }

    outfile.open(cstrTemp);
	if(!outfile.is_open()){
		cout << "Cannot create outfile\n";
		return 1;
	}

    c = infile.get();
    while(isspace(c)){ // next read line
        if(c == '\n')
            lineCount++;
        c = infile.get();
    }
    while(true){
    	if(c >= 0){
                if(!isalpha(c))
                    c = infile.get();
    			while(isspace(c)){ // skip space
                    if(c == '\n')
                        lineCount++;
                        c = infile.get();
                }
    			while(isalpha(c)){
    				strHold += c;
    				c = infile.get();
    			}
    			if(strHold == "species"){
    			    strHold.resize(0);
    				if(!readSpecies())
                        return 1;
    			}
    			else if(strHold == "rules"){
    			    strHold.resize(0);
    				if(!readRules())
                        return 1;
    			}
    			else if(strHold == "method"){
    			    strHold.resize(0);
    				if(!readMethod())
						return 1;
    			}
    			else{
    			    cout << "Invalid header on line " << lineCount << endl;
    			    return 1;
    			}
    	}
    	else
    		break;
    }

    writeOut();

    cout << endl << cstrTemp << " created\n";

	infile.close();
	outfile.close();

    delete cstrTemp;

    return 0;
}



bool readSpecies(){
    while(isspace(c)){ // next read line
        if(c == '\n')
            lineCount++;
        c = infile.get();
    }
    while(c != '#' and !infile.eof()){
        if(isalnum(c) or c == '_'){
            while(isalnum(c) or c == '_'){
                strTemp += c;
                c = infile.get();
            }
            for(size_t i = 0; i < strSpecies.size(); i++)
                if(strSpecies[i] == strTemp){
                    cout << "Repeated molecule on line " << lineCount;
                    return false;
                }
            strSpecies.push_back(strTemp);
            state.push_back(1);
            strTemp.resize(0);
        }
        else if(c == '!'){
            c = infile.get();
            while(isalnum(c) or c == '_'){
                strTemp += c;
                c = infile.get();
            }
            for(size_t i = 0; i < strSpecies.size(); i++)
                if(strSpecies[i] == strTemp){
                    cout << "Repeated molecule on line " << lineCount;
                    return false;
                }
            strSpecies.push_back(strTemp);
            state.push_back(0);
            strTemp.resize(0);
        }
        else if(!isspace(c)){
            cout << "Invalid character on line " << lineCount << endl;
            return false;
        }
        while(isspace(c)){ // next read line
            if(c == '\n')
                lineCount++;
            c = infile.get();
        }
    }
    return true;
}



bool readRules(){
	bool makeRules();
    BoolExprParser parser;
    while(true){
        while(isspace(c)){ // next read line
            if(c == '\n')
                lineCount++;
            c = infile.get();
        }
        while(true)
            if(isalnum(c) or c == '_'){ // get var before =
                strHold += c;
                c = infile.get();
            }
            else
                break;

        if(c == '#')
            break;

        var.push_back(strHold);
        ruleLine.push_back(lineCount);
        strHold.resize(0);
        while(true) // skip ', :, and =; if not there, error
            if(!isalnum(c) and c != '_' and c != '(' and c != '!'){
                strHold += c;
                c = infile.get();
            }
            else
                break;
        if(strHold != "': = "){
            cout << "Invalid formatting on line " << lineCount << endl;
            return false;
        }
        strHold.resize(0);
        for(int i = 0; i < 2; i++){ // do 2x, once for yes, once for not
            if(i == 0){
                while(true)
                    if(c != '\n'){ // read rest of line after =
                        strHold += c;
                        c = infile.get();
                    }
                    else
                        break;
            }
            else if(i == 1)
                strHold = "!(" + strHold + ')';
            try{
                BoolExpr<string> *expr = parser.parse(strHold);
                assert(expr != NULL);
                expr = BoolExpr<string>::getDisjunctiveNormalForm(expr);
                if(i == 0)
                    streamTemp << expr << '*';
                else
                    streamTemp << '*' << expr << '*';
                getline(streamTemp, strTemp);
                boolRules.push_back(strTemp);
                streamTemp.str("");
                streamTemp.clear();
                strTemp.resize(0);
                if(expr == NULL)
                    cout << "Line " << lineCount << ": invalid expression\n";
                delete expr;
            }
            catch(BoolExprParser::Error &err){
                cout << "Line " << lineCount << ": ";
                switch(err.code){
                    case BoolExprParser::Error::GARBAGE_AT_END:
                        cout << "garbage at end\n";
                        return false;
                    case BoolExprParser::Error::RUNAWAY_PARENTHESIS:
                        cout << "runaway parenthesis\n";
                        return false;
                    case BoolExprParser::Error::IDENTIFIER_EXPECTED:
                        cout << "identifier expected\n";
                        return false;
                }
            }
        }
        strHold.resize(0);
    }

    if(!makeRules())
        return false;
    boolRules.resize(0);

    c = infile.get(); // was #, now read next
    while(isspace(c)){ // next read line
        if(c == '\n')
            lineCount++;
        c = infile.get();
    }

    return true;
}



bool makeRules(){
    void reduce();
    bool found = false;
    bool reversed;
    bool skip;
    int w;

    reduce();

	for(size_t i = 0; i < 2 * var.size(); i++){
		if(i % 2 != 0)
			reversed = true;
		else
			reversed = false;
		for(size_t j = 0; j < fullRule[i].size(); j++){
            skip = false;
            for(size_t k = 0; k < fullRule[i][j].size() and !skip; k++){
                w = 0;
                c = fullRule[i][j][k][w++];
                    if(c != '!'){
                        while(isalnum(c) or c == '_'){
                            strTemp += c;
                            c = fullRule[i][j][k][w++];
                        }
                        for(size_t m = 0; m < strSpecies.size(); m++)
                            if(strTemp == strSpecies[m]){
                                found = true;
                                break;
                            }
                        if(!found){
                            cout << "Unrecognized molecule used in rule on line " << ruleLine[i] << endl;
                            return false;
                        }
                        found = false;
                        if(strTemp != var[i / 2]){
                            skip = false;
                            strHold += strTemp;
                            strHold += "(s~1)";
                        }
                        else{ // if strTemp == var[i / 2]
                            strHold.resize(0);
                            skip = true;
                        }
                        strTemp.resize(0);
                    }
                    else{ // if !
                        c = fullRule[i][j][k][w++];
                        while(isalnum(c) or c == '_'){
                            strTemp += c;
                            c = fullRule[i][j][k][w++];
                        }
                        for(size_t m = 0; m < strSpecies.size(); m++)
                            if(strTemp == strSpecies[m]){
                                found = true;
                                break;
                            }
                        if(!found){
                            cout << "Unrecognized molecule used in rule on line " << ruleLine[i] << endl;
                            return false;
                        }
                        found = false;
                        if(strTemp != var[i / 2]){
                            skip = false;
                            strHold += strTemp;
                            strHold += "(s~0)";
                        }
                        else{ // if strTemp == var[i / 2]
                            strHold.resize(0);
                            skip = true;
                        }
                        strTemp.resize(0);
                    }
                    if(k != fullRule[i][j].size() - 1)
                        strHold += " + ";
            }
            if(!skip){
                if(!reversed)
                    strRulesFinal.push_back(var[i / 2] + "(s~0) + " + strHold + " -> " + var[i / 2] + "(s~1) + " + strHold);
                else // if reversed
                    strRulesFinal.push_back(var[i / 2] + "(s~1) + " + strHold + " -> " + var[i / 2] + "(s~0) + " + strHold);
                ruleCount++;
            }
            strHold.resize(0);
        }
	}
	var.resize(0);

	return true;
}



void reduce(){
    bool bump = false;
    int equiNameCount;
    size_t minSize;
    int erasePos;
    for(size_t i = 0; i < boolRules.size(); i++){
        int j = 0;
        c = boolRules[i][j++];
        if(i % 2 != 0)
            c = boolRules[i][j++];
        while(c != '*'){
            while(c != '|' and c != '*'){
                while(c != '&' and c != '|' and c != '*'){
                    strHold += c;
                    c = boolRules[i][j++];
                }
                andRule.push_back(strHold);
                strHold.resize(0);
                if(c != '*' and c != '|')
                    c = boolRules[i][j++];
            }
            orRule.push_back(andRule);
            andRule.resize(0);
            if(c != '*')
                c = boolRules[i][j++];
        }
        fullRule.push_back(orRule);
        orRule.resize(0);
    }

    boolRules.resize(0);

    for(size_t i = 0; i < fullRule.size(); i++) // fullRule
        for(size_t j = 0; j < fullRule[i].size(); j++){ // orRule
            bump = false;
            for(size_t k = 0; k < fullRule[i].size() and !bump; k++) // orRule
                if(k != j){
                    equiNameCount = 0;
                    if(fullRule[i][j].size() > fullRule[i][k].size()){
                        minSize = fullRule[i][k].size();
                        erasePos = j;
                    }
                    else{
                        minSize = fullRule[i][j].size();
                        erasePos = k;
                    }
                    for(size_t m = 0; m < fullRule[i][j].size(); m++)
                        for(size_t n = 0; n < fullRule[i][k].size(); n++)
                            if(fullRule[i][j][m] == fullRule[i][k][n])
                                equiNameCount++;
                    if(equiNameCount == int(minSize)){
                        fullRule[i].erase(fullRule[i].begin() + erasePos);
                        if(minSize == fullRule[i][k].size()){
                            bump = true;
                            j--;
                        }
                        k--;
                    }
                }
        }

}



bool readMethod(){
    c = infile.get();
    while(isspace(c)){ // next read line
        if(c == '\n')
            lineCount++;
        c = infile.get();
    }
	switch(c){ // all are same for now - need to figure out s and h
		case 'a':
			while(true){
				c = infile.get();
				if(isdigit(c))
					strHold += c;
				else if(c == ')')
					break;
			}
            streamTemp << float(atoi(strHold.c_str()))/(strRulesFinal.size())*2;
			getline(streamTemp, strTemp);
			strMethod.push_back("simulate_ssa({t_end=>" + strTemp + ",n_steps=>" + strHold + "});");
			break;
		case 's':
			while(true){
				c = infile.get();
				if(isdigit(c))
					strHold += c;
				else if(c == ')')
					break;
			}
            streamTemp << float(atoi(strHold.c_str()))/(strRulesFinal.size())*2;
			getline(streamTemp, strTemp);
			strMethod.push_back("simulate_ssa({t_end=>" + strTemp + ",n_steps=>" + strHold + "});");
			break;
		case 'h':
			while(true){
				c = infile.get();
				if(isdigit(c))
					strHold += c;
				else if(c == ')')
					break;
			}
            streamTemp << float(atoi(strHold.c_str()))/(strRulesFinal.size())*2;
			getline(streamTemp, strTemp);
			strMethod.push_back("simulate_ssa({t_end=>" + strTemp + ",n_steps=>" + strHold + "});");
			break;
	}
	c = infile.get();
	if(c >= 0)
        while(isspace(c)){ // next read line
            if(c == '\n')
                lineCount++;
            c = infile.get();
        }
    strTemp.resize(0);
    streamTemp.str("");
    streamTemp.clear();
    strHold.resize(0);
    return true;
}




void writeOut(){
	outfile << "begin parameters" << endl
			<< "c 1" << endl
			<< "k 1" << endl
			<< "end parameters" << endl << endl;

	outfile << "begin molecule types" << endl;
	for(size_t i = 0; i < strSpecies.size(); i++)
        outfile << strSpecies[i] << "(s~0~1)" << endl;
	outfile << "end molecule types" << endl << endl;

	outfile << "begin species" << endl;
    for(size_t i = 0; i < strSpecies.size(); i++)
		outfile << strSpecies[i] << "(s~" << state[i] << ") c" << endl;
	outfile << "end species" << endl << endl;

	outfile << "begin observables" << endl;
	for(size_t i = 0; i < strSpecies.size(); i++)
		outfile << "Molecules " << strSpecies[i] << " " << strSpecies[i] << "(s~1)" << endl;
	outfile << "end observables" << endl << endl;

	outfile << "begin reaction rules" << endl;
	for(size_t i = 0; i < strRulesFinal.size(); i++){
		if(i > 0){
		    int j = 0;
		    c = strRulesFinal[i][j];
            while(c != '('){
                strHold += c;
                c = strRulesFinal[i][++j];
            }
            j = 0;
            c = strRulesFinal[i - 1][j];
            while(c != '('){
                strTemp += c;
                c = strRulesFinal[i - 1][++j];
            }
            if(strHold != strTemp)
                outfile << "#-------------------------\n";
            strHold.resize(0);
            strTemp.resize(0);
		}
		outfile << strRulesFinal[i] << " k" << endl;
	}
	outfile << "end reaction rules" << endl << endl;

	outfile << "generate_network({overwrite=>1});" << endl
			<< strMethod[0];

    strRulesFinal.resize(0);
    strSpecies.resize(0);
    strMethod.resize(0);
}
