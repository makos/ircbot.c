# ircbot.c

Cross-platform (Windows and Linux) IRC bot in pure C.

Developed on Windows under mingw-w64 with gcc-8.1, tested on Windows, Arch Linux and CentOS 7.

Documentation can be found at [GitHub pages](https://makos.github.io/ircbot.c/index.html).

## Requirements

All platforms:
* gcc
* CMake `>=3.0.0`
* GNU make

No third-party C libraries needed.

## Building

As of `v0.2.0` only tests can be compiled and run.

````shell
git clone https://github.com/makos/ircbot.c.git
cd ircbot.c
mkdir build && cd build
cmake .. && make tests
tests/tests all
````

Use `tests/tests keepalive` to run a stability test. It will wait for and respond to server PING's 4 times before concluding.
