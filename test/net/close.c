#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

#define CLOSE_RETURN ESUCCESS

bool close = false;
bool hasSocket = false;

int customClose(Socket ** sock) {
    close = true;
    hasSocket = *sock != NULL;

    return CLOSE_RETURN;
}

int main() {
    Socket * sock;

    gHandler = NULL;
    sock = NULL;
    assert(netClose(&sock) == ENULL_POINTER && "Close with a null handler and socket did not return a null pointer error");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netClose(&sock) == ENULL_POINTER && "Close with a null socket and function did not return a null pointer error");
    free(gHandler);

    close = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &customClose, NULL};
    sock = NULL;
    assert(netClose(&sock) == CLOSE_RETURN && "Close did not return the correct return value");
    assert(close && "Close did not set the expected variable");
    assert(!hasSocket && "Has socket was set to true with a null socket");
    free(gHandler);

    close = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &customClose, NULL};
    sock = malloc(sizeof(Socket));
    assert(netClose(&sock) == CLOSE_RETURN && "Close did not return the correct return value");
    assert(close && "Close did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    free(gHandler);
    free(sock);

    exit(EXIT_SUCCESS);
}
