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

void net_cleanup() {
    NetHandler * handler = net_getHandler();

    if (handler) {
        if (handler->cleanup) {
            handler->cleanup();
        }

        free(handler);
    }
}

int net_connect(Socket * sock) {
    NetHandler * handler = net_getHandler();
    if (!handler || (handler && !(handler->connect))) {
        return ENULL_POINTER;
    }

    return handler->connect(sock);
}

int net_start(Socket * sock) {
    NetHandler * handler = net_getHandler();
    if (!handler || (handler && !(handler->start))) {
        return ENULL_POINTER;
    }

    return handler->start(sock);
}

int net_loop(Socket * sock) {
    NetHandler * handler = net_getHandler();
    if (!handler || (handler && !(handler->loop))) {
        return ENULL_POINTER;
    }

    return handler->loop(sock);
}

int net_close(Socket * sock) {
    NetHandler * handler = net_getHandler();
    if (!handler || (handler && !(handler->close))) {
        return ENULL_POINTER;
    }

    return handler->close(sock);
}
