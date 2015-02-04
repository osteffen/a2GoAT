#ifndef TESTPHYSICS_H
#define TESTPHYSICS_H

#include "AntPhysics.h"

class TH1;
namespace ant {
class ParticleCombinatoricsTest: public ant::Physics {
protected:
    TH1*    ggim;
    TH1*    gggim;
    TH1*    nphotons;
    TH1*    nprotons;
public:
    ParticleCombinatoricsTest();
    virtual ~ParticleCombinatoricsTest() {}

    virtual void ProcessEvent(const ant::Event& event);
    virtual void Finish();
    virtual void ShowResult();
};
}
#endif
