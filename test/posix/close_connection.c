#include <net/net.h>
#include <net/error.h>

#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>

#include "method.h"

extern NetHandler * gHandler;

extern int closeConnectionPosix(Socket * sock);

int close() {
    return 0;
}

ParameterizedTestParameters(posix, close_connection) {
    const size_t size = 6;
    Params * params = cr_malloc(sizeof(Params) * size);

    params[0] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .sock = NULL, .rv = ENULL_POINTER};
    params[1] = toParam((Socket) {.data = NULL}, ENULL_POINTER);
    params[2] = toParam((Socket) {.side = CLIENT, .data = createData(-1, -1)}, EINCORRECT_SIDE);
    params[3] = toParam((Socket) {.side = SERVER, .data = createData(-1, -1)}, EINVALID_STATE);
    params[4] = toParam((Socket) {.side = SERVER, .data = createData(-1, 0)}, ESUCCESS);
    params[5] = toParam((Socket) {.side = SERVER, .data = createData(-1, 1)}, ESUCCESS);

    return cr_make_param_array(Params, params, size, cleanupParams);
}

ParameterizedTest(Params * param, posix, close_connection) {
    cr_assert_not_null(param->handler, "Handler parameter was null");

    if (param->sock) {
        cr_log_info("Posix close connection test with {side=%d, type=%d, address=%s, addressType=%d, port=%u, data=%p}\n",
            param->sock->side, param->sock->type, param->sock->address, param->sock->addressType, param->sock->port, param->sock->data);
    } else {
        cr_log_info("Posix close connection test will null socket\n");
    }

    gHandler = param->handler;
    gHandler->closeConnection = &closeConnectionPosix;

    int rv = netCloseConnection(param->sock);
    cr_expect_eq(rv, param->rv, "Did not receive the expected return value");

    if (rv == ESUCCESS) {
        cr_expect_eq(param->sock->data->conn, -1, "Connection descriptor was not set to -1");
    }
}
