default: twsearch

all: twsearch package/src/wasm/generated/twsearch-wrapper.js

CXXFLAGS = -O3 -Wextra -Wall -pedantic -std=c++14 -g -march=native -Wsign-compare
COMMON_FLAGS = -DHAVE_FFSLL -Isrc -Isrc/cityhash/src
LDFLAGS = -lpthread

CSOURCE = src/antipode.cpp src/calcsymm.cpp src/canon.cpp src/cmdlineops.cpp \
   src/filtermoves.cpp src/findalgo.cpp src/generatingset.cpp src/god.cpp \
   src/index.cpp src/parsemoves.cpp src/prunetable.cpp src/puzdef.cpp \
   src/readksolve.cpp src/solve.cpp src/test.cpp src/threads.cpp \
   src/twsearch.cpp src/util.cpp src/workchunks.cpp src/rotations.cpp \
   src/orderedgs.cpp src/wasmapi.cpp

HSOURCE = src/antipode.h src/calcsymm.h src/canon.h src/cmdlineops.h \
   src/filtermoves.h src/findalgo.h src/generatingset.h src/god.h src/index.h \
   src/parsemoves.h src/prunetable.h src/puzdef.h src/readksolve.h src/solve.h \
   src/test.h src/threads.h src/util.h src/workchunks.h src/rotations.h \
   src/orderedgs.h src/wasmapi.h src/twsearch.h

CITYSRC = src/cityhash/src/city.cc

BINARY_FLAGS = $(COMMON_FLAGS) -DUSE_PTHREADS
twsearch: $(CSOURCE) $(HSOURCE)
	$(CXX) $(CXXFLAGS) $(BINARY_FLAGS) -o twsearch $(CSOURCE) $(CITYSRC) $(LDFLAGS)

package/src/wasm/generated/twsearch-wrapper.js: $(CSOURCE) $(HSOURCE) package/src/wasm/patches/prefix.js package/src/wasm/patches/suffix.js
	mkdir -p package/src/wasm/generated/
	em++ $(WASM_CXX) $(CXXFLAGS) $(COMMON_FLAGS) \
		-s ALLOW_MEMORY_GROWTH=1 -fno-exceptions -DWASM -DASLIBRARY \
		--pre-js package/src/wasm/patches/prefix.js \
		--post-js package/src/wasm/patches/suffix.js \
		-o package/src/wasm/generated/twsearch.js $(CSOURCE) $(CITYSRC)
		mv package/src/wasm/generated/twsearch.js package/src/wasm/generated/twsearch-wrapper.js

.PHONY: clean
clean:
	rm -rf \
		./twsearch \
		./package/src/wasm/generated/
