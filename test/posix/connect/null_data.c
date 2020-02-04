#include <net/net.h>
#include <net/error.h>

#include <stdlib.h>
#include <assert.h>

#include "tests.h"

int main() {
    setup();

    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.data = NULL};
    assert(netConnect(sock) == ENULL_POINTER && "Did not receive the expected value for null data");
    free(sock);

    cleanup();
}
