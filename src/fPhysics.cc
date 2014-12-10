#include "fPhysics.h"

#include "TCanvas.h"
#include "TH1D.h"

Bool_t fPhysics::Start()
{

}

void fPhysics::ProcessEvent()
{
    GeantTestPlot.Run(*this);
}

void fPhysics::ProcessScalerRead()
{

}

Bool_t fPhysics::Write()
{

}

fPhysics::fPhysics()
{
}

fPhysics::~fPhysics()
{
}

Bool_t fPhysics::Init(const char *configfile)
{
    return true;
}

void fPhysics::Display()
{
    GeantTestPlot.Display();
}



fGeantTestPlot::fGeantTestPlot()
{
    hCBEsum = new TH1D("hCBESum","CB ESum",100,0,1);
}

void fGeantTestPlot::Run(const fPhysics &p)
{
    hCBEsum->Fill(p.Geant().GetCBEsum());
}

void fGeantTestPlot::Display()
{
    new TCanvas("cGeantTestPlot","Geant Test Plot");
    hCBEsum->Draw();
}
