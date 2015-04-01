#include "plot/HistogramFactories.h"

#include "plot/SmartHist.h"

#include "TMath.h"

using namespace ant;
using namespace std;

SmartHist<const ParticlePtr &> SmartHistFactory::InvariantMass(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return SmartHist<const ParticlePtr&>::makeHist(
                [] (const ParticlePtr& p) { return p->M();},
            title,
            xlabel,
            ylabel,
            bins,
            name);
}


ant::SmartHist<const ParticlePtr &> ThetaAngle(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return SmartHist<const ParticlePtr&>::makeHist(
                [] (const ParticlePtr& p) { return p->Theta() * TMath::DegToRad();},
            title,
            xlabel,
            ylabel,
            bins,
            name);
}


ant::SmartHist<const ParticlePtr &> SmartHistFactory::KinEnergy(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return SmartHist<const ParticlePtr&>::makeHist(
                [] (const ParticlePtr& p) { return p->Ek();},
            title,
            xlabel,
            ylabel,
            bins,
            name);
}
