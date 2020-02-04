#include <net/net.h>
#include <net/error.h>

#include <stdlib.h>
#include <assert.h>

#include "tests.h"

int main() {
    setup();

    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.side = CLIENT, .data = malloc(sizeof(SocketData))};
    assert(netStart(sock) == EINCORRECT_SIDE && "Did not receive the expected value for incorrect side");
    free(sock->data);
    free(sock);

    cleanup();
}
