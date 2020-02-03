#include <net/net.h>
#include <net/error.h>

#include <criterion/parameterized.h>
#include <criterion/logging.h>

#include <stdlib.h>

#include "method.h"

extern NetHandler * gHandler;

extern int sendPosix(Socket *, const void *, size_t);

ssize_t send(int fd, const void * buf, size_t n, int flags) {
    if (n == 1) return -1;

    if (n == 4) cr_assert_eq(buf, (void *) 0, "Buf was not at the correct address");
    if (n == 2) cr_assert_eq(buf, (void *) 2, "Buf was not at the correct address");
    
    return 2;
}

ParameterizedTestParameters(posix, send) {
    const size_t size = 6;
    Params * params = cr_malloc(sizeof(Params) * size);

    params[0] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .sock = NULL, .rv = ENULL_POINTER};
    params[1] = toParam((Socket) {.data = NULL}, ENULL_POINTER);
    params[2] = toParam((Socket) {.side = SERVER, .data = createData(-1, -1)}, EINVALID_STATE);
    params[3] = toParam((Socket) {.side = CLIENT, .data = createData(-1, -1)}, EINVALID_STATE);
    params[4] = toParam((Socket) {.data = createData(1, 0)}, EUNKNOWN);
    params[5] = toParam((Socket) {.data = createData(4, 0)}, ESUCCESS);

    return cr_make_param_array(Params, params, size, cleanupParams);
}

ParameterizedTest(Params * param, posix, send) {
    cr_assert_not_null(param->handler, "Handler parameter was null");

    if (param->sock) {
        cr_log_info("Posix send test with {side=%d, type=%d, address=%s, addressType=%d, port=%u, data=%p}\n",
            param->sock->side, param->sock->type, param->sock->address, param->sock->addressType, param->sock->port, param->sock->data);
    } else {
        cr_log_info("Posix send test will null socket\n");
    }

    gHandler = param->handler;
    gHandler->send = &sendPosix;

    int rv = netSend(param->sock, (void *) 0, (param->sock && param->sock->data) ? param->sock->data->fd : 0);
    cr_expect_eq(rv, param->rv, "Did not receive expected return value. Expected %d, got %d", param->rv, rv);
}
