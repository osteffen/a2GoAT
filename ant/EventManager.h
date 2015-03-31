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

#include "AntPhysics.h"
#include <stdexcept>

class PStaticData;

namespace ant {

class data_check_exception : public std::exception {
protected:
    std::string msg;

public:
    data_check_exception(const std::string& message): msg(message) {}
    const char *what() const throw() { return msg.c_str(); }
};


class EventManager: public GTreeManager {
public:
    typedef std::unique_ptr<const ant::Particle>    uParticlePtr;
    typedef std::unique_ptr<const ant::Track>       uTrackPtr;

    typedef std::list<ant::Physics*>                PhysicsList;

protected:

    unsigned int maxevents;

    virtual Bool_t  Start();
    virtual void    ProcessEvent();
    virtual void	ProcessScalerRead();
    virtual Bool_t    Write();

    typedef std::list<Particle> ParticleList_t;
    typedef std::list<Track> TrackList_t;

    PhysicsList physics;
    void RunPhysics(const ant::Event& event);


    void CopyParticles(GTreeParticle* tree, const ant::ParticleTypeDatabase::Type& type, ant::Event& target);
    void CopyTracks(GTreeTrack* tree, Event& event);
    void CopyPlutoParticles(GTreePluto* tree, Event &event);
    void CopyTaggerHits(Event& event);
    void CopyTriggerInfo(GTreeTrigger* tree, Event& event);

    void checkMCIDs();

    PStaticData* pluto_database;

    ant::ParticlePtr GetPlutoParticle(GTreePluto* tree, const UInt_t n);

public:
    EventManager();
    virtual ~EventManager();
    virtual Bool_t  Init(const char* configfile);

    void AddPhysics(ant::Physics* phys) { physics.push_back(phys); }
    void SetMaxEvents(unsigned int max) { maxevents = max; }

    void Finish();

};
}

#endif
