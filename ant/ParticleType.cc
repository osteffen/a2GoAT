#include "ParticleType.h"
#include <iostream>

using namespace std;


std::ostream& operator<<(std::ostream &stream, const ant::ParticleTypeDatabase::Type& particle_type)
{
    stream << "ParticleType " << particle_type.Name() << ":";
    stream << "\tMass=" << particle_type.Mass();
    stream << "\t"  << (particle_type.Charged() ? "Charged" : "Neutral");
    return stream;
}


ant::ParticleTypeDatabase::Particles_t ant::ParticleTypeDatabase::types;

const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::Proton("Proton",               "p",            938.0,  true);
const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::Photon("Photon",               "#gamma",         0.0,  false);
const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::Pi0("Pion",                    "#pi^{0}",      135.0,  false);

const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::PiCharged("PiCharged",         "#pi^{#pm}",    135.0,  true);
const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::PiPlus("PiPlus",               "#pi^{+}",      135.0,  true, &ant::ParticleTypeDatabase::PiCharged);
const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::PiMinus("PiMinus",             "#pi^{-}",      135.0,  true, &ant::ParticleTypeDatabase::PiCharged);

const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::eCharged("eCharged",           "e^{#pm}",      0.511,  true);
const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::ePlus("Positron",              "e^{+}",        0.511,  true, &ant::ParticleTypeDatabase::eCharged);
const ant::ParticleTypeDatabase::Type ant::ParticleTypeDatabase::eMinus("Electron",             "e^{-}",        0.511,  true, &ant::ParticleTypeDatabase::eCharged);

ant::ParticleTypeDatabase::Type::Type(const string &_name, const string &_print_name, const ant::mev_t &_mass, const bool &_charged, const ant::ParticleTypeDatabase::Type *_sametype):
        name(_name),
        print_name(_print_name),
        mass(_mass),
        charged(_charged),
        sametype(_sametype)
    {
        types.insert( std::pair<std::string, const Type&>(_name,*this) );
    }

void ant::ParticleTypeDatabase::Print()
{
    for(auto& p : types) {
        cout << (p.second) << endl;
    }
}
