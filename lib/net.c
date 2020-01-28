#include "net/net.h"
#include "net/error.h"

#include <stdlib.h>

NetHandler * gHandler;

void netSetup() {
    netSetHandler(netSetupPlatform());
}

NetHandler * netGetHandler() {
    return gHandler;
}

void netSetHandler(NetHandler * handler) {
    gHandler = handler;
}

Socket * netSocket(int side, int type) {
    SocketData * data = NULL;
    NetHandler * handler = netGetHandler();
    if (handler && handler->initialize) {
        data = handler->initialize(side, type);
    }

    Socket * result = malloc(sizeof(Socket));
    *result = (Socket) {side, type, NULL, UNSPEC, DEFAULT_PORT, data};

    return result;
}

void netCleanup() {
    NetHandler * handler = netGetHandler();

    if (handler) {
        if (handler->cleanup) {
            handler->cleanup();
        }

        free(handler);
        netSetHandler(NULL);
    }
}

void netSetAddress(Socket * sock, const char * addr) {
    if (sock) {
        sock->address = addr;
    }
}

const char * netGetAddress(Socket * sock) {
    if (sock) {
        return sock->address;
    }

    return NULL;
}

void netSetAddressType(Socket * sock, int type) {
    if (sock) {
        sock->addressType = type;
    }
}

int netGetAddressType(Socket * sock) {
    if (sock) {
        return sock->addressType;
    }

    return UNSPEC;
}

void netSetPort(Socket * sock, unsigned port) {
    if (sock) {
        sock->port = port;
    }
}

unsigned netGetPort(Socket * sock) {
    if (sock) {
        return sock->port;
    }

    return DEFAULT_PORT;
}

int netConnect(Socket * sock) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->connect)) {
        return ENULL_POINTER;
    }

    return handler->connect(sock);
}

int netStart(Socket * sock) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->start)) {
        return ENULL_POINTER;
    }

    return handler->start(sock);
}

int netLoop(Socket * sock) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->loop)) {
        return ENULL_POINTER;
    }

    return handler->loop(sock);
}

int netReceive(Socket * sock, void * buf, size_t count, ssize_t * size) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->receive)) {
        return ENULL_POINTER;
    }

    return handler->receive(sock, buf, count, size);
}

int netSend(Socket * sock, const void * buf, size_t count) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->send)) {
        return ENULL_POINTER;
    }

    return handler->send(sock, buf, count);
}

int netCloseConnection(Socket * sock) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->closeConnection)) {
        return ENULL_POINTER;
    }

    return handler->closeConnection(sock);
}

int netClose(Socket ** sock) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->close)) {
        return ENULL_POINTER;
    }

    int ec;
    if ((ec = handler->close(sock)) != ESUCCESS) return ec;

    if (*sock) {
        free(*sock);
        *sock = NULL;
    }

    return ESUCCESS;
}
