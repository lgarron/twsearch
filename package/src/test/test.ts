import {setKPuzzle, solveScramble, solveState} from "../twsearch"
import { algCubingNetLink, parse } from "cubing/alg";

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

setKPuzzle(twsfile);

window.addEventListener("DOMContentLoaded", () => {
  const scrambleField = document.querySelector("#scramble") as HTMLInputElement;
  const solveButton = document.querySelector("#solve") as HTMLButtonElement;
  const solutionField = document.querySelector("#solution") as HTMLInputElement;
  const viewLink = document.querySelector("#view") as HTMLAnchorElement;
  solveButton.addEventListener("click", async () => {
    const scramble = scrambleField.value;

    solutionField.value = "";
    solutionField.classList.add("solving");
    viewLink.hidden = true;

    const solution = await solveScramble(scramble);
    solutionField.value = solution;

    solutionField.classList.remove("solving");
    viewLink.href = algCubingNetLink({
      setup: parse(scramble),
      alg: parse(solution)
    })
    viewLink.hidden = false;
  })
})
