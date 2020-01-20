#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

#define START_RETURN ESUCCESS

bool start = false;
bool hasSocket = false;

int customStart(Socket * sock) {
    start = true;
    hasSocket = sock != NULL;

    return START_RETURN;
}

int main() {
    Socket * sock;

    gHandler = NULL;
    sock = NULL;
    assert(netStart(sock) == ENULL_POINTER && "Start with a null handler and socket did not return a null pointer error");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netStart(sock) == ENULL_POINTER && "Start with a null socket and function did not return a null pointer error");
    free(gHandler);

    start = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, &customStart, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netStart(sock) == START_RETURN && "Start did not return the correct return value");
    assert(start && "Start did not set the expected variable");
    assert(!hasSocket && "Has socket was set to true with a null socket");
    free(gHandler);

    start = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, &customStart, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = malloc(sizeof(Socket));
    assert(netStart(sock) == START_RETURN && "Start did not return the correct return value");
    assert(start && "Start did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    free(gHandler);
    free(sock);

    exit(EXIT_SUCCESS);
}
