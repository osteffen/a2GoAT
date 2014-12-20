#include "fPhysics.h"
#include "HistFiller.h"


class RootFillNode;

class fPlutoGeant: public fPhysicsCode {
protected:
    RootFillNode* root_node;

public:
    fPlutoGeant();
    virtual ~fPlutoGeant();

    virtual void Run( const fPhysics& p);
    virtual void Display();
};
