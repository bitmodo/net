#include <stdlib.h>
#include "net/net.h"

int NetSetup() {
    if (net_global_socket) {
        free(net_global_socket);
        net_global_socket = NULL;
    }

    net_global_socket = NetSetupSocket();
    return net_global_socket ? 0 : -1;
}

void NetSetAddress(char const * address) {
    NetSetAddressSocket(net_global_socket, address);
}

void NetSetPort(unsigned port) {
    NetSetPortSocket(net_global_socket, port);
}

char const * NetGetAddress() {
    return NetGetAddressSocket(net_global_socket);
}

unsigned NetGetPort() {
    return NetGetPortSocket(net_global_socket);
}

int NetOpen() {
    return NetOpenSocket(net_global_socket);
}

bool NetListen() {
    return NetListenSocket(net_global_socket);
}

struct net_req_t const * NetGetRequest() {
    return NetGetRequestSocket(net_global_socket);
}

struct net_res_t * NetGetResponse() {
    return NetGetResponseSocket(net_global_socket);
}

void NetFinalizeResponse() {
    NetFinalizeResponseSocket(net_global_socket);
}

void NetClose() {
    NetCloseSocket(net_global_socket);
}

void NetDispose() {
    NetDisposeSocket(net_global_socket);
}

void NetDisposeRequest(struct net_req_t * req) {
    NetDisposeRequestSocket(net_global_socket, req);
}
