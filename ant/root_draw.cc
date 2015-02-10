#include "root_draw.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"
#include "TNamed.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace ant;
using namespace std;

unsigned int ant::canvas::num = 0;

TCanvas *ant::canvas::create(const string& title)
{
    stringstream s;
    s << "_canvas_" << setfill('0') << setw(3) << num++;
    name = s.str();

    TCanvas* c = new TCanvas(name.c_str(), title.c_str());
    return c;
}


TCanvas *ant::canvas::find()
{
    TObject* o = gROOT->FindObjectAny(name.c_str());
    TCanvas* c = dynamic_cast<TCanvas*>(o);
    if(c)
        return c;
    else
        return create();
}

canvas::canvas(const string &title)
{
    create(title);
}

canvas::~canvas()
{

}

void canvas::cd()
{
    TCanvas* c = find();
    if(c) {
        c->cd();
    }
}

canvas &canvas::operator<<(root_drawable_traits &drawable)
{
    this->operator <<(drawable.GetObject());
    return *this;
}

canvas &canvas::operator<<(TObject *hist)
{
    objs.emplace_back(hist, current_option);
    return *this;
}

canvas &canvas::operator<<(const _cend&)
{
    if(!objs.empty()) {

        TCanvas* c = find();

        if(c) {

            const int cols = ceil(sqrt(objs.size()));
            const int rows = ceil((double)objs.size()/(double)cols);

            c->Divide(cols,rows);
            int pad=1;
            for( auto& o : objs) {
                c->cd(pad++);
                o.first->Draw(o.second.c_str());
            }

        }
    }
    return *this;
}

canvas &canvas::operator<<(const canvas::drawoption &c)
{
    current_option = c.Option();
    return *this;
}

const ant::canvas::_cend ant::canvas::cend;


canvas::drawoption::drawoption(const string &opt):
    option(opt)
{}
