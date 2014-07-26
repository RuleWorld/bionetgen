#include "core.h"
#include "../gnuplot-cpp/gnuplot-cpp.h"

void WorkManager::get_worker(int argc, char* argv[]){
    int iarg = 1; 
    worker_name = *(argv + iarg++); 
   // worker_args = (argv + iarg); 
    worker_args = new char*[10000]; 

    for (int i=0; i<11; i++){
       try {
           *(worker_args + i) = new char[MAX_LEN]; 
       }
       catch (exception &e){
           cout << e.what() << endl; 
       }
     }
    

    sprintf(worker_args[0],"%s","-o"); 
    sprintf(worker_args[6],"%s","-g"); 

    double delta_t = 0; 
    double t_end = 0; 
    n_step = 0;  

    for (int i = 1; i < argc; i++){
        if (i == 1){
            worker_name = argv[i]; 
        }
        else if (!strcmp(argv[i],"-m")||!strcmp(argv[i],"-model")){
            worker_args[1] = argv[++i]; 
            worker_args[7] = argv[i]; 
            worker_args[8] = argv[i]; 
            
            model_file_name = argv[i]; 
            set_observable_names(); 
        }        
        else if (!strcmp(argv[i],"-q")||!strcmp(argv[i],"-thread")){
            sprintf(worker_args[2],"%s",argv[i]);  
            worker_args[3] = argv[++i]; 
            grt = new Greeting[atoi(argv[i])]; 
            for (int kk = 0; kk < atoi(argv[i]); kk++){
                (grt + kk)->key = 0; 
            }
        }
        else if (!strcmp(argv[i],"-p")){
            worker_args[4] = argv[i]; 
            worker_args[5] = argv[++i]; 
        }
        else if (!strcmp(argv[i],"-t")){
             t_end = atof(argv[++i]); 
        }
        else if (!strcmp(argv[i],"-n")){
             n_step = atoi(argv[++i]); 
             if (n_step == 1) n_step = 2; // there should be atleast 2 outputs in gdat files, one for initial time and the other for final time 
        }
        else if (!strcmp(argv[i],"-log") || !strcmp(argv[i],"-lin") || !strcmp(argv[i],"-gauss") || !strcmp(argv[i],"-lgauss") ){
            /*bzero(grt->par_name,MAX_LEN); 
            string type = argv[i]; 
            try {
                if (!strcmp(argv[i],"-log"))
                    grt->key = 1; 
                else if (!strcmp(argv[i],"-lin"))
                    grt->key = 2; 
                else
                    perr("Unknown scaling; parameter scan must be specified in log or linear scale",__LINE__); 


                sprintf(grt->par_name,"%s",argv[++i]); 
                grt->par_value_min = atof(argv[++i]); 
                grt->par_value_max = atof(argv[++i]); */

            try {
                 
                int np = atoi(worker_args[3]);  
                string type = argv[i]; 
                string par_name = argv[++i]; 
                double par_value_min = atof(argv[++i]); 
                double par_value_max = atof(argv[++i]); 

                parameter_scan(np, type, par_name, par_value_min, par_value_max ); 
            }
            catch (exception &e){
                perr("Incorrect arguments for parameter scan",__LINE__); 
                cout << e.what() << endl; 
            }
        }
          
        delta_t = t_end/n_step; 
        t_stop = t_end - 2*delta_t; 
        sprintf(worker_args[9],"%g",delta_t); 
        sprintf(worker_args[10],"%d",n_step); 
    }     
     
    
    while(argv[iarg]){
        if (!strcmp(argv[iarg],"-q")){
            n_new_born =  atoi(argv[++iarg]); 
            if (!n_new_born) perr("Number spawned  must be a positive integer",__LINE__); 
            break; 
        }
        iarg++; 
    }
    get_t_stop(argc, argv);     
}

void WorkManager::start_MPI(){
    MPI_Init(0, NULL); 
}

void WorkManager::end_MPI(){
    MPI_Finalize(); 
}

void WorkManager::check_work_manager_size(){
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
    if (world_size != 1) perr("Top heavy management", __LINE__); 
}

void WorkManager::spawn_child_workers(){
    int index = n_worker; 

    MPI_Comm_dup(MPI_COMM_SELF, &worker[index].universe); 
    MPI_Comm_spawn(worker_name, worker_args, n_new_born,  MPI_INFO_NULL, 0, 
            worker[index].universe, &worker[index].world, spawn_error_codes);
    merge_childs(index); 
    n_worker++; 
    n_child += n_new_born; 
    msg.worker_id = index; 
   // sprintf(grt->key_msg,"%s","hello"); 
    // bcast_message(grt, index); 


    for (int i = 0; i < n_worker; i++){
        for (int j = 1; j < worker[i].size; j++){
            cout << "PARAMETER_VALUE " << (grt + j - 1)->par_value << endl; 
            //(grt + j -1)->key = 0; 
            MPI_Send( (grt + j - 1), sizeof(Greeting), MPI_BYTE, j, 100, worker[i].universe);
        }
    }

    for (int i = 0; i < n_worker; i++){
        for (int j = 1; j < worker[i].size; j++){
           Greeting* grtemp = new Greeting(); 
           MPI_Status status; 
           MPI_Recv( grtemp, sizeof(Greeting), MPI_BYTE, j, 100, worker[i].universe, &status);
        }
    }
}

void WorkManager::merge_childs(int index){
    MPI_Intercomm_merge(worker[index].world, low, &worker[index].universe);
    MPI_Comm_size(worker[index].universe, &worker[index].size); 
    cout << "Universe size " << worker[index].size << endl; 
}

void WorkManager::bcast_message(Greeting* grt, int index){
    MPI_Bcast(grt, sizeof(Greeting), MPI_BYTE, 0, worker[index].universe); 
}

void WorkManager::send_message(Msg msg, int indexi, int indexj){
    MPI_Send( &msg, sizeof(Msg), MPI_BYTE, indexj, 1, worker[indexi].universe);
}

void WorkManager::gather_message(int index){
    worker[index].msglist = new Msg[worker[index].size]; 
    MPI_Gather(&msg, sizeof(Msg), MPI_BYTE, worker[index].msglist, sizeof(Msg), MPI_BYTE, 0, worker[index].universe);   
} 

void WorkManager::recv_message(Msg* msglist, int indexi, int indexj){
    MPI_Status status; 
    MPI_Recv( msglist + indexj - 1, sizeof(Msg), MPI_BYTE, indexj, 1, worker[indexi].universe, &status);
}

void WorkManager::get_t_stop(int argc, char** argv){
    if (argc < 2) return;  
    /*int n_stop; 
    double t_delt; 
    try {
        n_stop = atoi(argv[argc - 1]); 
        t_delt = atof(argv[argc - 2]); 
        t_stop = t_delt*n_stop - 2*t_delt; 
        if (!t_stop) perr("End time zero not permitted",__LINE__); 
    }
    catch(exception& e){
        cout << e.what() << endl; 
    }*/
}

bool WorkManager::terminate_work(){
    if (worker[0].msglist->tau >= t_stop){  // stop conditoin satisfied if any worker reaches end of simulation time
    //if (n_stop == n_step - 1){  // stop conditoin satisfied if any worker reaches end of simulation time
        stop_cond = true; 
        bzero(msg.msg1, MAX_LEN); 
        sprintf(msg.msg1, "%s", "die");  
        for (int i = 0; i < n_worker; i++){
            for (int j = 1; j < worker[i].size; j++){
                send_message(msg, i, j);
            }
        }

        bzero(msg.msg1,MAX_LEN); 
        MPI_Status status; 
        for (int i = 0; i < n_worker; i++){
            worker[i].msglist = new Msg[worker[i].size-1]; 
            for (int j = 1; j < worker[i].size; j++){
                MPI_Recv( worker[i].msglist + j - 1, sizeof(Msg), MPI_BYTE, j, 111, worker[i].universe, &status); // tag is 111, final message sent by child processes 
                cout << "TIME   " << (worker[0].msglist + j - 1)->tau << endl; 
            }
            worker[i].data.push_back(worker[i].msglist); 
            cout << "WAIT....." << endl; 
        }
        
        //MPI_Finalize(); 
        if (grt->key == 0)  fprint_statistics(); // print avg and std if not a parameter scan 
        cout << "DONE....." << endl; 
        return true; 
    }
    return false; 
}

void WorkManager::fprint_statistics(){
    char fname_mean[MAX_LEN]; 
    char fname_stdv[MAX_LEN]; 
    
    map <int, vector<double> >  mean; 
    map <int, vector<double> > stdv; 
    double mean_val, stdv_val, sum; 

    for (int i = 0; i < n_worker; i++){
        bzero(fname_mean, MAX_LEN); 
        bzero(fname_stdv, MAX_LEN); 

        if (n_worker == 1){
            sprintf(fname_mean,"%s","avg.gdat"); 
            sprintf(fname_stdv,"%s","std.gdat"); 
        }
        else{
            sprintf(fname_mean,"%s_%d_avg.gdat","worker",i); 
            sprintf(fname_stdv,"%s_%d_std.gdat","worker",i); 
        }

        ofstream mean_output; 
        ofstream stdv_output; 
        
        mean_output.precision(3); 
        stdv_output.precision(3); 

        mean_output.open(fname_mean); 
        stdv_output.open(fname_stdv); 

        mean_output << "Time" << " ";
        stdv_output << "Time" << " "; 

        for (int m = 0; m < (*worker[i].data.begin())->group_len; m++){
            mean_output << observable_names[m] << "    ";
            stdv_output << observable_names[m] << "    "; 
        }

        mean_output << endl; 
        stdv_output << endl; 

        for (vector<Msg* >::const_iterator it = worker[i].data.begin(); it != worker[i].data.end(); it++){
            mean_output << scientific << (*it)-> tau << "    "; 
            stdv_output << scientific << (*it)-> tau << "    "; 
            for (int j = 0; j < (*it)->group_len; j++){
                sum = 0; 
                for (int k = 0; k < worker[i].size - 1; k++){
                    sum += (*it + k)->group_conc[j]; 
                }
                mean_val =  sum/(worker[i].size - 1) ; 
                mean_output << mean_val << "    "; 

                sum = 0; 
                for (int k = 0; k < worker[i].size - 1; k++){
                    sum += ((*it + k)->group_conc[j] - mean_val)*((*it + k)->group_conc[j] - mean_val); 
                }
                stdv_val = sqrt(sum/(worker[i].size - 1)); 
                stdv_output << stdv_val << "    "; 
            }

            mean_output << endl; 
            stdv_output << endl; 
        }

        mean_output.close(); 
        stdv_output.close(); 
    }
}
 
void WorkManager::exchange_data(){
while(1){
    for (int i = 0; i < n_worker; i++){
        for (int j = 1; j < worker[i].size; j++){
            send_message(msg, i, j);
        }
    }

    bzero(msg.msg1,MAX_LEN); 
    for (int i = 0; i < n_worker; i++){
        worker[i].msglist = new Msg[worker[i].size-1]; 
        for (int j = 1; j < worker[i].size; j++){
            recv_message(worker[i].msglist, i, j);
        }
        worker[i].data.push_back(worker[i].msglist); 
    }
    
    n_stop++; 
    if (terminate_work()) {
       return; 
    }
 }

}


void WorkManager::set_observable_names(){ 
        ifstream read_obs_names(get_model_file_name()); 
        string line; 
        int k = 0; 
        if (read_obs_names.is_open()){
            while(getline(read_obs_names,line)){
                if (line.find("end groups") != string::npos){
                     --k;  
                }

                if (k == 1){
                    line.erase(0,line.find_first_not_of(" ")); 
                    //line.erase(line.find_first_not_of(" "),line.find_first_of(" ")); 
                    line.replace(line.find(" "),1,"#"); 
                    line.erase(line.find(" "),string::npos); 
                    line.erase(0,line.find("#")+1); 
                    observable_names.push_back(line); 
                }

                if (line.find("begin groups") != string::npos){
                    ++k;  
                }
            }
        }
      read_obs_names.close(); 
}


void WorkManager::parameter_scan(int n_proc, string type, string param, double val1, double val2) {

map<string, int> discrete_t; 
discrete_t["-log"] = 1; 
discrete_t["-lin"] = 2; 
discrete_t["-gauss"] = 3; 
discrete_t["-lgauss"] = 4; 

double min, max, step, value; 
const gsl_rng_type * T;
gsl_rng * r;

switch(discrete_t[type]){
case 1: 
    //if (grt->par_value_min <= 0 || grt->par_value_max <= 0)
        //perr("Logarithmic conversion failed. Parameter value must be greater than zero",__LINE__); 

    try {
        min = log10(val1); 
        max = log10(val2); 
    } catch (exception& e) {
          cout << e.what() << endl; 
    }

    step = (max - min)/(n_proc - 1);  
    for (int i = 0; i < n_proc; i++) {
        value = pow(10,(min + i*step)); 
        scan_param_list[param].push_back(value); 
    
        bzero((grt + i)->par_name, MAX_LEN); 
        sprintf((grt + i)->par_name, param.c_str()); 
        (grt + i)->par_value = value; 
        (grt + i)->key = discrete_t[type]; 
    }
        
    break; 

case 2: 
    if (grt->par_value_min < 0 || grt->par_value_max < 0)
        perr("Negative parameter value not permitted",__LINE__); 


    min = val1; 
    max = val2; 
    step = (max - min)/(n_proc - 1);  

    for (int i = 0; i < n_proc; i++) {
        value = min + i*step; 
        scan_param_list[param].push_back(value); 
  
        bzero((grt + i)->par_name, MAX_LEN); 
        sprintf((grt + i)->par_name, param.c_str()); 
        (grt + i)->par_value = value; 
        (grt + i)->key = discrete_t[type]; 
    }
        
    break; 

case 3: 
    gsl_rng_env_setup();

    T = gsl_rng_default; 
    r = gsl_rng_alloc (T);

    for (int i = 0; i < n_proc; i++) {
        value = val1 + val2*gsl_ran_ugaussian(r); 
        if (value < 0) {
           cout << "Negative parameter value detected; forcing to take zero instead" << endl; 
           value = 0; 
        }
            
        bzero((grt + i)->par_name, MAX_LEN); 
        sprintf((grt + i)->par_name, param.c_str()); 
        (grt + i)->par_value = value; 
        (grt + i)->key = discrete_t[type]; 
    }

    gsl_rng_free (r);
    break; 

case 4: 
    gsl_rng_env_setup();

    T = gsl_rng_default; 
    r = gsl_rng_alloc (T);

    for (int i = 0; i < n_proc; i++) {
        // Assuming both mean (\mu) and standard deviation (\delta) are provided in log scale 
        // e.g., if intended parameter mean is 10e-7, command line specification of mean must be -7
        // and if intended std in log scale is 3, command line specification of std must be 3 
        
        value = val1 + val2*gsl_ran_ugaussian(r); 
            
        bzero((grt + i)->par_name, MAX_LEN); 
        sprintf((grt + i)->par_name, param.c_str()); 
        (grt + i)->par_value = pow(10,value); 
        (grt + i)->key = discrete_t[type]; 
    }

    gsl_rng_free (r);
    break; 
}

}

void WorkManager::print_output(char* input){
    ReadInput* imsg = new ReadInput; 
    imsg->read(input); 
    vector<Msg*>::const_iterator it; 
    switch(imsg->cue){
        case 1: 
            for (int i = 0; i < n_worker; i++){
                for (int j = 1; j < worker[i].size; j++){
                    it  = worker[i].data.end()-1; //latest data for all processes in worker i; 
                    cout << "pid " << (*it + j - 1)->pid << "  rank "<< (*it + j - 1)->rank << endl; 
                }
            }
            break;
        case 2: 
            for (int i = 0; i < n_worker; i++){
                for (int j = 1; j < worker[i].size; j++){
                    it  = worker[i].data.end()-1; //latest data for all processes in worker i; 
                    cout << "rank "<< (*it + j - 1)->rank << "  pid " << (*it + j - 1)->pid << endl; 
                }
            }
            break;
        case 3: 
            for (int i = 0; i < n_worker; i++){
                for (int j = 1; j < worker[i].size; j++){
                    cout << "rank " << j-1 << "    "; 
                    it  = worker[i].data.end()-1; //latest data for all processes in worker i; 
                    cout <<  "time " << (*it + j - 1)->tau << "     "; 
                    for (int k = 0; k < (*it + j - 1)->group_len; k++){
                       cout << "   " << (*it + j - 1)->group_conc[k]; 
                    } 
                    cout << endl; 
                }
            }
           // plot_data(this);  
            break;
        case 4: 
            for (int i = 0; i < n_worker; i++){
                for (int j = 1; j < worker[i].size; j++){
                    it  = worker[i].data.end()-1; //latest data for all processes in worker i; 
                    cout << "rank " << j - 1 << "  ";
                    cout << " pid " << (*it + j - 1)->pid << "  ";
                    cout << " time " << (*it + j - 1)->tau;
                    cout  << endl; 
                }
            }
            break;
        case 5: 
            for (int i = 0; i < n_worker; i++){
                it  = worker[i].data.end()-1; //latest data for all processes in worker i; 
                cout << "pid " << (*it + imsg->rankval)->pid << "    "; 
                cout <<  "time " << (*it + imsg->rankval)->tau << "     "; 
                for (int k = 0; k < (*it + imsg->rankval)->group_len; k++){
                   cout << "   " << (*it + imsg->rankval)->group_conc[k]; 
                } 
                cout << endl; 
            }
            break;
        case 6: 
            for (int i = 0; i < n_worker; i++){
                it  = worker[i].data.end()-1; //latest data for all processes in worker i; 
                for (int j = 1; j < worker[i].size; j++){
                    cout << "rank " << j - 1 << "    "; 
                    cout << " pid " << (*it + j - 1)->pid << "  ";
                    cout <<  "time " << (*it + j - 1)->tau << "     "; 
                    cout << "   " << (*it + j - 1)->group_conc[imsg->grpid-1] << "    " << "group id " << imsg->grpid; 
                    cout << endl; 
                }
            }
            break;
        case 7: 
            for (int i = 0; i < n_worker; i++){
                it  = worker[i].data.end()-1; //latest data for all processes in worker i; 
                cout << "rank "<< (*it)->rank << "     "; 
                cout << "pid " << (*it + imsg->rankval)->pid << "    "; 
                cout <<  "time " << (*it + imsg->rankval)->tau << "     "; 
                cout << "   " << (*it + imsg->rankval)->group_conc[imsg->grpid-1]; 
                cout << endl; 
            }
            break;
    }
    delete (imsg);          
}    

void split(char* input, const char* delim, vector<char*>& tlist){
    char* token;
    tlist.clear(); 
    token = strtok(input, delim); 
    while(token){
        tlist.push_back(token); 
        token = strtok(NULL, delim); 
    }
}

void ClearScreen(){
    cout << string(10,'\n');
}

void io_interface(WorkManager* wmg){
    sleep(5); 
    ClearScreen();  
    cout << "******************************************** Work Manager *******************************************************" << endl;
    cout << "                                     " << wmg->n_worker << " worker groups; " << wmg->n_child << " processes" << "                             " << endl; 
    cout << "*****************************************************************************************************************" << endl; 
    string input; 
    cout << ">>" ; 
    while(getline(cin, input)){
        char buf[MAX_LEN]; 
        bzero(buf, MAX_LEN); 
        try{
            memcpy(buf, input.data(), MAX_LEN); 
        }
        catch(exception& e){
            cout << e.what() << endl; 
        }
        wmg->print_output(buf); 

        if (!strcmp(input.c_str(),"close")) break;
        bzero(wmg->msg.msg1,MAX_LEN);
        sprintf(wmg->msg.msg1,"%s",input.c_str());
     
        vector<Msg*>::const_iterator it; 

        cout << ">>"; 
        if (wmg->stop_cond) return; // if simulation is over, exit this thread;  
    }
}

void perr(const char* errmsg, int line_no){
    char errbuf[5*MAX_LEN]; 
    bzero(errbuf,5*MAX_LEN); 
    sprintf(errbuf, "%s:%d: error: %s", __FILE__, line_no, errmsg); 
    cout << errbuf << endl; 
    exit(1); 
}

void ReadInput::read(char* input){
    vector<char*> tlist, slist;
    split(input, " ", tlist);
    for (vector<char*>::const_iterator it = tlist.begin(); it != tlist.end(); it++){
        split(*it,"--",slist);
        for (vector<char*>::size_type st = 0; st != slist.size(); st++){
             if ((st == 0) && !strcmp(slist[st],"rank")){
                 rank = true; 
             }
             if ((st == 0) && !strcmp(slist[st],"pid")){
                 pid = true;
             }
             if ((st == 0) && !strcmp(slist[st],"conc")){
                 conc = true;
             }
             if ((st == 0) && !strcmp(slist[st],"time")){
                 time = true;
             }
             if ((st == 0) && !strcmp(slist[st],"push")){
                 push = true;
             }
             if ((st == 0) && !strcmp(slist[st],"species")){
                 species = true; 
             }
             if ((st == 1) && !strcmp(slist[0],"group")){
                 group = true;              
             }
             if ((st == 1) && !strcmp(slist[0],"rank")){
                 rankval = atoi(slist[st]);
             }
             if ((st == 1) && !strcmp(slist[0],"pid")){
                 pidval = atoi(slist[st]);
             }
             if ((st == 1) && !strcmp(slist[0],"conc")){
                 rankval = atoi(slist[st]);
             }
             if ((st == 1) && !strcmp(slist[0],"group")){
                 grpid = atoi(slist[st]);
             }
             if ((st == 1) && !strcmp(slist[0],"push")){
                 conval = atoi(slist[st]);
             }
             if ((st == 1) && !strcmp(slist[0],"species")){
                 spcid = atoi(slist[st]);
             }
        }
    }
   
    if (rank) cue = 1; 
    if (pid) cue = 2;
    if (conc) cue = 3;
    if (time) cue = 4;
    if (rank && rankval) cue = 5;
    if (group && grpid) cue = 6; 
    if (rank && rankval && group && grpid) cue = 7; 
    if (push && conval && species && spcid) cue = 8; 
    if (push && conval && species & spcid && rank && rankval) cue = 9; 
}
