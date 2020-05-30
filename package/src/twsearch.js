// import twsearch from "
const {w_args, w_setksolve, w_solvescramble, w_solveposition} = require(__dirname + "/generated/js/twsearch.js");

// Theoretically, we should avoid any other calls to the twsearch API until this
// async call has returned. However, we know that the generated `twsearch.js`
// has all the functions waiting on the same initialization Promise. Since JS is
// single-threaded, we can gbe guaranteed that this call finishes before
// anything below.

w_args("--nowrite");

async function solveKPuzzleScramble(def, scramble) {
  await w_setksolve(twsfile);
  return await w_solvescramble(scramble);
}

async function solveKPuzzleState(def, state) {
  await w_setksolve(twsfile); 
  return await w_solveposition(state);
}

const twsfile = `Name PuzzleGeometryPuzzle

Set CORNER 8 3

Solved
CORNER
1 2 3 4 5 6 7 8
0 0 0 0 0 0 0 0
End

Move F
CORNER
7 1 3 2 5 6 4 8
2 1 0 2 0 0 1 0
End

Move B
CORNER
1 2 5 4 8 3 7 6
0 0 1 0 2 2 0 1
End

Move D
CORNER
1 4 3 8 2 6 7 5
0 0 0 0 0 0 0 0
End

Move U
CORNER
3 2 6 4 5 7 1 8
0 0 0 0 0 0 0 0
End

Move L
CORNER
1 2 3 7 5 8 6 4
0 0 0 1 0 1 2 2
End

Move R
CORNER
2 5 1 4 3 6 7 8
1 2 2 0 1 0 0 0
End
`;

const scrfile = `Scramble CornerTwist
CORNER
1 2 3 4 5 6 7 8
1 2 0 0 0 0 0 0
End
`;

(async () => {
  console.log(await solveKPuzzleScramble(twsfile, "U F R D B L U F R D B L U F R D B L"));
  console.log(await solveKPuzzleState(twsfile, scrfile));
})();
