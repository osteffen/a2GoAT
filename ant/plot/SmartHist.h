#ifndef HIST_H
#define HIST_H

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"

#include "plot/Histogram.h"
#include "root_draw.h"

#include <memory>
#include <string>

#include <iostream>
using namespace std;
namespace ant {

/**
 *@brief smart histogram abstract base class
 * Used to abstract the different filling function subtypes away.
 */
template <typename T>
class Histogram: public ant::root_drawable_traits {
public:
    virtual ~Histogram() {}
    virtual Histogram<T>* MakeCopy(const std::string& newname) const =0;
    virtual void Fill(T data, double weight=1.0) =0;
    virtual void Draw(const std::string& option="") const =0;

};

/**
 *@brief 1D smart histogram. Binds a TH1D and a function for filling together
 */
template <typename T, typename FunctionType>
class Hist1: public Histogram<T> {
private:
    mutable TH1D* hist;
    FunctionType f;

public:
    Hist1(TH1D* histogram, FunctionType _f): hist(histogram), f(_f) {}

    virtual ~Hist1() {}

    virtual Histogram<T>* MakeCopy(const std::string& newname) const {
        TH1D* root_hist = new TH1D(*hist);
        root_hist->SetName(newname.c_str()); //TODO: let histogram factory handle this
        return new Hist1<T, FunctionType>(root_hist, f);
    }

    virtual void Fill(T data, double weight=1.0) {
        hist->Fill(f(data), weight);
    }

    void Draw(const std::string& option="") const {
        hist->Draw(option.c_str());
    }

};

/**
 *@wrapper around the shared_ptr of Histograms.
 * shared ptrs are always a little weird to use.
 */
template <typename T>
class SmartHist: public ant::root_drawable_traits {
protected:
    using hist_ptr = std::shared_ptr< Histogram<T> >;
    SmartHist(hist_ptr hp): ptr(move(hp)) {}

public:
    hist_ptr ptr;

    SmartHist(): ptr(nullptr) {}

    //HistWrap(HistWrap&& hist): ptr(move(hist.ptr)) {}

    SmartHist(const SmartHist& other, const std::string& newname) { this->Copy(other, newname); }

    template<typename FunctionType>
    static SmartHist<T> makeHist(FunctionType func,
        const std::string& title,
        const std::string& xlabel,
        const std::string& ylabel,
        const BinSettings& bins,
        const std::string& name="",
        HistogramFactory& factory=ant::HistogramFactory::Default())
    {
        TH1D* hist = factory.Make1D(
            title,
            xlabel,
            ylabel,
            bins,
            name
            );
        return move(SmartHist<T>( move(hist_ptr(new Hist1<T, FunctionType>(hist, func))) ));
    }

    static SmartHist<T> makeHist(
        const std::string& title,
        const std::string& xlabel,
        const std::string& ylabel,
        const BinSettings& bins,
        const std::string& name="",
        HistogramFactory& factory=ant::HistogramFactory::Default())
    {
        return move(makeHist([] (T data) { return data;},title, xlabel, ylabel, bins, name, factory));
    }

    void Fill(T data, double weight=1.0) {
        if(ptr)
            ptr->Fill(data, weight);
    }

    void Draw(const std::string &option) const {
        if(ptr)
            ptr->Draw(option);
    }

    void Copy( const SmartHist& rhs, const std::string& newname) {
        ptr = move(hist_ptr(rhs.ptr->MakeCopy(newname)));
    }

};

// specialization for strings
template<>
SmartHist<const std::string&> SmartHist<const std::string&>::makeHist(
    const std::string& title,
    const std::string& xlabel,
    const std::string& ylabel,
    const BinSettings& bins,
    const std::string& name,
    HistogramFactory& factory);

}

#endif
