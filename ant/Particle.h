#ifndef ANT_PARTICLE_H
#define ANT_PARTICLE_H

#include "types.h"
#include "ParticleType.h"
#include "Track.h"

#include "TLorentzVector.h"
#include <list>
#include <ostream>

namespace ant {

class Particle: public TLorentzVector {
public:
    typedef std::list<const Particle*> ParticleList_t;

protected:
    ParticleList_t daughters;
    const Particle* parent;
    const ant::ParticleTypeDatabase::Type* type;

public:

    Particle(const ant::ParticleTypeDatabase::Type& _type, ant::mev_t _Ek, ant::radian_t _theta, ant::radian_t _phi);

    Particle(const ParticleTypeDatabase::Type &_type, const TLorentzVector &_lorentzvector):
        TLorentzVector(_lorentzvector),
        type(&_type)
    {}

    virtual ~Particle() {}

    void ChangeType( const ParticleTypeDatabase::Type& type );

    const ParticleTypeDatabase::Type& Type() const { return *type; }

    const Particle* Partent()           const { return parent; }
    const ParticleList_t& Daughters()   const { return daughters; }

    bool hasParent() const { return parent != nullptr; }
    bool hasDaughters() const { return daughters.size() != 0; }

    void SetParent(const Particle* particle) { parent = particle; }
    void AddDaughter(const Particle* particle) { daughters.push_back(particle); }

    void SetLorentzVector( const TLorentzVector& lv ) {
        *((TLorentzVector*)this) = lv;
    }
};


/**
 * @brief Particle class fpr Monte Carlo true data
 *
 * This is the same as Particle for the moment. Can be extended as needed.
 */
class MCParticle: public Particle {
public:
    MCParticle(const ant::ParticleTypeDatabase::Type& _type, ant::mev_t _Ek, ant::radian_t _theta, ant::radian_t _phi):
        Particle(_type, _Ek, _theta, _phi )
    {}

    MCParticle(const ParticleTypeDatabase::Type& _type, const TLorentzVector& _lorentzvector):
        Particle(_type, _lorentzvector)
    {}

    virtual ~MCParticle() {}
};



/**
 * @brief Particle class for reconstructed particles.
 *
 * Stores additional track and detector inforamtion
 */

class RecParticle: public Particle {
protected:
    const ant::Track& track;

public:
    RecParticle(const ParticleTypeDatabase::Type& _type, const ant::Track& _track):
        Particle(_type, _track.ClusterEnergy(), _track.Theta(), _track.Phi()),
        track(_track)
    {}

    RecParticle(const ant::Particle& _particle, const ant::Track& _track):
        Particle(_particle),
        track(_track)
    {}

    const ant::Track& Track() const { return track; }

};

}

std::ostream& operator<< (std::ostream& stream, const ant::Particle& particle);
#endif
