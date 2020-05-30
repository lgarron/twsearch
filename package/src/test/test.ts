import { setKPuzzle, solveScramble, solveState } from "../twsearch";
import { algCubingNetLink, parse } from "cubing/alg";

const lastDefinition = "";
const definitionField = document.querySelector(
  ".definition"
) as HTMLInputElement;

class SolverSection {
  private section: HTMLDivElement;
  private toSolveField: HTMLInputElement;
  private solveButton: HTMLButtonElement;
  private solutionField: HTMLInputElement;
  private viewLink: HTMLAnchorElement;
  private viewLinkButton: HTMLButtonElement;
  private currentToSolve: string;
  constructor(selector: string, private fileInput: boolean) {
    this.section = document.querySelector(selector);
    this.toSolveField = this.section.querySelector(
      ".to-solve"
    ) as HTMLInputElement;
    this.solveButton = this.section.querySelector(
      ".solve"
    ) as HTMLButtonElement;
    this.solutionField = this.section.querySelector(
      ".solution"
    ) as HTMLInputElement;
    this.viewLink = this.section.querySelector(".view") as HTMLAnchorElement;
    this.viewLinkButton = this.viewLink.querySelector(
      "button"
    ) as HTMLButtonElement;

    this.solveButton.addEventListener("click", this.onStartSolve.bind(this));
  }

  async onStartSolve(): Promise<void> {
    const def = definitionField.value;
    if (def != lastDefinition) {
      await setKPuzzle(def);
    }
    this.currentToSolve = this.toSolveField.value;
    console.log(this.section.offsetWidth);
    this.startedSolving();
    // setTimeout(async () => {
    // In the general case, we need to make sure we get back solutions in the
    // right order. But we happen to know that will happen in this case.
    if (this.fileInput) {
      this.finishedSolving(await solveState(this.currentToSolve));
    } else {
      this.finishedSolving(await solveScramble(this.currentToSolve));
    }
  }

  startedSolving(): void {
    this.solutionField.value = "";
    this.solutionField.classList.add("solving");
    this.viewLinkButton.hidden = true;
  }

  finishedSolving(solution: string): void {
    this.solutionField.classList.remove("solving");
    this.solutionField.value = solution;
    if (this.fileInput) {
      this.viewLink.href = algCubingNetLink({
        alg: parse(solution),
        type: "reconstruction-end-with-setup",
      });
    } else {
      this.viewLink.href = algCubingNetLink({
        setup: parse(this.currentToSolve),
        alg: parse(solution),
      });
    }
    this.viewLinkButton.hidden = false;
  }
}

window.addEventListener("DOMContentLoaded", () => {
  new SolverSection("#scramble-input", false);
  new SolverSection("#file-input", true);
});
