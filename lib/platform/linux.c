#include "net/net.h"
#include "net/error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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

int addressTypeToFamilyType(int type) {
    if (type == IPv6) {
        return AF_INET6;
    } else if (type == IPv4) {
        return AF_INET;
    } else {
        return AF_UNSPEC;
    }
}

SocketData * initializeLinux(int side, int type) {
    SocketData * data = malloc(sizeof(SocketData));
    *data = (SocketData) {-1, -1};

    return data;
}

int prepareSocket(int (* function)(int, struct addrinfo *), Socket * sock) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = addressTypeToFamilyType(sock->addressType); // Allow IPv4 or IPv6
    hints.ai_socktype = socketTypeToNetworkType(sock->type);
    hints.ai_flags = sock->side == SERVER ? AI_PASSIVE : 0; // For wildcard IP address
    hints.ai_protocol = 0; // Any protocol

    struct addrinfo * info;
    int err = getaddrinfo(sock->address, NULL, &hints, &info);
    if (err != 0) {
        // fprintf(stderr, "Error: %s\n", gai_strerror(err));
        return err;
    }

    int sfd;
    struct addrinfo * rp;
    for (rp = info; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (rp->ai_family == AF_INET) {
            ((struct sockaddr_in *) rp->ai_addr)->sin_port = sock->port;
        } else if (rp->ai_family == AF_INET6) {
            ((struct sockaddr_in6 *) rp->ai_addr)->sin6_port = sock->port;
        } else {
            fprintf(stderr, "Non IPv4 addresses are not yet supported\n");
            return EUNSUPPORTED;
        }

        if (sfd == -1)
            continue;
        
        if (function(sfd, rp) != -1)
            break;
        
        close(sfd);
    }

    if (rp == NULL) {
        return -1;
    }

    freeaddrinfo(info);

    sock->data->fd = sfd;

    return 0;
}

int connectFunction(int sfd, struct addrinfo *info) {
    return connect(sfd, info->ai_addr, info->ai_addrlen);
}

int connectLinux(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1) return ENULL_POINTER;

    int code;
    if ((code = prepareSocket(&connectFunction, sock)) == -1) return code;

    return ESUCCESS;
}

int startFunction(int sfd, struct addrinfo *info) {
    if (bind(sfd, info->ai_addr, info->ai_addrlen) == -1) return -1;

    if (listen(sfd, 10) == -1) return -1;

    return 0;
}

int startLinux(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1) return ENULL_POINTER;

    if (prepareSocket(&startFunction, sock) == -1) return ENULL_POINTER;

    return ESUCCESS;
}

int loopLinux(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1 || sock->data->fd == -1) return ECLOSED;

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

NetHandler * net_setupPlatform() {
    NetHandler * handler = malloc(sizeof(NetHandler));
    *handler = (NetHandler) {&initializeLinux, &connectLinux, &startLinux, &loopLinux, &receiveLinux, &sendLinux, &closeConnectionLinux, &closeLinux, NULL};

    return handler;
}
