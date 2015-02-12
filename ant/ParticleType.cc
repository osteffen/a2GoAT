#include "ParticleType.h"
#include <iostream>

using namespace std;
using namespace ant;

ostream& operator<<(ostream &stream, const ParticleTypeDatabase::Type& particle_type)
{
    stream << "ParticleType " << particle_type.Name() << ":";
    stream << "\tMass=" << particle_type.Mass();
    stream << "\t"  << (particle_type.Charged() ? "Charged" : "Neutral");
    return stream;
}


ParticleTypeDatabase::Particles_t ParticleTypeDatabase::types;

const ParticleTypeDatabase::Type ParticleTypeDatabase::Proton("Proton",               "p",            938.272046, true);
const ParticleTypeDatabase::Type ParticleTypeDatabase::Neutron("Neutron",             "n",            939.565378, false);
const ParticleTypeDatabase::Type ParticleTypeDatabase::Photon("Photon",               "#gamma",       0.0,        false);

const ParticleTypeDatabase::Type ParticleTypeDatabase::Pi0("Pi0",                     "#pi^{0}",      134.9766,  false);
const ParticleTypeDatabase::Type ParticleTypeDatabase::PiCharged("PiCharged",         "#pi^{#pm}",    139.57018, true);
const ParticleTypeDatabase::Type ParticleTypeDatabase::PiPlus("PiPlus",               "#pi^{+}",      139.57018, true, &ParticleTypeDatabase::PiCharged);
const ParticleTypeDatabase::Type ParticleTypeDatabase::PiMinus("PiMinus",             "#pi^{-}",      139.57018, true, &ParticleTypeDatabase::PiCharged);

const ParticleTypeDatabase::Type ParticleTypeDatabase::eCharged("eCharged",           "e^{#pm}",      0.510998928, true);
const ParticleTypeDatabase::Type ParticleTypeDatabase::ePlus("Positron",              "e^{+}",        0.510998928, true, &ParticleTypeDatabase::eCharged);
const ParticleTypeDatabase::Type ParticleTypeDatabase::eMinus("Electron",             "e^{-}",        0.510998928, true, &ParticleTypeDatabase::eCharged);

ParticleTypeDatabase::Type::Type(const string &_name, const string &_print_name, const mev_t &_mass, const bool &_charged, const ParticleTypeDatabase::Type *_sametype):
        name(_name),
        print_name(_print_name),
        mass(_mass),
        charged(_charged),
        sametype(_sametype)
    {
        types.insert( pair<string, const Type&>(_name,*this) );
    }

void ParticleTypeDatabase::Print()
{
    for(auto& p : types) {
        cout << (p.second) << endl;
    }
}

const std::vector<const ParticleTypeDatabase::Type*> ParticleTypeDatabase::detectables = { &ParticleTypeDatabase::Photon,
                                                                                           &ParticleTypeDatabase::Proton,
                                                                                           &ParticleTypeDatabase::PiCharged,
                                                                                           &ParticleTypeDatabase::eCharged
                                                                                         };

const std::vector<const ParticleTypeDatabase::Type*> ParticleTypeDatabase::mc_finalstate = { &ParticleTypeDatabase::Photon,
                                                                                             &ParticleTypeDatabase::Proton,
                                                                                             &ParticleTypeDatabase::PiMinus,
                                                                                             &ParticleTypeDatabase::PiPlus,
                                                                                             &ParticleTypeDatabase::eMinus,
                                                                                             &ParticleTypeDatabase::ePlus
                                                                                           };
