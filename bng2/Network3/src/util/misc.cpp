/*
 * misc.cpp
 *
 *  Created on: Apr 20, 2012
 *      Author: Leonard Harris
 */

#include "misc.hh"

// Check whether a variable is NAN
bool Util::isNAN(double x){
	return x != x;
}

// Remove leading and trailing whitespace from a string
void Util::remove_whitespace(string& s){
	// Remove leading whitespace
	while (s.at(0) == ' ' || s.at(0) == '\t') s.erase(0,1);
	// Remove trailing whitespace
	while (s.at(s.size()-1) == ' ' || s.at(s.size()-1) == '\t') s.erase(s.size()-1,1);
}

double Util::Mratio(double a, double b, double z){
	// Original Fortran code written by William Hlavacek (2018)
	// Tranlsated to Python and then C++ by Leonard A. Harris (2019)

	//# c This routine calculates the ratio M(a+1,b+1,z)/M(a,b,z)
	//# c as a continued fraction f in the class of Gauss's continued fraction
	//# c [Gauss CF (1813)]
	//# c using the modified method of Lentz WJ (1976) [Applied Optics 15:668-671]
	//# c [Thompson IJ, Barnett AR (1986) J Comput Phys 64: 490-509].
	//# c M(a,b,z) = {}_1F_1(a;b;z) is Kummer's (confluent hypergeometric) function
	//# c [Kummer EE (1837) Crelle's Journal 17:228-242].
	//# c In general, we take f to have the following form:
	//# c f = q_0 + \frac{p_1}{q_1 +} \frac{p_2}{q_2 +} \frac{p_3}{q_3 +} \cdots
	//# c For the ratio of interest,
	//# c p_1 = 1
	//# c p_2 = z*[a-(b+0)]/[(b+0)*(b+1)]
	//# c p_3 = z*(a+1)    /[(b+1)*(b+2)]
	//# c p_4 = z*[a-(b+1)]/[(b+2)*(b+3)]
	//# c p_5 = z*(a+2)    /[(b+3)*(b+4)]
	//# c q_0 = 0
	//# c q_j = 1 for j = 1, 2, ...
	//# c [Van Vleck EB (1901) Ann Math 3: 1-18].
    double eps = 1e-16;
    double tiny = 1e-32;
	int j,iodd,ieven;
	int odd,oddsave,even,evensave;
	double f,fsave,C,Csave,D,Dsave,err;
	double p,q,num,den,Delta;

	//# c initialize
	//# c In general, we set f_0 = q_0.
	//# c However, if q_0 = 0, we set f_0 = tiny instead.
	//# c We then set C_0 = f_0 and D_0 = 0.
    fsave=tiny;
    Csave=fsave;
    Dsave=0.0;

	//# c We set err to a value greater than eps.
    err=1.0+eps;

    odd=1;
    even=0;
    iodd=0;
    ieven=0;
    j=0;
    while(err > eps){
        j=j+1;

		//# c calculate p_j and q_j

		//# c p_1 = 1
		//# c p_j = num_j/den_j for j = 2, 3, ...
		//# c den_j = [b+(j-2)]*[b+(j-1)] for j = 2, 3, ...
		//# c if j>1 is even, then num_j = z*[a-(b+(j-2)/2)] = z*(a-(b+ieven-1))
		//# c if j>1 is odd, then num_j = z*[a+(j-1)/2] = z*(a+iodd)
        if(j == 1){
            p=1.0;
        }
        else if(j > 1){
            den=(b+double(j-2))*(b+double(j-1));
            if(odd == 1){
                iodd=iodd+1;
                num=z*(a+double(iodd));
            }
            else if(even == 1){
                ieven=ieven+1;
                num=z*(a-(b+double(ieven-1)));
            }
            else{
                cout << "Error: iodd=" << iodd << ", ieven=" << ieven << endl;
                exit(1);
            }
            p=num/den;
        }
        else{
            cout << "Error: j=" << j << endl;
            exit(1);
        }

		//# c q_j = 1 for j = 1, 2, ...
        q=1.0;

		//# c calculate jth terms in recurrence relations
		//# c C_j = q_j + p_j/C_{j-1}
		//# c D_j = 1/(q_j + p_j*D_{j-1})
        D=q+p*Dsave;
        if(abs(D) < tiny){
            D=tiny;
        }
        C=q+p/Csave;
        if(abs(C) < tiny){
            C=tiny;
        }
        D=1.0/D;

		//# c current approximation
		//# c f_j = C_j*D_j*f_{j-1}
        Delta=C*D;
        f=Delta*fsave;

		//# c if Delta is sufficiently close to 1, then the current approximation is acceptable.
        err=abs(Delta-1.0);

		//# c prepare for next iteration
        fsave=f;
        Csave=C;
        Dsave=D;
        oddsave=odd;
        evensave=even;
        odd=evensave;
        even=oddsave;
    }

    return f;
}
