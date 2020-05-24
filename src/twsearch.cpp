#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <strings.h>
#include <math.h>
#include <cstdio>
#include <functional>
#include "city.h"
#include "util.h"
#include "threads.h"
#include "puzdef.h"
#include "generatingset.h"
#include "orderedgs.h"
#include "readksolve.h"
#include "index.h"
#include "antipode.h"
#include "canon.h"
#include "god.h"
#include "findalgo.h"
#include "workchunks.h"
#include "prunetable.h"
#include "solve.h"
#include "test.h"
#include "parsemoves.h"
#include "filtermoves.h"
#include "parsemoves.h"
#include "cmdlineops.h"
#include "rotations.h"
using namespace std ;
int checkbeforesolve ;
generatingset *gs ;
int bestsolve = 1000000 ;
int optmaxdepth = 0 ;
void dophase2(const puzdef &pd, setval scr, setval p1sol, prunetable &pt,
              const char *p1str) {
   stacksetval p2(pd) ;
   if (optmaxdepth == 0)
      optmaxdepth = maxdepth ;
   pd.mul(scr, p1sol, p2) ;
   maxdepth = min(optmaxdepth - globalinputmovecount,
                  bestsolve - globalinputmovecount - 1) ;
   int r = solve(pd, pt, p2, gs) ;
   if (r >= 0) {
      cout << "Phase one was " << p1str << endl ;
      bestsolve = r + globalinputmovecount ;
      cout << "Found a solution totaling " << bestsolve << " moves." << endl ;
   }
}
void runorderedgs(const puzdef &pd) {
   vector<int> order ;
   int v = -1 ;
   while (cin >> v)
      order.push_back(v) ;
   orderedgs *ogs = new orderedgs(pd, order) ;
   vector<int> r = ogs->getsizes() ;
   for (int i=0; i<(int)r.size(); i++)
      cout << " " << r[i] ;
   cout << endl ;
   delete ogs ;
}
int dogod, docanon, doalgo, dosolvetest, dotimingtest, douniq,
    dosolvelines, doorder, doshowmoves, doshowpositions, genrand,
    checksolvable, doss, doorderedgs,dosyms ;
const char *scramblealgo = 0 ;
const char *legalmovelist = 0 ;

string def3x3x3("# /usr/local/bin/node /Users/rokicki/twizzle/puzzlegeometry/pg.js --optimize --ksolve 3x3x3\
# PuzzleGeometry 0.1 Copyright 2018 Tomas Rokicki.\
# c f 0.333333333333333 optimize true\
\
# Rotations: 24\
# Base planes: 6\
# Face vertices: 4\
# Boundary is Q[1,1,0,0]\
# Distances: face 1 edge 1.414213562373095 vertex 1.732050807568877\
# Faces is now 9\
# Short edge is 0.6666666666666659\
# Total stickers is now 54\
# Move plane sets: 2,2,2\
# Cubies: 26\
# Cubie orbit sizes 6,12,8\
# PuzzleGeometry 0.1 Copyright 2018 Tomas Rokicki.\
# c f 0.333333333333333 optimize true\
Name PuzzleGeometryPuzzle\
\
Set EDGE 12 2\
Set CORNER 8 3\
\
Solved\
EDGE\
1 2 3 4 5 6 7 8 9 10 11 12\
0 0 0 0 0 0 0 0 0 0 0 0\
CORNER\
1 2 3 4 5 6 7 8\
0 0 0 0 0 0 0 0\
End\
\
Move F\
EDGE\
10 1 3 4 2 6 7 8 9 5 11 12\
1 1 0 0 1 0 0 0 0 1 0 0\
CORNER\
7 1 3 2 5 6 4 8\
2 1 0 2 0 0 1 0\
End\
\
Move B\
EDGE\
1 2 6 4 5 7 11 8 9 10 3 12\
0 0 1 0 0 1 1 0 0 0 1 0\
CORNER\
1 2 5 4 8 3 7 6\
0 0 1 0 2 2 0 1\
End\
\
Move D\
EDGE\
1 9 3 2 5 4 7 8 6 10 11 12\
0 0 0 0 0 0 0 0 0 0 0 0\
CORNER\
1 4 3 8 2 6 7 5\
0 0 0 0 0 0 0 0\
End\
\
Move U\
EDGE\
1 2 3 4 5 6 7 11 9 8 12 10\
0 0 0 0 0 0 0 0 0 0 0 0\
CORNER\
3 2 6 4 5 7 1 8\
0 0 0 0 0 0 0 0\
End\
\
Move L\
EDGE\
1 2 3 4 12 6 9 8 5 10 11 7\
0 0 0 0 0 0 0 0 0 0 0 0\
CORNER\
1 2 3 7 5 8 6 4\
0 0 0 1 0 1 2 2\
End\
\
Move R\
EDGE\
4 2 8 3 5 6 7 1 9 10 11 12\
0 0 0 0 0 0 0 0 0 0 0 0\
CORNER\
2 5 1 4 3 6 7 8\
1 2 2 0 1 0 0 0\
End\
\
");

int main(int argc, const char **argv) {
   int seed = 0 ;
   int forcearray = 0 ;
   init_util() ;
   init_threads() ;
   cout << "# This is twsearch 0.1 (C) 2018 Tomas Rokicki." << endl ;
   cout << "#" ;
   for (int i=0; i<argc; i++)
      cout << " " << argv[i] ;
   cout << endl << flush ;
   while (argc > 1 && argv[1][0] == '-') {
      argc-- ;
      argv++ ;
      if (argv[0][1] == '-') {
         if (strcmp(argv[0], "--moves") == 0) {
            legalmovelist = argv[1] ;
            argc-- ;
            argv++ ;
         } else if (strcmp(argv[0], "--showmoves") == 0) {
            doshowmoves++ ;
         } else if (strcmp(argv[0], "--showpositions") == 0) {
            doshowpositions++ ;
         } else if (strcmp(argv[0], "--newcanon") == 0) {
            ccount = atol(argv[1]) ;
            argc-- ;
            argv++ ;
         } else if (strcmp(argv[0], "--nocorners") == 0) {
            nocorners++ ;
         } else if (strcmp(argv[0], "--nocenters") == 0) {
            nocenters++ ;
         } else if (strcmp(argv[0], "--noorientation") == 0) {
            ignoreori = 1 ;
         } else if (strcmp(argv[0], "--noearlysolutions") == 0) {
            noearlysolutions = 1 ;
         } else if (strcmp(argv[0], "--checkbeforesolve") == 0) {
            checkbeforesolve = 1 ;
         } else if (strcmp(argv[0], "--orientationgroup") == 0) {
            origroup = atol(argv[1]) ;
            argc-- ;
            argv++ ;
         } else if (strcmp(argv[0], "--noedges") == 0) {
            noedges++ ;
         } else if (strcmp(argv[0], "--scramblealg") == 0) {
            scramblealgo = argv[1] ;
            argc-- ;
            argv++ ;
         } else if (strcmp(argv[0], "--schreiersims") == 0) {
            doss = 1 ;
         } else if (strcmp(argv[0], "--orderedgs") == 0) {
            doorderedgs = 1 ;
         } else if (strcmp(argv[0], "--showsymmetry") == 0) {
            dosyms = 1 ;
         } else if (strcmp(argv[0], "--nowrite") == 0) {
            nowrite++ ;
         } else if (strcmp(argv[0], "--mindepth") == 0) {
            optmindepth = atol(argv[1]) ;
            argc-- ;
            argv++ ;
         } else {
            error("! Argument not understood ", argv[0]) ;
         }
      } else {
         switch (argv[0][1]) {
case 'q':
            quarter++ ;
            break ;
case 'v':
            verbose++ ;
            if (argv[0][2] != 0)
               verbose = argv[0][2] - '0' ;
            break ;
case 'm':
case 'd':
            maxdepth = atol(argv[1]) ;
            argc-- ;
            argv++ ;
            break ;
case 'r':
            genrand = 1 ;
            break ;
case 'R':
            seed = atol(argv[1]) ;
            argc-- ;
            argv++ ;
            break ;
case 'M':
            maxmem = 1048576 * atoll(argv[1]) ;
            argc-- ;
            argv++ ;
            break ;
case 'y':
            symcoordgoal = atoll(argv[1]) ;
            if (symcoordgoal <= 0)
               symcoordgoal = 1 ;
            argc-- ;
            argv++ ;
            break ;
case 'c':
            solutionsneeded = atoll(argv[1]) ;
            argc-- ;
            argv++ ;
            break ;
case 'g':
            dogod++ ;
            break ;
case 'o':
            doorder++ ;
            break ;
case 'u':
            douniq++ ;
            if (argv[0][2] >= '0')
               proclim = atoll(argv[0]+2) ;
            break ;
case 's':
            dosolvelines++ ;
            break ;
case 'C':
            docanon++ ;
            break ;
case 'F':
            forcearray++ ;
            break ;
case 'a':
            antipodecount = atoll(argv[1]) ;
            argc-- ;
            argv++ ;
            break ;
case 'A':
            if (argv[0][2] == '1')
               doalgo = 1 ;
            else if (argv[0][2] == '2')
               doalgo = 2 ;
            else if (argv[0][2] == '3')
               doalgo = 3 ;
            else
               doalgo = -1 ;
            break ;
case 'T':
            dotimingtest++ ;
            break ;
case 'S':
            dosolvetest++ ;
            if (argv[0][2])
               scramblemoves = atol(argv[0]+2) ;
            break ;
case 't':
            numthreads = atol(argv[1]) ;
            if (numthreads > MAXTHREADS)
               error("Numthreads cannot be more than ", to_string(MAXTHREADS)) ;
            argc-- ;
            argv++ ;
            break ;
case '2':
            phase2 = 1 ;
            break ;
default:
            error("! did not argument ", argv[0]) ;
         }
      }
   }
   if (seed)
      srand48(seed) ;
   else
      srand48(time(0)) ;
   if (argc <= 1)
      error("! please provide a twsearch file name on the command line") ;
   // istringstream f(def3x3x3);
   ifstream f;
#ifndef WASM
   f.open(argv[1], ifstream::in);
#endif
   // if (f == 0)
      // error("! could not open file ", argv[1]) ;
   int sawdot = 0 ;
   for (int i=0; argv[1][i]; i++) {
      if (argv[1][i] == '.')
         sawdot = 1 ;
      else if (argv[1][i] == '/' || argv[1][i] == '\\') {
         sawdot = 0 ;
         inputbasename.clear() ;
      } else if (!sawdot)
         inputbasename.push_back(argv[1][i]) ;
   }
   puzdef pd = readdef(&f) ;
   addmovepowers(pd) ;
   if (legalmovelist)
      filtermovelist(pd, legalmovelist) ;
   if (nocorners)
      pd.addoptionssum("nocorners") ;
   if (nocenters)
      pd.addoptionssum("nocenters") ;
   if (noedges)
      pd.addoptionssum("noedges") ;
   if (doss || checkbeforesolve)
      gs = new generatingset(pd) ;
   if (doorderedgs)
      runorderedgs(pd) ;
   if (genrand) {
      showrandompos(pd) ;
      return 0 ;
   }
   if (pd.rotations.size())
      calcrotations(pd) ;
   calculatesizes(pd) ;
   calclooseper(pd) ;
   if (ccount == 0)
      makecanonstates(pd) ;
   else
      makecanonstates2(pd) ;
   cout << "Calculated canonical states in " << duration() << endl << flush ;
   showcanon(pd, docanon) ;
//   gensymm(pd) ;
   if (dogod) {
      int statesfit2 = pd.logstates <= 50 && ((ll)(pd.llstates >> 2)) <= maxmem ;
      int statesfitsa = forcearray ||
          (pd.logstates <= 50 &&
             ((ll)(pd.llstates * sizeof(loosetype) * looseper) <= maxmem)) ;
      if (statesfit2 && pd.canpackdense() && pd.rotations.size() == 0) {
         cout << "Using twobit arrays." << endl ;
         dotwobitgod2(pd) ;
      } else if (statesfitsa) {
         if (pd.rotations.size()) {
            cout << "Using sorting bfs symm and arrays." << endl ;
            doarraygodsymm(pd) ;
         } else {
            cout << "Using sorting bfs and arrays." << endl ;
            doarraygod(pd) ;
         }
      } else {
         cout << "Using canonical sequences and arrays." << endl ;
         doarraygod2(pd) ;
      }
   }
   if (doalgo)
      findalgos(pd, doalgo) ;
   if (dosolvetest)
      solvetest(pd, gs) ;
   if (dotimingtest)
      timingtest(pd) ;
   if (! phase2 && scramblealgo)
      solvecmdline(pd, scramblealgo) ;
   if (douniq)
      processlines(pd, uniqit) ;
   if (dosyms)
      processlines(pd, symsit) ;
   if (doorder)
      processlines2(pd, orderit) ;
   if (doshowmoves)
      processlines2(pd, emitmove) ;
   if (doshowpositions)
      processlines(pd, emitposition) ;
   if (dosolvelines) {
      prunetable pt(pd, maxmem) ;
      string emptys ;
      processlines(pd, [&](const puzdef &pd, setval p, const char *) {
                          solveit(pd, pt, emptys, p) ;
                       }) ;
   }
   if (phase2) {
      if (argc <= 2 && !scramblealgo)
         error("! need a scramble file for phase 2") ;
      stacksetval scr(pd) ;
      if (scramblealgo) {
         pd.assignpos(scr, pd.solved) ;
         vector<setval> movelist = parsemovelist_generously(pd, scramblealgo) ;
         for (int i=0; i<(int)movelist.size(); i++)
            domove(pd, scr, movelist[i]) ;
      } else {
         ifstream scrambles;
#ifndef WASM
         scrambles.open(argv[2], ifstream::in);
#endif
         // if (f == 0)
         //    error("! could not open scramble file ", argv[2]) ;
         readfirstscramble(&scrambles, pd, scr) ;
#ifndef WASM
         scrambles.close();
#endif
      }
      prunetable pt(pd, maxmem) ;
      processlines2(pd, [&](const puzdef &pd, setval p1sol, const char *p1str) {
                               dophase2(pd, scr, p1sol, pt, p1str); }) ;
   } else if (argc > 2) {
         ifstream scrambles;
#ifndef WASM
         scrambles.open(argv[2], ifstream::in);
#endif
      // if (f == 0)
         // error("! could not open scramble file ", argv[2]) ;
      processscrambles(&scrambles, pd) ;
#ifndef WASM
      scrambles.close();
#endif
   }
}
