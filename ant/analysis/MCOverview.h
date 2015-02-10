#ifndef MCOVERVIEW_H
#define MCOVERVIEW_H

#include "AntPhysics.h"
#include "plotter.h"
#include "Histogram.h"


namespace ant {
namespace analysis {

class MCOverview: public ant::Physics {

    PlotList<Particle> mc_particle_stats;

    HistogramFactory hf;


    // Physics interface
public:
    MCOverview();
    virtual ~MCOverview();
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};
}
}

#endif
