#ifndef PLOTTER_H
#define PLOTTER_H

#include <iostream>
#include <functional>
#include <memory>
#include <list>
#include <TH1D.h>
#include <TH2D.h>
#include "Histogram.h"
#include <tuple>
#include "root_draw.h"

class TObject;

namespace ant {

template <class Data>
class DataInput {
public:
    virtual ~DataInput() {}
    virtual void operator() (const Data& data) =0;
};

template <class Data>
class DrawableDataInput: public DataInput<Data>, public ant::root_drawable_traits {
public:
    virtual ~DrawableDataInput() {}
};

template <class Data, class Function>
class Hist1D: public DrawableDataInput<Data> {
protected:
    Function f;
    TH1D* hist;
public:
    Hist1D( Function _f, TH1D* h): f(_f), hist(h) {}
    virtual ~Hist1D() {}

    virtual void operator () (const Data& data) { hist->Fill( f(data) ); }
    virtual TObject* GetObject() { return hist; }
    virtual void Draw() { hist->Draw("colz"); }
};

template <class Data, class Function>
class Hist2D: public DrawableDataInput<Data> {
protected:
    Function f;
    TH2D* hist;
public:
    Hist2D( Function _f, TH2D* h): f(_f), hist(h) {}
    virtual ~Hist2D() {}

    virtual void operator () (const Data& data) {
        const auto result = f(data);
        hist->Fill( std::get<0>(result), std::get<1>(result) );
    }

    virtual TObject* GetObject() { return hist; }
    virtual void Draw() { hist->Draw("colz"); }
};

template <typename Data>
class PlotList: public DataInput<Data> {
protected:
    std::list<DrawableDataInput<Data>*> plots;
public:
    PlotList() {}

    template<class Function>
    void AddHist1D(Function func, TH1D* hist) {
        plots.emplace_back( new Hist1D<Data,Function>(func, hist) );
    }

    template<class Function>
    void AddHist2D(Function func, TH2D* hist) {
        plots.emplace_back( new Hist2D<Data,Function>(func, hist) );
    }

    void operator() (const Data& data) {
        for( auto& plot: plots) {
            (*plot)(data);
        }
    }

    void Fill(const Data& data) {
        operator ()(data);
    }

    std::list<DrawableDataInput<Data>*>& Plots() { return plots; }


};

}

#endif
