#include "DeltaPlusPhysics.h"

#include "Histogram.h"
#include "Event.h"
#include "Particle.h"
#include "ParticleType.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

using namespace ant;
using namespace ant::analysis;
using namespace std;

DeltaPlusPhysics::DeltaPlusPhysics():
    prompt("DeltaPlus_prompt"),
    random("DeltaPlus_random"),
    diff("DeltaPlus_diff"),
    pi0_cut(110,150)
{

}

void DeltaPlusPhysics::ProcessEvent(const Event &event)
{
    Event::ParticleList_t photons;
    Event::ParticleList_t protons;

    for( auto& particle : event.Particles() ) {

        if( particle->Type() ==  ParticleTypeDatabase::Photon )
            photons.emplace_back(particle);
        else if ( particle->Type() == ParticleTypeDatabase::Proton )
            protons.emplace_back(particle);
    }

    Histogm& h = prompt;

    if(photons.size() == 2) {
        const TLorentzVector pi0 = *photons.at(0) + *photons.at(1);
        h["2gIM"]->Fill(pi0.M());

        if( pi0_cut.Contains( pi0.M()) ) {

        }
    }

    if(protons.size() == 1) {

    }
}

void DeltaPlusPhysics::Finish()
{
    diff = prompt;
    diff.AddScaled(random, -1.0);
}

void DeltaPlusPhysics::ShowResult()
{
    diff.Draw();

}


void DeltaPlusPhysics::Histogm::AddHistogram(const string &name, const string &title, const string &x_label, const string &y_label, const int x_bins_n, const double x_bins_low, const double x_bins_up)
{

    // setup one dimensional histogram TH1D
    h_title[name] = title;

    h[name] = hf.Make1D(
                title,
                x_label,
                y_label,
                HistogramFactory::BinSettings(x_bins_n, x_bins_low, x_bins_up));

}

void DeltaPlusPhysics::Histogm::AddHistogram(const string &name, const string &title, const string &x_label, const string &y_label, const int x_bins_n, const double x_bins_low, const double x_bins_up, const int y_bins_n, const double y_bins_low, const double y_bins_up)
{

    // setup two dimensional histogram TH2D
    h_title[name] = title;
    h[name] = hf.Make2D(
                title,
                x_label,
                y_label,
                HistogramFactory::BinSettings(x_bins_n, x_bins_low, x_bins_up),
                HistogramFactory::BinSettings(y_bins_n, y_bins_low, y_bins_up));
}

DeltaPlusPhysics::Histogm::Histogm(const string &prefix):
    hf(prefix)
{
    AddHistogram("nPart", "number of particles",
                   "number of particles / event", "",
                   10, 0, 10); // 10 bins from 0 to 10

    AddHistogram("pid", "PID Bananas",
                   "CB Energy [MeV]", "dE [MeV]",
                   100,0,450, // 100 bins from 0 to 450 in x
                   100,0,20   // 100 bins from 0 to 20  in y
                   );

    AddHistogram("2gIM", "2#gamma invariant mass",
                   "M_{#gamma #gamma} [MeV]", "",
                   100,0,300);

    AddHistogram("tag_time", "Tagger time",
                   "t [ns]", "",
                   100,-50,50);

    AddHistogram("tag_energy", "Tagged Photon Energy",
                   "E_{#gamma} [MeV]", "",
                   100,100,450);

    AddHistogram("mmp", "Missing Mass Proton",
                   "MM_{p} [MeV]", "",
                   100,600,1100);

    AddHistogram("pi0angle", "#pi^{0} #Theta angle (boosted)",
                   "cos(#theta_{#pi^{0}})", "",
                   180,-1,1);

    AddHistogram("pi0angle_noboost", "#pi^{0} #Theta angle (not boosted)",
                   "cos(#theta_{#pi^{0}})", "",
                   180,-1,1);

    AddHistogram("pi0angle_tagged", "#pi^{0} #Theta angle (boosted) vs tagged E",
                   "E_{#gamma} [MeV]", "cos(#theta_{pi^{0}})",
                   180,-1,1,14,110,300);

    AddHistogram("delta_pz", "#Delta^{+} momentum magnitude (boosted)",
                   "p_{#Delta^{+}} [MeV]","",
                   100,0,300);

    AddHistogram("delta_IM", "#Delta^{+} Invariant mass",
                   "M_{#Delta^{+}} [MeV]","",
                   100,800,1500);
}

void DeltaPlusPhysics::Histogm::Draw()
{
    {
        TCanvas* c = new TCanvas(Form("%s_c",pref.c_str()),pref.c_str());
        const int cols = ceil(sqrt(h.size()));
        const int rows = ceil((double)h.size()/(double)cols);
        c->Divide(cols,rows);
        int pad=1;
        for(auto i=h.begin(); i!=h.end(); ++i) {
            c->cd(pad);
            TH2D* h2 = dynamic_cast<TH2D*>(i->second);
            if(h2 != nullptr) {
                h2->Draw("colz");
            }
            else {
                i->second->Draw();
            }
            pad++;
        }
    }
}

DeltaPlusPhysics::Histogm &DeltaPlusPhysics::Histogm::operator*=(const Double_t factor)
{

    for(auto i=h.begin(); i!=h.end(); ++i) {
        i->second->Scale(factor);
    }
    return *this;

}

DeltaPlusPhysics::Histogm DeltaPlusPhysics::Histogm::operator=(const DeltaPlusPhysics::Histogm &other)
{

    for(auto i=h.begin(); i!=h.end(); ++i) {
        TH1* h = i->second;
        h->Reset();
        h->Add(other[i->first]);
    }
    return *this;

}
void DeltaPlusPhysics::Histogm::AddScaled(const DeltaPlusPhysics::Histogm &h2, const Double_t f)
{
    for(auto i=h.begin(); i!=h.end(); ++i) {
        i->second->Add(h2.h.at(i->first),f);
    }
}
