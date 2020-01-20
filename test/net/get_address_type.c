#include "net/net.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Socket * sock;

    sock = NULL;
    assert(netGetAddressType(sock) == UNSPEC && "Address type of null socket is not unspecified");

    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, NULL, 0, 0, NULL};
    assert(netGetAddressType(sock) == UNSPEC && "Address type of default socket is not unspecified");
    free(sock);
    
    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, NULL, TCP, 0, NULL};
    assert(netGetAddressType(sock) == TCP && "Address type was not equal to tcp");
    free(sock);
    
    sock = malloc(sizeof(Socket));
    *sock = (Socket) {0, 0, NULL, UDP, 0, NULL};
    assert(netGetAddressType(sock) == UDP && "Address type was not equal to udp");
    free(sock);

    exit(EXIT_SUCCESS);
}
