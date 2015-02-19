#ifndef OMEGA_H
#define OMEGA_H

#include "AntPhysics.h"
#include "interval.h"

class TH1D;
class TH2D;

namespace ant {
namespace analysis {

class Omega: public Physics {
protected:
    TH1D* omega_IM;
    TH1D* eta_IM;
    TH1D* p_MM;
    TH1D* omega_rec_multi;

    TH1D* nr_ngamma;
    TH1D* nr_3gim;
    TH1D* nr_2gim;

    interval<mev_t> eta_im_cut;
    interval<mev_t> omega_im_cut;
    interval<mev_t> tagger_energy_cut;

    TLorentzVector target;

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
