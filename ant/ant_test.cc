#include <iostream>
#include <typeinfo>

#include "types.h"
#include "ParticleType.h"
#include "Track.h"
#include "Particle.h"

using namespace std;
using namespace ant;

int main() {

    ParticleTypeDatabase::Print();

    for( ParticleTypeDatabase::const_iterator a = ParticleTypeDatabase::begin(); a!=ParticleTypeDatabase::end(); a++) {
        cout << *a << endl;
    }

    for( auto& a : ParticleTypeDatabase() ) {
        cout << endl;
        for( auto& b : ParticleTypeDatabase() ) {
            cout << a << " " << b << "    " << (a == b) << endl;
        }
    }

    Track t(mev_t(100),
            radian_t(2.0),
            radian_t(1.0),
            time_t(0.324),
            clustersize_t(4),
            apparatus_t::CB,
            mev_t(0.4),
            mev_t(0.3),
            mev_t(0.2)
            );
    cout << t << endl;

    RecParticle g(ParticleTypeDatabase::Photon, t);
    cout << g << endl;
    g.ChangeType(ParticleTypeDatabase::Proton);
    cout << g << endl;
    g.ChangeType(ParticleTypeDatabase::PiCharged);
    cout << g << endl;

    return 0;


}
