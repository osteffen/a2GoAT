#include "plot/HistogramFactories.h"

#include "plot/SmartHist.h"

#include "TMath.h"
#include "TDirectory.h"

using namespace ant;
using namespace std;


TDirectory *SmartHistFactory::begin_make_histogram()
{
    TDirectory* old = gDirectory;
    if(dir)
        dir->cd();

    return old;
}

void SmartHistFactory::end_make_histogram(TDirectory* dir)
{
    if(dir)
        dir->cd();
}

SmartHistFactory::SmartHistFactory(const string &directory_name, const SmartHistFactory& parent)
{
    dir = parent.dir->mkdir(directory_name.c_str());
    if(!dir)
        dir=gDirectory;
}

SmartHistFactory::SmartHistFactory(const string &directory_name, TDirectory* root) {

    if(!root)
        root=gDirectory;

    dir = root->mkdir(directory_name.c_str());

    if(!dir)
        dir=gDirectory;
}

TH1D *SmartHistFactory::makeTH1D(const string &title, const string &xlabel, const string &ylabel, const BinSettings &bins, const string &name)
{

    TDirectory* old = begin_make_histogram();
    TH1D* r = base_factory.Make1D(title,xlabel,ylabel,bins,name);
    end_make_histogram(old);
    return r;

}

TH2D *SmartHistFactory::makeTH2D(const string &title, const string &xlabel, const string &ylabel, const BinSettings &xbins, const BinSettings &ybins, const string &name)
{
    TDirectory* old = begin_make_histogram();
    TH2D* r = base_factory.Make2D(title,xlabel,ylabel,xbins,ybins,name);
    end_make_histogram(old);
    return r;
}

TH3D *SmartHistFactory::makeTH3D(const string &title,
                                 const string &xlabel,
                                 const string &ylabel,
                                 const string &zlabel,
                                 const BinSettings &xbins,
                                 const BinSettings &ybins,
                                 const BinSettings &zbins,
                                 const string &name)
{
    TDirectory* old = begin_make_histogram();
    TH3D* r = base_factory.Make3D(title,xlabel,ylabel,zlabel,xbins,ybins,zbins,name);
    end_make_histogram(old);
    return r;
}

SmartHist<const ParticlePtr &> SmartHistFactory::InvariantMass(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return makeHist<const ParticlePtr&>(
                [] (const ParticlePtr& p) { return p->M();},
            title,
            xlabel,
            ylabel,
            bins,
    name);
}

ant::SmartHist<const ParticlePtr &> SmartHistFactory::ThetaAngle(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return makeHist<const ParticlePtr&>(
                [] (const ParticlePtr& p) { return p->Theta() * TMath::DegToRad();},
            title,
            xlabel,
            ylabel,
            bins,
            name);
}

ant::SmartHist<const ParticlePtr &> SmartHistFactory::KinEnergyPlot(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return makeHist<const ParticlePtr&>(
                [] (const ParticlePtr& p) { return p->Ek();},
            title,
            xlabel,
            ylabel,
            bins,
            name);
}
