#include <iostream>
#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TRint.h"
#include "TROOT.h"
#include "base/interval.h"
#include "TGraph.h"
#include "TCanvas.h"
#include <ostream>

using namespace std;

ant::IntervalD pi0_range(100,180);
ant::IntervalD eta_range(500,600);
ant::IntervalD bg_range(pi0_range.Stop(), eta_range.Start());

template <typename T>
struct ValueError {
    T value;
    T error;
    ValueError(const T& v=0, const T& e=0) : value(v), error(e) {}
    double RelativeError() const { return double(error)/double(value); }

};

template <typename T>
std::ostream& operator<< (std::ostream& stream, const ValueError<T> value) {
    stream << "(" << value.value << " \u00B1 " << value.error << ")";
    return stream;
}

template <typename T>
T sqr(const T& x) { return x*x; }

using value = ValueError<double>;

value integrate(TH1* hist, const ant::IntervalD& interval) {
    const auto startbin = hist->FindBin(interval.Start());
    const auto stopbin = hist->FindBin(interval.Stop());
    //const auto startbin_center = hist->GetBinCenter(startbin);
    //const auto stopbin_center = hist->GetBinCenter(stopbin);
    //const auto binwidth = hist->GetBinWidth();

    value v;
    v.value = hist->IntegralAndError(startbin, stopbin, v.error);
    // i += hist->GetBinContent(startbin) * (1-(interval.Start()-startbin_center-binwidth/2.0)/binwidth);

    return v;

}

value integrate(TF1* fct, const ant::IntervalD& interval) {
    value v;
    v.value = fct->Integral(interval.Start(), interval.Stop());
    v.error = fct->IntegralError(interval.Start(), interval.Stop());
    return v;
}

TGraph* ResidaulPlot(TH1* hist, TF1* fct) {
    double min,max;
    fct->GetRange(min,max);
    const auto startbin = hist->FindBin(min);
    const auto stopbin  = hist->FindBin(max);
    const auto nbins = stopbin - startbin +1;

    TGraph* g= new TGraph(nbins);

    for(auto bin=startbin; bin<=stopbin; ++bin) {
        const auto x = hist->GetBinCenter(bin);
        const auto y = hist->GetBinContent(bin);
        const auto diff = y - fct->Eval(x);
        g->SetPoint(bin-startbin, x, double(diff)/double(y));
    }
    return g;
}

value SignalOverBG(TH1* hist, TF1* bg, const ant::IntervalD& range, Color_t color=kBlue) {

    auto f = new TF1(*bg);
    f->SetRange(range.Start(), range.Stop());
    f->SetLineColor(color);

    value bg_estimage = integrate(f, range);
    value integral = integrate(hist,range);

    cout << "BG Estibate = " << bg_estimage << endl;
    cout << "Integral    = " << integral << endl;

    value result;
    result.value = integral.value - bg_estimage.value;
    result.error = sqrt(sqr(integral.error) + sqr(bg_estimage.error));

    cout << "Signal      = " << result << endl;

    f->Draw("same");

    return result;
}

void analyse(TH1* gg) {

    TF1* bg = new TF1("bg","pol1", bg_range.Start(), bg_range.Stop());

    gg->Fit(bg,"R","",bg_range.Start(),bg_range.Stop());

    double rmin,rmax;
    bg->GetRange(rmin,rmax);
    cout << rmin << " " << rmax << endl;

    cout << "\npi0:" << endl;
    auto pi0 = SignalOverBG(gg,bg,pi0_range,kRed);

    cout << "\neta:" << endl;
    auto eta = SignalOverBG(gg,bg,eta_range, kBlue);

    cout << "Ratio = " << eta.value / pi0.value << endl;

    new TCanvas;
    auto residuals = ResidaulPlot(gg,bg);
    residuals->Draw("AP");

}

int main(int argc, char** argv) {

    TRint Rint("omega analyse hists", &argc, argv);

    TFile* f = new TFile("0_o.root","READ");

    if(f && f->IsOpen()) {

        TH1* gg = nullptr;

        f->GetObject("Omega3Rec/gg_IM", gg);

        if(gg) {
            gg->Draw();
            analyse(gg);

        }
    }

    Rint.Run();

    return 0;
}
