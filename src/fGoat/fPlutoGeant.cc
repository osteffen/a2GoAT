#include "fPlutoGeant.h"
#include "PParticle.h"
#include "GTreeA2Geant.h"
#include "GTreePluto.h"
#include <list>
#include "HistFiller.h"
#include "HistogramFactory.h"

typedef std::pair<const GTreeA2Geant&, const PParticle&> GeantPlutoPair;

struct fGeantPlutoPID {
    typedef GeantPlutoPair DataType;
    int operator() ( DataType data ) {
        return data.second.ID();
    }
};

struct fGeantPlutoTAPSESum {

    typedef GeantPlutoPair DataType;

    double operator() ( DataType data ) {
        return data.first.GetTAPSESum();
    }
};

struct fGeantPlutoCBESum {

    typedef GeantPlutoPair DataType;

    double operator() ( DataType data ) {
        return data.first.GetCBESum();
    }
};

struct fGeantPlutoVetoESum {

    typedef GeantPlutoPair DataType;

    double operator() ( DataType data ) {
        return data.first.GetTAPSVetoESum();
    }
};

struct fGeantPlutoPIDESum {

    typedef GeantPlutoPair DataType;

    double operator() ( DataType data ) {
        return data.first.GetPIDESum();
    }
};

struct fGeantPlutoEk {

    typedef GeantPlutoPair DataType;

    double operator() ( DataType data ) {
        return data.second.KE();
    }
};

struct fGeantPlutoApparatus {

    typedef GeantPlutoPair DataType;

    int operator() ( DataType data ) {
        const double angle = data.second.Theta() * TMath::RadToDeg();

        if( angle < 20.0 )
            return 2;
        else if( angle < 160.0 )
            return 1;
        else return 0;
    }
};

template <class fESUM, class fVetoESum>
class EnergyHistograms {
public:
    typedef TH2Filler< f2D1< fGeantPlutoEk, fESUM> > EFiller;
    typedef TH2Filler< f2D1< fESUM, fVetoESum> > dEEFiller;
    typedef typename EFiller::DataType Etype;
    typedef FillList< Etype > eList;

    EFiller protonE;
    EFiller gammaE;
    EFiller pionE;
    EFiller electronE;
    dEEFiller protondEE;
    dEEFiller gammadEE;
    dEEFiller piondEE;
    dEEFiller electrondEE;

    eList lproton;
    eList lgamma;
    eList lpion;
    eList lelectron;

    FillDecision<fGeantPlutoPID> branch;


    EnergyHistograms( const std::string& title, const std::string& prefix):
        protonE(NULL),
        gammaE(NULL),
        pionE(NULL),
        electronE(NULL),
        protondEE(NULL),
        gammadEE(NULL),
        piondEE(NULL),
        electrondEE(NULL),
        lproton(prefix+"proton",title+"Pluto/Geant Proton"),
        lgamma(prefix+"gamma",title+"Pluto/Geant #gamma"),
        lpion(prefix+"pion",title+"Pluto/Geant #pi^{#pm}"),
        lelectron(prefix+"electron",title+"Pluto/Geant #e^#{pm}")

    {

        HistogramFactory& hf = *HistogramFactory::GetInstanceFresh();

        protonE.SetHist(hf.Make2DN(
                            title + "Energy Pluto/Geant: p",
                            "E_{Pluto} [GeV]",
                            "E_{Geant} [GeV]",
                            100,0,1,
                            100,0,1 ));
        gammaE.SetHist(hf.Make2DN(
                           title + "Energy Pluto/Geant: #gamma",
                           "E_{Pluto} [GeV]",
                           "E_{Geant} [GeV]",
                           100,0,1,
                           100,0,1 ));
        pionE.SetHist(hf.Make2DN(
                          title + "Energy Pluto/Geant: #pi^{#pm}",
                          "E_{Pluto} [GeV]",
                          "E_{Geant} [GeV]",
                          100,0,1,
                          100,0,1 ));
        electronE.SetHist(hf.Make2DN(
                             title + "Energy Pluto/Geant: #e^{#pm}",
                             "E_{Pluto} [GeV]",
                             "E_{Geant} [GeV]",
                             100,0,1,
                             100,0,1 ));

        protondEE.SetHist(hf.Make2DN(
                                    "dE - E Geant: p",
                                    "E_{ESsum,Geant} [GeV]",
                                    "E_{Veto,Geant} [GeV]",
                                    100,0,1,
                                    100,0,0.01 ));

        gammadEE.SetHist(hf.Make2DN(
                                    "dE - E Geant: #gamma",
                                   "E_{ESsum,Geant} [GeV]",
                                   "E_{Veto,Geant} [GeV]",
                                    100,0,1,
                                    100,0,0.01 ));

        piondEE.SetHist(hf.Make2DN(
                                    "dE - E Geant: #pi^{#pm}",
                                  "E_{ESsum,Geant} [GeV]",
                                  "E_{Veto,Geant} [GeV]",
                                    100,0,1,
                                    100,0,0.01 ));

        electrondEE.SetHist(hf.Make2DN(
                                    "dE - E Geant: #e^{#pm}",
                                    "E_{ESsum,Geant} [GeV]",
                                    "E_{Veto,Geant} [GeV]",
                                    100,0,1,
                                    100,0,0.01 ));


        lproton.add(&protonE);
        lproton.add(&protondEE);

        lgamma.add(&gammaE);
        lgamma.add(&gammadEE);

        lpion.add(&pionE);
        lpion.add(&piondEE);

        lelectron.add(&electronE);
        lelectron.add(&electrondEE);

        branch.setBranch(1, &lgamma);
        branch.setBranch(14, &lproton);
        branch.setBranch(8, &lpion);
        branch.setBranch(9, &lpion);
        branch.setBranch(2, &lelectron);
        branch.setBranch(3, &lelectron);
    }

    FillDecision<fGeantPlutoPID>* Node() { return &branch; }


};

class RootFillNode: public FillNode<GeantPlutoPair> {
public:
    FillDecision<fGeantPlutoApparatus> dApp;
    EnergyHistograms<fGeantPlutoTAPSESum,fGeantPlutoVetoESum> tapshists;
    EnergyHistograms<fGeantPlutoCBESum,fGeantPlutoPIDESum> cbhists;

    RootFillNode():
        dApp(),
        tapshists("TAPS:", "taps"),
        cbhists("CB:", "cb")
    {
        dApp.setBranch(2, tapshists.Node());
        dApp.setBranch(1, cbhists.Node());
    }

    virtual ~RootFillNode() {}
    virtual void Fill(GeantPlutoPair data) {
        dApp.Fill(data);
    }

    virtual void Draw(TVirtualPad* pad=nullptr) {
        dApp.Draw(pad);
    }

};

fPlutoGeant::fPlutoGeant()
{
    root_node = new RootFillNode();
}

fPlutoGeant::~fPlutoGeant()
{
    delete root_node;
}

void fPlutoGeant::Run(const fPhysics &p)
{

    const GTreePluto::ParticleList particles = p.Pluto().GetFinalState();
        if( particles.size() == 1 ) {
            const PParticle& particle = *(particles.front());
            root_node->Fill( GeantPlutoPair( p.Geant(), particle) );
        }

}

void fPlutoGeant::Display()
{
    root_node->Draw();
}
