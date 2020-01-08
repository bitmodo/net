#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "util.h"

typedef struct Socket Socket;

// typedef struct TCPServer {

// } TCPServer;

// typedef struct UDPServer {

// } UDPServer;

NET_EXPORT Socket * net_tcpServerSocket();

NET_EXPORT Socket * net_udpServerSocket();

#endif
