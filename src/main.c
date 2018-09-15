/**
 * @mainpage
 *
 * @section index_about About
 * This is a cross-platform (Linux and Windows tested) IRC bot written purely in
 * C without third-party libraries.
 *
 * @section index_req Requirements
 * For building from source: (all platforms)
 *  - CMake (version >= 3.0.0)
 *  - gcc
 *  - preferably GNU make
 *
 * @section index_building Building
 * Navigate to the folder with source code (where README.md is located). Then:
 * ````
 * mkdir build && cd build
 * cmake .. && make tests
 * tests/tests all
 * ````
 * As of version 0.2.0 only tests will compile and run.
 */

/* TODO: list of things I'd like to have:
 * command-line for the bot (send message, whisper, join/leave channels)
 * UTF-8 printing
 * cross-platform
 * multi-channel capability
 * automatic response to common IRC queries
 * some ice cream
 */

int main(int argc, char const *argv[])
{
    return 0;
}
