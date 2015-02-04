#ifndef COMBINATORICS_H
#define COMBINATORICS_H

#include <vector>

namespace ant {

template <class T>
class KofNvector {

protected:
    const std::vector<T>& data;

    typedef std::size_t index_type;
    typedef std::vector<index_type> index_list;
    index_list indices;


    bool nextlevel( index_type i ) {

        if(indices.at(i) >= data.size() - (indices.size() - i)) {
            if( i!=0 && nextlevel(i-1) ) {
                indices.at(i) = indices.at(i-1) +1;
                return true;
            } else
                return false;
        } else {
            indices.at(i)++;
            return true;
        }
    }

public:
    typedef T data_type;

    KofNvector( const std::vector<T>& d, index_type k): data(d) {

        if(k>data.size())
            k=0;

        indices.resize(k);

        for(index_type i=0;i<k; ++i) {
            indices.at(i) = i;
        }
    }

    const T& at( const size_t i ) const { return data.at(indices.at(i)); }

    bool next() {
        if(k()==0)
            return false;
        return nextlevel(indices.size()-1);
    }


    class const_iterator : public std::iterator<std::input_iterator_tag, T>
    {
        index_list::const_iterator index;
        const KofNvector<T>& v;
    public:

        const_iterator( const KofNvector<T>& c, const index_list::const_iterator& i) : index(i), v(c) {}

        const_iterator(const const_iterator& mit) : index(mit.index), v(mit.v) {}

        const_iterator& operator++() {++index;return *this;}

        bool operator==(const const_iterator& rhs) {return index==rhs.index;}

        bool operator!=(const const_iterator& rhs) {return index!=rhs.index;}

        const T& operator*() const { return v.data.at(*index); }

        typedef T data_type;
    };

    const_iterator begin() const { return const_iterator(*this, indices.begin()); }
    const_iterator end() const { return const_iterator(*this, indices.end()); }

    KofNvector& operator++() { next(); }

    const std::vector<T>& Indices() const { return indices; }

    std::size_t k() const { return indices.size(); }
    std::size_t n() const { return data.size();}
    std::size_t size() const { }

};

}

#endif
