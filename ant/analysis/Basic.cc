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

    const HistogramFactory::BinSettings energy_bins(100,0,energy_scale);
    const HistogramFactory::BinSettings veto_bins(100,0,10.0);
    const HistogramFactory::BinSettings particle_bins(10,0,10);

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

    const int max_gammas_im=8;
    ngammaim.reserve(max_gammas_im);

    for(int i=2;i<=max_gammas_im;++i) {

        ngammaim.push_back(
                    hf.Make1D( to_string(i) + " #gamma IM",
                              "M [MeV]",
                              "#",
                              energy_bins,
                               to_string(i)+"_photon_IM")
                    );
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

    for(int ngammas=2; ngammas <= min((size_t)8, gammas.size()); ++ngammas ) {

        for( auto c = makeCombination(gammas, ngammas); !c.Done(); ++c) {
            TLorentzVector m;
            for( auto& g : c) {
                m+= *g;
            }

            ngammaim.at(ngammas-2)->Fill(m.M());
        };
    }

}

void ant::analysis::Basic::Finish()
{

}

void ant::analysis::Basic::ShowResult()
{
    canvas c("Basic");
    c  << canvas::drawoption("colz") << banana << particles << canvas::cend;

    canvas ngim("Basic - Inv. Masses");
    for( auto& hist : ngammaim ) {
        ngim << hist;
    }
    ngim << canvas::cend;

}
