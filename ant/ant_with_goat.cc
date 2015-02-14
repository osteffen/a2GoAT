#ifndef __CINT__
// straight out of pi0_example.cc
#include "TRint.h"
#include "EventManager.h"
#include <time.h>
#include "AntPhysics.h"
#include "TestPhysics.h"
#include "DeltaPlusPhysics.h"
#include "MCOverview.h"
#include "MCSingleParticles.h"
#include "Basic.h"
using namespace std;

/**
 * @brief the main routine
 * @param argc number of parameters
 * @param argv the parameters as strings
 * @return exit code
 */
int main(int argc, char *argv[])
{
    int fake_argc = 0;
    TRint app("ant_with_goat", &fake_argc, NULL);

    clock_t start, end;
    start = clock();

    // Create instance of analysis class
    ant::EventManager analysis;
   // ant::DebugPhysics debug;
    //ant::ParticleCombinatoricsTest ctest;
    //ant::analysis::DeltaPlusPhysics d;

    //ant::PlotterTest plots;
    //analysis.AddPhysics(&plots);

   // analysis.AddPhysics(&debug);
    //analysis.AddPhysics(&ctest);
    //analysis.AddPhysics(&d);

    ant::analysis::MCOverview mcoverview;
    analysis.AddPhysics(&mcoverview);

    ant::analysis::MCSingleParticles single;
    analysis.AddPhysics(&single);

    ant::analysis::Basic basic;
    analysis.AddPhysics(&basic);

    // Perform basic configuration
    if(!analysis.BaseConfig(argc, argv, "GoAT", "Physics"))
    {
        system("man ./documents/goat.man");
        return 0;
    }

    // Perform full initialisation
    if(!analysis.Init(""))
    {
        cout << "ERROR: Init failed!" << endl;
        return 0;
    }

    // Run over files
    analysis.TraverseFiles();

    analysis.Finish();

    end = clock();
    cout << endl;
    cout << "Time required for execution: "
    << (Double_t)(end-start)/CLOCKS_PER_SEC
    << " seconds." << "\n\n";

    basic.ShowResult();

    app.Run(kTRUE);

    return 0;
}

#endif
