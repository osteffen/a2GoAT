#ifndef GTREEGOATPARTICLE_H
#define GTREEGOATPARTICLE_H

#include "Rtypes.h"
#include "TLorentzVector.h"
#include <list>
#include <map>

#include "GParticle.h"

class GTreeParticle;

class GGoatParticleManager {
public:
    typedef std::list<const GParticle*> ParticleList;

protected:

    struct ParticleType {
        Int_t id;
        Double_t mass;
        bool ischagred;
    };

    std::map<const GTreeParticle*, ParticleType> trees;

    ParticleList particles;

public:
    GGoatParticleManager();

    virtual ~GGoatParticleManager();

    virtual void Clear();

    virtual const ParticleList& Particles() const;
    virtual void CollectParticles();

    virtual void AddTree( const GTreeParticle* tree, Int_t particle_id, const Double_t particle_mass, const bool charged );

};

#endif
