#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

#define CLOSE_CONNECTION_RETURN ESUCCESS

bool closeConnection = false;
bool hasSocket = false;

int customCloseConnection(Socket * sock) {
    closeConnection = true;
    hasSocket = sock != NULL;

    return CLOSE_CONNECTION_RETURN;
}

int main() {
    Socket * sock;

    gHandler = NULL;
    sock = NULL;
    assert(netCloseConnection(sock) == ENULL_POINTER && "Close connection with a null handler and socket did not return a null pointer error");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netCloseConnection(sock) == ENULL_POINTER && "Close connection with a null socket and function did not return a null pointer error");
    free(gHandler);

    closeConnection = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, &customCloseConnection, NULL, NULL};
    sock = NULL;
    assert(netCloseConnection(sock) == CLOSE_CONNECTION_RETURN && "Close connection did not return the correct return value");
    assert(closeConnection && "Close connection did not set the expected variable");
    assert(!hasSocket && "Has socket was set to true with a null socket");
    free(gHandler);

    closeConnection = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, &customCloseConnection, NULL, NULL};
    sock = malloc(sizeof(Socket));
    assert(netCloseConnection(sock) == CLOSE_CONNECTION_RETURN && "Close connection did not return the correct return value");
    assert(closeConnection && "Close connection did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    free(gHandler);
    free(sock);

    exit(EXIT_SUCCESS);
}
