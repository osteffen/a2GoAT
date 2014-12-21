#ifndef GTREEGOATPARTICLE_H
#define GTREEGOATPARTICLE_H

#include "Rtypes.h"
#include "TLorentzVector.h"
#include <list>
#include <map>

class GTreeParticle;

class GVirtualParticleTrack {
public:
    virtual Double_t Energy() const =0;
    virtual Double_t Theta() const =0;
    virtual Double_t Phi() const =0;
    virtual Int_t    Apparatus() const =0;
    virtual Double_t Time() const =0;
    virtual Double_t vetoEnergy() const =0;
    virtual UInt_t   clusterSize() const =0;
    virtual Double_t MWPC0Enery() const =0;
    virtual Double_t MWPC1Energy() const =0;
};

class GParticle: public GVirtualParticleTrack {
protected:
    TLorentzVector  lv;
    Int_t           apparatus;
    Double_t        time;
    Double_t        vetoenergy;
    UInt_t          clustersize;
    Double_t        mwpc0energy;
    Double_t        mwpc1energy;
    UInt_t          pid;
    Double_t        mass;
    bool            ischarged;

public:
    GParticle( const GVirtualParticleTrack* pt, UInt_t id, Double_t m, bool charged ):
        apparatus(pt->Apparatus()),
        time(pt->Time()),
        vetoenergy(pt->vetoEnergy()),
        clustersize(pt->clusterSize()),
        mwpc0energy(pt->MWPC0Enery()),
        mwpc1energy(pt->MWPC1Energy()),
        pid(id),
        mass(m),
        ischarged(charged)
    {
        SetEThetaPhi(pt->Energy(), pt->Theta(), pt->Phi());
    }

    GParticle( Double_t e, Double_t theta, Double_t phi, Int_t app, Double_t t, UInt_t size, Double_t veto, Double_t mwpc0, Double_t mwpc1, UInt_t id, Double_t m, bool charged):
        apparatus(app),
        time(t),
        vetoenergy(veto),
        clustersize(size),
        mwpc0energy(mwpc0),
        mwpc1energy(mwpc1),
        pid(id),
        mass(m),
        ischarged(charged)
    {
        SetEThetaPhi(e,theta,phi);
    }

    GParticle( const TLorentzVector& lvect, Int_t app, Double_t t, UInt_t size, Double_t veto, Double_t mwpc0, Double_t mwpc1, UInt_t id, Double_t m, bool charged):
        lv(lvect),
        apparatus(app),
        time(t),
        vetoenergy(veto),
        clustersize(size),
        mwpc0energy(mwpc0),
        mwpc1energy(mwpc1),
        pid(id),
        mass(m),
        ischarged(charged)
    {
    }

    virtual ~GParticle() {}

    virtual Double_t Energy()    const { return lv.E(); }
    virtual Double_t Theta()     const { return lv.Theta(); }
    virtual Double_t Phi()       const { return lv.Phi(); }
    virtual Int_t    Apparatus() const { return apparatus; }
    virtual Double_t Time()      const { return time; }
    virtual UInt_t   clusterSize() const { return clustersize; }
    virtual Double_t vetoEnergy() const { return vetoenergy; }
    virtual const TLorentzVector& Vector() const { return lv; }
    virtual void SetEThetaPhi(const Double_t E, const Double_t theta, const Double_t phi);
    virtual UInt_t ID()             const { return pid; }
    virtual Double_t MWPC0Enery() const { return mwpc0energy; }
    virtual Double_t MWPC1Energy() const { return mwpc1energy; }

};


class GGoatParticleManager {
public:
    typedef std::list<const GParticle*> ParticleList;

protected:

    struct ParticleType {
        Int_t id;
        Double_t mass;
        bool ischagred;
    };

    std::map<const GTreeParticle*, ParticleType> trees;

    ParticleList particles;

public:
    GGoatParticleManager();

    virtual ~GGoatParticleManager();

    virtual void Clear();

    virtual const ParticleList& Particles() const;
    virtual void CollectParticles();

    virtual void AddTree( const GTreeParticle* tree, Int_t particle_id, const Double_t particle_mass, const bool charged );

};

#endif
