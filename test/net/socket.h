#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "net/net.h"

#include <criterion/alloc.h>

#define emptySocket ((Socket) {0, 0, NULL, 0, 0, NULL})

Socket * fillEmptySocket(Socket * sock) {
    *sock = emptySocket;
    return sock;
}

Socket * createSocket() {
    return cr_malloc(sizeof(Socket));
}

Socket * createEmptySocket() {
    return fillEmptySocket(createSocket());
}

#endif
