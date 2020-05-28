.PHONY: dev
dev: node_modules wasm
	npx parcel src/test/index.html

# Set as `.PHONY` in order to rely on the parent dependency calculations.
.PHONY: wasm
wasm:
	cd .. && make package/src/generated/twsearch.wasm package/src/generated/wasmtest.wasm

node_modules:
	npm install
