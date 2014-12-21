#include "GTreeGoatParticle.h"
#include "GTreeParticle.h"
#include "TVector3.h"


// TODO: Fix all of this
void GParticle::SetEThetaPhi(const Double_t E, const Double_t theta, const Double_t phi)
{
    const Double_t Et = E + mass;
    const Double_t p = sqrt(Et*Et - mass*mass);

    TVector3 v(p, 0.0, 0.0);
    v.SetPtThetaPhi(p, theta, phi);

    lv = TLorentzVector(v, Et);
}

GGoatParticleManager::GGoatParticleManager()
{
}

GGoatParticleManager::~GGoatParticleManager()
{
    Clear();
}

void GGoatParticleManager::Clear()
{
    for( ParticleList::iterator i=particles.begin(); i!=particles.end(); ++i) {
        delete (*i);
    }
    particles.clear();
}

const GGoatParticleManager::ParticleList &GGoatParticleManager::Particles() const
{
    return particles;
}

void GGoatParticleManager::CollectParticles()
{
    Clear();

    for( auto entry=trees.begin(); entry !=trees.end(); ++entry ) {

        const GTreeParticle* pTree = entry->first;
        const ParticleType&  type = entry->second;

        for( UInt_t i=0; i < pTree->GetNParticles(); ++i) {
            particles.push_back(
                        new GParticle(
                            pTree->Particle(i),
                            pTree->GetApparatus(i),
                            pTree->GetTime(i),
                            pTree->GetClusterSize(i),
                            pTree->GetVetoEnergy(i),
                            pTree->GetMWPC0Energy(i),
                            pTree->GetMWPC1Energy(i),
                            type.id,
                            type.mass,
                            type.ischagred));
        }
    }
}

void GGoatParticleManager::AddTree(const GTreeParticle *tree, Int_t particle_id, const Double_t particle_mass, const bool charged)
{
    trees[tree] = ParticleType( {particle_id, particle_mass, charged} );
}
