#include "net/net.h"
#include "net/error.h"

#include <stdlib.h>

NetHandler * g_handler;

void netSetup() {
    netSetHandler(netSetupPlatform());
}

NetHandler * netGetHandler() {
    return g_handler;
}

void netSetHandler(NetHandler * handler) {
    g_handler = handler;
}

Socket * netSocket(int side, int type) {
    SocketData * data = NULL;
    NetHandler * handler = netGetHandler();
    if (handler && handler->initialize) {
        data = handler->initialize(side, type);
    }

    Socket * result = malloc(sizeof(Socket));
    *result = (Socket) {side, type, NULL, UNSPEC, 0, data};

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
    } else {
        return NULL;
    }
}

void netSetAddressType(Socket * sock, int type) {
    if (sock) {
        sock->addressType = type;
    }
}

int netGetAddressType(Socket * sock) {
    if (sock) {
        return sock->addressType;
    } else {
        return -1;
    }
}

void netSetPort(Socket * sock, unsigned port) {
    if (sock) {
        sock->port = port;
    }
}

unsigned netGetPort(Socket * sock) {
    if (sock) {
        return sock->port;
    } else {
        return 0;
    }
}

#define NET_FUNCTION(capname, name)                   \
int net ## capname(Socket * sock) {            \
    NetHandler * handler = netGetHandler(); \
    if (!handler || !(handler->name)) {      \
        return ENULL_POINTER;                \
    }                                        \
                                             \
    return handler->name(sock);              \
}

NET_FUNCTION(Connect, connect)
NET_FUNCTION(Start, start)
NET_FUNCTION(Loop, loop)
NET_FUNCTION(CloseConnection, closeConnection)

int netClose(Socket ** sock) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->close)) {
        return ENULL_POINTER;
    }

    return handler->close(sock);
}

int netReceive(Socket * sock, void * buf, int count) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->receive)) {
        return ENULL_POINTER;
    }

    return handler->receive(sock, buf, count);
}

char * netReceiveText(Socket * sock) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->receiveText)) {
        return NULL;
    }

    return handler->receiveText(sock);
}

int netSend(Socket * sock, const void * buf, int count) {
    NetHandler * handler = netGetHandler();
    if (!handler || !(handler->send)) {
        return ENULL_POINTER;
    }

    return handler->send(sock, buf, count);
}
