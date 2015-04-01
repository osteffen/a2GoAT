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
#include "TH3D.h"

using namespace std;
using namespace ant;


const BinSettings analysis::GeoAcceptance::ParticleThetaPhiPlot::theta_bins(180,0.0,180.0);
const BinSettings analysis::GeoAcceptance::ParticleThetaPhiPlot::phi_bins(360,-180.0,180.0);

analysis::GeoAcceptance::ParticleThetaPhiPlot::ParticleThetaPhiPlot(const string &title, const string &name)
{
    hist = HistogramFactory::Make2D(title,"#theta [#circ]","#phi [#circ]",theta_bins,phi_bins,name);
}

void analysis::GeoAcceptance::ParticleThetaPhiPlot::Fill(const ParticlePtr &p)
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
    lost_pos("Lost","lost"),
    lost3d("Lost 3D","GeoAcceptance_lost3d")
{
    angle_regions_protons = HistogramFactory::Make2D("Angle Regions Protons","Region","Punch",BinSettings(3),BinSettings(2));
    angle_regions_photons = HistogramFactory::Make1D("Angle Regions Photons","Region","",BinSettings(3));
    n_photons_lost = HistogramFactory::Make1D("# lost photons","# lost","",BinSettings(10));
}

analysis::GeoAcceptance::~GeoAcceptance()
{

}

void analysis::GeoAcceptance::ProcessEvent(const Event &event)
{
    if( event.MCTrue().Particles().Get(ParticleTypeDatabase::Photon).size() == 1 ) {

        ParticlePtr input = event.MCTrue().Particles().Get(ParticleTypeDatabase::Photon).at(0);

        mctrue_pos.Fill(input);

        if( event.Reconstructed().Particles().GetAll().size() == 0) {
            lost_pos.Fill(input);
            lost3d.Fill(input);
        }
        else if (event.Reconstructed().Particles().GetAll().size() == 1) {
            ParticlePtr output = event.Reconstructed().Particles().GetAll().at(0);
            const double angle = output->Angle(input->Vect()) * TMath::RadToDeg();
            if(angle < 30.0)
                matched_pos.Fill(input);
        }


        detector_t region = geo.DetectorFromAngles(*input);
        stringstream region_name;
        region.Print(region_name);

        if(input->Type() == ParticleTypeDatabase::Photon) {
            angle_regions_photons->Fill(region_name.str().c_str(),1);
        } else if(input->Type() == ParticleTypeDatabase::Proton) {
            angle_regions_protons->Fill(region_name.str().c_str(), (input->Ek()<400.0) ? "Rec":"Punch",1);
        }
    }

    int nphotonslost=0;
    for(auto& p : event.MCTrue().Particles().Get(ParticleTypeDatabase::Photon)) {
            if(geo.DetectorFromAngles(*p) == detector_t::None)
                nphotonslost++;
    }
    n_photons_lost->Fill(nphotonslost);

}

void analysis::GeoAcceptance::Finish()
{
    angle_regions_photons->Scale(1.0/angle_regions_photons->GetEntries());
    angle_regions_protons->Scale(1.0/angle_regions_protons->GetEntries());
    n_photons_lost->Scale(1.0/n_photons_lost->GetEntries());

}

void analysis::GeoAcceptance::ShowResult()
{
    canvas("GeoAcceptance") << mctrue_pos << matched_pos << lost_pos << angle_regions_photons << angle_regions_protons << n_photons_lost << endc;

}


analysis::GeoAcceptance::ParticleThetaPhiPlot3D::ParticleThetaPhiPlot3D(const string &title, const string &name):
    hist(new TH3D(name.c_str(), title.c_str(),100,-1,1,100,-1,1,100,-1,1)),
    n(0)
{
}

void analysis::GeoAcceptance::ParticleThetaPhiPlot3D::Fill(const ParticlePtr& p)
{
        TVector3 v(p->Vect().Unit());
        hist->Fill(v.X(),v.Y(),v.Z());
}

TObject *analysis::GeoAcceptance::ParticleThetaPhiPlot3D::GetObject()
{
    return hist;
}

void analysis::GeoAcceptance::ParticleThetaPhiPlot3D::Draw(const string &option) const
{
    hist->Draw();
}
