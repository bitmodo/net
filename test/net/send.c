#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

#define SEND_RETURN ESUCCESS

bool send = false;
bool hasSocket = false;

int customSend(Socket * sock, const void * buf, size_t size) {
    send = true;
    hasSocket = sock != NULL;

    return SEND_RETURN;
}

int main() {
    Socket * sock;

    gHandler = NULL;
    sock = NULL;
    assert(netSend(sock, NULL, 0) == ENULL_POINTER && "Send with a null handler and socket did not return a null pointer error");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netSend(sock, NULL, 0) == ENULL_POINTER && "Send with a null socket and function did not return a null pointer error");
    free(gHandler);

    send = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, &customSend, NULL, NULL, NULL};
    sock = NULL;
    assert(netSend(sock, NULL, 0) == SEND_RETURN && "Send did not return the correct return value");
    assert(send && "Send did not set the expected variable");
    assert(!hasSocket && "Has socket was set to true with a null socket");
    free(gHandler);

    send = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, &customSend, NULL, NULL, NULL};
    sock = malloc(sizeof(Socket));
    assert(netSend(sock, NULL, 0) == SEND_RETURN && "Send did not return the correct return value");
    assert(send && "Send did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    free(gHandler);
    free(sock);

    exit(EXIT_SUCCESS);
}
