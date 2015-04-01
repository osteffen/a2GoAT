#ifndef OMEGA_BOTTOMUP_H
#define OMEGA_BOTTOMUP_H

#include "AntPhysics.h"
#include "plot/Hist.h"
#include "base/interval.h"
#include "Particle.h"


#include <list>

namespace ant {
namespace analysis {

class OmegaBottomUp: public ant::Physics {
protected:

    IntervalD eta_mass_cut;
    IntervalD pi0_mass_cut;
    IntervalD omega_mass_cut;

    struct omega_decay {
        omega_decay(ParticlePtr omega_, ParticlePtr meson2_): omega(omega_), meson2(meson2_) {}
        ParticlePtr omega;
        ParticlePtr meson2;
    };

    using decaylist_t = std::list<omega_decay>;

    void findPossibleDecays(const ParticlePtr gamma1, const ParticlePtr meson_gamma_1, const ParticlePtr meson_gamma_2, decaylist_t &decays);

    HistWrap<int> omega_eta_found;
    HistWrap<int> omega_pi0_found;

    HistWrap<const ParticlePtr&> omega_IM;
    HistWrap<const ParticlePtr&> eta_IM;
    HistWrap<const ParticlePtr&> pi0_IM;

public:
    OmegaBottomUp();
    virtual ~OmegaBottomUp() {}

    virtual void ProcessEvent(const ant::Event& event);
    virtual void Finish();
    virtual void ShowResult();
};
}
}
#endif
