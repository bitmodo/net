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

int server() {
    net_setup();
    fprintf(stdout, "Starting server\n");

    // Setup the socket
    Socket * sock = net_socket(SERVER, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create server socket\n");
        net_cleanup();
        return EXIT_FAILURE;
    }

    net_setAddress(sock, HOST);
    net_setPort(sock, PORT);
    int ec;
    if ((ec = net_start(sock)) != ESUCCESS) {
        fprintf(stderr, "Failed to start server with code %d\n", ec);
        net_cleanup();
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Server: Waiting for connections on %s:%d\n", net_getAddress(sock), net_getPort(sock));
    int lc;
    while ((lc = net_loop(sock)) == ESUCCESS) {
        fprintf(stdout, "Server: Connected to client. Waiting for message\n");

        bool open = true;
        while(open) {
            char * message = net_receiveText(sock, BUFF_SIZE);
            fprintf(stdout, "Server: Received message:\n\t%s\n", message);

            if (strcmp(message, "close") == 0) {
                fprintf(stdout, "Server: Closing\n");
                net_close(&sock);
                open = false;
            } else {
                fprintf(stdout, "Server: Sending pong\n");
                net_send(sock, message, strlen(message));
            }

            free(message);
        }
    }

    if (lc != ECLOSED) {
        fprintf(stderr, "Failed to run server\n");
        net_cleanup();
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Finishing server\n");
    net_cleanup();
    return EXIT_SUCCESS;
}

void client() {
    net_setup();
    fprintf(stdout, "Starting client\n");

    // Setup the socket
    Socket * sock = net_socket(CLIENT, TCP);
    if (!sock) {
        fprintf(stderr, "Failed to create client socket\n");
        net_cleanup();
        exit(EXIT_FAILURE);
    }

    net_setAddress(sock, HOST);
    net_setPort(sock, PORT);
    fprintf(stdout, "Client: Establishing connection on %s:%d\n", net_getAddress(sock), net_getPort(sock));

    // Connect to the server
    int code;
    if ((code = net_connect(sock)) != ESUCCESS) {
        fprintf(stderr, "Client: Failed to connect with code %d\n", code);
        net_cleanup();
        exit(EXIT_FAILURE);
    }

    char message[BUFF_SIZE * 2] = {0};
    memset(message, 'a', BUFF_SIZE * 2 - 1);
    fprintf(stdout, "Client: Sending message:\n\t%s\n", message);
    net_send(sock, message, strlen(message));

    fprintf(stdout, "Client: Receiving message\n");
    char * received = net_receiveText(sock, BUFF_SIZE);
    fprintf(stdout, "Client: Received:\n\t%s\n", received);
    free(received);

    char close[] = "close";
    fprintf(stdout, "Client: Sending close command\n");
    net_send(sock, close, strlen(close));

    // Close the socket
    net_close(&sock);
    fprintf(stdout, "Finishing client\n");

    net_cleanup();
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
