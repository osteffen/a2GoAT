#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "TColor.h"

#include <vector>
#include <string>
#include "base/interval.h"

class TH1D;
class TH2D;
class TDirectory;

namespace ant {

class BinSettings: public interval<double>  {
protected:
    unsigned int bins;
public:
    BinSettings(unsigned int number_of_bins, double minimum, double maximum): interval<double>(minimum,maximum),
        bins(number_of_bins) {}

    BinSettings(unsigned int number_of_bins): interval<double>(0,number_of_bins),
        bins(number_of_bins) {}

    BinSettings(unsigned int number_of_bins, const interval<double>& i): interval<double>(i), bins(number_of_bins) {}

    virtual ~BinSettings() {}

    const unsigned int& Bins()    const { return bins; }
    unsigned int& Bins()   { return bins; }
    double BinWidth() const { return Length() / bins; }
};

class HistogramFactory {
private:
    static std::vector<EColor> colors;          // list of colors to use for 1D hitofram lines

    static std::vector<EColor>::const_iterator color;  // loops over the colors to assign different ones to histograms
    static unsigned int histnum;                       // number of unnamed histograms generated

    static std::string name_prefix;                    // prefix for the histogram names

    static EColor GetNextColor();
    static bool loopColors;
    static unsigned int GetNextHistnum();
    static std::string GetNextHistName(const std::string &name="");

    static TDirectory* root_directory;
    static TDirectory* current_directory;

public:

    static void SetName(const std::string& name);

    /**
     * @brief Make a new 1D histogram
     * @param title The title of the histogram
     * @param xlabel X axis label (required, cause... always label your axes ;-) )
     * @param ylabel Y axis label (required)
     * @param bins Binning settings, (Number, min, max)
     * @param name optional name. If "", a number will be inserted
     * @return A pointer to the new histogram
     */
    static TH1D* Make1D (const std::string& title, const std::string& xlabel, const std::string& ylabel, const BinSettings& bins=BinSettings(100,0,100), const std::string& name="");

    /**
     * @brief Make a new 2D histogram
     * @param title The title of the histogram
     * @param xlabel X axis label (required, cause... always label your axes ;-) )
     * @param ylabel Y axis label (required)
     * @param bins X axis binning settings, (Number, min, max)
     * @param bins Y axis binning settings, (Number, min, max)
     * @param name optional name. If "", a number will be inserted
     * @return A pointer to the new histogram
     */
    static TH2D* Make2D (const std::string& title, const std::string& xlabel, const std::string& ylabel, const BinSettings& xbins=BinSettings(100,0,100), const BinSettings& ybins=BinSettings(100,0,100), const std::string& name="");

    static void ApplySettings(TH1D* hist, const std::string& title="", const std::string& xlabel="", const std::string& ylabel="");
    static void ApplySettings(TH2D* hist, const std::string& title="", const std::string& xlabel="", const std::string& ylabel="");

    static void SetLoopColors( bool onoff ) { loopColors=onoff; }
    static bool GetLoopColors() { return loopColors; }

    static void ResetColors();
    static void SetOutputRoot(TDirectory* dir);

};

}

#endif
