#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include "Particle.h"
#include "Track.h"

#include <vector>
#include <memory>

namespace ant {

class Event {
public:

    typedef std::shared_ptr<const ant::Particle>    sParticlePtr;
    typedef std::shared_ptr<const ant::Track>       sTrackPtr;

    typedef std::vector<sParticlePtr>   ParticleList_t;
    typedef std::vector<sTrackPtr>      TrackList_t;

protected:

    TrackList_t tracks;
    ParticleList_t particles;

public:
    Event() {}
    virtual ~Event() {}

    const TrackList_t& Tracks() const { return tracks; }
          TrackList_t& Tracks()       { return tracks; }

    const ParticleList_t& Particles() const { return particles; }
          ParticleList_t& Particles()       { return particles; }

};
}

#endif
