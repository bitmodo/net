#include "net/net.h"
#include "net/error.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define HOST_SIZE 100
#define BUF_SIZE 4096

int main() {
    netSetup();

    bool run = true;
    while (run) {
        fprintf(stdout, "Enter a URL: ");

        char host[HOST_SIZE];
        unsigned port;
        scanf("%99s %u", host, &port);

        if (strcmp(host, "close") == 0) {
            fprintf(stdout, "Finishing up\n");
            run = false;
            continue;
        }

        Socket * sock = netSocket(CLIENT, TCP);

        netSetAddress(sock, host);
        netSetPort(sock, port);

        fprintf(stdout, "Connecting to %s:%u\n", host, port);
        int ce;
        if ((ce = netConnect(sock)) != ESUCCESS) {
            fprintf(stderr, "Error connecting: %d\n", ce);

            netCleanup();
            exit(EXIT_FAILURE);
        }

        char request[] = "GET / HTTP/1.0\r\n\r\n";
        netSend(sock, request, sizeof(request)/sizeof(char));

        char buf[BUF_SIZE];
        fprintf(stdout, "Received message:\n\n");
        size_t len;
        while (netReceive(sock, buf, BUF_SIZE, &len) == ESUCCESS && len >= BUF_SIZE) {
            fprintf(stdout, "%.*s", (int) len, buf);
        }
        fprintf(stdout, "%.*s\n\n", (int) len, buf);

        netClose(&sock);
    }

    netCleanup();
    exit(EXIT_SUCCESS);
}
