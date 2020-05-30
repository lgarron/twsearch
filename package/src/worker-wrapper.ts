import {wrap} from "comlink"
import { TwSearch, TwSearchConstructor } from "./worker-impl";

export const constructor = wrap(
  new Worker("./worker-impl.ts")
) as any as TwSearchConstructor;
