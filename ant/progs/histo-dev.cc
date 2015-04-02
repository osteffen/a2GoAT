#include <iostream>
#include "TH1D.h"
#include "Particle.h"
#include <memory>
#include <list>
#include <algorithm>
#include "plot/Histogram.h"
#include "plot/SmartHist.h"
#include "plot/HistogramFactories.h"
#include "TH1D.h"

#include "TRint.h"

using namespace std;
using namespace ant;


int main(int argc, char** argv) {
    TRint app("A", &argc, argv);

    SmartHist<int> a = SmartHist<int>::makeHist([] (int a) { return a * 1;},"t","a","b",BinSettings(10),"t_a");
    a.Fill(5);

    SmartHist<int> b(SmartHist<int>::makeHist([] (int a) { return a * 1;},"t","a","b",BinSettings(10),"t_b"));
    b.Fill(2);

    SmartHist<int> c;
    c.Copy(b, "c_of_b");
    c.Fill(3);

    // ERROR!
    // HistWrap<int> d = c;

    SmartHist<int> d(c,"d");

    SmartHist<double> f = SmartHist<double>::makeHist("Direct double","x","y",BinSettings(10));

    std::list<SmartHist<double>> liste;
    liste.emplace_back(SmartHist<double>::makeHist("Direct double list1","x","y",BinSettings(10)));
    liste.push_back(   SmartHist<double>::makeHist("Direct double list2","x","y",BinSettings(10)));

    for( auto& h : liste ) {
        h.Fill(1);
    }

    SmartHistFactory sf("factory");

    auto x = sf.makeHist<int>([] (int a) { return 2*a; },"form factory","","",BinSettings(20),"from factory");

    SmartHistFactory sf2("another_factory");
    auto x2 = sf2.makeHist<int>([] (int a) { return 3*a; },"form factory","","",BinSettings(20),"from another factory");

    TH1D* roothist = HistogramFactory::Default().Make1D("root","x","y");

    SmartHistFactory sf3("sub_factory", sf2);
    auto x4 = sf3.makeHist<int>([] (int a) { return 3*a; },"form sub factory","","",BinSettings(20),"from another f");

    auto x5 = sf2.makeHist<int>([] (int a) { return 3*a; },"form factory again","","",BinSettings(20),"from another factory333");

    app.Run();
}
