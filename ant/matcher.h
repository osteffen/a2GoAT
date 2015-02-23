#ifndef TRUEMATCHER_H
#define TRUEMATCHER_H

#include <list>
#include <vector>
#include <algorithm>

#include "TLorentzVector.h"

namespace ant {

static double matchAngle( const TLorentzVector* const a, const TLorentzVector* const b ) {
    return a->Vect().Angle(b->Vect());
}

template <typename T>
static double matchDistance( const T& a, const T& b ) {
    return fabs( double(a - b) );
}

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
