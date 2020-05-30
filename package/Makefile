.PHONY: build
build: node_modules wasm
	npx parcel build --public-url "https://experiments.cubing.net/twsearch.js" src/test/index.html

.PHONY: dev
dev: node_modules wasm
	npx parcel src/test/index.html

# Set as `.PHONY` in order to rely on the parent dependency calculations.
.PHONY: wasm
wasm:
	cd .. && make package/src/wasm/generated/twsearch-wrapper.js

node_modules:
	npm install

SFTP_PATH = "towns.dreamhost.com:~/experiments.cubing.net/twsearch.js"
URL       = "https://experiments.cubing.net/twsearch.js"

.PHONY: deploy
deploy: build
	rsync -avz \
		--exclude .DS_Store \
		--exclude .git \
		./dist/ \
		${SFTP_PATH}
	echo "\nDone deploying. Go to ${URL}\n"

.PHONY: clean
clean:
	rm -rf \
		dist/ \
		src/wasm/generated/twsearch-wrapper.js \
		.cache \
		.parcel-cache
