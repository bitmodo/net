#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

extern NetHandler * gHandler;

#define RECEIVE_RETURN ESUCCESS

bool receive = false;
bool hasSocket = false;

int customReceive(Socket * sock, void * buf, int size, int * count) {
    receive = true;
    hasSocket = sock != NULL;

    if (hasSocket && buf) {
        memset(buf, 1, size);
    }

    return RECEIVE_RETURN;
}

int main() {
    Socket * sock;
    void * buf;

    gHandler = NULL;
    sock = NULL;
    buf = NULL;
    assert(netReceive(sock, buf, sizeof(void), NULL) == ENULL_POINTER && "Receive with a null handler and socket did not return a null pointer error");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    buf = NULL;
    assert(netReceive(sock, buf, sizeof(void), NULL) == ENULL_POINTER && "Receive with a null socket and function did not return a null pointer error");
    free(gHandler);

    receive = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, &customReceive, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    buf = NULL;
    assert(netReceive(sock, buf, sizeof(void), NULL) == RECEIVE_RETURN && "Receive did not return the correct return value");
    assert(receive && "Receive did not set the expected variable");
    assert(!hasSocket && "Has socket was set to true with a null socket");
    free(gHandler);

    receive = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, &customReceive, NULL, NULL, NULL, NULL, NULL};
    sock = malloc(sizeof(Socket));
    buf = NULL;
    assert(netReceive(sock, buf, sizeof(void), NULL) == RECEIVE_RETURN && "Receive did not return the correct return value");
    assert(receive && "Receive did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    free(gHandler);
    free(sock);

    void * cmp = malloc(sizeof(int));
    memset(cmp, 1, sizeof(int));
    receive = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, &customReceive, NULL, NULL, NULL, NULL, NULL};
    sock = malloc(sizeof(Socket));
    buf = malloc(sizeof(int));
    assert(netReceive(sock, buf, sizeof(int), NULL) == RECEIVE_RETURN && "Receive did not return the correct return value");
    assert(receive && "Receive did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    assert(memcmp(cmp, buf, sizeof(int)) == 0 && "Buffer was not set to expected value after receive");
    free(gHandler);
    free(sock);
    free(buf);

    exit(EXIT_SUCCESS);
}
