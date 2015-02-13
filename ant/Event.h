#ifndef EVENT_H
#define EVENT_H

#include "types.h"

#include "Particle.h"
#include "Track.h"
#include "TaggerHit.h"

#include "printable.h"

#include <vector>
#include <memory>

namespace ant {

class Event: public ant::printable_traits {
public:

    typedef std::vector<ant::RecParticle>   RecParticleList_t;
    typedef std::vector<ant::MCParticle>    MCParticleList_t;
    typedef std::vector<ant::Track>         TrackList_t;
    typedef std::vector<ant::TaggerHit>     TaggerHitList_t;

protected:

    TrackList_t tracks;
    RecParticleList_t particles;
    MCParticleList_t mctrue;
    TaggerHitList_t taggerhits;


public:
    Event() {}
    virtual ~Event() {}

    const TrackList_t& Tracks() const { return tracks; }
    TrackList_t& Tracks()       { return tracks; }

    const RecParticleList_t& Particles() const { return particles; }
    RecParticleList_t& Particles()       { return particles; }

    const MCParticleList_t& MCTrue() const { return mctrue; }
    MCParticleList_t& MCTrue()       { return mctrue; }

    const TaggerHitList_t& TaggerHits() const { return taggerhits; }
    TaggerHitList_t& TaggerHits()       { return taggerhits; }


    virtual std::ostream &Print(std::ostream &stream) const;
};
}

#endif
