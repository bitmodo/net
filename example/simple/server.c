#include "net/error.h"
#include "net/net.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HOST "localhost"
#define PORT 8080
#define BUF_SIZE 4096

int main() {
    netSetup();

    fprintf(stdout, "Setting up\n");
    Socket * sock = netSocket(SERVER, TCP);
    netSetAddress(sock, HOST);
    netSetPort(sock, PORT);

    fprintf(stdout, "Starting\n");
    int se;
    if ((se = netStart(sock)) != ESUCCESS) {
        fprintf(stderr, "Error starting: %d\n", se);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Waiting for a connection\n");
    int lc;
    if ((lc = netLoop(sock)) != ESUCCESS) {
        fprintf(stderr, "Error: %d\n", lc);
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    fprintf(stdout, "Received message:\n");
    size_t len;
    while (netReceive(sock, buf, BUF_SIZE, &len) == ESUCCESS && len >= BUF_SIZE) {
        fprintf(stdout, "%.*s", (int) len, buf);
    }
    fprintf(stdout, "%.*s\n\n", (int) len, buf);

    fprintf(stdout, "Sending response\n");
    char * res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello world!";
    netSend(sock, res, (int) strlen(res));

    fprintf(stdout, "Closing\n");
    netCloseConnection(sock);
    netClose(&sock);

    netCleanup();
    exit(EXIT_SUCCESS);
}
