#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include "Particle.h"
#include "Track.h"
#include "printable.h"

#include <vector>
#include <memory>

namespace ant {

class Event: public ant::printable_traits {
public:

    typedef std::shared_ptr<const ant::Particle>    sParticlePtr;
    typedef std::shared_ptr<const ant::Track>       sTrackPtr;

    typedef std::vector<sParticlePtr>   ParticleList_t;
    typedef std::vector<sTrackPtr>      TrackList_t;

protected:

    TrackList_t tracks;
    ParticleList_t particles;
    ParticleList_t mctrue;

public:
    Event() {}
    virtual ~Event() {}

    const TrackList_t& Tracks() const { return tracks; }
          TrackList_t& Tracks()       { return tracks; }

    const ParticleList_t& Particles() const { return particles; }
          ParticleList_t& Particles()       { return particles; }

    const ParticleList_t& MCTrue() const { return mctrue; }
          ParticleList_t& MCTrue()       { return mctrue; }


    virtual std::ostream &Print(std::ostream &stream) const;
};
}

#endif
