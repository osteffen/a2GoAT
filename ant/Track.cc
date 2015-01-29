#include "Track.h"

using namespace std;
using namespace ant;


ostream& operator<<(ostream &stream, const Track& t)
{
    stream << "Track "
           << " ClusterEnergy=" << t.ClusterEnergy()
           << " Theta=" << t.Theta()
           << " Phi=" << t.Phi()
           << " Time=" << t.Time()
           << " ClusterSize=" << t.ClusterSize()
           << " CentralCrystal=" << t.CentralCrystal()
           << " CentralVeto=" << t.CentralVeto()
           << " Apparatus=" << t.Apparatus()
           << " VetoEnergy=" << t.VetoEnergy()
           << " MWPC0Energy=" << t.MWPC0Energy()
           << " MWPC1Energy=" << t.MWPC1Energy();
    return stream;
}
