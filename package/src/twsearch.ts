// We need to use the default import due to `node` limitations.
import {w_args, w_setksolve, w_solvescramble, w_solveposition} from "./wasm/generated/twsearch-wrapper"
import {KPuzzle} from "cubing/kpuzzle";

// Theoretically, we should avoid any other calls to the twsearch API until this
// async call has returned. However, we know that the generated `twsearch.js`
// has all the functions waiting on  the same initialization Promise. Since JS is
// single-threaded, we can be guaranteed that this call finishes before
// anything below.
w_args("--nowrite");

export async function setKPuzzle(def) {
  await w_setksolve(def);
}

export async function solveScramble(scramble) {
  return await w_solvescramble(scramble);
}

export async function solveState(state) {
  return await w_solveposition(state);
}
