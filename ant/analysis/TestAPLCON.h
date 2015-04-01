#ifndef APLCON_H
#define APLCON_H

#include "AntPhysics.h"

#include <APLCON.hpp>
#include "plot/Histogram.h"


#include <vector>
#include <map>
#include <random>

#include "TH1D.h"
#include "TH2D.h"


namespace ant {
namespace analysis {

class TestAPLCON: public Physics {

protected:

    const bool includeIMconstraint = false;
    const size_t nPhotons = 6;
//    const double IM = ParticleTypeDatabase::Pi0.Mass();
    const double IM = ParticleTypeDatabase::EtaPrime.Mass();


    ant::HistogramFactory hf;
    TH2D* banana;
    TH1D* particles;
    TH1D* tagger;
    TH1D* ntagged;
    TH1D* cbesum;

    TH1D* im_true;
    TH1D* im_smeared;
    TH1D* im_fit;



    std::map<const ParticleTypeDatabase::Type*, TH1D*> numParticleType;

    TH1D* chisquare;
    TH1D* probability;
    std::map<std::string, TH1D*> pulls;

    // lightweight structure for linking to fitter
    struct FitParticle {
    private:
        static std::default_random_engine generator;
    public:
        void SetFromVector(const TLorentzVector& p_) {
            Ek = p_.E()-p_.M();
            Theta = p_.Theta();
            Phi = p_.Phi();
        }

        static TLorentzVector Make(const std::vector<double>& EkThetaPhi,
                                           const Double_t m) {
            const double E = EkThetaPhi[0] + m;
            const Double_t p = sqrt( E*E - m*m );
            TVector3 pv(1,0,0);
            pv.SetMagThetaPhi(p, EkThetaPhi[1], EkThetaPhi[2]);
            TLorentzVector l(pv, E);
            return l;
        }
        static TLorentzVector Make(const FitParticle& p,
                                   const Double_t m) {
            return Make(std::vector<double>{p.Ek, p.Theta, p.Phi}, m);
        }

        std::vector<double*> Link() {
            return {std::addressof(Ek),
                    std::addressof(Theta),
                    std::addressof(Phi)};
        }
        std::vector<double*> LinkSigma() {
            return {std::addressof(Ek_Sigma),
                    std::addressof(Theta_Sigma),
                    std::addressof(Phi_Sigma)};
        }

        void Smear() {
            // set the sigmas here,
            // then the fitter knows them as well
            Ek_Sigma = 0.02*Ek*pow(Ek,-0.36);
            Theta_Sigma = 2.5*TMath::DegToRad();
            if(Theta>20*TMath::DegToRad() && Theta<160*TMath::DegToRad()) {
                Phi_Sigma = Theta_Sigma/sin(Theta);
            }
            else {
                Phi_Sigma = 1*TMath::DegToRad();
            }

            //Phi_Sigma = Theta_Sigma/sin(Theta);
            //if(!std::isfinite(Phi_Sigma))
            //    Phi_Sigma = Theta_Sigma;
            //if(!std::isfinite(Phi_Sigma) || Phi_Sigma>10*TMath::DegToRad())
            //    Phi_Sigma = 10*TMath::DegToRad();
            //if(Theta>0 && Theta<10*TMath::DegToRad())
            //   Phi_Sigma += Theta_Sigma/sin(Theta);


            // then artificially smear the values with gaussians
            using gauss_t = std::normal_distribution<double>;
            gauss_t gauss_Ek(0, Ek_Sigma);
            Ek += gauss_Ek(generator);
            gauss_t gauss_Theta(0, Theta_Sigma);
            Theta += gauss_Theta(generator);
            gauss_t gauss_Phi(0, Phi_Sigma);
            Phi += gauss_Phi(generator);
        }

        double Ek;
        double Ek_Sigma;
        double Theta;
        double Theta_Sigma;
        double Phi;
        double Phi_Sigma;
    };

    void FillIM(TH1D* h, const std::vector<FitParticle>& photons) {
        TLorentzVector sum(0,0,0,0);
        for(const auto& p : photons) {
            sum += FitParticle::Make(p, ParticleTypeDatabase::Photon.Mass());
        }
        h->Fill(sum.M());
    }

    APLCON fitter;
    FitParticle beam;
    std::vector<FitParticle> photons;
    FitParticle proton;


public:
    TestAPLCON(const mev_t energy_scale=1000.0);
    virtual ~TestAPLCON() {}
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};

}
}
#endif
