#include "MCSingleParticles.h"
#include "Particle.h"
#include "Track.h"
#include "utils/combinatorics.h"
#include "plot/root_draw.h"
#include "Event.h"
#include "TMath.h"
#include <tuple>
#include "TVector3.h"
#include "Detector.h"
#include <sstream>

using namespace std;
using namespace ant;

const ant::ParticleTypeDatabase::Type* GetMCType( const analysis::MCSingleParticles::Track_MC_pair& p )
{
    return &(p.second.Type());
}

const ant::detector_t GetMCDetecor( const analysis::MCSingleParticles::Track_MC_pair& p )
{
    return (p.second.Theta() > 21*TMath::DegToRad()) ? detector_t::anyCB : detector_t::anyTAPS;
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


    //auto ptype = MC_track_pair_stats.AddBranchNode<const ParticleTypeDatabase::Type*>(GetMCType);
    auto detector = MC_track_pair_stats.AddBranchNode<detector_t>(GetMCDetecor);

    for( auto& d : std::list<detector_t>{detector_t::anyCB, detector_t::anyTAPS}) {

        auto dbranch = detector->AddBranch(d);

        stringstream dname;
        d.Print(dname);

        auto ptype = dbranch->AddBranchNode<const ParticleTypeDatabase::Type*>(GetMCType);


        for( auto& pt : ParticleTypeDatabase::MCFinalStateTypes() ) {

            auto branch = ptype->AddBranch(pt);

            branch->AddHist2D(
                        [] ( const Track_MC_pair& pair ) { return  make_tuple( pair.first.ClusterEnergy(), pair.first.VetoEnergy()); },
            hf.Make2D("PID Banana for true " + pt->PrintName(),
                      "Cluster Energy [MeV]",
                      "Veto Energy [MeV]",
                      energy_bins,
                      veto_bins,
                      pt->Name() + "_banana_" + dname.str()
                      )
            );

            branch->AddHist2D(
                        [] ( const Track_MC_pair& pair ) { return  make_tuple( pair.second.Ek(), pair.first.ClusterEnergy()); },
            hf.Make2D("Energy Reconstruction " + pt->PrintName(),
                      "MC true E_{k} [MeV]",
                      "Cluster Energy [MeV]",
                      energy_bins,
                      energy_bins,
                      pt->Name() + "_energy_" + dname.str()
                      )
            );

            branch->AddHist2D(
                        [] ( const Track_MC_pair& pair ) { return  make_tuple( pair.second.Theta()*TMath::RadToDeg(), (pair.first.Theta() - pair.second.Theta())*TMath::RadToDeg()); },
            hf.Make2D("#theta Difference " + pt->PrintName(),
                      "true #theta [#circ]",
                      "rec #theta [#circ]",
                      theta_bins,
                      theta_diff_bins,
                      pt->Name() + "_theta_diff_" + dname.str()
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
                      pt->Name() + "_angle_diff_" + dname.str()
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
                      pt->Name() + "_theta-theta_" + dname.str()
                      )
            );

        }
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
                    [] ( const Rec_MC_pair& pair ) { return  pair.first->Type().PrintName().c_str(); },
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
    const refMCParticleList_t& mc_particles = event.MCTrue();
    const refTrackList_t& tracks = event.Tracks();
    const refRecParticleList_t& rec_particles = event.Particles();

    if( mc_particles.size() ==1 ) {

        const MCParticle* mc = mc_particles.front();

        MC_tracklist_pair_stats.Fill( make_pair(tracks, *mc));

        for( auto& track : tracks ) {
            MC_track_pair_stats.Fill( make_pair(*track, *mc) );
        }

        for( auto& rp : rec_particles ) {
            Rec_MC_stats.Fill( make_pair( rp, *mc));
        }
    }
}

void ant::analysis::MCSingleParticles::Finish()
{

}

void ant::analysis::MCSingleParticles::ShowResult()
{
}
