#include "net/net.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Socket * socket;
    
    socket = NULL;
    netSetAddress(socket, NULL);
    assert(socket == NULL && "Socket was changed when it was null");

    socket = NULL;
    netSetAddress(socket, "test");
    assert(socket == NULL && "Socket was changed when it was null but had an actual address");

    socket = malloc(sizeof(Socket));
    *socket = (Socket) {0, 0, NULL, 0, 0, NULL};
    netSetAddress(socket, NULL);
    assert(socket != NULL && "Socket was nulled when setting the address");
    assert(socket->address == NULL && "Socket address was not null after setting it");
    free(socket);

    socket = malloc(sizeof(Socket));
    *socket = (Socket) {0, 0, NULL, 0, 0, NULL};
    netSetAddress(socket, "test");
    assert(socket != NULL && "Socket was nulled when setting the address");
    assert(strcmp(socket->address, "test") == 0 && "Socket address was incorrectly set");
    free(socket);

    exit(EXIT_SUCCESS);
}
