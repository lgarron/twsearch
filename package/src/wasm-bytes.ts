
// It doesn't seem currently possible to use Parcel in a nice way that:
//
// 1. works in `node` and in Parcel (and ideally direclty in the browser), and
// 2. works with `WebAssembly.instantiate` (which is hard to avoid for C++
//    code).
//
// This file foregoes the latter in favor of the former, using the feature of
// Parcel that inlines `readFileSync` calls.
//
// Benefits:
//
// - The wasm source is inline in the build, and doesn't have to be saved or
//   served separately.
//
// Drawbacks:
//
// - The `wasm` file can't be downloaded separately and cached in the normal
//   way.
// - We can't use streaming instantiation (more performant).
//
// There will hopefully be a better way in the future.

import { readFileSync } from "fs";

const twsearchWasmFileContents = readFileSync(__dirname + "/generated/twsearch.wasm", "binary");
const wasmtestWasmFileContents = readFileSync(__dirname + "/generated/wasmtest.wasm", "binary");

function stringToArrayBuffer(str: string): ArrayBuffer {
  // Binary string to buffer
  const buffer = new ArrayBuffer(str.length);
  const byteView = new Uint8Array(buffer);
  for (let i = 0; i < str.length; i++) {
    byteView[i] = str.charCodeAt(i);
  }
  return buffer;
}

const twsearchBytes = stringToArrayBuffer(twsearchWasmFileContents);
const wasmtestBytes = stringToArrayBuffer(wasmtestWasmFileContents);
export {twsearchBytes, wasmtestBytes};
