#include "HistogramFactory.h"

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

HistogramFactory::HistogramFactory():
    histnum(0)
{
    colors.push_back(kBlue);
    colors.push_back(kRed);
    colors.push_back(kGreen);
    colors.push_back(kMagenta);
    colors.push_back(kCyan);
    colors.push_back(kBlack);
    color=colors.begin();
}

TH1D *HistogramFactory::Make1D(const string &name, const string &title, const string &xlabel, const string &ylabel, const UInt_t nxbins, const Double_t xmin, const Double_t xmax)
{
    TH1D* h = new TH1D( name.c_str(), title.c_str(), nxbins, xmin, xmax);
    ApplySettings(h, title, xlabel, ylabel);
    return h;
}

TH1D *HistogramFactory::Make1DN(const string &title, const string &xlabel, const string &ylabel, const UInt_t nxbins, const Double_t xmin, const Double_t xmax)
{
    return Make1D( GetNextHistName(), title, xlabel, ylabel, nxbins, xmin, xmax);
}

TH2D *HistogramFactory::Make2D(const string &name, const string &title, const string &xlabel, const string &ylabel, const UInt_t nxbins, const Double_t xmin, const Double_t xmax, const UInt_t nybins, const Double_t ymin, const Double_t ymax)
{
    TH2D* h = new TH2D( name.c_str(), title.c_str(), nxbins, xmin, xmax, nybins, ymin, ymax);
    ApplySettings(h, title, xlabel, ylabel);
    return h;
}

TH2D *HistogramFactory::Make2DN(const string &title, const string &xlabel, const string &ylabel, const UInt_t nxbins, const Double_t xmin, const Double_t xmax, const UInt_t nybins, const Double_t ymin, const Double_t ymax)
{
    return Make2D( GetNextHistName(), title, xlabel, ylabel, nxbins, xmin, xmax, nybins, ymin, ymax);
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
    //hist->SetFillColor(kGray);
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

string HistogramFactory::GetNextHistName()
{
    return std::string( Form("_hist%05d", GetNextHistnum()) );
}

void HistogramFactory::ResetColors()
{
    color=colors.begin();
}

HistogramFactory *HistogramFactory::GetInstance()
{
    return _instance;
}

HistogramFactory *HistogramFactory::GetInstanceFresh()
{
    _instance->ResetColors();
    return _instance;
}

HistogramFactory* HistogramFactory::_instance = new HistogramFactory();
