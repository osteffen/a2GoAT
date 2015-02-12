#include "MCSingleParticles.h"
#include "Particle.h"
#include "Track.h"
#include "combinatorics.h"
#include "root_draw.h"
#include "Event.h"
#include "TMath.h"
#include <tuple>

using namespace std;
using namespace ant;

const ant::ParticleTypeDatabase::Type* GetMCType( const analysis::MCSingleParticles::MC_track_pair& p )
{
    return &(p.second.Type());
}


ant::analysis::MCSingleParticles::MCSingleParticles(): hf("MCSingleParticles")
{
    const HistogramFactory::BinSettings energy_bins(100,0,400);
    const HistogramFactory::BinSettings veto_bins(100,0,20);
    const HistogramFactory::BinSettings theta_bins(100,0,180);

    const HistogramFactory::BinSettings ntrack_bins(10,0,10);


    auto ptype = MC_track_pair_stats.AddBranchNode<const ParticleTypeDatabase::Type*>(GetMCType);

    for( auto& pt : ParticleTypeDatabase::MCFinalStateTypes() ) {

        auto branch = ptype->AddBranch(pt);

        branch->AddHist2D(
                    [] ( const MC_track_pair& pair ) { return  make_tuple( pair.first.ClusterEnergy(), pair.first.VetoEnergy()); },
                    hf.Make2D("PID Banana for true " + pt->PrintName(),
                              "Cluster Energy [MeV]",
                              "Veto Energy [MeV]",
                              energy_bins,
                              veto_bins)
                    );

        branch->AddHist2D(
                    [] ( const MC_track_pair& pair ) { return  make_tuple( pair.second.Ek(), pair.first.ClusterEnergy()); },
                    hf.Make2D("Energy Reconstruction " + pt->PrintName(),
                              "MC true E_{k} [MeV]",
                              "Cluster Energy [MeV]",
                              energy_bins,
                              energy_bins)
        );

    }

    auto ptype2 = MC_tracklist_pair_stats.AddBranchNode<const ParticleTypeDatabase::Type*>( [] (const MC_tracklist_pair& p) { return &p.second.Type();});

    for( auto& pt : ParticleTypeDatabase::MCFinalStateTypes() ) {

        auto branch = ptype2->AddBranch(pt);

        branch->AddHist1D(
                    [] ( const MC_tracklist_pair& pair ) { return  pair.first.size(); },
                    hf.Make1D("Number of Tracks " + pt->PrintName(),
                              "# tracks",
                              "# events",
                              ntrack_bins)
                    );
    }

}

ant::analysis::MCSingleParticles::~MCSingleParticles()
{

}

void ant::analysis::MCSingleParticles::ProcessEvent(const ant::Event &event)
{
    const Event::MCParticleList_t& mc_particles = event.MCTrue();
    const Event::TrackList_t& tracks = event.Tracks();

    if( mc_particles.size() ==1 ) {

        const MCParticle& mc = *mc_particles.front().get();

        MC_tracklist_pair_stats.Fill( make_pair(tracks, mc));

        for( auto& track : tracks ) {
            MC_track_pair_stats.Fill( make_pair(*track.get(), mc) );
        }
    }
}

void ant::analysis::MCSingleParticles::Finish()
{

}

void ant::analysis::MCSingleParticles::ShowResult()
{
}
