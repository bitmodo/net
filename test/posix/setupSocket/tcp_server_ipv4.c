#include "test.h"

#include <netdb.h>

int getaddrinfo(const char * name, const char * service, const struct addrinfo * req, struct addrinfo ** pai) {
    assert(name == NULL && "Name is not null");
    assert(service == NULL && "Service is not null");

    assert(req != NULL && "Hints is null");
    assert(req->ai_addrlen == 0 && "Hints address length is not zero");
    assert(req->ai_addr == NULL && "Hints address is not null");
    assert(req->ai_canonname == NULL && "Hints cannonical name is not null");
    assert(req->ai_next == NULL && "Hints next is not null");

    assert(req->ai_family == AF_INET && "Did not get expected address family");
    assert(req->ai_socktype == SOCK_STREAM && "Did not get expected socket type");
    assert((req->ai_flags & (AI_PASSIVE | AI_V4MAPPED | AI_ADDRCONFIG)) && "Did not get expected socket flags");
    assert(req->ai_protocol == IPPROTO_TCP && "Did not get expected protocol");

    return EAI_SYSTEM;
}

void freeaddrinfo(struct addrinfo * ai) { }

int main() {
    Socket * sock = malloc(sizeof(Socket));
    *sock = (Socket) {.side = SERVER, .type = TCP, .addressType = IPv4};

    assert(prepareSocket(NULL, sock) == EUNKNOWN && "Did not get the expected return value from error");

    free(sock);
}
