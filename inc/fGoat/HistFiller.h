#ifndef HISTFILLER_H
#define HISTFILLER_H

#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "HistogramFactory.h"

#include <map>
#include <list>
#include <stdexcept>
#include <set>


template <typename f1, typename f2>
struct f2D2 {
    typedef std::pair< typename f1::DataType, typename f2::DataType> DataType;

    std::pair<double, double> operator() ( DataType data ) {
        f1 f1_;
        f2 f2_;
        return std::pair<double, double>(f1_(data.first),f2_(data.second));
    }
};

template <typename f1, typename f2>
struct f2D1 {
    typedef typename f2::DataType DataType;

    std::pair<double, double> operator() ( DataType data ) {
        f1 f1_;
        f2 f2_;
        return std::pair<double, double>(f1_(data),f2_(data));
    }
};

template <class DataType>
class FillNode
{
public:
    virtual ~FillNode() {}
    virtual void Fill(DataType data) =0;
    virtual void Draw( TVirtualPad* pad=nullptr ) =0;
};

template <class DataType>
class FillLeaf: public FillNode<DataType>
{
public:
    virtual ~FillLeaf() {}
};

class TH1FillerBase {
protected:
    TH1D* h;

public:
    TH1FillerBase( TH1D* hist ): h(hist) {}
    virtual ~TH1FillerBase() {}
    TH1D* GetHist() { return h; }\
    void SetHist( TH1D* hist ) { h = hist; }
};

class TH2FillerBase {
protected:
    TH2D* h;

public:
    TH2FillerBase( TH2D* hist ): h(hist) {}


    virtual ~TH2FillerBase() {}
    TH2D* GetHist() { return h; }
    void SetHist( TH2D* hist ) { h = hist; }
};


template <typename Formula>
class TH1Filler: public FillLeaf<typename Formula::DataType>, public TH1FillerBase
{
public:
    typedef typename Formula::DataType DataType;

    TH1Filler( TH1D* hist ): TH1FillerBase(hist) {}
    TH1Filler(const std::string& title, const std::string& xlabel, const std::string& ylabel, const UInt_t nxbins, const Double_t xmin=-1, const Double_t xmax=-1):
        TH1FillerBase( HistogramFactory::GetInstance()->Make1DN(title, xlabel,ylabel,nxbins,xmin, xmax)) {}


    virtual void Fill( DataType data ) {
        Formula f;
        h->Fill(f(data));
    }

    virtual void Draw(TVirtualPad* pad=nullptr) {
        if(!pad) {
            pad = new TCanvas();
        }
        pad->cd();
        h->Draw();
    }
};

template <typename Formula>
class TH2Filler: public FillLeaf<typename Formula::DataType>, public TH2FillerBase
{
protected:
    std::string draw_option="colz";

public:
    typedef typename Formula::DataType DataType;

    TH2Filler( TH2D* hist ): TH2FillerBase(hist) {}
    TH2Filler( const std::string& title, const std::string& xlabel, const std::string& ylabel, const UInt_t nxbins, const Double_t xmin, const Double_t xmax, const UInt_t nybins, const Double_t ymin, const Double_t ymax):
        TH2FillerBase( HistogramFactory::GetInstance()->Make2DN(title,xlabel,ylabel,nxbins,xmin,xmax,nybins,ymin,ymax)) {}

    virtual void Fill( DataType data ) {
        Formula f;
        const std::pair<double, double> res = f(data);
        h->Fill( res.first, res.second);
    }

    virtual void Draw(TVirtualPad* pad=nullptr) {
        if(!pad) {
            pad = new TCanvas();
        }
        pad->cd();
        h->Draw(draw_option.c_str());
    }
};

template <class DataType>
class FillList: public FillNode<DataType>
{
public:
    typedef std::list<FillNode<DataType>*> ListType;

protected:
    ListType nodes;
    bool grouped;
    std::string _name;
    std::string _title;

public:

    FillList( const std::string& name="", const std::string& title="group"):
        grouped(true),
        _name(name),
        _title(title)
    {}

    virtual ~FillList() {}

    void SetGrouped( bool group=true ) { grouped = group; }
    bool isGrouped() const { return grouped; }

    void add( FillNode<DataType>* node) {
        nodes.push_back(node);
    }

    virtual void Fill(DataType data) {
        for( auto &node : nodes ) {
            node->Fill(data);
        }
    }

    virtual void Draw(TVirtualPad* c=nullptr) {

        if(grouped) {
            if(!c)
                c = new TCanvas(_name.c_str(), _title.c_str());
            const int cols = ceil(sqrt(nodes.size()));
            const int rows = ceil((double)nodes.size()/(double)cols);
            c->Divide(cols,rows);
        }

        int padnum=1;
        for( auto &node : nodes ) {
            if(grouped) {
                if ( FillLeaf<DataType>* leaf = dynamic_cast<FillLeaf<DataType>*> (node) ) {
                    TVirtualPad* pad = c->cd(padnum++);
                    leaf->Draw(pad);
                } else {
                    node->Draw();
                }
            } else
                node->Draw();
        }
    }
};



template <typename decsisionFormula>
class FillDecision: public FillNode< typename decsisionFormula::DataType>
{
public:
    typedef typename decsisionFormula::DataType DataType;
    typedef std::map<int, FillNode<DataType>*> MapType;

protected:
    MapType map;
    bool    grouped;

public:
    FillDecision():
        grouped(false) {}
    virtual ~FillDecision() {}

    virtual void Fill(DataType data) {
        decsisionFormula f;

        try {
            map.at( f(data) )->Fill(data);
        } catch (const std::out_of_range& ) {}
    }

    virtual void Draw(TVirtualPad* c=nullptr) {

        std::set< FillLeaf<DataType>* > inCanvas;
        std::set< FillNode<DataType>* > newCanvas;

        for( auto &node : map) {
            FillLeaf<DataType>* leaf = nullptr;

            if ( grouped && (leaf = dynamic_cast<FillLeaf<DataType>*> (node.second)) ) {
                inCanvas.insert( leaf );
            } else {
                newCanvas.insert( node.second );
            }
        }

        if(grouped) {
            if(!c)
                c = new TCanvas();
            const int cols = ceil(sqrt(inCanvas.size()));
            const int rows = ceil((double)inCanvas.size()/(double)cols);
            c->Divide(cols,rows);

            int padnum=1;
            for( auto &leaf : inCanvas ) {
                TVirtualPad* pad = c->cd(padnum++);
                leaf->Draw(pad);
            }
        }

        for( auto &node : newCanvas ) {
            node->Draw();
        }


    }

    virtual void setBranch(int i, FillNode<DataType>* node) {
        map[i] = node;
    }

    virtual void SetGrouped( const bool g = true ) { grouped = g; }
};


#endif
