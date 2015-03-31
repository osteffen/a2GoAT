#include "TestAPLCON.h"
#include "Particle.h"
#include "Track.h"
#include "plot/plotter.h"
#include "plot/Histogram.h"
#include "Track.h"
#include "plot/root_draw.h"
#include <string>
#include "utils/combinatorics.h"
#include <vector>
#include <numeric>
#include <functional>
#include <APLCON.hpp>
#include <iomanip>

using namespace std;
using namespace ant;

ant::analysis::TestAPLCON::TestAPLCON(const mev_t energy_scale) :
    fitter("TestAPLCON"), photons(2)
{
    HistogramFactory hf("TestAPLCON");

    const HistogramFactory::BinSettings energy_bins(1000,0,energy_scale);
    const HistogramFactory::BinSettings tagger_bins(2000,0.0,2000);
    const HistogramFactory::BinSettings ntaggerhits_bins(100);
    const HistogramFactory::BinSettings veto_bins(1000,0,10.0);
    const HistogramFactory::BinSettings particle_bins(10,0,10);
    const HistogramFactory::BinSettings particlecount_bins(16,0,16);

    banana = hf.Make2D(
                "PID Bananas",
                "Cluster Energy [MeV]",
                "Veto Energy [MeV]",
                energy_bins,
                veto_bins,
                "pid"
                );

    particles = hf.Make1D(
                "Identified particles",
                "Particle Type",
                "#",
                particle_bins,
                "ParticleTypes"
                );
    tagger = hf.Make1D(
                "Tagger Spectrum",
                "Photon Beam Energy",
                "#",
                tagger_bins,
                "TaggerSpectrum"
                );

    ntagged = hf.Make1D(
                "Tagger Hits",
                "Tagger Hits / event",
                "#",
                ntaggerhits_bins,
                "nTagged"
                );

    cbesum = hf.Make1D(
                "CB Energy Sum",
                "E [MeV]",
                "#",
                energy_bins,
                "esum"
                );


    for( auto& t : ParticleTypeDatabase::DetectableTypes() ) {
        numParticleType[t]= hf.Make1D("Number of "+t->PrintName(),
                                      "number of "+t->PrintName()+"/ event",
                                      "",
                                      particlecount_bins);
    }

    // setup fitter

    fitter.LinkVariable("Beam",    beam.Link(),       vector<double>{1});
    fitter.LinkVariable("Photon1", photons[0].Link(), vector<double>{1});
    fitter.LinkVariable("Photon2", photons[1].Link(), vector<double>{2});
    fitter.LinkVariable("Proton",  proton.Link(),     vector<double>{3});

    auto constraint = [] (
            vector<double> beam,
            vector<double> photon1,
            vector<double> photon2,
            vector<double> proton) -> vector<double>
    {

        const TLorentzVector target(0,0,0, ParticleTypeDatabase::Proton.Mass());
        const TLorentzVector diff =
                FitParticle::Make(beam, ParticleTypeDatabase::Photon.Mass())
                + target
                - FitParticle::Make(photon1, ParticleTypeDatabase::Photon.Mass())
                - FitParticle::Make(photon2, ParticleTypeDatabase::Photon.Mass())
                - FitParticle::Make(proton, ParticleTypeDatabase::Proton.Mass());

        return {diff.X(), diff.Y(), diff.Z(), diff.T()};

    };

    APLCON::PrintFormatting::Width = 13;
    cout.precision(3);

    fitter.AddConstraint("EnergyMomentumBalance",{"Beam", "Photon1", "Photon2", "Proton"}, constraint);
}


void ant::analysis::TestAPLCON::ProcessEvent(const ant::Event &event)
{
    for(auto& track : event.Tracks()) {
        banana->Fill(track->ClusterEnergy(), track->VetoEnergy());
    }

    for(auto& particle : event.Particles()) {
        particles->Fill(particle->Type().PrintName().c_str(), 1);
    }

    ntagged->Fill(event.TaggerHits().size());

    cbesum->Fill(event.Trigger().CBEenergySum());

    for( auto& t : ParticleTypeDatabase::DetectableTypes() ) {
        try {
            numParticleType.at(t)->Fill(event.ParticleType(*t).size());
        } catch (...) {}
    }

    for(const auto& taggerhit : event.TaggerHits()) {
        tagger->Fill(taggerhit->PhotonEnergy());

        size_t foundPhotons = 0;
        for(const refMCParticle & p : event.MCTrue()) {
            //cout << p->Type() << endl;
            if(p->Type() == ParticleTypeDatabase::Proton)
                proton.SetFromVector(*p);
            else if(foundPhotons<2 && p->Type() == ParticleTypeDatabase::Photon) {
                photons[foundPhotons].SetFromVector(*p);
                foundPhotons++;
           }
        }
        if(foundPhotons != 2)
            continue;
        beam.SetFromVector(taggerhit->PhotonBeam());

        cout << fitter.DoFit() << endl;
    }

}

void ant::analysis::TestAPLCON::Finish()
{

}

void ant::analysis::TestAPLCON::ShowResult()
{
    canvas c("TestAPLCON: Overview");
    c << drawoption("colz") << banana << particles << tagger << ntagged << cbesum << endc;

    canvas types("TestAPLCON: Particle Types per Event");
    for( auto& t : numParticleType ) {
        types << t.second;
    }
    types << endc;


}
