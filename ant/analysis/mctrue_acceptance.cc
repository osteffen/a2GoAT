#include "analysis/mctrue_acceptance.h"
#include "plot/root_draw.h"

ant::analysis::MCTrueAcceptance::MCTrueAcceptance():
    Physics("McTrueAcceptance"), events_seen(0)
{
    detect = HistFac.makeHist<std::string>("Geometric Acceptance (MC True)","Particle Type","% events | all particles found", BinSettings(0),"detection");
}

bool ant::analysis::MCTrueAcceptance::AllAccepted(const ParticleList& particles) {
    for( auto& p : particles ) {
        if( geo.DetectorFromAngles(*p) == detector_t::None )
            return false;
    }
    return true;
}

void ant::analysis::MCTrueAcceptance::ProcessEvent(const ant::Event &event)
{

    for( auto& type : ParticleTypeDatabase::DetectableTypes() ) {

        const ParticleList& particles = event.MCTrue().Particles().Get(*type);

        if( particles.size() != 0 && AllAccepted(particles) )
            detect.Fill(type->PrintName(), 1.0);
    }

    events_seen++;

}

void ant::analysis::MCTrueAcceptance::Finish()
{
    detect.Scale(100.0/events_seen);
}

void ant::analysis::MCTrueAcceptance::ShowResult()
{
    canvas("MCTrueAcceptance") << detect << endc;
}

