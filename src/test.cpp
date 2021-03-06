#include "test.h"
#include "prunetable.h"
#include "solve.h"
#include <iostream>
int scramblemoves = 1 ;
void timingtest(puzdef &pd) {
   stacksetval p1(pd), p2(pd) ;
   pd.assignpos(p1, pd.solved) ;
   cout << "Timing moves." << endl << flush ;
   duration() ;
   int cnt = 100000000 ;
   for (int i=0; i<cnt; i += 2) {
      int rmv = myrand(pd.moves.size()) ;
      pd.mul(p1, pd.moves[rmv].pos, p2) ;
      rmv = myrand(pd.moves.size()) ;
      pd.mul(p2, pd.moves[rmv].pos, p1) ;
   }
   double tim = duration() ;
   cout << "Did " << cnt << " in " << tim << " rate " << cnt/tim/1e6 << endl << flush ;
   cout << "Timing moves plus hash." << endl << flush ;
   duration() ;
   cnt = 100000000 ;
   ull sum = 0 ;
   for (int i=0; i<cnt; i += 2) {
      int rmv = myrand(pd.moves.size()) ;
      pd.mul(p1, pd.moves[rmv].pos, p2) ;
      sum += fasthash(pd.totsize, p2) ;
      rmv = myrand(pd.moves.size()) ;
      pd.mul(p2, pd.moves[rmv].pos, p1) ;
      sum += fasthash(pd.totsize, p1) ;
   }
   tim = duration() ;
   cout << "Did " << cnt << " in " << tim << " rate " << cnt/tim/1e6 << " sum " << sum << endl << flush ;
   prunetable pt(pd, maxmem) ;
   cout << "Timing moves plus lookup." << endl << flush ;
   duration() ;
   cnt = 100000000 ;
   sum = 0 ;
   for (int i=0; i<cnt; i += 2) {
      int rmv = myrand(pd.moves.size()) ;
      pd.mul(p1, pd.moves[rmv].pos, p2) ;
      sum += pt.lookup(p2) ;
      rmv = myrand(pd.moves.size()) ;
      pd.mul(p2, pd.moves[rmv].pos, p1) ;
      sum += pt.lookup(p1) ;
   }
   tim = duration() ;
   cout << "Did " << cnt << " in " << tim << " rate " << cnt/tim/1e6 << " sum " << sum << endl << flush ;
   const int MAXLOOK = 128 ;
   ull tgo[MAXLOOK] ;
   for (int look=2; look<=MAXLOOK; look *= 2) {
      int mask = look - 1 ;
      for (int i=0; i<look; i++)
         tgo[i] = 0 ;
      cout << "Timing moves plus lookup piped " << look << endl << flush ;
      duration() ;
      cnt = 100000000 ;
      sum = 0 ;
      for (int i=0; i<cnt; i += 2) {
         int rmv = myrand(pd.moves.size()) ;
         pd.mul(p1, pd.moves[rmv].pos, p2) ;
         sum += pt.lookuph(tgo[i&mask]) ;
         tgo[i&mask] = fasthash(pd.totsize, p2) ;
         pt.prefetch(tgo[i&mask]) ;
         rmv = myrand(pd.moves.size()) ;
         pd.mul(p2, pd.moves[rmv].pos, p1) ;
         sum += pt.lookuph(tgo[1+(i&mask)]) ;
         tgo[1+(i&mask)] = fasthash(pd.totsize, p1) ;
         pt.prefetch(tgo[1+(i&mask)]) ;
      }
      tim = duration() ;
      cout << "Did " << cnt << " in " << tim << " rate " << cnt/tim/1e6 << " sum " << sum << endl << flush ;
   }
}
void solvetest(puzdef &pd, generatingset *gs) {
   stacksetval p1(pd), p2(pd) ;
   pd.assignpos(p1, pd.solved) ;
   prunetable pt(pd, maxmem) ;
   while (1) {
      solve(pd, pt, p1, gs) ;
      for (ll i=0; i<scramblemoves; i++) {
         while (1) {
            int rmv = myrand(pd.moves.size()) ;
            pd.mul(p1, pd.moves[rmv].pos, p2) ;
            if (pd.legalstate(p2)) {
               pd.assignpos(p1, p2) ;
               break ;
            }
         }
      }
   }
}
