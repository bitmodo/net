#include "test.h"

#include <netdb.h>

int getaddrinfo(const char * name, const char * service, const struct addrinfo * req, struct addrinfo ** pai) {
    *pai = NULL;

    return 0;
}

void freeaddrinfo(struct addrinfo * ai) { }

int main() {
    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {};

    assert(prepareSocket(NULL, sock) == EINVALID_IP && "Did not get the expected return value from error");

    free(sock);
}
