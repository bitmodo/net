#ifndef NET_POSIX_SETUP_SOCKET_H
#define NET_POSIX_SETUP_SOCKET_H

#include <net/net.h>
#include <net/error.h>

#include <stdlib.h>
#include <assert.h>

struct addrinfo;

extern int prepareSocket(int (*)(int, struct addrinfo *), Socket *);

#endif
