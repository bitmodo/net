#include "net/net.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>
#include <stdbool.h>

#include "handler.h"

extern NetHandler * gHandler;

typedef struct SocketParams {
    NetHandler * handler;
    bool useInitialize;
    int side;
    int type;
    SocketData * rValue;
} SocketParams;

struct SocketData {
    int side;
    int type;
};

SocketData * customInitialize(int side, int type) {
    SocketData * data = malloc(sizeof(SocketData));
    *data = (SocketData) {.side = side, .type = type};
    return data;
}

SocketData * customSocketData(int side, int type) {
    SocketData * data = cr_malloc(sizeof(SocketData));
    *data = (SocketData) {.side = side, .type = type};
    return data;
}

void cleanupSocketParam(struct criterion_test_params *ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        SocketParams * tup = (SocketParams *) ctp->params + i;

        if (tup->handler) {
            cr_free(tup->handler);
        }

        if (tup->rValue) {
            cr_free(tup->rValue);
        }
    }

    cr_free(ctp->params);
}

ParameterizedTestParameters(socket, initialize) {
    const size_t size = 12;
    SocketParams * params = cr_malloc(sizeof(SocketParams) * size);

    int pos = 0;
    for (int side = CLIENT; side <= SERVER; side++) {
        for (int type = TCP; type <= UDP; type++) {
            params[pos++] = (SocketParams) {.handler = NULL, .useInitialize = false, .side = side, .type = type, .rValue = NULL};
            params[pos++] = (SocketParams) {.handler = createEmptyHandler(), .useInitialize = false, .side = side, .type = type, .rValue = NULL};
            params[pos++] = (SocketParams) {.handler = createEmptyHandler(), .useInitialize = true, .side = side, .type = type, .rValue = customSocketData(side, type)};
        }
    }

    return cr_make_param_array(SocketParams, params, size, cleanupSocketParam);
}

ParameterizedTest(SocketParams * param, socket, initialize) {
    cr_log_info("Address set test with {makeHandler: %p, useInitialize: %d, side: %d, type: %d, rValue: %p}\n",
        param->handler, param->useInitialize, param->side, param->type, param->rValue);

    gHandler = param->handler;
    if (param->useInitialize) {
        gHandler->initialize = &customInitialize;
    }

    Socket * sock = netSocket(param->side, param->type);
    cr_assert_eq(sock->side, param->side, "Socket side did not equal the expected side");
    cr_assert_eq(sock->type, param->type, "Socket type did not equal the expected type");
    cr_assert_null(sock->address, "Default socket did not return null address");
    cr_assert_eq(sock->addressType, UNSPEC, "Default socket did not return unspecified address type");
    cr_assert_eq(sock->port, DEFAULT_PORT, "Default socket did not return the default port");

    if (param->handler && param->useInitialize) {
        cr_assert_eq(sock->data->side, param->rValue->side, "The returned data side was not equal to the expected side");
        cr_assert_eq(sock->data->type, param->rValue->type, "The returned data type was not equal to the expected type");

        free(sock->data);
    } else {
        cr_assert_null(sock->data, "Default socket did not return null data");
    }

    free(sock);
}
