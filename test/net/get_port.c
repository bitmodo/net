#include "net/net.h"

#include <assert.h>
#include <stdlib.h>

#define PORT 100

int main() {
    Socket * sock;

    sock = NULL;
    assert(netGetPort(sock) == DEFAULT_PORT && "Null socket did not return zero as the port");

    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, NULL, 0, DEFAULT_PORT, NULL};
    assert(netGetPort(sock) == DEFAULT_PORT && "Default port did not return zero");
    free(sock);

    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, NULL, 0, PORT, NULL};
    assert(netGetPort(sock) == PORT && "Get port did not return expected port");
    free(sock);

    exit(EXIT_SUCCESS);
}
