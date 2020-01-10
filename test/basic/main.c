#include "net/error.h"
#include "net/net.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define HOST "localhost"
#define PORT 8080

void * server(void * data) {
    fprintf(stdout, "Starting server\n");

    Socket * sock = net_socket(SERVER, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create server socket\n");
        return NULL;
    }

    net_setAddress(sock, HOST);
    net_setPort(sock, PORT);
    if (net_start(sock) != ESUCCESS) {
        fprintf(stderr, "Failed to start server\n");
        return NULL;
    }

    fprintf(stdout, "Server: Waiting for connections on %s:%d\n", net_getAddress(sock), net_getPort(sock));
    int lc;
    while ((lc = net_loop(sock)) == ESUCCESS) {
        fprintf(stdout, "Server: Received a connection. Waiting for data\n");

        for (int i = 0; i < 5; i++) {
            char buf[1024] = {0};

            int readSize = net_receive(sock, buf, sizeof(buf)-1);
            if (readSize == -1) {
                fprintf(stderr, "Server: Error reading from the socket\n");
            } else if (readSize == 0) {
                fprintf(stdout, "Server: Got empty read\n");
            } else {
                fprintf(stdout, "Server: Received message:\n\t%s\n", buf);

                if (strcmp(buf, "close") == 0) {
                    fprintf(stdout, "Server: Closing socket\n");
                    net_close(&sock);
                    break;
                }

                fprintf(stdout, "Server: Sending pong\n");
                net_send(sock, buf, readSize);
            }
        }

        fprintf(stdout, "Server: Closing connection\n");
        net_closeConnection(sock);
    }

    if (lc != ECLOSED) {
        fprintf(stderr, "Failed to run server\n");
        return NULL;
    }

    fprintf(stdout, "Finishing server\n");
    return NULL;
}

void * client(void * data) {
    fprintf(stdout, "Starting client\n");

    Socket * sock = net_socket(CLIENT, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create client socket\n");
        return NULL;
    }

    net_setAddress(sock, HOST);
    net_setPort(sock, PORT);
    fprintf(stdout, "Client: Establishing connection on %s:%d\n", net_getAddress(sock), net_getPort(sock));
    int code;
    if ((code = net_connect(sock)) != ESUCCESS) {
        fprintf(stderr, "Client: Failed to connect with code %d\n", code);
        return NULL;
    }

    char message[] = "This is a test";
    fprintf(stdout, "Client: Sending: %s\n", message);
    net_send(sock, message, (int) sizeof(message));

    char buf[1024] = {0};
    int receiveSize = net_receive(sock, buf, sizeof(buf)-1);
    if(receiveSize == -1) {
        fprintf(stderr, "Client: Error receiving\n");
    } else if (receiveSize == 0) {
        fprintf(stdout, "Client: Empty receive\n");
    } else {
        fprintf(stdout, "Client: Received message:\n\t%s\n", buf);
    }

    char closeMessage[] = "close";
    fprintf(stdout, "Client: Sending close command\n");
    net_send(sock, closeMessage, sizeof(closeMessage));

    net_close(&sock);
    fprintf(stdout, "Finishing client\n");
    return NULL;
}

int main(int argc, char ** argv) {
    net_setup();

    pthread_t tClient, tServer;
    // Start the server then the client
    pthread_create(&tServer, NULL, &server, NULL);
    sleep(1);
    pthread_create(&tClient, NULL, &client, NULL);

    // Join the client when it finishes then the server
    pthread_join(tClient, NULL);
    pthread_join(tServer, NULL);

    net_cleanup();

    return EXIT_SUCCESS;
}
