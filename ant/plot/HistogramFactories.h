#ifndef HISTOGRAMFACTORIES_H
#define HISTOGRAMFACTORIES_H

#include "plot/Histogram.h"
#include "plot/SmartHist.h"
#include "Particle.h"

#include <string>

class TDirectory;
class TH1D;
class TH2D;
class TH3D;

namespace ant {

class SmartHistFactory {
private:

    TDirectory* dir;

    TDirectory *begin_make_histogram();
    void end_make_histogram(TDirectory* dir);

    HistogramFactory base_factory;


public:
    SmartHistFactory(const std::string& directory_name, TDirectory* root=nullptr);
    SmartHistFactory(const std::string& directory_name, const SmartHistFactory &parent);

    void SetRootDir(TDirectory* root_dir=nullptr);



    TH1D* makeTH1D(
            const std::string& title,
            const std::string& xlabel,
            const std::string& ylabel,
            const BinSettings& bins,
            const std::string& name="");

    TH2D* makeTH2D(
            const std::string& title,
            const std::string& xlabel,
            const std::string& ylabel,
            const BinSettings& xbins,
            const BinSettings& ybins,
            const std::string& name="");

    TH3D* makeTH3D(const std::string& title,
            const std::string& xlabel,
            const std::string& ylabel,
            const std::string& zlabel,
            const BinSettings& xbins,
            const BinSettings& ybins,
            const BinSettings& zbins,
            const std::string& name="");


    // Simple smart hists

    template<typename T, typename FunctionType>
    SmartHist<T> makeHist(FunctionType func,
            const std::string& title,
            const std::string& xlabel,
            const std::string& ylabel,
            const BinSettings& bins,
            const std::string& name="") {
        TDirectory* old = begin_make_histogram();
            SmartHist<T> r = SmartHist<T>::makeHist(func,title,xlabel,ylabel,bins,name,base_factory);
        end_make_histogram(old);
        return std::move(r);
    }

    template<typename T>
    SmartHist<T> makeHist(
            const std::string& title,
            const std::string& xlabel,
            const std::string& ylabel,
            const BinSettings& bins,
            const std::string& name="") {
        TDirectory* old = begin_make_histogram();
            SmartHist<T> r = SmartHist<T>::makeHist(title,xlabel,ylabel,bins,name,base_factory);
        end_make_histogram(old);
        return std::move(r);
    }

    // Predef smart hists

    ant::SmartHist<const ParticlePtr&> InvariantMass(
            const std::string& title,
            const std::string& xlabel="M [MeV]",
            const std::string& ylabel="",
            ant::BinSettings bins=BinSettings(1000),
            const std::string& name="");

    ant::SmartHist<const ParticlePtr&> ThetaAngle(
            const std::string& title,
            const std::string& xlabel="#theta [#circ]",
            const std::string& ylabel="",
            ant::BinSettings bins=BinSettings(180),
            const std::string& name="");

    ant::SmartHist<const ParticlePtr &> KinEnergyPlot(
            const std::string& title,
            const std::string& xlabel="#E_{k} [MeV]",
            const std::string& ylabel="",
            ant::BinSettings bins=BinSettings(1000),
            const std::string& name="");
};

}

#endif
