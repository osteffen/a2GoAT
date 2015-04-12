#ifndef MCTRUE_ACCEPTANCE_H
#define MCTRUE_ACCEPTANCE_H

#include "AntPhysics.h"
#include "plot/SmartHist.h"

#include "A2GeoAcceptance.h"

namespace ant {
namespace analysis {

class MCTrueAcceptance: public ant::Physics {
protected:
    SmartHist1<std::string> detect;
    A2SimpleGeometry geo;
    unsigned int events_seen;

    bool AllAccepted(const ParticleList& particles);

public:
    MCTrueAcceptance();

    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};

}

}


#endif
