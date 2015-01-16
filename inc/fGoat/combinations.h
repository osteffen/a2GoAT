#ifndef COMBINATIONS_H
#define COMBINATIONS_H

template <class ContainerClass>
class Combine2 {
    private:

        const ContainerClass& container;
        typename ContainerClass::const_iterator a,b;

    public:
        Combine2( const ContainerClass& container_ ):
            container(container_),
            a(container.begin()),
            b(container.begin())
        {
            if(b!=container.end()) {
                ++b;
            };
        }

        void operator() () {

            if( done() )
                return;

            b++;

            if( b==container.end() ) {
                a++;
                if( a!=container.end()) {
                    b=a;
                    b++;
                }
            }
        }

        const typename ContainerClass::value_type& first() const { return *a; }
        const typename ContainerClass::value_type& second() const { return *b; }

        bool done() { return a==container.end() || b==container.end(); }
};

#endif
