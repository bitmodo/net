#include "net/net.h"
#include "net/error.h"

#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>

extern NetHandler * gHandler;

typedef struct Params {
    NetHandler * handler;
    bool useMethod;
    int returnValue;
} Params;

void cleanupParams(struct criterion_test_params * ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        Params * tup = (Params *) ctp->params + i;

        if (tup->handler) {
            cr_free(tup->handler);
        }
    }

    cr_free(ctp->params);
}

int customFunction() {
    return ESUCCESS;
}

ParameterizedTestParameters(method, send) {
    const size_t size = 3;
    Params * params = cr_malloc(sizeof(Params) * size);

    params[0] = (Params) {.handler = NULL, .useMethod = false, .returnValue = ENULL_POINTER};
    params[1] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .useMethod = false, .returnValue = ENULL_POINTER};
    params[2] = (Params) {.handler = cr_malloc(sizeof(NetHandler)), .useMethod = true, .returnValue = ESUCCESS};

    return cr_make_param_array(Params, params, size, cleanupParams);
}

ParameterizedTest(Params * param, method, send) {
    cr_log_info("Running send test with {handler=%p, useMethod=%d, returnValue=%d}\n",
            param->handler, param->useMethod, param->returnValue);

    gHandler = param->handler;
    if (gHandler && param->useMethod) {
        gHandler->send = &customFunction;
    }

    cr_assert_eq(netSend(NULL, NULL, 0), param->returnValue, "Did not get the expected return value");
}
