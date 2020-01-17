#include "net/error.h"
#include "net/net.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#define HOST "localhost"
#define PORT 8081
#define BUFF_SIZE 1024

static int server() {
    netSetup();
    fprintf(stdout, "Starting server\n");

    // Setup the socket
    Socket * sock = netSocket(SERVER, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create server socket\n");
        netCleanup();
        return EXIT_FAILURE;
    }

    netSetAddress(sock, HOST);
    netSetPort(sock, PORT);
    int ec;
    if ((ec = netStart(sock)) != ESUCCESS) {
        fprintf(stderr, "Failed to start server with code %d\n", ec);
        netCleanup();
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Server: Waiting for connections on %s:%d\n", netGetAddress(sock), netGetPort(sock));
    int lc;
    while ((lc = netLoop(sock)) == ESUCCESS) {
        bool open = true;
        while(open) {
            fprintf(stdout, "Server: Connected to client. Waiting for message\n");
            char * message = netReceiveText(sock);
            fprintf(stdout, "Server: Received message:\n\t%s\n", message);

            if (strcmp(message, "close") == 0) {
                fprintf(stdout, "Server: Closing\n");
                netClose(&sock);
                open = false;
            } else {
                fprintf(stdout, "Server: Sending pong\n");
                netSend(sock, message, strlen(message));
            }

            free(message);
        }
    }

    if (lc != ECLOSED) {
        fprintf(stderr, "Failed to run server\n");
        netCleanup();
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Finishing server\n");
    netCleanup();
    return EXIT_SUCCESS;
}

static void client() {
    netSetup();
    fprintf(stdout, "Starting client\n");

    // Setup the socket
    Socket * sock = netSocket(CLIENT, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create client socket\n");
        netCleanup();
        exit(EXIT_FAILURE);
    }

    netSetAddress(sock, HOST);
    netSetPort(sock, PORT);
    fprintf(stdout, "Client: Establishing connection on %s:%d\n", netGetAddress(sock), netGetPort(sock));

    // Connect to the server
    int code;
    if ((code = netConnect(sock)) != ESUCCESS) {
        fprintf(stderr, "Client: Failed to connect with code %d\n", code);
        netCleanup();
        exit(EXIT_FAILURE);
    }

    char message[BUFF_SIZE * 2] = "";
    memset(message, 'a', BUFF_SIZE * 2 - 1);
    fprintf(stdout, "Client: Sending message:\n\t%s\n", message);
    if (netSend(sock, message, sizeof message) != ESUCCESS) {
        fprintf(stderr, "Client: Error sending\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Client: Receiving message\n");
    char * received = netReceiveText(sock);
    fprintf(stdout, "Client: Received:\n\t%s\n", received);
    free(received);

    char close[] = "close";
    fprintf(stdout, "Client: Sending close command\n");
    netSend(sock, close, sizeof(close)/sizeof(char) - 1);

    // Close the socket
    netClose(&sock);
    fprintf(stdout, "Finishing client\n");

    netCleanup();
}

int main(int argc, char ** argv) {
    pid_t pid = fork();

    if (pid == -1) {
        // error
        switch (errno) {
            case EAGAIN:
                fprintf(stderr, "There are too many threads\n");
                exit(EXIT_FAILURE);
            case ENOMEM:
                fprintf(stderr, "Insufficient memory space\n");
                exit(EXIT_FAILURE);
            case ENOSYS:
                fprintf(stderr, "Fork is not supported on this platform\n");
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Unknown error\n");
                exit(EXIT_FAILURE);
        }
    } else if (pid == 0) {
        // child
        sleep(1);
        client();
    } else {
        // parent
        int ss = server();

        int status;
        waitpid(pid, &status, 0);

        if (ss != EXIT_SUCCESS)
            return ss;
        
        exit(WEXITSTATUS(status));
    }

    exit(EXIT_SUCCESS);
}
