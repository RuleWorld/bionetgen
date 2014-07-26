#include "core/core.h"
#include "gnuplot-cpp/gnuplot-cpp.h"

using namespace std; 

int main(int argc, char *argv[]) { 
    WorkManager* wmg = new WorkManager(); 
    wmg->get_worker(argc, argv); 
    wmg->start_MPI(); 
    wmg->check_work_manager_size(); 

    for (int i = 0; i < 1; i++){
        wmg->spawn_child_workers(); 
    }
    //wmg->exchange_data(); 
    thread t1(io_interface, wmg);
    wmg->exchange_data(); 
    
    t1.join(); 
    plot_data(wmg);   
    //MPI_Finalize(); 
    return 0; 
}
