#include "net/error.h"
#include "net/net.h"
#include "net/server.h"
#include "net/client.h"

#include <stdlib.h>
#include <threads.h>

int server(void * data) {
    Socket * sock = net_tcpServerSocket();

    while (net_loop(sock) == ESUCCESS) {

    }

    return EXIT_SUCCESS;
}

int client(void * data) {
    Socket * sock = net_tcpClientSocket();
    net_connect(sock);

    net_close(sock);
    return EXIT_SUCCESS;
}

int main(int argc, char ** argv) {
    net_setup();

    thrd_t tClient, tServer;
    // Start the server then the client
    thrd_create(&tServer, &server, NULL);
    thrd_create(&tClient, &client, NULL);

    // Join the client when it finishes then the server
    thrd_join(tClient, NULL);
    thrd_join(tServer, NULL);

    net_cleanup();

    return EXIT_SUCCESS;
}
