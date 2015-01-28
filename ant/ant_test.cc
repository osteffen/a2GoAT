#include <iostream>
#include <typeinfo>

#include "types.h"

using namespace std;

int main() {

    ant::radiant_t angle = 5;
    ant::mev_t energy = 2.1;

    cout << energy + angle << endl;

    return 0;
}
