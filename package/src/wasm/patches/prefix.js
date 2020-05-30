const {readFileSync} = require("fs");

const wasmFileContents = readFileSync(__dirname + "/twsearch.wasm", "binary");

function stringToArrayBuffer(str) {
  // Binary string to buffer
  const buffer = new ArrayBuffer(str.length);
  const byteView = new Uint8Array(buffer);
  for (let i = 0; i < str.length; i++) {
    byteView[i] = str.charCodeAt(i);
  }
  return buffer;
}

Module.wasmBinary = stringToArrayBuffer(wasmFileContents);
