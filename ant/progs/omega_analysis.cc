#ifndef __CINT__

#include "TRint.h"

#include "EventManager.h"
#include "AntPhysics.h"

#include "analysis/omega.h"
#include "analysis/omega_bottomup.h"

#include "GoatExceptions.h"
#include "TFile.h"
#include "analysis/GeoAcceptance.h"
#include "analysis/mctrue_acceptance.h"
#include "plot/Histogram.h"
using namespace std;


TFile* OpenAsOutput(const std::string& filename) {
    TFile* file = new TFile(filename.c_str(),"recreate");
    if(!file || !file->IsOpen())
        throw GoatOutputFileError(filename, "Can't open output file for writing: "+filename);

    return file;
}


TRint* LaunchRint(const std::string& name, int argc, char** argv, const int root_options_at) {

    int fake_argc=0;
    char** fake_argv=nullptr;

    if(argc > root_options_at) {
        fake_argc = argc-root_options_at+1;
        fake_argv = new char*[fake_argc];
        fake_argv[0] = argv[0];
        for( int i=root_options_at; i<argc; ++i){
            fake_argv[i-root_options_at+1]=argv[i];
        }
    } else {
        fake_argc = 1;
        fake_argv = new char*[1];
        fake_argv[0] = argv[0];
    }

    cout << "Options to RINT: " << fake_argc << ": ";
    for(int i =0;i<fake_argc; ++i) {
        cout << fake_argv[i] << " ";
    }
    cout << endl;

    TRint* rint = new TRint(name.c_str(), &fake_argc, fake_argv);
    // delete argv;
    return rint;
}

/**
 * @brief the main routine
 * @param argc number of parameters
 * @param argv the parameters as strings
 * @return exit code
 */
int main(int argc, char *argv[])
{
    // argv[0]: self
    // argv[1]: goat config
    // argv[2]: input file
    // argv[3]: output file
    if(argc<4) {
        cerr << "usage: " << argv[0] << " <config> <infile> <outfile>"<<endl;
        exit(1);
    }

    TRint* app = LaunchRint("ant_with_goat", argc, argv, 4);

    // Create instance of analysis class
    ant::EventManager analysis;
    analysis.SetMaxEvents(0);

    TFile* ant_output = OpenAsOutput(argv[3]);

//    ant::analysis::Omega omegaRec("Omega Rec",false);
//    analysis.AddPhysics(&omegaRec);
//    ant::analysis::Omega omegaTrue("Omega True",true);
//    analysis.AddPhysics(&omegaTrue);

//    ant::analysis::Omega2 omega2True("Omega2 True",true);
//    analysis.AddPhysics(&omega2True);

//    ant::analysis::Omega2 omega2Rec("Omega2 Rec",false);
//    analysis.AddPhysics(&omega2Rec);

    ant::analysis::Omega3 omega3Rec("Omega3 Rec",false);
    analysis.AddPhysics(&omega3Rec);

    ant::analysis::Omega3 omega3True("Omega3 True",true);
    analysis.AddPhysics(&omega3True);

    //ant::analysis::OmegaBottomUp omega2;
    //analysis.AddPhysics(&omega2);
    ant::analysis::MCTrueAcceptance mctrueacceptance;
    analysis.AddPhysics(&mctrueacceptance);

    std::vector<char*> gargs;
    std::string f("-f");
    char *gf = new char[f.length() + 1];
    strcpy(gf, f.c_str());

    gargs.push_back(argv[0]);
    gargs.push_back(argv[1]);
    gargs.push_back(gf);
    gargs.push_back(argv[2]);

    // Perform basic configuration
    if(!analysis.BaseConfig(gargs.size(), &(gargs[0]), "GoAT", "Physics"))
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

    //omegaRec.ShowResult();
    //omegaTrue.ShowResult();
    //omega2.ShowResult();
    //mctrueacceptance.ShowResult();
    //omega2True.ShowResult();
    //omega2Rec.ShowResult();

    //omega3Rec.ShowResult();


    ant_output->Write();

    app->ProcessLine("new TBrowser()");

    app->Run(kTRUE);

    delete app;

    return 0;
}

#endif
