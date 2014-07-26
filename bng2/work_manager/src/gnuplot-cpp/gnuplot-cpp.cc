// Example for C++ Interface to Gnuplot

// requirements:
// * gnuplot has to be installed (http://www.gnuplot.info/download.html)
// * for Windows: set Path-Variable for Gnuplot path (e.g. C:/program files/gnuplot/bin)
//             or set Gnuplot path with: Gnuplot::set_GNUPlotPath(const std::string &path);


#include <iostream>
#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot
#include "gnuplot-cpp.h" 

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
 #include <conio.h>   //for getch(), needed in wait_for_key()
 #include <windows.h> //for Sleep()
 void sleep(int i) { Sleep(i*1000); }
#endif


#define SLEEP_LGTH 2  // sleep time in seconds
#define NPOINTS    50 // length of array

void wait_for_key(); // Programm halts until keypress

using std::cout;
using std::endl;

int plot_data(WorkManager* w)
{
    // if path-variable for gnuplot is not set, do it with:
    // Gnuplot::set_GNUPlotPath("C:/program files/gnuplot/bin/");

    // set a special standard terminal for showonscreen (normally not needed),
    //   e.g. Mac users who want to use x11 instead of aqua terminal:
    // Gnuplot::set_terminal_std("x11");

    cout << "*** example of gnuplot control through C++ ***" << endl << endl;

    //
    // Using the GnuplotException class
    //
    try
    {

        vector<Msg*> data; 
        data = (w->worker[0]).data; 

        size_t n_parallel = (w->worker[0]).size - 1;   
        int n_group = data[0]->group_len; 
        Msg* msg; 
        vector<double> x, y, avg;

        for (int i = 0; i < n_group; i++){
            Gnuplot* window = new Gnuplot("lines");
            Gnuplot* window_temp = new Gnuplot("lines"); 

            window->set_title(w->observable_names[i]); 
            window->set_xlabel("time");
            window->set_ylabel("Concentration"); 
            window->set_grid(); 

            window_temp->set_title(w->observable_names[i]); 
            window_temp->set_xlabel("time"); 
            window_temp->set_ylabel("Concentration"); 
            window_temp->set_grid(); 
       
            for (int k = 0; k < n_parallel; k++)
            {
                x.clear(); y.clear(); avg.clear(); 
                for (vector<Msg*> ::const_iterator it = data.begin(); it != data.end(); it++)  // fill double arrays x, y, z
                {
                    msg = (*it + k); 
                    x.push_back((*it + k)->tau);             // x[i] = i
                    y.push_back((*it + k)->group_conc[i]); 
                }
                
                window_temp->plot_xy(x,y,"");
                window->savetops(w->observable_names[i]); 
                window->plot_xy(x,y,"");
                //g.set_smooth("bezier").plot_xy(x,y,"bezier");
            }
           // sleep(1); 
        }

        wait_for_key();

    }
    catch (GnuplotException ge)
    {
        cout << ge.what() << endl;
    }


    //cout << endl << w->model_name + ".xml"  << endl;

    return 0;
}



void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}
