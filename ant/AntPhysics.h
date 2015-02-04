#ifndef ANTPHYSICS_H
#define ANTPHYSICS_H

#include "Event.h"

namespace ant {

class Physics {
public:
    virtual ~Physics() {}
    virtual void ProcessEvent(const ant::Event& event) =0;
    virtual void Finish() =0;
    virtual void ShowResult() =0;
};

class DebugPhysics: public Physics {
public:
    DebugPhysics() {}
    virtual ~DebugPhysics() {}

    virtual void ProcessEvent(const ant::Event& event);
    virtual void Finish();
    virtual void ShowResult();
};
}


#endif
