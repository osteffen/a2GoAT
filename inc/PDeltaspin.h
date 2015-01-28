#ifndef __PDeltaspin_h__
#define __PDeltaspin_h__

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "TCanvas.h"

#include "GTreeManager.h"
#include "PPhysics.h"

class	PDeltaspin  : public PPhysics
{
private:

    // Class that groups histograms together
    class Histogm {

    public:
        string  pref;  // prefix to label whole group of histograms
        map<string, TH1* > h; // container for histograms by name (without prefix)
        map<string, string> h_title; // container for histogram titles by name (without prefix)

        // Add 1D histogram
        void AddHistogram(const string& name,       // short specifier for histogram
                          const string& title,      // descriptive title for histogram
                          const string& x_label,    // x axis label
                          const string& y_label,    // y axis label
                          const int x_bins_n,       // number of bins in x
                          const double x_bins_low,  // lower bound of x axis
                          const double x_bins_up    // upper bound of x axis
                          ) {
            // setup one dimensional histogram TH1D
            h_title[name] = title;
            h[name] = new TH1D("","", x_bins_n, x_bins_low, x_bins_up);
            // set proper name and title
            SetNameTitle(name);
            // modify x axis title
            h[name]->SetXTitle(x_label.c_str());
            h[name]->SetYTitle(y_label.c_str());
        }

        // Add 2D histogram
        void AddHistogram(const string& name,       // short specifier for histogram
                          const string& title,      // descriptive title for histogram
                          const string& x_label,    // x axis label
                          const string& y_label,    // y axis label
                          const int x_bins_n,       // number of bins in x
                          const double x_bins_low,  // lower bound of x axis
                          const double x_bins_up,   // upper bound of y axis
                          const int y_bins_n,       // number of bins in y
                          const double y_bins_low,  // lower bound of y axis
                          const double y_bins_up    // upper bound of y axis
                          ) {
            // setup two dimensional histogram TH2D
            h_title[name] = title;
            h[name] = new TH2D("","",
                               x_bins_n, x_bins_low, x_bins_up,
                               y_bins_n, y_bins_low, y_bins_up
                               );
            // set proper name and title
            SetNameTitle(name);
            // modify x axis title
            h[name]->SetXTitle(x_label.c_str());
            h[name]->SetYTitle(y_label.c_str());
        }


        void SetNameTitle(const string& name) {
            h[name]->SetNameTitle(Form("%s_h_%s", pref.c_str(), name.c_str()),
                                 Form("%s: %s",  pref.c_str(), h_title[name].c_str())
                                 );
        }

        Histogm( const string& prefix ): pref(prefix) { }

        Histogm( const Histogm& h2, const string& prefix) : pref(prefix)
        {
            for(auto i=h2.h.begin(); i!=h2.h.end(); ++i) {
                const string key = i->first;
                h_title[key] = h2.h_title.at(key);
                TH1D* h1 = dynamic_cast<TH1D*>(i->second);
                if(h1 != nullptr) {
                    h[key] = new TH1D(*h1);
                }
                TH2D* h2 = dynamic_cast<TH2D*>(i->second);
                if(h2 != nullptr) {
                    h[key] = new TH2D(*h2);
                }
                SetNameTitle(key);
            }
        }

        void Draw() {
            TCanvas* c = new TCanvas(Form("%s_c",pref.c_str()),pref.c_str());
            const int cols = ceil(sqrt(h.size()));
            const int rows = ceil((double)h.size()/(double)cols);
            c->Divide(cols,rows);
            int pad=1;
            for(auto i=h.begin(); i!=h.end(); ++i) {
                c->cd(pad);
                TH2D* h2 = dynamic_cast<TH2D*>(i->second);
                if(h2 != nullptr) {
                    h2->Draw("colz");
                }
                else {
                    i->second->Draw();
                }
                pad++;
            }
        }

        Histogm& operator*= ( const Double_t factor ) {
            for(auto i=h.begin(); i!=h.end(); ++i) {
                i->second->Scale(factor);
            }
            return *this;
        }

        void AddScaled( const Histogm& h2, const Double_t f=1.0 ) {
            for(auto i=h.begin(); i!=h.end(); ++i) {
                i->second->Add(h2.h.at(i->first),f);
            }
        }

        TH1* operator[] (const string& key) {
            return h[key];
        }
    };
    Histogm prompt;
    Histogm random;
    Histogm diff;


protected:
    virtual Bool_t  Start();
    virtual void    ProcessEvent();
    virtual void	ProcessScalerRead();
    virtual Bool_t    Write();
			
public:
    PDeltaspin();
    virtual ~PDeltaspin();
    virtual Bool_t  Init(const char* configfile);

    void Finish();

};



#endif
