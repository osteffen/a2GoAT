#include "PDeltaspin.h"
#include <iostream>

using namespace std;


PDeltaspin::PDeltaspin(): prompt("prompt"), random("random"), diff("diff")
{
    prompt.AddHistogram("nPart", "number of particles",
                   "number of particles / event", "",
                   10, 0, 10); // 10 bins from 0 to 10

    prompt.AddHistogram("pid", "PID Bananas",
                   "CB Energy [MeV]", "dE [MeV]",
                   100,0,450, // 100 bins from 0 to 450 in x
                   100,0,20   // 100 bins from 0 to 20  in y
                   );

    prompt.AddHistogram("2gIM", "2#gamma invariant mass",
                   "M_{#gamma #gamma} [MeV]", "",
                   100,0,300);

    prompt.AddHistogram("tag_time", "Tagger time",
                   "t [ns]", "",
                   100,-50,50);

    prompt.AddHistogram("tag_energy", "Tagged Photon Energy",
                   "E_{#gamma} [MeV]", "",
                   100,100,450);

    prompt.AddHistogram("mmp", "Missing Mass Proton",
                   "MM_{p} [MeV]", "",
                   100,600,1100);

    prompt.AddHistogram("pi0angle", "#pi^{0} #Theta angle (boosted)",
                   "cos(#theta_{#pi^{0}})", "",
                   180,-1,1);

    prompt.AddHistogram("pi0angle_noboost", "#pi^{0} #Theta angle (not boosted)",
                   "cos(#theta_{#pi^{0}})", "",
                   180,-1,1);

    prompt.AddHistogram("pi0angle_tagged", "#pi^{0} #Theta angle (boosted) vs tagged E",
                   "cos(#theta_{pi^{0}})", "E_{#gamma} [MeV]",
                   180,-1,1,14,110,300);

    prompt.AddHistogram("delta_pz", "#Delta^{+} momentum magnitude (boosted)",
                   "p_{#Delta^{+}} [MeV]","",
                   100,0,300);

    prompt.AddHistogram("delta_IM", "#Delta^{+} Invariant mass",
                   "M_{#Delta^{+}} [MeV]","",
                   100,800,1500);

    random = Histogm(prompt, "random");
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

void PDeltaspin::Finish()
{
    diff = Histogm(prompt,"diff");
    diff.AddScaled(random, -1.0);

    random.Draw();
    prompt.Draw();
    diff.Draw();
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
    // Loop over Tagger hits
    for( int tag_index=0; tag_index < GetTagger()->GetNTagged(); ++tag_index ) {

        const Double_t ttime =  GetTagger()->GetTaggedTime(tag_index);
        const Double_t tenergy = GetTagger()->GetTaggedEnergy(tag_index);


        const Int_t nParticles = GetTracks()->GetNTracks();

        // decide if tagger hit is prompt or random
        bool isPrompt = false;
        if( ttime > -8 && ttime < 8 ) {
            isPrompt = true;
        } else if( ttime > -16 && ttime < 16 ) {
            isPrompt = false;
        } else
            continue;



        // select the right histogram
        Histogm& h = isPrompt ? prompt : random;

        // some basic histograms
        h["nPart"]->Fill(nParticles);
        h["tag_energy"]->Fill(tenergy);
        h["tag_time"]->Fill(ttime);

        // might be that proton is lost,
        // so two or three particle events are interesting

        if( GetNeutralPions()->GetNMesons() != 1)
            continue;

        if( GetProtons()->GetNParticles() > 1)
            continue;

        const TLorentzVector& pi0 = GetNeutralPions()->Meson(0);

        h["2gIM"]->Fill(pi0.M());

        // construct beam photon 4-vector and, using this, the delta restframe
        const TLorentzVector beam(0,0,tenergy,tenergy);
        const TLorentzVector delta_beam(beam + GetTarget());
        const TVector3 boost = -(delta_beam.BoostVector());

        // boost pi0
        TLorentzVector pi0_ = pi0;
        pi0_.Boost(boost);

        // missing mass plot (should peak at proton)
        TLorentzVector mmp = delta_beam - pi0;
        h["mmp"]->Fill( mmp.M() );

        // plot boosted pi0 angle, our desired plot!
        h["pi0angle"]->Fill( cos(pi0_.Theta()) );
        h["pi0angle_noboost"]->Fill( cos(pi0.Theta()));
        h["pi0angle_tagged"]->Fill( cos(pi0_.Theta()), beam.E());

        // have a look at proton reconstruction quality
        // by creating a delta
        if( GetProtons()->GetNParticles() == 1) {
            const TLorentzVector& proton = GetProtons()->Particle(0);
            TLorentzVector delta = pi0 + proton;

            TLorentzVector delta_ = delta;
            delta_.Boost(boost);

            h["delta_pz"]->Fill(delta_.P());
            h["delta_IM"]->Fill(delta_.M());
        }
    }
}

void	PDeltaspin::ProcessScalerRead()
{
}

Bool_t	PDeltaspin::Write()
{
	// Write all GH1's easily
	GTreeManager::Write();
}
