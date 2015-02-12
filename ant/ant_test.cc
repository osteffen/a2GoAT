#include <iostream>
#include <typeinfo>
#include <vector>

#include "types.h"
#include "ParticleType.h"
#include "Track.h"
#include "Particle.h"

#include "combinatorics.h"

#include "interval.h"

#include "plotter.h"

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
            detector_t::CB,
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

    cout << "=== combinatorics test ===" <<endl;

    vector<int> numbers = {1,2,3,4,5};

    KofNvector<int> combinations(numbers,4);
    do {

        for( auto& i: combinations ) {
            cout << i << " ";
        }

        cout << endl;

    } while(combinations.next());

    cout << "----" << endl;
    vector<int> numbers2 = {1};

    KofNvector<int> combinations2(numbers2,2);
    do {

        for( auto& i: combinations2 ) {
            cout << i << " ";
        }

        cout << endl;

    } while(combinations2.next());

    interval<double> i;
    cout << i << endl;

    i.SetWidth(5);
    cout << i << endl;

    i.SetCenter(2.5);
    cout << i << endl;

    cout << i.Center() << " " << i.Length() << endl;

    cout << "========================" << endl;



    return 0;


}
