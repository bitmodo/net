#include "tests.h"

#include <netdb.h>

int getaddrinfo(const char * name, const char * service, const struct addrinfo * req, struct addrinfo ** pai) {
    *pai = malloc(sizeof(struct addrinfo));
    struct addrinfo * next = malloc(sizeof(struct addrinfo));
    struct addrinfo * last = malloc(sizeof(struct addrinfo));

    **pai = (struct addrinfo) {.ai_family = 0, .ai_socktype = 1, .ai_next = next};
    *next = (struct addrinfo) {.ai_family = 0, .ai_socktype = 2, .ai_next = last};
    *last = (struct addrinfo) {.ai_family = 0, .ai_socktype = 3, .ai_next = NULL};
    
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
    return type;
}

int close(int fd) { }

int bind(int fd, const struct sockaddr * addr, socklen_t len) {
    return fd >= 2 ? 0 : -1;
}

int listen(int fd, int n) {
    return fd >= 3 ? 0 : -1;
}

int main() {
    setup();

    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.side = SERVER, .data = malloc(sizeof(SocketData))};
    sock->data->fd = -1;
    assert(netStart(sock) == ESUCCESS && "Did not receive the expected value for success");
    assert(sock->data->fd == 3 && "Socket file descriptor was no the expected value");
    free(sock->data);
    free(sock);

    cleanup();
}
