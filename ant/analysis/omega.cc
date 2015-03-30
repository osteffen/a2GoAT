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



ant::analysis::Omega::Omega(const ant::mev_t energy_scale):
    eta_im_cut(   IntervalD::CenterWidth( ParticleTypeDatabase::Eta.Mass(), 50.0)),
    pi0_im_cut( IntervalD::CenterWidth(ParticleTypeDatabase::Pi0.Mass(),20.0)),
    omega_im_cut( IntervalD::CenterWidth( ParticleTypeDatabase::Omega.Mass(), 80.0)),
    tagger_energy_cut(1420, 1575),
    target(0.0, 0.0, 0.0, ParticleTypeDatabase::Proton.Mass())
{
    const HistogramFactory::BinSettings energy_bins(1000, 0.0, energy_scale);
    const HistogramFactory::BinSettings p_MM_bins(1000, 500.0, 1500.0);
    const HistogramFactory::BinSettings angle_diff_bins(200,0.0,20.0);
    HistogramFactory hf("Omega");

    eta_IM = hf.Make1D(ParticleTypeDatabase::Eta.PrintName()+" IM",
                       "M_{"+ParticleTypeDatabase::Eta.PrintName()+"} [MeV]",
                       "# / " + to_string(energy_bins.BinWidth())+" MeV",
                       energy_bins,
                       ParticleTypeDatabase::Eta.Name()+"_IM"
                       );
    omega_IM = hf.Make1D(ParticleTypeDatabase::Omega.PrintName()+" IM",
                       "M_{"+ParticleTypeDatabase::Omega.PrintName()+"} [MeV]",
                       "# / " + to_string(energy_bins.BinWidth())+" MeV",
                       energy_bins,
                       ParticleTypeDatabase::Omega.Name()+"_IM"
                       );
    p_MM = hf.Make1D(ParticleTypeDatabase::Proton.PrintName()+" MM",
                       "MM_{"+ParticleTypeDatabase::Proton.PrintName()+"} [MeV]",
                       "# / " + to_string(p_MM_bins.BinWidth())+" MeV",
                       p_MM_bins,
                       ParticleTypeDatabase::Proton.Name()+"_MM"
                       );
    omega_rec_multi = hf.Make1D( ParticleTypeDatabase::Omega.PrintName()+ " Reconstruction Multiplicity",
                                 "n","", HistogramFactory::BinSettings(5,0,5)
                                 );
    omega_rec_multi->GetXaxis()->SetDecimals();

    nr_ngamma = hf.Make1D("Not reconstructed: number of photons",
                          "number of photons/event","",HistogramFactory::BinSettings(16,0,16));
    nr_2gim = hf.Make1D("Not reconstructed: 2#gamma IM",
                        "M_{2#gamma} [MeV]","",energy_bins);
    nr_3gim = hf.Make1D("Not reconstructed: 3#gamma IM",
                        "M_{3#gamma} [MeV]","",energy_bins);
    step_levels = hf.Make1D("Check pass count",
                            "Check",
                            "# passed",
                            HistogramFactory::BinSettings(10,0,10)
                            );
    step_levels->SetStats(kFALSE);

    omega_mc_rec_angle = hf.Make1D(ParticleTypeDatabase::Omega.PrintName()+" MC/Rec angle",
                                   "angle [#circ]",
                                   "# / " + to_string(angle_diff_bins.BinWidth())+" #circ",
                                   angle_diff_bins,
                                   ParticleTypeDatabase::Eta.Name()+"_mc_rec_angle"
                                   );

    mesons_found = hf.Make1D("Mesons Found", "Meson","", HistogramFactory::BinSettings(3));
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
    step_levels->Fill("0 Events Seen",1);

    if(event.Trigger().CBEenergySum()<550.0)
        return;

    step_levels->Fill("1 ESum Cut passed",1);

    const refRecParticleList_t photons = event.ParticleType(ParticleTypeDatabase::Photon);

    if(photons.size()<3)
        return;

    step_levels->Fill("2 NPhotons 3+",1);

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
            step_levels->Fill("3 #omega IM cut passed",1);

            for( auto gcomb = makeCombination(ggg,2); !gcomb.Done(); ++gcomb) {

                TLorentzVector g1(*gcomb.at(0));
                TLorentzVector g2(*gcomb.at(1));
                TLorentzVector eta = g1 + g2;

                eta_IM->Fill(eta.M());

                if(eta_im_cut.Contains(eta.M())) {

                    step_levels->Fill("4 #eta IM cut passed",1);
                    omega_IM->Fill(omega.M());
                    n_omega_found++;

                    for( auto& taggerhit : event.TaggerHits() ) {
                        if( tagger_energy_cut.Contains(taggerhit->PhotonEnergy())) {
                            TLorentzVector p = taggerhit->PhotonBeam() + target - omega;
                            p_MM->Fill(p.M());
                        }
                    }

                    if(mc_omega) {
                        double omega_angles = mc_omega->Angle(omega.Vect()) * TMath::RadToDeg();
                        omega_mc_rec_angle->Fill(omega_angles);
                    }
                }
            }

        }

    }
    omega_rec_multi->Fill(n_omega_found);

    if(n_omega_found == 0) {
        nr_ngamma->Fill(photons.size());

        for( auto comb = makeCombination(photons,3); !comb.Done(); ++comb) {
            TLorentzVector m = sum(comb, TLorentzVector() );
            nr_3gim->Fill(m.M());
        }

        for( auto comb = makeCombination(photons,2); !comb.Done(); ++comb) {
            TLorentzVector m = sum(comb, TLorentzVector() );
            nr_2gim->Fill(m.M());
        }
    }

    auto decays = FindDecays(event.ParticleType(ParticleTypeDatabase::Photon));

    refMCParticleList_t mcphotons;
    std::copy_if (event.MCTrueFinalState().begin(), event.MCTrueFinalState().end(), std::back_inserter(mcphotons),
                [](refMCParticle p) { return p->Type() == ParticleTypeDatabase::Photon;
              });
    auto mc_dacays = FindDecays(mcphotons);
    cout << "--- Rec ---"  << "\n";
    for( auto& d : decays) {
        cout << d.mesonx.Type().Name() << " ";
        mesons_found->Fill(d.mesonx.Type().PrintName().c_str(),1);
    }
    cout << "\n --- MC ---\n";
    for( auto& d : mc_dacays) {
        cout << d.mesonx.Type().Name() << " ";
    }
    cout << endl;
}


void ant::analysis::Omega::Finish()
{

}


void ant::analysis::Omega::ShowResult()
{
    canvas("Omega (Reconstructed)") << eta_IM << omega_IM << p_MM << omega_rec_multi << step_levels <<  omega_mc_rec_angle << endc;
    canvas("Omega (Not Reconstructed)") << nr_ngamma << nr_2gim << nr_3gim << mesons_found << endc;

}
