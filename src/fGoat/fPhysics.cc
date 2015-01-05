#include "fPhysics.h"

#include "TCanvas.h"
#include "TH1D.h"
#include "PParticle.h"

#include "fPlutoGeant.h"
#include "fPlutoGoat.h"

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
    GoatParticles.CollectParticles();
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
    GoatParticles.AddTree(photons,   1, 0.0,        false);
    GoatParticles.AddTree(protons,  14, 0.938272,    true);
    GoatParticles.AddTree(chargedPi, 8, 0.13957018,  true);
    GoatParticles.AddTree(electrons, 2, 0.000511,    true);
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
    physics.push_back(new fGeantTestPlot() );
    physics.push_back(new fPlutoTestPlot() );
    physics.push_back(new fPlutoGeant() );
    physics.push_back(new fGoatParticlesTestPlot());
    physics.push_back(new fPlutoGoat());
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


fGoatParticlesTestPlot::fGoatParticlesTestPlot():
        hPIDs(new TH1D("hgPIDs"," Goat PIDs",15,0,15))
{
}

void fGoatParticlesTestPlot::Run(const fPhysics &p)
{
    const GGoatParticleManager::ParticleList& particles = p.Goat().Particles();

    for( auto p = particles.begin(); p != particles.end(); ++p) {
        hPIDs->Fill((*p)->ID());
    }
}


void fGoatParticlesTestPlot::Display()
{
    new TCanvas("cgoatPIDs","Goat Particles Test Plot");
    hPIDs->Draw();
}
