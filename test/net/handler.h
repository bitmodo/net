#ifndef NET_HANDLER_H
#define NET_HANDLER_H

#include "net/net.h"

#include <criterion/parameterized.h>

#define emptyHandler (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}

NetHandler * fillEmptyHandler(NetHandler * handler) {
    *handler = emptyHandler;
    return handler;
}

NetHandler * createHandler() {
    return cr_malloc(sizeof(NetHandler));
}

NetHandler * createEmptyHandler() {
    return fillEmptyHandler(createHandler());
}

typedef struct HandlerParam {
    NetHandler * handler;
} HandlerParam;

HandlerParam toParam(NetHandler * handler) {
    return (HandlerParam) {.handler = handler};
}

void cleanupHandlerParam(struct criterion_test_params *ctp) {
    for (size_t i = 0; i < ctp->length; ++i) {
        struct HandlerParam *tup = (struct HandlerParam *) ctp->params + i;
        cr_free(tup->handler);
    }

    cr_free(ctp->params);
}

#endif
