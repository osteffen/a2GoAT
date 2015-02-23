#include "RecoCheck.h"

#include "TH1D.h"
#include "matcher.h"
#include "TLorentzVector.h"
#include "Particle.h"
#include "root_draw.h"
#include "Histogram.h"


using namespace std;
using namespace ant;



analysis::RecoCheck::RecoCheck():
    cb_angle(20.0*TMath::DegToRad(), 160.0*TMath::DegToRad())
{
    HistogramFactory hf("RecoCheck");
    const HistogramFactory::BinSettings angle_bins(300,0.0,30.0);
    const HistogramFactory::BinSettings npart_bins(10);

    angle_diff = hf.Make1D( "MC/Rec Angle IM",
                           "angle [#circ]",
                           "# / " + to_string(angle_bins.BinWidth())+" #circ",
                           angle_bins,
                           "angle_diff"
                            );
    n_unmatched = hf.Make1D("Unmatched particles / event",
                            "# unmatched / event","",npart_bins,"n_unmatched");
}

void analysis::RecoCheck::ProcessEvent(const Event &event)
{
    const refMCParticleList_t& mc = event.MCTrue();
    const refRecParticleList_t& rec = event.Particles();

    refMCParticleList_t mc_in_cb;

    for( auto& p : mc ) {
        if( cb_angle.Contains(p->Theta()))
            mc_in_cb.push_back(p);
    }

    // find mc-reco matches
    auto matched = Match(mc_in_cb,rec,matchAngle);

    // cacluate angles for all matches
    for( auto& match : matched ) {
        const double a = match.first->Angle(match.second->Vect());
        angle_diff->Fill(a);
    }

    // fill number of unmatched mc particles
    n_unmatched->Fill(mc_in_cb.size() - matched.size());

}

void analysis::RecoCheck::Finish()
{

}

void analysis::RecoCheck::ShowResult()
{
    canvas("RecoCheck") << angle_diff << n_unmatched << canvas::cend;
}

