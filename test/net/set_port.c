#include "net/net.h"

#include <assert.h>
#include <stdlib.h>

#define PORT 100

int main() {
    Socket * sock;

    sock = NULL;
    netSetPort(sock, 0);
    assert(sock == NULL && "Null socket was changed after setting port");

    sock = NULL;
    netSetPort(sock, PORT);
    assert(sock == NULL && "Null socket was changed after setting port");

    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, NULL, 0, 0, NULL};
    netSetPort(sock, 0);
    assert(sock != NULL && "Socket was nulled after setting port");
    assert(sock->port == 0 && "Socket port was not set to zero");
    free(sock);

    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, NULL, 0, 0, NULL};
    netSetPort(sock, PORT);
    assert(sock != NULL && "Socket was nulled after setting port");
    assert(sock->port == PORT && "Socket port was not set to correct port");
    free(sock);

    exit(EXIT_SUCCESS);
}
