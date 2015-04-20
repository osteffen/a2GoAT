#ifndef OMEGA_H
#define OMEGA_H

#include "AntPhysics.h"
#include "base/interval.h"
#include "plot/SmartHist.h"

class TH1D;
class TH2D;

namespace ant {
namespace analysis {

class Omega: public Physics {
protected:

    SmartHist1<const TLorentzVector&> omega_IM;
    SmartHist1<const TLorentzVector&> eta_IM;
    SmartHist1<const TLorentzVector&> p_MM;

    SmartHist1<int> omega_rec_multi;

    SmartHist1<int> nr_ngamma;
    SmartHist1<const TLorentzVector&> nr_3gim;
    SmartHist1<const TLorentzVector&> nr_2gim;

    int n=0;
    SmartHist1<const ParticlePtr&> test;

    IntervalD eta_im_cut;
    IntervalD pi0_im_cut;
    IntervalD omega_im_cut;
    IntervalD tagger_energy_cut;

    TLorentzVector target;

    SmartHist1<std::string> step_levels;

    SmartHist1< std::pair<const TLorentzVector&,const TLorentzVector&> > omega_mc_rec_angle;

    SmartHist1<const TLorentzVector&> makeInvMassPlot(const std::string& title, const std::string& xlabel, const std::string& ylabel, ant::BinSettings bins, const std::string& name="");
    SmartHist1< std::pair<const TLorentzVector&, const TLorentzVector&> > makeAngleDiffPlot(const std::string& title, const std::string& xlabel, const std::string& ylabel, BinSettings bins, const std::string& name);
    bool run_on_true;

public:
    Omega(const std::string& name, bool mctrue, const mev_t energy_scale=1000.0);
    virtual ~Omega() {}
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};

}
}
#endif
