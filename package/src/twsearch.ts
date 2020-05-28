interface TwSearchWasmModule extends WebAssembly.Module {
  exports: {
    w_args(s: string): void;
    w_setksolve(s: string): void;
    w_solveposition(s: string): string;
    w_solvescramble(s: string): string;
    _start?: () => void;
  };
}

export class TwSearch {
  private instance: TwSearchWasmModule
  constructor(instance: WebAssembly.Module) {
    this.instance = instance as TwSearchWasmModule;
  }

  async main(): Promise<void> {
    (await this.instance).exports._start();
  }

  async args(s: string): Promise<void> {
    console.log(await this.instance);
    (await this.instance).exports.w_args(s);
  }

  async setKSolve(s: string): Promise<void> {
    (await this.instance).exports.w_setksolve(s);
  }

  async solvePosition(s: string): Promise<string> {
    return (await this.instance).exports.w_solveposition(s);
  }

  async solveScramble(s: string): Promise<string> {
    return (await this.instance).exports.w_solvescramble(s);
  }
}
