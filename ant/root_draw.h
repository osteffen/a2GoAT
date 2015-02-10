#ifndef ROOT_DRAW_H
#define ROOT_DRAW_H

#include <string>
#include <list>

class TCanvas;
class TObject;

namespace ant {

class root_drawable_traits {
public:
    virtual TObject* GetObject() =0;
    virtual void Draw() =0;
};

class canvas {
protected:
    static unsigned int num;

    std::string name;

    TCanvas* create(const std::string& title="");
    TCanvas* find();

    typedef std::pair<TObject*, std::string> ObjectOption;
    std::list<ObjectOption> objs;

    std::string current_option;
public:

    class modifier {};

    class _cend: public modifier {
    public:
        virtual ~_cend() {}
    };

    class drawoption: public modifier {
    protected:
        std::string option;
    public:
        drawoption(const std::string& opt="");
        virtual ~drawoption() {}
        const std::string& Option() const { return option; }
    };

    static const _cend cend;


    canvas(const std::string& title="");
    virtual ~canvas();

    virtual void cd();

    virtual canvas& operator<< (root_drawable_traits& drawable);

    virtual canvas& operator<< (TObject* hist);

    virtual canvas& operator<< (const _cend& c);

    virtual canvas &operator<< (const drawoption& c);


};


}

#endif
