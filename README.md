# rudp

![trans rights](https://pride-badges.pony.workers.dev/static/v1?label=trans%20rights&stripeWidth=6&stripeColors=5BCEFA,F5A9B8,FFFFFF,F5A9B8,5BCEFA)
![GitHub repo size](https://img.shields.io/github/repo-size/chickenspaceprogram/rudp)
![C11](https://img.shields.io/badge/C11-blue)


This is an implementation of the RUDP protocol described here:
<https://datatracker.ietf.org/doc/html/draft-ietf-sigtran-reliable-udp-00>

This project does not intend for 100% compliance with the above paper.
Rather, it's loosely based on and takes heavy inspiration from it.

The primary goal is to guarantee reliable, in-order transmission of data
packets. As well, this implementation is designed to remain entirely within
userspace so as to improve portability. This project is intended to be
portable across Linux, BSD, and illumos, although it is primarily tested on
Linux. In future it's possible a Windows port will be developed.

Performance is also (obviously) a goal, however for the moment it's of less
priority. This is a hobby project; while I might optimize for performance later
the goal is currently primarily reliability and getting the thing functioning.

## Project status

## Building

This project uses CMake to build, so you must have that installed.
You'll also need a C compiler and some sort of build toolchain; Unix Makefiles
work fine.

To configure the project for release, enter 
`cmake -B build -DCMAKE_BUILD_TYPE=Release`. Then, to build the project, you
can enter `cmake --build build.` Optionally, you can build with multiple cores;
something like `cmake --build build -j8` builds with 8 cores, for example.

For debug builds, a Makefile is provided for convenience. You should have both
the clang-tidy and cppcheck tools installed, although neither are required.
You'll also want to edit the THREADS variable inside the makefile if your
machine has more or fewer than 8 threads. The build will work regardless but
there's no sense waiting for no reason.

There are multiple different debug targets. If you don't wish to link ASAN,
enter `make dbg`. This will create a debug build that will work nicely with
Valgrind. If you do wish to link ASAN, enter `make asan`. This will not work
nicely with Valgrind, but ASAN itself is useful too.

To clean the build directories without reconfiguring cmake, try `make clean`.
To `rm -rf` the build directories and force CMake to reconfigure next time,
try `make wipe`.
