#include <map>
#include <iostream>
#include "canon.h"
#include "findalgo.h"
#include "threads.h"
map<ll, ll> bestsofar ;
const int HIWR = 4 ;
ll extendkey(ll k, int nwr, int npwr) {
   return k * 10 + nwr * 2 + (npwr == 0 ? 0 : 1) ;
}
ll bigcnt = 0 ;
struct algo1worker {
   void recurfindalgo(const puzdef &pd, int togo, int sp, int st) {
      if (togo == 0) {
         bigcnt++ ;
         int wr = pd.numwrong(posns[sp], pd.solved) ;
         if (wr > HIWR || wr == 0)
            return ;
         ll key = 0 ;
         for (int i=0; i<(int)pd.setdefs.size(); i++) {
            key = extendkey(key,
                             pd.numwrong(posns[sp], pd.solved, 1LL << i),
                             pd.permwrong(posns[sp], pd.solved, 1LL << i)) ;
         }
         int mvs = sp ;
         get_global_lock() ;
         if (bestsofar.find(key) == bestsofar.end() || bestsofar[key] >= mvs) {
            bestsofar[key] = mvs ;
            cout << key << " " << mvs << " (" ;
            for (int i=0; i<sp; i++) {
               if (i)
                  cout << " " ;
               cout << pd.moves[movehist[i]].name ;
            }
            cout << ")" << endl << flush ;
         }
         release_global_lock() ;
         return ;
      }
      ull mask = canonmask[st] ;
      const vector<int> &ns = canonnext[st] ;
      for (int m=0; m<(int)pd.moves.size(); m++) {
         const moove &mv = pd.moves[m] ;
         if ((mask >> mv.cs) & 1)
            continue ;
         movehist[sp] = m ;
         pd.mul(posns[sp], mv.pos, posns[sp+1]) ;
         recurfindalgo(pd, togo-1, sp+1, ns[mv.cs]) ;
      }
   }
   void findalgos1(const puzdef &pd) {
      posns.clear() ;
      movehist.clear() ;
      for (int d=1; ; d++) {
         while ((int)posns.size() <= d + 1) {
            posns.push_back(allocsetval(pd, pd.id)) ;
            movehist.push_back(-1) ;
         }
         bigcnt = 0 ;
         recurfindalgo(pd, d, 0, 0) ;
      }
   }
   vector<int> movehist ;
   vector<allocsetval> posns ;
} algo1worker ;
void *doalgo1work(void *o) {
   const puzdef *pd = (const puzdef *)o ;
   algo1worker.findalgos1(*pd) ;
   return 0 ;
}
struct algo2worker {
   void recurfindalgo2(const puzdef &pd, int togo, int sp, int st) {
      if (togo == 0) {
         vector<int> cc = pd.cyccnts(posns[sp]) ;
         ll o = puzdef::order(cc) ;
         for (int pp=2; pp<=3; pp++) {
            if (o % pp == 0) {
               pd.pow(posns[sp], posns[sp+1], o/pp) ;
               int wr = pd.numwrong(posns[sp+1], pd.id) ;
               if (wr > HIWR || wr == 0)
                  continue ;
               ll key = 0 ;
               for (int i=0; i<(int)pd.setdefs.size(); i++) {
                  key = extendkey(key, pd.numwrong(posns[sp+1], pd.id, 1LL << i),
                                   pd.permwrong(posns[sp+1], pd.id, 1LL << i)) ;
               }
               ll mvs = o / pp * sp ;
               get_global_lock() ;
               if (bestsofar.find(key) == bestsofar.end() || bestsofar[key] >= mvs) {
                  bestsofar[key] = mvs ;
                  cout << key << " " << mvs << " (" ;
                  for (int i=0; i<sp; i++) {
                     if (i)
                        cout << " " ;
                     cout << pd.moves[movehist[i]].name ;
                  }
                  cout << ")" << (mvs / sp) << " (" ;
                  const char *spacer = "" ;
                  for (int i=1; i<(int)cc.size(); i++) {
                     if (cc[i]) {
                        cout << spacer ;
                        spacer = " " ;
                        cout << i << ":" << cc[i] ;
                     }
                  }
                  cout << ") " ;
                  cout << o << endl << flush ;
               }
               release_global_lock() ;
            }
         }
         return ;
      }
      ull mask = canonmask[st] ;
      const vector<int> &ns = canonnext[st] ;
      for (int m=0; m<(int)pd.moves.size(); m++) {
         const moove &mv = pd.moves[m] ;
         if ((mask >> mv.cs) & 1)
            continue ;
         movehist[sp] = m ;
         pd.mul(posns[sp], mv.pos, posns[sp+1]) ;
         recurfindalgo2(pd, togo-1, sp+1, ns[mv.cs]) ;
      }
   }
   void findalgos2(const puzdef &pd) {
      posns.clear() ;
      movehist.clear() ;
      for (int d=1; ; d++) {
         while ((int)posns.size() <= d + 3) {
            posns.push_back(allocsetval(pd, pd.id)) ;
            movehist.push_back(-1) ;
         }
         recurfindalgo2(pd, d, 0, 0) ;
      }
   }
   vector<int> movehist ;
   vector<allocsetval> posns ;
} algo2worker ;
void *doalgo2work(void *o) {
   const puzdef *pd = (const puzdef *)o ;
   algo2worker.findalgos2(*pd) ;
   return 0 ;
}
struct algo3worker {
   void recurfindalgo3b(const puzdef &pd, int togo, int sp, int st, int fp) {
      if (togo == 0) {
         pd.inv(posns[sp], posns[sp+1]) ;
         pd.mul(posns[fp], posns[sp], posns[sp+2]) ;
         pd.mul(posns[sp+2], posns[fp+1], posns[sp+3]) ;
         pd.mul(posns[sp+3], posns[sp+1], posns[sp+2]) ;
         int wr = pd.numwrong(posns[sp+2], pd.id) ;
         if (wr > HIWR || wr == 0)
            return ;
         ll key = 0 ;
         for (int i=0; i<(int)pd.setdefs.size(); i++) {
            key = extendkey(key, pd.numwrong(posns[sp+2], pd.id, 1LL << i),
                             pd.permwrong(posns[sp+2], pd.id, 1LL << i)) ;
         }
         int mvs = 2 * (fp + (sp - (fp + 2))) ;
         get_global_lock() ;
         if (bestsofar.find(key) == bestsofar.end() || bestsofar[key] >= mvs) {
            bestsofar[key] = mvs ;
            cout << key << " " << mvs << " [" ;
            for (int i=0; i<fp; i++) {
               if (i)
                  cout << " " ;
               cout << pd.moves[movehist[i]].name ;
            }
            cout << "," ;
            for (int i=fp+2; i<sp; i++) {
               if (i != fp+2)
                  cout << " " ;
               cout << pd.moves[movehist[i]].name ;
            }
            cout << "]" << endl << flush ;
         }
         release_global_lock() ;
         return ;
      }
      ull mask = canonmask[st] ;
      const vector<int> &ns = canonnext[st] ;
      for (int m=0; m<(int)pd.moves.size(); m++) {
         const moove &mv = pd.moves[m] ;
         if ((mask >> mv.cs) & 1)
            continue ;
         movehist[sp] = m ;
         pd.mul(posns[sp], mv.pos, posns[sp+1]) ;
         recurfindalgo3b(pd, togo-1, sp+1, ns[mv.cs], fp) ;
      }
   }
   void recurfindalgo3a(const puzdef &pd, int togo, int sp, int st, int b) {
      if (togo == 0) {
         pd.inv(posns[sp], posns[sp+1]) ;
         pd.assignpos(posns[sp+2], pd.id) ;
         recurfindalgo3b(pd, b, sp+2, 0, sp) ;
         return ;
      }
      ull mask = canonmask[st] ;
      const vector<int> &ns = canonnext[st] ;
      for (int m=0; m<(int)pd.moves.size(); m++) {
         const moove &mv = pd.moves[m] ;
         if ((mask >> mv.cs) & 1)
            continue ;
         movehist[sp] = m ;
         pd.mul(posns[sp], mv.pos, posns[sp+1]) ;
         recurfindalgo3a(pd, togo-1, sp+1, ns[mv.cs], b) ;
      }
   }
   void findalgos3(const puzdef &pd) {
      for (int d=2; ; d++) {
         posns.clear() ;
         movehist.clear() ;
         while ((int)posns.size() <= d + 7) {
            posns.push_back(allocsetval(pd, pd.id)) ;
            movehist.push_back(-1) ;
         }
         for (int a=1; a+a<=d; a++)
            recurfindalgo3a(pd, d-a, 0, 0, a) ;
      }
   }
   vector<int> movehist ;
   vector<allocsetval> posns ;
} algo3worker ;
void *doalgo3work(void *o) {
   const puzdef *pd = (const puzdef *)o ;
   algo3worker.findalgos3(*pd) ;
   return 0 ;
}
void findalgos(const puzdef &pd, int which) {
   if (which < 0 || which == 1) {
      spawn_thread(0, doalgo1work, (void *)&pd) ;
   }
   if (which < 0 || which == 2) {
      spawn_thread(1, doalgo2work, (void *)&pd) ;
   }
   if (which < 0 || which == 3) {
      spawn_thread(2, doalgo3work, (void *)&pd) ;
   }
   if (which < 0 || which == 1)
      join_thread(0) ;
   if (which < 0 || which == 2)
      join_thread(1) ;
   if (which < 0 || which == 3)
      join_thread(2) ;
}
