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

    Track t(100, 2.0, 1.0, 0.324, 4, 402, 503, apparatus_t::CB, 0.4, 0.3, 0.2);
    cout << t << endl;

    RecParticle g(ParticleTypeDatabase::Photon, t);
    cout << g << endl;
    g.ChangeType(ParticleTypeDatabase::Proton);
    cout << g << endl;
    g.ChangeType(ParticleTypeDatabase::PiCharged);
    cout << g << endl;

    return 0;


}
