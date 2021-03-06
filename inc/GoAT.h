#ifndef __GoAT_h__
#define __GoAT_h__

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string> 


#include "GSort.h"
#include "GParticleReconstruction.h"
#include "GMesonReconstruction.h"


class	GoAT : public GSort, public GParticleReconstruction, public GMesonReconstruction
{
private:
	Int_t	UsePeriodMacro;
	Int_t 	period;
	
	Bool_t 	UseParticleReconstruction;
    Bool_t 	UseMesonReconstruction;

	Int_t 	nEvents_written;
protected:
    virtual void 	ProcessEvent();
    virtual Bool_t	Start();


public:
    GoAT();
    virtual ~GoAT();

    virtual Bool_t	Init(const char* configfile);
};
#endif
