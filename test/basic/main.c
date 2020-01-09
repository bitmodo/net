#include "net/error.h"
#include "net/net.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>

#define HOST "127.0.0.1"
#define PORT 8080

int server(void * data) {
    fprintf(stdout, "Starting server\n");

    Socket * sock = net_socket(SERVER, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create server socket\n");
        return EXIT_FAILURE;
    }

    net_setAddress(sock, HOST);
    net_setPort(sock, PORT);
    net_start(sock);

    while (net_loop(sock) == ESUCCESS) {
        for (int i = 0; i < 5; i++) {
            char buf[1024] = {0};

            int readSize = net_receive(sock, buf, 1024);
            if (readSize == -1) {
                fprintf(stderr, "Server: Error reading from the socket\n");
            } else if (readSize == 0) {
                fprintf(stdout, "Server: Got empty read\n");
            } else {
                fprintf(stdout, "Server: Received message:\n\t%s\n", buf);

                if (strcmp(buf, "close") == 0) {
                    fprintf(stdout, "Server: Closing socket\n");
                    net_close(sock);
                    break;
                }

                net_send(sock, buf, readSize);
            }
        }

        fprintf(stdout, "Server: Closing connection\n");
        net_closeConnection(sock);
    }

    fprintf(stdout, "Finishing server\n");
    return EXIT_SUCCESS;
}

int client(void * data) {
    fprintf(stdout, "Starting client\n");

    Socket * sock = net_socket(CLIENT, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create client socket\n");
        return EXIT_FAILURE;
    }

    net_setAddress(sock, HOST);
    net_setPort(sock, PORT);
    fprintf(stdout, "Client: Establishing connection\n");
    net_connect(sock);

    char * message = "This is a test";
    fprintf(stdout, "Client: Sending: %s\n", message);
    net_send(sock, message, (int) strlen(message));

    char buf[1024] = {0};
    fprintf(stdout, "Client: Receiving\n");
    int receiveSize = net_receive(sock, buf, 1024);
    if(receiveSize == -1) {
        fprintf(stderr, "Client: Error receiving\n");
    } else if (receiveSize == 0) {
        fprintf(stdout, "Client: Empty receive\n");
    } else {
        fprintf(stdout, "Client: Received message:\n\t%s\n", buf);
    }

    char * closeMessage = "close";
    fprintf(stdout, "Client: Sending close command\n");
    net_send(sock, closeMessage, (int) strlen(message));

    net_close(sock);
    fprintf(stdout, "Finishing client\n");
    return EXIT_SUCCESS;
}

int main(int argc, char ** argv) {
    net_setup();

    thrd_t tClient, tServer;
    // Start the server then the client
    thrd_create(&tServer, &server, NULL);
    thrd_sleep(&(struct timespec){.tv_sec=1}, NULL);
    thrd_create(&tClient, &client, NULL);

    // Join the client when it finishes then the server
    thrd_join(tClient, NULL);
    thrd_join(tServer, NULL);

    net_cleanup();

    return EXIT_SUCCESS;
}
