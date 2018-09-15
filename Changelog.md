# Changelog

## 0.3.0
### WIP
* `bot` fully working, hides the `connection` socket API and exposes following functions:
    * `create`
    * `connect`
    * `send`
    * `disconnect`
    * `join`
    * `leave`
    * `pong`
    * `whois`
    * `add_command`
    * `execute_command`
* Unit tests for every function mentioned above
* Removed `pointer == NULL` checks in favor of `!pointer` checks
* Comments and documentation for the API

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
