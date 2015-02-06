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

    typedef std::shared_ptr<const ant::RecParticle>    sRecParticlePtr;
    typedef std::shared_ptr<const ant::MCParticle>    sMCParticlePtr;
    typedef std::shared_ptr<const ant::Track>       sTrackPtr;
    typedef std::shared_ptr<const ant::TaggerHit>   sTaggerHitPtr;

    typedef std::vector<sRecParticlePtr>   RecParticleList_t;
    typedef std::vector<sMCParticlePtr>   MCParticleList_t;
    typedef std::vector<sTrackPtr>      TrackList_t;
    typedef std::vector<sTaggerHitPtr>     TaggerHitList_t;

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
