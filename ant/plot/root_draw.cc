#include "plot/root_draw.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"
#include "TNamed.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "THStack.h"

using namespace ant;
using namespace std;

unsigned int ant::canvas::num = 0;

const ant::endcanvas endc;

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

canvas &canvas::operator<<(const endcanvas&)
{
    if(!objs.empty()) {

        TCanvas* c = find();

        if(c) {

            const int cols = ceil(sqrt(objs.size()));
            const int rows = ceil((double)objs.size()/(double)cols);

            c->Divide(cols,rows);
            int pad=1;
            for( auto& o : objs) {
                TVirtualPad* vpad = c->cd(pad++);

                o.first->Draw(o.second.c_str());

                if( dynamic_cast<THStack*>(o.first) ) {
                    vpad->BuildLegend();
                }
            }

        }
    }
    return *this;
}

canvas &canvas::operator<<(const drawoption &c)
{
    current_option = c.Option();
    return *this;
}

canvas &canvas::operator>>(const string &filename)
{
    TCanvas* c = find();
    if(c) {
        c->SaveAs(filename.c_str());
    }
    return *this;
}


//************ hstack ************


hstack::hstack(const string &name, const std::string &title):
    stack(new THStack(name.c_str(),title.c_str())),
    current_option("")
{}

hstack::~hstack()
{}

hstack &hstack::operator<<(TH1D *hist)
{
    stack->Add(hist, current_option.c_str());
    return *this;
}

hstack &hstack::operator<<(const drawoption &c)
{
    current_option = c.Option();
    return *this;
}

TObject *hstack::GetObject()
{
    return stack;
}

void hstack::Draw(const string &option) const
{
    stack->Draw(option.c_str());
}

