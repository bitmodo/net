#include "net/net.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>
#include <stdbool.h>

#include "handler.h"

extern NetHandler * gHandler;

// Fix free called from cleanup
void free(void *ptr) {
    cr_free(ptr);
}

Test(handler, setup) {
    gHandler = NULL;
    netSetup();
    cr_assert_not_null(gHandler, "Handler was null even after setup");
    netCleanup();
    cr_expect_null(gHandler, "Handler was not null after setup then cleanup");
}

bool ranCleanup = false;

void customCleanup() {
    ranCleanup = true;
}

ParameterizedTestParameters(handler, cleanup) {
    const size_t size = 3;
    HandlerParam *params = cr_malloc(sizeof(HandlerParam) * size);

    params[0] = toParam(NULL);
    params[1] = toParam(createEmptyHandler());

    NetHandler * param = createEmptyHandler();
    param->cleanup = &customCleanup;
    params[2] = toParam(param);

    return cr_make_param_array(HandlerParam, params, size, cleanupHandlerParam);
}

ParameterizedTest(HandlerParam * param, handler, cleanup) {
    if (param->handler && param->handler->cleanup) {
        param->handler->cleanup = &customCleanup;
    }

    gHandler = param->handler;
    ranCleanup = false;
    netCleanup();
    cr_assert_null(gHandler, "Handler was not null after cleanup");

    if (param->handler && param->handler->cleanup) {
        cr_assert(ranCleanup, "Custom cleanup function did not change global variable");
    }
}
