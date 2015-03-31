#ifndef APLCON_H
#define APLCON_H

#include "AntPhysics.h"

#include <APLCON.hpp>

#include <vector>
#include <map>
#include <random>

class TH1D;
class TH2D;

namespace ant {
namespace analysis {

class TestAPLCON: public Physics {

protected:
    TH2D* banana;
    TH1D* particles;
    TH1D* tagger;
    TH1D* ntagged;
    TH1D* cbesum;
    TH1D* pull;


    std::map<const ParticleTypeDatabase::Type*, TH1D*> numParticleType;

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

        std::vector<double*> Link() {
            return {std::addressof(Ek),
                    std::addressof(Theta),
                    std::addressof(Phi)};
        }

        void Smear(const double sigma) {
            std::normal_distribution<double> gaussian(0, sigma);
            Ek += gaussian(generator);
            Theta += gaussian(generator);
            Phi += gaussian(generator);
        }

        double Ek;
        double Theta;
        double Phi;
    };

    APLCON fitter;
    FitParticle beam;
    std::vector<FitParticle> photons;
    FitParticle proton;

    double sigma_beam;
    std::vector<double> sigma_photons;
    double sigma_proton;


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