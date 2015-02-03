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
           << " Apparatus=" << t.Apparatus()
           << " VetoEnergy=" << t.VetoEnergy()
           << " MWPC0Energy=" << t.MWPC0Energy()
           << " MWPC1Energy=" << t.MWPC1Energy();
    return stream;
}

ostream& operator<<(ostream &stream, const Track* t)
{
   stream << *t;
   return stream;
}


ostream &operator<<(ostream &stream, const std::shared_ptr<const Track> &t)
{
    stream << *t;
    return stream;
}
