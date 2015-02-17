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
#include <sstream>
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
        stringstream titles;
        titles << i << ParticleTypeDatabase::Photon.PrintName() << " IM";
        ngammaim.push_back(
                    hf.Make1D( titles.str(),
                              "M [MeV]",
                              "#",
                              energy_bins,
                               "gamma_IM_"+i)
                    );
    }

}


void ant::analysis::Basic::ProcessEvent(const ant::Event &event)
{
    for(auto& track : event.Tracks()) {
        banana->Fill(track.ClusterEnergy(), track.VetoEnergy());
    }

    for(auto& particle : event.Particles()) {
        particles->Fill(particle.Type().PrintName().c_str(), 1);
    }

    vector< reference_wrapper<const Particle> > gammas;
    for( auto& particle : event.Particles() ) {
        if( particle.Type() == ParticleTypeDatabase::Photon )
            gammas.push_back(particle);
    }

    gammas.reserve(10);

    for(int ngammas=2; ngammas <= min((size_t)8, gammas.size()); ++ngammas ) {

        for( auto c = makeCombination(gammas,ngammas); !c.Done(); ++c) {
            TLorentzVector m = accumulate(c.begin(), c.end(), TLorentzVector());
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
