#include "Particle.h"
#include "TMath.h"

using namespace ant;

Particle::Particle(const ParticleTypeDatabase::Type &_type, mev_t Ek, radiant_t theta, radiant_t phi):
    type(_type)
{
    const mev_t E = Ek + type->Mass();
    const mev_t p = sqrt( E*E - m*m );

    TVector3 pv(1,0,0);

    pv.SetMagThetaPhi(p,theta,phi);

    TLorentzVector(pv, E);
}

Particle::Particle(const ParticleTypeDatabase::Type &_type, const TLorentzVector &_lorentzvector):
    TLorentzVector(_lorentzvector),
    type(_type)
{
}


std::ostream operator<<(std::ostream &stream, const Particle &particle)
{
    stream << "Particle(" << particle.Type().Name() << " IM=" << particle.M() << ")";
    return stream;
}


RecParticle::RecParticle(const ParticleTypeDatabase::Type &_type, ant::Track &_track)
{

}
