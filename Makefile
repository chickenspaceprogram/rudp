# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.


.PHONY: rel dbg asan wipe clean make

DEBUG=-DCMAKE_BUILD_TYPE=Debug
RELEASE=-DCMAKE_BUILD_TYPE=Release
STATIC_TOOLS=-DRUDP_CLANG_TIDY=ON -DRUDP_CPPCHECK=ON
TESTS=-DRUDP_TESTS=ON
ASAN=-DRUDP_USE_ASAN=ON
SENTINEL=build_sentinel
THREADS=-j8

# this is a makefile i have to shorten the commands I have to type

make: $(SENTINEL)
	: 'build'
	cmake --build build $(THREADS)

rel: wipe
	: 'rel'
	cmake -B build $(RELEASE) $(TESTS) $(STATIC_TOOLS)
	touch $(SENTINEL)
	cmake --build build $(THREADS)

dbg: wipe
	: 'dbg'
	cmake -B build $(DEBUG) $(TESTS) $(STATIC_TOOLS)
	touch $(SENTINEL)
	cmake --build build $(THREADS)

asan: wipe
	: 'asan'
	cmake -B build $(DEBUG) $(TESTS) $(STATIC_TOOLS) $(ASAN)
	touch $(SENTINEL)
	cmake --build build $(THREADS)

wipe:
	: 'wipe'
	rm -rf build
	rm -f $(SENTINEL)

clean:
	: 'clean'
	cmake --build build --target clean

$(SENTINEL):
	: '$(SENTINEL)'
	cmake -B build $(DEBUG) $(TESTS) $(STATIC_TOOLS)
	touch $(SENTINEL)


