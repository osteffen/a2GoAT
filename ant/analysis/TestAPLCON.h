#ifndef APLCON_H
#define APLCON_H

#include "AntPhysics.h"
#include <vector>
#include <map>
#include <APLCON.hpp>

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

    std::map<const ParticleTypeDatabase::Type*, TH1D*> numParticleType;

    // lightweight structure for linking to fitter
    struct FitParticle {
    private:
        const ant::Particle* p;
    public:
        FitParticle(const ant::Particle& p_) :
            p(&p_), E(p_.E()), Theta(p_.Theta()), Phi(p_.Phi())  {}
        FitParticle() {}

        std::vector<double*> Link() {
            return {std::addressof(E),
                    std::addressof(Theta),
                    std::addressof(Phi)};
        }

        double E;
        double Theta;
        double Phi;
    };

    APLCON fitter;
    FitParticle photon1;
    FitParticle photon2;
    FitParticle photon3;
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
