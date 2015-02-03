#include "Particle.h"
#include "TMath.h"

template <class T>
T square(const T& a) { return a*a; }

using namespace ant;

Particle::Particle(const ParticleTypeDatabase::Type &_type, mev_t _Ek, radian_t _theta, radian_t _phi):
    type(&_type)
{
    const mev_t E = _Ek + type->Mass();
    const mev_t p = sqrt( square(E) - square(type->Mass()) );

    //TODO: fix. This might be inefficeint...

    TVector3 pv(1,0,0);

    pv.SetMagThetaPhi(p,_theta,_phi);

    SetLorentzVector(TLorentzVector(pv, E));
}

void Particle::ChangeType(const ParticleTypeDatabase::Type &newtype)
{
    const mev_t newE = Ek() + newtype.Mass();
    const mev_t newP = sqrt( square(newE) - square(newtype.Mass()) );

    TVector3 pv = Vect();
    pv.SetMag(newP);

    SetVect(pv);
    SetE(newE);
    type = &newtype;

}

std::ostream &Particle::streamit(std::ostream &stream) const
{
    stream << "Particle" <<Type().Name();
    stream << " IM=" << M();
    stream << " E=" << E();
    stream << " Theta=" << Theta();
    return stream;
}


std::ostream& operator<<(std::ostream &stream, const Particle &particle)
{
    particle.streamit(stream);
    return stream;
}


std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<const Particle> &particle)
{
    stream << *particle;
    return stream;
}


std::ostream &RecParticle::streamit(std::ostream &stream) const
{
    Particle::streamit(stream);
    stream << "\n\t" << Track();
    return stream;
}
