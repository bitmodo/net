#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include "util.h"

typedef struct Socket Socket;

// typedef struct TCPClient {

// } TCPClient;

// typedef struct UDPClient {

// } UDPClient;

NET_EXPORT Socket * net_tcpClientSocket();

NET_EXPORT Socket * net_udpClientSocket();

#endif
