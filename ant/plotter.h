#ifndef PLOTTER_H
#define PLOTTER_H

#include <iostream>
#include <functional>
#include <memory>
#include <list>

namespace ant {

template <class Data>
class DataProcessor {
public:
    virtual void operator() (const Data& data) { std::cout << data << std::endl; }
};

template <class Data, class Function>
class AH: public DataProcessor<Data> {
protected:
    Function f;
public:
    AH( Function _f): f(_f) {}
    virtual void operator () (const Data& data) { DataProcessor<Data>::operator ()(f(data)); }
};

template <typename Data, class Function>
DataProcessor<Data>* AHMake( Function f ) {
    return new AH<Data,Function>(f);
}


template <typename Data>
class PlotList: public DataProcessor<Data> {
protected:
    std::list< std::unique_ptr<DataProcessor<Data>> > plots;
public:
    PlotList() {}

    template<class Function>
    void AddPlot(Function func) {
        plots.emplace_back( new AH<Data,Function>(func) );
    }

    void Add( std::unique_ptr<DataProcessor<Data>> dp) {
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
