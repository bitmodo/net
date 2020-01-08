#ifndef NET_LINUX
#define NET_LINUX

#include "net/net.h"
#include "net/request.h"
#include "net/response.h"

struct net_req_t {
    int fd;
};

struct net_data_t {
    int fd;
    bool open;

    char const * address;
    unsigned port;

    int current_req_fd;
    struct net_req_t * current_req;
    struct net_res_t * current_res;
};

NET_EXPORT NetSocket NetLinuxSetup();

#endif
