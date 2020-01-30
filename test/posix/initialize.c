#include "net/net.h"

#include <criterion/logging.h>
#include <criterion/criterion.h>

#include "stdlib.h"

extern NetHandler * gHandler;

struct SocketData {
    int fd;
    int conn;
};

Test(posix, initialize) {
    cr_log_info("Running posix initialize test\n");

    netSetup();

    Socket * sock = netSocket(0, 0);
    cr_assert_eq(sock->data->fd, -1, "Did not get the expected file descriptor");
    cr_assert_eq(sock->data->conn, -1, "Did not get the expected connection descriptor");

    free(sock->data);
    free(sock);

    netCleanup();
}
