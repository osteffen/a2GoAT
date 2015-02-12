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

    Event e;

    CopyTracks(GetTracks(), e.Tracks());

    CopyParticles(GetPhotons(), ParticleTypeDatabase::Photon, e);
    CopyParticles(GetProtons(), ParticleTypeDatabase::Proton, e);
    CopyParticles(GetChargedPions(), ParticleTypeDatabase::PiCharged, e);
    CopyParticles(GetElectrons(), ParticleTypeDatabase::eCharged, e);
    CopyTaggerHits(e.TaggerHits());

#ifdef hasPluto
    CopyPlutoParticles(GetPluto(), e.MCTrue());
#endif

    RunPhysics(e);
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
        const Event::sTrackPtr track = target.Tracks().at(trackIndex);

        target.Particles().emplace_back(
                    new RecParticle(
                        Particle(type,lv),
                        *track.get())
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

        container.emplace_back( GetTrack(tree, i) );
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

        container.emplace_back( new MCParticle(
                    *type,
                    lv)
                    );
    }
}

void EventManager::CopyTaggerHits(Event::TaggerHitList_t &container)
{
    const GTreeTagger& tagger = *GetTagger();

    for( Int_t i=0; i<tagger.GetNTagged(); ++i) {
        container.emplace_back(
                    new TaggerHit(
                    tagger.GetTaggedChannel(i),
                    tagger.GetTaggedEnergy(i),
                    tagger.GetTaggedTime(i))
                    );
    }
}

Event::sTrackPtr EventManager::GetTrack(GTreeTrack *tree, const UInt_t n)
{

    if(n >= tree->GetNTracks() )
        throw out_of_range("EventManager::GetTrack: Track index out of bounds");

    Event::sTrackPtr track(
                new Track(                    tree->GetClusterEnergy(n),
                                              tree->GetTheta(n) * TMath::DegToRad(),
                                              tree->GetPhi(n) * TMath::DegToRad(),
                                              tree->GetTime(n),
                                              MapClusterSize(tree->GetClusterSize(n)),
                                              IntToDetector_t(tree->GetDetectors(n)),
                                              tree->GetVetoEnergy(n),
                                              tree->GetMWPC0Energy(n),
                                              tree->GetMWPC1Energy(n))
                );
    return move(track);
}

#endif
