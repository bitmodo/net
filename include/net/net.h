#ifndef NET_NET_H
#define NET_NET_H

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

/** Implementation specific data to associate with a socket */
typedef struct SocketData SocketData;

/** A generic socket */
typedef struct Socket {
    int side; /* The side of the socket. Corresponds to ENetSide */
    int type; /* The type of the socket. Corresponds to ENetType */
    SocketData * data; /* Implementation specific data associated with this socket */
} Socket;

/** A collection of functions to allow implementations dynamically change how different tasks are handled */
typedef struct NetHandler {
    SocketData * (* initialize)(int side, int type); /* Initialize socket data for when creating a socket */
    int (* connect)(Socket *); /* Client. Connect to the specified server and prepare the environment to use the connection */
    int (* start)(Socket *); /* Server. Initialize a socket and prepare it for incoming connections */
    int (* loop)(Socket *); /* Accept a connection and prepare the environment to use the connection */
    int (* close)(Socket *); /* Close and dispose of a socket. This should free the socket and any related data */
    void (* cleanup)(); /* Clean up the environment and free any related memory */
} NetHandler;

/**  */
NET_EXPORT void net_setup();

/** An implementation specific setup function to provide a handler for said implementation */
NET_EXPORT NetHandler * net_setupPlatform();

/**  */
NET_EXPORT NetHandler * net_getHandler();

/**  */
NET_EXPORT void net_setHandler(NetHandler *);

/**  */
NET_EXPORT void net_cleanup();

/**  */
NET_EXPORT int net_connect(Socket *);

/**  */
NET_EXPORT int net_start(Socket *);

/**  */
NET_EXPORT int net_loop(Socket *);

/**  */
NET_EXPORT int net_close(Socket *);

#endif
