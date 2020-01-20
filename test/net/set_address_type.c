#include "net/net.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Socket * socket;
    
    socket = NULL;
    netSetAddressType(socket, UNSPEC);
    assert(socket == NULL && "Socket was changed when it was null");

    socket = NULL;
    netSetAddressType(socket, UNSPEC);
    assert(socket == NULL && "Socket was changed when it was null but had an address type");

    socket = malloc(sizeof(Socket));
    *socket = (Socket) {0, 0, NULL, 0, 0, NULL};
    netSetAddressType(socket, UNSPEC);
    assert(socket != NULL && "Socket was nulled when setting the address type");
    assert(socket->addressType == UNSPEC && "Socket address type was not unspecified after setting it");
    free(socket);
    
    socket = NULL;
    netSetAddressType(socket, TCP);
    assert(socket == NULL && "Socket was changed when it was null");

    socket = NULL;
    netSetAddressType(socket, TCP);
    assert(socket == NULL && "Socket was changed when it was null but had an address type");

    socket = malloc(sizeof(Socket));
    *socket = (Socket) {0, 0, NULL, 0, 0, NULL};
    netSetAddressType(socket, TCP);
    assert(socket != NULL && "Socket was nulled when setting the address type");
    assert(socket->addressType == TCP && "Socket address type was not tcp after setting it");
    free(socket);
    
    socket = NULL;
    netSetAddressType(socket, UDP);
    assert(socket == NULL && "Socket was changed when it was null");

    socket = NULL;
    netSetAddressType(socket, UDP);
    assert(socket == NULL && "Socket was changed when it was null but had an address type");

    socket = malloc(sizeof(Socket));
    *socket = (Socket) {0, 0, NULL, 0, 0, NULL};
    netSetAddressType(socket, UDP);
    assert(socket != NULL && "Socket was nulled when setting the address type");
    assert(socket->addressType == UDP && "Socket address type was not udp after setting it");
    free(socket);

    exit(EXIT_SUCCESS);
}
