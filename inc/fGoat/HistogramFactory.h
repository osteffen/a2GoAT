#ifndef HISTOGRAMFACOTRY_H
#define HISTOGRAMFACOTRY_H
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TColor.h"
#include <vector>
#include <string>

class HistogramFactory {
private:
    std::vector<EColor> colors;
    std::vector<EColor>::const_iterator color;
    UInt_t histnum;

    const EColor& GetNextColor();
    const UInt_t GetNextHistnum();

    static HistogramFactory* _instance;

public:
    HistogramFactory();
    virtual ~HistogramFactory() {}

    virtual TH1D* Make1D( const std::string& name, const std::string& title, const std::string& xlabel, const std::string& ylabel, const UInt_t nxbins, const Double_t xmin=-1, const Double_t xmax=-1);
    virtual TH1D* Make1DN( const std::string& title, const std::string& xlabel, const std::string& ylabel, const UInt_t nxbins, const Double_t xmin=-1, const Double_t xmax=-1);

    virtual TH2D* Make2D( const std::string& name, const std::string& title, const std::string& xlabel, const std::string& ylabel, const UInt_t nxbins, const Double_t xmin, const Double_t xmax,const UInt_t nybins, const Double_t ymin, const Double_t ymax);
    virtual TH2D* Make2DN(const std::string& title, const std::string& xlabel, const std::string& ylabel, const UInt_t nxbins, const Double_t xmin, const Double_t xmax, const UInt_t nybins, const Double_t ymin, const Double_t ymax);

    virtual void ApplySettings(TH1D* hist, const std::string& title="", const std::string& xlabel="", const std::string& ylabel="");
    virtual void ApplySettings(TH2D* hist, const std::string& title="", const std::string& xlabel="", const std::string& ylabel="");

    virtual std::string GetNextHistName();

    virtual void ResetColors();

    static HistogramFactory* GetInstance();
    static HistogramFactory* GetInstanceFresh();

};


#endif
