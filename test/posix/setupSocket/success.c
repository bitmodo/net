#include "test.h"

#include <netdb.h>

int getaddrinfo(const char * name, const char * service, const struct addrinfo * req, struct addrinfo ** pai) {
    *pai = malloc(sizeof(struct addrinfo));
    struct addrinfo * first = malloc(sizeof(struct addrinfo));
    struct addrinfo * second = malloc(sizeof(struct addrinfo));
    struct addrinfo * third = malloc(sizeof(struct addrinfo));

    struct sockaddr_in * second_addr = malloc(sizeof(struct sockaddr_in));
    struct sockaddr_in6 * third_addr = malloc(sizeof(struct sockaddr_in6));

    *second_addr = (struct sockaddr_in) {};
    *third_addr = (struct sockaddr_in6) {};

    **pai = (struct addrinfo) {.ai_socktype = 0, .ai_next = first};
    *first = (struct addrinfo) {.ai_family = 0, .ai_socktype = 1, .ai_next = second};
    *second = (struct addrinfo) {.ai_family = AF_INET, .ai_socktype = 2, .ai_addr = second_addr, .ai_next = third};
    *third = (struct addrinfo) {.ai_family = AF_INET6, .ai_socktype = 3, .ai_addr = third_addr, .ai_next = NULL};

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
    return type - 1;
}

int close(int fd) { }

int func(int fd, struct addrinfo * ai) {
    return fd == 2 ? 0 : -1;
}

int main() {
    SocketData * data = malloc(sizeof(SocketData));
    *data = (SocketData) {.fd = -1, .conn = -1};

    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.data = data};

    assert(prepareSocket(&func, sock) == ESUCCESS && "Did not get the expected return value from error");
    assert(sock->data->fd == 2 && "Socket file descriptor was not set to the expected value");

    free(sock);
    free(data);
}
