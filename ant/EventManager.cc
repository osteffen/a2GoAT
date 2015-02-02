#include "EventManager.h"
#include <stdexcept>
#include "TMath.h"
#include "Rtypes.h"
#ifdef hasPluto
#include "PParticle.h"
#endif

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


Bool_t EventManager::Start()
{
    SetAsPhysicsFile();

    //TraverseValidEvents();
    TraverseEntries(0,10);

    return kTRUE;
}

void EventManager::ProcessEvent()
{
    // THIS IS JUST A TEST
    TrackList_t tracks;

    // NOTE: Same tracks appear inside of reconstrucred particles again!
    CopyTracks(GetTracks(), tracks);

    cout << "--------------------" << endl;
    for( auto& track : tracks ) {
        cout << track << endl;
    }

    ParticleList_t particles;

    CopyParticles(GetPhotons(), ParticleTypeDatabase::Photon, particles);
    CopyParticles(GetProtons(), ParticleTypeDatabase::Proton, particles);
    CopyParticles(GetNeutralPions(), ParticleTypeDatabase::Pi0, particles);
    //... and all the other trees

    for( auto& praticle : particles ) {
        cout << praticle << endl;
    }

#ifdef hasPluto
    ParticleList_t mcparticles;
    CopyPlutoParticles(GetPluto(), mcparticles);
    cout << "MC:" <<endl;
    for( auto& praticle : mcparticles ) {
        cout << praticle << endl;
    }
#endif
}

void EventManager::ProcessScalerRead()
{
}

Bool_t EventManager::Write()
{
}

void EventManager::CopyParticles(GTreeParticle *tree, const ParticleTypeDatabase::Type &type, EventManager::ParticleList_t &container)
{
    for(UInt_t i=0; i<tree->GetNParticles(); ++i) {

        // goat partile also has track type members.... what to do with them?
        container.emplace_back(
                    type,
                    tree->Particle(i)
                    );
    }
}


/**
 * @brief map goat apparatus numbers to apparatus_t enum values
 * in case unknown values show up: -> exception and do not sliently ignore
 */
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
                    IntToAppatatus_t(tree->GetApparatus(i)),
                    tree->GetVetoEnergy(i),
                    tree->GetMWPC0Energy(i),
                    tree->GetMWPC1Energy(i)
                    );
    }
}

#ifdef hasPluto
void EventManager::CopyPlutoParticles(GTreePluto *tree, ParticleList_t& container)
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

        container.emplace_back(
                    *type,
                    *((TLorentzVector*) p)
                    );
    }
}
#endif
