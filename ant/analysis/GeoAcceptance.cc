#include "GeoAcceptance.h"
#include "Particle.h"
#include "Track.h"
#include "utils/combinatorics.h"
#include "plot/root_draw.h"
#include "Event.h"
#include "TMath.h"
#include <tuple>
#include "TVector3.h"
#include "Detector.h"
#include <sstream>

using namespace std;
using namespace ant;



const HistogramFactory::BinSettings analysis::GeoAcceptance::ParticleThetaPhiPlot::theta_bins(180,0.0,180.0);
const HistogramFactory::BinSettings analysis::GeoAcceptance::ParticleThetaPhiPlot::phi_bins(360,-180.0,180.0);

analysis::GeoAcceptance::ParticleThetaPhiPlot::ParticleThetaPhiPlot(const string &title, const string &name)
{
    hist = hf.Make2D(title,"#theta [#circ]","#phi [#circ]",theta_bins,phi_bins,name);
}

void analysis::GeoAcceptance::ParticleThetaPhiPlot::Fill(refPartcile p)
{
    hist->Fill(p->Theta()*TMath::RadToDeg(), p->Phi()*TMath::RadToDeg());
}

TObject *analysis::GeoAcceptance::ParticleThetaPhiPlot::GetObject()
{
    return hist;
}

void analysis::GeoAcceptance::ParticleThetaPhiPlot::Draw(const string &option) const
{
    hist->Draw(option.c_str());
}


analysis::GeoAcceptance::GeoAcceptance(const mev_t energy_scale):
    mctrue_pos("True Tracks","true"),
    matched_pos("Found 1 match","matched1"),
    lost_pos("Lost","lost")
{
}

analysis::GeoAcceptance::~GeoAcceptance()
{

}

ant::HistogramFactory analysis::GeoAcceptance::hf("GeoAcceptance");

void analysis::GeoAcceptance::ProcessEvent(const Event &event)
{
    if( event.MCTrue().size() != 1 )
        return;
    refPartcile input = event.MCTrue().at(0);

    mctrue_pos.Fill(input);

    if( event.Particles().size() == 0)
        lost_pos.Fill(input);
    else if (event.Particles().size() == 1) {
        refPartcile output = event.Particles().at(0);
        const double angle = output->Angle(input->Vect()) * TMath::RadToDeg();
        if(angle < 30.0)
            matched_pos.Fill(input);
    }

}

void analysis::GeoAcceptance::Finish()
{

}

void analysis::GeoAcceptance::ShowResult()
{
    canvas("GeoAcceptance") << mctrue_pos << matched_pos << lost_pos << endc;

}
