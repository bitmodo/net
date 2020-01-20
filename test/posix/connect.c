#include "net/net.h"
#include "net/error.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <netdb.h>
#include <sys/socket.h>

struct SocketData {
    int fd;
    int conn;
};

int getaddrinfoReturnCode = 0;
bool getaddrinfoHasAddress = false;

int socketReturnValue = 0;

int connectReturnCode = 0;

int side;
int type;
int addressType;

int getaddrinfo (const char *__restrict name,
                        const char *__restrict service,
                        const struct addrinfo *__restrict req,
                        struct addrinfo **__restrict pai) {
    if (addressType == IPv4) {
        assert(req->ai_family == AF_INET && "IPv4 address type did not map to inet address family");
    } else if (addressType == IPv6) {
        assert(req->ai_family == AF_INET6 && "IPv6 address type did not map to inet6 address family");
    } else {
        assert(req->ai_family == AF_UNSPEC && "Unspeicified address type did not map to unspecified address family");
    }

    if (type == TCP) {
        assert(req->ai_socktype == SOCK_STREAM && "TCP socket type did not map to stream");
        assert(req->ai_protocol == IPPROTO_TCP && "TCP socket type did not map to correct protocol");
    } else {
        assert(req->ai_socktype == SOCK_DGRAM && "UDP socket type did not map to datagram");
        assert(req->ai_protocol == IPPROTO_UDP && "UDP socket type did not map to correct protocol");
    }

    if (side == SERVER) {
        assert(req->ai_flags & (AI_PASSIVE | AI_V4MAPPED | AI_ADDRCONFIG) && "Server side did not get correct flags");
    } else {
        assert(req->ai_flags & (AI_V4MAPPED | AI_ADDRCONFIG) && "Client side did not get correct flags");
    }

    if (getaddrinfoHasAddress) {
        *pai = malloc(sizeof(struct addrinfo));
        memset(*pai, 0, sizeof(struct addrinfo));

        if (addressType == IPv6) {
            (*pai)->ai_family = AF_INET6;
            (*pai)->ai_addr = malloc(sizeof(struct sockaddr_in6));
            memset((*pai)->ai_addr, 0, sizeof(struct sockaddr_in6));
        } else {
            (*pai)->ai_family = AF_INET;
            (*pai)->ai_addr = malloc(sizeof(struct sockaddr_in));
            memset((*pai)->ai_addr, 0, sizeof(struct sockaddr_in));
        }

        if (type == TCP) {
            (*pai)->ai_socktype = SOCK_STREAM;
            (*pai)->ai_protocol = IPPROTO_TCP;
        } else {
            (*pai)->ai_socktype = SOCK_DGRAM;
            (*pai)->ai_protocol = IPPROTO_UDP;
        }

        (*pai)->ai_next = NULL;
    } else {
        *pai = NULL;
    }

    return getaddrinfoReturnCode;
}

void freeaddrinfo(struct addrinfo *ai) {
    if (ai) {
        if (ai->ai_addr) {
            free(ai->ai_addr);
        }

        free(ai);
    }
}

int socket (int domain, int type, int protocol) {
    return socketReturnValue;
}

int connect (int fd, __CONST_SOCKADDR_ARG addr, socklen_t len) {
    if (connectReturnCode != 0) {
        errno = connectReturnCode;
        return -1;
    }
    
    return 0;
}

int close (int fd) {
    return 0;
}

void test(int s, int t, int at, int gair, bool gaih, int srv, int crc, int erc) {
    getaddrinfoReturnCode = gair;
    getaddrinfoHasAddress = gaih;
    socketReturnValue = srv;
    connectReturnCode = crc;
    side = s;
    type = t;
    addressType = at;

    Socket * sock = malloc(sizeof(Socket));
    SocketData * data = malloc(sizeof(SocketData));
    *data = (SocketData) {-1, -1};
    *sock = (Socket) {side, type, NULL, addressType, DEFAULT_PORT, data};

    assert(netConnect(sock) == erc && "Connect did not return the expected return code");

    if (erc == ESUCCESS) {
        assert(sock->data->fd == srv && "Socket file descriptor was not the expected value");
    }

    free(data);
    free(sock);
}

int main() {
    Socket * sock;
    SocketData * data;

    netSetup();

    assert(netConnect(NULL) == ENULL_POINTER && "Connect to null pointer did not return null pointer exception");

    sock = malloc(sizeof(Socket));
    *sock = (Socket) {CLIENT, TCP, NULL, UNSPEC, DEFAULT_PORT, NULL};
    assert(netConnect(sock) == ENULL_POINTER && "Connect with null data did not return null pointer exception");
    free(sock);
    
    sock = malloc(sizeof(Socket));
    data = malloc(sizeof(SocketData));
    *data = (SocketData) {-1, -1};
    *sock = (Socket) {SERVER, TCP, NULL, UNSPEC, DEFAULT_PORT, data};
    assert(netConnect(sock) == EINCORRECT_SIDE && "Connect using server as side did not return incorrect side error");
    free(data);
    free(sock);
    
    sock = malloc(sizeof(Socket));
    data = malloc(sizeof(SocketData));
    *data = (SocketData) {1, 1};
    *sock = (Socket) {CLIENT, TCP, NULL, UNSPEC, DEFAULT_PORT, data};
    assert(netConnect(sock) == EIN_USE && "Connect with already set file descriptors did not return invalid state error");
    free(data);
    free(sock);
    
    test(CLIENT, TCP, UNSPEC, 0, true, 1, 0, ESUCCESS);

    test(CLIENT, TCP, UNSPEC, EAI_BADFLAGS, true, 0, 0, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, EAI_FAIL, true, 0, 0, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, EAI_FAMILY, true, 0, 0, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, EAI_NONAME, true, 0, 0, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, EAI_SOCKTYPE, true, 0, 0, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 1, true, 0, 0, EUNKNOWN);
    
    test(CLIENT, TCP, UNSPEC, 0, true, -1, 0, EINVALID_IP);
    
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EACCES, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EADDRNOTAVAIL, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EAGAIN, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EALREADY, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EBADF, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, ECONNREFUSED, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EFAULT, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EINPROGRESS, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EINTR, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, ENETUNREACH, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, ENOTSOCK, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EPROTOTYPE, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, ETIMEDOUT, EINVALID_IP);
    
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EADDRINUSE, EINVALID_IP);
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EISCONN, EINVALID_IP);
    
    test(CLIENT, TCP, UNSPEC, 0, true, 0, EAFNOSUPPORT, EINVALID_IP);
    
    test(CLIENT, TCP, UNSPEC, 0, true, 0, -1, EINVALID_IP);

    netCleanup();
    exit(EXIT_SUCCESS);
}
