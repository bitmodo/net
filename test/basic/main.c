#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "net/net.h"
#include "net/request.h"
#include "net/response.h"

#define SETUP_FAILURE(message) fprintf(stderr, message); return EXIT_FAILURE;

int main(int argc, char ** argv) {
    if (NetSetup() == -1) {
        switch (errno) {
            case EACCES:
            SETUP_FAILURE("Permission to create socket was denied\n")
            case EAFNOSUPPORT:
            SETUP_FAILURE("Address family is not supported\n")
            case EINVAL:
            SETUP_FAILURE("Invalid protocol or flags\n")
            case EMFILE:
            SETUP_FAILURE("Too many file descriptors were opened for this process\n")
            case ENFILE:
            SETUP_FAILURE("Too many file descriptors were opened\n")
            case ENOBUFS:
            case ENOMEM:
            SETUP_FAILURE("Insufficient memory\n")
            case EPROTONOSUPPORT:
            SETUP_FAILURE("The protocol is not supported with this domain\n")
            default:
            SETUP_FAILURE("Unknown setup error occurred\n")
        }
    } else {
        fprintf(stdout, "Successfully created a socket\n");
    }

    NetSetAddress("localhost");
    NetSetPort(8080);

    fprintf(stdout, "Listening on %s:%u\n", NetGetAddress(), NetGetPort());

    if (NetOpen() == -1) {
        switch (errno) {
            case EACCES:
            SETUP_FAILURE("The address is protected, and the user is not a superuser\n")
            case EADDRINUSE:
            SETUP_FAILURE("The address is already in use\n")
            case EBADF:
            SETUP_FAILURE("Got a bad file descriptor\n")
            case EINVAL:
            SETUP_FAILURE("The socket was already bound to an address\n")
            case ENOTSOCK:
            SETUP_FAILURE("The file descriptor does not refer to a socket\n")
            case EOPNOTSUPP:
            SETUP_FAILURE("The socket does not support listening\n")
            default:
            SETUP_FAILURE("Unknown open error occurred\n")
        }
    }

    fprintf(stdout, "Opened the socket successfully\n");
    while (NetListen()) {
        fprintf(stdout, "Received connection\n");

        char * buffer = malloc(sizeof(char));
        char * body = malloc(sizeof(char));
        while (NetRequestRead(buffer, 1) && strcmp(buffer, "") != 0) {
            body = strcat(body, buffer);
        }

        fprintf(stdout, "Request body:\n%s\n", body);

        NetResponse response = NetGetResponse();
        NetResponseSetBody(response, body);

        if (strcmp(body, "close") == 0) {
            fprintf(stdout, "Closing the connection\n");

            NetClose();
        }

        NetFinalizeResponse();
    }

    NetDispose();

    return EXIT_SUCCESS;
}
