#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

#define CONNECT_RETURN ESUCCESS

bool connect = false;
bool hasSocket = false;

int customConnect(Socket * sock) {
    connect = true;
    hasSocket = sock != NULL;

    return CONNECT_RETURN;
}

int main() {
    Socket * sock;

    gHandler = NULL;
    sock = NULL;
    assert(netConnect(sock) == ENULL_POINTER && "Connect with a null handler and socket did not return a null pointer error");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netConnect(sock) == ENULL_POINTER && "Connect with a null socket and function did not return a null pointer error");
    free(gHandler);

    connect = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, &customConnect, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netConnect(sock) == CONNECT_RETURN && "Connect did not return the correct return value");
    assert(connect && "Connect did not set the expected variable");
    assert(!hasSocket && "Has socket was set to true with a null socket");
    free(gHandler);

    connect = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, &customConnect, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = malloc(sizeof(Socket));
    assert(netConnect(sock) == CONNECT_RETURN && "Connect did not return the correct return value");
    assert(connect && "Connect did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    free(gHandler);
    free(sock);

    exit(EXIT_SUCCESS);
}
