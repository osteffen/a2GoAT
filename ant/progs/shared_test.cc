#include <iostream>

#include <memory>
#include <list>
using namespace std;

struct myclass {
    myclass(int x): a(x) { cout << __PRETTY_FUNCTION__ << endl; }
    ~myclass() { cout << __PRETTY_FUNCTION__ << endl; }
    int a;

};

void readit( const shared_ptr<myclass> x) {
    cout << "x=" <<x->a << endl;
}

void readall( const std::list< shared_ptr<myclass> > li ) {
    for(auto& a : li)
        readit(a);

    const shared_ptr<myclass> q = li.front();
    readit(q);
}

int main() {

    shared_ptr<myclass> a = make_shared<myclass>( 5 );

    cout << a->a << endl;

    auto b=a;

    b->a=4;
    cout << b->a << endl;
    cout << a->a << endl;

    readit(b);

    //std::shared_ptr<const myclass> c = make_shared<const myclass>(2);
    //readit(c); //ERROR

    std::list< shared_ptr<myclass> > l;
    l.push_back(b);

    readall(l);

    return 0;
}
