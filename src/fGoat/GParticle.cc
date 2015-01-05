#include "GParticle.h"
#include "TVector3.h"
#include <iostream>
using namespace std;
// TODO: Fix all of this
void GParticle::SetEThetaPhi(const Double_t E, const Double_t theta, const Double_t phi)
{
    const Double_t Et = E + mass;
    const Double_t p = sqrt(Et*Et - mass*mass);

    TVector3 v(p, 0.0, 0.0);
    v.SetPtThetaPhi(p, theta, phi);

    lv = TLorentzVector(v, Et);
}

std::map<int, GParticleDatabase::GParticleDatabaseRecord> makeDatabase() {
    std::map<int, GParticleDatabase::GParticleDatabaseRecord> m;
    m[-1] = { "fail", "fail" };
    m[ 0] = { "rootino", "rootino" };
    m[ 1] = { "photon", "#gamma" };
    m[ 2] = { "electron", "e^{#pm}" };
    m[ 3] = { "pion", "#pi^{#pm}" };
    m[ 4] = { "proton", "p" };
    return m;
}

const std::map<int, GParticleDatabase::GParticleDatabaseRecord> GParticleDatabase::GP = makeDatabase();


void GParticleDatabase::SetBinNames(TH1* hist) {
    for (auto p = GP.begin(); p!=GP.end(); ++p) {
        const Int_t bin = hist->FindBin(p->first);
        hist->GetXaxis()->SetBinLabel(bin, p->second.printname.c_str());
    }
}

const GParticle GParticleDatabase::GFail(0.0, 0.0, 0.0, 0, 0.0, 0, 0.0, 0.0,0.0, -1, 0.0, false);
