#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

extern NetHandler * gHandler;

#define RECEIVE_TEXT_RETURN NULL

bool receiveText = false;
bool hasSocket = false;

char * customReceiveText(Socket * sock) {
    receiveText = true;
    hasSocket = sock != NULL;

    if (hasSocket) {
        char * result = malloc(sizeof(char)*2);
        result[0] = 'a';
        result[1] = '\0';
        return result;
    }

    return RECEIVE_TEXT_RETURN;
}

int main() {
    Socket * sock;

    gHandler = NULL;
    sock = NULL;
    assert(netReceiveText(sock) == RECEIVE_TEXT_RETURN && "Receive text with a null handler and socket did not return a null pointer");

    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netReceiveText(sock) == RECEIVE_TEXT_RETURN && "Receive text with a null socket and function did not return a null pointer");
    free(gHandler);

    receiveText = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, &customReceiveText, NULL, NULL, NULL, NULL};
    sock = NULL;
    assert(netReceiveText(sock) == RECEIVE_TEXT_RETURN && "Receive text did not return the correct return value");
    assert(receiveText && "Receive text did not set the expected variable");
    assert(!hasSocket && "Has socket was set to true with a null socket");
    free(gHandler);

    receiveText = false;
    hasSocket = false;
    gHandler = malloc(sizeof(NetHandler));
    *gHandler = (NetHandler) {NULL, NULL, NULL, NULL, NULL, &customReceiveText, NULL, NULL, NULL, NULL};
    sock = malloc(sizeof(Socket));
    char * text = netReceiveText(sock);
    assert(strcmp(text, "a") == 0 && "Receive text did not return the correct return value");
    assert(receiveText && "Loop did not set the expected variable");
    assert(hasSocket && "Has socket was set to false with a socket");
    free(gHandler);
    free(sock);
    free(text);

    exit(EXIT_SUCCESS);
}
