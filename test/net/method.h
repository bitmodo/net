#ifndef NET_METHOD_H
#define NET_METHOD_H

#include "net/error.h"
#include "net/net.h"

#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdbool.h>
#include <stdlib.h>

extern NetHandler * gHandler;

typedef struct Params {
  NetHandler * handler;
  bool useMethod;
  Socket * socket;
} Params;

Params createParams(bool useHandler, bool useMethod, bool useSocket) {
  NetHandler * handler = NULL;
  Socket * sock = NULL;

  if (useHandler) {
    handler = cr_malloc(sizeof(NetHandler));
    *handler = (NetHandler){NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
  }

  if (useSocket) {
    sock = cr_malloc(sizeof(Socket));
    *sock = (Socket){0, 0, NULL, 0, 0, NULL};
  }

  return (Params){.handler = handler, .useMethod = useMethod, .socket = sock};
}

void cleanupAddressParam(struct criterion_test_params *ctp) {
  for (size_t i = 0; i < ctp->length; ++i) {
    Params * tup = (Params *) ctp->params + i;

    if (tup->handler) {
      cr_free(tup->handler);
    }

    if (tup->socket) {
      cr_free(tup->socket);
    }
  }

  cr_free(ctp->params);
}

int customMethod() { return ESUCCESS; }

#define METHOD(name, signature)                                                    \
    ParameterizedTestParameters(method, name) {                                    \
        const size_t size = 8;                                                     \
        Params * params = cr_malloc(sizeof(Params) * size);                        \
                                                                                   \
        for (unsigned i = 0; i < size; i++) {                                      \
            params[i] = createParams(i & (1 << 0), i & (1 << 1), i & (1 << 2));    \
        }                                                                          \
                                                                                   \
        return cr_make_param_array(Params, params, size, cleanupAddressParam);     \
    }                                                                              \
                                                                                   \
    ParameterizedTest(Params *param, method, name) {                               \
        cr_log_info("Running " #name                                               \
                    " test with {handler=%p, method=%d, socket=%p}\n",             \
                    param->handler, param->useMethod, param->socket);              \
                                                                                   \
        gHandler = param->handler;                                                 \
        if (gHandler && param->useMethod) {                                        \
            gHandler->name = &customMethod;                                        \
        }                                                                          \
                                                                                   \
        int rv = signature(param->socket);                                         \
        if (!(param->handler) || !(param->useMethod)) {                            \
            cr_assert_eq(rv, ENULL_POINTER,                                        \
                    "Expected to get a null pointer error, but didn\'t");          \
        } else {                                                                   \
            cr_assert_eq(rv, ESUCCESS, "Expected to return success, but didn\'t"); \
        }                                                                          \
    }

#endif
