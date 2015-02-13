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
    MCOverview(const mev_t energy_scale=1000.0);
    virtual ~MCOverview();
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};
}
}

#endif
