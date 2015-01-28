#ifndef __GTree_h__
#define __GTree_h__


#include <iostream>
#include <stdlib.h>

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <exception>


class   GTreeManager;

class  GTree    : protected TObject
{
public:
    enum
    {
        FLAG_CLOSED           = 0,
        FLAG_OPENFORINPUT     = 1,
        FLAG_OPENFOROUTPUT    = 2
    };

    friend  class   GTreeManager;

    class e_brach_not_found: public std::exception {
    protected:
        std::string fbranchname;
    public:
        e_brach_not_found( const std::string& branchname=""): fbranchname(branchname) {}

        virtual const char* what() const throw()
          {
            std::string message = "Can't find branch named ";
            message + fbranchname;
            return message.c_str();
          }
    };

private:
    TString         name;
    Bool_t          correlatedToScalerRead;
    Bool_t          singleRead;
    Int_t           status;

    void    GetEntryFast(const UInt_t index)    {inputTree->GetEntry(index);}

protected:
    TTree*          inputTree;
    TTree*          outputTree;
    GTreeManager*   manager;
    bool            saveToFile;

    virtual void    SetBranchAdresses() = 0;
    virtual void    SetBranches() = 0;

    template <class T>
    void SetBranchAddessChecked( TTree* tree, const std::string& branchname, T& branch, bool required=true) {
        Int_t result = tree->SetBranchAddress( branchname.c_str(), &branch);
        if( result != 0 ) {
            if( required )
                throw e_brach_not_found(branchname);
        }
    }

public:
    GTree(GTreeManager* Manager, const TString& _Name, const Bool_t CorrelatedToScalerRead = kFALSE, const Bool_t SingleRead = kFALSE);
    virtual ~GTree();

    virtual void        Clear() = 0;
            void        CloseForInput();
            void        CloseForOutput();
            void        Close();
            void        Fill();
    inline  Bool_t      GetEntry(const UInt_t index);
    const   char*       GetName() const {return name.Data();}
            UInt_t      GetNEntries()   { if(IsOpenForInput()) return inputTree->GetEntries(); return 0;}
            Bool_t      IsClosed()          {return !status;}
            Bool_t      IsOpenForInput()    {return status & FLAG_OPENFORINPUT;}
            Bool_t      IsOpenForOutput()   {return status & FLAG_OPENFOROUTPUT;}
            Bool_t      OpenForInput();
            Bool_t      OpenForOutput();
    virtual void        Print() const;
    virtual Bool_t      Write();

};

Bool_t  GTree::GetEntry(const UInt_t index)
{
    if(index > inputTree->GetEntries())
        return kFALSE;
    inputTree->GetEntry(index);
}


#endif
