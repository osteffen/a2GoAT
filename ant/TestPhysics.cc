#include "TestPhysics.h"

#include "Event.h"
#include "Particle.h"
#include "ParticleType.h"
#include "combinatorics.h"
#include "TH1D.h"
#include <memory>
#include <iostream>

#include "TCanvas.h"

using namespace std;
using namespace ant;

ParticleCombinatoricsTest::ParticleCombinatoricsTest()
{
    ggim = new TH1D("testphysics_ggim","ggim",100,0,250);
    gggim = new TH1D("testphysics_gggim","gggim",100,0,250);
    nphotons = new TH1D("testphysics_nphotons","Number of photons",10,0,10);
    nprotons = new TH1D("testphysics_nprotons","Number of protons",10,0,10);
}


void ParticleCombinatoricsTest::ProcessEvent(const Event &event)
{
    Event::ParticleList_t photons;
    Event::ParticleList_t protons;

    for( auto& particle : event.Particles() ) {
        if( particle->Type() ==  ParticleTypeDatabase::Photon )
            photons.emplace_back(particle);
        else if ( particle->Type() == ParticleTypeDatabase::Proton )
            protons.emplace_back(particle);
    }

    nphotons->Fill(photons.size());
    nprotons->Fill(protons.size());

    KofNvector< typename Event::sParticlePtr > combinations2(photons,2);
    do {
        TLorentzVector v;
        for( auto& i: combinations2 ) {
            v += *i;
        }

        ggim->Fill(v.M());

    } while(combinations2.next());

    KofNvector< typename Event::sParticlePtr > combinations3(photons,3);
    do {
        TLorentzVector v;
        for( auto& i: combinations3 ) {
            v += *i;
        }

        gggim->Fill(v.M());

    } while(combinations3.next());
}

void ParticleCombinatoricsTest::Finish()
{

}

void ParticleCombinatoricsTest::ShowResult()
{
    new TCanvas();
    ggim->Draw();

    new TCanvas();
    gggim->Draw();

    new TCanvas();
    nphotons->Draw();

    new TCanvas();
    nprotons->Draw();
}
