#ifndef NET_POSIX_METHOD_H
#define NET_POSIX_METHOD_H

#include <net/net.h>

#include <criterion/parameterized.h>

struct SocketData {
    int fd;
    int conn;
};

typedef struct Params {
    NetHandler * handler;
    Socket * sock;
    int rv;
} Params;

Params toParam(Socket socket, int rv) {
    NetHandler * handler = cr_malloc(sizeof(NetHandler));

    Socket * sock = cr_malloc(sizeof(Socket));
    *sock = socket;

    return (Params) {.handler = handler, .sock = sock, .rv = rv};
}

SocketData * createData(int fd, int conn) {
    SocketData * data = cr_malloc(sizeof(SocketData));
    *data = (SocketData) {.fd = fd, .conn = conn};

    return data;
}

void cleanupParams(struct criterion_test_params * ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        Params * tup = (Params *) ctp->params + i;

        if (tup->handler) {
            cr_free(tup->handler);
        }

        if (tup->sock) {
            if (tup->sock->data) {
                cr_free(tup->sock->data);
            }

            cr_free(tup->sock);
        }
    }

    cr_free(ctp->params);
}

#endif
