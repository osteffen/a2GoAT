#ifndef CALIBRATION_TESTS_H
#define CALIBRATION_TESTS_H

#include "AntPhysics.h"
#include "plot/SmartHist.h"

#include <string>

class TTree;

namespace ant {
namespace analysis {

class TAPSCalibrationTest : public ant::Physics {
protected:
    SmartHist1<TLorentzVector> ggIM_1taps;

    TH2D* ggIM_mult;

    TTree* tree;
    double branchM;
    int branchElement;
    double branch_theta;
    double branch_time;
    int branch_nmult;


public:
    TAPSCalibrationTest(const std::string& name="TAPSCalibrationTest");

    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};

}
}

#endif
