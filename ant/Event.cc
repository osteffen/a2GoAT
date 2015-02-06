#include "Event.h"


std::ostream &ant::Event::Print(std::ostream &stream) const
{

    stream << "------------------------------\n";

    for( auto& taggerhit : taggerhits ) {
        stream << *taggerhit << '\n';
    }

    for( auto& track : tracks ) {
        stream << *track << '\n';
    }

    stream << '\n';

    for( auto& rec : particles ) {
        stream << *rec << '\n';
    }

    stream << '\n';

    for( auto& mc : mctrue ) {
        stream << *mc << '\n';
    }
    stream << "------------------------------\n";

    return stream;

}
