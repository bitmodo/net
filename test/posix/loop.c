#include <net/net.h>
#include <net/error.h>

#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>

#include "method.h"

extern NetHandler * gHandler;

extern int loopPosix(Socket * sock);

int accept(int sockfd) {
    if (sockfd == 0) return -1;

    return sockfd;
}

ParameterizedTestParameters(posix, loop) {
    const size_t size = 7;
    Params * params = cr_malloc(sizeof(Params) * size);

    params[0] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .sock = NULL, .rv = ECLOSED};
    params[1] = toParam((Socket) {.data = NULL}, ECLOSED);
    params[2] = toParam((Socket) {.side = CLIENT, .data = createData(0, -1)}, EINCORRECT_SIDE);
    params[3] = toParam((Socket) {.side = SERVER, .data = createData(-1, -1)}, EINVALID_STATE);
    params[4] = toParam((Socket) {.side = SERVER, .data = createData(0, 0)}, EINVALID_STATE);
    params[5] = toParam((Socket) {.side = SERVER, .data = createData(0, -1)}, EUNKNOWN);
    params[6] = toParam((Socket) {.side = SERVER, .data = createData(1, -1)}, ESUCCESS);

    return cr_make_param_array(Params, params, size, cleanupParams);
}

ParameterizedTest(Params * param, posix, loop) {
    cr_assert_not_null(param->handler, "Handler parameter was null");

    if (param->sock) {
        cr_log_info("Posix loop test with {side=%d, type=%d, address=%s, addressType=%d, port=%u, data=%p}\n",
            param->sock->side, param->sock->type, param->sock->address, param->sock->addressType, param->sock->port, param->sock->data);
    } else {
        cr_log_info("Posix loop test will null socket\n");
    }

    gHandler = param->handler;
    gHandler->loop = &loopPosix;

    int rv = netLoop(param->sock);
    cr_expect_eq(rv, param->rv, "Did not receive the return value that was expected");

    if (rv == ESUCCESS) {
        cr_expect_eq(param->sock->data->conn, param->sock->data->fd, "The connection value was not set to the expected value");
    } else if (rv == EUNKNOWN) {
        cr_expect_eq(param->sock->data->conn, -1, "The connection value did not stay -1");
    }
}
