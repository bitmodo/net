#include "net/net.h"

#include <assert.h>
#include <stdlib.h>

extern NetHandler * gHandler;

int main() {
    netSetHandler(NULL);
    assert(gHandler == NULL && "The handler was not equal to null when using netSetHandler");

    NetHandler * handler = netSetupPlatform();
    netSetHandler(handler);
    assert(gHandler == handler && "The handler was not equal to a platform specific handler using netSetHandler");

    exit(EXIT_SUCCESS);
}
