#include "net/net.h"
#include "net/error.h"

#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

// Fix free called in close
void free(void * ptr) {
    cr_free(ptr);
}

typedef struct Params {
    NetHandler * handler;
    bool useMethod;
    Socket * sock;
    int rv;
} Params;

void cleanupParams(struct criterion_test_params * ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        Params * tup = (Params *) ctp->params + i;

        if (tup->handler) {
            cr_free(tup->handler);
        }

        if (tup->sock) {
            cr_free(tup->sock);
        }
    }

    cr_free(ctp->params);
}

int customFunction(Socket ** sock) {
    if (*sock) {
        int side = (*sock)->side;
        if (side) {
            cr_free(*sock);
            *sock = NULL;
        }

        return side;
    }

    return ESUCCESS;
}

ParameterizedTestParameters(method, close) {
    const size_t size = 5;
    Params * params = cr_malloc(sizeof(Params) * size);

    params[0] = (Params) {.handler = NULL, .useMethod = false, .sock = NULL, .rv = ENULL_POINTER};
    params[1] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .useMethod = false, .sock = NULL, .rv = ENULL_POINTER};
    params[2] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .useMethod = true, .sock = NULL, .rv = ESUCCESS};

    params[3] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .useMethod = true, .sock = cr_malloc(sizeof(Socket)), .rv = ESUCCESS};
    params[3].sock->side = ESUCCESS;

    params[4] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .useMethod = true, .sock = cr_malloc(sizeof(Socket)), .rv = EUNKNOWN};
    params[4].sock->side = EUNKNOWN;

    return cr_make_param_array(Params, params, size, cleanupParams);
}

ParameterizedTest(Params * param, method, close) {
    cr_log_info("Close test with {handler=%p, useMethod=%d, sock=%p, rv=%d}\n",
            param->handler, param->useMethod, param->sock, param->rv);

    gHandler = param->handler;
    if (gHandler && param->useMethod) {
        gHandler->close = &customFunction;
    }

    cr_assert_eq(netClose(&(param->sock)), param->rv, "Return value was not what was expected");
    cr_assert_null(param->sock, "Socket was not nulled after closing");
}
