#include "test.h"

#include <netdb.h>

int getaddrinfo(const char * name, const char * service, const struct addrinfo * req, struct addrinfo ** pai) {
    *pai = malloc(sizeof(struct addrinfo));

    **pai = (struct addrinfo) {.ai_family = 0, .ai_socktype = 0, .ai_next = NULL};

    return 0;
}

void freeaddrinfo(struct addrinfo * ai) {
    if (ai) {
        freeaddrinfo(ai->ai_next);

        if (ai->ai_addr) {
            free(ai->ai_addr);
        }

        free(ai);
    }
}

int socket(int domain, int type, int protocol) {
    return 0;
}

int close(int fd) {
    return -1;
}

int func() {
    return -1;
}

int main() {
    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {};

    assert(prepareSocket(&func, sock) == EUNKNOWN && "Did not get the expected return value from error");

    free(sock);
}
