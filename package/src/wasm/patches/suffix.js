
const initialized = new Promise((resolve) => {
  Module.onRuntimeInitialized = resolve
});

function onceInitialized(fn) {
  return async (...args) => {
    await initialized;
    return fn(...args);
  }
}

module.exports = {
  w_args: onceInitialized(cwrap('w_args', 'void', ['string'])),
  w_setksolve: onceInitialized(cwrap('w_setksolve', 'void', ['string'])),
  w_solvescramble: onceInitialized(cwrap('w_solvescramble', 'string', ['string'])),
  w_solveposition: onceInitialized(cwrap('w_solveposition', 'string', ['string'])),
};
