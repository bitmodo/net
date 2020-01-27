#include "net/net.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>
#include <stdbool.h>

#include "handler.h"

extern NetHandler * gHandler;

// Socket functions

#define emptySocket ((Socket) {0, 0, NULL, 0, 0, NULL})

Socket * fillEmptySocket(Socket * sock) {
    *sock = emptySocket;
    return sock;
}

Socket * createSocket() {
    return cr_malloc(sizeof(Socket));
}

Socket * createEmptySocket() {
    return fillEmptySocket(createSocket());
}

// Address get and set tests


