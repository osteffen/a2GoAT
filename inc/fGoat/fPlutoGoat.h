#ifndef FPLUTOGOAT_H
#define FPLUTOGOAT_H

#include "fPhysics.h"
#include "HistFiller.h"
#include "PParticle.h"
#include "GTreeGoatParticle.h"




class fPlutoGoat: public fPhysicsCode {
protected:
    class ReconstructAs;
    class RootFillNode;

public:
    RootFillNode* root_node;

    fPlutoGoat();
    virtual ~fPlutoGoat();

    virtual void Run( const fPhysics& p);
    virtual void Display();
};

#endif
