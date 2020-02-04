#include "../test.h"

#include <netdb.h>

int getaddrinfo(const char * name, const char * service, const struct addrinfo * req, struct addrinfo ** pai) {
    return EAI_BADFLAGS;
}

void freeaddrinfo(struct addrinfo * ai) { }

int main() {
    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {};

    assert(prepareSocket(NULL, sock) == EINVALID_IP && "Did not get the expected return value from bad flags error");

    free(sock);
}
