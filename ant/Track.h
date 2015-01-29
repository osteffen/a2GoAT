#ifndef TRACK_H
#define TRACK_H

#include <ostream>
#include "types.h"

namespace ant {

class Track
{
private:
    mev_t clusterEnergy;
    radian_t theta;
    radian_t phi;
    ns_t time;
    clustersize_t clusterSize;
    element_index_t centralCrystal;
    element_index_t centralVeto;
    apparatus_t apparatus;
    mev_t vetoEnergy;
    mev_t _MWPC0Energy;
    mev_t _MWPC1Energy;

public:
    Track(const mev_t& _clusterEnergy,
          const radian_t& _theta,
          const radian_t& _phi,
          const ns_t& _time,
          const clustersize_t& _clusterSize,
          const element_index_t& _centralCrystal,
          const element_index_t& _centralVeto,
          const apparatus_t& _apparatus,
          const mev_t& _vetoEnergy,
          const mev_t& _MWPC0Energy,
          const mev_t& _MWPC1Energy
          ) :
        clusterEnergy(_clusterEnergy),
        theta(_theta),
        phi(_phi),
        time(_time),
        clusterSize(_clusterSize),
        centralCrystal(_centralCrystal),
        centralVeto(_centralVeto),
        apparatus(_apparatus),
        vetoEnergy(_vetoEnergy),
        _MWPC0Energy(_MWPC0Energy),
        _MWPC1Energy(_MWPC1Energy)
    {}


    mev_t ClusterEnergy() const { return clusterEnergy; }
    radian_t Theta() const { return theta; }
    radian_t Phi() const { return phi; }
    ns_t Time() const { return time; }
    clustersize_t ClusterSize() const { return clusterSize; }
    element_index_t CentralCrystal() const { return centralCrystal; }
    element_index_t CentralVeto() const { return centralVeto; }
    apparatus_t Apparatus() const { return apparatus; }
    mev_t VetoEnergy() const { return vetoEnergy; }
    mev_t MWPC0Energy() const { return _MWPC0Energy; }
    mev_t MWPC1Energy() const { return _MWPC1Energy; }

};

}

std::ostream& operator<< ( std::ostream& stream, const ant::Track& t );

#endif // TRACK_H
