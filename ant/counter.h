#ifndef COUNTER_H
#define COUNTER_H

#include "types.h"
#include "printable.h"
#include <string>

namespace ant {

class counter : public ant::printable_traits {

protected:
    uint64_t count;
    std::string title;
public:
    counter(const std::string& Title, const uint64_t init=0): title(Title), count(init) {}

    void operator() (const uint64_t& inc=1) { count+=inc; }

    bool operator== (const counter& rhs) const { return count==rhs.count; }

    const std::string& Title() const { return title; }

    // printable_traits interface
public:
    std::ostream &Print(std::ostream &stream) const;
};
}

#endif
