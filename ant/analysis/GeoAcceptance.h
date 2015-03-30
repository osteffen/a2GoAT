#ifndef GEOACCEPTANCE_H
#define GEOACCEPTANCE_H

#include "AntPhysics.h"
#include "plot/plotter.h"
#include "plot/Histogram.h"
#include "plot/root_draw.h"
#include "A2GeoTools.h"

class TH3;
class TH1D;

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

    class ParticleThetaPhiPlot3D: public ant::root_drawable_traits {
    public:
        TH3* hist;
        unsigned int n;

        ParticleThetaPhiPlot3D(const std::string& title, const std::string& name="");
        void Fill(refPartcile p);


        TObject *GetObject();
        void Draw(const std::string &option) const;
    };

    ParticleThetaPhiPlot mctrue_pos;
    ParticleThetaPhiPlot matched_pos;
    ParticleThetaPhiPlot lost_pos;
    ParticleThetaPhiPlot3D lost3d;

    TH2D* angle_regions_protons;
    TH1D* angle_regions_photons;

    TH1D* n_photons_lost;

    A2SimpleGeometry geo;



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

