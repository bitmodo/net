#include "../test.h"

#include <errno.h>
#include <netdb.h>

int accept(int fd, struct sockaddr * addr, socklen_t * len) {
    errno = ENOMEM;
    return -1;
}

int main() {
    setup();

    SocketData * data = malloc(sizeof(SocketData));
    *data = (SocketData) {.fd = 0, .conn = -1};

    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.side = SERVER, .data = data};

    assert(netLoop(sock) == EINVALID_STATE && "Did not get expected return value");

    free(sock);
    free(data);

    cleanup();
}
