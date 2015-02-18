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

typedef const ant::Particle*     refPartcile;
typedef const ant::MCParticle*   refMCParticle;
typedef const ant::RecParticle*  refRecParticle;
typedef const ant::Track*        refTrack;
typedef const ant::TaggerHit*    refTaggerHit;

typedef std::vector< refPartcile >      refParticleList_t;
typedef std::vector< refMCParticle >    refMCParticleList_t;
typedef std::vector< refRecParticle >   refRecParticleList_t;
typedef std::vector< refTrack >         refTrackList_t;
typedef std::vector< refTaggerHit >     refTaggerHitList_t;

typedef const ant::ParticleTypeDatabase::Type*   refType;

class Event: public ant::printable_traits {
public:
    typedef std::vector< ant::Track >       TrackList_t;
    typedef std::vector< ant::RecParticle > RecParticleList_t;
    typedef std::vector< ant::MCParticle >  MCParticleList_t;
    typedef std::vector< ant::TaggerHit >   TaggerHitList_t;

protected:

    TrackList_t         tracks;
    refTrackList_t      ref_tracks;

    RecParticleList_t       particles;
    refRecParticleList_t    ref_particles;

    MCParticleList_t    mctrue;
    refMCParticleList_t ref_mctrue;

    TaggerHitList_t     taggerhits;
    refTaggerHitList_t  ref_taggerhits;

    std::map<refType, refRecParticleList_t> particles_by_type;

public:
    Event() {}
    virtual ~Event() {}

    const refTrackList_t&       Tracks()     const { return ref_tracks; }
    const refRecParticleList_t& Particles()  const { return ref_particles; }
    const refMCParticleList_t&  MCTrue()     const { return ref_mctrue; }
    const refTaggerHitList_t&   TaggerHits() const { return ref_taggerhits; }
    const refRecParticleList_t& ParticleType( const ParticleTypeDatabase::Type& type) const {
        try {
            return particles_by_type.at(&type);
        } catch(...) {
            return std::move(refRecParticleList_t());
        }
        }

    TrackList_t&        TrackStorage()      { return tracks; }
    RecParticleList_t&  ParticleStorage()   { return particles; }
    MCParticleList_t&   MCTrueStorage()            { return mctrue; }
    TaggerHitList_t&    TaggerHitStorage()        { return taggerhits; }

    void Finalize();


    virtual std::ostream &Print(std::ostream &stream) const;
};
}

#endif
