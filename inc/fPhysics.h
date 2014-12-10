#ifndef __fPhysics_h__
#define __fPhysics_h__

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "GTreeManager.h"
#include "GTreeA2Geant.h"
#include "PPhysics.h"

class fPhysics;

class fPhysicsCode {
public:
    virtual ~fPhysicsCode() {}
    virtual void Run( const fPhysics& p) =0;
    virtual void Display() =0;
};

class fGeantTestPlot: public fPhysicsCode {
protected:
    TH1D* hCBEsum;

public:
    fGeantTestPlot();
    virtual ~fGeantTestPlot() {}

    virtual void Run( const fPhysics& p);
    virtual void Display();

};

class	fPhysics  : public GTreeManager
{
protected:
    virtual Bool_t      Start();
    virtual void        ProcessEvent();
    virtual void        ProcessScalerRead();
    virtual Bool_t      Write();

    fGeantTestPlot GeantTestPlot;

public:
    fPhysics();
    virtual ~fPhysics();
    virtual Bool_t  Init(const char* configfile);

    const GTreeA2Geant& Geant() const { return *geant; }

    void Display();

};




#endif
