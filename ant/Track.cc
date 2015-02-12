#include "Track.h"

using namespace std;
using namespace ant;

ostream& Track::Print(ostream &stream) const
{
    stream << "Track "
           << " ClusterEnergy=" << ClusterEnergy()
           << " Theta=" << Theta()
           << " Phi=" << Phi()
           << " Time=" << Time()
           << " ClusterSize=" << ClusterSize()
           << " Apparatus=" << Detector()
           << " VetoEnergy=" << VetoEnergy()
           << " MWPC0Energy=" << MWPC0Energy()
           << " MWPC1Energy=" << MWPC1Energy();
    return stream;
}
