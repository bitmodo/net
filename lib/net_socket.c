#include <stdlib.h>
#include <errno.h>
#include "net/net.h"

void NetSetAddressSocket(NetSocket sock, char const * address) {
    if (sock && sock->data && sock->setAddress) {
        sock->setAddress(sock->data, address);
    }
}

void NetSetPortSocket(NetSocket sock, unsigned port) {
    if (sock && sock->data && sock->setPort) {
        sock->setPort(sock->data, port);
    }
}

char const * NetGetAddressSocket(NetSocket sock) {
    if (sock && sock->data && sock->getAddress) {
        return sock->getAddress(sock->data);
    } else {
        errno = EFAULT;
        return NULL;
    }
}

unsigned NetGetPortSocket(NetSocket sock) {
    if (sock && sock->data && sock->getPort) {
        return sock->getPort(sock->data);
    } else {
        errno = EFAULT;
        return 0;
    }
}

int NetOpenSocket(NetSocket sock) {
    if (sock && sock->data && sock->open) {
        return sock->open(sock->data);
    } else {
        errno = EFAULT;
        return -1;
    }
}

bool NetListenSocket(NetSocket sock) {
    if (sock && sock->data && sock->listen) {
        return sock->listen(sock->data);
    } else {
        errno = EFAULT;
        return false;
    }
}

struct net_req_t const * NetGetRequestSocket(NetSocket sock) {
    if (sock && sock->data && sock->getRequest) {
        return sock->getRequest(sock->data);
    } else {
        errno = EFAULT;
        return NULL;
    }
}

struct net_res_t * NetGetResponseSocket(NetSocket sock) {
    if (sock && sock->data && sock->getResponse) {
        return sock->getResponse(sock->data);
    } else {
        errno = EFAULT;
        return NULL;
    }
}

void NetFinalizeResponseSocket(NetSocket sock) {
    if (sock && sock->data && sock->finalizeResponse) {
        sock->finalizeResponse(sock->data);
    }
}

void NetCloseSocket(NetSocket sock) {
    if (sock && sock->data && sock->close) {
        sock->close(sock->data);
    }
}

void NetDisposeSocket(NetSocket sock) {
    if (sock) {
        if (sock->data) {
            if (sock->disposeData) {
                sock->disposeData(sock->data);
            } else {
                free(sock->data);
            }
        }

        free(sock);
    }
}

void NetDisposeRequestSocket(NetSocket sock, struct net_req_t * req) {
    if (sock && sock->disposeRequest) {
        sock->disposeRequest(req);
    } else {
        if (req) {
            free(req);
        }
    }
}
