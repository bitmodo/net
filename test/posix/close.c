#include "criterion/alloc.h"
#include <net/net.h>
#include <net/error.h>

#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>

extern NetHandler * gHandler;

extern int closePosix(Socket ** sock);

int * currentFd;

void free(void * ptr) {
    cr_free(ptr);
}

int close(int fd) {
    if (currentFd) *currentFd = fd;

    return 0;
}

struct SocketData {
    int fd;
    int conn;
};

typedef struct Params {
    NetHandler * handler;
    bool nullSock;
    Socket * sock;
    int rv;
    int * fd;
} Params;

Params toParam(Socket socket, int rv) {
    NetHandler * handler = cr_malloc(sizeof(NetHandler));
    int * fd = cr_malloc(sizeof(int));

    Socket * sock = cr_malloc(sizeof(Socket));
    *sock = socket;

    return (Params) {.handler = handler, .nullSock = false, .sock = sock, .rv = rv, .fd = fd};
}

SocketData * createData(int fd, int conn) {
    SocketData * data = cr_malloc(sizeof(SocketData));
    *data = (SocketData) {.fd = fd, .conn = conn};

    return data;
}

void cleanupCloseParams(struct criterion_test_params * ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        Params * tup = (Params *) ctp->params + i;

        if (tup->handler) {
            cr_free(tup->handler);
        }

        if (tup->fd) {
            cr_free(tup->fd);
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

ParameterizedTestParameters(posix, close_connection) {
    const size_t size = 6;
    Params * params = cr_malloc(sizeof(Params) * size);

    params[0] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .nullSock = true, .sock = NULL, .rv = ENULL_POINTER, .fd = cr_malloc(sizeof(int))};
    params[1] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .nullSock = false, .sock = NULL, .rv = ENULL_POINTER, .fd = cr_malloc(sizeof(int))};
    params[2] = toParam((Socket) {.data = NULL}, ENULL_POINTER);
    params[3] = toParam((Socket) {.data = createData(-1, -1)}, ESUCCESS);
    params[4] = toParam((Socket) {.data = createData(0, -1)}, ESUCCESS);
    params[5] = toParam((Socket) {.data = createData(1, -1)}, ESUCCESS);

    return cr_make_param_array(Params, params, size, cleanupCloseParams);
}

ParameterizedTest(Params * param, posix, close_connection) {
    cr_assert_not_null(param->handler, "Handler parameter was null");

    if (param->sock) {
        cr_log_info("Posix close test with {side=%d, type=%d, address=%s, addressType=%d, port=%u, data=%p}\n",
            param->sock->side, param->sock->type, param->sock->address, param->sock->addressType, param->sock->port, param->sock->data);
    } else {
        cr_log_info("Posix close test will null socket\n");
    }

    bool hasSocket = param->sock != NULL;
    bool hasData = param->sock != NULL && param->sock->data != NULL;

    int fd = -1;
    if (param->sock && param->sock->data) {
        fd = param->sock->data->fd;
    }

    gHandler = param->handler;
    gHandler->close = &closePosix;

    currentFd = param->fd;
    *currentFd = -1;

    Socket ** sock = NULL;
    if (!param->nullSock) {
        sock = &(param->sock);
    }

    int rv = netClose(sock);
    cr_expect_eq(rv, param->rv, "Did not receive the expected return value");

    if (rv == ESUCCESS) {
        cr_expect_null(param->sock, "Socket was not nulled afterwards");
        cr_expect_eq(*currentFd, fd, "Close did not get called with the correct value");
    } else if (!param->nullSock && hasSocket && hasData) {
        cr_assert_not_null(param->sock, "Socket was nulled even after an error");
        cr_expect_not_null(param->sock->data, "Socket data was nulled even after an error");
    }

    currentFd = NULL;
}
