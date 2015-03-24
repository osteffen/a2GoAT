#ifndef GEOACCEPTANCE_H
#define GEOACCEPTANCE_H

#include "AntPhysics.h"
#include "plot/plotter.h"
#include "plot/Histogram.h"
#include "plot/root_draw.h"


namespace ant {
namespace analysis {



class GeoAcceptance: public ant::Physics {

private:
    static HistogramFactory hf;

    class ParticleThetaPhiPlot: public ant::root_drawable_traits {
    public:
        TH2D* hist;
        const static HistogramFactory::BinSettings theta_bins;
        const static HistogramFactory::BinSettings phi_bins;

        ParticleThetaPhiPlot(const std::string& title, const std::string& name="");
        void Fill(refPartcile p);


        TObject *GetObject();
        void Draw(const std::string &option) const;
    };

    ParticleThetaPhiPlot mctrue_pos;
    ParticleThetaPhiPlot matched_pos;
    ParticleThetaPhiPlot lost_pos;


public:
    GeoAcceptance(const mev_t energy_scale=1000.0);
    virtual ~GeoAcceptance();
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};
}
}

#endif

