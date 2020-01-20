#include "net/net.h"
#include "net/error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
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
    hints.ai_flags = (sock->side == SERVER ? AI_PASSIVE : 0) | AI_V4MAPPED | AI_ADDRCONFIG; // For wildcard IP address
    hints.ai_protocol = sock->type == TCP ? IPPROTO_TCP : sock->type == UDP ? IPPROTO_UDP : 0; // Any protocol

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
            ((struct sockaddr_in *) rp->ai_addr)->sin_port = htons(sock->port);
        } else if (rp->ai_family == AF_INET6) {
            ((struct sockaddr_in6 *) rp->ai_addr)->sin6_port = htons(sock->port);
        }

        if (sfd < 0)
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

char * receiveTextPosix(Socket * sock) {
    if (!sock || !(sock->data) || (sock->side == SERVER && sock->data->conn == -1)) return NULL;

    struct pollfd pfds[1];
    pfds[0].fd = sock->side == SERVER ? sock->data->conn : sock->data->fd;
    pfds[0].events = POLLIN;

    char * result = malloc(sizeof(char));
    int length = 0;

    while (1) {
        int events = poll(pfds, 1, -1);
        if (events <= 0) {
            return NULL;
        }

        if (!(pfds[0].revents & POLLIN)) {
            return NULL;
        }

        int len = 0;
        ioctl(pfds[0].fd, FIONREAD, &len);
        if (len <= 0) {
            result = realloc(result, length+1);
            result[length] = '\0';
            
            return result;
        }

        int idx = length;
        length += len;
        result = realloc(result, length);
        while (len > 0) {
            int i = recv(pfds[0].fd, result + idx, len, 0);
            if (i < 0) return NULL;
            idx += i;
            len -= i;
        }
    }
}

int sendPosix(Socket * sock, const void * buf, int count) {
    if (!sock || !(sock->data) || (sock->side == SERVER && sock->data->conn == -1)) return ENULL_POINTER;

    int fd = sock->side == SERVER ? sock->data->conn : sock->data->fd;
    while (count > 0) {
        int i = send(fd, buf, count, 0);
        if (i < 1) return EUNKNOWN;
        buf += i;
        count -= i;
    }

    return ESUCCESS;
}

int closeConnectionPosix(Socket * sock) {
    if (!sock || !(sock->data) || sock->data->conn == -1) return ENULL_POINTER;
    if (sock->side == CLIENT) return EINCORRECT_SIDE;

    close(sock->data->conn);
    sock->data->conn = -1;

    return ESUCCESS;
}

int closePosix(Socket ** sock) {
    if (!sock || !(*sock) || !((*sock)->data)) return ENULL_POINTER;

    if ((*sock)->data->fd != -1) close((*sock)->data->fd);

    free((*sock)->data);
    return ESUCCESS;
}

NetHandler * netSetupPlatform() {
    NetHandler * handler = malloc(sizeof(NetHandler));
    *handler = (NetHandler) {&initializePosix, &connectPosix, &startPosix, &loopPosix, &receivePosix, &receiveTextPosix, &sendPosix, &closeConnectionPosix, &closePosix, NULL};

    return handler;
}
