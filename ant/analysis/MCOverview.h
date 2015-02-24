#ifndef MCOVERVIEW_H
#define MCOVERVIEW_H

#include "AntPhysics.h"
#include "plotter.h"
#include "Histogram.h"

class TH1D;
class TH2D;

namespace ant {
namespace analysis {

class MCOverview: public ant::Physics {
    mev_t energy_scale;

    PlotList<Particle> mc_particle_stats;

    // Physics interface
public:
    MCOverview(const mev_t _energy_scale=1000.0);
    virtual ~MCOverview();
    void Init();
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};
}
}

#endif
