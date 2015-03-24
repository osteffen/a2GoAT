#ifndef TRUEMATCHER_H
#define TRUEMATCHER_H

#include <list>
#include <vector>
#include <algorithm>

#include "TLorentzVector.h"

namespace ant {

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
std::list<std::pair<T1,T2>> Match( const std::vector<T1>& list1, const std::vector<T2>& list2, MatchFunction f) {

    typedef std::pair<T1,T2> match;
    typedef std::list<match> matchlist;

    typedef std::pair<match, double> scored_match;
    typedef std::list<scored_match> scorelist;


    scorelist scores;

    for( auto& i : list1 )
        for( auto& j : list2 ) {
            const double score = f(i,j);
            scores.emplace_back( scored_match(match(i, j), score ));
        }

    scores.sort( [] (const scored_match& a, const scored_match& b) { return a.second < b.second; } );

    matchlist matches;

    while( !scores.empty() ) {

        auto a = scores.begin();

        match& ma = a->first;
        matches.emplace_back( ma );

        //remove all matches that include one of the two just matched particles
        while( a!=scores.end() ) {

            if(a->first.first == ma.first || a->first.second == ma.second)
                a = scores.erase(a);
            else
                ++a;
        }
    }

    return std::move(matches);
}

}
#endif
