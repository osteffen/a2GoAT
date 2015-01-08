#include "fPlutoGoat.h"

#include "GTreeGoatParticle.h"
#include "HistFiller.h"
#include "HistogramFactory.h"
#include "PParticle.h"
#include <iostream>
#include "TMath.h"
using namespace std;

typedef const std::pair<const GParticle*, const PParticle*> GoatPlutoPair;

struct fGoatID {
    typedef GoatPlutoPair DataType;
    Int_t operator() ( DataType data ) {
        return data.first->ID();
    }
};

struct fPlutoID {
    typedef GoatPlutoPair DataType;
    Int_t operator() ( DataType data ) {
        return data.second->ID();
    }
};

// Decide if CB (0) or TAPS (1) depending on theta angle of pluto particle
struct fPlutoApparatus {
    typedef GoatPlutoPair DataType;
    Int_t operator() ( DataType data ) {
        return (data.second->Theta() < 20.0*TMath::DegToRad()) ? 1 : 0;
    }
};

class fPlutoGoat::ReconstructAs {
public:
    typedef TH1Filler<fGoatID> GoatIDHist;
    GoatIDHist hProton;
    GoatIDHist hGamma;
    GoatIDHist hPion;
    GoatIDHist hElectron;
    FillDecision<fPlutoID> PlutoIDDec;

    ReconstructAs(const std::string& prefix):
        hProton(prefix+" true proton goes to", "Goat Particle ID", "", 6,-1,5),
        hGamma(prefix+" true photon goes to", "Goat Particle ID", "", 6,-1,5),
        hPion(prefix+" true pion goes to", "Goat Particle ID", "", 6,-1,5),
        hElectron(prefix+" true electron goes to", "Goat Particle ID", "", 6,-1,5)
    {
        PlutoIDDec.setBranch(1,&hGamma);
        PlutoIDDec.setBranch(14,&hProton);
        PlutoIDDec.setBranch(8,&hPion);
        PlutoIDDec.setBranch(9,&hPion);
        PlutoIDDec.setBranch(2,&hElectron);
        PlutoIDDec.setBranch(3,&hElectron);

        GParticleDatabase::SetBinNames(hGamma.GetHist());
        GParticleDatabase::SetBinNames(hProton.GetHist());
        GParticleDatabase::SetBinNames(hPion.GetHist());
        GParticleDatabase::SetBinNames(hElectron.GetHist());

    }

    FillDecision<fPlutoID>* Node() { return &PlutoIDDec; }

};

class fPlutoGoat::RootFillNode: public FillNode<GoatPlutoPair> {
public:
    FillDecision<fPlutoApparatus> dApp;
    fPlutoGoat::ReconstructAs cb;
    fPlutoGoat::ReconstructAs taps;

    RootFillNode():
        cb("CB:"),
        taps("TAPS:")
    {
        dApp.setBranch(0,cb.Node());
        dApp.setBranch(1,taps.Node());
    }
    virtual ~RootFillNode() {}
    virtual void Fill(GoatPlutoPair data) {
        dApp.Fill(data);
    }

    virtual void Draw(TVirtualPad* pad=nullptr) {
        dApp.Draw(pad);
    }
};

fPlutoGoat::fPlutoGoat()
{
    root_node = new RootFillNode();
}

fPlutoGoat::~fPlutoGoat()
{
    delete root_node;
}

void fPlutoGoat::Run(const fPhysics &p)
{

    const GTreePluto::ParticleList pps = p.Pluto().GetFinalState();
    const GGoatParticleManager::ParticleList& gps= p.Goat().Particles();

    if( pps.size() == 1) {

        const PParticle* pp= (pps.front());

        if( gps.size() != 0) {
           for( auto gp=gps.begin(); gp!= gps.end(); ++gp) {
               root_node->Fill(GoatPlutoPair( *gp, pp ));
           }

        } else {
               root_node->Fill( GoatPlutoPair( &GParticleDatabase::GFail, pp ));
        }

    }

}

void fPlutoGoat::Display()
{
    root_node->Draw();
}
