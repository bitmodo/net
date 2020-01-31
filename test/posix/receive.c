#include <net/net.h>
#include <net/error.h>

#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>

#include "method.h"

extern NetHandler * gHandler;

extern int receivePosix(Socket * sock, void * buf, size_t count, ssize_t * size);

ssize_t recv(int sockfd) {
    if (sockfd == 0) return -1;

    return sockfd;
}

ParameterizedTestParameters(posix, receive) {
    const size_t size = 8;
    Params * params = cr_malloc(sizeof(Params) * size);

    params[0] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .sock = NULL, .rv = ENULL_POINTER};
    params[1] = toParam((Socket) {.data = NULL}, ENULL_POINTER);
    params[2] = toParam((Socket) {.side = CLIENT, .data = createData(-1, -1)}, EINVALID_STATE);
    params[3] = toParam((Socket) {.side = CLIENT, .data = createData(0, -1)}, EUNKNOWN);
    params[4] = toParam((Socket) {.side = CLIENT, .data = createData(1, -1)}, ESUCCESS);
    params[5] = toParam((Socket) {.side = SERVER, .data = createData(-1, -1)}, EINVALID_STATE);
    params[6] = toParam((Socket) {.side = SERVER, .data = createData(-1, 0)}, EUNKNOWN);
    params[7] = toParam((Socket) {.side = SERVER, .data = createData(-1, 1)}, ESUCCESS);

    return cr_make_param_array(Params, params, size, cleanupParams);
}

ParameterizedTest(Params * param, posix, receive) {
    cr_assert_not_null(param->handler, "Handler parameter was null");

    if (param->sock) {
        cr_log_info("Posix receive test with {side=%d, type=%d, address=%s, addressType=%d, port=%u, data=%p}\n",
            param->sock->side, param->sock->type, param->sock->address, param->sock->addressType, param->sock->port, param->sock->data);
    } else {
        cr_log_info("Posix receive test with null socket\n");
    }

    gHandler = param->handler;
    gHandler->receive = &receivePosix;

    ssize_t size = -1;
    int rv = netReceive(param->sock, NULL, -1, &size);
    cr_expect_eq(rv, param->rv, "Did not receive the correct return value");

    if (rv == ESUCCESS) {
        cr_expect_eq(size, param->sock->side == SERVER ? param->sock->data->conn : param->sock->data->fd, "Size was not set to the expected value");
    } else if (rv == EUNKNOWN) {
        cr_expect_eq(size, -1, "Size was not set to -1");
    }
}
