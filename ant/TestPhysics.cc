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

ParticleCombinatoricsTest::ParticleCombinatoricsTest():
    hf("TestPhysics")
{

    const HistogramFactory::BinSettings im_binning(100,0,250);
    const HistogramFactory::BinSettings energy_binning(100,0,250);
    const HistogramFactory::BinSettings npart_binning(10,0,10);

    ggim = hf.Make1D("2 #gamma IM", "M_{#gamma #gamma} [MeV]","#", im_binning);
    gggim = hf.Make1D("3 #gamma im","M_{#gamma #gamma #gamma} [MeV]","#", im_binning);
    nphotons = hf.Make1D("Number of photons", "N", "", npart_binning);
    nprotons = hf.Make1D("Number of protons","N","",npart_binning);

    // Build a map of ParticleType -> Histogram, and fill it
    for( auto& type : ParticleTypeDatabase() ) {
        EHists.insert(
                    std::pair<const ParticleTypeDatabase::Type*, TH1*>(
                        &type,
                        hf.Make1D( type.PrintName()+" Energy", "E [MeV]", "#", energy_binning )
                        )
                    );

    }

}


void ParticleCombinatoricsTest::ProcessEvent(const Event &event)
{
    Event::ParticleList_t photons;
    Event::ParticleList_t protons;

    for( auto& particle : event.Particles() ) {

        // fill the histogram corresponding to the partice type of the current particle
        try {
            EHists.at( &(particle->Type()) )->Fill(particle->E());
        } catch (...) {}

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

    new TCanvas();
    EHists.at( &ParticleTypeDatabase::Photon )->Draw();
}
