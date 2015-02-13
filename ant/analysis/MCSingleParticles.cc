#include "MCSingleParticles.h"
#include "Particle.h"
#include "Track.h"
#include "combinatorics.h"
#include "root_draw.h"
#include "Event.h"
#include "TMath.h"
#include <tuple>
#include "TVector3.h"
using namespace std;
using namespace ant;

const ant::ParticleTypeDatabase::Type* GetMCType( const analysis::MCSingleParticles::Track_MC_pair& p )
{
    return &(p.second.Type());
}


ant::analysis::MCSingleParticles::MCSingleParticles(const mev_t energy_scale): hf("MCSingleParticles")
{
    const HistogramFactory::BinSettings energy_bins(100,0,energy_scale);
    const HistogramFactory::BinSettings veto_bins(100,0,20);
    const HistogramFactory::BinSettings theta_bins(100,0,180);
    const HistogramFactory::BinSettings theta_diff_bins(100,-20,20);
    const HistogramFactory::BinSettings angle_diff_bins(100,0,20);

    const HistogramFactory::BinSettings ntrack_bins(10,0,10);
    const HistogramFactory::BinSettings RecParticleBins(0, 0, 0);


    auto ptype = MC_track_pair_stats.AddBranchNode<const ParticleTypeDatabase::Type*>(GetMCType);

    for( auto& pt : ParticleTypeDatabase::MCFinalStateTypes() ) {

        auto branch = ptype->AddBranch(pt);

        branch->AddHist2D(
                    [] ( const Track_MC_pair& pair ) { return  make_tuple( pair.first.ClusterEnergy(), pair.first.VetoEnergy()); },
                    hf.Make2D("PID Banana for true " + pt->PrintName(),
                              "Cluster Energy [MeV]",
                              "Veto Energy [MeV]",
                              energy_bins,
                              veto_bins,
                              pt->Name() + "_banana"
                              )
                    );

        branch->AddHist2D(
                    [] ( const Track_MC_pair& pair ) { return  make_tuple( pair.second.Ek(), pair.first.ClusterEnergy()); },
                    hf.Make2D("Energy Reconstruction " + pt->PrintName(),
                              "MC true E_{k} [MeV]",
                              "Cluster Energy [MeV]",
                              energy_bins,
                              energy_bins,
                              pt->Name() + "_energy"
                              )
        );

        branch->AddHist2D(
                    [] ( const Track_MC_pair& pair ) { return  make_tuple( pair.second.Theta()*TMath::RadToDeg(), (pair.first.Theta() - pair.second.Theta())*TMath::RadToDeg()); },
                    hf.Make2D("#theta Difference " + pt->PrintName(),
                              "true #theta [#circ]",
                              "rec #theta [#circ]",
                              theta_bins,
                              theta_diff_bins,
                              pt->Name() + "_theta_diff"
                              )
        );

        branch->AddHist2D(
                    [] ( const Track_MC_pair& pair ) {
                            TVector3 v(1,0,0); v.SetPtThetaPhi(1,pair.first.Theta(), pair.first.Phi());
                            return make_tuple( pair.second.Theta()*TMath::RadToDeg(), v.Angle(pair.second.Vect())*TMath::RadToDeg()); },
                    hf.Make2D("Angle between true/rec " + pt->PrintName(),
                              "true #theta [#circ]",
                              "angle [#circ]",
                              theta_bins,
                              angle_diff_bins,
                              pt->Name() + "_angle_diff"
                              )
        );

        branch->AddHist2D(
                    [] ( const Track_MC_pair& pair ) {
                            return make_tuple( pair.second.Theta()*TMath::RadToDeg(), pair.first.Theta()*TMath::RadToDeg()); },
                    hf.Make2D("rec/true #theta " + pt->PrintName(),
                              "true #theta [#circ]",
                              "angle [#circ]",
                              theta_bins,
                              theta_bins,
                              pt->Name() + "_theta-theta"
                              )
        );

    }

    auto ptype2 = MC_tracklist_pair_stats.AddBranchNode<const ParticleTypeDatabase::Type*>( [] (const MC_tracklist_pair& p) { return &p.second.Type();});
    auto b = Rec_MC_stats.AddBranchNode<const ParticleTypeDatabase::Type*>( [] (const Rec_MC_pair& p) { return &p.second.Type();});

    for( auto& pt : ParticleTypeDatabase::MCFinalStateTypes() ) {

        auto branch = ptype2->AddBranch(pt);

        branch->AddHist1D(
                    [] ( const MC_tracklist_pair& pair ) { return  pair.first.size(); },
                    hf.Make1D("Number of Tracks " + pt->PrintName(),
                              "# tracks",
                              "# events",
                              ntrack_bins,
                              pt->Name() + "_nTracks"
                              )
                    );

        auto branch2 = b->AddBranch(pt);
        branch2->AddHist1D(
                    [] ( const Rec_MC_pair& pair ) { return  pair.first.Type().PrintName().c_str(); },
                    hf.Make1D("Reconstruction of " + pt->PrintName(),
                              "as patricle type",
                              "#",
                              RecParticleBins,
                              pt->Name() + "_rec_as"
                              )
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
    const Event::RecParticleList_t& rec_particles = event.Particles();

    if( mc_particles.size() ==1 ) {

        const MCParticle& mc = *mc_particles.front().get();

        MC_tracklist_pair_stats.Fill( make_pair(tracks, mc));

        for( auto& track : tracks ) {
            MC_track_pair_stats.Fill( make_pair(*track.get(), mc) );
        }

        for( auto& rp : rec_particles ) {
            Rec_MC_stats.Fill( make_pair( *rp.get(), mc));
        }
    }
}

void ant::analysis::MCSingleParticles::Finish()
{

}

void ant::analysis::MCSingleParticles::ShowResult()
{
}
