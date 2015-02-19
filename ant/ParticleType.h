#ifndef ANT_PARTICLETYPES_H
#define ANT_PARTICLETYPES_H

#include "types.h"

#include <string>
#include <ostream>
#include <map>
#include <iterator>
#include <vector>

namespace ant {

class ParticleTypeDatabase {
public:

    class Type {

    friend class ParticleTypeDatabase;

    protected:
        std::string name;
        std::string print_name;
        mev_t mass;
        bool charged;
        const Type* sametype;

        Type( const Type& p ) {}
        Type( const std::string& _name, const std::string& _print_name, const mev_t& _mass, const bool& _charged, const Type* _sametype=nullptr );

    public:
        virtual ~Type() {}

        const std::string& PrintName()  const { return print_name; }
        const std::string& Name()       const { return name; }
        mev_t Mass()                    const { return mass; }
        bool  Charged()                 const { return charged; }

        virtual bool operator== (const Type& rhs) const {
            if( this == &rhs )
                return true;
            if( sametype == &rhs )
                return true;
            if( rhs.sametype == this )
                return true;
            return false;
        }
    };



protected:

    typedef std::map<std::string, const Type&> Particles_t;
    static Particles_t types;

    static const std::vector<const Type*> detectables;
    static const std::vector<const Type*> mc_finalstate;

public:

    ParticleTypeDatabase() {}

    virtual ~ParticleTypeDatabase() {}

    static void Print();

    static const Type Proton;
    static const Type Neutron;
    static const Type Photon;
    static const Type Pi0;
    static const Type PiPlus;
    static const Type PiMinus;
    static const Type PiCharged;
    static const Type ePlus;
    static const Type eMinus;
    static const Type eCharged;

    static const Type Eta;
    static const Type Omega;
    static const Type EtaPrime;

    static const Particles_t& GetParticles() { return types; }

    class const_iterator : public Particles_t::const_iterator {
    public:

      const_iterator(const Particles_t::const_iterator& i):Particles_t::const_iterator(i)  {}

      const Type& operator*() const { return Particles_t::const_iterator::operator*().second; }

    };

    static const_iterator begin() { return const_iterator(types.begin()); }
    static const_iterator end()   { return const_iterator(types.end()); }

    static const std::vector<const Type*>& DetectableTypes() { return detectables; }
    static const std::vector<const Type*>& MCFinalStateTypes() { return mc_finalstate; }


};

}

std::ostream& operator<< ( std::ostream& stream, const ant::ParticleTypeDatabase::Type& particle_type );

#endif
