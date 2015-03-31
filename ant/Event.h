#ifndef EVENT_H
#define EVENT_H

#include "base/types.h"

#include "Particle.h"
#include "Track.h"
#include "TaggerHit.h"
#include "TriggerInfo.h"
#include "base/printable.h"

#include <vector>
#include <memory>
#include <map>

namespace ant {


class Event: public ant::printable_traits {
public:
    class Data: public ant::printable_traits {
    public:

        class PTypeLists: public ant::printable_traits {
        protected:
            ant::ParticleList particles;
            std::map<const ParticleTypeDatabase::Type*, ant::ParticleList> lists;

            static const ant::ParticleList empty;

        public:
            void AddParticle(ParticlePtr&& particle) {
                lists[&particle->Type()].emplace_back(particle);
            }


            const ParticleList& GetAll() const { return particles; }

            const ParticleList& Get(const ParticleTypeDatabase::Type& type) const {
                auto entry = lists.find(&type);
                if(entry == lists.end()) {
                    return empty;
                }
                return entry->second;
            }

            std::ostream &Print(std::ostream &stream) const;
        };


        PTypeLists   particles;      //final state / reconstructred particles
        PTypeLists   intermediates;  //intermediate particles (if any)

        ant::TrackList      tracks;         //detector tracks (if any)
        ant::TaggerHistList taggerhits;     //tagger hits
        ant::TriggerInfo    triggerinfo;

    public:

        const PTypeLists& Particles() const { return particles; }
              PTypeLists& Particles()       { return particles; }

        const PTypeLists& Intermediates() const { return intermediates; }
              PTypeLists& Intermediates()       { return intermediates; }

        const TrackList& Tracks() const { return tracks; }
              TrackList& Tracks()       { return tracks; }

        const TaggerHistList& TaggerHits() const { return taggerhits; }
              TaggerHistList& TaggerHits()       { return taggerhits; }

        const TriggerInfo& TriggerInfos() const { return triggerinfo; }
              TriggerInfo& TriggerInfos()       { return triggerinfo; }

              std::ostream &Print(std::ostream &stream) const;
    };

protected:
    Data    reconstructed;
    Data    mctrue;
public:
    const Data& Reconstructed() const { return reconstructed; }
          Data& Reconstructed()       { return reconstructed; }

    const Data& MCTrue() const { return mctrue; }
          Data& MCTrue()       { return mctrue; }

    std::ostream &Print(std::ostream &stream) const;
};

/*

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
    refMCParticleList_t ref_mctruefinalstate;

    TaggerHitList_t     taggerhits;
    refTaggerHitList_t  ref_taggerhits;

    std::map<refType, refRecParticleList_t> particles_by_type;

    TriggerInfo     trigger;

public:
    Event() {}
    virtual ~Event() {}

    const refTrackList_t&       Tracks()     const { return ref_tracks; }
    const refRecParticleList_t& Particles()  const { return ref_particles; }
    const refMCParticleList_t&  MCTrue()     const { return ref_mctrue; }
    const refMCParticleList_t&  MCTrueFinalState() const { return ref_mctruefinalstate; }
    const refTaggerHitList_t&   TaggerHits() const { return ref_taggerhits; }

    const refRecParticleList_t ParticleType( const ParticleTypeDatabase::Type& type) const {
        auto entry = particles_by_type.find(&type);
        if(entry!=particles_by_type.cend()) {
            return entry->second;
        } else {
            return std::move(refRecParticleList_t());
        }

    }

    TrackList_t&        TrackStorage()      { return tracks; }
    RecParticleList_t&  ParticleStorage()   { return particles; }
    MCParticleList_t&   MCTrueStorage()            { return mctrue; }
    TaggerHitList_t&    TaggerHitStorage()        { return taggerhits; }

    const TriggerInfo&  Trigger() const { return trigger; }
    TriggerInfo&  Trigger() { return trigger; }

    void Finalize();


    virtual std::ostream &Print(std::ostream &stream) const;
};
*/
}

#endif
