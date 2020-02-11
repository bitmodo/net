#ifndef NET_POSIX_LOOP_H
#define NET_POSIX_LOOP_H

#include <net/net.h>
#include <net/error.h>

#include <stdlib.h>
#include <assert.h>

extern NetHandler * gHandler;

extern int loopPosix(Socket *);

struct SocketData {
    int fd;
    int conn;
};

void setup() {
    gHandler = malloc(sizeof(NetHandler));
    gHandler->loop = &loopPosix;
}

void cleanup() {
    free(gHandler);
}

#endif
