#include "EventManager.h"
#include <stdexcept>
#include "TMath.h"
#include "Rtypes.h"
#ifdef hasPluto
#include "PParticle.h"
#endif

#include "GTreeTrack.h"
#include "Detector.h"

using namespace std;
using namespace ant;

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

Bool_t EventManager::Init(const char *configfile)
{
    // nothing to do...
    return true;
}

void EventManager::Finish()
{
    for( auto& p : physics ) {
        p->Finish();
    }
}

Bool_t EventManager::Start()
{
    SetAsPhysicsFile();

    //TraverseValidEvents();
    TraverseEntries(0,1000);

    return kTRUE;
}

void EventManager::ProcessEvent()
{

    Event event;

    CopyTracks(GetTracks(), event.TrackStorage());

    CopyParticles(GetPhotons(),         ParticleTypeDatabase::Photon,       event);
    CopyParticles(GetProtons(),         ParticleTypeDatabase::Proton,       event);
    CopyParticles(GetChargedPions(),    ParticleTypeDatabase::PiCharged,    event);
    CopyParticles(GetElectrons(),       ParticleTypeDatabase::eCharged,     event);

    CopyTaggerHits(event.TaggerHitStorage());

#ifdef hasPluto
    CopyPlutoParticles(GetPluto(), event.MCTrueStorage());
#endif

    event.Finalize();

    RunPhysics(event);
}

void EventManager::ProcessScalerRead()
{
}

Bool_t EventManager::Write()
{
}

void EventManager::RunPhysics(const Event &event)
{
    for( auto& p : physics ) {
        p->ProcessEvent(event);
    }
}

void EventManager::CopyParticles(GTreeParticle *tree, const ParticleTypeDatabase::Type &type, Event &target)
{
    for(UInt_t i=0; i<tree->GetNParticles(); ++i) {

        const TLorentzVector& lv = tree->Particle(i);
        const Int_t trackIndex = tree->GetTrackIndex(i);
        const ant::Track* track = &target.TrackStorage().at(trackIndex);

        target.ParticleStorage().emplace_back(
                   RecParticle(
                        Particle(type,lv),
                        track)
                    );
    }
}


/**
 * @brief map goat apparatus numbers to apparatus_t enum values
 * in case unknown values show up: -> exception and do not sliently ignore
 */
detector_t IntToDetector_t( const int& a ) {
    detector_t d = detector_t::None;
    if(a & GTreeTrack::DETECTOR_NaI) {
        d |= detector_t::NaI;
    }
    if(a & GTreeTrack::DETECTOR_PID) {
        d |= detector_t::PID;
    }
    if(a & GTreeTrack::DETECTOR_MWPC) {
        d |= detector_t::MWPC;
    }
    if(a & GTreeTrack::DETECTOR_BaF2) {
        d |= detector_t::BaF2;
    }
    if(a & GTreeTrack::DETECTOR_PbWO4) {
        d |= detector_t::PbWO4;
    }
    if(a & GTreeTrack::DETECTOR_Veto) {
        d |= detector_t::Veto;
    }
    return d;
}

/**
 * @brief map the cluster sizes from goat to unisgend ints
 * negative values mean no hit in the calorimeter
 * map those to 0
 */
clustersize_t MapClusterSize(const int& size) {
    return size < 0 ? 0 : size;
}

void EventManager::CopyTracks(GTreeTrack *tree, Event::TrackList_t &container)
{
    for(UInt_t i=0; i<tree->GetNTracks(); ++i) {

        container.emplace_back(
                    tree->GetClusterEnergy(i),
                    tree->GetTheta(i) * TMath::DegToRad(),
                    tree->GetPhi(i) * TMath::DegToRad(),
                    tree->GetTime(i),
                    MapClusterSize(tree->GetClusterSize(i)),
                    IntToDetector_t(tree->GetDetectors(i)),
                    tree->GetVetoEnergy(i),
                    tree->GetMWPC0Energy(i),
                    tree->GetMWPC1Energy(i)
                    );
    }
}

#ifdef hasPluto
void EventManager::CopyPlutoParticles(GTreePluto *tree, Event::MCParticleList_t &container)
{
    const GTreePluto::ParticleList particles = tree->GetFinalState();

    const ParticleTypeDatabase::Type* type=nullptr;
    for( auto& p : particles ) {
        switch(p->ID()) {
        case 1:
            type = &ParticleTypeDatabase::Photon;
            break;
        case 14:
            type = &ParticleTypeDatabase::Proton;
            break;
        case 2:
            type = &ParticleTypeDatabase::ePlus;
            break;
        case 3:
            type = &ParticleTypeDatabase::eMinus;
            break;
        case 7:
            type = &ParticleTypeDatabase::Pi0;
            break;
        case 8:
            type = &ParticleTypeDatabase::PiPlus;
            break;
        case 9:
            type = &ParticleTypeDatabase::PiMinus;
            break;
        default:
            continue;
        }

        TLorentzVector lv = *p;
        lv *= 1000.0;   // convert to MeV

        container.emplace_back( MCParticle(
                    *type,
                    lv)
                    );
    }
}
#endif

void EventManager::CopyTaggerHits(Event::TaggerHitList_t &container)
{
    const GTreeTagger& tagger = *GetTagger();

    for( Int_t i=0; i<tagger.GetNTagged(); ++i) {
        container.emplace_back(
                    TaggerHit(
                    tagger.GetTaggedChannel(i),
                    tagger.GetTaggedEnergy(i),
                    tagger.GetTaggedTime(i))
                    );
    }
}
