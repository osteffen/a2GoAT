#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

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

namespace ant {
class EventManager: public GTreeManager {

protected:

    virtual Bool_t  Start();
    virtual void    ProcessEvent();
    virtual void	ProcessScalerRead();
    virtual Bool_t    Write();

    typedef std::list<Particle> ParticleList_t;
    typedef std::list<Track> TrackList_t;

    void CopyParticles(GTreeParticle* tree, const ant::ParticleTypeDatabase::Type& type, ParticleList_t& container);
    void CopyTracks(GTreeTrack* tree, TrackList_t& container);

public:
    EventManager();
    virtual ~EventManager();
    virtual Bool_t  Init(const char* configfile);

};
}

#endif
