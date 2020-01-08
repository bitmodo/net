#include <errno.h>
#include <include/net/net.h>
#include "net/request.h"

ssize_t NetRequestRead(void * buffer, size_t count) {
    return NetRequestReadSocket(net_global_socket, buffer, count);
}

ssize_t NetRequestReadSocket(struct net_socket_t * socket, void * buffer, size_t count) {
    if (socket && socket->req_handler && socket->req_handler->read) {
        return socket->req_handler->read(NetGetRequestSocket(socket), buffer, count);
    } else {
        errno = EFAULT;
        return -1;
    }
}
