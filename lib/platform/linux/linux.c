#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "linux.h"

void setAddressLinux(struct net_data_t * const data, char const * address) {
    if (data) {
        data->address = address;
    }
}

void setPortLinux(struct net_data_t * const data, unsigned port) {
    if (data) {
        data->port = port;
    }
}

char const * getAddressLinux(struct net_data_t * const data) {
    if (!data) {
        errno = EFAULT;
        return NULL;
    }

    return data->address;
}

unsigned getPortLinux(struct net_data_t * const data) {
    if (!data) {
        errno = EFAULT;
        return 0;
    }

    return data->port;
}

int openLinux(struct net_data_t * const data) {
    if (!data) {
        errno = EFAULT;
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(data->port);

    if (bind(data->fd, (const struct sockaddr *) &addr, sizeof(addr))) {
        return -1;
    }

    if (listen(data->fd, 100)) {
        return -1;
    }

    return 0;
}

bool listenLinux(struct net_data_t * const data) {
    if (!data) {
        errno = EFAULT;
        return false;
    }

    if (data->current_req || data->current_res) {
        errno = EINPROGRESS;
        return false;
    }

    int req_fd = accept(data->fd, NULL, NULL);
    if (req_fd == -1) {
        return false;
    }

    data->current_req = malloc(sizeof(struct net_req_t));
    data->current_res = malloc(sizeof(struct net_res_t));
    *(data->current_req) = (struct net_req_t) {req_fd};
    *(data->current_res) = (struct net_res_t) {0x0};

    return data->open;
}

struct net_req_t const * getRequestLinux(struct net_data_t * const data) {
    if (!data) {
        errno = EFAULT;
        return NULL;
    }

    return data->current_req;
}

struct net_res_t * getResponseLinux(struct net_data_t * const data) {
    if (!data) {
        errno = EFAULT;
        return NULL;
    }

    return data->current_res;
}

void finalizeResponseLinux(struct net_data_t * const data) {
    if (!data) {
        return;
    }

    char const * body = data->current_res->body;
    size_t bodylen = strlen(body);
    if (bodylen > 0) {
        send(data->current_req->fd, body, bodylen, 0);
    }

    if (close(data->current_req->fd) == -1) {
        return;
    }

    NetDisposeRequest(data->current_req);
    free(data->current_res);

    data->current_req = NULL;
    data->current_res = NULL;

    if (!data->open) {
        if (close(data->fd) == -1) {
            return;
        }
    }
}

void closeLinux(struct net_data_t * const data) {
    if (!data) {
        return;
    }

    data->open = false;
}

void disposeDataLinux(struct net_data_t * const data) {
    if (!data) {
        return;
    }

    NetDisposeRequest(data->current_req);
    free(data->current_res);
    data->current_req = NULL;
    data->current_res = NULL;
}

void disposeRequestLinux(struct net_req_t * const req) {
    if (!req) {
        return;
    }

    free(req);
}

ssize_t readRequestLinux(NetRequest req, void * buffer, size_t count) {
    if (!req) {
        errno = EFAULT;
        return -1;
    }

    return read(req->fd, buffer, count);
}

NetSocket NetLinuxSetup() {
    int domain = socket(AF_INET, SOCK_STREAM, 0);
    if (domain == -1) {
        return NULL;
    }

    struct net_data_t * data = malloc(sizeof(struct net_data_t));
    *data = (struct net_data_t) {domain, true, NULL, 0, -1, NULL, NULL};

    struct net_req_handler_t * req_handler = malloc(sizeof(struct net_req_handler_t));
    *req_handler = (struct net_req_handler_t) {&readRequestLinux};

    struct net_socket_t * socket = malloc(sizeof(struct net_socket_t));
    *socket = (struct net_socket_t) {data,
                                     req_handler,
                                     &setAddressLinux,
                                     &setPortLinux,
                                     &getAddressLinux,
                                     &getPortLinux,
                                     &openLinux,
                                     &listenLinux,
                                     &getRequestLinux,
                                     &getResponseLinux,
                                     &finalizeResponseLinux,
                                     &closeLinux,
                                     &disposeDataLinux,
                                     &disposeRequestLinux};
    return socket;
}
