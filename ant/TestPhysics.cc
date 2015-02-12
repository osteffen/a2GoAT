#include "TestPhysics.h"

#include "Event.h"
#include "Particle.h"
#include "ParticleType.h"
#include "combinatorics.h"
#include "TH1D.h"
#include <memory>
#include <iostream>

#include "TCanvas.h"

#include "plotter.h"
#include "root_draw.h"

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
                        hf.Make1D( type.PrintName()+" Energy", "E_{k} [MeV]", "#", energy_binning )
                        )
                    );

    }

}


void ParticleCombinatoricsTest::ProcessEvent(const Event &event)

{
    Event::RecParticleList_t photons;
    Event::RecParticleList_t protons;

    for( auto& particle : event.Particles() ) {

        // fill the histogram corresponding to the partice type of the current particle
        try {
            EHists.at( &(particle->Type()) )->Fill(particle->Ek());
        } catch (...) {}

        if( particle->Type() ==  ParticleTypeDatabase::Photon )
            photons.emplace_back(particle);
        else if ( particle->Type() == ParticleTypeDatabase::Proton )
            protons.emplace_back(particle);
    }

    nphotons->Fill(photons.size());
    nprotons->Fill(protons.size());

    KofNvector< typename Event::sRecParticlePtr > combinations2(photons,2);
    do {
        TLorentzVector v;
        for( auto& i: combinations2 ) {
            v += *i;
        }

        ggim->Fill(v.M());

    } while(combinations2.next());

    KofNvector< typename Event::sRecParticlePtr > combinations3(photons,3);
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
    canvas cc("ParticleCombinatoricsTest");
    cc << ggim << gggim << nphotons << nprotons << canvas::cend;

    canvas cc2("ParticleCombinatoricsTest2");
    for( auto& e : EHists) {
        cc2 << e.second;
    }
    cc2 << canvas::cend;

}


double GetEnergyFromTrack(const ant::Track& p ) {
    return p.ClusterEnergy();
}

double GetThetaFromTrack(const ant::Track& p ) {
    return p.Theta()*TMath::RadToDeg();
}

PlotterTest::PlotterTest():
    hf("PlotterTest")
{
    const HistogramFactory::BinSettings pid_binnning(100,0,25);
    const HistogramFactory::BinSettings energy_binning(100,0,250);
    const HistogramFactory::BinSettings theta_binning(180,0,180);


    track_plots.AddHist1D(
                GetEnergyFromTrack,
                hf.Make1D("Track Energy (2)","E [MeV]","#", energy_binning));

    auto pid_banana_fuction = [] (const Track& p) { return move( make_tuple(p.ClusterEnergy(), p.VetoEnergy()) );};

    track_plots.AddHist2D(
                pid_banana_fuction,
                hf.Make2D("plotter test 2d","x","y",energy_binning,pid_binnning));

    auto list2 = track_plots.AddList();
    list2->AddHist1D(
                GetEnergyFromTrack,
                hf.Make1D("Track Energy (2) new list","E [MeV]","#", energy_binning));

    auto b = list2->AddBranchNode<detector_t>([] (const Track& t) { return t.Apparatus(); });

    auto b_cb = b->AddBranch(detector_t::CB);
    b_cb->AddHist1D(GetEnergyFromTrack, hf.Make1D("CB Theta","#theta [#circ]","",theta_binning));

    auto b_taps = b->AddBranch(detector_t::TAPS);
    b_taps->AddHist1D(GetEnergyFromTrack, hf.Make1D("TAPS Theta","#theta [#circ]","",theta_binning));
}

void PlotterTest::ProcessEvent(const Event &event)
{

    for (auto& track : event.Tracks() ) {
        track_plots.Fill(*track);
    }

    for (auto& particle : event.Particles() ) {
        particle_plots.Fill(*particle);
    }

}

void PlotterTest::Finish()
{

}

void PlotterTest::ShowResult()
{
}
