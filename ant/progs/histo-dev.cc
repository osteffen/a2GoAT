#include <iostream>
#include "TH1D.h"
#include "Particle.h"
#include <memory>
#include <list>
#include <algorithm>
#include "plot/Histogram.h"
using namespace std;
using namespace ant;

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
 *@brief A shared pointer to a smart histogram.
 */
template <typename T>
using HistSPtr = shared_ptr<Histogram<T>>;


/**
 *@brief make a shared pointer to a new smart histogram
 */
template<typename T, typename FunctionType>
HistSPtr<T> makeSHist1(
        FunctionType func,
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
    return HistSPtr<T>(new Hist1<T, FunctionType>(hist, func));
}

/**
 *@wrapper around the shared_ptr of Histograms.
 * shared ptrs are always a little weird to use.
 */
template <typename T>
class HistWrap: public Histogram<T> {
private:
    class BufferClass: public Histogram<T> {
    public:
        virtual void Fill(T data) { cerr << __PRETTY_FUNCTION__ <<endl; }
        virtual void Draw(const string &option) { cerr << __PRETTY_FUNCTION__ << endl; }
    };
    static BufferClass defaultptr;
public:
    HistSPtr<T> ptr;

    HistWrap(): ptr(nullptr) {}

    // hide!
    HistWrap(HistSPtr<T> h): ptr(move(h)) {}

    HistWrap(const HistogramFactory::BinSettings& bins):
        ptr(makeSHist1<T>([] (T data) { return data;},"Title: direct","x","y",bins))
    {}

    void Fill(T data) {
        if(ptr)
            ptr->Fill(data);
    }

    void Draw(const string &option) {
        if(ptr)
            ptr->Draw(option);
    }
};


template <typename T, typename FunctionType>
Histogram<T>* makeHist1(FunctionType f) {
    return new Hist1<T, FunctionType>(f);
}

template<typename T>
HistWrap<T> makeS() {
    return HistSPtr<T>( makeSHist1<T>([] (int a){return 10*a;},"title","x","y",HistogramFactory::BinSettings(10)));
}

int main() {
    HistWrap<int> a;

    a = makeS<int>();

    HistWrap<int> b;
    b=a;

    HistWrap<int> c(a);
    c.Fill(22);

    HistWrap<int> direct(HistogramFactory::BinSettings(10));
    direct.Fill(2);
}
