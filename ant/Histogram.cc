#include "Histogram.h"

#include "TH1D.h"
#include "TH2D.h"

#include <sstream>
#include <iomanip>

using namespace ant;
using namespace std;


const EColor &HistogramFactory::GetNextColor()
{
    ++color;
    if( color == colors.end() )
        color = colors.begin();
    return *color;
}

const UInt_t HistogramFactory::GetNextHistnum()
{
    return histnum++;
}

HistogramFactory::HistogramFactory(const std::string& prefix):
    color(colors.begin()),
    histnum(0),
    name_prefix(prefix)
{
}


TH1D *HistogramFactory::Make1D(const string &title, const string &xlabel, const string &ylabel, const BinSettings& bins, const string &name)
{
    TH1D* h = new TH1D( GetNextHistName(name).c_str(), title.c_str(), bins.Bins(), bins.Start(), bins.Stop());
    h->SetXTitle(xlabel.c_str());
    h->SetYTitle(ylabel.c_str());
    h->SetLineColor(GetNextColor());
    return h;
}

TH2D *HistogramFactory::Make2D(const std::string& title, const std::string& xlabel, const std::string& ylabel, const BinSettings& xbins, const BinSettings& ybins, const string &name)
{
    TH2D* h = new TH2D( GetNextHistName(name).c_str(), title.c_str(), xbins.Bins(), xbins.Start(), xbins.Stop(), ybins.Bins(), ybins.Start(), ybins.Stop());
    h->SetXTitle(xlabel.c_str());
    h->SetYTitle(ylabel.c_str());
    return h;
}

void HistogramFactory::ApplySettings(TH1D *hist, const string &title, const string &xlabel, const string &ylabel)
{
    if(!title.empty())
        hist->SetTitle(title.c_str());
    if(!xlabel.empty())
        hist->SetXTitle(xlabel.c_str());
    if(!ylabel.empty())
        hist->SetYTitle(ylabel.c_str());
    hist->SetLineColor(GetNextColor());
}

void HistogramFactory::ApplySettings(TH2D *hist, const string &title, const string &xlabel, const string &ylabel)
{
    if(!title.empty())
        hist->SetTitle(title.c_str());
    if(!xlabel.empty())
        hist->SetXTitle(xlabel.c_str());
    if(!ylabel.empty())
        hist->SetYTitle(ylabel.c_str());
}

string HistogramFactory::GetNextHistName(const std::string& name)
{
    stringstream s;

    s << name_prefix << "_";

    if(name.empty()) {
        s << setfill('0') << setw(3) << GetNextHistnum();
    } else {
        s << name;
    }
    return s.str();
}

void HistogramFactory::ResetColors()
{
    color=colors.begin();
}

// Color set for 1D histogram lines.
std::vector<EColor> HistogramFactory::colors = {kBlue, kRed, kGreen, kMagenta, kCyan, kBlack};
