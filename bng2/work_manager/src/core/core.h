#ifndef CORE_H_
#define CORE_H_

#include <mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include <iostream>
#include<fstream>
#include <sstream> 
#include <exception>
#include <vector>
#include <map>
#include <cmath>
#include <thread>
#include <gsl_rng.h>
#include <gsl_randist.h>

#define MAX_LEN 100
#define UNIVERSE_SIZE 1000
#define MAX_SIZE 1000 
#define MAX_DAT 5000
#define ERR_LEN 1000
#define FATAL 1
#define low 0
using namespace std;

typedef struct{
    char msg1[MAX_LEN];
    int worker_id; 
    int count; 
    int rank; 
    int pid; 
    double tau; 
    int group_len; 
    int spec_len; 
    double group_conc[50]; 
    double spec_conc[MAX_DAT]; 
} Msg; 


typedef struct{
    int key; 
    int n_param; 
    char par_name[MAX_LEN][MAX_LEN]; 
    double par_value_min[MAX_LEN]; 
    double par_value_max[MAX_LEN]; 
    double par_value[MAX_LEN]; 
} Greeting;      

class Worker{
public: 
    Worker():size(0) {} 
    MPI_Comm universe;  
    MPI_Comm world; 
    int size; 
    Msg *msglist; 
    vector<Msg* > data; 
    ~Worker(){
        for (vector<Msg* >::const_iterator it = data.begin(); it != data.end(); it++){
            delete[] *it; 
        }
    }
}; 

class WorkManager{
public: 
    WorkManager(): stop_cond(false), n_worker(0), n_child(0), n_new_born(0), n_stop(0){}; 
    Worker worker[MAX_SIZE];
    Msg msg; 
    bool stop_cond; 
    int n_worker;  // number of work groups
    int n_child;   // total number of  active child processes (in all work groups) 
    void get_worker(int, char**);  
    void start_MPI(); 
    void end_MPI(); 
    void check_work_manager_size();  
    void exchange_data(); 
    void spawn_child_workers(); 
    void send_message(Msg, int, int); 
    void recv_message(Msg*, int ,int); 
    void bcast_message(Greeting*, int); 
    void gather_message(int); 
    void parameter_scan(int, string, string, double, double, int ); 
    void print_output(char*); 
    void set_observable_names(); 
    vector<string> observable_names; 
    string get_model_file_name(){ return model_file_name;}
    Greeting* grt; 
    ~WorkManager() {}; 
private:
    char* worker_name; 
    char** args_tt; 
    char** worker_args; 
    map<int, string> worker_args_1; 
    int world_size; 
    int n_new_born; 
    double t_stop; 
    int spawn_error_codes[10]; 
    void merge_childs(int); 
    void get_t_stop(int, char**); 
    bool terminate_work(); 
    void fprint_statistics(); 
    void fprint_all_obs_data(); 
    void fprint_param_info(); 
    int n_stop; 
    int n_step; 
    map<string, vector<double> > scan_param_list; 
    string model_file_name; 
}; 

class ReadInput{
public: 
    ReadInput(): cue(0), rankval(0), pidval(0), grpid(0), spcid(0), conval(0), timeval(0), push(false), 
        group(false), species(false), conc(false), time(false) { }  
    int cue; 
    int rankval; 
    int pidval; 
    int grpid;
    int spcid; 
    int conval; 
    double timeval;
    void read(char* msg); 
    ~ReadInput() { } 
private: 
    bool push, pid, rank, group, species, conc, time;
}; 

void perr(const char*, int); 
void io_interface(WorkManager*); 
void split(char*, const char*, vector<char*>& );
void ClearScreen(); 

#endif 
