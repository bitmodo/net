#include "net/net.h"
#include "net/error.h"

#include <stdlib.h>

NetHandler * g_handler;

void net_setup() {
    net_setHandler(net_setupPlatform());
}

NetHandler * net_getHandler() {
    return g_handler;
}

void net_setHandler(NetHandler * handler) {
    g_handler = handler;
}

Socket * net_socket(int side, int type) {
    SocketData * data = NULL;
    NetHandler * handler = net_getHandler();
    if (handler && handler->initialize) {
        data = handler->initialize(side, type);
    }

    Socket * result = malloc(sizeof(Socket));
    *result = (Socket) {side, type, NULL, UNSPEC, 0, data};

    return result;
}

void net_cleanup() {
    NetHandler * handler = net_getHandler();

    if (handler) {
        if (handler->cleanup) {
            handler->cleanup();
        }

        free(handler);
        net_setHandler(NULL);
    }
}

void net_setAddress(Socket * sock, const char * addr) {
    if (sock) {
        sock->address = addr;
    }
}

const char * net_getAddress(Socket * sock) {
    if (sock) {
        return sock->address;
    } else {
        return NULL;
    }
}

void net_setAddressType(Socket * sock, int type) {
    if (sock) {
        sock->addressType = type;
    }
}

int net_getAddressType(Socket * sock) {
    if (sock) {
        return sock->addressType;
    } else {
        return -1;
    }
}

void net_setPort(Socket * sock, unsigned port) {
    if (sock) {
        sock->port = port;
    }
}

unsigned net_getPort(Socket * sock) {
    if (sock) {
        return sock->port;
    } else {
        return 0;
    }
}

#define NET_FUNCTION(name)                   \
int net_ ## name(Socket * sock) {            \
    NetHandler * handler = net_getHandler(); \
    if (!handler || !(handler->name)) {      \
        return ENULL_POINTER;                \
    }                                        \
                                             \
    return handler->name(sock);              \
}

NET_FUNCTION(connect)
NET_FUNCTION(start)
NET_FUNCTION(loop)
NET_FUNCTION(closeConnection)

int net_close(Socket ** sock) {
    NetHandler * handler = net_getHandler();
    if (!handler || !(handler->close)) {
        return ENULL_POINTER;
    }

    return handler->close(sock);
}

int net_receive(Socket * sock, void * buf, int count) {
    NetHandler * handler = net_getHandler();
    if (!handler || !(handler->receive)) {
        return ENULL_POINTER;
    }

    return handler->receive(sock, buf, count);
}

char * net_receiveText(Socket * sock, int size) {
    NetHandler * handler = net_getHandler();
    if (!handler || !(handler->receiveText)) {
        return NULL;
    }

    return handler->receiveText(sock, size);
}

int net_send(Socket * sock, const void * buf, int count) {
    NetHandler * handler = net_getHandler();
    if (!handler || !(handler->send)) {
        return ENULL_POINTER;
    }

    return handler->send(sock, buf, count);
}

int net_sendText(Socket * sock, const char * text) {
    NetHandler * handler = net_getHandler();
    if (!handler || !(handler->sendText)) {
        return ENULL_POINTER;
    }

    return handler->sendText(sock, text);
}
