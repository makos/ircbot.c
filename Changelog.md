# Changelog

## 0.3.0
### 22.09.2018
* Doxygen-generated documentation for libbot API available at [GitHub pages](https://makos.github.io/ircbot.c/index.html).
* IRC_Bot, along with internal IRC_Connection is now compiled into a shared library `libbot`.
* Unit tests for all functions.
* Mock server for testing purposes (`tests/server`).
* IRC_Bot has all the required features to be able to connect to a server, send and receive
  messages and execute arbitrary commands that can be added at runtime(`Bot_Command`).

## 0.2.0 
### 15.09.2018
* Consolidate `server` and `client` functionality into one `connection` file.
* Cross-platform (at least it compiles on Arch Linux VM)
* Tests using custom minimal `minunit` test suite
* New `bot` skeleton

## 0.1.0 
### 13.09.2018
* Stable connection with an irc server
* First version
