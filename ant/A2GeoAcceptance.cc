#include "A2GeoAcceptance.h"

ant::A2SimpleGeometry::A2SimpleGeometry():
    cb_region(21.0*TMath::DegToRad(), 160.0*TMath::RadToDeg()),
    equator_region(-2.0*TMath::DegToRad(),+2.0*TMath::DegToRad()),
    taps_region(2.0*TMath::DegToRad(),20.0*TMath::DegToRad())
{

}

ant::detector_t ant::A2SimpleGeometry::DetectorFromAngles(const ant::radian_t theta, const ant::radian_t phi) const
{
    if( cb_region.Contains(theta) ) {

        if(!equator_region.Contains(phi)) {
            return detector_t::NaI;
        }

    } else if( taps_region.Contains(theta)) {
        return detector_t::BaF2 | detector_t::PbWO4;
    }

    return detector_t::None;

}
