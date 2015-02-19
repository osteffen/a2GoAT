#include "omega.h"
#include "Particle.h"
#include "Event.h"
#include "TLorentzVector.h"
#include "TH1D.h"
#include "root_draw.h"
#include "Histogram.h"
#include "combinatorics.h"
#include "TaggerHit.h"

ant::analysis::Omega::Omega(const ant::mev_t energy_scale):
    eta_im_cut(   interval<mev_t>::CenterWidth( ParticleTypeDatabase::Eta.Mass(), 50.0)),
    omega_im_cut( interval<mev_t>::CenterWidth( ParticleTypeDatabase::Omega.Mass(), 80.0)),
    tagger_energy_cut(1420,1575),
    target(0.0, 0.0, 0.0, ParticleTypeDatabase::Proton.Mass())
{
    const HistogramFactory::BinSettings energy_bins(1000, 0.0, energy_scale);
    const HistogramFactory::BinSettings p_MM_bins(1000, 500.0, 1500.0);
    HistogramFactory hf("Omega");

    eta_IM = hf.Make1D(ParticleTypeDatabase::Eta.PrintName()+" IM",
                       "M_{"+ParticleTypeDatabase::Eta.PrintName()+"} [MeV]",
                       "",
                       energy_bins,
                       ParticleTypeDatabase::Eta.Name()+"_IM"
                       );
    omega_IM = hf.Make1D(ParticleTypeDatabase::Omega.PrintName()+" IM",
                       "M_{"+ParticleTypeDatabase::Omega.PrintName()+"} [MeV]",
                       "",
                       energy_bins,
                       ParticleTypeDatabase::Omega.Name()+"_IM"
                       );
    p_MM = hf.Make1D(ParticleTypeDatabase::Proton.PrintName()+" MM",
                       "MM_{"+ParticleTypeDatabase::Proton.PrintName()+"} [MeV]",
                       "",
                       p_MM_bins,
                       ParticleTypeDatabase::Proton.Name()+"_MM"
                       );
    omega_rec_multi = hf.Make1D( ParticleTypeDatabase::Omega.PrintName()+ " Reconstruction Multiplicity",
                                 "n","", HistogramFactory::BinSettings(5,0,5)
                                 );

    nr_ngamma = hf.Make1D("Not reconstructed: number of photons",
                          "number of photons/event","",HistogramFactory::BinSettings(16,0,16));
    nr_2gim = hf.Make1D("Not reconstructed: 2#gamma IM",
                        "M_{2#gamma} [MeV]","",energy_bins);
    nr_3gim = hf.Make1D("Not reconstructed: 3#gamma IM",
                        "M_{3#gamma} [MeV]","",energy_bins);
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
    const refRecParticleList_t photons = event.ParticleType(ParticleTypeDatabase::Photon);

    unsigned int n_omega_found = 0;

    for( auto comb = makeCombination(photons,3); !comb.Done(); ++comb) {

        refRecParticleList_t ggg;
        ggg.assign(comb.begin(),comb.end());

        TLorentzVector omega = *comb.at(0)+*comb.at(1)+*comb.at(2);

        if( omega_im_cut.Contains(omega.M())) {

            for( auto gcomb = makeCombination(ggg,2); !gcomb.Done(); ++gcomb) {
                TLorentzVector eta = *gcomb.at(0) + *gcomb.at(1);
                eta_IM->Fill(eta.M());
                if(eta_im_cut.Contains(eta.M())) {
                    omega_IM->Fill(omega.M());
                    n_omega_found++;

                    for( auto& taggerhit : event.TaggerHits() ) {
                        if( tagger_energy_cut.Contains(taggerhit->PhotonEnergy())) {
                            TLorentzVector p = taggerhit->PhotonBeam() + target - omega;
                            p_MM->Fill(p.M());
                        }
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

}


void ant::analysis::Omega::Finish()
{

}


void ant::analysis::Omega::ShowResult()
{
    canvas("Omega (Reconstructed)") << eta_IM << omega_IM << p_MM << omega_rec_multi << canvas::cend;
    canvas("Omega (Not Reconstructed)") << nr_ngamma << nr_2gim << nr_3gim << canvas::cend;

}
