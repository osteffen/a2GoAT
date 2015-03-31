#ifndef OMEGA_H
#define OMEGA_H

#include "AntPhysics.h"
#include "base/interval.h"
#include "plot/Hist.h"

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

    HistWrap<const TLorentzVector&> omega_IM;
    HistWrap<const TLorentzVector&> eta_IM;
    HistWrap<const TLorentzVector&> p_MM;

    HistWrap<int> omega_rec_multi;

    HistWrap<int> nr_ngamma;
    HistWrap<const TLorentzVector&> nr_3gim;
    HistWrap<const TLorentzVector&> nr_2gim;
    HistWrap<const std::string&> mesons_found;

    IntervalD eta_im_cut;
    IntervalD pi0_im_cut;
    IntervalD omega_im_cut;
    IntervalD tagger_energy_cut;

    TLorentzVector target;

    HistWrap<const std::string&> step_levels;

    HistWrap< std::pair<const TLorentzVector&,const TLorentzVector&> > omega_mc_rec_angle;

    template <typename list_of_particles>
    std::list<decay> FindDecays(const list_of_particles &photons);

    HistWrap<const TLorentzVector&> makeInvMassPlot(const std::string& title, const std::string& xlabel, const std::string& ylabel, ant::BinSettings bins, const std::string& name="");
    HistWrap< std::pair<const TLorentzVector&, const TLorentzVector&> > makeAngleDiffPlot(const std::string& title, const std::string& xlabel, const std::string& ylabel, BinSettings bins, const std::string& name);

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
