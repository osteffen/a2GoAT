#ifndef HISTOGRAMFACTORIES_H
#define HISTOGRAMFACTORIES_H

#include "plot/SmartHist.h"
#include "Particle.h"

#include <string>

namespace ant {

namespace SmartHistFactory {

ant::SmartHist<const ParticlePtr&> InvariantMass(
        const std::string& title,
        const std::string& xlabel="M [MeV]",
        const std::string& ylabel="",
        ant::BinSettings bins=BinSettings(1000),
        const std::string& name="");

ant::SmartHist<const ParticlePtr&> ThetaAngle(
        const std::string& title,
        const std::string& xlabel="#theta [#circ]",
        const std::string& ylabel="",
        ant::BinSettings bins=BinSettings(180),
        const std::string& name="");

ant::SmartHist<const ParticlePtr&> KinEnergy(
        const std::string& title,
        const std::string& xlabel="#E_{k} [MeV]",
        const std::string& ylabel="",
        ant::BinSettings bins=BinSettings(1000),
        const std::string& name="");
}

}

#endif
