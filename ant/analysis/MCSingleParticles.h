#ifndef MCSINGLEPARTICLES_H
#define MCSINGLEPARTICLES_H

#include "AntPhysics.h"
#include "plotter.h"
#include "Histogram.h"


namespace ant {
namespace analysis {

class MCSingleParticles: public ant::Physics {

public:
    typedef std::pair<const ant::Track&, const ant::MCParticle&> Track_MC_pair;
    typedef std::pair<const Event::TrackList_t&, const ant::MCParticle&> MC_tracklist_pair;
private:

    PlotList<Track_MC_pair> MC_track_pair_stats;
    PlotList<MC_tracklist_pair> MC_tracklist_pair_stats;

    HistogramFactory hf;


    // Physics interface
public:
    MCSingleParticles(const mev_t energy_scale=1000.0);
    virtual ~MCSingleParticles();
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};
}
}

#endif
