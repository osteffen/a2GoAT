#include "omega.h"
#include "Particle.h"
#include "Event.h"
#include "TLorentzVector.h"
#include "TH1D.h"
#include "plot/root_draw.h"
#include "plot/Histogram.h"
#include "utils/combinatorics.h"
#include "TaggerHit.h"
#include <string>
#include <iostream>
#include "plot/Hist.h"

using namespace std;

template <typename list_of_particles>
std::list<ant::analysis::Omega::decay> ant::analysis::Omega::FindDecays(const list_of_particles &photons)
{
    std::list<decay> decays;

    for( auto comb = makeCombination(photons,3); !comb.Done(); ++comb) {

        refParticleList_t ggg;
        ggg.assign(comb.begin(),comb.end());

        TLorentzVector omega = *comb.at(0)+*comb.at(1)+*comb.at(2);

        if( omega_im_cut.Contains(omega.M())) {

            for( auto gcomb = makeCombination(ggg,2); !gcomb.Done(); ++gcomb) {

                const TLorentzVector& g1 = *(gcomb.at(0));
                const TLorentzVector& g2 = *(gcomb.at(1));

                TLorentzVector mesonx = g1 + g2;

                const ParticleTypeDatabase::Type* mesontype = nullptr;

                if(pi0_im_cut.Contains(mesonx.M()))
                    mesontype = &ParticleTypeDatabase::Pi0;
                else if(eta_im_cut.Contains(mesonx.M()))
                    mesontype = &ParticleTypeDatabase::Eta;

                if( mesontype ) {

                    decays.push_back(
                                decay( {
                                    Particle(*mesontype, mesonx),
                                    Particle(ParticleTypeDatabase::Omega, omega) }
                                    )
                                );

                }
            }

        }

    }

    return decays;

}

ant::HistWrap<const TLorentzVector&> ant::analysis::Omega::makeInvMassPlot(const std::string& title, const std::string& xlabel, const std::string& ylabel, BinSettings bins, const std::string& name) {
    return ant::HistWrap<const TLorentzVector&>::makeHist(
                [] (const TLorentzVector& p) { return p.M();},
                title,
                xlabel, ylabel, bins, name);
}

ant::HistWrap< std::pair<const TLorentzVector&, const TLorentzVector&> > ant::analysis::Omega::makeAngleDiffPlot(const std::string& title, const std::string& xlabel, const std::string& ylabel, BinSettings bins, const std::string& name) {
    return ant::HistWrap<std::pair<const TLorentzVector&, const TLorentzVector&>>::makeHist(
                                                                                     [] (std::pair<const TLorentzVector&, const TLorentzVector&> particles) {
                                                                                 return particles.first.Angle(particles.second.Vect())* TMath::RadToDeg();},
            title,
            xlabel, ylabel, bins, name);
}

ant::analysis::Omega::Omega(const ant::mev_t energy_scale):

    eta_im_cut(   IntervalD::CenterWidth( ParticleTypeDatabase::Eta.Mass(), 50.0)),
    pi0_im_cut( IntervalD::CenterWidth(ParticleTypeDatabase::Pi0.Mass(),20.0)),
    omega_im_cut( IntervalD::CenterWidth( ParticleTypeDatabase::Omega.Mass(), 80.0)),
    tagger_energy_cut(1420, 1575),
    target(0.0, 0.0, 0.0, ParticleTypeDatabase::Proton.Mass())
{
    const BinSettings energy_bins(1000, 0.0, energy_scale);
    const BinSettings p_MM_bins(1000, 500.0, 1500.0);
    const BinSettings angle_diff_bins(200,0.0,20.0);

    HistogramFactory::SetName("Omega");

    eta_IM      = makeInvMassPlot("2 #gamma IM (after omega cut)",  "M_{3#gamma}", "", energy_bins, "eta_IM");
    omega_IM    = makeInvMassPlot("3 #gamma IM (->#omega)",         "M_{3#gamma}", "", energy_bins, "omega_IM");
    p_MM        = makeInvMassPlot("MM",                             "MM [MeV]",    "", p_MM_bins,   "omega_MM");

    omega_rec_multi = HistWrap<int>::makeHist(
                ParticleTypeDatabase::Omega.PrintName() + " Reconstruction Multiplicity",
                "n",
                "",
                BinSettings(5));

    nr_ngamma = HistWrap<int>::makeHist(
                "Not reconstructed: number of photons",
                "number of photons/event",
                "",
                BinSettings(16));

    nr_2gim = makeInvMassPlot(
                "Not reconstructed: 2#gamma IM",
                "M_{2#gamma} [MeV]",
                "",
                energy_bins);

    nr_3gim = makeInvMassPlot(
                "Not reconstructed: 3#gamma IM",
                "M_{3#gamma} [MeV]",
                "",
                energy_bins);

    step_levels = HistWrap<const std::string&>::makeHist(
                "Check pass count",
                "Check",
                "# passed",
                BinSettings(10));

    omega_mc_rec_angle = makeAngleDiffPlot(
                ParticleTypeDatabase::Omega.PrintName()+" MC/Rec angle",
                "angle [#circ]",
                "# / " + to_string(angle_diff_bins.BinWidth())+" #circ",
                angle_diff_bins,
                ParticleTypeDatabase::Eta.Name()+"_mc_rec_angle"
                );

    mesons_found = HistWrap<const std::string&>::makeHist("Mesons Found", "Meson","", BinSettings(3));
}

template <class InputIterator, class T>
T sum (InputIterator first, InputIterator last, T init) {
    while (first!=last) {
        init += **first;
        ++first;
    }
    return std::move(init);
}

template <class C, class T>
T sum (const C& data, T init) {
    return std::move(sum(data.begin(), data.end(), init));
}

void ant::analysis::Omega::ProcessEvent(const ant::Event &event)
{
    step_levels.Fill("0 Events Seen");

    if(event.Trigger().CBEenergySum()<550.0)
        return;

    step_levels.Fill("1 ESum Cut passed");

    const refRecParticleList_t photons = event.ParticleType(ParticleTypeDatabase::Photon);

    if(photons.size()<3)
        return;

    step_levels.Fill("2 NPhotons 3+");

    const MCParticle* mc_omega=nullptr;
    for( auto& mcp : event.MCTrue() ) {
        if(mcp->Type() == ParticleTypeDatabase::Omega) {
            if(mc_omega!=nullptr)
                throw string("Multiple omegas found in MC True");
            mc_omega = mcp;
        }
    }

    unsigned int n_omega_found = 0;

    for( auto comb = makeCombination(photons,3); !comb.Done(); ++comb) {

        refRecParticleList_t ggg;
        ggg.assign(comb.begin(),comb.end());

        TLorentzVector omega = *comb.at(0)+*comb.at(1)+*comb.at(2);

        if( omega_im_cut.Contains(omega.M())) {
            step_levels.Fill("3 #omega IM cut passed");

            for( auto gcomb = makeCombination(ggg,2); !gcomb.Done(); ++gcomb) {

                TLorentzVector g1(*gcomb.at(0));
                TLorentzVector g2(*gcomb.at(1));
                TLorentzVector eta = g1 + g2;

                eta_IM.Fill(eta);

                if(eta_im_cut.Contains(eta.M())) {

                    step_levels.Fill("4 #eta IM cut passed");
                    omega_IM.Fill(omega);
                    n_omega_found++;

                    for( auto& taggerhit : event.TaggerHits() ) {
                        if( tagger_energy_cut.Contains(taggerhit->PhotonEnergy())) {
                            TLorentzVector p = taggerhit->PhotonBeam() + target - omega;
                            p_MM.Fill(p);
                        }
                    }

                    if(mc_omega) {
                        omega_mc_rec_angle.Fill( make_pair(*mc_omega,omega) );
                    }
                }
            }

        }

    }
    omega_rec_multi.Fill(n_omega_found);

    if(n_omega_found == 0) {
        nr_ngamma.Fill(photons.size());

        for( auto comb = makeCombination(photons,3); !comb.Done(); ++comb) {
            TLorentzVector m = sum(comb, TLorentzVector() );
            nr_3gim.Fill(m);
        }

        for( auto comb = makeCombination(photons,2); !comb.Done(); ++comb) {
            TLorentzVector m = sum(comb, TLorentzVector() );
            nr_2gim.Fill(m);
        }
    }

    auto decays = FindDecays(event.ParticleType(ParticleTypeDatabase::Photon));

    refMCParticleList_t mcphotons;
    std::copy_if (event.MCTrueFinalState().begin(), event.MCTrueFinalState().end(), std::back_inserter(mcphotons),
                [](refMCParticle p) { return p->Type() == ParticleTypeDatabase::Photon;
              });
//    auto mc_dacays = FindDecays(mcphotons);

    for( auto& d : decays) {
        mesons_found.Fill(d.mesonx.Type().PrintName());
    }
}


void ant::analysis::Omega::Finish()
{

}


void ant::analysis::Omega::ShowResult()
{
    canvas("Omega (Reconstructed)") << omega_IM << eta_IM << p_MM << step_levels << omega_rec_multi << omega_mc_rec_angle << endc;
    canvas("Omega (Not Reconstructed)") << nr_ngamma << nr_2gim << nr_3gim << mesons_found << endc;

}
