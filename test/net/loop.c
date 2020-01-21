#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

#define LOOP_RETURN ESUCCESS

bool loop = false;
bool hasSocket = false;

int customLoop(Socket * sock) {
    loop = true;
    hasSocket = sock != NULL;

    return LOOP_RETURN;
}

int main() {
    Socket * sock;

    gHandler = NULL;
    sock = NULL;
    assert(netLoop(sock) == ENULL_POINTER && "Loop with a null handler and socket did not return a null pointer error");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netLoop(sock) == ENULL_POINTER && "Loop with a null socket and function did not return a null pointer error");
    free(gHandler);

    loop = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, &customLoop, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netLoop(sock) == LOOP_RETURN && "Loop did not return the correct return value");
    assert(loop && "Loop did not set the expected variable");
    assert(!hasSocket && "Has socket was set to true with a null socket");
    free(gHandler);

    loop = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, &customLoop, NULL, NULL, NULL, NULL, NULL};
    sock = malloc(sizeof(Socket));
    assert(netLoop(sock) == LOOP_RETURN && "Loop did not return the correct return value");
    assert(loop && "Loop did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    free(gHandler);
    free(sock);

    exit(EXIT_SUCCESS);
}
