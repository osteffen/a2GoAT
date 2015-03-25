#ifndef TRUEMATCHER_H
#define TRUEMATCHER_H

#include <list>
#include <vector>
#include <algorithm>

#include "base/interval.h"
#include "TLorentzVector.h"
#include <limits>



namespace ant {
namespace utils {

/**
 * @brief Matcher Function: By Angle
 * @param a first LorentzVector
 * @param b second LoentzVector
 * @return
 */
static double matchAngle( const TLorentzVector* const a, const TLorentzVector* const b ) {
    return a->Vect().Angle(b->Vect());
}

/**
 * @gbrief Matcher Function Template: Match by simple distance |a-b|
 * @param a
 * @param b
 * Use for scalar types
 */
template <typename T>
static double matchDistance( const T& a, const T& b ) {
    return fabs( double(a - b) );
}

template <typename T1, typename T2>
struct scored_match {
    double score;
    T1 a;
    T2 b;

    bool operator< (const scored_match<T1,T2>& rhs ) const { return score < rhs.score; }

    std::pair<T1,T2> getPair() const { return std::pair<T1,T2>(a,b); }
};

/**
 * @brief Particle Matcher
 * @param list1 vector of elements 1
 * @param list2 vector of elements 2
 *
 * Matches the elements in list1 and list2 by a score calculated by a MatchFunction.
 * returns a list of best-matching pairs (minimal score),
 * every element of list1,list2 occurs only once in the pair list
 *
 * The Matcher Function can be a lambda function
 *
 * @todo add a cutoff/max score for pairs
 */
template <class MatchFunction, typename T1, typename T2=T1>
std::list<scored_match<T1,T2>> match1to1( const std::vector<T1>& list1, const std::vector<T2>& list2, MatchFunction f, const ant::IntervalD& score_window=ant::IntervalD(-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()) ) {

    typedef std::list<scored_match<T1,T2>> scorelist;


    scorelist scores;

    for( const auto& i : list1 )
        for( const auto& j : list2 ) {
            const double score = f(i,j);
            if( score_window.Contains(score)) {
                scored_match<T1,T2> s = {score,i,j};
                scores.emplace_back(s);
            }
        }

    scores.sort();

    scorelist matches;

    while( !scores.empty() ) {

        auto a = scores.begin();

        matches.emplace_back( *a );

        const scored_match<T1,T2>& tofind = *a;

        //remove all matches that include one of the two just matched particles
        while( a!=scores.end() ) {

            if( a->a == tofind.a || a->b == tofind.b)
                a = scores.erase(a);
            else
                ++a;
        }
    }

    return std::move(matches);
}
}
}

#endif
