#ifndef TESTPHYSICS_H
#define TESTPHYSICS_H

#include "AntPhysics.h"
#include <map>
#include "plot/plotter.h"
#include "plot/Histogram.h"

class TH1;

namespace ant {
class ParticleCombinatoricsTest: public ant::Physics {
protected:
    TH1*    ggim;
    TH1*    gggim;
    TH1*    nphotons;
    TH1*    nprotons;

    std::map<const ant::ParticleTypeDatabase::Type*, TH1*> EHists;


public:
    ParticleCombinatoricsTest();
    virtual ~ParticleCombinatoricsTest() {}

    virtual void ProcessEvent(const ant::Event& event);
    virtual void Finish();
    virtual void ShowResult();
};

class PlotterTest: public ant::Physics {
protected:
    ant::PlotList<const ant::Track*> track_plots;
    ant::PlotList<const ant::Particle*> particle_plots;

    // Physics interface
public:
    PlotterTest();
    virtual ~PlotterTest() {}

    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};
}
#endif
