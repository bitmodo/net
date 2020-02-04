#include <net/net.h>
#include <net/error.h>

#include <stdlib.h>
#include <assert.h>

#include "tests.h"

int main() {
    setup();

    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.side = SERVER, .data = malloc(sizeof(SocketData))};
    assert(netConnect(sock) == EINCORRECT_SIDE && "Did not receive the expected value for incorrect side");
    free(sock->data);
    free(sock);

    cleanup();
}
