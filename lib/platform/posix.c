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

SocketData * initializePosix(int side, int type) {
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

int connectPosix(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1) return ENULL_POINTER;
    if (sock->side == SERVER) return EINCORRECT_SIDE;

    int code;
    if ((code = prepareSocket(&connectFunction, sock)) == -1) return code;

    return ESUCCESS;
}

int startFunction(int sfd, struct addrinfo *info) {
    if (bind(sfd, info->ai_addr, info->ai_addrlen) == -1) return -1;

    if (listen(sfd, 10) == -1) return -1;

    return 0;
}

int startPosix(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1) return ENULL_POINTER;
    if (sock->side == CLIENT) return EINCORRECT_SIDE;

    if (prepareSocket(&startFunction, sock) == -1) return ENULL_POINTER;

    return ESUCCESS;
}

int loopPosix(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn != -1 || sock->data->fd == -1) return ECLOSED;
    if (sock->side == CLIENT) return EINCORRECT_SIDE;

    int fd;
    if ((fd = accept(sock->data->fd, NULL, 0)) < 0) return EUNKNOWN;

    sock->data->conn = fd;

    return ESUCCESS;
}

// TODO: Fix the error codes
int receivePosix(Socket * sock, void * buf, int count) {
    if (!sock || !(sock->data) || (sock->side == SERVER && sock->data->conn == -1)) return ENULL_POINTER;

    int size;
    if ((size = recv(sock->side == SERVER ? sock->data->conn : sock->data->fd, buf, count, 0)) < 0) return EUNKNOWN;

    return size;
}

char * receiveTextPosix(Socket * sock, int size) {
    if (!sock || !(sock->data) || (sock->side == SERVER && sock->data->conn == -1)) return NULL;

    size_t bufferSize = sizeof(char) * size;
    char * buffer = malloc(bufferSize);
    memset(buffer, 0, bufferSize);

    char * result = malloc(sizeof(char));
    *result = 0;
    unsigned long resultLen = 0;
    int rs;
    // while ((rs = net_receive(sock, buffer, bufferSize)) > 0) {
    do {
        rs = net_receive(sock, buffer, bufferSize);
        // Create a new buffer that has enough space for both buffers then
        // concatenate the previous ones with the new buffer then set the
        // result to the new buffer
        unsigned recvSize = rs >= bufferSize ? bufferSize : rs;
        // fprintf(stdout, "%lu - %s\n", resultLen, result);
        // fprintf(stdout, "%u - %s\n\n", recvSize, buffer);
        unsigned size = resultLen + recvSize + 1;
        char * tmp = malloc(size);
        memset(tmp, 0, size);
        memcpy(tmp, result, sizeof(char) * resultLen);
        memcpy(tmp + resultLen, buffer, sizeof(char) * recvSize);
        // memset(tmp, 0, newBufferSize);
        // strcat(tmp, result);
        // strcat(tmp, buffer);
        free(result);
        result = tmp;
        resultLen += recvSize;
    } while (rs == bufferSize);

    free(buffer);

    return result;
}

int sendPosix(Socket * sock, const void * buf, int count) {
    if (!sock || !(sock->data) || (sock->side == SERVER && sock->data->conn == -1)) return ENULL_POINTER;

    if (send(sock->side == SERVER ? sock->data->conn : sock->data->fd, buf, count, 0) < 0) return EUNKNOWN;

    return ESUCCESS;
}

int sendTextPosix(Socket * sock, const char * text) {
    return net_send(sock, text, strlen(text));
}

int closeConnectionPosix(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn == -1) return ENULL_POINTER;
    if (sock->side == CLIENT) return EINCORRECT_SIDE;

    close(sock->data->conn);
    sock->data->conn = -1;

    return ESUCCESS;
}

int closePosix(Socket ** sock) {
    if (!sock || !((*sock)->data)) return ENULL_POINTER;

    if ((*sock)->data->fd != -1) close((*sock)->data->fd);

    free((*sock)->data);
    free(*sock);
    *sock = NULL;
    return ESUCCESS;
}

NetHandler * net_setupPlatform() {
    NetHandler * handler = malloc(sizeof(NetHandler));
    *handler = (NetHandler) {&initializePosix, &connectPosix, &startPosix, &loopPosix, &receivePosix, &receiveTextPosix, &sendPosix, &sendTextPosix, &closeConnectionPosix, &closePosix, NULL};

    return handler;
}
