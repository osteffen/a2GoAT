#ifndef ANT_TYPES_H
#define ANT_TYPES_H

#include <ostream>
#include <printable.h>


namespace ant {

typedef double radian_t;
typedef double mev_t;
typedef double ns_t;
typedef unsigned int index_t;
typedef unsigned int clustersize_t;
typedef int element_index_t;

// strongly typed enum, C++11 feature
// in combination with usual boolean operations


class detector_t : public printable_traits {
private:
    unsigned int v;
    detector_t(unsigned int _v) : v(_v) {}
public:
    detector_t() : v(0) {}

    const static detector_t None;
    const static detector_t NaI;
    const static detector_t PID;
    const static detector_t MWPC;
    const static detector_t BaF2;
    const static detector_t PbWO4;
    const static detector_t Veto;

    bool operator==(const detector_t& o) {
        return v==o.v;
    }

    bool operator&(const detector_t& o) {
        return v & o.v;
    }

    bool operator<(const detector_t& o) const {
        return v < o.v;
    }

    detector_t& operator|=(const detector_t& o) {
        v |= o.v;
        return *this;
    }



    std::ostream& Print(std::ostream &stream) const;
};

}

#endif
