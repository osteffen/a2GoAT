#include "MCOverview.h"
#include "Particle.h"
#include "Track.h"
#include "combinatorics.h"
#include "root_draw.h"
#include "Event.h"
#include "TMath.h"
#include <tuple>

using namespace std;

ant::analysis::MCOverview::MCOverview(): hf("MCOverview")
{
    const HistogramFactory::BinSettings energy_bins(100,0,400);
    const HistogramFactory::BinSettings theta_bins(100,0,180);

    mc_particle_stats.AddHist1D(
                [] (const Particle& p) { return p.Ek();},
                hf.Make1D("MC Energy","E [MeV]","", energy_bins)
    );

    mc_particle_stats.AddHist2D(
                [] (const Particle& p) { return make_tuple(p.Ek(), p.Theta()*TMath::RadToDeg());},
                hf.Make2D(
            "MC Energy",
            "E [MeV]",
            "#theta [#circ]",
            energy_bins,
            theta_bins)
    );
}

ant::analysis::MCOverview::~MCOverview()
{

}

void ant::analysis::MCOverview::ProcessEvent(const ant::Event &event)
{
    const Event::MCParticleList_t& mc_particles = event.MCTrue();

    for( auto& mcp : mc_particles ) {
        mc_particle_stats(*mcp);
    }
}

void ant::analysis::MCOverview::Finish()
{

}

void ant::analysis::MCOverview::ShowResult()
{
    canvas c("MC Overview");
    c << canvas::drawoption("colz");
    for(auto& plot : mc_particle_stats.Plots()) {
        c << *plot;
    }
    c << canvas::cend;
}
