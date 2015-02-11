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
#include <map>

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
    virtual void operator () (const Data& data) {
        hist->Fill( f(data) );
    }
    virtual TObject* GetObject() { return hist; }
    virtual TH1D* GetHistogram() { return hist; }
    virtual void Draw(const std::string& option) const { hist->Draw(option.c_str()); }
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
    virtual TH2D* GetHistogram() { return hist; }
    virtual void Draw(const std::string& option) const { hist->Draw(option.c_str()); }
};

template <typename compateType, typename Data, class Function>
class BranchNode;

template <typename Data>
class PlotList: public DataInput<Data> {
public:
    typedef std::shared_ptr<DataInput<Data>> NodePtr;
    typedef std::list<NodePtr> NodeList;

protected:
    NodeList plots;
public:
    typedef std::shared_ptr< PlotList<Data> > sharedptr;

    PlotList() {}


    template<class Function>
    std::shared_ptr< Hist1D<Data,Function> > AddHist1D(Function func, TH1D* hist) {
        plots.emplace_back( new Hist1D<Data,Function>(func, hist) );
        return std::dynamic_pointer_cast< Hist1D<Data,Function> >(plots.back());
    }

    template<class Function>
    std::shared_ptr< Hist2D<Data,Function> > AddHist2D(Function func, TH2D* hist) {
        plots.emplace_back( new Hist2D<Data,Function>(func, hist) );
        return std::dynamic_pointer_cast< Hist2D<Data,Function> >(plots.back());
    }

    sharedptr AddList() {
        sharedptr newlist = std::make_shared<PlotList<Data>>();
        plots.emplace_back(newlist);
        return std::move(newlist);
    }

    void AddNode( NodePtr node ) { plots.push_back(node); }

    template <typename ct, class Function>
    std::shared_ptr< BranchNode<ct, Data, Function>> AddBranchNode(Function f) {
        std::shared_ptr< BranchNode<ct, Data, Function> > b = std::make_shared<BranchNode<ct, Data, Function>>(f);
        plots.emplace_back(b);
        return std::move(b);
    }

    void operator() (const Data& data) {
        for( auto& plot: plots) {
            (*plot)(data);
        }
    }

    void Fill(const Data& data) {
        operator ()(data);
    }

    const NodeList& Nodes() const { return plots; }

};

template <typename compateType, typename Data, class Function>
class BranchNode: public DataInput<Data> {
protected:
    typedef std::shared_ptr<DataInput<Data>> NodePtr;
    std::map<const compateType, NodePtr> m;
    Function f;
public:
    BranchNode(Function _f): f(_f) {}
    virtual ~BranchNode() {}

    virtual void operator() (const Data& data) {
        const compateType r = f(data);
        try {
            m.at(r)->operator ()(data);
        } catch(...) {};
    }

    std::shared_ptr< PlotList<Data> > AddBranch( const compateType& value ) {
        std::shared_ptr< PlotList<Data> > newlist = std::make_shared<PlotList<Data>>();
        m[value] = newlist;
        return std::move(newlist);
    }
};

}

#endif
