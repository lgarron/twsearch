import "../twsearch"

// import { instantiateBarebones, instantiateStub } from "../instantiate";
// import { TwSearch } from "../twsearch";
// import { twsearchBytes, wasmtestBytes } from "../wasm-bytes";

// document.querySelector("#wasmtest-barebones")!.addEventListener("click", async () => {
//   const instance = instantiateBarebones(wasmtestBytes);
//   const twsearch = new TwSearch(await instance);
//   await twsearch.main();
// })

// document.querySelector("#wasmtest-stub")!.addEventListener("click", async () => {
//   const instance = instantiateStub(wasmtestBytes);
//   const twsearch = new TwSearch(await instance);
//   await twsearch.main();
// })

// // document.querySelector("#wasmtest-wasmer")!.addEventListener("click", async () => {
// //   instantiateWasmerAndRunMain(wasmtestBytes, {runMain: true});
// // })

// document.querySelector("#twsearch-barebones")!.addEventListener("click", async () => {
//   const instance = instantiateBarebones(twsearchBytes)
//   const twsearch = new TwSearch(await instance);
//   await twsearch.args("--nowrite");
// })

// document.querySelector("#twsearch-stub")!.addEventListener("click", async () => {
//   const instance = instantiateStub(twsearchBytes)
//   const twsearch = new TwSearch(await instance);
//   await twsearch.args("--nowrite");
// })

// // document.querySelector("#twsearch-wasmer")!.addEventListener("click", async () => {
// //   const instance = instantiateWasmerAndRunMain(twsearchBytes, {runMain: false});
// //   const twsearch = new TwSearch(await instance);
// //   await twsearch.args("--nowrite");
// // })
