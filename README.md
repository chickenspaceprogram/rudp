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
It also has a Makefile, for convenience, so you probably should also have some
version of make installed. To build the project for release, enter `make rel`.
Thereafter, to rebuild the project, you can enter just `make`.

There are multiple different debug targets. If you don't wish to link ASAN,
enter `make dbg`. This will create a debug build that will work nicely with GDB
and the like. If you do wish to link ASAN, enter `make asan`. 

To clean the build directories without reconfiguring cmake, try `make clean`.
To `rm -rf` the build directories and force CMake to reconfigure next time,
try `make wipe`.

It's possible to build using CMake directly, the Makefile in the project's
root directory is just a convenience. See the Makefile for the commands you'll
need to run.
