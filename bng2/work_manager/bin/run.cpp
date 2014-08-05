#include<iostream>
#include<cstdlib>
#include<map>

using namespace std; 

int main(int argc, char* argv[]) {

    map<string, int> args; 
    args["-m"]=0; 
    args["-t"]=1; 
    args["-n"]=2; 
    args["-q"]=3; 
    args["-p"]=4;
    args["-log"]=5; 
    args["-lin"]=5; 
    args["-lgauss"]=5; 
    
    map<int, string> s; 

    int i = 1; 
    int k = 10; 
    while(i < argc )  {
        switch(args[argv[i]]) {
            case 0: 
                s[0]=argv[i++]; 
                s[1]=argv[i++]; 
                break; 
            case 1: 
                s[2]=argv[i++]; 
                s[3]=argv[i++]; 
                break; 
            case 2:
                s[4]=argv[i++]; 
                s[5]=argv[i++]; 
                break; 
            case 3:
                s[6]=argv[i++]; 
                s[7]=argv[i++]; 
                break; 
            case 4:
                s[8]=argv[i++]; 
                s[9]=argv[i++]; 
                break; 
            case 5:
                s[k++]=argv[i++];
                s[k++]=argv[i++]; 
                s[k++]=argv[i++]; 
                s[k++]=argv[i++]; 
                break; 
            default: 
                i++; 
        }
    }

    string path = argv[0]; 
    path.erase(path.end()-3, path.end()); 
    
    string mpirun = path + "mpirun"; 
    string work_manager = path + "work_manager"; 
    string run_network = path + "run_network"; 
    
    string cmd = mpirun + " -np 1 -quiet " + work_manager + " " + run_network + " "; 
    for (map<int, string>::const_iterator it = s.begin(); it != s.end(); it++) {
         cmd += it->second + " "; 
    }
    cout << "Full command: " << cmd << endl; 
    system(cmd.c_str()); 
    return 0; 

}
