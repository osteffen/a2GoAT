#include "types.h"

using namespace std;
using namespace ant;

ostream& operator<<(ostream &stream, const apparatus_t& app)
{
   switch(app) {
   case apparatus_t::CB:
       stream << "CB"; break;
   case apparatus_t::TAPS:
       stream << "TAPS"; break;
   default:
       stream << "No string representation"; break;
   }
   return stream;
}
