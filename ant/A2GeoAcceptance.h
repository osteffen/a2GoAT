#ifndef A2GEOACCEPTANCE_H
#define A2GEOTOOLS_H

#include "interval.h"
#include "TMath.h"
#include "Detector.h"

namespace ant {

class A2SimpleGeometry {
protected:
    IntervalD cb_region;        // theta
    IntervalD equator_region;   // phi
    IntervalD taps_region;      // theta

public:
    A2SimpleGeometry();

    detector_t DetectorFromAngles( const radian_t theta, const radian_t phi) const;

    template <class T>
    detector_t DetectorFromAngles( const T& v) const {
        return DetectorFromAngles(v.Theta(), v.Phi());
    }

};

}
#endif
