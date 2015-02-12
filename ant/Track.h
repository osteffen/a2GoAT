#ifndef TRACK_H
#define TRACK_H

#include <ostream>
#include <memory>
#include "types.h"
#include "Detector.h"
#include "printable.h"

namespace ant {

/**
 * @brief The Track class
 * Representation of GoAT information, with emphasis on
 * physical particle information, not on detector information!
 */
class Track: public ant::printable_traits
{
private:
    detector_t detector;
    radian_t theta;
    radian_t phi;
    ns_t time;
    mev_t clusterEnergy;
    clustersize_t clusterSize;
    mev_t vetoEnergy;
    mev_t _MWPC0Energy;
    mev_t _MWPC1Energy;
public:
    Track(const mev_t& _clusterEnergy,
          const radian_t& _theta,
          const radian_t& _phi,
          const ns_t& _time,
          const clustersize_t& _clusterSize,
          const detector_t& _apparatus,
          const mev_t& _vetoEnergy,
          const mev_t& _MWPC0Energy,
          const mev_t& _MWPC1Energy
          ) :
        clusterEnergy(_clusterEnergy),
        theta(_theta),
        phi(_phi),
        time(_time),
        clusterSize(_clusterSize),
        detector(_apparatus),
        vetoEnergy(_vetoEnergy),
        _MWPC0Energy(_MWPC0Energy),
        _MWPC1Energy(_MWPC1Energy)
    {}


    mev_t ClusterEnergy() const { return clusterEnergy; }
    radian_t Theta() const { return theta; }
    radian_t Phi() const { return phi; }
    ns_t Time() const { return time; }
    clustersize_t ClusterSize() const { return clusterSize; }
    detector_t Detector() const { return detector; }
    mev_t VetoEnergy() const { return vetoEnergy; }
    mev_t MWPC0Energy() const { return _MWPC0Energy; }
    mev_t MWPC1Energy() const { return _MWPC1Energy; }

    virtual std::ostream &Print(std::ostream &stream) const;

};

}

#endif // TRACK_H
