#include "PDeltaspin.h"
#include <iostream>

using namespace std;

PDeltaspin::PDeltaspin()
{ 
    time 	= new GH1("time", 	"time", 	1400, -700, 700);
    time_cut 	= new GH1("time_cut", 	"time_cut", 	1400, -700, 700);

    time_2g 	= new GH1("time_2g",	"time_2g", 	1400, -700, 700);
    time_2g_cut = new GH1("time_2g_cut","time_2g_cut", 	1400, -700, 700);

    IM 		= new GH1("IM", 	"IM", 		400,   0, 400);
    IM_2g 	= new GH1("IM_2g", 	"IM_2g", 	400,   0, 400);
  
    MM		= new GH1("MM", 	"MM", 	 	400,   800, 1200);     
    MM_2g	= new GH1("MM_2g", 	"MM_2g", 	400,   800, 1200);

    TaggerAccScal = new TH1D("TaggerAccScal","TaggerAccScal",16,0,16);
    h_apparatus = new TH1D("h_apparatus","apparatus",5,0,5);
    h_ggim = new TH1D("h_ggim","ggim",100,0,300);
}

PDeltaspin::~PDeltaspin()
{
}

Bool_t	PDeltaspin::Init(const char* configfile)
{
	cout << "Initialising physics analysis..." << endl;
	cout << "--------------------------------------------------" << endl << endl;
	if(configfile) SetConfigFile(configfile);

	if(!InitBackgroundCuts()) return kFALSE;
	if(!InitTargetMass()) return kFALSE;
	if(!InitTaggerChannelCuts()) return kFALSE;
	if(!InitTaggerScalers()) return kFALSE;
	cout << "--------------------------------------------------" << endl;
	return kTRUE;
}

Bool_t	PDeltaspin::Start()
{
    if(!IsGoATFile())
    {
        cout << "ERROR: Input File is not a GoAT file." << endl;
        return kFALSE;
    }

    SetAsPhysicsFile();

    TraverseValidEvents();

    return kTRUE;
}

void	PDeltaspin::ProcessEvent()
{
    if( GetPhotons()->GetNParticles() == 2 ) {
        TLorentzVector gg = GetPhotons()->Particle(0) + GetPhotons()->Particle(1);
        h_ggim->Fill(gg.M());
    }

    // fill time diff (tagger - pi0), all pi0
    FillTime(*GetNeutralPions(),time);
    FillTimeCut(*GetNeutralPions(),time_cut);
	
	// fill missing mass, all pi0
    FillMissingMass(*GetNeutralPions(),MM);
	
	// fill invariant mass, all pi0
    FillMass(*GetNeutralPions(),IM);
		
    // Some neutral decays
    for (Int_t i = 0; i < GetNeutralPions()->GetNParticles(); i++)
    {
        // Fill MM for 2 photon decay
        if ((GetNeutralPions()->GetNSubParticles(i) == 2) & (GetNeutralPions()->GetNSubPhotons(i) == 2))
        {
		// fill time diff (tagger - pi0), this pi0
        FillTime(*GetNeutralPions(),i,time_2g);
        FillTimeCut(*GetNeutralPions(),i,time_2g_cut);
			
		// fill missing mass, this pi0
                FillMissingMass(*GetNeutralPions(),i,MM_2g);
            
		// fill invariant mass, this pi0
            FillMass(*GetNeutralPions(),i,IM_2g);
        }

    }

}

void	PDeltaspin::ProcessScalerRead()
{
	// Fill Tagger Scalers
	FillScalers(GetTC_scaler_min(),GetTC_scaler_max(),TaggerAccScal);
}

Bool_t	PDeltaspin::Write()
{
	// Write some TH1s
	GTreeManager::Write(TaggerAccScal);
    GTreeManager::Write(h_apparatus);
    GTreeManager::Write(h_ggim);

	// Write all GH1's easily
	GTreeManager::Write();
}
