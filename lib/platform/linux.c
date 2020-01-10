#include "net/net.h"
#include "net/error.h"

#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct SocketData {
    int fd;
    int conn;
};

int socketTypeToNetworkType(int type) {
    if (type == UDP) {
        return SOCK_DGRAM;
    } else {
        return SOCK_STREAM;
    }
}

SocketData * initializeLinux(int side, int type) {
    int fd = socket(AF_INET, socketTypeToNetworkType(type), 0);
    if (fd == -1) return NULL;

    SocketData * data = malloc(sizeof(SocketData));
    *data = (SocketData) {fd, -1};

    return data;
}

int prepareAddress(struct sockaddr_in * addr, Socket * sock, bool any) {
    addr->sin_family = AF_INET;
    addr->sin_port = htons(sock->port);

    if (sock->address) {
        if (inet_pton(AF_INET, sock->address, &(addr->sin_addr)) <= 0) return EINVALID_IP;
    } else {
        if (any) {
            addr->sin_addr.s_addr = INADDR_ANY;
        } else {
            return -1;
        }
    }

    return 0;
}

int connectLinux(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1) return ENULL_POINTER;

    struct sockaddr_in addr;
    if (prepareAddress(&addr, sock, false) == -1) return ENULL_POINTER;

    if (connect(sock->data->fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) return EUNKNOWN;

    return ESUCCESS;
}

int startLinux(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1) return ENULL_POINTER;

    struct sockaddr_in addr;
    if (prepareAddress(&addr, sock, false) == -1) return ENULL_POINTER;

    if (bind(sock->data->fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) return EUNKNOWN;

    if (listen(sock->data->fd, 10)) return EUNKNOWN;

    return ESUCCESS;
}

int loopLinux(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1) return ENULL_POINTER;

    int fd;
    if ((fd = accept(sock->data->fd, NULL, 0)) < 0) return EUNKNOWN;

    sock->data->conn = fd;

    return ESUCCESS;
}

int receiveLinux(Socket * sock, void * buf, int count) {
    if (!sock || !(sock->data) || (sock->side == SERVER && sock->data->conn == -1)) return ENULL_POINTER;

    int size;
    if ((size = recv(sock->side == SERVER ? sock->data->conn : sock->data->fd, buf, count, 0)) < 0) return EUNKNOWN;

    return size;
}

int sendLinux(Socket * sock, const void * buf, int count) {
    if (!sock || !(sock->data) || (sock->side == SERVER && sock->data->conn == -1)) return ENULL_POINTER;

    if (send(sock->side == SERVER ? sock->data->conn : sock->data->fd, buf, count, 0) < 0) return EUNKNOWN;

    return ESUCCESS;
}

int closeConnectionLinux(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn == -1) return ENULL_POINTER;

    close(sock->data->conn);
    sock->data->conn = -1;

    return ESUCCESS;
}

int closeLinux(Socket ** sock) {
    if (!sock || !((*sock)->data)) return ENULL_POINTER;

    if ((*sock)->data->fd != -1) close((*sock)->data->fd);

    free((*sock)->data);
    free(*sock);
    *sock = NULL;
    return ESUCCESS;
}

NetHandler * net_linux() {
    NetHandler * handler = malloc(sizeof(NetHandler));
    *handler = (NetHandler) {&initializeLinux, &connectLinux, &startLinux, &loopLinux, &receiveLinux, &sendLinux, &closeConnectionLinux, &closeLinux, NULL};

    return handler;
}
