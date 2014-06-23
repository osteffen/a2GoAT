#include "GTreeMeson.h"
#include "GTreeManager.h"


GTreeMeson::GTreeMeson(GTreeManager *Manager, const TString& _Name)    :
    GTreeParticle(Manager, _Name),
    subPhotons(new TClonesArray("TClonesArray", GTreeParticle_MaxEntries)),
    subChargedPi(new TClonesArray("TClonesArray", GTreeParticle_MaxEntries))
{
    for(int i=0; i<GTreeParticle_MaxEntries; i++)
    {
        nSubParticles[i]  = 0;
        nSubPhotons[i]    = 0;
        nSubChargedPi[i]  = 0;
    }
}

GTreeMeson::~GTreeMeson()
{
    if (subPhotons)
    {
        for(int i=0; i<subPhotons->GetEntries(); i++)
            ((TClonesArray*)subPhotons->At(i))->Delete();
        delete subPhotons;
    }
    if (subChargedPi)
    {
        for(int i=0; i<subChargedPi->GetEntries(); i++)
            ((TClonesArray*)subChargedPi->At(i))->Delete();
        delete subChargedPi;
    }
}

void    GTreeMeson::Clear()
{
    GTreeParticle::Clear();
    for(int i=0; i<subPhotons->GetEntries(); i++)
        ((TClonesArray*)subPhotons->At(i))->Clear("C");
    subPhotons->Clear();
    for(int i=0; i<subPhotons->GetEntries(); i++)
        ((TClonesArray*)subPhotons->At(i))->Clear("C");
    subChargedPi->Clear();
}

void    GTreeMeson::SetBranchAdresses()
{
    GTreeParticle::SetBranchAdresses();
    tree_in->SetBranchAddress("nSubParticles", nSubParticles);
    tree_in->SetBranchAddress("nSubPhotons", nSubPhotons);
    tree_in->SetBranchAddress("nSubChargedPi", nSubChargedPi);
    tree_in->SetBranchAddress("subPhotons.", &subPhotons);
    tree_in->SetBranchAddress("subChargedPi.", &subChargedPi);
}

void    GTreeMeson::SetBranches()
{
    GTreeParticle::SetBranches();
    tree_out->Branch("nSubParticles", nSubParticles, "nSubParticles[nParticles]/b");
    tree_out->Branch("nSubPhotons", nSubPhotons,"nSubParticles[nParticles]/b");
    tree_out->Branch("nSubChargedPi", nSubChargedPi,"nSubParticles[nParticles]/b");
    tree_out->Branch("subPhotons.", &subPhotons, 32, 0);
    //subPhotons->BypassStreamer();
    tree_out->Branch("subChargedPi.", &subChargedPi, 32, 0);
    //subChargedPi->BypassStreamer();
}

void    GTreeMeson::AddParticle(const Int_t _NSubPhotons, Int_t* subPhotons_index, TLorentzVector** subPhotons_list, const Int_t _NSubChargedPi, Int_t* subChargedPi_index, TLorentzVector** subChargedPi_list)
{
    new((*subPhotons)[nParticles]) TClonesArray("TLorentzVector", _NSubPhotons);
    new((*subChargedPi)[nParticles]) TClonesArray("TLorentzVector", _NSubChargedPi);
    Apparatus[nParticles]    = GTreeRawEvent::APPARATUS_NONE;
    time[nParticles]         = 0;
    clusterSize[nParticles]  = 0;
    d_E[nParticles]          = 0;
    WC0_E[nParticles]        = 0;
    WC1_E[nParticles]        = 0;

    nSubParticles[nParticles]  = _NSubPhotons + _NSubChargedPi;
    nSubPhotons[nParticles]    = _NSubPhotons;
    nSubChargedPi[nParticles]  = _NSubChargedPi;
    TLorentzVector  sum;
    for(int i=0; i<_NSubPhotons; i++)
    {
        sum += *subPhotons_list[i];
        new((*((TClonesArray*)subPhotons->At(nParticles)))[i]) TLorentzVector(*subPhotons_list[i]);
        Apparatus[nParticles]    = Apparatus[nParticles] | manager->photons->GetApparatus(subPhotons_index[i]);
        time[nParticles]         += manager->photons->GetTime(subPhotons_index[i]);
        clusterSize[nParticles]  += manager->photons->GetClusterSize(subPhotons_index[i]);
        d_E[nParticles]          += manager->photons->Get_dE(subPhotons_index[i]);
        WC0_E[nParticles]        += manager->photons->GetWC0_E(subPhotons_index[i]);
        WC1_E[nParticles]        += manager->photons->GetWC1_E(subPhotons_index[i]);
    }
    for(int i=0; i<_NSubChargedPi; i++)
    {
        sum += *subChargedPi_list[i];
        new((*((TClonesArray*)subChargedPi->At(nParticles)))[i]) TLorentzVector(*subChargedPi_list[i]);
        Apparatus[nParticles]    = Apparatus[nParticles] | manager->chargedPi->GetApparatus(subPhotons_index[i]);
        time[nParticles]         += manager->chargedPi->GetTime(subPhotons_index[i]);
        clusterSize[nParticles]  += manager->chargedPi->GetClusterSize(subPhotons_index[i]);
        d_E[nParticles]          += manager->chargedPi->Get_dE(subPhotons_index[i]);
        WC0_E[nParticles]        += manager->chargedPi->GetWC0_E(subPhotons_index[i]);
        WC1_E[nParticles]        += manager->chargedPi->GetWC1_E(subPhotons_index[i]);
    }
    time[nParticles]         /= nSubParticles[nParticles];
    new((*particles)[nParticles]) TLorentzVector(sum);
    nParticles++;
    manager->countReconstructed++;
}


void    GTreeMeson::AddParticle(const Int_t subParticle_index0, const TLorentzVector& subParticle0, const Int_t pdg0, const Int_t subParticle_index1, const TLorentzVector& subParticle1, const Int_t pdg1)
{
    new((*subPhotons)[nParticles]) TClonesArray("TLorentzVector", 2);
    new((*subChargedPi)[nParticles]) TClonesArray("TLorentzVector", 2);

    nSubParticles[nParticles]  = 2;
    nSubPhotons[nParticles]    = 0;
    nSubChargedPi[nParticles]  = 0;

    if(pdg0 == manager->pdgDB->GetParticle("gamma")->PdgCode())
    {
        nSubPhotons[nParticles]++;
        new((*((TClonesArray*)subPhotons->At(nParticles)))[0]) TLorentzVector(subParticle0);
        Apparatus[nParticles]    = manager->photons->GetApparatus(subParticle_index0);
        time[nParticles]         = manager->photons->GetTime(subParticle_index0);
        clusterSize[nParticles]  = manager->photons->GetClusterSize(subParticle_index0);
        d_E[nParticles]          = manager->photons->Get_dE(subParticle_index0);
        WC0_E[nParticles]        = manager->photons->GetWC0_E(subParticle_index0);
        WC1_E[nParticles]        = manager->photons->GetWC1_E(subParticle_index0);
    }
    else
    {
        nSubChargedPi[nParticles]++;
        new((*((TClonesArray*)subChargedPi->At(nParticles)))[0]) TLorentzVector(subParticle0);
        Apparatus[nParticles]    = manager->chargedPi->GetApparatus(subParticle_index0);
        time[nParticles]         = manager->chargedPi->GetTime(subParticle_index0);
        clusterSize[nParticles]  = manager->chargedPi->GetClusterSize(subParticle_index0);
        d_E[nParticles]          = manager->chargedPi->Get_dE(subParticle_index0);
        WC0_E[nParticles]        = manager->chargedPi->GetWC0_E(subParticle_index0);
        WC1_E[nParticles]        = manager->chargedPi->GetWC1_E(subParticle_index0);
    }
    if(pdg1 == manager->pdgDB->GetParticle("gamma")->PdgCode())
    {
        nSubPhotons[nParticles]++;
        new((*((TClonesArray*)subPhotons->At(nParticles)))[1]) TLorentzVector(subParticle1);
        Apparatus[nParticles]    = Apparatus[nParticles] | manager->photons->GetApparatus(subParticle_index1);
        time[nParticles]         += manager->photons->GetTime(subParticle_index1);
        clusterSize[nParticles]  += manager->photons->GetClusterSize(subParticle_index1);
        d_E[nParticles]          += manager->photons->Get_dE(subParticle_index1);
        WC0_E[nParticles]        += manager->photons->GetWC0_E(subParticle_index1);
        WC1_E[nParticles]        += manager->photons->GetWC1_E(subParticle_index1);
    }
    else
    {
        nSubChargedPi[nParticles]++;
        new((*((TClonesArray*)subChargedPi->At(nParticles)))[1]) TLorentzVector(subParticle1);
        Apparatus[nParticles]    = Apparatus[nParticles] | manager->chargedPi->GetApparatus(subParticle_index1);
        time[nParticles]         += manager->chargedPi->GetTime(subParticle_index1);
        clusterSize[nParticles]  += manager->chargedPi->GetClusterSize(subParticle_index1);
        d_E[nParticles]          += manager->chargedPi->Get_dE(subParticle_index1);
        WC0_E[nParticles]        += manager->chargedPi->GetWC0_E(subParticle_index1);
        WC1_E[nParticles]        += manager->chargedPi->GetWC1_E(subParticle_index1);
    }
    time[nParticles]         /= 2;
    new((*particles)[nParticles]) TLorentzVector(subParticle0 + subParticle1);
    nParticles++;
    manager->countReconstructed++;
}
