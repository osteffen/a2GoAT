#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <memory>
#include <stdexcept>

#include "GTreeManager.h"
#include "GTree.h"
#include "GTreeTrack.h"
#include "GTreeParticle.h"
#include "GTreeMeson.h"
#include "GTreeDetectorHits.h"
#include "GTreeEventParameters.h"
#include "GTreeA2Geant.h"
#include "GTreePluto.h"

#include "ParticleType.h"
#include "Particle.h"

#include "Event.h"

namespace ant {
class EventManager: public GTreeManager {
public:
    typedef std::unique_ptr<const ant::Particle>    uParticlePtr;
    typedef std::unique_ptr<const ant::Track>       uTrackPtr;

protected:

    virtual Bool_t  Start();
    virtual void    ProcessEvent();
    virtual void	ProcessScalerRead();
    virtual Bool_t    Write();

    typedef std::list<Particle> ParticleList_t;
    typedef std::list<Track> TrackList_t;

    void CopyParticles(GTreeParticle* tree, const ant::ParticleTypeDatabase::Type& type, ant::Event& target);
    void CopyTracks(GTreeTrack* tree, Event::TrackList_t & container);
    void CopyPlutoParticles(GTreePluto* tree, ant::Event::ParticleList_t& container);

    uTrackPtr GetTrack(GTreeTrack* tree, const UInt_t n);

    uTrackPtr GetPlutoParticle(GTreePluto* tree, const UInt_t n);

public:
    EventManager();
    virtual ~EventManager();
    virtual Bool_t  Init(const char* configfile);

};
}

#endif
