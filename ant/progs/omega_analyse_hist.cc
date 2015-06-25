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
    stream << "(" << value.value << "+/-" << value.error << ")";
    return stream;
}

using value = ValueError<double>;

value integrate(TH1* hist, const ant::IntervalD& interval) {
    const auto startbin = hist->FindBin(interval.Start());
    const auto stopbin = hist->FindBin(interval.Stop());
    //const auto startbin_center = hist->GetBinCenter(startbin);
    //const auto stopbin_center = hist->GetBinCenter(stopbin);
    //const auto binwidth = hist->GetBinWidth();

    value v(0,0);
    v.value = hist->IntegralAndError(startbin, stopbin, v.error);
    // i += hist->GetBinContent(startbin) * (1-(interval.Start()-startbin_center-binwidth/2.0)/binwidth);

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

value SignalOverBG(TH1* hist, TF1* bg, const ant::IntervalD& range, TColor color=kBlue) {

    auto f = new TF1(*bg);
    f->SetRange(range.Start(), range.Stop());
    f->SetLineColor(color);

    const auto bg_estimage = bg->Integral(range.Start(), range.Stop());
    const auto integral = integrate(hist,range);

    value result;
    result.value = integral.value - bg_estimage;

    f->Draw();

    return result;
}

void analyse(TH1* gg) {

    TF1* bg = new TF1("bg","pol1", bg_range.Start(), bg_range.Stop());

    gg->Fit(bg,"R","",bg_range.Start(),bg_range.Stop());

    double rmin,rmax;
    bg->GetRange(rmin,rmax);
    cout << rmin << " " << rmax << endl;


    const double pi0_bg = bg->Integral(pi0_range.Start(), pi0_range.Stop());
    const double eta_bg = bg->Integral(eta_range.Start(), eta_range.Stop());

    cout << "bg in pi0 range = " << pi0_bg << endl;
    cout << "bg in eta range = " << eta_bg << endl;

    TF1* pi0_bgf = new TF1(*bg);
    pi0_bgf->SetRange(pi0_range.Start(), pi0_range.Stop());

    TF1* eta_bgf = new TF1(*bg);
    eta_bgf->SetRange(eta_range.Start(), eta_range.Stop());

    pi0_bgf->SetLineColor(kGreen);
    eta_bgf->SetLineColor(kBlue);

    pi0_bgf->Draw("same");
    eta_bgf->Draw("same");

    auto pi0peak = integrate(gg,pi0_range);
    auto etapeak = integrate(gg,eta_range);

    cout << "pi0 integral = " << pi0peak << endl;
    cout << "eta integral = " << etapeak << endl;

    auto pi0s = pi0peak.value - pi0_bg;
    auto etas = etapeak.value - eta_bg;

    cout << "pi0 singal = " << pi0s << endl;
    cout << "eta singal = " << etas << endl;

    new TCanvas;
    auto residuals = ResidaulPlot(gg,bg);
    residuals->Draw("AP");

}

int main(int argc, char** argv) {

    TRint Rint("omega analyse hists", &argc, argv);

    TFile* f = new TFile("0_o.root","READ");

    if(f && f->IsOpen()) {

        TH1* gg = nullptr;

        f->GetObject("Omega2 Rec/gg_IM", gg);

        if(gg) {
            gg->Draw();
            analyse(gg);

        }
    }

    Rint.Run();

    return 0;
}
