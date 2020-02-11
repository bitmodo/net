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

#ifndef NET_BUFFER_COUNT
#define NET_BUFFER_COUNT 10
#endif

struct SocketData {
    int fd;
    int conn;
};

NET_CONST
NET_NO_THROW
int socketTypeToNetworkType(int type) {
    if (type == UDP) {
        return SOCK_DGRAM;
    }

    return SOCK_STREAM;
}

NET_CONST
NET_NO_THROW
int addressTypeToFamilyType(int type) {
    if (type == IPv4) {
        return AF_INET;
    }

    if (type == IPv6) {
        return AF_INET6;
    }

    return AF_UNSPEC;
}

NET_MALLOC
SocketData * initializePosix() {
    SocketData * data = malloc(sizeof(SocketData));
    *data = (SocketData) {-1, -1};

    return data;
}

NET_NON_NULL(1, 2)
int prepareSocket(NET_NO_ESCAPE int (* function)(int, struct addrinfo *), NET_NO_ESCAPE Socket * NET_RESTRICT sock) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = addressTypeToFamilyType(sock->addressType); // Allow IPv4 or IPv6
    hints.ai_socktype = socketTypeToNetworkType(sock->type); // Use correct socket type
    hints.ai_flags = (sock->side == SERVER ? AI_PASSIVE : 0) | AI_V4MAPPED | AI_ADDRCONFIG; // Set necessary flags
    hints.ai_protocol = sock->type == UDP ? IPPROTO_UDP : IPPROTO_TCP; // Use correct protocol

    struct addrinfo * info;
    int err = getaddrinfo(sock->address, NULL, &hints, &info);
    if (err != 0) {
        freeaddrinfo(info);

        switch (err) {
            case EAI_BADFLAGS:
            case EAI_FAIL:
            case EAI_FAMILY:
            case EAI_NONAME:
            case EAI_SOCKTYPE:
                return EINVALID_IP;
            default: return EUNKNOWN;
        }
    }

    int sfd;
    struct addrinfo * rp;
    for (rp = info; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (sfd < 0) continue;

        if (rp->ai_family == AF_INET) {
            ((struct sockaddr_in *) rp->ai_addr)->sin_port = htons(sock->port);
        } else if (rp->ai_family == AF_INET6) {
            ((struct sockaddr_in6 *) rp->ai_addr)->sin6_port = htons(sock->port);
        }
        
        if (function(sfd, rp) != -1) break;
        
        if (NET_UNLIKELY(close(sfd) == -1)) {
            freeaddrinfo(info);
            return EUNKNOWN;
        }
    }

    if (rp == NULL) {
        freeaddrinfo(info);
        return EINVALID_IP;
    }

    freeaddrinfo(info);

    sock->data->fd = sfd;

    return ESUCCESS;
}

NET_NON_NULL(2)
int connectFunction(int sfd, NET_NO_ESCAPE struct addrinfo * info) {
    return connect(sfd, info->ai_addr, info->ai_addrlen);
}

NET_NON_NULL(1)
int connectPosix(Socket * sock) {
    if (!NET_LIKELY(sock) || !NET_LIKELY(sock->data)) return ENULL_POINTER;
    if (NET_UNLIKELY(sock->side != CLIENT)) return EINCORRECT_SIDE;
    if (NET_UNLIKELY(sock->data->fd != -1)) return EIN_USE;

    return prepareSocket(&connectFunction, sock);
}

NET_NON_NULL(2)
int startFunction(int sfd, NET_NO_ESCAPE struct addrinfo * info) {
    if (bind(sfd, info->ai_addr, info->ai_addrlen) == -1) return -1;

    if (listen(sfd, NET_BUFFER_COUNT) == -1) return -1;

    return 0;
}

NET_NON_NULL(1)
int startPosix(Socket * sock) {
    if (!NET_LIKELY(sock) || !NET_LIKELY(sock->data)) return ENULL_POINTER;
    if (NET_UNLIKELY(sock->side == CLIENT)) return EINCORRECT_SIDE;
    if (NET_UNLIKELY(sock->data->fd != -1)) return EIN_USE;

    return prepareSocket(&startFunction, sock);
}

NET_NON_NULL(1)
int loopPosix(NET_NO_ESCAPE Socket * NET_RESTRICT sock) {
    if (!NET_LIKELY(sock) || !NET_LIKELY(sock->data)) return ECLOSED;
    if (NET_UNLIKELY(sock->side == CLIENT)) return EINCORRECT_SIDE;
    if (NET_UNLIKELY(sock->data->conn != -1) || NET_UNLIKELY(sock->data->fd == -1)) return EINVALID_STATE;

    int fd;
    if (NET_UNLIKELY((fd = accept(sock->data->fd, NULL, 0)) < 0)) {
        switch (errno) {
            case EBADF:
            case ECONNABORTED:
            case EINTR:
            case EINVAL:
            case EMFILE:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM: return EINVALID_STATE;
            default: return EUNKNOWN;
        }
    }

    sock->data->conn = fd;

    return ESUCCESS;
}

NET_NON_NULL(1, 2)
int receivePosix(NET_NO_ESCAPE Socket * NET_RESTRICT sock, void * buf, size_t count, ssize_t * size) {
    if (!NET_LIKELY(sock) || !NET_LIKELY(sock->data)) return ENULL_POINTER;
    if (NET_UNLIKELY(sock->side == SERVER && sock->data->conn == -1) || NET_UNLIKELY(sock->side == CLIENT && sock->data->fd == -1)) return EINVALID_STATE;

    if (NET_UNLIKELY((*size = recv(sock->side == SERVER ? sock->data->conn : sock->data->fd, buf, count, 0)) < 0)) {
        switch (errno) {
            case EFAULT:
            case EINVAL:
            case ENOMEM:
            case ENOTCONN:
            case ENOTSOCK:
            case EBADF: return EINVALID_STATE;
            default: return EUNKNOWN;
        }
    }

    return ESUCCESS;
}

NET_NON_NULL(1, 2)
int sendPosix(NET_NO_ESCAPE Socket * NET_RESTRICT sock, const void * buf, size_t count) {
    if (!NET_LIKELY(sock) || !NET_LIKELY(sock->data)) return ENULL_POINTER;
    if (NET_UNLIKELY(sock->side == SERVER && sock->data->conn == -1) || NET_UNLIKELY(sock->side == CLIENT && sock->data->fd == -1)) return EINVALID_STATE;

    int fd = sock->side == SERVER ? sock->data->conn : sock->data->fd;
    while (count > 0) {
        int i = send(fd, buf, count, 0);
        if (NET_UNLIKELY(i == -1)) {
            switch (errno) {
                case EDESTADDRREQ:
                case EFAULT:
                case EINTR:
                case EISCONN:
                case EMSGSIZE:
                case ENOBUFS:
                case ENOMEM:
                case ENOTCONN:
                case ENOTSOCK:
                case EPIPE:
                case EBADF: return EINVALID_STATE;
                default: return EUNKNOWN;
            }
        }

        buf += i;
        count -= i;
    }

    return ESUCCESS;
}

NET_NON_NULL(1)
int closeConnectionPosix(NET_NO_ESCAPE Socket * NET_RESTRICT sock) {
    if (!NET_LIKELY(sock) || !NET_LIKELY(sock->data)) return ENULL_POINTER;
    if (NET_UNLIKELY(sock->side == CLIENT)) return EINCORRECT_SIDE;
    if (NET_UNLIKELY(sock->data->conn == -1)) return EINVALID_STATE;

    if (NET_UNLIKELY(close(sock->data->conn) == -1)) return EUNKNOWN;
    sock->data->conn = -1;

    return ESUCCESS;
}

NET_NON_NULL(1)
int closePosix(Socket ** sock) {
    if (!NET_LIKELY(sock) || !NET_LIKELY(*sock) || !NET_LIKELY((*sock)->data)) return ENULL_POINTER;

    if (NET_LIKELY((*sock)->data->fd != -1)) {
        if (NET_UNLIKELY(close((*sock)->data->fd) == -1)) return EUNKNOWN;
    }

    free((*sock)->data);
    return ESUCCESS;
}

NetHandler * netSetupPlatform() {
    NetHandler * handler = malloc(sizeof(NetHandler));
    *handler = (NetHandler) {&initializePosix, &connectPosix, &startPosix, &loopPosix, &receivePosix, &sendPosix, &closeConnectionPosix, &closePosix, NULL};

    return handler;
}
