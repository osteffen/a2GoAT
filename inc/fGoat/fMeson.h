#ifndef FMESON_H
#define FMESON_H

#include "fPhysics.h"
#include "TH1D.h"

class fMesonTest: public fPhysicsCode {
protected:
    TH1D* h_2GIM;
    TH1D* h_3GIM;

public:
    fMesonTest();
    virtual ~fMesonTest() {}

    virtual void Run( const fPhysics& p);
    virtual void Display();

};

#endif
