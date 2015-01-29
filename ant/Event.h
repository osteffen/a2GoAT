#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include "Particle.h"
#include "Track.h"

namespace ant {

class Event {
protected:
    typedef std::list<const Particle*> ParticleList_t;
    typedef std::list<const Track*> TrackList_t;

};
}

#endif
