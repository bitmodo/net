#include <net/net.h>
#include <net/error.h>

#include <stdlib.h>
#include <assert.h>

#include "tests.h"

int main() {
    setup();

    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.side = SERVER, .data = malloc(sizeof(SocketData))};
    sock->data->fd = 0;
    assert(netStart(sock) == EIN_USE && "Did not receive the expected value for null data");
    free(sock->data);
    free(sock);

    cleanup();
}
