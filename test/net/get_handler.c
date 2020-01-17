#include "net/net.h"

#include <assert.h>
#include <stdlib.h>

extern NetHandler * gHandler;

int main() {
    gHandler = NULL;
    assert(netGetHandler() == NULL && "netGetHandler did not return null when set to null");

    NetHandler * handler = netSetupPlatform();
    gHandler = handler;
    assert(netGetHandler() == handler && "netGetHandler did not return a platform specific handler");

    exit(EXIT_SUCCESS);
}
