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

    // Setup the socket
    Socket * sock = netSocket(SERVER, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create server socket\n");
        return NULL;
    }

    netSetAddress(sock, HOST);
    netSetPort(sock, PORT);
    if (netStart(sock) != ESUCCESS) {
        fprintf(stderr, "Failed to start server\n");
        return NULL;
    }

    // Listen for connections
    fprintf(stdout, "Server: Waiting for connections on %s:%d\n", netGetAddress(sock), netGetPort(sock));
    int lc;
    while ((lc = netLoop(sock)) == ESUCCESS) {
        fprintf(stdout, "Server: Received a connection. Waiting for data\n");

        for (int i = 0; i < 5; i++) {
            char buf[1024] = {0};

            // Receive data from the connection
            int readSize = netReceive(sock, buf, sizeof(buf)-1);
            if (readSize == -1) {
                fprintf(stderr, "Server: Error reading from the socket\n");
            } else if (readSize == 0) {
                fprintf(stdout, "Server: Got empty read\n");
            } else {
                fprintf(stdout, "Server: Received message:\n\t%s\n", buf);

                // Close the socket if a "close" command is received
                if (strcmp(buf, "close") == 0) {
                    fprintf(stdout, "Server: Closing socket\n");
                    netClose(&sock);
                    break;
                }

                // Send a "pong" back with the same message
                fprintf(stdout, "Server: Sending pong\n");
                netSend(sock, buf, readSize);
            }
        }

        // Close the currently open connection
        fprintf(stdout, "Server: Closing connection\n");
        netCloseConnection(sock);
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

    // Setup the socket
    Socket * sock = netSocket(CLIENT, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create client socket\n");
        return NULL;
    }

    netSetAddress(sock, HOST);
    netSetPort(sock, PORT);
    fprintf(stdout, "Client: Establishing connection on %s:%d\n", netGetAddress(sock), netGetPort(sock));

    // Connect to the server
    int code;
    if ((code = netConnect(sock)) != ESUCCESS) {
        fprintf(stderr, "Client: Failed to connect with code %d\n", code);
        return NULL;
    }

    // Send a simple message
    char message[] = "This is a test";
    fprintf(stdout, "Client: Sending: %s\n", message);
    netSend(sock, message, (int) sizeof(message));

    // Receive a pong message
    char buf[1024] = {0};
    int receiveSize = netReceive(sock, buf, sizeof(buf)-1);
    if(receiveSize == -1) {
        fprintf(stderr, "Client: Error receiving\n");
    } else if (receiveSize == 0) {
        fprintf(stdout, "Client: Empty receive\n");
    } else {
        fprintf(stdout, "Client: Received message:\n\t%s\n", buf);
    }

    // Send a close message
    char closeMessage[] = "close";
    fprintf(stdout, "Client: Sending close command\n");
    netSend(sock, closeMessage, sizeof(closeMessage));

    // Close the socket
    netClose(&sock);
    fprintf(stdout, "Finishing client\n");
    return NULL;
}

int main(int argc, char ** argv) {
    netSetup();

    pthread_t tClient, tServer;
    // Start the server then the client
    pthread_create(&tServer, NULL, &server, NULL);
    sleep(1);
    pthread_create(&tClient, NULL, &client, NULL);

    // Join the client when it finishes then the server
    pthread_join(tClient, NULL);
    pthread_join(tServer, NULL);

    netCleanup();

    exit(EXIT_SUCCESS);
}
