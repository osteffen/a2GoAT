#ifndef ANT_TYPES_H
#define ANT_TYPES_H

#include <ostream>

namespace ant {

typedef double radian_t;
typedef double mev_t;
typedef double ns_t;
typedef unsigned int index_t;
typedef unsigned int clustersize_t;

// strongly typed enum, C++11 feature
enum class apparatus_t {
    CB,
    TAPS
};

}

std::ostream& operator<<(std::ostream &stream, const ant::apparatus_t& app);

#endif
