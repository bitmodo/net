#include "net/net.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Socket * sock;

    sock = NULL;
    assert(netGetAddress(sock) == NULL && "Address of null pointer is not null");

    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, NULL, 0, 0, NULL};
    assert(netGetAddress(sock) == NULL && "Address of socket is not null");
    free(sock);
    
    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, "test", 0, 0, NULL};
    assert(strcmp(netGetAddress(sock), "test") == 0 && "Socket address was not the expected value");
    free(sock);

    exit(EXIT_SUCCESS);
}
