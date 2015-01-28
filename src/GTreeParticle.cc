#include "GTreeParticle.h"
#include "GTreeManager.h"

#include <TCanvas.h>

using namespace std;


GTreeParticle::GTreeParticle(GTreeManager *Manager, const TString& _Name)    :
    GTree(Manager,_Name),
    nParticles(0),
    particles(new TClonesArray("TLorentzVector", GTreeParticle_MAX))
{
    for(Int_t i=0; i<GTreeParticle_MAX; i++)
    {
        apparatus[i]    = 0;
        time[i]         = 0;
        clusterSize[i]  = 0;
        vetoEnergy[i]   = 0;
        MWPC0Energy[i]  = 0;
        MWPC1Energy[i]  = 0;
    }
}

GTreeParticle::~GTreeParticle()
{
    if (particles)  delete particles;
}

void    GTreeParticle::SetBranchAdresses()
{
    SetBranchAddessChecked(inputTree, "nParticles", nParticles);
    SetBranchAddessChecked(inputTree, "particles", particles);
    SetBranchAddessChecked(inputTree, "apparatus", apparatus);
    SetBranchAddessChecked(inputTree, "time", time);
    SetBranchAddessChecked(inputTree, "clusterSize", clusterSize);
    SetBranchAddessChecked(inputTree, "vetoEnergy", vetoEnergy);
    SetBranchAddessChecked(inputTree, "MWPC0Energy", MWPC0Energy);
    SetBranchAddessChecked(inputTree, "MWPC1Energy", MWPC1Energy);
}

void    GTreeParticle::SetBranches()
{
    outputTree->Branch("nParticles",&nParticles, "nParticles/I");
    outputTree->Branch("particles", &particles, 32000, 0);
    outputTree->Branch("apparatus", apparatus, "apparatus[nParticles]/I");
    outputTree->Branch("time", time, "time[nParticles]/D");
    outputTree->Branch("clusterSize", clusterSize, "clusterSize[nParticles]/I");
    outputTree->Branch("vetoEnergy", vetoEnergy, "vetoEnergy[nParticles]/D");
    outputTree->Branch("MWPC0Energy", MWPC0Energy, "MWPC0Energy[nParticles]/D");
    outputTree->Branch("MWPC1Energy", MWPC1Energy, "MWPC1Energy[nParticles]/D");
}


Bool_t	GTreeParticle::Write()
{
    if(!manager->outputFile)          return kFALSE;
    if(!outputTree)                   return kFALSE;
    if(!IsOpenForOutput())          return kFALSE;

    TCanvas c1("c1");
    c1.cd();
    outputTree->Draw("nParticles>>htmp(1,0,1)");
    TH1F* hist = (TH1F*)gDirectory->Get("htmp");
    //std::cout << hist->GetEntries()<< "\t" << hist->GetBinContent(hist->GetBin(1)) << std::endl;
    if(hist->GetEntries() == hist->GetBinContent(hist->GetBin(1)))
    {
        std::cout << "tree " << GetName() << " has not been written to disk. All Events have 0 " << GetName() << "." << std::endl;
        saveToFile  = false;
        return kTRUE;
    }

    return GTree::Write();
}

void    GTreeParticle::AddParticle(const TLorentzVector& vec, const Int_t _Apparatus, const Double_t _vetoEnergy, const Double_t _MWPC0Energy, const Double_t _MWPC1Energy, const Double_t _Time, const Int_t _ClusterSize)
{
    apparatus[nParticles]   = _Apparatus;
    time[nParticles]        = _Time;
    clusterSize[nParticles] = _ClusterSize;
    vetoEnergy[nParticles]  = _vetoEnergy;
    MWPC0Energy[nParticles] = _MWPC0Energy;
    MWPC1Energy[nParticles] = _MWPC1Energy;
    new((*particles)[nParticles]) TLorentzVector(vec);
    nParticles++;
    manager->countReconstructed++;
}


void    GTreeParticle::RemoveAllParticles()
{
    manager->countReconstructed -= nParticles;
    Clear();
}


void    GTreeParticle::RemoveParticles(const Int_t nIndices, const Int_t* indices)
{
    Int_t*  sort    = new Int_t[nIndices];
    TMath::Sort(nIndices, indices, sort);
    for(Int_t i=0; i<nIndices; i++)
    {
        nParticles--;
        if(sort[i] != nParticles)
        {
            apparatus[sort[i]]   = apparatus[nParticles];
            time[sort[i]]        = time[nParticles];
            clusterSize[sort[i]] = clusterSize[nParticles];
            vetoEnergy[sort[i]]  = vetoEnergy[nParticles];
            MWPC0Energy[sort[i]] = MWPC0Energy[nParticles];
            MWPC1Energy[sort[i]] = MWPC1Energy[nParticles];
            //particles->RemoveAt(sort[i]);
            new((*particles)[sort[i]]) TLorentzVector(*((TLorentzVector*)particles->At(nParticles)));
        }
        particles->RemoveAt(nParticles);
    }
    manager->countReconstructed -= nIndices;
}


void    GTreeParticle::PrintParticle(const Int_t i) const
{
    cout << "\tParticle " << i << ": " << endl;
    cout << "\tPx: " << Particle(i).Px() << "   Py: " << Particle(i).Py() << "   Pz: " << Particle(i).Pz() << "   E: " << Particle(i).E() << endl;
    cout << "\tApparatus: " << apparatus[i] << "   clusterSize: " << clusterSize[i]  << endl;
    cout << "\tvetoEnergy: " << vetoEnergy[i] << "   MWPC0Energy: " << MWPC0Energy[i] << "   MWPC1Energy: " << MWPC1Energy[i]  << endl;
}

void    GTreeParticle::Print() const
{
    GTree::Print();
    cout << "nParticles: " << nParticles << endl;
    for(Int_t i=0; i<nParticles; i++)
        PrintParticle(i);
}
