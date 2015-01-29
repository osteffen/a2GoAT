#include <iostream>
#include <typeinfo>

#include "types.h"
#include "ParticleType.h"
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

    return 0;
}
