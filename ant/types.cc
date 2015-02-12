#include "types.h"

using namespace std;
using namespace ant;

ostream& operator<<(ostream &stream, const detector_t& app)
{
   switch(app) {
   case detector_t::CB:
       stream << "CB"; break;
   case detector_t::TAPS:
       stream << "TAPS"; break;
   default:
       stream << "No string representation"; break;
   }
   return stream;
}
