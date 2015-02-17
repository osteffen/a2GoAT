#ifndef BASIC_H
#define BASIC_H

#include "AntPhysics.h"
#include <vector>

class TH1D;
class TH2D;

namespace ant {
namespace analysis {

class Basic: public Physics {
protected:
    TH2D* banana;
    TH1D* particles;

    typedef std::vector<TH1D*> Hist1DList;

    Hist1DList ngammaim;

public:
    Basic(const mev_t energy_scale=1000.0);
    virtual ~Basic() {}
    void ProcessEvent(const Event &event);
    void Finish();
    void ShowResult();
};

}
}
#endif
