#include "net/net.h"
#include "net/error.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char ** argv) {
    net_setup();

    bool run = true;
    while (run) {
        fprintf(stdout, "Enter a URL: ");

        char host[100];
        unsigned port;
        scanf("%99s %u", host, &port);

        if (strcmp(host, "close") == 0) {
            fprintf(stdout, "Finishing up\n");
            run = false;
            continue;
        }

        Socket * sock = net_socket(CLIENT, TCP);

        net_setAddress(sock, host);
        net_setPort(sock, port);

        fprintf(stdout, "Connecting to %s:%u\n", host, port);
        int ce;
        if ((ce = net_connect(sock)) != ESUCCESS) {
            fprintf(stderr, "Error connecting: %d\n", ce);

            net_cleanup();
            exit(EXIT_FAILURE);
        }

        char request[] = "GET / HTTP/1.0\r\n\r\n";
        fprintf(stdout, "Request:\n%s\n\n", request);
        net_send(sock, request, strlen(request));

        char * response = net_receiveText(sock, 1024);
        fprintf(stdout, "Received message:\n%s\n\n", response);
        free(response);

        net_close(&sock);
    }

    net_cleanup();
    exit(EXIT_SUCCESS);
}
