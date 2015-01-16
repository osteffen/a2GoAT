#include "fMeson.h"
#include "HistogramFactory.h"
#include "GTreeGoatParticle.h"
#include "combinations.h"

#include "TCanvas.h"

fMesonTest::fMesonTest()
{
    HistogramFactory& hf = *(HistogramFactory::GetInstance());

    h_2GIM = hf.Make1DN("2 #gamma IM","M [MeV]","",100,0,1000);
}

void fMesonTest::Run(const fPhysics &p)
{
    GGoatParticleManager::ParticleList particles;
    particles = p.Goat().Particles();

    GGoatParticleManager::ParticleList gammas;

    for( auto& p : particles ) {
        if(p->ID() == 1) {
            gammas.push_back(p);
        }
    }

    for( Combine2<GGoatParticleManager::ParticleList> c(gammas); !c.done(); c() ) {
        TLorentzVector m = c.first()->Vector() + c.second()->Vector();
        h_2GIM->Fill(m.M());
    }

}

void fMesonTest::Display()
{
    new TCanvas();
    h_2GIM->Draw();
}
