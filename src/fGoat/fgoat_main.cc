#ifndef __CINT__

#include "GoAT.h"
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include <time.h>
#include "fPhysics.h"
#include "TRint.h"


using namespace std;



/**
 * @brief the main routine
 * @param argc number of parameters
 * @param argv the parameters as strings
 * @return exit code
 */
int main(int argc, char *argv[])
{
    int a=0;
    TRint app("fGoAT", &a, NULL);
    app.Init();


    clock_t start, end;
    start = clock();

    // Create instance of analysis class
    fPhysics analysis;

    // Perform basic configuration
    if(!analysis.BaseConfig(argc, argv, "Acqu", "GoAT"))
    {
        cerr << "invalid command line" << endl;
        return 0;
    }

    // Perform full initialisation
    if(!analysis.GConfigFile::Init())
    {
        cerr << "ERROR: Init failed!" << endl;
        return 0;
    }

    // Run over files
    analysis.TraverseFiles();

    end = clock();
    cout << endl;
    cout << "Time required for execution: "
    << (double)(end-start)/CLOCKS_PER_SEC
    << " seconds." << "\n\n";


    analysis.Display();

    app.Run();

    return 0;
}



#endif
