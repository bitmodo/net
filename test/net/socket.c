#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern NetHandler * gHandler;

struct SocketData {
    bool value;
};

bool initialize = false;
bool dataValue = false;

SocketData * customInitialize() {
    SocketData * data = malloc(sizeof(SocketData));
    *data = (SocketData) {dataValue};
    return data;
}

void checkSocket(NetHandler * handler, int side, int type, bool value) {
    if (handler) {
        gHandler = malloc(sizeof(NetHandler));
        *gHandler = *handler;
    } else {
        gHandler = NULL;
    }

    dataValue = value;

    Socket * sock = netSocket(side, type);
    assert(sock && "Returned socket is null");
    assert(sock->side == side && "Socket side is not client");
    assert(sock->type == type && "Socket type is not tcp");
    assert(sock->address == NULL && "Socket address is not null");
    assert(sock->addressType == UNSPEC && "Socket address type is not unspecified");
    assert(sock->port == DEFAULT_PORT && "Socket port is not the default value");

    if (!gHandler || !(gHandler->initialize)) {
        assert(!(sock->data) && "Socket data is not null");
    } else {
        assert(sock->data && "Socket data is null");
        assert(sock->data->value == value && "Socket data value is not the expected value");

        free(sock->data);
    }

    free(sock);
    if (gHandler) {
        free(gHandler);
    }
}

int main() {
    checkSocket(NULL, CLIENT, TCP, false);
    checkSocket(NULL, CLIENT, UDP, false);
    checkSocket(NULL, SERVER, TCP, false);
    checkSocket(NULL, SERVER, UDP, false);

    NetHandler nullHandler = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    checkSocket(&nullHandler, CLIENT, TCP, false);
    checkSocket(&nullHandler, CLIENT, UDP, false);
    checkSocket(&nullHandler, SERVER, TCP, false);
    checkSocket(&nullHandler, SERVER, UDP, false);

    NetHandler handler = {&customInitialize, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    checkSocket(&handler, CLIENT, TCP, false);
    checkSocket(&handler, CLIENT, UDP, false);
    checkSocket(&handler, SERVER, TCP, false);
    checkSocket(&handler, SERVER, UDP, false);

    checkSocket(&handler, CLIENT, TCP, true);
    checkSocket(&handler, CLIENT, UDP, true);
    checkSocket(&handler, SERVER, TCP, true);
    checkSocket(&handler, SERVER, UDP, true);

    exit(EXIT_SUCCESS);
}
