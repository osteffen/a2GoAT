#ifndef TAPS_H
#define TAPS_H

#include "base/types.h"

#include "TVector3.h"

namespace ant {
namespace detectors {

class TAPS {
protected:
        static double z_pos;
public:

    enum class ElementType {
        BaF2,
        PbWO4
    };

    enum class ElementShape {
        Hexagon,
        QuaterUL,
        QuaterUR,
        QuaterDR,
        QuaterDL
    };

    class Element {
    protected:
        index_t index;

        friend class TAPS;

        Element(index_t i): index(i) {}

    public:
        index_t Index() const { return index; }
        radian_t Theta() const;
        radian_t Phi() const;
        TVector3 Position() const;
        double X() const;
        double Y() const;
        double DistanceFromBeam() const;
        ElementType Type() const;
        ElementShape Shape() const;
    };

protected:

    struct dbentry {
        double x;
        double y;
        ElementType type;
        ElementShape shape;
    };

    static const std::vector<dbentry> elementdb;


public:

    static unsigned int NumberOfElements();

    static Element GetElement(index_t index);

    static void SetZPos(double z);
    static double GetZPos();

};

}
}

#endif
