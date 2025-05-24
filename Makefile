.PHONY: rel dbg asan wipe clean make

# this is a makefile i have to shorten the commands I have to type

make: build_sentinel
	: 'build'
	cmake --build build -j8

rel: wipe
	: 'rel'
	cmake -B build -DCMAKE_BUILD_TYPE=Release
	touch build_sentinel
	cmake --build build -j8

dbg: wipe
	: 'dbg'
	cmake -B build -DRUDP_DEBUG=ON
	touch build_sentinel
	cmake --build build -j8

asan: wipe
	: 'asan'
	cmake -B build -DRUDP_DEBUG=ON -DRUDP_USE_ASAN=ON
	touch build_sentinel
	cmake --build build -j8

wipe:
	: 'wipe'
	rm -rf build
	rm -f build_sentinel

clean:
	: 'clean'
	cmake --build build --target clean

build_sentinel:
	: 'build_sentinel'
	cmake -B build -DRUDP_DEBUG=ON
	touch build_sentinel


