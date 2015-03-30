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

using namespace std;
using namespace ant;

ant::analysis::TestAPLCON::TestAPLCON(const mev_t energy_scale)
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


    const int max_photons_event = 10;

    for( int photons_per_event=2; photons_per_event <= max_photons_event; ++ photons_per_event) {
        std::map<int, TH1D*>& im_list = nGammaImEvent[photons_per_event];
        for( int photons_per_IM=2; photons_per_IM <= photons_per_event; ++photons_per_IM) {
            im_list[photons_per_IM] = hf.Make1D( to_string(photons_per_IM) + " #gamma IM in " + to_string(photons_per_event) + " #gamma events",
                                                 "M [MeV]",
                                                 "#",
                                                 energy_bins,
                                                 to_string(photons_per_IM)+"_photon_IM_"+to_string(photons_per_event)+"gevnts");
        }
    }

    for( auto& t : ParticleTypeDatabase::DetectableTypes() ) {
        numParticleType[t]= hf.Make1D("Number of "+t->PrintName(),"number of "+t->PrintName()+"/ event","",particlecount_bins);
    }

}


void ant::analysis::TestAPLCON::ProcessEvent(const ant::Event &event)
{
    for(auto& track : event.Tracks()) {
        banana->Fill(track->ClusterEnergy(), track->VetoEnergy());
    }

    for(auto& particle : event.Particles()) {
        particles->Fill(particle->Type().PrintName().c_str(), 1);
    }

    const refRecParticleList_t gammas = event.ParticleType(ParticleTypeDatabase::Photon);

    auto entry = nGammaImEvent.find(gammas.size());

    if( entry != nGammaImEvent.end() ) {

        std::map<int, TH1D*>& im_list = entry->second;

        for( auto& im_hist_entry : im_list ) {

            for( auto c = makeCombination(gammas, im_hist_entry.first); !c.Done(); ++c) {
                TLorentzVector m;
                for( auto& g : c) {
                    m+= *g;
                }
                im_hist_entry.second->Fill(m.M());
            };
        }
    }

    for( auto& taggerhit : event.TaggerHits()) {
        tagger->Fill(taggerhit->PhotonEnergy());
    }

    ntagged->Fill(event.TaggerHits().size());

    cbesum->Fill(event.Trigger().CBEenergySum());

    for( auto& t : ParticleTypeDatabase::DetectableTypes() ) {
        try {
            numParticleType.at(t)->Fill(event.ParticleType(*t).size());
        } catch (...) {}
    }

}

void ant::analysis::TestAPLCON::Finish()
{

}

void ant::analysis::TestAPLCON::ShowResult()
{
    canvas c("TestAPLCON");
    c << drawoption("colz") << banana << particles << tagger << ntagged << cbesum << endc;


    for( auto& list : nGammaImEvent) {
        canvas c("TestAPLCON - Inv. Masses " + to_string(list.first)+" #gamma events");
        for( auto& hist : list.second ) {
            c << hist.second;
        }
        c << endc;
    }

    canvas types("TestAPLCON: Particle Types per Event");
    for( auto& t : numParticleType ) {
        types << t.second;
    }
    types << endc;


}
