/*# Routine for parallel SSA implementation and interprocess client-server communication 

############ For parallel SSA without client-server, generate the net file and then use the following command:
      mpirun -np 100  /bngpath/bin/run_network -o ./fceri_ji -p ssa -g ./modelname.net ./modelname.net 1 600 
# Results from the 100 runs will be stored in gdat files with unique names (format 'modelname_pid#.gdat'). 

############ For parallel SSA with client-server generate the network and follow the 3 steps: 
  # step 1: start ompi server on a terminal using command: ompi-server --no-daemonize -r, which  will output a unique string similar to as follows:  
      ompi-server --no-daemonize -r +
          460128256.0;tcp://130.49.212.142:44934
  # step 2: In a separate terminal run the server program (server.cpp, code provided in the commented out bloc at the end of this file):
          mpic++ server.cpp -o server 
          mpirun -np 1 --ompi-server "460128256.0;tcp://130.49.212.142:44934" server 
  # step 3: In a third terminal run SSA (client): 
          mpirun -np 100 --ompi-server "460128256.0;tcp://130.49.212.142:44934" /bngpath/bin/run_network -o ./fceri_ji -p ssa -g ./modelname.net ./modelname.net 1 600

# Step 3 will establish a connection between the simulations and the server program. An interface will be displaced on the server terminal prompting for user input. 
# current inputs: pid (displays ranks and pids of all parallel runs in a table), conc (displays ranks, pids and corresponding concentrations of all observables in a 
# table), time (displays ranks, pids and SSA times), close (terminates all parallel runs). */

#ifndef CLIENT_H_
#define CLIENT_H_
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
#include "mpi.h"

#define MAX_DAT 1000
#define MAX_LEN 100
#define MAX_SIZE 1000
#define WCOUNT 10
#define DIE 0 

using namespace std; 

typedef struct{
   char msg0[MAX_LEN]; 
   char msg1[MAX_LEN];
   char msg2[MAX_LEN];
   int count; 
   int rank; 
   int pid; 
   double tau; 
   int conc[MAX_DAT]; 
} Msg; 

Msg msglist[MAX_SIZE]; 
Msg msg; 

static int cond1 = 1; 
static int cond2 = 1; 
static int ismpi = 1; 
int cbuf[MAX_LEN];
int rank_of_interest; 
int tag, rank; 
int key[10]; 
map<string, int> comkey; 

MPI_Status status;
MPI_Comm server;
MPI_Request *request; 
 
int tokenize(char*, char**); 
Msg nonroot_response(int, vector<int>&, double, int);

int client( int argc, char **argv, Group* group, Elt_array* earray, double tau, int* size )
{ 
if (!ismpi) return *size; // if not a mpi parallel run, get out of here and do normal SSA  

vector<int> v; // stores group/observable concentrations  
v.clear(); 
for (Group* g = group; g != NULL; g = g->next){
     g->total_val = 0;
     for (int i = 0; i < g->n_elt; i++){
	 g->total_val += g->elt_factor[i]
					* earray->elt[g->elt_index[i] - 1]->val;
     }
     v.push_back(g->total_val); 
}

/****************** MPI initiation**********************************************************************************************************/
if (cond1){
    MPI_Init( 0, NULL );
    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    try{
        MPI_Comm_size(MPI_COMM_WORLD, size); 
        if (*size > MAX_SIZE)
            throw ("Client size exceeded maximum limit "); 

        if (*size == 1){ // if number of process is 1, it is not a mpi parallel run 
            ismpi = 0; 
            MPI_Finalize();
            return *size; 
           }
    }
    catch (const char* e){
        cerr << e << MAX_SIZE << endl; 
        exit(1); 
    }
    
    cond1 = 0; 
    
    comkey["pid"] = 1; comkey["conc"] = 2; comkey["time"] = 3; comkey["num"] = 4; // keys to interpret server commands  
    return *size; /* One-time call; just initiate connection and get out without doing any work" */
 }

/**************** Connection initiation betweem the root node and the server ***************************************************************************/
if (cond2){
    char port_name[MPI_MAX_PORT_NAME];
    char port[MPI_MAX_PORT_NAME];
    sprintf(port,"bngTCPIP",MPI_MAX_PORT_NAME); 
    if (MPI_Lookup_name(port, MPI_INFO_NULL, port_name) != MPI_SUCCESS) { // check if the parallel implementation is for client-server application (false) or not (true)
        ismpi = 0; 
        MPI_Finalize(); 
        return *size; // If if not for client-server application, ignore the rest of the code and return from here   
    }
 
    MPI_Comm_connect( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &server ); // Request server connection 

    char greeting[MAX_LEN];
    strcpy(greeting, ""); 
    MPI_Send( greeting, MAX_LEN, MPI_CHAR, 0, 1, server ); // first handshake with server 
    cond2 = 0; 
 }

/**************** Network communication and data exchange **************************************************************************************************/
    int flag = 0; 
    int tag = 2;   

if (rank == 0){ // master node has rank zero; manages worker nodes having rank 1 and higher    
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, server, &flag, &status); // listen to any new request from server 

    if (flag) {
        MPI_Recv( &msg, sizeof(Msg), MPI_BYTE, 0, MPI_ANY_TAG, server, &status ); // if a new request from server (probe returns true), receive the request
        tag = status.MPI_TAG; 
    }

    switch(tag){
        case 0: // terminate 
             cout << "Received server request for termination..." << endl; 
             MPI_Comm_disconnect( &server ); // disconnect from server
             for (int i = 1; i < *size; i++){ // send kill message (tag = 0) to all workers 
                 MPI_Send(cbuf, 1, MPI_INT, i, DIE, MPI_COMM_WORLD); 
             }    
             MPI_Finalize();
             exit(0); // kill self (master) 

        case 1: // do the job (information exchange with server and workers)  
             char* token; 
             char* tlist[WCOUNT]; 

             for (int i=0; i < WCOUNT; i++){
                  key[i] = 0; 
             } 

             int id;
             id = 0; 
             id = tokenize(msg.msg1, tlist); 
             rank_of_interest = 0; 

             for (int i = 0; i < id; i++){
                 if (comkey[tlist[i]] > 0) {
                     key[comkey[tlist[i]]-1] = 1; 
                 }
                 else {
                     try {
                         rank_of_interest = atoi(tlist[i]);
                         if (!rank_of_interest) throw ("Unrecognized conmand\n"); 
                     }
                     catch (const char* e){
                         cout << e << endl; 
                     }
                 }
             }     

             int c; c = 0;  

             if ((id  == 1) && key[0]){
                 c = 1;   // pid  
             }
             else if ((id == 1) && key[1]){
                 c = 2;  // conc 
             }
             else if ((id == 1) && key[2]){
                 c = 3;  // time 
             } 
             else if ((id == 3) && key[0] && key[1] && rank_of_interest){  // this part has implementatin issue; yet to be fixed 
                 c = 4;  // pid--xxx--conc 
             }
             else {
                 c = 5; 
             } 

             for (int i = 1; i < *size; i++){ // send data request to all workers   
                 MPI_Send(&c, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
             }    

             memset(msglist, 0, MAX_SIZE*sizeof(Msg)); 
             for (int i = 1; i < *size; i++){
                 MPI_Recv((msglist+i), sizeof(Msg), MPI_BYTE, i, tag, MPI_COMM_WORLD, &status); // collect data sent by the workers  
             }

             MPI_Send( msglist, (*size)*sizeof(Msg), MPI_BYTE, 0, c, server ); // send collected data to the server 
             printf ("%s\n",msg.msg1); /* print request received from the server */
             *msg.msg1 = '\0';     
             break; 

        case 2: 
             break; 
        }
 }

if (rank != 0 ){
    int sflag;
    int c; 
    MPI_Status sstatus; 

    MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &sflag, &sstatus);
    if (sflag){
         MPI_Recv(&c, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
         if (!status.MPI_TAG){
             MPI_Finalize(); 
             cout << "Exiting process at node: " << rank << endl; 
             exit(0); //  die after kill message from the master  
         }
         Msg newmsg = nonroot_response(c, v, tau, rank_of_interest);  
         MPI_Send(&newmsg, sizeof(Msg), MPI_BYTE, 0, 1, MPI_COMM_WORLD); 
    }
  }
    
    return *size;
} 

Msg nonroot_response(int c, vector<int>& v, double tau, int rank_of_interest){
    Msg newmsg; 
    bzero(newmsg.msg0, MAX_LEN);
    bzero(newmsg.msg1, MAX_LEN);
    bzero(newmsg.msg2, MAX_LEN);  
    newmsg.count = 0; 
    newmsg.rank = 0; 
    newmsg.pid = 0; 
    bzero(newmsg.conc, MAX_DAT); 

    switch(c){

        case 1:
            newmsg.rank = rank; 
            newmsg.pid = getpid();
            break; 

        case 2:
            for (vector<int>::size_type i = 0; i < v.size(); i++){
                newmsg.count++ ; 
                newmsg.conc[i] = v[i]; 
            } 

        case 3:
            newmsg.rank = rank; 
            newmsg.pid = getpid();
            newmsg.tau = tau; 

        case 4: 
            if (rank_of_interest == rank){
                for (vector<int>::size_type i = 0; i < v.size(); i++){
                    newmsg.count++ ; 
                    newmsg.conc[i] = v[i]; 
                } 
            }    

        case 0: 
            break; 
   }
   return newmsg; 
   
}

int tokenize(char* command, char** tlist){
    char* token;
    int id; 
    id = 0; 
    token = strtok(command, " "); 
    while(token){
        tlist[id] = token; 
        token = strtok(NULL, " "); 
        id++; 
    }
    return id; 
}
         
#endif


/*
//server.cpp: Should be used as a separate program for MPI inter-process communication. Creates a common interface for all parallel simulations 

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <error.h>
#include <errno.h>
#include "mpi.h"

#define MAX_LEN 100
#define MAX_SIZE 1000 
#define MAX_DAT 1000
#define FATAL 1

using namespace std; 

typedef struct{
   char msg0[MAX_LEN]; 
   char msg1[MAX_LEN];
   char msg2[MAX_LEN];
   int count; 
   int rank; 
   int pid; 
   double tau; 
   int conc[MAX_DAT]; 
} Msg; 

typedef struct{
     char msg[MAX_LEN];
     int pid;  
     bool time;
     bool conc; 
     } Cmd;  

Msg msglist[MAX_SIZE]; 
Msg msg; 
Cmd cmd; 
int size, remote_size;

void print_output(int); 

int main( int argc, char **argv )
{

int buf[MAX_LEN];
int tag;  
char port[MPI_MAX_PORT_NAME];
char pport[MPI_MAX_PORT_NAME]; 
sprintf(pport,"bngTCPIP", MPI_MAX_PORT_NAME); 
MPI_Comm client;
MPI_Status status;

MPI_Init( &argc, &argv );
MPI_Comm_size(MPI_COMM_WORLD, &size);
if (size != 1) error(FATAL, errno, "Server cannot be bigger than 1");

//#************* Open port and establish connection with client **************************************************************#/ 
MPI_Info info; 
MPI_Info_create(&info);
char key[MPI_MAX_INFO_KEY];
char val[MPI_MAX_INFO_VAL];

sprintf(key,"ompi_global_scope");
sprintf(val,"true"); 
MPI_Info_set(info, key, val); 

MPI_Open_port(MPI_INFO_NULL, port);
MPI_Publish_name(pport, info, port); 

MPI_Comm_accept( port, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &client ); // A blocking call, released once client connects 
MPI_Recv( buf, MAX_LEN, MPI_CHAR, 0, 1, client, &status ); // first handshake wih the client after connection 
MPI_Comm_remote_size(client, &remote_size); 
cout << "*******************************************" << endl; 
cout << "Client " << buf <<": 1 manager, " << remote_size - 1 << " workers" <<  endl; 
cout << "*******************************************" << endl; 
//#*************Connection established***************************************************************************************#/ 

cout << ">> "; 
tag = status.MPI_TAG; 
string input; 

//#************Input/Output bloc *******************************************************************************************#/
while (getline(cin,input))
{
    try{
        size_t len = input.size(); 
        if (len > MAX_LEN)
            throw ("Input command exceeds approved maximum length"); 
    }
    catch (exception &e){
        cerr << e.what() << endl; 
    }

    bzero(msg.msg1, MAX_LEN);
    memcpy( msg.msg1, input.data(), MAX_LEN);
 
    if (input == "close") 
        tag = 0; 
    else 
        tag = 1;

    
    input.clear(); 

    switch(tag){

        case 0: // if tag is 0, time to disconnect 
            MPI_Send( &msg, sizeof(Msg), MPI_BYTE, 0, tag, client); // send signal to client for termination (tag = m = 0) 
            cout << "Termination signal received: closing connection with cient" << endl; 
            goto stop; 

        case 100:   // Not implemented yet 
            MPI_Abort( MPI_COMM_WORLD, 1 );

        default : // Do work  
            MPI_Send( &msg, sizeof(Msg), MPI_BYTE, 0, tag, client);
            MPI_Recv( msglist, remote_size*sizeof(Msg), MPI_BYTE, 0, MPI_ANY_TAG, client, &status ); // status == 1 for work do be done; status = 0 for termination 
            int c = status.MPI_TAG; 
            print_output(c); 
            memset(msglist, 0, MAX_SIZE*sizeof(Msg)); 
            cout << ">> "; 
            break; 
    }
 }
//#*************End of Input/Output***************************************************************************************#/ 
   stop: 
   MPI_Comm_disconnect(&client); 
   MPI_Close_port(port); 
   MPI_Info_free(&info); 
   MPI_Finalize();
   return 0;
}

void print_output(int c){
    switch (c){
        case 1: 
            for (int i = 1; i< remote_size; i++){
                cout << "rank: " << msglist[i].rank << " " << "pid: " << msglist[i].pid <<" " <<  endl; 
            }
            break;     

        case 2: 

            cout << "Nodes";  
            for (int i = 1; i < msglist[1].count + 1; i++){
                cout << "      " << "Group" << i; 
            }

            for (int i = 1; i < remote_size; i++){
                cout << "" << endl; 
                cout << "Node " << i; 
                for (int j = 0; j < msglist[i].count; j++){
                    cout << "         " <<  msglist[i].conc[j]; 
                }
            }
            cout << "" << endl; 
            break;     

        case 3: 
            for (int i = 1; i< remote_size; i++){
                cout << "rank: " << msglist[i].rank << " " << "pid: " << msglist[i].pid <<" " <<  "time: " <<  msglist[i].tau << endl; 
            }
            break;     

        case 4: 
            cout << "Nodes";  
            for (int i = 1; i < msglist[1].count + 1; i++){
                cout << "   " << "species" << i; 
            }

            for (int i = 1; i < remote_size; i++){
                if (msglist[i].count > 0){
                    cout << "" << endl; 
                    cout << "Node " << i; 
                    for (int j = 0; j < msglist[i].count; j++){
                        cout << "         " <<  msglist[i].conc[j]; 
                    }
                }
            }
            cout << "" << endl; 
            break;     
    }
}    
*/
