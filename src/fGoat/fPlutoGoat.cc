#include "fPlutoGoat.h"

#include "GTreeGoatParticle.h"
#include "HistFiller.h"
#include "HistogramFactory.h"
#include "PParticle.h"

#include "TMath.h"


fPlutoGoat::fPlutoGoat()
{

    HistogramFactory& hf = *HistogramFactory::GetInstanceFresh();

    root_node = new RootFillNode( hf.Make2DN(
                              "Particle Reco",
                              "Pluto",
                              "Goat",
                              15,0,15,
                              15,0,15
                              ));
}

fPlutoGoat::~fPlutoGoat()
{
    delete root_node;
}

void fPlutoGoat::Run(const fPhysics &p)
{

    const GTreePluto::ParticleList pps = p.Pluto().GetFinalState();
    const GGoatParticleManager::ParticleList& gps= p.Goat().Particles();

    for( auto pp=pps.begin(); pp!= pps.end(); ++pp) {
        for( auto gp=gps.begin(); gp!= gps.end(); ++gp) {
            if( (*pp)->Theta() > 20*TMath::DegToRad())
                root_node->Fill( GoatPlutoPair( *(*gp), *(*pp)));
        }
    }


}

void fPlutoGoat::Display()
{
    root_node->Draw();
}
