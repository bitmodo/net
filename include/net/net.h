#ifndef NET_NET_H
#define NET_NET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"

// Naming conventions:
//
// Struct: TitleCase
// Struct members: cammelCase
// Enum: ETitleCase
// Enum members: cammelCase or pALL_CAPS where p = one letter prefix
// Public functions: net_cammelCase
// Private functions: _TitleCase
// Trivial variables: i, x, n, f, etc...
// Local variables: cammelCase
// Global variables: g_cammelCase

/** The side of a connection */
enum ENetSide {
    CLIENT, /* The client side of a connection */
    SERVER /* The server side of a connection */
};

/** The type of connection to make */
enum ENetType {
    TCP, /* A Transmission Control Protocol socket */
    UDP /* A User Datagram Protocol socket */
};

/** The type of the specified address address */
enum EAddressType {
    IPv4, /* An IPv4 address */
    IPv6 /* An IPv6 address */
};

/** Implementation specific data to associate with a socket */
typedef struct SocketData SocketData;

/** A generic socket */
typedef struct Socket {
    int side; /* The side of the socket. Corresponds to ENetSide */
    int type; /* The type of the socket. Corresponds to ENetType */
    const char * address; /* The address to connect to or listen on */
    int addressType; /* The type of the address */
    unsigned port; /* The port to connect to or listen on */
    SocketData * data; /* Implementation specific data associated with this socket */
} Socket;

/** A collection of functions to allow implementations dynamically change how different tasks are handled */
typedef struct NetHandler {
    SocketData * (* initialize)(int side, int type); /* Initialize socket data for when creating a socket */
    int (* connect)(Socket *); /* Client. Connect to the specified server and prepare the environment to use the connection */
    int (* start)(Socket *); /* Server. Initialize a socket and prepare it for incoming connections */
    int (* loop)(Socket *); /* Server. Accept a connection and prepare the environment to use the connection */
    int (* receive)(Socket *, void *, int); /* Receive data from the currently opened connection. Works on client and server */
    int (* send)(Socket *, const void *, int); /* Send data on the currently opened connection. Works on client and server */
    int (* closeConnection)(Socket *); /* Server. Close the currently opened connection and free memory relating to it */
    int (* close)(Socket **); /* Close and dispose of a socket. This should free the socket and any related data */
    void (* cleanup)(); /* Clean up the environment and free any related memory */
} NetHandler;

/** Setup the environment for networking. This basically just sets the environment to whatever
    setupPlatform spits out, so it you wanted to use your own handler, you could simply omit this
    call and set handler to your implementation. */
NET_EXPORT void net_setup();

/** An implementation specific setup function to provide a handler for said implementation */
NET_EXPORT NetHandler * net_setupPlatform();

/** Get the current handler */
NET_EXPORT NetHandler * net_getHandler();

/** Set the current handler */
NET_EXPORT void net_setHandler(NetHandler *);

/** Create a socket using the specified side and type. Side refers to ENetSide and type refers to ENetType */
NET_EXPORT Socket * net_socket(int side, int type);

/** Cleanup the environment of networking related things. This should be the last call when using networking.
    After this is called, it would need to be setup again before networking functions can be used. */
NET_EXPORT void net_cleanup();

// Functions to manipulate the ip and port of the socket

/** Set a socket's address */
NET_EXPORT void net_setAddress(Socket *, const char *);

/** Get a socket's address */
NET_EXPORT const char * net_getAddress(Socket *);

/** Set the socket's address' type */
NET_EXPORT void net_setAddressType(Socket *, int);

/** Get the socket's address' type */
NET_EXPORT int net_getAddressType(Socket *);

/** Set a socket's port */
NET_EXPORT void net_setPort(Socket *, unsigned);

/** Get a socket's port */
NET_EXPORT unsigned net_getPort(Socket *);

// Functions relating to the manipulation of a socket

/** Client side only. This will connect the specified socket to whatever address and port
    were previously assigned. Once connected, data can be sent and received on this socket. */
NET_EXPORT int net_connect(Socket *);

/** Server side only. Starts the socket on the previously assigned address and port. Once
    started, connections can be listened for */
NET_EXPORT int net_start(Socket *);

/** Server side only. Listen for a connection on the socket. When one is made, return ESUCCESS
    and prepare the socket for further access to the connection. */
NET_EXPORT int net_loop(Socket *);

/** Recieve data from the socket. Put the data recieved in the specified port up to the specified
    count. The number of bytes actually read will be returned and 0 if the connection closed. */
NET_EXPORT int net_receive(Socket *, void *, int);

/** Send data on the socket. Use the data in the specified buffer up to the specified count. */
NET_EXPORT int net_send(Socket *, const void *, int);

/** Server side only. Close the currently open connection. */
NET_EXPORT int net_closeConnection(Socket *);

/** Close the socket. This should be a reference to the socket pointer, and will free and null the pointer
    so that it cannot be used later. */
NET_EXPORT int net_close(Socket **);

#ifdef __cplusplus
}
#endif

#endif
