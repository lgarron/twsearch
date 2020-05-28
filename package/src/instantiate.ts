// import "regenerator-runtime/runtime";

import {stubWasi} from "./wasi/stub"
import {barebonesWASI} from "./wasi/barebones"

// import { WASI as WasmerWASI } from "@wasmer/wasi";
// // import wasiBindings from "@wasmer/wasi/lib/bindings/node";
// import wasiBindings from "@wasmer/wasi/lib/bindings/browser";
// import { WasmFs } from "@wasmer/wasmfs";

async function instantiate(wasmBytes: ArrayBuffer, wasi: WebAssembly.ModuleImports): Promise<WebAssembly.Module> {
  // Specify both APIs for now, just to be safe.
  const moduleImports = {
    wasi_unstable: wasi,
    wasi_snapshot_preview1: wasi,
  };
  const instantiated = await WebAssembly.instantiate(wasmBytes, moduleImports);
  return instantiated.instance;
}

export async function instantiateBarebones(wasmBytes: ArrayBuffer): Promise<WebAssembly.Module> {
  const wasi = barebonesWASI();
  const instance = await instantiate(wasmBytes, wasi)
  wasi.setModuleInstance(instance);
  return instance;
}

export async function instantiateStub(wasmBytes: ArrayBuffer): Promise<WebAssembly.Module> {
  return instantiate(wasmBytes, stubWasi)
}

// export async function instantiateWasmerAndRunMain(wasmBytes: ArrayBuffer, options: {runMain: boolean}): Promise<WebAssembly.Module> {

//   // Instantiate a new WASI Instance
//   const wasmFs = new WasmFs();
//   const wasi = new WasmerWASI({
//     args: [],
//     env: {},
//     bindings: {
//       ...wasiBindings,
//       fs: wasmFs.fs
//     }
//   });
//   let module = await WebAssembly.compile(wasmBytes);
//   let instance = await WebAssembly.instantiate(module, {
//     ...wasi.getImports(module)
//   });

//   if (options?.runMain) {
//     wasi.start(instance);
//     (async() => {
//       const stdout = await wasmFs.getStdOut()
//       console.log("Receiver wasmer stdout!", stdout);
//     })();
//   }

//   return instance;
// }
