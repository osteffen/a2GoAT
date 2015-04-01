#include "plot/HistogramFactories.h"

#include "plot/Hist.h"

#include "TMath.h"

using namespace ant;
using namespace std;

HistWrap<const ParticlePtr &> HistFactory::InvariantMass(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return HistWrap<const ParticlePtr&>::makeHist(
                [] (const ParticlePtr& p) { return p->M();},
            title,
            xlabel,
            ylabel,
            bins,
            name);
}


ant::HistWrap<const ParticlePtr &> ThetaAngle(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return HistWrap<const ParticlePtr&>::makeHist(
                [] (const ParticlePtr& p) { return p->Theta() * TMath::DegToRad();},
            title,
            xlabel,
            ylabel,
            bins,
            name);
}


ant::HistWrap<const ParticlePtr &> HistFactory::KinEnergy(const string &title, const string &xlabel, const string &ylabel, BinSettings bins, const string &name)
{
    return HistWrap<const ParticlePtr&>::makeHist(
                [] (const ParticlePtr& p) { return p->Ek();},
            title,
            xlabel,
            ylabel,
            bins,
            name);
}
