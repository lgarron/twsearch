.PHONY: dev
dev: node_modules wasm
	npx parcel src/test/index.html

# Set as `.PHONY` in order to rely on the parent dependency calculations.
.PHONY: wasm
wasm:
	cd .. && make package/src/wasm/generated/twsearch-wrapper.js

node_modules:
	npm install

.PHONY: clean
clean:
	rm -rf \
		src/wasm/generated/twsearch-wrapper.js \
		.cache \
		.parcel-cache
