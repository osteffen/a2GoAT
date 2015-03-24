#ifndef ROOT_DRAW_H
#define ROOT_DRAW_H

#include <string>
#include <list>

class THStack;
class TH1D;
class TCanvas;
class TObject;


namespace ant {

class root_drawable_traits {
public:
    virtual TObject* GetObject() =0;
    virtual void Draw(const std::string& option) const =0;
};

class modifier {};

class endcanvas: public modifier {
};
static const endcanvas endc;

class drawoption: public modifier {
protected:
    std::string option;
public:
    drawoption(const std::string& opt=""): option(opt) {}
    virtual ~drawoption() {}
    const std::string& Option() const { return option; }
};

/**
 * @brief The canvas class
 *
 * Wrapper for TCanvas.
 * Drawable classes, either TObject or classes inhereting from root_drawable_traits,
 * can be added via the << operator. Just like with cout:
 * canvas c("test");
 * c << hist1 << hist2 << endc;
 *
 * The canvas is automatically subdivided to fit all added histograms.
 * To finally draw the canvas add the endc obejct to it.
 *
 * Draw options can be set using the drawoption modifier:
 *
 * c << drawoption("colz") << hist2d_a << hist2d_b << endc;
 */
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

    canvas(const std::string& title="");
    virtual ~canvas();

    virtual void cd();

    virtual canvas& operator<< (root_drawable_traits& drawable);

    virtual canvas& operator<< (TObject* hist);

    virtual canvas& operator<< (const endcanvas& c);

    virtual canvas &operator<< (const drawoption& c);

    virtual canvas &operator>> (const std::string& filename);

};

/**
 * @brief The hstack class
 *
 * Wrapper for ROOT's THStack.
 * Overloads the << operator to add histograms to it, and of course
 * can be drawn the same way with the canvas class.
 * @see ant::canvas
 *
 * When drawn, ant::canvas automatically builds the legend.
 *
 * @BUG No axis lables. Because they have to be set *after* the THStack is drawn
 *  (https://root.cern.ch/root/html/THStack.html#THStack:GetXaxis).
 *  ant::canvas stores a list of TObjects and draws them alter all together, so there
 *  is no way hstack can set the title afterwards. :-(
 */
class hstack: public root_drawable_traits {
protected:
    THStack* stack;
    std::string current_option;
public:
    hstack(const std::string& name, const std::string &title="");
    virtual ~hstack();

    virtual hstack &operator<< (TH1D* hist);
    virtual hstack &operator<< (const drawoption& c);

    TObject *GetObject();
    void Draw(const std::string &option) const;
};


}

#endif
