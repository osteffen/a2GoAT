#ifndef OMEGA_H
#define OMEGA_H

#include "AntPhysics.h"
#include "base/interval.h"

class TH1D;
class TH2D;

namespace ant {
namespace analysis {

class Omega: public Physics {
protected:

    struct decay {
        Particle mesonx;
        Particle omega;
    };

    TH1D* omega_IM;
    TH1D* eta_IM;
    TH1D* p_MM;
    TH1D* omega_rec_multi;

    TH1D* nr_ngamma;
    TH1D* nr_3gim;
    TH1D* nr_2gim;
    TH1D* mesons_found;

    IntervalD eta_im_cut;
    IntervalD pi0_im_cut;
    IntervalD omega_im_cut;
    IntervalD tagger_energy_cut;

    TLorentzVector target;

    TH1D* step_levels;

    TH1D* omega_mc_rec_angle;

    template <typename list_of_particles>
    std::list<decay> FindDecays(const list_of_particles &photons);

public:
    Omega(const mev_t energy_scale=1000.0);
    virtual ~Omega() {}
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};

}
}
#endif
