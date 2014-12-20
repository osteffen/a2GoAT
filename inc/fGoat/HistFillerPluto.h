#ifndef HISTFILLERPLUTO_H
#define HISTFILLERPLUTO_H

#include "PParticle.h"
#include "TMath.h"

namespace formulaPParticle {

struct Ek {
    typedef const PParticle& DataType;

    double operator() ( DataType p ) {
        return p.KE();
    }
};

struct ID {
    typedef const PParticle& DataType;

    int operator() ( DataType p ) {
        return p.ID();
    }
};

struct Theta {
    typedef const PParticle& DataType;

    double operator() ( DataType p ) {
        return p.Theta() * TMath::RadToDeg();
    }
};

struct Phi {
    typedef const PParticle& DataType;

    double operator() ( DataType p ) {
        return p.Phi() * TMath::RadToDeg();
    }
};

struct E {
    typedef const PParticle& DataType;

    double operator() ( DataType p ) {
        return p.E();
    }
};

}

#endif
