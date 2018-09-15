# ircbot.c

Cross-platform (Windows and Linux) IRC bot in pure C.

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
build/tests/tests all
````

Use `build/tests/tests keepalive` to run a stability test. It will wait for and respond to server PING's 4 times before concluding.
