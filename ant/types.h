#ifndef ANT_TYPES_H
#define ANT_TYPES_H

#include <ostream>

namespace ant {

typedef double radian_t;
typedef double mev_t;
typedef double ns_t;
typedef unsigned int index_t;
typedef unsigned int clustersize_t;
typedef int element_index_t;

// strongly typed enum, C++11 feature
// in combination with usual boolean operations


enum class detector_t {
    CB,
    TAPS,
    CBTAPS
};

}

std::ostream& operator<<(std::ostream &stream, const ant::detector_t& app);

#endif
