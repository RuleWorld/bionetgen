#ifndef PARALLEL_H_
#define PARALLEL_H_
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <exception>
#include <map>
#include <mpi.h>
#define MAX_DAT 5000
#define MAX_LEN 100
#define MAX_SIZE 1000
#define DIE 0 
#define ROOT 0
#define high 1

using namespace std; 

typedef struct{
   char msg1[MAX_LEN];
   int worker_id; 
   int count; 
   int rank; 
   int pid; 
   double tau; 
   int group_len; 
   double group_conc[50]; 
   double spec_conc[MAX_DAT]; 
} Msg; 

typedef struct{
    int key; 
    char par_name[MAX_LEN]; 
    double par_value_min; 
    double par_value_max; 
    double par_value; 
} Greeting;      

class ReadMSG{
public: 
   ReadMSG(): cue(0), rankval(0), pidval(0), grpid(0), spcid(0), timeval(0), push(false), 
       rank(false), pid(false), group(false), species(false), conval(0), conc(false), time(false) { }  
   int cue; 
   int rankval; 
   int pidval; 
   int grpid; 
   int spcid; 
   int conval; 
   double timeval;
   void read(char* msg); 
   ~ReadMSG() { } 
private: 
   bool push, pid, rank, group, species, conc, time;  
}; 

class Worker{
public: 
   Worker():isMPI(false){ } 
   Msg msg; 
   bool isMPI;  
   int id;     // worker group id 
   int size;   // worker group size (number of child process in the group)  
   int pid;    // process ids; unique for each child process in the group
   int rank;   // process ranks; 0, 1, 2, ...size-1 
   bool start_MPI(char**, int, NETWORK); 
   int worker_client( int argc, char **argv, Group* group, Elt_array* earray, double tau ); 
   void send_final( Group* group, Elt_array* earray, double tau );  
   void scan_parameter(Elt_array*, NETWORK*); 
   Greeting grt; 
   ~Worker(){ }
private:
   MPI_Comm universe; 
   MPI_Comm parent; 
   void mpi_send(); 
   void mpi_recv(); 
   void mpi_bcast(); 
   void mpi_gather(); 
   void get_spec_conc(Elt_array*); 
   void get_group_conc(Group*, Elt_array*); 
   void get_sim_time(double); 
}; 


Msg msglist[MAX_SIZE]; 
Worker worker; 

char cbuf[MAX_LEN];
int tag, rank; 
vector<char*> tokenlist; 
map<string, int> comkey; 

MPI_Status status;
MPI_Comm server;
MPI_Request *request; 
    char buf[100]; 
 
void split(char*, const char*, vector<char*>&); 
void perr(const char*, int); 
Msg nonroot_response(char*, vector<int>&, double, double*);

bool Worker::start_MPI(char** argv, int iarg, NETWORK net){
    isMPI = true; 
    int n_sibling = 0; 

    MPI_Init(0, NULL);

    pid = getpid(); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    MPI_Comm_get_parent(&parent); 
    n_sibling = atoi(*(argv + iarg)); 

    if (parent == MPI_COMM_NULL){
        assert(n_sibling == size);  
        MPI_Finalize();
        return (n_sibling > 0)? true : false; 
    }
    
    //merge_parent(); 
    MPI_Intercomm_merge(parent, 1, &universe); 
    //mpi_bcast(); 
    MPI_Recv(&grt, sizeof(Greeting), MPI_BYTE, 0, 100, universe, &status);  
    MPI_Send(&grt, sizeof(Greeting), MPI_BYTE, 0, 100, universe);  
    
    cout << "I am rank " << rank << " with pid " << pid << " in " << n_sibling << " child processes" <<  endl; 
    return (n_sibling > 0)? true : false; 
}


void Worker::scan_parameter(Elt_array* rates, NETWORK* net){

    if (grt.key > 0) {
    
        size_t par_name_len = ( (string) grt.par_name).length();   

        if (par_name_len > MAX_LEN) {
            perr("Parameter name exceeds 100 characters",__LINE__);  
        }

        /*double par_value, par_value_min, par_value_max, par_value_step; 

        if (grt.key == 1){ // log scale
            if (grt.par_value_min <= 0 || grt.par_value_max <= 0)
                perr("Logarithmic conversion failed. Parameter value must be greater than zero",__LINE__); 

            par_value_min = log10(grt.par_value_min); 
            par_value_max = log10(grt.par_value_max); 
            par_value_step = (par_value_max - par_value_min)/(size-1);  
            par_value = pow(10,(par_value_min + rank*par_value_step)); 
        
            grt.par_value = par_value; 
        }
        else if (grt.key == 2) { // linear scale
            par_value_min = grt.par_value_min; 
            par_value_max = grt.par_value_max;
            par_value_step = (par_value_max - par_value_min)/(size - 1); 
            par_value = par_value_min + rank*par_value_step; 
       
            grt.par_value = par_value; 
        }
        else 
            perr("Unknown scaling of parameter scan, must be in log or linear scale",__LINE__);*/ 

        char par1[MAX_LEN]; 
        char par2[MAX_LEN]; 
        bool parameter_not_found = true; 

        for (vector<myParser>::iterator it = (net->parameters).begin(); it != (net->parameters).end(); it++) {
            bzero(par1, MAX_LEN); 
            bzero(par2, MAX_LEN); 
        
           // sprintf(par1,"%s",par_name.c_str()); 
           // cout << "PARAMETER NAME " << grt.par_name << endl; 
            sprintf(par1,"%s", grt.par_name); 
            sprintf(par2,"%s", (it->name).c_str()); 

            if (!strcmp(par1, par2)) {
                //cout << par1 << "      " << par_value << "       " << getpid() <<  endl; 
                parameter_not_found = false;  
                it->val = grt.par_value; 
                char expr[MAX_LEN]; 
                bzero(expr,MAX_LEN); 
                sprintf(expr,"%e",it->val); 
                (it->p).SetExpr(expr); 
            } 
             
        }

        if (parameter_not_found) 
            perr("No matching parameter name found for parameter scan",__LINE__); 
        else { 
            map<string, double*> param_map; 
            for(vector<myParser>::iterator it = (net->parameters).begin(); it != (net->parameters).end(); it++){
                param_map[it->name] = &(it->val); 
            }

            for (vector<myParser>::iterator it = (net->parameters).begin(); it != (net->parameters).end(); it++) {
                mu::varmap_type variables = (it->p).GetVar(); 
                mu::varmap_type::const_iterator item = variables.begin(); 
                for (; item != variables.end(); ++item){
                    try {
                        (it->p).DefineVar(item->first, param_map[item->first]); 
                       }
                    catch(exception &e) {
                        cout << e.what() << endl; 
                    }
                }
            }
 
            for (vector<myParser>::iterator it = (net->parameters).begin(); it != (net->parameters).end(); it++){
                string varlist; 
                mu::varmap_type variables = (it->p).GetVar(); 
                mu::varmap_type::const_iterator item = variables.begin();
                it->val  = (it->p).Eval(); 
                mu::console() << "param " << (it->name) << "    " << "Expression " << (it->p).GetExpr() <<  "  value " << it->val <<  _T("\n");

                for (; item!=variables.end(); ++item) {
                     mu::console() << "      " << item->first << "     "  << *(item->second) << endl;
                }

            }

	    if (network.has_functions){
	        for (Group* curr = network.spec_groups; curr != NULL; curr = curr->next) {
		    curr->total_val = 0;
		    for (int i = 0; i < curr->n_elt; i++)
		        curr->total_val += curr->elt_factor[i] * network.species->elt[curr->elt_index[i]-1]->val;
		}

		for (unsigned int j=0;j < network.var_parameters.size();j++) {
			network.rates->elt[network.var_parameters[j]-1]->val = network.functions[j].Eval();
		}
	    }

           for (Elt* elt = rates->list; elt != NULL; elt = elt->next) 
                elt->val = *(param_map[elt->name]); 
        }
       
    }

}


void Worker::mpi_send(){
    MPI_Send(&msg, sizeof(Msg), MPI_BYTE, 0, 1, universe);  
}

void Worker::mpi_recv(){
    MPI_Recv(&msg, sizeof(Msg), MPI_BYTE, 0, MPI_ANY_TAG, universe, &status);  
}

void Worker::mpi_bcast(){
    MPI_Bcast(&grt, sizeof(Greeting), MPI_BYTE, 0, universe); 
}

void Worker::mpi_gather(){
    Msg msglist[size]; 
    MPI_Gather(&msg, sizeof(Msg), MPI_BYTE, msglist, sizeof(Msg), MPI_BYTE, 0, universe);   
}

void Worker::get_spec_conc(Elt_array* earray){
    //vector<int> spec_conc; // species concentrations  
    //spec_conc.clear(); 
    int i = 0; Elt* e; 
    //while (e = earray->elt[i++]){
    bzero(msg.spec_conc,MAX_DAT); 
    try {
   	 while (e = earray->elt[i]){
        // spec_conc.push_back(e->val); 
       	     msg.spec_conc[i] = e->val; 
         }
     }
     catch(exception &e){
         cout << e.what() << endl; 
     } 
     
    //if (spec_conc.size() > MAX_DAT) perr("Species concentration list exceeds maximum limit",__LINE__); 
    //for (vector<int>::size_type i = 0; i != spec_conc.size(); i++){
      //  msg.spec_conc[i] = spec_conc[i]; 
    //}
}

void Worker::get_group_conc(Group* group, Elt_array* earray){
    //vector<int> group_conc; // species group concentrations  
    //group_conc.clear(); 
    int k = 0; 
    bzero(msg.group_conc,50); 
    for (Group* g = group; g != NULL; g = g->next){
         g->total_val = 0;
         for (int i = 0; i < g->n_elt; i++){
	     g->total_val += g->elt_factor[i]
					* earray->elt[g->elt_index[i] - 1]->val;
         }

         try{
             msg.group_conc[k] = g->total_val; 
             k++; 
         }
         catch(exception &e){
              cout << e.what() << endl; 
         }
      }
 
    msg.group_len = k;      
}

void Worker::get_sim_time(double tau){
        msg.tau = tau; 
}

void perr(const char* errmsg, int lineno){
    char errbuf[MAX_LEN]; 
    bzero(errbuf, MAX_LEN); 
    sprintf(errbuf, "%s:%d: error : %s", __FILE__, lineno, 
    errmsg); 
    cout << errbuf << endl; 
    exit(1); 
}

int Worker::worker_client( int argc, char **argv, Group* group, Elt_array* earray, double tau ){ 
if (!isMPI) return(0); 
if (parent == MPI_COMM_NULL) return (0); 

    mpi_recv(); 


    msg.rank = rank; 
    msg.pid = getpid(); 
    msg.tau = tau;     
    msg.worker_id = id; 
    
    get_group_conc(group, earray);  
    //get_spec_conc(earray); 
    get_sim_time(tau); 
   
    mpi_send();  

    if (!strcmp(msg.msg1,"die")) 
        return 1; 
    else 
        return 0; 
}

void Worker::send_final( Group* group, Elt_array* earray, double tau ){ 
    msg.rank = rank; 
    msg.pid = getpid(); 
    msg.tau = tau;     
    msg.worker_id = id; 
    
    get_group_conc(group, earray);  
    //get_spec_conc(earray); 
    get_sim_time(tau); 
   
    MPI_Send(&msg, sizeof(Msg), MPI_BYTE, 0, 111, universe);  // final data tag 111, different from 1 in mpi_send() 
    MPI_Finalize(); 

}

int client( int argc, char **argv, Group* group, Elt_array* earray, double tau ){ 
    int final_iter = worker.worker_client(argc, argv, group, earray, tau ); 
}

int send_final( Group* group, Elt_array* earray, double tau ){
    worker.send_final(group, earray, tau); 
}; 
#endif
