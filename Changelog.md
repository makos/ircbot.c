# Changelog

## 1.0.0
### WIP
* Major version change because of incompatible API changes.
* New helper library `libdebug`, for toggleable logging or printing various
  messages.
* Unit tests for `libdebug`.
* Documentation for `libdebug`.
#### API change
* Remove Commands system from IRC_Bot, in favor of user-defined functions in 
  their software to carry out functions. Example in Chatterbot.
* Fixed (removed) tests that were broken because of API change.
#### Chatter, simple bot as an example
* Basic functionality: connecting, maintaining connection, reading and sending
  data, parsing of incoming traffic and responding to basic queries (`PING`,
  `VERSION`).

## 0.3.0
### 22.09.2018
* Doxygen-generated documentation for libbot API available at 
  [GitHub pages](https://makos.github.io/ircbot.c/index.html).
* IRC_Bot, along with internal IRC_Connection is now compiled into a shared 
  library `libbot`.
* Unit tests for all functions.
* Mock server for testing purposes (`tests/server`).
* IRC_Bot has all the required features to be able to connect to a server, send 
  and receive messages and execute arbitrary commands that can be added at 
  runtime (`Bot_Command`).

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
