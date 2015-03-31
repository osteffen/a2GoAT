#include <iostream>
#include "TH1D.h"
#include "Particle.h"
#include <memory>
#include <list>
#include <algorithm>
#include "plot/Histogram.h"
#include "plot/Hist.h"


#include "TRint.h"

using namespace std;
using namespace ant;


int main(int argc, char** argv) {
    TRint app("A", &argc, argv);

    HistWrap<int> a = HistWrap<int>::makeHist([] (int a) { return a * 1;},"t","a","b",BinSettings(10),"t_a");
    a.Fill(5);

    HistWrap<int> b(HistWrap<int>::makeHist([] (int a) { return a * 1;},"t","a","b",BinSettings(10),"t_b"));
    b.Fill(2);

    HistWrap<int> c;
    c.Copy(b, "c_of_b");
    c.Fill(3);

    // ERROR!
    // HistWrap<int> d = c;

    HistWrap<int> d(c,"d");

    HistWrap<double> f = HistWrap<double>::makeHist("Direct double","x","y",BinSettings(10));

    app.Run();
}
