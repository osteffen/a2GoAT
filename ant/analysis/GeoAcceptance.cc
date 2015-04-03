#include "GeoAcceptance.h"
#include "Particle.h"
#include "Track.h"
#include "utils/combinatorics.h"
#include "utils/matcher.h"
#include "plot/root_draw.h"
#include "Event.h"
#include "TMath.h"
#include <tuple>
#include "TVector3.h"
#include "Detector.h"
#include <sstream>
#include "TH3D.h"
#include "TH2D.h"

using namespace std;
using namespace ant;


analysis::GeoAcceptance::ParticleThetaPhiPlot::ParticleThetaPhiPlot(SmartHistFactory &factory, const string &title, const string &name,const BinSettings& thetabins,const BinSettings& phibins)
{
    hist = factory.makeTH2D(title,"#theta [#circ]","#phi [#circ]",thetabins,phibins,name);
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


analysis::GeoAcceptance::GeoAcceptance(const std::string& name, const mev_t energy_scale):
    Physics(name),
    photon_acceptance(HistFac,geo,"Photons")
{}

analysis::GeoAcceptance::~GeoAcceptance()
{
}

void analysis::GeoAcceptance::ProcessEvent(const Event &event)
{
    photon_acceptance.Fill(
                event.MCTrue().Particles().Get(ParticleTypeDatabase::Photon),
                event.Reconstructed().Particles().Get(ParticleTypeDatabase::Photon)
                );
}

void analysis::GeoAcceptance::Finish()
{
//    angle_regions_photons->Scale(1.0/angle_regions_photons->GetEntries());
 //   n_photons_lost->Scale(1.0/n_photons_lost->GetEntries());
}

void analysis::GeoAcceptance::ShowResult()
{
    photon_acceptance.ShowResult();
}


analysis::GeoAcceptance::ParticleThetaPhiPlot3D::ParticleThetaPhiPlot3D(SmartHistFactory& factory, const string &title, const string &name):
    hist(factory.makeTH3D(title, "x","y","z",BinSettings(200,-1,1),BinSettings(200,-1,1),BinSettings(200,-1,1),name)),
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


analysis::GeoAcceptance::AcceptanceAnalysis::AcceptanceAnalysis(SmartHistFactory& factory, const A2SimpleGeometry &geo_, const string &name_):
    name(name_),
    HistFac(name,factory),
    geo(geo_),
    mctrue_pos(HistFac, "True Tracks","true"),
    matched_pos(HistFac,"Reconstructed 1","matched1"),
    multimatched_pos(HistFac,"Reconstructed 1+","multimatched"),
    lost_pos(HistFac,"Not Reconstructed","lost"),
    lost_pos_zoom(HistFac,"Not Reconstructed, Zoomed","lostzoom",BinSettings(180),BinSettings(100,-10,10)),
    lost3d(HistFac,"Not Reconctructed 3D","lost3d"),
    angle_regions(HistFac.makeTH1D("Angle Regions "+name,"Region","",BinSettings(3),"regions")),
    nlost(HistFac.makeTH1D("# lost "+name,"lost","",BinSettings(3),"nlost")),
    energy_reco(HistFac.makeHist<double>("Energy Reconstrution "+name,"E_{rec}/E_{true}","",BinSettings(100,.5,1),"energy_reco"))
{
}

template <typename T>
void remove_low_energy(T& data, double min) {

    using element_type = typename T::value_type;

    data.erase(remove_if(data.begin(), data.end(),
              [min] (const element_type& m) { return (m.b->E()/m.a->E()) < min;}), data.end());
}

void analysis::GeoAcceptance::AcceptanceAnalysis::Fill(const ParticleList &mctrue, const ParticleList &reconstructed)
{
    if( mctrue.size() != 1)
        return;

    const auto& input = mctrue.front();



    mctrue_pos.Fill(input);

    auto matched = utils::match1to1(mctrue, reconstructed, Particle::calcAngle, IntervalD(0.0, 20.0*TMath::DegToRad()));
    remove_low_energy(matched, 0.9);

    if( matched.size() ==0 ) {
        lost_pos.Fill(input);
        lost_pos_zoom.Fill(input);
        lost3d.Fill(input);
    } else if( matched.size() ==1 ) {

        matched_pos.Fill(input);
        energy_reco.Fill( matched.front().b->E()/matched.front().a->E());

        if(reconstructed.size()>1)
            multimatched_pos.Fill(input);
    }

    detector_t region = geo.DetectorFromAngles(*input);
    stringstream region_name;
    region.Print(region_name);

    angle_regions->Fill(region_name.str().c_str(),1);

    int nphotonslost=0;
    for(auto& p : mctrue) {
        if(geo.DetectorFromAngles(*p) == detector_t::None)
            nphotonslost++;
    }
    nlost->Fill(nphotonslost);

}

void analysis::GeoAcceptance::AcceptanceAnalysis::ShowResult()
{
    canvas("GeoAcceptance: "+name)
            << drawoption("colz")
            << mctrue_pos
            << matched_pos
            << multimatched_pos
            << lost_pos
            << lost_pos_zoom
            << lost3d
            << angle_regions
            << nlost
            << energy_reco
            << endc;
}
