#include "EventManager.h"
#include <stdexcept>
#include "TMath.h"
#include "Rtypes.h"


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
    return true;
}


Bool_t EventManager::Start()
{
    SetAsPhysicsFile();

    TraverseValidEvents();

    return kTRUE;
}

void EventManager::ProcessEvent()
{
    TrackList_t tracks;

    CopyTracks(GetTracks(), tracks);

    cout << "--------------------" << endl;
    for( auto& track : tracks ) {
        cout << track << endl;
    }
}

void EventManager::ProcessScalerRead()
{
}

Bool_t EventManager::Write()
{
}

void EventManager::CopyParticles(GTreeParticle *tree, const ParticleTypeDatabase::Type &type, EventManager::ParticleList_t &container)
{
}

apparatus_t IntToAppatatus_t( const int& a ) {
    switch(a) {
    case 1:
        return apparatus_t::CB;
    case 2:
        return apparatus_t::TAPS;
    case 3:
        return apparatus_t::CBTAPS;
    default:
        throw std::logic_error("Encountered Unknown Apparatus Type.");
    }
}

/**
 * @brief map the cluster sizes from goat to unisgend ints
 * negative values mean no hit in the calorimeter
 * map those to 0
 */
clustersize_t MapClusterSize(const int& size) {
    return size < 0 ? 0 : size;
}

void EventManager::CopyTracks(GTreeTrack *tree, EventManager::TrackList_t &container)
{
    for(UInt_t i=0; i<tree->GetNTracks(); ++i) {

        container.emplace_back(
                    tree->GetClusterEnergy(i),
                    tree->GetTheta(i) * TMath::DegToRad(),
                    tree->GetPhi(i) * TMath::DegToRad(),
                    tree->GetTime(i),
                    MapClusterSize(tree->GetClusterSize(i)),
                    tree->GetCentralCrystal(i),
                    tree->GetCentralVeto(i),
                    IntToAppatatus_t(tree->GetApparatus(i)),
                    tree->GetVetoEnergy(i),
                    tree->GetMWPC0Energy(i),
                    tree->GetMWPC1Energy(i)
                    );
    }
}
