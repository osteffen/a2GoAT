#ifndef FPLUTOGOAT_H
#define FPLUTOGOAT_H

#include "fPhysics.h"
#include "HistFiller.h"
#include "PParticle.h"
#include "GTreeGoatParticle.h"

class fPlutoGoat: public fPhysicsCode {
protected:


    typedef std::pair<const GParticle&, const PParticle&> GoatPlutoPair;

    struct fGoatID {
        typedef GoatPlutoPair DataType;
        Int_t operator() ( DataType data ) {
            return data.first.ID();
        }
    };

    struct fPlutoID {
        typedef GoatPlutoPair DataType;
        Int_t operator() ( DataType data ) {
            return data.second.ID();
        }
    };

    typedef TH2Filler< f2D1< fPlutoID, fGoatID > > RootFillNode;
    RootFillNode* root_node;

public:
    fPlutoGoat();
    virtual ~fPlutoGoat();

    virtual void Run( const fPhysics& p);
    virtual void Display();
};

#endif
