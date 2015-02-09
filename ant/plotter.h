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

namespace ant {

template <class Data>
class DataInput {
public:
    virtual void operator() (const Data& data) =0;
};

template <class Data, class Function>
class DataPrint: public DataInput<Data> {
protected:
    Function f;
public:
    DataPrint( Function _f): f(_f) {}
    virtual void operator () (const Data& data) { std::cout << f(data); }
};

template <class Data, class Function>
class Hist1D: public DataInput<Data> {
protected:
    Function f;
    TH1D* hist;
public:
    Hist1D( Function _f, TH1D* h): f(_f), hist(h) {}
    virtual void operator () (const Data& data) { hist->Fill( f(data) ); }
};

template <class Data, class Function>
class Hist2D: public DataInput<Data> {
protected:
    Function f;
    TH2D* hist;
public:
    Hist2D( Function _f, TH2D* h): f(_f), hist(h) {}
    virtual void operator () (const Data& data) {
        const auto result = f(data);
        hist->Fill( std::get<0>(result), std::get<1>(result) );
    }
};

template <typename Data>
class PlotList: public DataInput<Data> {
protected:
    std::list< std::unique_ptr<DataInput<Data>> > plots;
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

    template<class Function>
    void AddPrint(Function func) {
        plots.emplace_back( new DataPrint<Data,Function>(func));
    }

    void Add( std::unique_ptr<DataInput<Data>> dp) {
        plots.emplace_back(move(dp));
    }

    void operator() (const Data& data) {
        for( auto& plot: plots) {
            (*plot)(data);
        }
    }

    void Fill(const Data& data) {
        operator ()(data);
    }
};

}

#endif
