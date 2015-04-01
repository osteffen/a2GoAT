#ifndef TESTPHYSICS_H
#define TESTPHYSICS_H

#include "AntPhysics.h"
#include "plot/Histogram.h"
#include "plot/SmartHist.h"

#include <map>

namespace ant {
class ParticleCombinatoricsTest: public ant::Physics {
protected:
    SmartHist<double> ggim;
    SmartHist<double> gggim;
    SmartHist<int>    nphotons;
    SmartHist<int>    nprotons;

    std::map<const ant::ParticleTypeDatabase::Type*, SmartHist<const ParticlePtr&>> EHists;


public:
    ParticleCombinatoricsTest();
    virtual ~ParticleCombinatoricsTest() {}

    virtual void ProcessEvent(const ant::Event& event);
    virtual void Finish();
    virtual void ShowResult();
};

}
#endif
