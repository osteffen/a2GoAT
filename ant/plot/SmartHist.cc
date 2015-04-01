#include "plot/SmartHist.h"

template<>
ant::SmartHist<const std::string&> ant::SmartHist<const std::string&>::makeHist(
    const std::string& title,
    const std::string& xlabel,
    const std::string& ylabel,
    const BinSettings& bins,
    const std::string& name)
{
    return move(makeHist([] (const std::string& data) -> const char* { return data.c_str();}, title, xlabel, ylabel, bins, name));
}
