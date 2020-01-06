/**
 * @file connection.h
 * @author Mateusz Makowski
 * @date 15.09.2018
 * @brief File containing the IRC_Connection struct and related functions.
 */
#ifndef IRCBOT_CONNECTION_CONNECTION_H_
#define IRCBOT_CONNECTION_CONNECTION_H_

#define MAX_ADDR_LEN 32
#define MAX_PORT_LEN 8
#define MAX_MSG_LEN 512

#ifdef _WIN32
typedef unsigned int UINT_PTR;
typedef UINT_PTR SOCKET;
#else
typedef int SOCKET;
#define INVALID_SOCKET ((SOCKET)(~0))
#define SOCKET_ERROR (-1)
#endif
/**
 * IRC_Connection struct holds the socket handle, receiving buffer and
 * basic info about the connection.
 * @see connection_create()
 */
typedef struct IRC_Connection {
    /** Socket handle */
    SOCKET socket;
    /**
     * Addrinfo result pointer.
     * See
     * [getaddrinfo()](http://man7.org/linux/man-pages/man3/getaddrinfo.3.html)
     * for more information.
     */
    struct addrinfo *ai_result;
    /** @see ai_result() */
    struct addrinfo *ai_hints;
    /**
     * Address of the server.
     * E.g. "irc.freenode.net".
     */
    char address[MAX_ADDR_LEN];
    /**
     * Port to be used.
     * Note that this is a string, not an integer.
     */
    char port[MAX_PORT_LEN];
    /** Buffer used by `recv()` to store incoming data. */
    char *recvbuf;
    /** Length of `recvbuf`, by default `= 512`. */
    int recvbuflen;
} IRC_Connection;

/**
 * "Constructor" for IRC_Connection. The preferred way to create new
 * connections.
 * @param address a string with the server's URI
 * @param port a string with the port to be used
 * @return pointer to newly created IRC_Connection or null pointer on failure
 */
IRC_Connection *connection_create(const char address[], const char port[]);

/**
 * Actually connect to the remote server specified in `connection_create()`.
 * @param connection a pointer to IRC_Connection object
 * @return 1 on success, 0 on failure
 */
int connection_connect(IRC_Connection *connection);
/**
 * Send data (char) to remote server.
 * @param connection a pointer to IRC_Connection object
 * @param msg data to be sent
 * @return 1 on success, 0 on failure
 */
int connection_send(IRC_Connection *connection, const char msg[]);
/**
 * Receive data from remote server.
 * @param connection a pointer to IRC_Connection object
 * @return 1 on success, 0 on failure
 */
int connection_read(IRC_Connection *connection);
/**
 * Disconnect from remote server and close the socket.
 * @param connection a pointer to IRC_Connection object
 * @return 1 on success, 0 on failure
 */
int connection_disconnect(IRC_Connection *connection);

#endif // IRCBOT_CONNECTION_CONNECTION_H_
