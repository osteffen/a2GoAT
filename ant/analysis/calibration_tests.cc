#include "analysis/calibration_tests.h"
#include "Track.h"
#include "plot/root_draw.h"
#include "TTree.h"
#include <iostream>

using namespace std;
using namespace ant;

ant::analysis::TAPSCalibrationTest::TAPSCalibrationTest(const string &name):
    Physics(name)
{
    ggIM_1taps = HistFac.makeHist<TLorentzVector>([] (const TLorentzVector& l) { return l.M();}, "2 #gamma 1 TAPS", "m_{#gamma #gamma} [MeV]","", BinSettings(1000),"ggIM1taps");

    ggIM_mult = HistFac.makeTH2D("2 #gamma IM (1TAPS) over neutral multiplicity","m_{#gamma #gamma} [MeV]","# neutral particles",BinSettings(1000),BinSettings(8,2,10));

    HistFac.EnterDirectory();
    tree = new TTree("imvalues","");
    HistFac.LeaveDirectory();

    tree->Branch("M", &branchM,"Invariant Mass/D");
//    tree->Branch("element", &branchElement,"Central Element/I");
    tree->Branch("theta", &branch_theta,"Theta/D");
    tree->Branch("time", &branch_time,"Time/D");
    tree->Branch("nmult", &branch_nmult,"Neutral Multiplicity/I");
}

void ant::analysis::TAPSCalibrationTest::ProcessEvent(const ant::Event &event)
{

    TrackList neutrals;
    //neutrals.reserve(10);
    for( auto& t : event.Reconstructed().Tracks() ) {
        if(t->VetoEnergy() == 0)
            neutrals.emplace_back(t);
    }

    if(neutrals.size() < 2)
        return;

//    cout << event.Reconstructed().Tracks().size() << " " << neutrals.size() << endl;

    for( auto t1 = neutrals.begin(); t1 != neutrals.end(); ++t1 ) {

        const Track& track1 = **t1;

        const Particle gamma1(ParticleTypeDatabase::Photon, *t1);

        for( auto t2 = t1; t2 != neutrals.end(); ++t2 ) {

            const Track& track2 = **t2;

            const Particle gamma2(ParticleTypeDatabase::Photon, *t2);

            if( (track1.Detector() & detector_t::anyTAPS) ^ (track2.Detector() & detector_t::anyTAPS)) {
                TLorentzVector m = TLorentzVector(gamma1)+TLorentzVector(gamma2);
                ggIM_1taps.Fill(m);
                ggIM_mult->Fill(m.M(),neutrals.size());


                const Track& tapstrack = (track1.Detector() & detector_t::anyTAPS) ? track1 : track2;

                branchM = m.M();
                branch_theta = tapstrack.Theta()*TMath::RadToDeg();
                branch_time = tapstrack.Time();
                branch_nmult = neutrals.size();
                tree->Fill();

            }

        }

    }

}

void ant::analysis::TAPSCalibrationTest::Finish()
{
    tree->Write();
}

void ant::analysis::TAPSCalibrationTest::ShowResult()
{    canvas("TAPSCalibrationTest")
            << ggIM_1taps
            << drawoption("colz") << ggIM_mult
            << endc;
}
