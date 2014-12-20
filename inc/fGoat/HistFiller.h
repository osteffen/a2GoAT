#ifndef HISTFILLER_H
#define HISTFILLER_H

#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

#include <map>
#include <list>
#include <stdexcept>


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
    virtual void Draw() =0;
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
class TH1Filler: public FillNode<typename Formula::DataType>, public TH1FillerBase
{
public:
    typedef typename Formula::DataType DataType;

    TH1Filler( TH1D* hist ): TH1FillerBase(hist) {}

    virtual void Fill( DataType data ) {
        Formula f;
        h->Fill(f(data));
    }

    virtual void Draw() {
        new TCanvas;
        h->Draw();
    }
};

template <typename Formula>
class TH2Filler: public FillNode<typename Formula::DataType>, public TH2FillerBase
{
public:
    typedef typename Formula::DataType DataType;

    TH2Filler( TH2D* hist ): TH2FillerBase(hist) {}

    virtual void Fill( DataType data ) {
        Formula f;
        const std::pair<double, double> res = f(data);
        h->Fill( res.first, res.second);
    }

    virtual void Draw() {
        new TCanvas;
        h->Draw("colz");
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
        for( typename ListType::iterator node = nodes.begin(); node != nodes.end(); ++node ) {
            (*node)->Fill(data);
        }
    }

    virtual void Draw() {
        TCanvas* c=NULL;

        if(grouped) {
            c = new TCanvas(_name.c_str(), _title.c_str());
            const int cols = ceil(sqrt(nodes.size()));
            const int rows = ceil((double)nodes.size()/(double)cols);
            c->Divide(cols,rows);
        }

        int pad=1;
        for( typename ListType::iterator node = nodes.begin(); node != nodes.end(); ++node ) {

            if( grouped) {
                if ( TH1FillerBase* h1 = dynamic_cast<TH1FillerBase*> (*node) ) {
                    c->cd(pad++);
                    h1->GetHist()->Draw();
                } else
                    if(TH2FillerBase* h2 = dynamic_cast<TH2FillerBase*> (*node) ) {
                        c->cd(pad++);
                        h2->GetHist()->Draw("colz");
                    }

            } else
                (*node)->Draw();
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

public:
    virtual void Fill(DataType data) {
        decsisionFormula f;

        try {
            map.at( f(data) )->Fill(data);
        } catch (const std::out_of_range& ) {}
    }

    virtual void Draw() {
        for( typename MapType::iterator node = map.begin(); node != map.end(); ++node ) {
            node->second->Draw();
        }
    }

    virtual void setBranch(int i, FillNode<DataType>* node) {
        map[i] = node;
    }
};


#endif
