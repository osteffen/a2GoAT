#include "Basic.h"
#include "Particle.h"
#include "Track.h"
#include "plotter.h"
#include "Histogram.h"
#include "Track.h"
#include "root_draw.h"
#include <string>
#include "combinatorics.h"
#include <vector>
#include <numeric>
#include <functional>

using namespace std;
using namespace ant;

ant::analysis::Basic::Basic(const mev_t energy_scale)
{
    HistogramFactory hf("Basic");

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

    const int max_gammas_im=10;
    ngammaim.reserve(max_gammas_im);

    for(int i=2;i<=max_gammas_im;++i) {

        ngammaim.push_back(
                    make_pair(
                    hf.Make1D( to_string(i) + " #gamma IM",
                              "M [MeV]",
                              "#",
                              energy_bins,
                               to_string(i)+"_photon_IM")
                        ,i
                    ));
    }

    for( auto& t : ParticleTypeDatabase::DetectableTypes() ) {
        numParticleType[t]= hf.Make1D("Number of "+t->PrintName(),"number of "+t->PrintName()+"/ event","",particlecount_bins);
    }

}


void ant::analysis::Basic::ProcessEvent(const ant::Event &event)
{
    for(auto& track : event.Tracks()) {
        banana->Fill(track->ClusterEnergy(), track->VetoEnergy());
    }

    for(auto& particle : event.Particles()) {
        particles->Fill(particle->Type().PrintName().c_str(), 1);
    }

    const refRecParticleList_t gammas = event.ParticleType(ParticleTypeDatabase::Photon);

    for( auto& implot : ngammaim ) {

        for( auto c = makeCombination(gammas, implot.second); !c.Done(); ++c) {
            TLorentzVector m;
            for( auto& g : c) {
                m+= *g;
            }
            implot.first->Fill(m.M());
        };
    }

    for( auto& taggerhit : event.TaggerHits()) {
        tagger->Fill(taggerhit->PhotonEnergy());
    }

    ntagged->Fill(event.TaggerHits().size());

    for( auto& t : ParticleTypeDatabase::DetectableTypes() ) {
        try {
            numParticleType.at(t)->Fill(event.ParticleType(*t).size());
        } catch (...) {}
    }

}

void ant::analysis::Basic::Finish()
{

}

void ant::analysis::Basic::ShowResult()
{
    canvas c("Basic");
    c  << canvas::drawoption("colz") << banana << particles << tagger << ntagged << canvas::cend;

    canvas ngim("Basic - Inv. Masses");
    for( auto& hist : ngammaim ) {
        ngim << hist.first;
    }
    ngim << canvas::cend;

    canvas types("Basic: Particle Types per Event");
    for( auto& t : numParticleType ) {
        types << t.second;
    }
    types << canvas::cend;


}
