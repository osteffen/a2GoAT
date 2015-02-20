#include "counter.h"

std::ostream &ant::counter::Print(std::ostream &stream) const
{
    stream << title << " = " << count;
    return stream;
}
