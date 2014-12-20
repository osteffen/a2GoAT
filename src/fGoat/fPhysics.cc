#include "fPhysics.h"

#include "TCanvas.h"
#include "TH1D.h"
#include "PParticle.h"

Bool_t fPhysics::Start()
{
    if(!IsGoATFile())
    {
        cout << "ERROR: Input File is not a GoAT file." << endl;
        return kFALSE;
    }
    SetAsPhysicsFile();

    //TraverseValidEvents();
    TraverseEntries(0,1000);

    return kTRUE;
}

void fPhysics::ProcessEvent()
{
    PhysicsList::iterator p;
    for( p = physics.begin(); p!=physics.end(); ++p)
        (*p)->Run(*this);
}

void fPhysics::ProcessScalerRead()
{

}

Bool_t fPhysics::Write()
{

}

fPhysics::fPhysics()
{
}

fPhysics::~fPhysics()
{
    PhysicsList::iterator p;
    for( p = physics.begin(); p!=physics.end(); ++p) {
        delete (*p);
    }
}

Bool_t fPhysics::Init(const char *configfile)
{
    physics.push_back(new fGeantTestPlot);
    physics.push_back(new fPlutoTestPlot);
    return true;
}

void fPhysics::Display()
{
    PhysicsList::iterator p;
    for( p = physics.begin(); p!=physics.end(); ++p)
        (*p)->Display();
}

fGeantTestPlot::fGeantTestPlot()
{
    hCBEsum = new TH1D("hCBESum","CB ESum",100,0,1);
}

void fGeantTestPlot::Run(const fPhysics &p)
{
    hCBEsum->Fill(p.Geant().GetCBESum());
}

void fGeantTestPlot::Display()
{
    new TCanvas("cGeantTestPlot","Geant Test Plot");
    hCBEsum->Draw();
}


fPlutoTestPlot::fPlutoTestPlot():
    hPIDs(new TH1D("hPIDs","PIDs",15,0,15))
{}

void fPlutoTestPlot::Run(const fPhysics &p)
{
    GTreePluto::ParticleList particles = p.Pluto().GetFinalState();

    for( GTreePluto::ParticleList::const_iterator p= particles.begin(); p != particles.end(); ++p) {
        hPIDs->Fill((*p)->ID());
    }
}

void fPlutoTestPlot::Display()
{
    new TCanvas("cPIDs","Pluto Test Plot");
    hPIDs->Draw();
}
