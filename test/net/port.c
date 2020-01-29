#include "net/net.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>

#include "socket.h"

typedef struct PortParams {
    Socket * sock;
    int port;
} PortParams;

void cleanupPortParam(struct criterion_test_params *ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        PortParams * tup = (PortParams *) ctp->params + i;

        if (tup->sock) {
            cr_free(tup->sock);
        }
    }

    cr_free(ctp->params);
}

#define PORT_PARAMS                                                                                         \
    const size_t size = 6;                                                                                  \
    PortParams * params = cr_malloc(sizeof(PortParams) * size);                                             \
                                                                                                            \
    params[0] = (PortParams) {.sock = NULL, .port = 0};                                                     \
    params[1] = (PortParams) {.sock = NULL, .port = DEFAULT_PORT};                                          \
    params[2] = (PortParams) {.sock = NULL, .port = 80};                                                    \
    params[3] = (PortParams) {.sock = createSocket(), .port = 0};                                           \
    params[4] = (PortParams) {.sock = createSocket(), .port = DEFAULT_PORT};                                \
    params[5] = (PortParams) {.sock = createSocket(), .port = 80};                                          \
                                                                                                            \
    return cr_make_param_array(PortParams, params, size, cleanupPortParam)

ParameterizedTestParameters(port, set) {
    PORT_PARAMS;
}

ParameterizedTest(PortParams * param, port, set) {
    cr_log_info("Port set test with {sock: %p, port: %d}\n", param->sock, param->port);
    netSetPort(param->sock, param->port);

    if (param->sock) {
        cr_assert_eq(param->sock->port, param->port, "Set port did not set the correct port");
    }
}

ParameterizedTestParameters(port, get) {
    PORT_PARAMS;
}

ParameterizedTest(PortParams * param, port, get) {
    cr_log_info("Port get test with {sock: %p, port: %d}\n", param->sock, param->port);

    if (param->sock) {
        param->sock->port = param->port;
        cr_assert_eq(netGetPort(param->sock), param->port, "Returned port is not equal to the expected port");
    } else {
        cr_assert_eq(netGetPort(param->sock), DEFAULT_PORT, "Null socket did not return the default port");
    }
}
