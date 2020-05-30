import {wrap} from "comlink"
import { TwSearchWorker, TwSearchWorkerConstructor } from "./worker-impl";

export class TwSearch {
  private worker: TwSearchWorker;
  constructor() {
    const twsearchWorkerConstructor = wrap(
      new Worker("./worker-impl.ts")
    ) as any as TwSearchWorkerConstructor;
    this.worker = new twsearchWorkerConstructor();
  }

  async setKPuzzle(def) {
    await (await this.worker).setKPuzzle(def);
  }

  async solveScramble(scramble) {
    return await (await this.worker).solveScramble(scramble);
  }

  async solveState(state) {
    return await (await this.worker).solveState(state);
  }
}
