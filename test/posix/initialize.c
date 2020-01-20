#include "net/net.h"

#include <assert.h>
#include <stdlib.h>

int main() {
    netSetup();

    Socket * sock = netSocket(CLIENT, TCP);
    assert(sock->data && "Socket data is null");

    netCleanup();
    exit(EXIT_SUCCESS);
}
