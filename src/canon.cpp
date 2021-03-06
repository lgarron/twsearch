#include <iostream>
#include <map>
#include <unordered_set>
#include "canon.h"
#include "util.h"
#include "index.h"
#include "city.h"
vector<ull> canonmask ;
vector<vector<int> > canonnext ;
vector<ull> canonseqcnt ;
vector<ull> canontotcnt ;
template<typename T>
struct hashvector {
   size_t operator()(const T&v) const {
      return CityHash64((const char *)&v[0], sizeof(T)*v.size()) ;
   }
} ;
template<typename T>
void freeContainer(T& c) {
   T empty;
   swap(c, empty);
}
void makecanonstates(puzdef &pd) {
   int nbase = pd.basemoves.size() ;
   if (quarter) { // rewrite base
      int at = 1 ;
      for (int i=0; i<(int)pd.moves.size(); i++) {
         moove &mv = pd.moves[i] ;
         if (mv.cost > 1)
            mv.cs = 0 ;
         else
            mv.cs = at++ ;
      }
      nbase = at ;
      cout << "For quarter turn, rewrote bases to " << nbase << endl ;
   } else {
      for (int i=0; i<(int)pd.moves.size(); i++) {
         moove &mv = pd.moves[i] ;
         mv.cs = mv.base ;
      }
   }
   if (nbase > 63)
      error("! too many base moves for canonicalization calculation") ;
   pd.ncs = nbase ;
   vector<ull> commutes(nbase) ;
   stacksetval p1(pd), p2(pd) ;
   for (int i=0; i<nbase; i++)
      commutes[i] = (1LL << nbase) - 1 ;
   for (int i=0; i<(int)pd.moves.size(); i++)
      for (int j=0; j<i; j++) {
         pd.mul(pd.moves[i].pos, pd.moves[j].pos, p1) ;
         pd.mul(pd.moves[j].pos, pd.moves[i].pos, p2) ;
         if (pd.comparepos(p1, p2) != 0) {
            commutes[pd.moves[i].cs] &= ~(1LL << pd.moves[j].cs) ;
            commutes[pd.moves[j].cs] &= ~(1LL << pd.moves[i].cs) ;
         }
      }
   using trip = tuple<ull, int, int> ;
   map<trip, int> statemap ;
   vector<trip > statebits ;
   trip firststate = make_tuple(0LL, -1, 0) ;
   statemap[firststate] = 0 ;
   statebits.push_back(firststate) ;
   int qg = 0 ;
   int statecount = 1 ;
   while (qg < (int)statebits.size()) {
      vector<int> nextstate(nbase) ;
      for (int i=0; i<nbase; i++)
         nextstate[i] = -1 ;
      trip statev = statebits[qg] ;
      ull stateb = get<0>(statev) ;
      int prevm = get<1>(statev) ;
      int prevcnt = get<2>(statev) ;
      canonmask.push_back(quarter ? 1 : 0) ;
      int fromst = qg++ ;
      int ms = 0 ;
      for (int m=0; m<nbase; m++) {
         if ((stateb & commutes[m] & ((1LL << m) - 1)) != 0) {
            canonmask[fromst] |= 1LL << m ;
            continue ;
         }
         if (!quarter && (((stateb >> m) & 1) != 0)) {
            canonmask[fromst] |= 1LL << m ;
            continue ;
         }
         ull nstb = (stateb & commutes[m]) | (1LL << m) ;
         int thism = -1 ;
         int thiscnt = 0 ;
         if (quarter) {
            if (m == 0) {
               canonmask[fromst] |= 1LL << m ;
               continue ;
            }
            while (pd.moves[ms].cs != m)
               ms++ ;
            // don't do opposing moves in a row
            if (prevm >= 0 && ms != prevm &&
                pd.moves[ms].base == pd.moves[prevm].base) {
               canonmask[fromst] |= 1LL << m ;
               continue ;
            }
            if (ms == prevm) {
               if (2*(prevcnt+1)+(pd.moves[ms].twist != 1) >
                   pd.basemoveorders[pd.moves[ms].base]) {
                  canonmask[fromst] |= 1LL << m ;
                  continue ;
               }
            }
            thism = ms ;
            thiscnt = (ms == prevm ? prevcnt + 1 : 1) ;
         }
         trip nsi = make_tuple(nstb, thism, thiscnt) ;
         if (statemap.find(nsi) == statemap.end()) {
            statemap[nsi] = statecount++ ;
            statebits.push_back(nsi) ;
         }
         int nextst = statemap[nsi] ;
         nextstate[m] = nextst ;
      }
      canonnext.push_back(nextstate) ;
   }
   cout << "Found " << statecount << " canonical move states." << endl ;
 /*
   for (int i=0; i<(int)canonnext.size(); i++) {
      cout << i << " " << hex << canonmask[i] << dec ;
      for (int j=0; j<nbase; j++)
         cout << " " << canonnext[i][j] ;
      cout << endl ;
   }
 */
}
map<ull,int> statemap ;
int movebits, ccount ;
vector<loosetype> ccenc ;
unordered_set<vector<loosetype>, hashvector<vector<loosetype>>> ccseen ;
vector<int> ccnextstate ;
int ccstalloc = 0 ;
int ccnbase = 0 ;
int recurcanonstates2(const puzdef &pd, int togo, ull moveset, int sp) {
   if (togo == 0) {
      loosepack(pd, posns[sp], &ccenc[0], 1) ;
      if (ccseen.find(ccenc) == ccseen.end()) {
         ccseen.insert(ccenc) ;
         if (sp > ccount) {
            ull hibit = (1LL << (ccount * movebits)) ;
            moveset = hibit | (moveset & (hibit - 1)) ;
         }
         if (statemap.find(moveset) == statemap.end()) {
// cout << "Allocating new state for " << hex << moveset << dec << endl ;
            statemap[moveset] = ccstalloc++ ;
         }
         return statemap[moveset] ;
      } else {
         return -1 ;
      }
   }
   ull newmask = 1 ;
   ull oldmask = 1 ;
   if (statemap.find(moveset) == statemap.end()) {
 cout << "Moveset is " << hex << moveset << dec << endl ;
      error("! can't find state number for moveset") ;
   }
   int cs = statemap[moveset] ;
   if (togo > 1) {
      if ((int)canonmask.size() <= cs) {
         cout << "Size of canonmask " << canonmask.size() << " cs " << cs << endl ;
         error("! canonmask not large enough") ;
      }
      oldmask = canonmask[cs] ;
   }
   for (int i=0; i<(int)pd.moves.size(); i++) {
      if ((oldmask >> pd.moves[i].cs) & 1)
         continue ;
      pd.mul(posns[sp], pd.moves[i].pos, posns[sp+1]) ;
      int nextst = recurcanonstates2(pd, togo-1,
                                     (moveset << movebits) + pd.moves[i].cs,
                                     sp+1) ;
      if (nextst < 0)
         newmask |= 1LL << pd.moves[i].cs ;
      if (togo == 1)
         ccnextstate[pd.moves[i].cs] = nextst ;
   }
   if (togo == 1) {
/*
 cout << "Adding values to mask and next; newmask " << hex << newmask << dec << endl ;
 for (int i=0; i<(int)ccnextstate.size(); i++)
   cout << " " << ccnextstate[i] ;
 cout << endl ;
 */
      canonmask.push_back(newmask) ;
      canonnext.push_back(ccnextstate) ;
   }
   return 1 ;
}
/**
 *   Another mechanism for canonical states that eliminates sequences
 *   c d if there's an earlier sequence a b that goes to the same
 *   state.  May be effective for the 2x2x2x2, or for puzzles where we
 *   don't reduce by canonical.
 */
void makecanonstates2(puzdef &pd) {
   int at = 1 ;
   for (int i=0; i<(int)pd.moves.size(); i++) {
      moove &mv = pd.moves[i] ;
      if (quarter && mv.cost > 1)
         mv.cs = 0 ;
      else
         mv.cs = at++ ;
   }
   ccnbase = at ;
   statemap[1] = 0 ;
   ccstalloc = 1 ;
   if (ccnbase > 63)
      error("! too many base moves for canonicalization calculation") ;
   movebits = ceillog2(ccnbase) ;
   pd.ncs = ccnbase ;
   ccenc = vector<loosetype>(looseiper) ;
   while (posns.size() <= 100) {
      posns.push_back(allocsetval(pd, pd.id)) ;
      movehist.push_back(-1) ;
   }
   pd.assignpos(posns[0], pd.id) ;
   ccnextstate = vector<int>(ccnbase) ;
   ccnextstate[0] = -1 ;
   for (int j=0; j<ccnbase; j++)
      ccnextstate[j] = -1 ;
   for (int d=0; d<=ccount+1; d++)
      recurcanonstates2(pd, d, 1, 0) ;
   cout << "Canonical states: " << canonmask.size() << endl ;
   freeContainer(statemap) ;
   freeContainer(ccseen) ;
}
void showcanon(const puzdef &pd, int show) {
   cout.precision(16) ;
   int nstates = canonmask.size() ;
   vector<vector<double> > counts ;
   vector<double> zeros(nstates) ;
   counts.push_back(zeros) ;
   counts[0][0] = 1 ;
   double gsum = 0 ;
   double osum = 1 ;
   for (int d=0; d<=100; d++) {
      while ((int)counts.size() <= d+1)
         counts.push_back(zeros) ;
      double sum = 0 ;
      for (int i=0; i<nstates; i++)
         sum += counts[d][i] ;
      canonseqcnt.push_back((ull)sum) ;
      gsum += sum ;
      canontotcnt.push_back((ull)gsum) ;
      if (show) {
         if (d == 0)
            cout << "D " << d << " this " << sum << " total " << gsum
                 << endl << flush ;
         else
            cout << "D " << d << " this " << sum << " total " << gsum
                 << " br " << (sum / osum) << endl << flush ;
      }
      osum = sum ;
      if (sum == 0 || gsum > 1e18)
         break ;
      for (int st=0; st<nstates; st++) {
         ull mask = canonmask[st] ;
         for (int m=0; m<(int)pd.moves.size(); m++) {
            if ((mask >> pd.moves[m].cs) & 1)
               continue ;
            counts[d+1][canonnext[st][pd.moves[m].cs]] += counts[d][st] ;
         }
      }
   }
}
