#ifndef PRINTABLE_H
#define PRINTABLE_H

#include <ostream>

namespace ant {

class printable_traits {
public:
    virtual std::ostream& Print( std::ostream& stream ) const =0;
};

}

static std::ostream& operator<< (std::ostream& stream, const ant::printable_traits& printable) {
    return printable.Print(stream);
}

static std::ostream& operator<< (std::ostream& stream, const ant::printable_traits* printable) {
    return printable->Print(stream);
}
#endif
