# rudp/src

This directory contains the main modules that form the rudp library.
Each module is structured roughly as follows:

~~~
modulename
|-- CMakeLists.txt
|-- include
    |-- rudp
        |-- (public headers)
|-- tests
    |-- (test files)
|-- (implementation files, private headers)
~~~

Most modules have some user-visible function; you can use rudp's generic polling instead of a platform-specific polling function if you want (although this is not required).

More module info:

| Module name | Description                           |
|-------------|---------------------------------------|
|     mem     | Custom allocator support              |
|   polling   | Generic fd polling and generic timers |


