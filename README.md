# user-rudp

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
