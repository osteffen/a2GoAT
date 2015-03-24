#include "Event.h"
#include <vector>

template <typename T>
std::vector<const T*> make_pointerlist(const std::vector<T>& v) {
    std::vector<const T*> vp;
    vp.reserve(v.size());
    for( auto& e : v ) {
        vp.push_back(&e);
    }
    return std::move(vp);
}

void ant::Event::Finalize()
{
    ref_tracks = make_pointerlist(tracks);
    ref_particles = make_pointerlist(particles);
    ref_mctrue = make_pointerlist(mctrue);
    ref_taggerhits = make_pointerlist(taggerhits);

    for( auto& p : particles ) {
        particles_by_type[&p.Type()].push_back(&p);
    }

    ref_mctruefinalstate.reserve(mctrue.size());
    for( auto& p : mctrue ) {
        if( p.IsFinalState() )
            ref_mctruefinalstate.push_back(&p);
    }
}

std::ostream &ant::Event::Print(std::ostream &stream) const
{

    stream << "------------------------------\n";

    for( auto& taggerhit : taggerhits ) {
        stream << taggerhit << '\n';
    }

    for( auto& track : tracks ) {
        stream << track << '\n';
    }

    stream << '\n';

    for( auto& rec : particles ) {
        stream << rec << '\n';
    }

    stream << '\n';

    for( auto& mc : mctrue ) {
        stream << mc << '\n';
    }

    stream << '\n';

    stream << trigger << "\n";

    stream << "------------------------------\n";

    return stream;

}
