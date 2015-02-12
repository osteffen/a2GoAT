#include "types.h"

using namespace std;
using namespace ant;

const detector_t detector_t::None;
const detector_t detector_t::NaI(1);
const detector_t detector_t::PID(2);
const detector_t detector_t::MWPC(4);
const detector_t detector_t::BaF2(8);
const detector_t detector_t::PbWO4(16);
const detector_t detector_t::Veto(32);

ostream& detector_t::Print(ostream& stream) const
{
    stream << "not implemented";
    return stream;
}
