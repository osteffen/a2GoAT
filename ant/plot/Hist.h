
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"

#include "plot/Histogram.h"

#include <memory>
#include <string>

namespace ant {

/**
 *@brief smart histogram abstract base class
 * Used to abstract the different filling function subtypes away.
 */
template <typename T>
class Histogram {
public:
    virtual void Fill(T data) =0;
    virtual void Draw(const std::string& option="") =0;
};

/**
 *@brief 1D smart histogram. Binds a TH1D and a function for filling together
 */
template <typename T, typename FunctionType>
class Hist1: public Histogram<T> {
protected:
    TH1D* hist;
    FunctionType f;

public:
    Hist1(TH1D* histogram, FunctionType _f): hist(histogram), f(_f) {}

    virtual void Fill(T data) {
        hist->Fill(f(data));
    }

    void Draw(const std::string& option="") {
        hist->Draw(option.c_str());
    }
};


/**
 *@wrapper around the shared_ptr of Histograms.
 * shared ptrs are always a little weird to use.
 */
template <typename T>
class HistWrap: public Histogram<T> {
protected:
    using HistSPtr = std::shared_ptr< Histogram<T> >;

public:
    HistSPtr ptr;

    HistWrap(): ptr(nullptr) {}

    // hide!
    HistWrap(HistSPtr h): ptr(move(h)) {}

    template<typename FunctionType>
    static HistWrap<T> makeHist(FunctionType func,
        const std::string& title,
        const std::string& xlabel,
        const std::string& ylabel,
        const HistogramFactory::BinSettings& bins,
        const std::string& name="")
    {
        HistogramFactory h("test");
        TH1D* hist = h.Make1D(
            title,
            xlabel,
            ylabel,
            bins,
            name
            );
        return HistWrap<T>( HistSPtr(new Hist1<T, FunctionType>(hist, func)) );
    }

    void Fill(T data) {
        if(ptr)
            ptr->Fill(data);
    }

    void Draw(const std::string &option) {
        if(ptr)
            ptr->Draw(option);
    }
};



// specific factory function
template<typename T>
HistWrap<T> makeS() {
    return HistWrap<T>::makeHist([] (int a){return 10*a;},"title","x","y",HistogramFactory::BinSettings(10));
}

}
